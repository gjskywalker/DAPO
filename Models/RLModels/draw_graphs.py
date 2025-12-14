import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
    
def Training_Inference_bar_ASPDAC():
    plt.style.use('default')
    colors = ['#cb7e83', '#95baa6', '#f1c7a8']  # 添加第三种颜色
    labels = ['Random', r'Greedy$^{[8]}$', r'Genetic$^{[24]}$', r'AutoPhase$^{[9]}$', 'DAPO-GCN', r'IR2Vec$^{[34]}$', r'HARP$^{[32]}$', 'DAPO-PNA', r'$\bf{DAPO}$-$\bf{RGCN}$']
    data = np.array([
        [0, 1.24],
        [0, 1.65],
        [0, 1.87],
        [1.05, 1.91],
        [1.06, 1.96],
        [1.35, 1.98],
        [1.54, 1.93],
        [1.61, 2.01],
        [1.93, 1.99]
    ])
    bar_labels = ['Inference', 'Training', 'Heuristics']  # 添加第三个标签

    searching_iteration = [7523, 3154, 6742, 104, 98, 143, 76, 120, 84]
    
    fig, ax1 = plt.subplots(figsize=(30, 15))
    fig.subplots_adjust(left=0.085, right=0.9, top=0.93, bottom=0.23)

    bar_width = 0.3
    x = np.arange(len(labels))

    # 绘制柱状图
    for i in range(data.shape[1]):  # Iterate over the columns of data
        for j in range(len(labels)):  # Iterate over each label
            # 特殊处理Random, Greedy, Genetic三列
            if data[j, i] == 0.0 and labels[j] in ['Random', r'Greedy$^{[19]}$', r'Genetic$^{[14]}$']:
                continue  # 跳过这些情况
                
            # 对于Random, Greedy, Genetic的非零值使用Heuristics颜色
            if labels[j] in ['Random', r'Greedy$^{[19]}$', r'Genetic$^{[14]}$'] and data[j, i] > 0:
                color_to_use = colors[2]  # 使用Heuristics颜色
                label_to_use = bar_labels[2] if (j == 0 and i == 1) else ""  # 只在第一个Heuristics时添加标签
            else:
                color_to_use = colors[i]
                label_to_use = bar_labels[i] if j == 4 and i < 2 else ""  # 只在GCN时添加Inference和Searching标签
                
            ax1.bar(
                x[j] + i * bar_width,
                data[j, i],
                color=color_to_use,
                edgecolor='black',
                linewidth=1.5,
                width=bar_width,
                label=label_to_use
            )
    
    ax1.set_xticks(x + bar_width / 2)
    ax1.set_xticklabels(labels, fontsize=50, rotation=20)
    
    # 设置y轴从0.85开始，只显示1.0, 1.5, 2.0三个刻度
    ax1.set_ylim(0.85, 2.2)
    ax1.set_yticks([1.0, 1.5, 2.0])
    ax1.set_ylabel('Performance Improvement', fontsize=55)
    ax1.set_title('Performance Improvement w.r.t -O0 and -O3', fontsize=55)
    ax1.tick_params(axis='y', labelsize=50)
    
    ax1.axhline(y=1.62, color='black', linestyle='--', linewidth=4)
    ax1.text(-0.2, 1.62, '-O3', color='black', fontsize=50, va='bottom', ha='left')
    ax1.axhline(y=1.0, color='black', linestyle='--', linewidth=4)
    ax1.text(-0.2, 0.98, '-O0', color='black', fontsize=50, va='top', ha='left')
    
    # 添加注释
    for i, bars in enumerate(ax1.containers):
        for p in bars:
            height = p.get_height()
            if height > 0:
                ax1.annotate(f'{height:.2f}', 
                            (p.get_x() + p.get_width() / 2., p.get_height()), 
                            ha='center', va='baseline', fontsize=40, color='black', xytext=(-3, 8), 
                            textcoords='offset points')

    # 添加红色虚线折线
    ax2 = ax1.twinx()  # 创建共享 x 轴的第二个 y 轴
    
    # 创建扩展的x轴位置和对应的searching iteration值
    # 前三个方法只有Training值，后六个方法有Inference和Training两个值
    x_extended = []
    y_extended = []
    
    # 前三个方法 (Random, Greedy, Genetic) - 只有Training值
    for i in range(3):
        x_extended.append(x[i] + bar_width)  # Training bar的位置
        y_extended.append(searching_iteration[i])  # Training的searching iteration
    
    # 后六个方法 - 每个方法有两个值 (Inference, Training)
    for i in range(3, 9):
        # Inference bar的位置和值
        x_extended.append(x[i])  # Inference bar的位置
        y_extended.append(1)  # Inference的searching iteration都是1
        
        # Training bar的位置和值
        x_extended.append(x[i] + bar_width)  # Training bar的位置
        y_extended.append(searching_iteration[i])  # Training的searching iteration
    
    # 绘制折线图
    ax2.plot(x_extended, y_extended, color='blue', linestyle='--', linewidth=2, marker='o', markersize=10, label='Searching Iterations')
    ax2.set_ylabel('# Searching Iterations', fontsize=55, color='black')
    ax2.tick_params(axis='y', labelsize=50, colors='black')
    ax2.set_yscale('log')  # 设置 y 轴为对数刻度
    ax2.set_ylim(1, 10000)  # 设置右侧 y 轴范围
    ax2.set_yticks([1, 10, 100, 1000, 10000])  # 设置右侧 y 轴刻度
    ax2.get_yaxis().set_major_formatter(plt.FuncFormatter(lambda y, _: f"$10^{{{int(np.log10(y))}}}$"))  # 格式化为指数形式

    # 在每个红点上标注 iteration numbers
    for i, (x_pos, iteration) in enumerate(zip(x_extended, y_extended)):
        if iteration == 7523:  # 第一个"7523"放在红点以下
            yy = -40
            va = 'top'
        elif iteration == 6742:  # "2154"往上偏移一点
            yy = -60
            va = 'bottom'
        elif iteration == 143:  # "143"往上偏移一点
            yy = 25
            va = 'bottom'
        else:
            yy = 20
            va = 'bottom'
        ax2.annotate(f'{iteration}', 
                     (x_pos, iteration), 
                     ha='center', va=va, fontsize=40, color='blue', xytext=(0, yy), 
                     textcoords='offset points',
                     bbox=dict(boxstyle='round,pad=0.1', facecolor='none', edgecolor='blue', linewidth=3))

    # 合并 legend
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_2, labels_2 = ax2.get_legend_handles_labels()
    ax1.legend(lines_1 + lines_2, labels_1 + labels_2, fontsize=50, loc='upper center', bbox_to_anchor=(0.5, -0.16), ncol=4, frameon=False)

    plt.savefig("Training_Inference_bar_with_heuristics.pdf", dpi=800)
    
def Training_Inference_bar_DATE():
    plt.style.use('default')
    colors = ['#cb7e83', '#95baa6', '#f1c7a8']  # 添加绿色
    labels = ['Random', r'AutoPhase1$^{[9]}$', r'Greedy$^{[8]}$', r'Genetic$^{[24]}$', r'AutoPhase2$^{[9]}$', r'IR2Vec$^{[16]}$', r'HARP$^{[10]}$', r'$\bf{DAPO}$']
    # 修改数据结构：前4个只有一个值，后4个有两个值[inference, training]
    inference_data = [1.24, 1.61, 1.65, 1.77, 1.25, 1.35, 1.54, 1.93]
    training_data = [None, None, None, None, 1.81, 1.86, 1.92, 1.99]  # 后4个的training结果
    bar_labels = ['Training', 'Inference']  # 修改标签

    searching_iteration = [7523, 132, 3154, 6742, 1, 1, 1, 1]
    # 后四个方法的training对应的searching iteration
    training_searching_iteration = [None, None, None, None, 104, 143, 176, 84]

    fig, ax1 = plt.subplots(figsize=(30, 15))
    fig.subplots_adjust(left=0.085, right=0.9, top=0.93, bottom=0.21)

    bar_width = 0.3  # 保持原有柱子宽度
    x = np.arange(len(labels)) * 0.8  # 缩小x轴间距

    # 绘制柱状图
    for j in range(len(labels)):  # 遍历每个标签
        # 前4个方法：只有一个值，使用Training颜色
        if j < 4:
            color_to_use = colors[0]  # Training颜色
            label_to_use = bar_labels[0] if j == 0 else ""  # 只在第一个柱子时添加Training标签
            
            ax1.bar(
                x[j],
                inference_data[j],
                color=color_to_use,
                edgecolor='black',
                linewidth=1.5,
                width=bar_width,
                label=label_to_use
            )
        else:
            # 后4个方法：绘制横向重叠的training和inference bars
            # 先绘制training bar (稍微靠左)
            ax1.bar(
                x[j] - bar_width * 0.2,  # 向左偏移
                training_data[j],
                color=colors[0],  # Training颜色
                edgecolor='black',
                linewidth=1.5,
                width=bar_width,
                label=bar_labels[0] if j == 4 else ""  # 只在第一个时添加Training标签
            )
            
            # 再绘制inference bar (稍微靠右，与training bar重叠)
            ax1.bar(
                x[j] + bar_width * 0.2,  # 向右偏移
                inference_data[j],
                color=colors[1],  # Inference颜色
                edgecolor='black',
                linewidth=1.5,
                width=bar_width,
                label=bar_labels[1] if j == 4 else ""  # 只在第一个时添加Inference标签
            )
    
    ax1.set_xticks(x)
    ax1.set_xticklabels(labels, fontsize=50, rotation=20)
    
    # 设置y轴从0.85开始，只显示1.0, 1.5, 2.0三个刻度
    ax1.set_ylim(0.85, 2.2)
    ax1.set_yticks([1.0, 1.5, 2.0])
    ax1.set_ylabel('Performance Improvement', fontsize=50)
    ax1.set_title('Performance Improvement w.r.t Vitis HLS', fontsize=50)
    ax1.tick_params(axis='y', labelsize=50)
    
    ax1.axhline(y=1.6, color='black', linestyle='--', linewidth=4)
    ax1.text(-0.3, 1.6, 'Vitis HLS', color='black', fontsize=50, va='bottom', ha='left')
    # ax1.axhline(y=1.0, color='black', linestyle='--', linewidth=4)
    # ax1.text(-0.2, 0.98, '-O0', color='black', fontsize=50, va='top', ha='left')
    
    # 添加注释
    for j in range(len(labels)):
        if j < 4:
            # 前4个方法：只注释一个值
            ax1.annotate(f'{inference_data[j]:.2f}', 
                        (x[j], inference_data[j]), 
                        ha='center', va='bottom', fontsize=50, color='black', xytext=(0, 8), 
                        textcoords='offset points')
        else:
            # 后4个方法：注释两个值
            # Training值注释 (左侧柱子)
            ax1.annotate(f'{training_data[j]:.2f}', 
                        (x[j] - bar_width * 0.2, training_data[j]), 
                        ha='center', va='bottom', fontsize=40, color='black', xytext=(0, 8), 
                        textcoords='offset points')
            # Inference值注释 (右侧柱子)
            ax1.annotate(f'{inference_data[j]:.2f}', 
                        (x[j] + bar_width * 0.2, inference_data[j]), 
                        ha='center', va='bottom', fontsize=40, color='black', xytext=(0, 8), 
                        textcoords='offset points')

    # 添加蓝色虚线折线 - 修改版本，为后4个方法的两个柱子分别标注
    ax2 = ax1.twinx()  # 创建共享 x 轴的第二个 y 轴
    
    # 创建x坐标和对应的迭代次数
    x_points = []
    y_points = []
    
    # 前4个方法：每个方法一个点
    for i in range(4):
        x_points.append(x[i])
        y_points.append(searching_iteration[i])
    
    # 后4个方法：每个方法两个点 (training和inference)
    for i in range(4, 8):
        # Training point (左侧柱子) - 使用新的training searching iteration
        x_points.append(x[i] - bar_width * 0.2)
        y_points.append(training_searching_iteration[i])
        # Inference point (右侧柱子) 
        x_points.append(x[i] + bar_width * 0.2)
        y_points.append(1)  # Inference都是1次迭代
    
    # 绘制折线图
    ax2.plot(x_points, y_points, color='blue', linestyle='--', linewidth=2, marker='o', markersize=10, label='Searching Iterations')
    ax2.set_ylabel('# Searching Iterations', fontsize=50, color='blue')
    ax2.tick_params(axis='y', labelsize=50, colors='blue')
    ax2.spines['right'].set_color('blue')
    ax2.set_yscale('log')  # 设置 y 轴为对数刻度
    ax2.set_ylim(1, 10000)  # 设置右侧 y 轴范围
    ax2.set_yticks([1, 10, 100, 1000, 10000])  # 设置右侧 y 轴刻度
    ax2.get_yaxis().set_major_formatter(plt.FuncFormatter(lambda y, _: f"$10^{{{int(np.log10(y))}}}$"))  # 格式化为指数形式

    # 在每个点上标注 iteration numbers
    for i, (x_pos, iteration) in enumerate(zip(x_points, y_points)):
        if iteration == 3154:  
            yy = -40
            va = 'top'
        elif iteration == 6742:  
            yy = -60
            va = 'top'
        elif iteration == 132:  
            yy = -65
            va = 'bottom'
        elif iteration == 7523:
            yy = -65
            va = 'bottom'
        elif iteration == 143:  # 新增的training iteration值
            yy = 25
            va = 'bottom'
        elif iteration == 176:  # 新增的training iteration值
            yy = 25
            va = 'bottom'
        else:
            yy = 20
            va = 'bottom'
        ax2.annotate(f'{iteration}', 
                     (x_pos, iteration), 
                     ha='center', va=va, fontsize=50, color='blue', xytext=(0, yy), 
                     textcoords='offset points',
                     bbox=dict(boxstyle='round,pad=0.1', facecolor='none', edgecolor='blue', linewidth=3))

    # 合并 legend
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_2, labels_2 = ax2.get_legend_handles_labels()
    ax1.legend(lines_1 + lines_2, labels_1 + labels_2, fontsize=50, loc='upper center', bbox_to_anchor=(0.5, -0.16), ncol=3, frameon=False)

    plt.savefig("Training_Inference_bar_with_heuristics_DATE.pdf", dpi=800)
    
if __name__ == "__main__":
    Training_Inference_bar_DATE()