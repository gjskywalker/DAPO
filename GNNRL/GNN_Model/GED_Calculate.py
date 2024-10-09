import pickle
import networkx as nx
import numpy as np
from multiprocessing import Process, Manager

def node_substitute(node1, node2):
    if (np.array_equal(node1['nodeattr'],node2['nodeattr'])):
        return 0
    elif (node1['nodeattr'][10] == 1 and node2['nodeattr'][11] == 0 and node2['nodeattr'][10] == 0):
        return 2
    elif (node2['nodeattr'][10] == 1 and node1['nodeattr'][11] == 0 and node2['nodeattr'][10] == 0):
        return 2
    else:
        return 1
    
def edge_substitute(edge1, edge2):
    if (np.array_equal(edge1['edgeattr'],edge2['edgeattr'])):
        return 0
    elif (edge1['edgeattr'][2] == 1 or edge2['edgeattr'][2] == 1):
        return 1
    else:
        return 2

def compute_distance(left, right, graphs, node_substitute, edge_substitute, return_dict):
    distance = nx.graph_edit_distance(graphs[left], graphs[right], node_subst_cost=node_substitute, edge_subst_cost=edge_substitute, timeout=300)
    if distance:
        return_dict[(left, right)] = distance

def main():
    import os
    if os.path.exists('GED_Result_trainingO03'):
        pass
    else:
        os.system("mkdir GED_Result_trainingO03")
    fp = open("Trainingsets_O03/graphs_random_dataset_networkx.pkl", "rb")
    graphs = pickle.load(fp)
    print(len(graphs))
    graph_idx = np.random.RandomState(0).randint(len(graphs), size=(14400, 2))
    targets = []
    index = []

    for i in range(0, 720):
        processes = []
        manager = Manager()
        return_dict = manager.dict()

        for left, right in graph_idx[i*30 : (i+1)*30]:
            p = Process(target=compute_distance, args=(left, right, graphs, node_substitute, edge_substitute, return_dict))
            p.start()
            processes.append(p)

        for p in processes:
            p.join()

            
        for key, distance in return_dict.items():
            index.append((key[0], key[1]))
            targets.append(distance)
        
    with open("GED_Result_trainingO03/GED_Result.txt", "w") as result:
        for (left, right), distance in zip(index, targets):
            print(f"{left}, {right} : {distance}", file=result)

    with open("GED_Result_trainingO03/GED_Result.pkl", "wb") as fp:
        pickle.dump(targets, fp)

    with open("GED_Result_trainingO03/Index.pkl", "wb") as ind:
        pickle.dump(index, ind)

if __name__ == "__main__":
    main()
