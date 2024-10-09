import gymnasium as gym

gym.envs.register(
    id='lighthls-v0',
    entry_point='gym_env.envs:light_HLS_env',
    max_episode_steps=45,
    kwargs= {"env_config" : dict()}
)
