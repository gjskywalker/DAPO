import pickle

# a = {}
# b = {}
# with open("cycles_random_RGCN_V1_training.pkl", "rb") as f:
#     a = pickle.load(f)

# with open("cycles_random_RGCN_V1_training_append.pkl", "rb") as f:
#     b = pickle.load(f)
#     print(b)
    
# for i in b:
#     a[i]['passes'] = b[i]['passes']
#     a[i]['cycle'] = b[i]['cycle']

with open("cycles_random_RGCN_V1_training_whole.pkl", "rb") as f:
    a = pickle.load(f)
    cout = 0
    for i in a:
        cout += 1
    print(cout)

