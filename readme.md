# DAPO: Design-Aware Learning Framework for Pass Ordering Optimization in High-Level Synthesis

DAPO is an open-source framework designed to generate customized optimization pass sequences for general HLS designs. DAPO leverages a heterogeneous graph with a contrastive learning method to generate expressive program embeddings. This process is implemented in a reinforcement learning framework with the support of an accurate light-weight HLS tool. Together, these components enable the creation of a robust inference model

To run this project. You need to build our QoR estimator and feature extractor first. (LLVM-18.1.0 is required to build these).

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
