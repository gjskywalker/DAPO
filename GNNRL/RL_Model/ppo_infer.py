import ray
import torch
from ray import tune
from gym_env.envs.gym_multienv import HLSMultiEnv
from ray.rllib.algorithms.ppo import PPOConfig

ray.init()

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--feature", "-f")
parser.add_argument("--record", "-r")
parser.add_argument("--bmidx", "-i", default=0)
parser.add_argument("--modelpath", "-m", default="Embedding_model_RGCNConv.pth")
args = parser.parse_args()

env_config = {
    'model_path' : "/home/eeuser/Desktop/GRL-HLS/GNNRL/GNN_Model/models/" + args.modelpath,
    'normalize': False,
    'orig_and_normalize':False,
    'bm_name':'test',
    'bm_idx' : args.bmidx,
    'num_pgms':10,
    'feature_type':args.feature,
    'record_file': args.record}

# Define the configuration with the desired RLlib agent and environment settings
config = (
    PPOConfig()
    .training(
            gamma = 0.9,
            sgd_minibatch_size = 76,
            train_batch_size = 5320,
            num_sgd_iter = 30,
            vf_clip_param = 1e2,
            entropy_coeff = 0,
            model={"fcnet_hiddens":[128, 64, 16]}, #models/catalog.py
        )
    .environment(env=HLSMultiEnv, env_config=env_config)
    .framework("torch")
    .rollouts(
            num_rollout_workers=1,
            num_envs_per_worker=1
        )
    .resources(num_cpus_for_local_worker=20, num_cpus_per_worker=8, num_learner_workers=1)
)

# Load the trained model for inference
trained_agent = config.build()
trained_agent.restore("/home/eeuser/ray_results/PPO_rgcn_random_Training/PPO_HLSMultiEnv_e0ece_00000_0_2024-09-07_15-22-02/checkpoint_000003")

count = 0
env = trained_agent.env_creator(env_config)
obs,info = env.reset()

while count < 20:
    action = trained_agent.compute_single_action(obs, explore=True)
    obs, reward, done, truncated, info = env.step(action)
    
    if done:
        obs,info = env.reset()
        count = count + 1

ray.shutdown()

# python ppo_infer.py --feature pna --modelpath Embedding_model_PNAConv.pth --record cycles_pna_test_0 -i 0