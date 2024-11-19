import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

def inference_graph():
    plt.style.use('default')
    fig, ax1 = plt.subplots(1, 1, figsize=(12.5 ,4))
    fig.subplots_adjust(left=0.08, right=0.98, bottom=0.12, top=0.93)

    labels = ['Fold 1', 'Fold 2', 'Fold 3', 'Fold 4', 'Fold 5', 'Fold 6', 'Fold 7', 'Fold 8', 'Avg']
    data1 = np.array([1.159, 1.011, 1.004, 1.064, 0.985, 1.057, 1.069, 1.058, 1.050875])
    print(data1.sum() / 8)
    data2 = np.array([1.449, 1.819, 1.433, 3.581, 1.365, 1.087, 1.081, 1.13, 1.618125])
    print(data2.sum() / 8)
    data3 = np.array([1.448, 1.829, 1.305, 3.877, 1.354, 1.088, 1.078, 2.258, 1.926755])
    print(data3.sum() / 8)
    # data1 = np.array([1.37, 1.08, 1.01, 1.38, 1.14, 1.40, 1.36, 1.29])
    # data1 = np.array([2.20, 6.54, 1.20, 3.76, 7.92, 6.53, 7.39, 2.96])
    # data2 = np.array([2.20, 6.54, 1.20, 3.76, 7.92, 6.53, 7.39, 2.96])
    # data3 = np.array([2.10, 4.83, 1.20, 2.40, 7.82, 5.26, 12.13, 3.58])
    # data3 = np.array([2.20, 6.54, 1.23, 3.94, 7.96, 6.55, 12.31, 4.01])
        # Create a DataFrame
    df = pd.DataFrame({
        'Group': labels * 3,
        'Value': np.concatenate([data1, data2, data3]),
        'Category': ['Autophase'] * 9 + ['-O3'] * 9 + ['DAPO'] * 9
    })

    sns.barplot(data=df, x='Group', y='Value', hue='Category', palette=['#a8acb9', '#6d4e7e', '#cb7e83'], ax=ax1)

    # Add title and labels
    ax1.set_title('Autophase, -O3, DAPO Inference Results w.r.t. -O0', fontsize=18)
    ax1.set_xlabel('')
    ax1.set_xticklabels(labels, fontsize=18)
    ax1.set_ylabel('Performance Improvement', fontsize=18)

    for p in ax1.patches:
        height = p.get_height()
        if height == 0.0:
            continue
        else:
            ax1.annotate(f'{p.get_height():.2f}', 
                        (p.get_x() + p.get_width() / 2., p.get_height()), 
                        ha='center', va='baseline', fontsize=14.5, color='black', xytext=(0, 1), 
                        textcoords='offset points')

    ax1.legend(fontsize=18)
    plt.savefig("Inference_O3D.pdf", dpi=800)

# fig, ax2 = plt.subplots(1, 1, figsize=(6 ,5))
# fig.subplots_adjust(left=0.1, right=0.98, bottom=0.1, top=0.93)

# labels = ['GCN', 'RGCN','Autophase', 'PNA']
# data1 = np.array([5.07, 5.88, 6.04, 6.26])
# data2 = np.array([1.17, 4.73, 1.21, 3.24])

# labels = ['Autophase', 'GCN','RGCN', 'PNA']
# data1 = np.array([6.04, 5.07, 5.88, 6.26])
# data2 = np.array([1.21, 1.17, 4.73, 3.24])

# # Create a DataFrame
# df = pd.DataFrame({
#     'Group': labels * 2,
#     'Value': np.concatenate([data1, data2]),
#     'Category': ['Training'] * 4 + ['Inference'] * 4
# })

# # Create a bar plot with a custom color palette
# sns.barplot(data=df, x='Group', y='Value', hue='Category', palette=['#eb9092', '#8ca3c3'], ax=ax2)
# # Add title and labels
# ax2.set_title('Training-Inference Gap Comparison with Respect to -O0', fontsize=14)
# ax2.set_xlabel('')
# ax2.set_xticklabels(labels, fontsize=14)
# ax2.set_ylabel('Speedup', fontsize=14)

# # Annotate bars with their values
# for p in ax2.patches:
#     height = p.get_height()
#     if height == 0.0:
#         continue
#     ax2.annotate(f'{p.get_height():.2f}x', 
#                 (p.get_x() + p.get_width() / 2., p.get_height()), 
#                 ha='center', va='baseline', fontsize=12, color='black', xytext=(0, 5), 
#                 textcoords='offset points')

# # Show plot
# ax2.legend(fontsize=14, loc='best').remove()

def train_infer():
    # 设置风格和颜色
    plt.style.use('default')
    # ['#a8acb9', '#cb7e83']
    colors = ['#a8acb9', '#95baa6', '#2b6688', '#cb7e83']
    
    # 创建数据
    data1 = np.array([6.04, 5.07, 6.26, 5.88])
    data2 = np.array([1.21, 1.17,  3.24, 4.73])

    # 创建图表
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 4), sharey=True)
    fig.subplots_adjust(wspace=0.3)

    # 函数用于绘制柱状图和连线
    def plot_bars_and_line(ax, data, with_hatch=False):
        bars = ax.bar(range(4), data, color=colors, edgecolor='black', linewidth=1.5, width=0.6,
                    hatch='//' if with_hatch else '')
        
        # 获取柱子的中心x坐标和顶部y坐标
        x = [bar.get_x() + bar.get_width() / 2 for bar in bars]
        y = [bar.get_height() for bar in bars]
        
        # 绘制连接线
        ax.plot(x, y, color='red', linestyle='-', linewidth=2, marker='o')

    # 绘制第一组柱状图（带斜线阴影）和连接线
    plot_bars_and_line(ax1, data1, with_hatch=True)

    # 绘制第二组柱状图（实心）和连接线
    plot_bars_and_line(ax2, data2)

    # for p in ax1.patches:
    #     height = p.get_height()
    #     if height == 0.0:
    #         continue
    #     else:
    #         ax1.annotate(f'{p.get_height():.2f}', 
    #                     (p.get_x() + p.get_width() / 2., p.get_height()), 
    #                     ha='center', va='baseline', fontsize=8, color='black', xytext=(0, 5), 
    #                     textcoords='offset points')
    for p in ax2.patches:
        height = p.get_height()
        if height == 0.0:
            continue
        else:
            ax2.annotate(f'{p.get_height():.2f}', 
                        (p.get_x() + p.get_width() / 2., p.get_height()), 
                        ha='center', va='baseline', fontsize=20, color='black', xytext=(0, 5), 
                        textcoords='offset points')
    # 设置标签和标题
    ax1.set_title('Training Result w.r.t -O0', fontsize=18)
    ax2.set_title('Inference Result w.r.t. -O0', fontsize=18)
    ax1.set_xlabel('', labelpad=10)  # Adjust labelpad to move xlabel closer
    ax2.set_xlabel('', labelpad=10)  # Adjust labelpad to move xlabel closer
    ax1.set_ylabel('Performance Improvement', fontsize=18)

    # 设置刻度
    for ax in (ax1, ax2):
        ax.set_xticks(range(4))
        ax.set_xticklabels(['Autophase', 'GCN', 'PNA', r'$\bf{RGCN^*}$'], fontsize=18)

    plt.tight_layout()
    # # 移除网格线
    # ax1.grid(False)
    
    # # 只保留底部和左侧的轴线
    # ax1.spines['top'].set_visible(False)
    # ax1.spines['right'].set_visible(False)
    # ax1.spines['left'].set_visible(True)
    # ax1.spines['bottom'].set_visible(True)
    # ax2.spines['top'].set_visible(False)
    # ax2.spines['right'].set_visible(False)
    # ax2.spines['left'].set_visible(True)
    # ax2.spines['bottom'].set_visible(True)
    plt.savefig("Train_Infer.pdf", dpi=800)

def new_train_infer():
    # 设置风格和颜色
    plt.style.use('default')
    colors = ['#a8acb9', '#95baa6', '#2b6688', '#cb7e83']
    plt.subplots_adjust(left=0.1, right=0.98, bottom=0.15, top=0.95)
    # 创建数据
    data1 = np.array([6.04, 5.07, 6.26, 5.88])
    data2 = np.array([1.21, 1.17, 3.24, 4.73])

    # 创建图表
    fig, ax = plt.subplots(figsize=(12, 6))

    # 自定义x位置
    x1 = np.arange(4)
    x2 = np.arange(5.5, 9.5)  # 增加间隔

    # 绘制柱状图
    bars1 = ax.bar(x1, data1, color=colors, edgecolor='black', linewidth=1.5, width=0.6, hatch='//', label='Training')
    bars2 = ax.bar(x2, data2, color=colors, edgecolor='black', linewidth=1.5, width=0.6, label='Inference')

    # 获取柱子的中心x坐标和顶部y坐标
    x = np.concatenate([x1, x2])
    y = np.concatenate([data1, data2])

    # 绘制连接线
    ax.plot(x1, data1, color='red', linestyle='-', linewidth=2, marker='o')
    ax.plot(x2, data2, color='red', linestyle='-', linewidth=2, marker='o')

    # 设置标签和标题
    ax.set_title('Performance Improvement', fontsize=18)
    ax.set_ylabel('Performance Improvement', fontsize=18)

    # 设置刻度
    ax.set_xticks(np.concatenate([x1, x2]))
    ax.set_xticklabels(['Autophase', 'GCN', 'PNA', r'$\bf{RGCN^*}$', 
                        'Autophase', 'GCN', 'PNA', r'$\bf{RGCN^*}$'], fontsize=18, rotation=45)

    # 添加注释
    for p in bars1 + bars2:
        height = p.get_height()
        ax.annotate(f'{height:.2f}', 
                    (p.get_x() + p.get_width() / 2., height), 
                    ha='center', va='baseline', fontsize=20, color='black', xytext=(0, 5), 
                    textcoords='offset points')

    # 添加图例
    ax.legend(fontsize=18)

    # 调整x轴限制以减少第八个柱

def Training_Inference_bar():
    plt.style.use('default')
    colors = ['#cb7e83', '#95baa6', '#2b6688']
    labels = ['AutoPhase', 'GCN', 'PNA', r'$\bf{DAPO}$']
    data = np.array([
        [1.05, 1.91, 3.9],
        [1.06, 1.96, 4.88],
        [1.61, 2.01, 4.06],
        [1.93, 1.99, 4.36]
    ])
    bar_labels = ['Inference', 'Training', 'Compilation Speedup']

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    fig.subplots_adjust(left=0.065, right=0.98, top=0.92, bottom=0.2, hspace=0.4)

    bar_width = 0.2
    x = np.arange(len(labels))

    for i in range(data.shape[1] - 1):  # Exclude the last column for bars
        bars = ax1.bar(x + i * bar_width, data[:, i], color=colors[i], edgecolor='black', linewidth=1.5, width=bar_width, label=bar_labels[i])

    # Plot stars for the last column
    star_x = x + bar_width / 2 - 0.12  # Further shift the stars to the left
    star_y = data[:, -1]  # Adjust height for stars
    ax2.bar(star_x, star_y, color=colors[2], edgecolor='black', linewidth=1.5, width=bar_width, label='Inference')  # Use the color of the "Inference" bars
    # ax2.plot(star_x, star_y, color=colors[0], linestyle='--', linewidth=2, marker='*', markersize=10, label='Inference')  # Use the color of the "Inference" bars
    ax2.set_xticks(x + bar_width / 2 - 0.12)
    ax2.set_xticklabels(labels, fontsize=20)
    
    ax1.set_xticks(x + bar_width)
    ax1.set_xticklabels(labels, fontsize=20)
    ax1.set_ylabel('Performance Improvement', fontsize=20)
    ax1.set_title('Performance Improvement w.r.t -O0 -O3 (a)', fontsize=20)
    ax1.tick_params(axis='y', labelsize=20)

    for i, bars in enumerate(ax1.containers):
        for p in bars:
            height = p.get_height()
            ax1.annotate(f'{height:.2f}', 
                        (p.get_x() + p.get_width() / 2., p.get_height()), 
                        ha='center', va='baseline', fontsize=14, color='black', xytext=(-0.5, 0.5), 
                        textcoords='offset points')

    for x, y in zip(star_x, star_y):
        ax2.annotate(f'{y :.2f}', 
                    (x, y), 
                    ha='center', va='baseline', fontsize=14, color='black', xytext=(-0.5, 0.5), 
                    textcoords='offset points')

    ax1.axhline(y=1.62, color='black', linestyle='--', linewidth=2)
    ax1.text(-0.2, 1.62, '-O3', color='black', fontsize=20, va='bottom', ha='left')
    ax1.axhline(y=1.0, color='black', linestyle='--', linewidth=2)
    ax1.text(-0.2, 0.98, '-O0', color='black', fontsize=20, va='top', ha='left')
    # ax1.set_xlabel('(a)', fontsize=20, labelpad=10)  # Adjust labelpad to move xlabel closer

    ax2.set_title('Compilation Speedup w.r.t -O3 (b)', fontsize=20)
    ax2.set_yticks([3.5, 4, 4.5, 5])
    ax2.set_ylim(3.5, 5)
    ax2.tick_params(axis='y', labelsize=20)
    ax2.set_ylabel('Compilation Time Speedup', fontsize=20)
    # ax2.set_xlabel('(b)', fontsize=20, labelpad=10)  # Adjust labelpad to move xlabel closer
    # ax2.set_title('Compilation Time Speedup', fontsize=20)
    # ax2.grid(True)  # Add grid background
    ax2.legend(fontsize=20, loc='upper center', bbox_to_anchor=(0.5, -0.08), ncol=1)  # Add legend

    ax1.legend(fontsize=20, loc='upper center', bbox_to_anchor=(0.5, -0.08), ncol=3)

    plt.savefig("Training_Inference_bar.pdf", dpi=800)

if __name__ == "__main__":
    # inference_graph()
    # train_infer()
    # new_train_infer()
    Training_Inference_bar()