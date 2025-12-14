import numpy as np
import matplotlib.pyplot as plt

# 设置随机种子以确保可重复性
np.random.seed(42)

# 模拟对数奖励曲线的函数
def generate_log_reward_curve(epochs, start_reward, end_reward_range, noise_level):
    # 生成一个从 1 到 epochs 的对数趋势
    x = np.linspace(1, epochs, epochs)
    log_trend = np.log(x)
    # 归一化到 start_reward 和 end_reward_range 之间
    log_trend = (log_trend - log_trend.min()) / (log_trend.max() - log_trend.min())
    end_reward = np.random.uniform(*end_reward_range)
    log_trend = log_trend * (end_reward - start_reward) + start_reward
    # 添加更大的随机噪声
    noise = np.random.normal(0, noise_level, epochs)
    # 生成最终的奖励曲线
    reward_curve = log_trend + noise
    return reward_curve

# 参数设置
epochs = 10000
noise_level = 0.5  # 增大噪声水平

# 各曲线的起始值和收敛区间
curve_params = {
    "rgcn": {"start_reward": 5, "end_reward_range": (30, 31)},
    "gcn": {"start_reward": 4, "end_reward_range": (26, 27)},
    "pna": {"start_reward": 5, "end_reward_range": (30, 31)},
    "static": {"start_reward": 3, "end_reward_range": (25, 26)}
}

# 生成奖励曲线
curves = {name: generate_log_reward_curve(epochs, **params, noise_level=noise_level) 
          for name, params in curve_params.items()}

# 绘制曲线
plt.figure(figsize=(12, 8))
for name, curve in curves.items():
    # 选择性地绘制每隔 20 个点
    plt.plot(curve[::20], label=f'{name} (end_reward={curve[-1]:.2f})')

plt.title('Simulated Logarithmic Reward Curves with Different Convergence')
plt.xlabel('Epochs')
plt.ylabel('Reward')
plt.legend()
plt.grid(True)
plt.savefig("reward_curve.pdf", dpi=800)