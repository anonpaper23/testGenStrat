import time
import random
import pandas as pd
import numpy as np
from sklearn import tree
import graphviz
from configparser import ConfigParser
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score
from os import sep
import os
from sklearn.tree import DecisionTreeRegressor
from imblearn.over_sampling import SMOTE
from skopt import BayesSearchCV
from skopt.space import Real, Categorical, Integer
import math

np.set_printoptions(threshold=np.inf)
file = 'config.ini'
config = ConfigParser()
config.read(file)

OpenWrtIp = config['OpenWrtMachine']['IP']
OpenWrtUser = config['OpenWrtMachine']['User']
CakeBandwidth = config['OpenWrtMachine']['CAKEbandwidth']

LinuxMachineIp = config['LinuxMachine']['IP']
LinuxMachineUser = config['LinuxMachine']['User']
LinuxMachinePass = config['LinuxMachine']['Password']


ServerIp1 = config['Server']['IP1']
ServerIp2 = config['Server']['IP2']
ServerIp3 = config['Server']['IP3']
ServerIp4 = config['Server']['IP4']
ServerIp5 = config['Server']['IP5']
ServerIp6 = config['Server']['IP6']
ServerIp7 = config['Server']['IP7']
ServerIp8 = config['Server']['IP8']

PathReport1 = config['Dpinger']['PathToReport1']
PathReport2 = config['Dpinger']['PathToReport2']
PathReport3 = config['Dpinger']['PathToReport3']
PathReport4 = config['Dpinger']['PathToReport4']
PathReport5 = config['Dpinger']['PathToReport5']
PathReport6 = config['Dpinger']['PathToReport6']
PathReport7 = config['Dpinger']['PathToReport7']
PathReport8 = config['Dpinger']['PathToReport8']
Report1Name = config['Dpinger']['Report1Name']
Report2Name = config['Dpinger']['Report2Name']
Report3Name = config['Dpinger']['Report3Name']
Report4Name = config['Dpinger']['Report4Name']
Report5Name = config['Dpinger']['Report5Name']
Report6Name = config['Dpinger']['Report6Name']
Report7Name = config['Dpinger']['Report7Name']
Report8Name = config['Dpinger']['Report8Name']


def DeleteFiles():
    for i in range(4):
        file1 = 'MOS' + str(i) + '.png'
        file2 = 'tree' + 'MOS' + str(i)
        try:
            f = open(file1)
            f = open(file2)
        except OSError:
            continue

        os.system("gnome-terminal -- bash -c 'rm " + file1 + "; exit; exec bash'")
        os.system("gnome-terminal -- bash -c 'rm " + file2 + "; exit; exec bash'")

    os.system("gnome-terminal -- bash -c 'rm logicalrulesoftrees.txt ; exit; exec bash'")

def GenerateTraffic(randomnuttcparray, l):
    # highest to lowest
    ports = ['5101', '5102', '5103', '5104', '5105', '5106', '5107', '5108']

    servers = [ServerIp1, ServerIp2, ServerIp3, ServerIp4, ServerIp5, ServerIp6, ServerIp7, ServerIp8]
    for i in range(len(randomnuttcparray)):
        if randomnuttcparray[i] != 0:
            os.system("gnome-terminal -- bash -c 'nuttcp -b -N1 -p" + str(ports[i]) + " -T60s -R" + str(
                randomnuttcparray[i]) + "M " + servers[i] + " >> randomtraffic" + str(l) + str(
                i) + ".txt; exit; exec bash'")

def RunDpinger():
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link11.pid -u /var/run/link11.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport1 + " " + ServerIp1 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link12.pid -u /var/run/link12.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport2 + " " + ServerIp2 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link13.pid -u /var/run/link13.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport3 + " " + ServerIp3 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link14.pid -u /var/run/link14.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport4 + " " + ServerIp4 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link15.pid -u /var/run/link15.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport5 + " " + ServerIp5 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link16.pid -u /var/run/link16.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport6 + " " + ServerIp6 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link17.pid -u /var/run/link17.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport7 + " " + ServerIp7 + "; sleep 0; exit; exec bash'")
    os.system(
        "gnome-terminal -- bash -c 'cd ..; cd dpinger; sudo ./dpinger -S -i link1 -B " + LinuxMachineIp + " -p /var/run/dpinger_link18.pid -u /var/run/link18.sock -C /etc/rc.failover -s 100 -l 400 -t 40s -r 40s -A 1s -D 900 -L 22 -d 256 -R -o " + PathReport8 + " " + ServerIp8 + "; sleep 0; exit; exec bash'")

def KillDpinger():
    for j in range(8):
        os.system("gnome-terminal -- bash -c 'pgrep dpinger | tail -1 |sudo xargs kill -9; exit; exec bash'")

def Fitness():
    #################Read Dpinger####################
    fitnessofdpingers = []

    files = [Report8Name, Report7Name, Report6Name, Report5Name, Report4Name, Report3Name, Report2Name, Report1Name]
    for j in range(8):
        comp = []
        fitness = 0
        with open(files[j]) as f:
            data = f.read()

        comp = data.split(" ")
        comp = comp[1:]
        try:
            comp[-1] = comp[-1][:-1]
        except IndexError:
            comp= [-1]

        for i in range(len(comp)):
            comp[i] = float(comp[i])

        fitnessofdpingers.append(comp)

    return fitnessofdpingers

def DeleteRandomTrafficFiles():
    for i in range(100):
        for j in range(8):
            if i == 0 and j == 0:
                continue
            else:
                file = 'randomtraffic' + str(i) + str(j) + '.txt'

                try:
                    f = open(file)
                except OSError:
                    continue

                os.system("gnome-terminal -- bash -c 'rm " + file + "; exit; exec bash'")

def ApplyCandidateSol(CS):
    # TOS = ['0x{:02x}'.format(x) for x in range(int(0x00),int(0xFF+1))]

    TOS = ['0x00', '0x08', '0x20', '0x28', '0x30', '0x38', '0x40', '0x48', '0x50', '0x58', '0x60', '0x68', '0x70',
           '0x78', '0x80', '0x88', '0x90', '0x98', '0xA0', '0xB8', '0xC0', '0xC8', '0xD0', '0xD8', '0xE0', '0xE8',
           '0xF0', '0xF4', '0xF8', '0xFC']

    for i in range(len(TOS) - 1, -1, -1):
        os.system(
            "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes \"tc filter replace dev eth1 parent 1: protocol all u32 match ip tos " +
            TOS[i] + " 0xff action skbedit priority 1:" + str(CS[i]) + "; exit\"; exit; exec bash'")

    os.system(
        "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes \"tc filter replace dev eth1 parent 1: protocol all u32 match ip protocol 6 0xff match u8 0x05 0x0f at 0 match u16 0x0000 0xffc0 at 2 match u8 0x10 0xff at 33 action skbedit priority 1:1; exit\"; exit; exec bash'")

def yclasses(data):
    ylatency = []

    for i in data.columns[0:1]:
        yl = data.loc[:, f"{i}"].to_numpy()
        ylatency.append(yl)

    return ylatency


def xfeatures(data):
    x = []

    for i in range(len(data.index)):
        lisst = []
        lisst.append(data.loc[i, "TIN 0 Req-BW"])
        lisst.append(data.loc[i, "TIN 1 Req-BW"])
        lisst.append(data.loc[i, "TIN 2 Req-BW"])
        lisst.append(data.loc[i, "TIN 3 Req-BW"])
        lisst.append(data.loc[i, "TIN 4 Req-BW"])
        lisst.append(data.loc[i, "TIN 5 Req-BW"])
        lisst.append(data.loc[i, "TIN 6 Req-BW"])
        lisst.append(data.loc[i, "TIN 7 Req-BW"])

        x.append(lisst)

    x = np.array(x)

    return x


def DecisionTree(x, yclass, index, mosorlatency, data, iteration, clf):

    if iteration == 0: #in first iteration we need to optimize
        clf = BayesSearchCV(tree.DecisionTreeRegressor(random_state= 0), {'min_samples_leaf': Integer(1, max(2, math.floor(len(yclass)/2)))}, n_iter = 50, optimizer_kwargs= {'acq_func' : 'EI'})
        clf.fit(x, yclass, sample_weight=None)
        clf = clf.best_estimator_
    else: #other iterations

        clf.fit(x, yclass, sample_weight=None)

    plt.figure(figsize=(200, 200))

    if mosorlatency == 'MOS':
        _ = tree.plot_tree(clf,
                           filled=True, fontsize=20, rounded=True)

    else:
        _ = tree.plot_tree(clf,
                           filled=True, fontsize=20, rounded=True)

    filename = mosorlatency + str(index) + '.png'
    plt.savefig(filename)
    plt.close('all')

    if mosorlatency == 'MOS':
        dot_data = tree.export_graphviz(clf, out_file='tree' + mosorlatency + str(index),
                                        feature_names=data.columns[6:14],
                                        filled=True, rounded=True)

    return clf


def MainProduceDecisionTree(data, iteration, clf):
    # ylatency, ymos = yclasses(data)
    ylatency = yclasses(data)
    xfeaturess = xfeatures(data)

    clfs = []

    clf = DecisionTree(xfeaturess, ylatency[0], 0, 'MOS', data, iteration,clf)

    f = open("logicalrulesoftress.txt", "w")
    # read latency trees

    # read MOS trees

    for i in range(0, 1):
        arrows, nodes = ReadTree('tree' + 'MOS' + str(i))
        convertoint(arrows)
        listofallpaths = []
        path = extractpath(0, arrows)
        listofallpaths.append(path)
        listofallpaths = FindListofPaths(listofallpaths, path, arrows)

        rules = FindRules(listofallpaths, arrows, nodes)

        rules = FindRules(listofallpaths, arrows, nodes)
        logicalrules = FindLogicalRules(rules, olddata)
        for i in range(len(logicalrules)):
            f.write(logicalrules[i])
            f.write('\n')
        f.write('***')

    f.close()
    return listofallpaths, clf

def ReadTree(file):
    f = open(file, "r")

    datafromtree = f.read().split('\n')

    datafromtree = datafromtree[3:]

    arrows = []
    nodes = []

    for i in range(len(datafromtree)):

        if '->' in datafromtree[i]:
            arrows.append(datafromtree[i])

        else:
            nodes.append(datafromtree[i])

    nodes = nodes[:-1]
    for i in range(len(arrows)):
        if ' [' in arrows[i]:
            ind = arrows[i].index(' [')
            arrows[i] = arrows[i][:ind]
        if ' ;' in arrows[i]:
            ind = arrows[i].index(' ;')
            arrows[i] = arrows[i][:ind]

    f.close()
    return arrows, nodes


def convertoint(arrows):
    for i in range(len(arrows)):
        if ' ' in arrows[i]:
            arrows[i] = arrows[i].replace(' ', '')


def extractpath(root, arrows):
    path = []
    i = 0
    while i < len(arrows):
        firstpart = int(arrows[i][0])
        ind = arrows[i].index('>')
        secondpart = int(arrows[i][ind + 1:])
        if int(root) == firstpart:
            path.append(arrows[i])
            root = secondpart
            i = 0
        i = i + 1

    return path


def checkforchild(node, arrows):
    ind = node.index('>')
    child = node[ind + 1:]
    children = []
    for i in range(len(arrows)):
        ind = arrows[i].index('-')
        if child == arrows[i][:ind]:
            children.append(arrows[i])

    return children


def FindListofPaths(listofallpaths, path, arrows):
    removed = []
    path.insert(0, '0->0')
    while (len(path) != 0):
        last = path[-1]
        b = checkforchild(last, arrows)
        if len(b) == 0:
            listofallpaths.append(path[:])
            path.remove(last)
            removed.append(last)
        else:
            i = 0
            while i < len(b):
                if b[i] in removed:
                    b.remove(b[i])
                    i = -1
                elif b[i] not in removed:
                    path.append(b[i])
                    break
                if len(b) == 0:
                    path.remove(last)
                    removed.append(last)
                    break
                i = i + 1

    listofallpaths = listofallpaths[1:]

    for i in range(len(listofallpaths)):
        listofallpaths[i].remove('0->0')

    return listofallpaths


def extractlabel(node, arrows):
    ind = node.index('-')
    first = node[:ind]
    ind = node.index('>')
    second = node[ind + 1:]
    for i in range(len(arrows)):
        ind = arrows[i].index('-')
        if first == arrows[i][:ind] and node != arrows[i]:
            if int(second) < int(arrows[i][ind + 2:]):
                return True
            else:
                return False


def FindRules(listofallpaths, arrows, nodes):
    rules = []
    for i in range(len(listofallpaths)):
        rule = []
        for j in range(len(listofallpaths[i])):
            a = extractlabel(listofallpaths[i][j], arrows)
            ind = listofallpaths[i][j].index('-')
            first = listofallpaths[i][j][:ind]
            for k in range(len(nodes)):
                listt = []
                ind = nodes[k].index(' [')
                if first == nodes[k][:ind]:
                    ind = nodes[k].index('"')
                    ind2 = nodes[k].index('\\')
                    listt.append(nodes[k][ind + 1:ind2])
                    listt.append(a)
                    ind = nodes[k].index('value')
                    ind2 = nodes[k].index('", fillcolor')
                    listt.append(nodes[k][ind:ind2])
                    rule.append(listt)

        ind = listofallpaths[i][len(listofallpaths[i]) - 1].index('>')
        second = listofallpaths[i][j][ind + 1:]
        for k in range(len(nodes)):
            listt = []
            ind = nodes[k].index(' [')
            if second == nodes[k][:ind]:
                ind = nodes[k].index('value')
                ind2 = nodes[k].index('", fillcolor')
                listt.append(nodes[k][ind:ind2])
                rule.append(listt)

        rules.append(rule)

    return rules


def FindLogicalRules(rules, data):
    logicalrules = []
    for i in range(len(rules)):
        s = ''
        for j in range(len(rules[i]) - 1):
            if rules[i][j][1] == True:
                s = s + rules[i][j][0] + ' ^ '
            else:
                s = s + 'Not( ' + rules[i][j][0] + ' )' + ' ^ '
        s = s[:-3]
        s = s + ' ' + rules[i][-1][0]  # class
        logicalrules.append(s)

    return logicalrules


def NumberofDistincTins(b, a):

    tinsa = [0 for i in range(8)]
    tinsb = [0 for i in range(8)]

    for i in range(len(b)):
        for j in range(len(tinsb)):
            if 'TIN ' + str(j) in b[i]:
                tinsb[j] = 1

    for i in range(len(a)):
        for j in range(len(tinsa)):
            if 'TIN ' + str(j) in a[i]:
                tinsa[j] = 1

    if sum(tinsb) > sum(tinsa):
        return True

    return False


def SeparateGoodandBad():
    f = open("logicalrulesoftress.txt", "r")
    datafromrules = f.read().split("***")
    for i in range(len(datafromrules)):
        datafromrules[i] = datafromrules[i][:-1]
        datafromrules[i] = datafromrules[i].split('\n')

    datafromrules = datafromrules[:-1]

    for i in range(len(datafromrules)):
        datafromrules[i].sort(key=lambda x: float(x.split()[-1]))

    separatedclasses = []

    for i in range(0, 1):  # MOS
        for j in range(len(datafromrules[i])):
            lisst = []
            if float(datafromrules[i][j].split()[-1]) >= 0.8:  # threshold
                h = j + 1
                longer = datafromrules[i][j]
                while (h < len(datafromrules[i]) and datafromrules[i][j].split()[-1] == datafromrules[i][h].split()[
                    -1]):
                    b = datafromrules[i][h].split(" ^ ")
                    if len(longer.split(" ^ ")) <= len(b):
                        if len(longer.split(" ^ ")) == len(b):
                            if NumberofDistincTins(b, longer.split(" ^ ")) == True:
                                longer = datafromrules[i][h]
                        else:
                            longer = datafromrules[i][h]

                    h = h + 1
                rule1 = longer
                rule2 = datafromrules[i][j - 1]
                h = j - 2
                longer = datafromrules[i][j - 1]
                while (h > 0) and datafromrules[i][j - 1].split()[-1] == datafromrules[i][h].split()[-1]:
                    b = datafromrules[i][h].split(" ^ ")
                    if len(longer.split(" ^ ")) <= len(b):
                        if len(longer.split(" ^ ")) == len(b):
                            if NumberofDistincTins(b, longer.split(" ^ ")) == True:
                                longer = datafromrules[i][h]
                        else:
                            longer = datafromrules[i][h]
                    h = h - 1
                rule2 = longer
                lisst.append(rule1)
                lisst.append(rule2)
                separatedclasses.append(lisst)
                break

    return separatedclasses


def CheckForValidSplit(condition, i):
    thresh = [3000, 2000, 2000, 1500, 2, 2, 3, 4]
    thresh = [3000, 2000, 2000, 1500, 0.8, 2, 3, 4]  # sum of mos
    if i < 4:  # latency

        f = open("treeLatency" + str(i + 4), "r")
        f = f.read().split("\n")

        label = condition[:condition.index('-')]
        arrows, nodes = ReadTree("treeLatency" + str(i + 4))
    else:

        f = open("treeMOS" + str(i - 4), "r")
        f = f.read().split("\n")

        label = condition[:condition.index('-')]

        arrows, nodes = ReadTree("treeMOS" + str(i - 4))


    # find labels of children
    children = []
    for j in range(len(arrows)):
        ind = arrows[j].index("-")
        if label == arrows[j][:ind - 1]:
            children.append(arrows[j][ind + 3:])
    # find values of children
    values = []
    for j in range(len(f)):

        if children[0] + " [" == f[j][:len(children[0]) + 2]:
            ind = f[j].index("value")
            ind2 = f[j].index('", fillcolor')
            values.append(float(f[j][ind + 8:ind2]))
        if children[1] + " [" == f[j][:len(children[1]) + 2]:
            ind = f[j].index("value")
            ind2 = f[j].index('", fillcolor')
            values.append(float(f[j][ind + 8:ind2]))

    if values[0] >= thresh[i] and values[1] >= thresh[i]:
        return False
    elif values[0] < thresh[i] and values[1] < thresh[i]:
        return False
    else:
        return True


def ExtractBounds(finalboundaries, listofallpaths):
    tinthresholds = [400, 350, 306, 267, 234, 205, 179, 157]

    bounds = [[] for i in range(8)]
    f = open("logicalrulesoftress.txt", "r")
    f = f.read().split("\n")
    for i in range(len(finalboundaries)):
        # intersections = [[] for i in range(9)]
        intersections = [[] for i in range(8)]
        rule1 = finalboundaries[i][0].split(' ^ ')
        rule2 = finalboundaries[i][1].split(' ^ ')

        for kk in range(len(f)):
            if finalboundaries[i][0] == f[kk]:
                rule1index = listofallpaths[kk]


        for kk in range(len(f)):
            if finalboundaries[i][1] == f[kk]:
                rule2index = listofallpaths[kk]

        ind = rule1[-1].index(' value')
        rule1[-1] = rule1[-1][:ind]
        ind = rule2[-1].index(' value')
        rule2[-1] = rule2[-1][:ind]
        copyrule1 = rule1
        minn = min(len(rule1), len(rule2))
        j = 0

        while j < minn and rule1[j] == rule2[j]:
            flag = False
            tinflag = False
            if rule1[j][0] != 'N':
                flag = False
            else:
                rule1[j] = rule1[j][:-2]
                flag = True
            try:
                ind = rule1[j].index('TIN')
            except ValueError:
                ind = rule1[j].index('sum')
                tinflag = True
            if tinflag == False:
                tin = int(rule1[j][ind + 4])
            else:
                tin = 8  # sum
            ind = rule1[j].index('<=')
            threshold = int(float(rule1[j][ind + 3:]))
            intersections[tin].append((threshold, flag))  # low, up, flag
            j = j + 1

        k = j
        l = len(rule1) - 1
        # a = rule1[l]
        a = rule1index[l]

        h = j
        while j < len(rule1):
            flag = False
            tinflag = False
            if rule1[j][0] != 'N':
                flag = False
            else:
                rule1[j] = rule1[j][:-2]
                flag = True
            try:
                ind = rule1[j].index('TIN')
            except ValueError:
                ind = rule1[j].index("sum")
                tinflag = True
            if tinflag == False:
                tin = int(rule1[j][ind + 4])
            else:
                tin = 8  # sum
            ind = rule1[j].index('<=')
            threshold = int(float(rule1[j][ind + 3:]))
            intersections[tin].append((threshold, flag))  # low, up, flag
            j = j + 1

        while h < len(rule2):
            flag = False
            tinflag = False
            if rule2[h][0] != 'N':
                flag = False
            else:
                rule2[h] = rule2[h][:-2]
                flag = True
            try:
                ind = rule2[h].index('TIN')
            except ValueError:
                ind = rule2[h].index('sum')
                tinflag = True
            if tinflag == False:
                tin = int(rule2[h][ind + 4])
            else:
                tin = 8  # sum
            ind = rule2[h].index('<=')
            threshold = int(float(rule2[h][ind + 3:]))
            intersections[tin].append((threshold, flag))  # low, up, flag
            h = h + 1

        for j in range(len(intersections)):
            if len(intersections[j]) > 0:
                falses = []
                trues = []
                for h in range(len(intersections[j])):
                    if intersections[j][h][1] == False:  # no Not
                        falses.append(intersections[j][h][0])
                    else:  # Not
                        trues.append(intersections[j][h][0])

                flagthreshold = False
                try:
                    up = min(falses)
                except ValueError:
                    up = 400
                    up = tinthresholds[j]
                    flagthreshold = True

                try:
                    down = max(trues)
                except ValueError:
                    down = 0

                if down != 0:
                    bounds[j].append((max(int(down - 20), 0), min(int(down + 20), tinthresholds[j]), 'lower'))

                if flagthreshold == False:
                    bounds[j].append((max(int(up - 20), 0), min(int(up + 20), tinthresholds[j]), 'upper'))

    for i in range(len(bounds)):
        bounds[i] = list(set(bounds[i]))

    for i in range(len(bounds)):
        if len(bounds[i]) == 0:
            bounds[i].append((0, tinthresholds[i], 'upper'))

    array = [0 for i in range(8)]
    for i in range(len(bounds)):
        rand = random.randint(0, len(bounds[i]) - 1)
        chosen = bounds[i][rand]
        array[i] = random.randint(bounds[i][rand][0], bounds[i][rand][1])

    return array, bounds

def LabelPassorFail(robb):
    if robb >= 0.8:
        return 1 #pass
    else:
        return 0 #fail

def AddToDataFrame(olddata, selectedpoint, allfitness, robb, type, delta, traindelta, surrogate):
    for i in range(len(allfitness)):
        if allfitness[i][0] == -1 or allfitness[i][3] == -1:
            return olddata, True

    label = LabelPassorFail(robb)
    olddata.loc[olddata.shape[0]] = [float(robb), int(label), type, delta, traindelta, surrogate, int(selectedpoint[0]), int(selectedpoint[1]), int(selectedpoint[2]),
                                     int(selectedpoint[3]), int(selectedpoint[4]), int(selectedpoint[5]),
                                     int(selectedpoint[6]), int(selectedpoint[7]),
                                     int(allfitness[0][0]), int(allfitness[1][0]), int(allfitness[2][0]),
                                     int(allfitness[3][0]), int(allfitness[4][0]), int(allfitness[5][0]),
                                     int(allfitness[6][0]), int(allfitness[7][0]),
                                     float(allfitness[0][3]), float(allfitness[1][3]), float(allfitness[2][3]),
                                     float(allfitness[3][3]), float(allfitness[4][3]), float(allfitness[5][3]),
                                     float(allfitness[6][3]), float(allfitness[7][3]),
                                     400, 350, 306, 267, 234, 205, 179, 157]

    return olddata, False


def ChooseTheRangesforNextIter(bounds1, bounds2):
    # bounds1 is from previous iter
    # bounds2 is from the current iter
    lower1 = upper1 = lower2 = upper2 = -1

    if len(bounds1) == 2:
        if bounds1[0][2] == 'lower':
            lower1 = bounds1[0]  # (c1,c2,'lower')
            upper1 = bounds1[1]
        else:
            upper1 = bounds1[0]
            lower1 = bounds1[1]

    else:
        if bounds1[0][2] == 'lower':
            lower1 = bounds1[0]
        else:
            upper1 = bounds1[0]

    if len(bounds2) == 2:
        if bounds2[0][2] == 'lower':
            lower2 = bounds2[0]  # (c1,c2,'lower')
            upper2 = bounds2[1]
        else:
            upper2 = bounds2[0]
            lower2 = bounds2[1]
    else:
        if bounds2[0][2] == 'lower':
            lower2 = bounds2[0]
        else:
            upper2 = bounds2[0]

    finalbound = []

    #check for default
    if upper2 != -1 and upper2[1] - upper2[0] > 40:
        if lower1 != -1:
            finalbound.append(lower1)
        if upper1 != -1:
            finalbound.append(upper1)

        return finalbound
    # checking process
    # check subset of lowers

    if lower1 != -1 and lower2 != -1:
        if lower1[0] >= lower2[0] and lower1[1] <= lower2[1]:
            finalbound.append(lower1)
        else:
            finalbound.append(lower2)
    elif lower1 == -1 and lower2 != -1:
        finalbound.append(lower2)

    if upper1 != -1 and upper2 != -1:
        if upper1[0] >= upper2[0] and upper1[1] <= upper2[1]:
            finalbound.append(upper1)
        else:
            finalbound.append(upper2)
    elif upper1 == -1 and upper2 != -1:
        finalbound.append(upper2)

    return finalbound


def WriteToExcel(data,name, s):
    data.to_excel(name + s + ".xlsx")

def GenerateArrayFromRanges(bounds):
    array = [0 for i in range(8)]
    for i in range(len(bounds)):
        rand = random.randint(0, len(bounds[i]) - 1)
        chosen = bounds[i][rand]
        array[i] = random.randint(bounds[i][rand][0], bounds[i][rand][1])

    return array

def ComputeDistance(point, selectedpoints):
    distances = []
    for i in range(len(selectedpoints)):
        d = np.linalg.norm(np.array(point) - np.array(selectedpoints[i]))
        distances.append(d)

    return min(distances)

def GeneratePointsbyAdaptiveRandomSearch(finalboundaries): #newbounds
    selectedpoints = []
    flag = False
    j = 0
    newarray = GenerateArrayFromRanges(finalboundaries)
    GenerateTraffic(newarray, j)
    time.sleep(10)
    RunDpinger()
    time.sleep(41)
    KillDpinger()
    time.sleep(90)
    allfitness = Fitness()  # all
    allfitness.reverse()
    for i in range(len(allfitness)):
        if allfitness[i][0] == -1 or allfitness[i][3] == -1:
            flag = True

    while flag == True:
        flag = False
        newarray = GenerateArrayFromRanges(finalboundaries)
        GenerateTraffic(newarray, j)
        time.sleep(10)
        RunDpinger()
        time.sleep(41)
        KillDpinger()
        time.sleep(90)
        allfitness = Fitness()  # all
        allfitness.reverse()
        for i in range(len(allfitness)):
            if allfitness[i][0] == -1 or allfitness[i][3] == -1:
                flag = True
                break

    fitnessofselectedpoints = []
    fitnessofselectedpoints.append(allfitness)
    flag = False
    selectedpoints.append(newarray)

    while len(selectedpoints) < 5:
        distancesofallpoints = []
        randompoints = []
        for i in range(10):
            randompoints.append(GenerateArrayFromRanges(finalboundaries))

        for i in range(len(randompoints)):
            minn = ComputeDistance(randompoints[i], selectedpoints)
            distancesofallpoints.append(minn)

        newselectedpoint = randompoints[distancesofallpoints.index(max(distancesofallpoints))]

        flag = False
        GenerateTraffic(newselectedpoint, j)
        time.sleep(10)
        RunDpinger()
        time.sleep(41)
        KillDpinger()
        time.sleep(90)
        allfitness = Fitness()  # all
        allfitness.reverse()
        for i in range(len(allfitness)):
            if allfitness[i][0] == -1 or allfitness[i][3] == -1:
                flag = True

        if flag == False:
            selectedpoints.append(newselectedpoint)
            fitnessofselectedpoints.append(allfitness)

    return selectedpoints, fitnessofselectedpoints

def Robustness(allfitness):

    #if the data is faulty
    for i in range(len(allfitness)):
        if allfitness[i][0] == -1 or allfitness[i][3] == -1:
            return -1, True

    #normalize and save into new array(mos):
    mos = [0 for i in range(8)]

    for i in range(len(allfitness)):
        mos[i] = allfitness[i][3] / (allfitness[i][3] + 1)

    #calculate robustness
    if mos[4] < 0.8:
        return mos[4], False
    elif mos[4] >= 0.8 and mos[3] < 0.8:
        return 1 + mos[3], False
    elif mos[4] >= 0.8 and mos[3] >= 0.8 and mos[2] < 0.8:
        return 2 + mos[2], False
    elif mos[4] >= 0.8 and mos[3] >= 0.8 and mos[2] >= 0.8 and mos[1] < 0.75:
        return 3 + mos[1], False
    elif mos[4] >= 0.8 and mos[3] >= 0.8 and mos[2] >= 0.8 and mos[1] >= 0.75 and mos[0] < 0.66:
        return 4 + mos[0], False
    elif mos[4] >= 0.8 and mos[3] >= 0.8 and mos[2] >= 0.8 and mos[1] >= 0.75 and mos[0] >= 0.66:
        return 5, False

def InitialGeneratePoint():

    randomnuttcparray = [0 for i in range(8)]

    randomnuttcparray[0] = random.randint(0,400)
    randomnuttcparray[1] = random.randint(0,350)
    randomnuttcparray[2] = random.randint(0,306)
    randomnuttcparray[3] = random.randint(0,267)
    randomnuttcparray[4] = random.randint(0,234)
    randomnuttcparray[5] = random.randint(0,205)
    randomnuttcparray[6] = random.randint(0,179)
    randomnuttcparray[7] = random.randint(0,157)

    return randomnuttcparray











CS = [8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1]

os.system(
    "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes tc qdisc replace dev eth1 root handle 1: cake ether-vlan nat diffserv8 bandwidth " + CakeBandwidth + "mbit flowblind ack-filter-aggressive no-split-gso; exit; exit; exec bash'")

os.system(
    "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes tc qdisc del dev eth1 root; exit; exit; exec bash'")
os.system(
    "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes tc qdisc replace dev eth1 root handle 1: cake ether-vlan nat diffserv8 bandwidth " + CakeBandwidth + "mbit flowblind ack-filter-aggressive no-split-gso; exit; exit; exec bash'")

ApplyCandidateSol(CS)

time.sleep(2)

allbounds = pd.DataFrame(
    columns=['TIN 0 Range', 'TIN 1 Range', 'TIN 2 Range', 'TIN 3 Range', 'TIN 4 Range', 'TIN 5 Range',
             'TIN 6 Range', 'TIN 7 Range'])
selectedbounds = pd.DataFrame(
    columns=['TIN 0 Range', 'TIN 1 Range', 'TIN 2 Range', 'TIN 3 Range', 'TIN 4 Range', 'TIN 5 Range',
             'TIN 6 Range', 'TIN 7 Range'])

olddata = pd.DataFrame(columns=['Robustness','Label', 'Type', 'Delta', 'Train Delta', 'Surrogate', 'TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW',
                                    'TIN 6 Req-BW', 'TIN 7 Req-BW', 'TIN 0 Latency', 'TIN 1 Latency', 'TIN 2 Latency',
                                    'TIN 3 Latency', 'TIN 4 Latency', 'TIN 5 Latency', 'TIN 6 Latency', 'TIN 7 Latency',
                                    'TIN 0 MOS', 'TIN 1 MOS', 'TIN 2 MOS', 'TIN 3 MOS', 'TIN 4 MOS', 'TIN 5 MOS',
                                    'TIN 6 MOS', 'TIN 7 MOS', 'TIN 0 Thresh', 'TIN 1 Thresh', 'TIN 2 Thresh',
                                    'TIN 3 Thresh', 'TIN 4 Thresh', 'TIN 5 Thresh', 'TIN 6 Thresh', 'TIN 7 Thresh'])

#First 50 simulated points
i = 0
type = 'SIM'
while len(olddata.index) < 50:
    newarray = InitialGeneratePoint() #1 array
    DeleteFiles()
    DeleteRandomTrafficFiles()

    GenerateTraffic(newarray, i)
    time.sleep(10)
    RunDpinger()
    time.sleep(41)
    KillDpinger()
    time.sleep(90)
    allfitness = Fitness()  # get values of latency and mos
    allfitness.reverse()
    rob = Robustness(allfitness) #get the value of quantitative measure
    rob = round(rob[0],1)
    olddata, flag = AddToDataFrame(olddata,newarray,allfitness,rob,'SIM', 0,0, 'Without')

    i = i + 1


#Apply SMOTE
sm = SMOTE(sampling_strategy = 'minority', k_neighbors = 2)
X = []
y = []
for i in range(len(olddata.index)):
    X.append(olddata.loc[i, ['TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW',
                                'TIN 6 Req-BW', 'TIN 7 Req-BW']].values.tolist())
    y.append(olddata.loc[i, "Label"])
new_X, new_y = sm.fit_resample(X, y)

#simulate the points generated by SMOTE
ones = 0
zeros = 0
for i in range(len(olddata.index)):
    if olddata.loc[i, "Label"] == 1:
        ones = ones + 1 #number of 1s in first 50 points
    elif olddata.loc[i, "Label"] == 0:
        zeros = zeros + 1

if zeros > ones:
    c = ones
    for i in range(len(new_X)-1, len(new_X)-(50-c), -1):
        flag = True
        while flag == True:
            GenerateTraffic(new_X[i], i)
            time.sleep(10)
            RunDpinger()
            time.sleep(41)
            KillDpinger()
            time.sleep(90)
            allfitness = Fitness()  # get values of latency and mos
            allfitness.reverse()
            rob = Robustness(allfitness) #get the value of quantitative measure
            if rob[1] == False:
                flag = False
                rob = round(rob[0],1)
                olddata, flagg = AddToDataFrame(olddata,new_X[i],allfitness,rob,'SIM', 0,0, 'SMOTE')
else:
    c = zeros
    for i in range(0, len(new_X)-(50-c)):
        flag = True
        while flag == True:
            GenerateTraffic(new_X[i], i)
            time.sleep(10)
            RunDpinger()
            time.sleep(41)
            KillDpinger()
            time.sleep(90)
            allfitness = Fitness()  # get values of latency and mos
            allfitness.reverse()
            rob = Robustness(allfitness) #get the value of quantitative measure
            if rob[1] == False:
                flag = False
                rob = round(rob[0],1)
                olddata, flagg = AddToDataFrame(olddata,new_X[i],allfitness,rob,'SIM', 0,0, 'SMOTE')

#add the remaining points to make initial 100 points
i = 0
while len(olddata.index) < 100:
     newarray = InitialGeneratePoint() #1 array
     DeleteFiles()
     DeleteRandomTrafficFiles()

     GenerateTraffic(newarray, i)
     time.sleep(10)
     RunDpinger()
     time.sleep(41)
     KillDpinger()
     time.sleep(90)
     allfitness = Fitness()  # get values of latency and mos
     allfitness.reverse()
     rob = Robustness(allfitness) #get the value of quantitative measure
     rob = round(rob[0],1)
     olddata, flag = AddToDataFrame(olddata,newarray,allfitness,rob,'SIM', 0, 0, 'Without')

     i = i + 1

############################################THE MAIN PROCESS FOR GETTING THE REG TREE #############################

listofallpaths, clf = MainProduceDecisionTree(olddata, 0, -1)

separatedclasses = SeparateGoodandBad()
newarray, newbounds = ExtractBounds(separatedclasses, listofallpaths)
allbounds = AddBoundstoDataFrame(allbounds,newbounds)

lastselectedbounds = newbounds
selectedbounds = AddBoundstoDataFrame(selectedbounds, newbounds)

i = 1

while len(olddata.index) < 150:

    newarray, fitnessofnewarray = GeneratePointsbyAdaptiveRandomSearch(newbounds)

    DeleteFiles()
    DeleteRandomTrafficFiles()
    for j in range(len(newarray)):
        rob = Robustness(fitnessofnewarray[j])

        if rob[1] == False:

            rob = round(rob[0],1)
            olddata, flag = AddToDataFrame(olddata,newarray[j],fitnessofnewarray[j],rob, 'SIM', 0, 0, 'RegTree')


    listofallpaths, clf = MainProduceDecisionTree(olddata, 1, clf)

    separatedclasses = SeparateGoodandBad()
    _, newbounds = ExtractBounds(separatedclasses, listofallpaths)
    allbounds = AddBoundstoDataFrame(allbounds,newbounds)


    for j in range(len(lastselectedbounds)):
        lastselectedbounds[j] = ChooseTheRangesforNextIter(lastselectedbounds[j],newbounds[j])

    selectedbounds = AddBoundstoDataFrame(selectedbounds, lastselectedbounds)

    DeleteRandomTrafficFiles()
    i = i + 1


