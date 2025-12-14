import subprocess

def main():
    import argparse
    parser = argparse.ArgumentParser(
                    usage="python Train_All_Models.py --epochs 200 --patience 3",
                    description='Train All GNN Models')
    parser.add_argument('--epochs', required=True, type=str)
    parser.add_argument('--patience', required=True, type=str)
    
    args = parser.parse_args()
    run_gcn = "python GCN_Pytorch.py --epochs " + args.epochs + " --patience " + args.patience
    gcn_result = subprocess.run(run_gcn, shell=True)
    if gcn_result.returncode == 0:
        run_gat = "python GATV2_Pytorch.py --epochs " + args.epochs + " --patience " + args.patience
        gat_result = subprocess.run(run_gat, shell=True)
        if gat_result.returncode == 0:
            run_pna = "python PNA_Pytorch.py --epochs " + args.epochs + " --patience " + args.patience
            pna_result = subprocess.run(run_pna, shell=True)
            if pna_result.returncode == 0:
                run_sage = "python SAGE_Pytorch.py --epochs " + args.epochs + " --patience " + args.patience
                sage_result = subprocess.run(run_sage, shell=True)
                if sage_result.returncode == 0:
                    run_han = "python HAN_Pytorch.py --epochs " + args.epochs + " --patience " + args.patience
                    han_result = subprocess.run(run_han, shell=True)
                    if han_result.returncode == 0:
                        run_rgcn = "python RGCN_Pytorch.py --epochs " + args.epochs + " --patience " + args.patience
                        rgcn_result = subprocess.run(run_rgcn, shell=True)
                        if rgcn_result.returncode == 0:
                                print("Finish Training All GNN Models!!!")
                        
if __name__ == "__main__":
    main()