import collections as cl
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from tabulate import tabulate

def SetPlotStyle():
    plt.style.use('ggplot')
    plt.rcParams['lines.markeredgewidth'] = 0.0
    plt.rcParams['lines.linewidth'] = 2.15
    plt.rcParams['font.size'] = 10
    plt.rcParams['axes.labelsize'] = 12
    plt.rcParams['axes.labelweight'] = 'bold'
    plt.rcParams['axes.facecolor'] = 'whitesmoke'
    plt.rcParams['grid.color'] = 'black'
    plt.rcParams['grid.linestyle'] = '--'
    plt.rcParams['grid.linewidth'] = '0.25'
    plt.rcParams['grid.alpha'] = '0.25'
    plt.rcParams['xtick.labelsize'] = 10
    plt.rcParams['ytick.labelsize'] = 10
    plt.rcParams['legend.fontsize'] = 10
    plt.rcParams['legend.frameon'] = False
    plt.rcParams['figure.titlesize'] = 'large'
    plt.rcParams['figure.titleweight'] = 'bold'
    plt.rcParams['axes.edgecolor'] = 'black'
    plt.rcParams['patch.edgecolor'] = 'none'
    plt.rcParams.update({'figure.max_open_warning': 0})

def AddBoxes(ax, xseries, yseries, label, color):
    for i in range(len(xseries) - 1):
        x0 = xseries[i]
        y0 = yseries[i]
        width = abs(xseries[i + 1] - xseries[i])
        height = abs(yseries[i + 1] - yseries[i]) 
        
        if (height > 0):
            label = '' if (i > 0) else label
            ax.add_patch(patches.Rectangle((x0, y0), width, height, alpha=0.25, color=color, label=label))

def plot(actual, intended, save):
    SetPlotStyle()

    fig = plt.figure()
    ax = fig.add_subplot(111)
    
    AddBoxes(ax, actual, intended, 'Actual to Intended', 'red')
    AddBoxes(ax, intended, intended,'Intended to Intended', 'blue')

    plt.xlabel('Actual Bins')
    plt.ylabel('Intended Bins')

    plt.xlim(actual[0], actual[-1])
    plt.ylim(intended[0], intended[-1])

    plt.legend(loc = 9)
    plt.savefig(save + '.pdf')

    df = pd.DataFrame(cl.OrderedDict([('Actual', actual), ('Intended', intended)]))
    table = tabulate(df, headers='keys', tablefmt='csv')

    outfile = open(save + '.csv', "w")
    outfile.write(table)
    outfile.close()


actual_pulls = [-4,-3.8,-4,-3.6,-3.8,-3.6,-3.8,-3.4,-3.6,-3.4,-3.4,-3.2,-3.4,-3,-3.2,-3,-3,-2.8,-3,-2.6,-2.8,-2.6,-2.8,-2.4,-2.6,-2.4,-2.4,-2.2,-2.4,-2,-2.2,-2,-2,-1.8,-2,-1.6,-1.8,-1.6,-1.8,-1.4,-1.6,-1.4,-1.4,-1.2,-1.4,-1,-1.2,-1,-1,-0.8,-1,-0.6,-0.8,-0.6,-0.8,-0.4,-0.6,-0.4,-0.4,-0.2,-0.4,0,-0.2,0,0,0.2,0,0.4,0.2,0.4,0.2,0.6,0.4,0.6,0.6,0.8,0.6,1,0.8,1,1,1.2,1,1.4,1.2,1.4,1.2,1.6,1.4,1.6,1.6,1.8,1.6,2,1.8,2,2,2.2,2,2.4,2.2,2.4,2.2,2.6,2.4,2.6,2.6,2.8,2.6,3,2.8,3,3,3.2,3,3.2,3.2,3.4,3.2,3.6,3.4,3.6,3.6,3.8,3.6,4,3.8,4]
intended_pulls = [-4,-3.875,-3.875,-3.75,-3.75,-3.625,-3.625,-3.5,-3.5,-3.375,-3.375,-3.25,-3.25,-3.125,-3.125,-3,-3,-2.875,-2.875,-2.75,-2.75,-2.625,-2.625,-2.5,-2.5,-2.375,-2.375,-2.25,-2.25,-2.125,-2.125,-2,-2,-1.875,-1.875,-1.75,-1.75,-1.625,-1.625,-1.5,-1.5,-1.375,-1.375,-1.25,-1.25,-1.125,-1.125,-1,-1,-0.875,-0.875,-0.75,-0.75,-0.625,-0.625,-0.5,-0.5,-0.375,-0.375,-0.25,-0.25,-0.125,-0.125,0,0,0.125,0.125,0.25,0.25,0.375,0.375,0.5,0.5,0.625,0.625,0.75,0.75,0.875,0.875,1,1,1.125,1.125,1.25,1.25,1.375,1.375,1.5,1.5,1.625,1.625,1.75,1.75,1.875,1.875,2,2,2.125,2.125,2.25,2.25,2.375,2.375,2.5,2.5,2.625,2.625,2.75,2.75,2.875,2.875,3,3,3.125,3.125,3.25,3.25,3.375,3.375,3.5,3.5,3.625,3.625,3.75,3.75,3.875,3.875,4]

actual_res = [-4.0625,-3.90625,-3.90625,-3.75,-3.75,-3.59375,-3.75,-3.4375,-3.59375,-3.28125,-3.4375,-3.28125,-3.28125,-3.125,-3.125,-2.96875,-3.125,-2.8125,-2.96875,-2.65625,-2.8125,-2.65625,-2.65625,-2.5,-2.5,-2.34375,-2.5,-2.1875,-2.34375,-2.03125,-2.1875,-2.03125,-2.03125,-1.875,-1.875,-1.71875,-1.875,-1.5625,-1.71875,-1.40625,-1.5625,-1.40625,-1.40625,-1.25,-1.25,-1.09375,-1.25,-0.9375,-1.09375,-0.78125,-0.9375,-0.78125,-0.78125,-0.625,-0.625,-0.46875,-0.625,-0.3125,-0.46875,-0.15625,-0.3125,-0.15625,-0.15625,0,0,0.15625,0,0.3125,0.15625,0.46875,0.3125,0.46875,0.46875,0.625,0.625,0.78125,0.625,0.9375,0.78125,1.09375,0.9375,1.09375,1.09375,1.25,1.25,1.40625,1.25,1.5625,1.40625,1.71875,1.5625,1.71875,1.71875,1.875,1.875,2.03125,1.875,2.1875,2.03125,2.34375,2.1875,2.34375,2.34375,2.5,2.5,2.65625,2.5,2.8125,2.65625,2.96875,2.8125,2.96875,2.96875,3.125,3.125,3.28125,3.125,3.4375,3.28125,3.59375,3.4375,3.59375,3.59375,3.75,3.75,3.90625,3.75,4.0625]
intended_res = [-4,-3.875,-3.875,-3.75,-3.75,-3.625,-3.625,-3.5,-3.5,-3.375,-3.375,-3.25,-3.25,-3.125,-3.125,-3,-3,-2.875,-2.875,-2.75,-2.75,-2.625,-2.625,-2.5,-2.5,-2.375,-2.375,-2.25,-2.25,-2.125,-2.125,-2,-2,-1.875,-1.875,-1.75,-1.75,-1.625,-1.625,-1.5,-1.5,-1.375,-1.375,-1.25,-1.25,-1.125,-1.125,-1,-1,-0.875,-0.875,-0.75,-0.75,-0.625,-0.625,-0.5,-0.5,-0.375,-0.375,-0.25,-0.25,-0.125,-0.125,0,0,0.125,0.125,0.25,0.25,0.375,0.375,0.5,0.5,0.625,0.625,0.75,0.75,0.875,0.875,1,1,1.125,1.125,1.25,1.25,1.375,1.375,1.5,1.5,1.625,1.625,1.75,1.75,1.875,1.875,2,2,2.125,2.125,2.25,2.25,2.375,2.375,2.5,2.5,2.625,2.625,2.75,2.75,2.875,2.875,3,3,3.125,3.125,3.25,3.25,3.375,3.375,3.5,3.5,3.625,3.625,3.75,3.75,3.875,3.875,4]

plot(actual_pulls, intended_pulls, 'Migration-Pulls')
plot(actual_res, intended_res, 'Migration-Res')
