import torch


def _load_checkpoint(path: str):
  try:
    return torch.load(path, weights_only=True)
  except (TypeError, RuntimeError):
    return torch.load(path)


def print_all_model():
  models = {
    "RGCNConv": "models/Embedding_model_RGCNConv.pth",
    "HAN": "models/Embedding_model_HAN.pth",
    "PNAConv": "models/Embedding_model_PNAConv.pth",
    "SAGEConv": "models/Embedding_model_SAGEConv.pth",
    "GATv2Conv": "models/Embedding_model_GATV2.pth",
    "GCNConv": "models/Embedding_model_GCNConv.pth",
  }

  for name, path in models.items():
    checkpoint = _load_checkpoint(path)
    loss = checkpoint.get("loss", "<missing>")
    print(f"Loss for {name} : {loss}")

if __name__ == "__main__":
    print_all_model()
    