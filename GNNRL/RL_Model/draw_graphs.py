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
    # data1 = np.array([2.20, 6.54, 1.22, 3.76, 7.92, 6.53, 7.39, 2.96])
    # data2 = np.array([2.20, 6.54, 1.22, 3.76, 7.92, 6.53, 7.39, 2.96])
    # data3 = np.array([2.10, 4.83, 1.20, 2.40, 7.82, 5.26, 12.13, 3.58])
    # data3 = np.array([2.22, 6.54, 1.23, 3.94, 7.96, 6.55, 12.31, 4.01])
        # Create a DataFrame
    df = pd.DataFrame({
        'Group': labels * 3,
        'Value': np.concatenate([data1, data2, data3]),
        'Category': ['Autophase'] * 9 + ['-O3'] * 9 + ['DAPO'] * 9
    })

    sns.barplot(data=df, x='Group', y='Value', hue='Category', palette=['#a8acb9', '#6d4e7e', '#cb7e83'], ax=ax1)

    # Add title and labels
    ax1.set_title('Autophase, -O3, DAPO Inference Results w.r.t. -O0', fontsize=14)
    ax1.set_xlabel('')
    ax1.set_xticklabels(labels, fontsize=14)
    ax1.set_ylabel('Performance Improvement', fontsize=14)

    for p in ax1.patches:
        height = p.get_height()
        if height == 0.0:
            continue
        else:
            ax1.annotate(f'{p.get_height():.2f}', 
                        (p.get_x() + p.get_width() / 2., p.get_height()), 
                        ha='center', va='baseline', fontsize=10.5, color='black', xytext=(0, 1), 
                        textcoords='offset points')

    ax1.legend(fontsize=14)
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
    plt.style.use('default')
    # ['#a8acb9', '#cb7e83']
    colors = ['#a8acb9', '#95baa6', '#2b6688', '#cb7e83']
    
    data1 = np.array([6.04, 5.07, 6.26, 5.88])
    data2 = np.array([1.21, 1.17,  3.24, 4.73])

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8, 4), sharey=True)
    fig.subplots_adjust(wspace=0.3)

    def plot_bars_and_line(ax, data, with_hatch=False):
        bars = ax.bar(range(4), data, color=colors, edgecolor='black', linewidth=1.5, width=0.6,
                    hatch='//' if with_hatch else '')
        
        x = [bar.get_x() + bar.get_width() / 2 for bar in bars]
        y = [bar.get_height() for bar in bars]
        
        ax.plot(x, y, color='red', linestyle='-', linewidth=2, marker='o')

    plot_bars_and_line(ax1, data1, with_hatch=True)

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
                        ha='center', va='baseline', fontsize=12, color='black', xytext=(0, 5), 
                        textcoords='offset points')
            
    ax1.set_title('Training Result w.r.t -O0', fontsize=14)
    ax2.set_title('Inference Result w.r.t. -O0', fontsize=14)
    ax1.set_xlabel('')
    ax2.set_xlabel('')
    ax1.set_ylabel('Performance Improvement', fontsize=14)

    for ax in (ax1, ax2):
        ax.set_xticks(range(4))
        ax.set_xticklabels(['Autophase', 'GCN', 'PNA', r'$\bf{RGCN^*}$'], fontsize=14)

    plt.tight_layout()
    # ax1.grid(False)

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
    plt.style.use('default')
    colors = ['#a8acb9', '#95baa6', '#2b6688', '#cb7e83']
    plt.subplots_adjust(left=0.1, right=0.98, bottom=0.15, top=0.95)
    data1 = np.array([6.04, 5.07, 6.26, 5.88])
    data2 = np.array([1.21, 1.17, 3.24, 4.73])

    fig, ax = plt.subplots(figsize=(12, 6))

    x1 = np.arange(4)
    x2 = np.arange(5.5, 9.5)  # 增加间隔

    bars1 = ax.bar(x1, data1, color=colors, edgecolor='black', linewidth=1.5, width=0.6, hatch='//', label='Training')
    bars2 = ax.bar(x2, data2, color=colors, edgecolor='black', linewidth=1.5, width=0.6, label='Inference')

    x = np.concatenate([x1, x2])
    y = np.concatenate([data1, data2])

    ax.plot(x1, data1, color='red', linestyle='-', linewidth=2, marker='o')
    ax.plot(x2, data2, color='red', linestyle='-', linewidth=2, marker='o')

    ax.set_title('Performance Improvement', fontsize=14)
    ax.set_ylabel('Performance Improvement', fontsize=14)

    ax.set_xticks(np.concatenate([x1, x2]))
    ax.set_xticklabels(['Autophase', 'GCN', 'PNA', r'$\bf{RGCN^*}$', 
                        'Autophase', 'GCN', 'PNA', r'$\bf{RGCN^*}$'], fontsize=14, rotation=45)

    for p in bars1 + bars2:
        height = p.get_height()
        ax.annotate(f'{height:.2f}', 
                    (p.get_x() + p.get_width() / 2., height), 
                    ha='center', va='baseline', fontsize=12, color='black', xytext=(0, 5), 
                    textcoords='offset points')

    ax.legend()

    ax.set_xlim(-0.5, 9)
  
    # plt.tight_layout(rect=[0, 0.1, 1, 1.0])
    plt.savefig("a.pdf", dpi=800)
    # plt.tight_layout()
if __name__ == "__main__":
    inference_graph()