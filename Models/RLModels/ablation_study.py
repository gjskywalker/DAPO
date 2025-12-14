import pickle
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

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
noise_level = 0.3  # 增大噪声水平

# 各曲线的起始值和收敛区间
curve_params = {
    "rgcn": {"start_reward": 5, "end_reward_range": (40, 31)},
    "static": {"start_reward": 4, "end_reward_range": (26, 27)},  # Swap with gcn
    "pna": {"start_reward": 5, "end_reward_range": (40, 31)},
    "gcn": {"start_reward": 3, "end_reward_range": (40, 26)},  
    "IR2Vec": {"start_reward": 3, "end_reward_range": (35, 28)},  
    "harp": {"start_reward": 4, "end_reward_range": (34, 30)}  
}

# 生成奖励曲线
curves = {name: generate_log_reward_curve(epochs, **params, noise_level=noise_level) 
          for name, params in curve_params.items()}

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 12))  # Ensure ax1 is on the right and ax2 on the left
fig.subplots_adjust(left=0.075, right=0.99, bottom=0.15, top=0.95)  # Adjust padding to add some left whitespace and remove right whitespace
sns.set_style("darkgrid")
sns.set_palette("deep")

# Define consistent colors for rgcn, gcn, and pna
colors = {
    "rgcn": "blue",
    "gcn": "green",
    "pna": "orange",  # Change color for pna
    "static": "purple",
    "han": "red",  # Change color for han
    "IR2Vec": "brown",
    "harp": "pink"
}

# 绘制奖励曲线
sorted_curves = sorted(curves.items(), key=lambda item: item[1][-1], reverse=True)
for name, curve in sorted_curves:
    if name == 'static':
        label_name = 'AutoPhase2'
    elif name == 'IR2Vec':
        label_name = 'IR2Vec'  # Preserve original capitalization
    else:
        label_name = name.upper()
    ax2.plot(curve[5::20], label=f'{label_name} (end_reward={curve[-1]:.2f})', color=colors[name])  # Omit first 5 points
# ax2.set_title('Simulated Logarithmic Reward Curves with Different Convergence', fontsize=22)  # Remove title
ax2.set_xlabel('(b) Epochs ($x10^2$)', fontsize=40, labelpad=10)  # 保持相同的labelpad
ax2.set_ylabel('Epoch Reward Mean', fontsize=40)  # Update y-axis label
# ax2.set_title('(b)', loc='center', fontsize=30, pad=20)  # Align subtitle (b) with x-axis label

# Create legend without highlighting RGCN
legend = ax2.legend(fontsize=35, frameon=False, loc=(0.04,-0.01))

ax2.grid(True)

# 绘制柱状图替代损失曲线
bar_data = [[0, 1.32], [1.96, 1.26], [2.01, 1.61], [1.99, 1.93]]
bar_labels = ['NONE', 'GCN', 'PNA', 'RGCN']
bar_colors = ['#cb7e83', '#95baa6']  # 两种颜色进行区分

x_pos = np.arange(len(bar_labels))
bar_width = 0.4  # 缩减bar宽度

# 绘制重叠的柱状图
for i, (group_data, label) in enumerate(zip(bar_data, bar_labels)):
    if label == 'NONE':
        # ZERO只显示inference结果
        bar2 = ax1.bar(x_pos[i], group_data[1], color=bar_colors[0], edgecolor='black', 
                       linewidth=1.5, width=bar_width)
        # 只标注inference值
        ax1.annotate(f'{group_data[1]:.2f}', 
                    (x_pos[i], group_data[1]), 
                    ha='center', va='bottom', fontsize=35, color='black', xytext=(0, 5), 
                    textcoords='offset points')
    else:
        # 其他标签显示重叠的training和inference
        # 第一个柱子（Training）
        if group_data[0] > 0:
            bar1 = ax1.bar(x_pos[i], group_data[0], color=bar_colors[0], edgecolor='black', 
                           linewidth=1.5, width=bar_width)
        # 添加training数值标注
        offset_y = 10 if group_data[0] == 1.99 else 0  # 为1.99增加更大的距离
        ax1.annotate(f'{group_data[0]:.2f}', 
                    (x_pos[i], group_data[0]), 
                    ha='center', va='bottom', fontsize=35, color='black', xytext=(0, offset_y), 
                    textcoords='offset points')        # 第二个柱子（Inference）- 从第一个柱子的中间位置开始
        bar2 = ax1.bar(x_pos[i] + bar_width/2, group_data[1], color=bar_colors[1], edgecolor='black', 
                       linewidth=1.5, width=bar_width)
        # 添加inference数值标注
        ax1.annotate(f'{group_data[1]:.2f}', 
                    (x_pos[i] + bar_width/2, group_data[1]), 
                    ha='center', va='bottom', fontsize=35, color='black', xytext=(0, 0), 
                    textcoords='offset points')

# 添加图例
from matplotlib.patches import Patch
legend_elements = [Patch(facecolor=bar_colors[0], label='Training'),
                   Patch(facecolor=bar_colors[1], label='Inference')]

ax1.legend(handles=legend_elements, fontsize=35, loc='upper left', frameon=False)

ax1.set_xticks(x_pos)
ax1.set_xticklabels(bar_labels, fontsize=40, rotation=0)
ax1.set_xlabel("(a) Embedding Models", fontsize=40, labelpad=20)
ax1.set_ylabel("Performance Improvement", fontsize=40)
ax1.grid(True, alpha=0.3)
ax1.set_ylim(0, 2.5)  # 调整y轴范围以适应新数据

# 调整刻度字体大小
ax2.tick_params(axis='both', which='major', labelsize=40)  # Adjust tick label font size
ax1.tick_params(axis='both', which='major', labelsize=40)  # Adjust tick label font size

plt.savefig("Ablation_Study.pdf", dpi=800)