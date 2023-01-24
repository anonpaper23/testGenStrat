from sklearn.linear_model import LogisticRegression
import time
import random
import pandas as pd
import numpy as np
from configparser import ConfigParser
from imblearn.over_sampling import SMOTE
import os
from skopt import BayesSearchCV
from skopt.space import Real, Categorical, Integer
from math import log

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

def LogReg(log_clf, points,p):

  mergedList = []
  distForEachInput = []
  numOfInputs = len(points)
  for i in range(numOfInputs):
      lengthOfInput = len(points[0])

      logodd = log(p/(1-p))
      numerator = log_clf.intercept_[0] - logodd
      for j in range(lengthOfInput):
          eachInputProduct = points[i][j]*log_clf.coef_[0][j]
          numerator = numerator+eachInputProduct
      sq_coeff = 0
      for k in range(lengthOfInput):
          eachsqCoeffProduct = log_clf.coef_[0][k]**2
          sq_coeff = sq_coeff + eachsqCoeffProduct
      denominator = np.sqrt(sq_coeff)
      dist = abs(numerator)/denominator
      distForEachInput.append(dist)
      mergedList.append([(dist,points[i])])
  sortedMergedList = sorted(mergedList,key=lambda x: x[0])
  finalList = []

  for i in range(len(sortedMergedList)):
      finalList.append(sortedMergedList[i][0][1])

  return finalList

def NumPassInDataset(olddata):

    passs = 0
    for i in range(len(olddata.index)):
        if olddata.loc[i, 'Label'] == 1:
            passs = passs + 1

    return passs/len(olddata.index)

def SplitData(data):
  
  X = []
  y = []

  for i in range(len(data.index)):
    listt = []
    listt.append(data.loc[i, 'TIN 0 Req-BW'])
    listt.append(data.loc[i, 'TIN 1 Req-BW'])
    listt.append(data.loc[i, 'TIN 2 Req-BW'])
    listt.append(data.loc[i, 'TIN 3 Req-BW'])
    listt.append(data.loc[i, 'TIN 4 Req-BW'])
    listt.append(data.loc[i, 'TIN 5 Req-BW'])
    listt.append(data.loc[i, 'TIN 6 Req-BW'])
    listt.append(data.loc[i, 'TIN 7 Req-BW'])

    X.append(listt)

    listt = []
    listt.append(data.loc[i, 'Label'])
    y.append(listt)

  return X, y

import random

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

###################################LOG REG MAIN LOOP###################################################

for i in range(10): #number of times to retrain the model

  if i == 0:

      logregmodel = BayesSearchCV(LogisticRegression(random_state=0), {'penalty': Categorical(['none','l2']),'solver' : Categorical(['newton-cg', 'lbfgs','sag','saga']), 'C' : Real(0.01, 100)}, n_iter = 50, optimizer_kwargs= {'acq_func' : 'EI'})
      X_train, y_train = olddata.loc[:,['TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW',
                                'TIN 6 Req-BW', 'TIN 7 Req-BW']].values, olddata.loc[:,"Label"].values.ravel()
      y_train=y_train.astype('int')
      logregmodel.fit(X_train,y_train)
      log_clf = logregmodel.best_estimator_

  else:
      X_train, y_train = olddata.loc[:,['TIN 0 Req-BW', 'TIN 1 Req-BW', 'TIN 2 Req-BW', 'TIN 3 Req-BW', 'TIN 4 Req-BW', 'TIN 5 Req-BW',
                                'TIN 6 Req-BW', 'TIN 7 Req-BW']].values, olddata.loc[:,"Label"].values.ravel()
      y_train=y_train.astype('int')
      log_clf.fit(X_train, y_train)

  for j in range(5):
    points = [InitialGeneratePoint() for k in range(5)]
    p = NumPassInDataset(olddata)
    sortedlist = LogReg(log_clf, points,p)
    winner = sortedlist[0] #choose the closest point

    flag = True
    while flag == True:
        GenerateTraffic(winner, i)
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
            olddata, flagg = AddToDataFrame(olddata,winner,allfitness,rob,'SIM', 0,0, 'LogReg')


olddata.to_excel("finaldatalogreg.xlsx")
