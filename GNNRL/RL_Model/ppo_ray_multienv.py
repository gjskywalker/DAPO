import ray
from ray import tune, train
from ray.rllib.algorithms.ppo import PPOConfig
from gym_env.envs.gym_multienv import HLSMultiEnv
from gym_env.envs.get_TestBench import get_chstone, get_polybench, get_random

ray.init()

class CustomStopper(tune.Stopper):
    def __init__(self):
        self.should_stop = False
        self.prev_reward = None

    def __call__(self, trial_id, result):
        current_reward = result['episode_reward_mean']
        current_iteration = result['training_iteration']
        if current_iteration == 10 :
        # if self.prev_reward is not None and current_reward < self.prev_reward or current_iteration == 10 or current_reward > 100:
            self.should_stop = True
        self.prev_reward = current_reward
        return self.should_stop

    def stop_all(self): 
        return self.should_stop

stopper = CustomStopper()

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
    'bm_name':'random',
    'bm_idx' : args.bmidx,
    'num_pgms':70,
    'feature_type':args.feature,
    'record_file': args.record}

config = (
        PPOConfig()
        .training(
            lr = 5e-2,
            lr_schedule = [[0, 5e-2], [5320, 1e-3], [10640, 5e-4], [15960, 1e-4], [21280, 5e-5], [26600, 1e-5]],
            gamma = 0.9,
            sgd_minibatch_size=76,
            train_batch_size=5320,
            num_sgd_iter = 30,
            vf_clip_param=1e2,
            entropy_coeff = 0.3,
            entropy_coeff_schedule = [[0, 3e-1], [5320, 1e-1], [10640, 5e-2], [15960, 2e-2]],
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
        .resources(num_cpus_for_local_worker=20, num_cpus_per_worker=8, num_learner_workers=1)
    )
    
tuner = tune.Tuner(
    "PPO",
    run_config=train.RunConfig(
        name="PPO_"+args.feature+"_random_Training",
        checkpoint_config=train.CheckpointConfig(checkpoint_at_end=True, checkpoint_frequency=10),
        stop={"training_iteration":100},
    ),
    param_space=config,
)

tuner.fit()

ray.shutdown()