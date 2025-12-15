import os
import ray
import torch
from ray import tune
from gym_env.envs.gym_multienv import HLSMultiEnv
from ray.rllib.algorithms.ppo import PPOConfig

ray.init()

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODELS_DIR = os.path.join(BASE_DIR, "GNNModels", "models")
print("Models directory:", MODELS_DIR)

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--feature", "-f")
parser.add_argument("--record", "-r")
parser.add_argument("--bmidx", "-i", default=0)
parser.add_argument("--modelpath", "-m", default="Embedding_model_RGCNConv.pth")
args = parser.parse_args()

env_config = {
    'model_path' : os.path.join(MODELS_DIR, args.modelpath),
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
            lr = 5e-2,
            # lr_schedule = [[0, 5e-2], [5320, 1e-3], [10640, 5e-4], [15960, 1e-4], [21280, 5e-5], [26600, 1e-5]],
            lr_schedule = [[0, 5e-2], [21000, 1e-3], [42000, 5e-4], [63000, 1e-5], [84000, 5e-6]],
            gamma = 0.9,
            # equal to the single-env iteration length
            sgd_minibatch_size=30,
            # equal to the multi-env iteration length
            train_batch_size=2100,
            num_sgd_iter = 30,
            vf_clip_param=1e2,
            entropy_coeff = 0.3,
            entropy_coeff_schedule = [[0, 3e-1], [21000, 1e-1], [42000, 2e-2], [63000, 1e-2], [84000, 0]],
            shuffle_sequences = True,
            model={"fcnet_hiddens":[128, 64, 16]}, #models/catalog.py
        )
        .environment(env=HLSMultiEnv, env_config=env_config, disable_env_checking=True)
        .framework(
            "torch"
        )
        .rollouts(
            num_rollout_workers=1,
            num_envs_per_worker=1
        )
        .resources(num_cpus_for_local_worker=4, num_cpus_per_worker=4, num_learner_workers=1)
    )
# Load the trained model for inference
trained_agent = config.build()
# Before restoring, ensure the correct path to the checkpoint is provided
trained_agent.restore("")

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