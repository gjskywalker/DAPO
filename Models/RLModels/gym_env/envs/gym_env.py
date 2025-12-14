import os
import gymnasium as gym
from torch_geometric.nn import PNAConv
from gymnasium.spaces import Discrete, Box
import shutil
from gym_env.envs import getfeatures
from gym_env.envs import getcycles
from torch_geometric.utils import degree
import torch
import sys
import pickle
import numpy as np
import copy

# Add the path to the folder containing my_module
sys.path.insert(0, os.path.abspath('/home/eeuser/Desktop/GRL-HLS/'))

# Now you can import the function
from GNNRL.GNN_Model import RGCN_Pytorch, GCN_Pytorch, PNA_Pytorch, HARP_Pytorch

class light_HLS_env(gym.Env):
    def __init__(self, env_config):
        # Initialize your environment here
        self.pass_len = 30 # we have 30 different passes
        self.feature_len = 32 # GNN output channel
        self.max_episode_steps = 30
        self.edge_dim = 3 
        self.in_channels = 12 # Input channel 
        
        self.reset_cycle = 100000000
        self.feature_type = env_config.get("feature_type")
        self.norm_obs = env_config.get("norm_obs", False)
        self.verbose = env_config.get("verbose", True)
        self.record_file = env_config['record_file']
        if self.feature_type == "rgcn" or self.feature_type == "pna" or self.feature_type == "gcn" or self.feature_type == "harp":
            self.model_path = env_config['model_path']
            self.check_point = torch.load(self.model_path) 
        # print(env_config)
        pgm = env_config['pgm']
        pgm_dir = env_config['pgm_dir']
        run_dir = env_config['run_dir']
        
        # TODO:
        # self.action_space = Tuple(Discrete(self.pass_len), Discrete(self.pass_len)), and everytime choose the best one;
        self.action_space = Discrete(self.pass_len)
     
        if self.feature_type == "act_hist":
            self.observation_space = Box(0, self.max_episode_steps, shape=(1, self.pass_len), dtype=np.float64)
            if self.norm_obs:
                self.observation_space = Box(0, 1, shape=(1, self.pass_len), dtype=np.float64)
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
        elif self.feature_type == "ir2vec":
            self.observation_space = Box(-100000.00, 100000.00, shape=(1, 75), dtype = np.float64)
            if self.norm_obs:
                self.observation_space = Box(0.00, 1.00, shape=(1, 75), dtype = np.float64)
        elif self.feature_type == "harp":
            self.observation_space = Box(-1000.00, 10000.00, shape=(1, self.feature_len), dtype = np.float64)
            if self.norm_obs:
                self.observation_space = Box(0.00, 1.00, shape=(1, self.feature_len), dtype = np.float64)
        elif self.feature_type == "none":
            self.observation_space = Box(0, 0, shape=(self.feature_len,), dtype=np.float64)
            if self.norm_obs:
                self.observation_space = Box(0, 1, shape=(self.feature_len,), dtype=np.float64)

        self.pre_cycle = 100000000
        self.prev_obs = None
        self.min_cycle = 100000000
        self.log_obs_reward = False
        self.delete_run_dir = True
        self.init_with_passes = False
        self.log_result = False
        self.act_hist = np.zeros((1,self.pass_len))
        self.valid = True
        self.pre_graphs = []
        self.count = 0
        self.errs_log = ""
        
        # self.delete_run_dir = env_config.get('delete_run_dir', True)
        # self.init_with_passes = env_config.get('init_with_passes', False)
        # self.log_result = env_config.get('log_results', False)
        
        if run_dir:
            self.run_dir = run_dir + '_p' +str(os.getpid())
        else:
            raise Exception("Please assign run_dir!")
        
        if self.log_result:
            self.log_file = open(self.run_dir + ".log", 'w')
            
        cwd = os.getcwd()
        self.run_dir = os.path.join(cwd, self.run_dir)
        if os.path.isdir(self.run_dir):
            shutil.rmtree(self.run_dir, ignore_errors=True)
        if pgm_dir:
            os.mkdir(self.run_dir)
            shutil.copy2(pgm_dir+pgm, self.run_dir)
            # shutil.copytree(pgm_dir, self.run_dir)
        self.pre_passes_str = ""
        self.pre_passes = getcycles.passes2indice(self.pre_passes_str)
        self.passes = []
        self.best_passes = []
        self.pgm = pgm
        self.pgm_dir = pgm_dir
        self.pgm_name = pgm.replace('.cc', '')
        self.bc = self.pgm_name + ".bc"
        
    def __del__(self):
        if self.delete_run_dir:
            if self.log_result:
                self.log_file.close()
        
        if os.path.isdir(self.run_dir):
            shutil.rmtree(self.run_dir)
        return
    
    def print_info(self, mesgcn, end = '\n'):
        sys.stdout.write('\x1b[1;34m' + mesgcn.strip() + '\x1b[0m' + end)
        
    def get_cycles(self, passes):
        cycle, _, _= getcycles.light_hls_getHWCycles(self.pgm_name, self.pgm_dir, passes, self.run_dir)
        return cycle
    
    def get_reward(self):
        cycle, valid, errs_log = getcycles.light_hls_getHWCycles(self.pgm_name, self.pgm_dir, self.passes, self.run_dir)
        
        self.errs_log = errs_log
        self.valid = valid
        if(valid == 0):
            cycle = self.pre_cycle
        if(self.verbose):
            try:
                record_file_name = self.record_file + ".pkl"
                cyc_dict = pickle.load(open(record_file_name,'rb'))
            except:
                cyc_dict = {}
            if self.pgm_name in cyc_dict:
                if cyc_dict[self.pgm_name]['cycle']>cycle:
                    cyc_dict[self.pgm_name]['cycle'] = cycle
                    cyc_dict[self.pgm_name]['passes'] = self.passes
            else:
                cyc_dict[self.pgm_name] = {}
                cyc_dict[self.pgm_name]['cycle'] = cycle
                cyc_dict[self.pgm_name]['passes'] = self.passes
            output = open(record_file_name, 'wb')
            pickle.dump(cyc_dict, output)
            log = open("errs_log.pkl", "wb")
            pickle.dump(self.errs_log, log)
            output.close()
            log.close()
        
        if (cycle < self.min_cycle):
            self.min_cycle = cycle
            self.best_passes = copy.deepcopy(self.passes)
        
        rew = ((self.pre_cycle - cycle) / self.reset_cycle) * 100
        self.pre_cycle = cycle
        return rew, valid
    
    def get_observation(self, get_normalizer=False):
        # feats = getfeatures.light_hls_run_stats(self.pgm_name, self.passes, self.run_dir)
        if self.feature_type == "static_feature":
            embeddings = getfeatures.extractnum_run_stats(self.pgm_name, [], path=self.run_dir)
            if get_normalizer:
                return np.asarray(embeddings).reshape(1,56), sum(embeddings)
            return np.asarray(embeddings).reshape(1,56)
        else:
            graphs = getfeatures.gnn_get_feature(self.pgm_name, self.pre_graphs, self.run_dir)
            self.pre_graphs = graphs
            # deg = torch.tensor(graphs.num_edges / graphs.num_nodes)
            if self.feature_type == "rgcn":
                rgcn = RGCN_Pytorch.GCCGraphInfer(in_channels=self.in_channels, out_channels=self.feature_len, num_relations=3)
                rgcn.load_state_dict(self.check_point['model_state_dict'])
                rgcn.eval()
                with torch.no_grad():
                    output = rgcn(graphs)
                    embeddings = np.asarray(output, dtype=np.float64)
                    normalizer = sum(embeddings)
                    if get_normalizer :
                        return embeddings, normalizer
                    else:
                        return embeddings
            elif self.feature_type == "gcn":
                gcn = GCN_Pytorch.GCCGraphInfer(in_channels=self.in_channels,out_channels=self.feature_len)
                gcn.load_state_dict(self.check_point['model_state_dict'])
                gcn.eval()
                with torch.no_grad():
                    output = gcn(graphs)
                    embeddings = np.asarray(output, dtype=np.float64)
                    normalizer = sum(embeddings)
                    if get_normalizer :
                        return embeddings, normalizer
                    else:
                        return embeddings
            elif self.feature_type == "pna":
                deg = degree(index=graphs.edge_index[1], num_nodes=graphs.num_nodes, dtype=torch.float32)
                pna = PNA_Pytorch.GCCGraphInfer(in_channels=self.in_channels, out_channels=self.feature_len, deg=deg, edge_dim=3)
                pna.load_state_dict(self.check_point['model_state_dict'])
                pna.eval()
                with torch.no_grad():
                    output = pna(graphs)
                    embeddings = np.asarray(output, dtype=np.float64)
                    normalizer = sum(embeddings)
                    if get_normalizer :
                        return embeddings, normalizer
                    else:
                        return embeddings
            elif self.feature_type == "ir2vec":
                embeddings = getfeatures.ir2vec_get_feature(self.pgm_name, self.pre_graphs, self.run_dir)
                normalizer = sum(embeddings)
                if get_normalizer:
                    return embeddings, normalizer
                return embeddings
            
            elif self.feature_type == "harp":
                data = getfeatures.harp_get_feature(self.pgm_name, self.pre_graphs, self.run_dir)
                harp = HARP_Pytorch.GCCGraphInfer(in_channels=145, out_channels=self.feature_len, edge_dim=14)
                harp.load_state_dict(self.check_point['model_state_dict'])
                harp.eval()
                with torch.no_grad():  
                    output = harp(data)
                    embeddings = np.asarray(output, dtype=np.float64)
                    normalizer = sum(embeddings)
                    if get_normalizer:
                        return embeddings, normalizer
                    return embeddings
            
            elif self.feature_type == "none":
                embeddings = np.zeros((1, self.feature_len), dtype=np.float64)
                return embeddings
    
    def reset(self, *, seed=None, options=None, init=None, get_obs=True, get_rew=False, ret=True):
        info = {}
        self.passes = []
        if self.init_with_passes:
            self.passes.append(self.pre_passes)
            
        if init:
            self.passes.extend(init)
        
        self.reset_cycle = self.get_cycles(self.passes)
        self.pre_cycle = self.get_cycles(self.passes)
        if(self.feature_type == "harp"):
            getcycles.harp_mimic(self.pgm_name, self.pgm_dir, self.passes, self.run_dir)
        if(self.verbose):
            self.print_info("program: {} --".format(self.pgm_name)+" reset cycles: {}".format(self.reset_cycle)+" count : {}".format(self.count))
        if ret:
            if get_rew:
                reward, _ = self.get_reward()
            if get_obs:
                if self.feature_type == "rgcn" or self.feature_type == "pna" or self.feature_type == "gcn" or self.feature_type == "harp":
                    # obs = np.zeros((1,self.feature_len))
                    obs, normalizer= self.get_observation(get_normalizer=True)
                    if self.norm_obs:
                        obs = [x/normalizer for x in obs]
                elif self.feature_type == "static_feature":
                    obs, normalizer= self.get_observation(get_normalizer=True)
                    if self.norm_obs:
                        obs = [x/normalizer for x in obs]
                elif self.feature_type == "ir2vec":
                    obs, normalizer= self.get_observation(get_normalizer=True)
                    if self.norm_obs:
                        obs = [x/normalizer for x in obs]
                elif self.feature_type == "act_hist":
                    self.act_hist = np.zeros((1,self.pass_len))
                    obs = self.act_hist
                elif self.feature_type == "gnn+act_hist":
                    self.act_hist = np.zeros((1,self.pass_len))
                    gnn_feature = self.get_observation(get_normalizer=False)
                    obs = np.concatenate((gnn_feature, self.act_hist), axis=1)
                elif self.feature_type == "none":
                    obs = np.zeros((1, self.feature_len), dtype=np.float64)
                    if self.norm_obs:
                        obs = np.zeros((1, self.feature_len), dtype=np.float64)
    
                if self.log_result:
                    self.prev_obs = obs
            if get_rew and not get_obs:
                return reward, info
            if get_obs and not get_rew:
                return obs, info
            if get_obs and get_rew:
                return (obs, reward), info
        else:
            return 0, info
    
    def step(self, action, get_obs=True):
        info = {}
        self.passes.append(action)
        reward, _ = self.get_reward()
        
        if(self.feature_type == "harp"):
            getcycles.harp_mimic(self.pgm_name, self.pgm_dir, self.passes, self.run_dir)
        
    
        if self.feature_type == "rgcn" or self.feature_type == "pna" or self.feature_type == "gcn" or self.feature_type == "static_feature" or self.feature_type == "ir2vec" or self.feature_type == "harp":    
            if self.norm_obs:
                obs, normalizer = self.get_observation(get_normalizer=True)
                obs = [x/normalizer for x in obs]
            else:
                obs = self.get_observation(get_normalizer=False)
                
        elif self.feature_type == "act_hist":
            self.act_hist[0][action] += 1
            obs = self.act_hist
            
        elif self.feature_type == "gnn+act_hist":
            gnn_feature = self.get_observation(get_normalizer=False)
            self.act_hist[0][action] += 1
            obs = np.concatenate((gnn_feature,self.act_hist), axis=1)
            
        elif self.feature_type == "none":
            obs = np.zeros((1, self.feature_len), dtype=np.float64)
            if self.norm_obs:
                obs = np.zeros((1, self.feature_len), dtype=np.float64)
       
        if self.log_result:
            self.log_file.write("{}|{}|{}|{}|{}|{}|{}\n".format(self.prev_obs, action, reward, self.prev_cycles, self.min_cycles, self.passes, self.best_passes))
            self.log_file.flush()

        self.prev_obs = obs
        
        truncated = False
        
        done = False
        
        self.count = self.count + 1
        if self.count == self.max_episode_steps :
            self.count = 0
            done = True
        if self.valid == 0:
            truncated = True
        
        if(self.verbose):
            self.print_info("program: {} ".format(self.pgm_name) + "current_cycles: {}".format(self.pre_cycle) + " --passes: {}".format(self.passes) + " reward: {}".format(reward))
            self.print_info("Observation: {}".format(str(obs).replace("\n", " ")))
            self.print_info("min_cycles: {} -- best_passes: {}".format(self.min_cycle, self.best_passes))
            
        return (obs, reward, done,  truncated, info)
    
    def render(self):
        print("pass: {}".format(self.passes))
        print("prev_cycles: {}".format(self.pre_cycle))
        