import torch
import torch.nn as nn
import torch.nn.functional as F
from torch_geometric.nn import global_mean_pool, GATv2Conv, summary
from torch_geometric.loader import DataLoader
import pickle
import matplotlib.pyplot as plt
from torch.utils.data import Dataset



class GraphDataset(Dataset):
    def __init__(self, graph_data, graph_pair_idx, targets):
        self.graph_data = graph_data
        self.graph_pair_idx = graph_pair_idx
        self.targets = targets
        
    def __len__(self):
        return len(self.graph_pair_idx)

    def __getitem__(self, idx):
        graph_idx1, graph_idx2 = self.graph_pair_idx[idx]
        graph1 = self.graph_data[graph_idx1]
        graph2 = self.graph_data[graph_idx2]
        target = self.targets[idx]
        return graph1, graph2, target

class GCCGraphInfer(nn.Module):
    def __init__(self, in_channels, out_channels, edge_dim):
        super(GCCGraphInfer, self).__init__()
        self.gat1 = GATv2Conv(in_channels, 256, edge_dim=edge_dim)
        self.gat2 = GATv2Conv(256, 128, edge_dim=edge_dim)
        
        self.dense1 = nn.Linear(128, 64)
        self.batch_norm1 = nn.BatchNorm1d(64)
        self.dropout = nn.Dropout(0.1)
        self.leaky_relu = nn.LeakyReLU(0.1)
        self.dense2 = nn.Linear(64, out_channels)
        
    def forward(self, data):
        x, edge_index, edge_attr, batch = data.node_attr.float(), data.edge_index, data.edge_attr.float(), data.batch
        x = F.relu(self.gat1(x, edge_index, edge_attr))
        x = F.relu(self.gat2(x, edge_index, edge_attr))
        x = global_mean_pool(x, batch)
        x = self.dense1(x)
        x = self.batch_norm1(x)
        x = self.dropout(x)
        x = self.leaky_relu(x)
        x = self.dense2(x)
        return x
        

def get_predictions(model, data1, data2):
    model.eval()
    with torch.no_grad():
        out1 = model(data1)
        out2 = model(data2)
    return out1, out2

if __name__ == "__main__":
    import os
    if os.path.exists('losses'):
        pass
    else:
        os.system("mkdir losses")
    if os.path.exists("models"):
        pass
    else:
        os.system("mkdir models")
    import argparse
    parser = argparse.ArgumentParser(
                    usage='python GATV2_Pytorch.py --epochs 200 --patience 3',
                    description='Train GATV2Conv')
    parser.add_argument('--epochs', required=True, type=int)
    parser.add_argument('--patience', required=True, type=int)
    
    args = parser.parse_args()
    with open('GED_Result_trainingO03/Index.pkl', 'rb') as fp:
        graph_pair_idx = pickle.load(fp)
    with open('GED_Result_trainingO03/GED_Result.pkl', 'rb') as fp:
        targets = pickle.load(fp)
    
    fp = open("Trainingsets_O03/graphs_random_dataset_pytorch.pkl", "rb")
    graph_data = pickle.load(fp)
    
    best_loss = 100
    dataset = GraphDataset(graph_data, graph_pair_idx, targets)
    train_loader = DataLoader(dataset, batch_size=32, shuffle=True)
    
    in_channels = 12 
    out_channels = 32
    model = GCCGraphInfer(in_channels, out_channels, edge_dim=3)
    print(summary(model, graph_data[0]))
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-2)
    loss_fn = nn.MSELoss()
    
    scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, 'min', factor=0.5, patience=int(args.patience), min_lr=1e-7)
    
    num_epochs = int(args.epochs)
    train_losses = []
    
    for epoch in range(num_epochs):
        model.train()
        running_loss = 0.0
        for data1, data2, target in train_loader:
            optimizer.zero_grad()
            out1 = model(data1)
            out2 = model(data2)
            L2_Distance = (out1 - out2).pow(2).sum(1).pow(1/2)
            loss = loss_fn(L2_Distance, target / 100)
            loss.backward()
            optimizer.step()
            running_loss += loss.item()
        
        epoch_loss = running_loss / len(train_loader)
        if epoch_loss < best_loss:
            best_loss = epoch_loss
            torch.save({
                'epoch': epoch,
                'model_state_dict': model.state_dict(),
                'optimizer_state_dict': optimizer.state_dict(),
                'loss': best_loss,
                }, 'models/Embedding_model_GATV2.pth')
        train_losses.append(epoch_loss)
        lr_train = scheduler.get_last_lr()    
        print(f'Epoch {epoch+1}/{num_epochs}, Loss: {epoch_loss:.4f}, lr: {lr_train}')
        scheduler.step(epoch_loss)
    
    # Plotting the training loss
    plt.plot(range(1, num_epochs + 1), train_losses)
    plt.xlabel('Epochs')
    plt.ylabel('Loss')
    plt.title('Training Loss')
    plt.savefig('losses/GATV2_URG.png')
    plt.show()
    # loss = 1.0 ~ 1.2