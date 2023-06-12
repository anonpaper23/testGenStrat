import random
import time
import os
from sklearn.tree import _tree
from sklearn import tree
import numpy as np
import pandas as pd
from skopt import BayesSearchCV
from skopt.space import Integer
import math
from configparser import ConfigParser
import pickle

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


def simplify(bounds):

    tinthresholds = [400, 350, 306, 267, 234, 205, 179, 157]

    for i in range(len(bounds)):
        if len(bounds[i]["greater"]) != 0:
            bounds[i]["greater"] = max(bounds[i]["greater"])
        else:
            bounds[i]["greater"] = 0

        if len(bounds[i]["less"]) != 0:
            bounds[i]["less"] = min(bounds[i]["less"])
        else:
            bounds[i]["less"] = tinthresholds[i]

    return bounds

def generate_data_from_bounds(bounds):

    bounds = simplify(bounds)
    
    test = [0 for i in range(8)]

    for i in range(len(bounds)):
        test[i] = random.randint(bounds[i]["greater"], bounds[i]["less"])

    return test

def extract_bounds_from_rule(r):

    tins = ['TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW', 'TIN 6 Req-BW', 'TIN 7 Req-BW']

    #a list of bounds of each tin shown as tuples
    ranges = [{"less":[], "greater":[]} for i in range(8)]

    

    for i in range(len(tins)):

        if r.find(tins[i]) != -1:
            occurances = r.count(tins[i])
            copyrr = r

            for j in range(occurances):
                g = 1000
                copyr = copyrr
                indexx = copyrr.find(tins[i])
                copyr = copyr[indexx: indexx + len(tins[i]) + 10]

                if copyr.find('>') != -1: #greater
                    g = copyr.find('>')
                    p = copyr.find(')')
                    #ranges[i].append(('greater', int(float(copyr[g + 2: p]))))
                    ranges[i]["greater"].append(int(float(copyr[g + 2: p])))

                elif copyr.find('<=') != -1:
                    g = copyr.find('<=')
                    p = copyr.find(')')
                    #ranges[i].append(('less', int(float(copyr[g + 3: p]))))
                    ranges[i]["less"].append(int(float(copyr[g + 3: p])))

                copyrr = copyrr[indexx + len(tins[i]): ]
                
    return ranges

def get_rules(tree, feature_names, class_names): #this function is taken from: https://mljar.com/blog/extract-rules-decision-tree/
    tree_ = tree.tree_
    feature_name = [
        feature_names[i] if i != _tree.TREE_UNDEFINED else "undefined!"
        for i in tree_.feature
    ]

    paths = []
    path = []

    def recurse(node, path, paths):

        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            name = feature_name[node]
            threshold = tree_.threshold[node]
            p1, p2 = list(path), list(path)
            p1 += [f"({name} <= {np.round(threshold, 3)})"]
            recurse(tree_.children_left[node], p1, paths)
            p2 += [f"({name} > {np.round(threshold, 3)})"]
            recurse(tree_.children_right[node], p2, paths)
        else:
            path += [(tree_.value[node], tree_.n_node_samples[node])]
            paths += [path]

    recurse(0, path, paths)

    # sort by samples count
    samples_count = [p[-1][1] for p in paths]
    ii = list(np.argsort(samples_count))
    paths = [paths[i] for i in reversed(ii)]

    rules = []
    for path in paths:
        rule = "if "

        for p in path[:-1]:
            if rule != "if ":
                rule += " and "
            rule += str(p)
        rule += " then "
        if class_names is None:
            rule += "response: "+str(np.round(path[-1][0][0][0],3))
        else:
            classes = path[-1][0][0]
            l = np.argmax(classes)
            rule += f"class: {class_names[l]} (proba: {np.round(100.0*classes[l]/np.sum(classes),2)}%)"
        rule += f" | based on {path[-1][1]:,} samples"
        rules += [rule]

    return rules

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

def LabelPassorFail(robb):
    if robb >= 0.8:
        return 1 #pass
    else:
        return 0 #fail

start = time.time()
CS = [8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1]

os.system(
    "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes tc qdisc replace dev eth1 root handle 1: cake ether-vlan nat diffserv8 bandwidth " + CakeBandwidth + "mbit flowblind ack-filter-aggressive no-split-gso; exit; exit; exec bash'")

os.system(
    "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes tc qdisc del dev eth1 root; exit; exit; exec bash'")
os.system(
    "gnome-terminal -- bash -c 'ssh " + OpenWrtUser + "@" + OpenWrtIp + " -yes tc qdisc replace dev eth1 root handle 1: cake ether-vlan nat diffserv8 bandwidth " + CakeBandwidth + "mbit flowblind ack-filter-aggressive no-split-gso; exit; exit; exec bash'")

ApplyCandidateSol(CS)

time.sleep(2)


#read the first loop

specifyrun = 1

data = pd.read_excel('ensrun' + str(specifyrun) + '.xlsx')[:100]
finaldata =  data

#build initial tree

X_train = data.loc[:, ['TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW', 'TIN 6 Req-BW', 'TIN 7 Req-BW']]
y_train = data.Label.values

numberoffails = data["Label"].value_counts()[0]
datasetsize = len(data.index)
dtmodel = tree.DecisionTreeClassifier(min_samples_leaf=1, min_samples_split=2,max_features=None, max_depth=5,random_state = 0, class_weight={0 : 1/numberoffails , 1: 1/(datasetsize - numberoffails) })

dtmodel.fit(X_train, y_train)


initialmodeltraining = time.time()

i = 0
loopcount = 0
while len(finaldata.index) < 150:
    loopcount = loopcount + 1

    rules = get_rules(dtmodel, X_train.columns, ['0','1'])
    for r in rules:
        if len(finaldata.index) > 150:
            break
        else:
            bounds = extract_bounds_from_rule(r)
            test = generate_data_from_bounds(bounds)
            flag = True
            while flag == True:
                GenerateTraffic(test, i)
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
                    i = i + 1
                    rob = round(rob[0],1)
                    label = LabelPassorFail(rob)
                    finaldata.loc[finaldata.shape[0]] = [rob, label, test[0], test[1], test[2], test[3], test[4], test[5], test[6], test[7]]

    X_train = data.loc[:, ['TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW', 'TIN 6 Req-BW', 'TIN 7 Req-BW']]
    y_train = data.Label.values
    dtmodel.fit(X_train, y_train)


#save the trained model
filename = 'SoTArun'+str(specifyrun) + 'model.pickle'
pickle.dump(dtmodel, open(filename, "wb"))

finaldata.to_excel("SoTArun"+str(specifyrun)+".xlsx", index = False)
print('time taken', time.time() - start)

end = time.time()

logtime = pd.read_excel('logtime.xlsx')
logtime.loc[logtime.shape[0]] = [specifyrun, initialmodeltraining - start, end-start, loopcount]

logtime.to_excel('logtime.xlsx', index = False)
