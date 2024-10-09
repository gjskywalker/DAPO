import ray
from ray import tune, train
from ray.rllib.algorithms.ppo import PPOConfig
from gym_env.envs.gym_env import light_HLS_env
from gym_env.envs.get_TestBench import get_chstone, get_polybench, get_random, get_testset
import argparse
env_config = {}
ray.init()

class CustomStopper(tune.Stopper):
    def __init__(self):
        self.should_stop = False
        self.prev_reward = None

    def __call__(self, trial_id, result):
        current_reward = result['episode_reward_mean']
        current_iteration = result['training_iteration']
        if current_iteration == 10 :
            self.should_stop = True
        self.prev_reward = current_reward
        return self.should_stop

    def stop_all(self):
        return self.should_stop

stopper = CustomStopper()

trainsets = [
    ("test0.cc","/home/eeuser/Desktop/GRL-HLS/Dataset/testsset/test0/"),
    ("test1.cc","/home/eeuser/Desktop/GRL-HLS/Dataset/testsset/test1/"),
    ("test2.cc","/home/eeuser/Desktop/GRL-HLS/Dataset/testsset/test2/")
]

parser = argparse.ArgumentParser()
parser.add_argument("--feature", "-f")
parser.add_argument("--record", "-r")
parser.add_argument("--modelpath", "-m")
args = parser.parse_args()
for i, bm in enumerate(trainsets):
    pgm, path = bm
    env_config["pgm"] = pgm
    env_config["pgm_dir"] = path
    env_config["run_dir"] = 'run_random' + str(i)
    env_config["feature_type"] = args.feature
    env_config["record_file"] = args.record
    env_config['model_path'] = "/home/eeuser/Desktop/GRL-HLS/GNNRL/GNN_Model/models/" + args.modelpath
    config = (
        PPOConfig()
        .training(
            lr = 5e-2,
            lr_schedule = [[0, 5e-2], [608, 1e-3], [1216, 5e-4], [1824, 1e-4], [2432, 5e-5], [3040, 1e-5]],
            gamma = 0.9,
            sgd_minibatch_size = 152,
            train_batch_size = 304,
            num_sgd_iter = 30,
            vf_clip_param=1e2,
            entropy_coeff = 0.3,
            entropy_coeff_schedule = [[0, 3e-1], [608, 1e-1], [1216, 5e-2], [1824, 2e-2], [2432, 0]],
            shuffle_sequences = True,
            model={"fcnet_hiddens":[128, 64, 16]}, #models/catalog.py
        )
        .environment(env=light_HLS_env, env_config=env_config, disable_env_checking=True)
        .framework(
            "torch"
        )
        .rollouts(
            num_rollout_workers=1,
            num_envs_per_worker=1
        )
        .resources(num_cpus_for_local_worker=18, num_cpus_per_worker=10, num_learner_workers=1)
    )

    tuner = tune.Tuner(
        "PPO",
        run_config=train.RunConfig(
            name="PPO_test_RGCN_V3_Training",
            checkpoint_config=train.CheckpointConfig(checkpoint_at_end=True, checkpoint_frequency=50),
            stop={"training_iteration":100},
        ),
        param_space=config,
    )

    tuner.fit()

ray.shutdown()