import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

boxPlotDataset = pd.read_excel('../../Evaluation Results/RQ2/0Precision&RecallDTDRForBoxplot.xlsx')


color_dict = dict({'Significant':'blue',
                  'Not Significant':'orange'})

fig, axes = plt.subplots(1, 2, figsize=(10,5),sharex=True)

boxPlotDatasetDT = boxPlotDataset[boxPlotDataset["FIM"]=="DT"]
boxPlotDatasetDR = boxPlotDataset[boxPlotDataset["FIM"]=="DR"]

box_plot2 = sns.boxplot(y='Recall', x='Algorithms', 
                 data=boxPlotDatasetDR,
                 palette="colorblind",
                 order=["SA","RS","LR","RT"],
                 showmeans=True,
                 width=0.4,
                 ax = axes[0]
                 )

sns.despine(right=True)
axes[0].set_xlabel("Algorithms",fontsize=16,fontdict=dict(weight='semibold'))
axes[0].set_ylabel("Recall",fontsize=16,fontdict=dict(weight='semibold'))
axes[0].set_xticklabels(["SA","RS","LR","RT"])
axes[0].tick_params(labelsize=16)
# axes[0].set_ylabel = "Recall for Fail class"
meann = boxPlotDatasetDR.groupby(['Algorithms'])['Recall'].mean()
print(meann)
mediann = boxPlotDatasetDR.groupby(['Algorithms'])['Recall'].median()
meann = round(meann,2)
vertical_offset = boxPlotDatasetDR['Recall'].median() * 0.19 # offset from median for display
mlmodels = ["SA","RS","LR","RT"]

i = 0
for xtick in box_plot2.get_xticks():
    box_plot2.text(xtick+0.45,meann[mlmodels[xtick]],f'{meann[mlmodels[xtick]]}', 
            horizontalalignment='center',size='large',color='black',weight='semibold')
    i = i + 1
    
    #===============================
    
box_plot3 = sns.boxplot(y='Precision', x='Algorithms', 
                 data=boxPlotDatasetDR, #[boxPlotDataset["FIM"]=="DR"]
                 palette="colorblind",
                 order=["SA","RS","LR","RT"],
                 showmeans=True,
                 width=0.4,
                 ax = axes[1]
                 )

sns.despine(right=True)
# axes[1].set_xlabel = "Algorithms"
axes[1].set_xlabel("Algorithms",fontsize=16,fontdict=dict(weight='semibold'))
axes[1].set_ylabel("Precision",fontsize=16,fontdict=dict(weight='semibold'))
axes[1].set_xticklabels(["SA","RS","LR","RT"])

axes[1].tick_params(labelsize=16)
meann = boxPlotDatasetDR.groupby(['Algorithms'])['Precision'].mean()
print(meann)
mediann = boxPlotDatasetDR.groupby(['Algorithms'])['Precision'].median()
meann = round(meann,2)
vertical_offset = boxPlotDatasetDR['Precision'].median() * 0.19 # offset from median for display
mlmodels = ["SA","RS","LR","RT"]

i = 0
for xtick in box_plot3.get_xticks():
    box_plot3.text(xtick+0.45,meann[mlmodels[xtick]],f'{meann[mlmodels[xtick]]}', 
            horizontalalignment='center',size='large',color='black',weight='semibold')
    i = i + 1

fig.tight_layout()
fig.show()
    
plt.savefig('%RQ2boxplotDTDRSeparate.pdf',)