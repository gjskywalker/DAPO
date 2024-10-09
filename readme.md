# DAPO:  Design-Aware pass-ordering in High-level Synthesis with Graph Neural Network and Deep Reinforcement Learning

DAPO aims to use reinforcement learning network to find the best pass sequences for each program, which can overcome the optimal redundancy and unstable optimization results caused by O3.

We utilize a feature_extractor to construct a graph representation derived from the program’s Control Data Flow Graph (CDFG). This graph is then input into a pre-trained Graph Neural Network (GNN) to generate program embeddings. The resulting feature vector serves as the observation space for Reinforcement Learning (RL) algorithms. Ultimately, the Reinforcement Learning algorithm generates a sequence of passes that optimizes the input program, aiming to achieve the lowest possible latency.

To run this project. You need to build our QoR estimator and feature extractor first. (LLVM-18 is required to build these).

```shell
# Build QoR estimator
cd Tools/Tests/AccelerationCycle/
mkdir build
cd build
cmake ../
make 
# Build FeatureExtractor
cd Tools/Tests/FeatureExtractor/
mkdir build
cd build
cmake ../
make 
```

For GNN and RL model. You can go into folder ```GNNRL```.

```shell
pip install -r requirements.txt
# GNN
cd GNNRL/GNN_Model
# Use the following two prorgams to generate Dataset.
python Read_Dataset.py
python Read_files.py
# Run UGR.py to train GNN and Infer.py to do inference.
python UGR.py
python Infer.py

# RL (Before running it, remember to change the default path in getcycles.py, getfeatures.py and get_TestBench.py)
cd GNNRL/RL_Model
# Do single environment rl training
python ppo_ray.py 
# Do multi environment rl training
python ppo_ray_multienv.py
# Do inference
python ppo_infer.py
```
