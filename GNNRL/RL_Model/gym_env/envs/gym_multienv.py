import os
import gymnasium as gym
from gymnasium.spaces import Discrete, Box, Tuple 
import shutil
from gym_env.envs import getfeatures
from gym_env.envs import getcycles
from gym_env.envs import light_HLS_env
import sys
import pickle
import ray
import math
import numpy as np

class HLSMultiEnv(gym.Env):
    def __init__(self, env_config):
        self.pass_len = 76
        
        self.feature_len = 32 # GNN output channel
        
        self.bm_name = env_config['bm_name']
        
        self.bm_idx = env_config['bm_idx']
        
        self.num_pgms = env_config['num_pgms']
        
        self.norm_obs = env_config['normalize']

        self.action_space = Discrete(self.pass_len)
        
        self.record_file = env_config["record_file"]
        
        self.feature_type = env_config['feature_type']
        
        self.model_path = env_config['model_path']
        
        if self.feature_type == "act_hist":
            self.observation_space = Box(0, 76, shape=(self.pass_len,), dtype=np.float64)
            if self.norm_obs:
                self.observation_space = Box(0, 1, shape=(self.pass_len,), dtype=np.float64)
        elif self.feature_type == "rgcn":
            self.observation_space = Box(-1000.00, 10000.00, shape=(1,self.feature_len), dtype = np.float64)
            if self.norm_obs:
                self.observation_space = Box(0.00, 1.00, shape=(1, self.feature_len), dtype = np.float64)
        elif self.feature_type == "gcn":
            self.observation_space = Box(-1000.00, 10000.00, shape=(1,self.feature_len), dtype = np.float64)
            if self.norm_obs:
                self.observation_space = Box(0.00, 1.00, shape=(1, self.feature_len), dtype = np.float64)
        elif self.feature_type == "pna":
            self.observation_space = Box(-1000.00, 10000.00, shape=(1,self.feature_len), dtype = np.float64)
            if self.norm_obs:
                self.observation_space = Box(0.00, 1.00, shape=(1, self.feature_len), dtype = np.float64)
        elif self.feature_type == "static_feature":
            self.observation_space = Box(0, 10000.0, shape=(1,56), dtype = np.float64)
        elif self.feature_type == "gnn+act_hist":
            self.observation_space = Box(-1000.00, 10000.00, shape=(1,self.feature_len+self.pass_len), dtype = np.float64)
            if self.norm_obs:
                self.observation_space = Box(0.00, 1.00, shape=(1,self.feature_len+self.pass_len), dtype = np.float64)
            
        self.envs = []
        
        # self.idx = np.random.randint(self.num_pgms)  # idx (int): random number between 0 and the number of programs we have to run.

        self.idx = 0
        
        if self.bm_name == "chstone":
            from gym_env.envs.get_TestBench import get_chstone
            bms = get_chstone(N=self.num_pgms)
            for i, bm in enumerate(bms):
                pgm, path = bm
                env_conf = {}
                env_conf['pgm'] = pgm
                env_conf['pgm_dir'] = path
                env_conf['run_dir'] = 'run_'+pgm.replace(".cc","")
                env_conf['model_path'] = self.model_path
                env_conf['feature_type'] = self.feature_type
                env_conf['record_file'] = self.record_file
                # env_conf['normalize'] = self.norm_obs
                # env_conf['verbose'] = env_config.get('verbose',False)
                # env_conf['orig_and_normalize'] = self.orig_norm_obs
                # env_conf['log_obs_reward']=env_config.get('log_obs_reward',False)
                # env_conf['log_results'] = env_config.get('log_results',False)
                self.envs.append(light_HLS_env(env_conf))

        elif self.bm_name == "random":
            from gym_env.envs.get_TestBench import get_random
            bms = get_random(idx=self.bm_idx, pgm_num=self.num_pgms)
            for i, bm in enumerate(bms):
                pgm, files = bm
                env_conf = {}
                env_conf['pgm'] = pgm
                env_conf['pgm_dir'] = files
                env_conf['run_dir'] = 'run_'+pgm.replace(".cc","")
                env_conf['model_path'] = self.model_path
                env_conf['feature_type'] = self.feature_type
                env_conf['record_file'] = self.record_file
                self.envs.append(light_HLS_env(env_config=env_conf))
        
        elif self.bm_name == "polybench":
            from gym_env.envs.get_TestBench import get_polybench
            bms = get_polybench(N=self.num_pgms)
            for i, bm in enumerate(bms):
                pgm, files = bm
                env_conf = {}
                env_conf['pgm'] = pgm
                env_conf['pgm_dir'] = files
                env_conf['run_dir'] = 'run_'+pgm.replace(".cc","")
                env_conf['model_path'] = self.model_path
                env_conf['feature_type'] = self.feature_type
                env_conf['record_file'] = self.record_file
                self.envs.append(light_HLS_env(env_config=env_conf))
                
        elif self.bm_name == "test":
            from gym_env.envs.get_TestBench import get_test
            bms = get_test(idx=self.bm_idx, pgm_num=self.num_pgms)
            for i, bm in enumerate(bms):
                pgm, files = bm
                env_conf = {}
                env_conf['pgm'] = pgm
                env_conf['pgm_dir'] = files
                env_conf['run_dir'] = 'run_'+pgm.replace(".cc","")
                env_conf['model_path'] = self.model_path
                env_conf['feature_type'] = self.feature_type
                env_conf['record_file'] = self.record_file
                self.envs.append(light_HLS_env(env_config=env_conf))
        else:
            raise
    
    def step(self, action):
        obs, reward, done,  truncated, info = self.envs[self.idx].step(action)
        
        return obs, reward, done, truncated, info
    
    def reset(self, *, seed=None, options=None):
        obs, info = self.envs[self.idx].reset()
        self.idx  = (self.idx + 1) % self.num_pgms
        return obs, info
    
    def render(self):
        self.envs[self.idx].render()        
        

