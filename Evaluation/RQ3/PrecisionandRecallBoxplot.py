import matplotlib.pyplot as plt

# import the numpy package
import numpy as np

import pandas as pd

import seaborn as sns

olddata = pd.read_excel("dataforboxplot.xlsx", header=0)

metric = 'FailPrecision'

summer_rain = [olddata.loc[:, metric+'ALHAZEN'], olddata.loc[:, metric+'SA']]

ticks = ['SoTA', 'SA']

avg1 = round(olddata[metric+'ALHAZEN'].mean(),2)
avg2 = round(olddata[metric+'SA'].mean(),2)


means = [avg1, avg2]

summer_rain_plot = plt.boxplot(summer_rain, widths=0.4, showmeans= True, patch_artist=True, meanprops={"markerfacecolor": "white", "markeredgecolor": "black"})
palette = sns.color_palette("colorblind", 10)

for i, box in enumerate(summer_rain_plot['boxes']):
    box.set(facecolor=palette[i])

for i, line in enumerate(summer_rain_plot['means']):
    x, y = line.get_xydata()[0][0], line.get_xydata()[0][1]
    text  = ' {:.2f} '.format(means[i])

    #if i != 11 and i != 12:
    plt.annotate(text, xy = (x + 0.20, y - 0.007), fontsize = 20)

def define_box_properties(plot_name, color_code):

    for k, v in plot_name.items():
        plt.setp(plot_name.get(k), color=palette[1], linewidth = 2)

    plt.plot([], c=palette[0])

for element in ['medians']:
    plt.setp(summer_rain_plot[element], color='black')

plt.yticks(fontsize = 25)
plt.xticks(np.array(np.arange(1, len(ticks) + 1)),ticks, fontsize = 25)

plt.ylabel('Precision', fontsize = 25, fontweight = 'bold')
plt.xlabel("Algorithm", fontsize = 25, fontweight = 'bold')

plt.show()

