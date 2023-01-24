import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

data2 = pd.read_excel("../../Evaluation Results/RQ1/allReqAverage.xlsx")
box_plot = sns.boxplot(y="% of incorrectly labelled test inputs over dataset size", x="Surrogate Algorithms", 
                 data=data2, 
                 palette="colorblind",
                 order=["SA - DYN","LSB","GL","NN","GNL","RT","RF","SVR"],
                 showmeans=True,
                 width=0.4
                 )

sns.despine(right=True)
meann = data2.groupby(['Surrogate Algorithms'])['% of incorrectly labelled test inputs over dataset size'].mean()
print(meann)
mediann = data2.groupby(['Surrogate Algorithms'])['% of incorrectly labelled test inputs over dataset size'].median()
meann = round(meann,3)
vertical_offset = data2['% of incorrectly labelled test inputs over dataset size'].median() * 0.19 # offset from median for display
mlmodels = ["SA - DYN","LSB","GL","NN","GNL","RT","RF","SVR"]
box_plot.set_xlabel("Surrogate Algorithms",fontsize=14,fontdict=dict(weight='semibold'))
box_plot.set_ylabel("% of incorrectly labelled tests \n over dataset size",fontsize=14,fontdict=dict(weight='semibold'))
box_plot.set_xticklabels(["SA - DYN","SA - LSB","SA - GL","SA - NN","SA - GNL","SA - RT","SA - RF","SA - SVR"], rotation=45)
box_plot.tick_params(labelsize=14)

i = 0
for xtick in box_plot.get_xticks():
    box_plot.text(xtick+0.50,meann[mlmodels[xtick]],f'{meann[mlmodels[xtick]]}', 
            horizontalalignment='center',size='small',color='black',weight='semibold')
    i = i + 1
# plt.tight_layout()
plt.savefig("ErrorboxplotWithFSM.pdf",bbox_inches='tight')