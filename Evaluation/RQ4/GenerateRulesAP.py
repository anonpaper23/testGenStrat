import wittgenstein as lw
from operator import itemgetter
import pandas as pd
from skopt import BayesSearchCV

def GetConfSamp(dictt, indexx, feat):
  
  data = pd.read_csv("autopilot_R1_6_regression_3500_ensemble_"+str(indexx+1)+".csv") #change the file name

  trues = 0
  inrange = 0

  for i in range(len(data.index)):

    count = 0

    for var in dictt:

      if dictt[var][2] == "between":

        if  dictt[var][0]<= data.loc[i, var]<= dictt[var][1]:
          count = count + 1
      
      elif dictt[var][2] == "greater":

        if  data.loc[i, var]>= dictt[var][0]:
          count = count + 1

      elif dictt[var][2] == "smaller":

        if  data.loc[i, var]<= dictt[var][0]:
          count = count + 1 

      elif dictt[var][2] == "equal":
        
        if  data.loc[i, var]== dictt[var][0]:
          count = count + 1 
        
    if count == len(dictt): #all the variables are in the ranges
      inrange = inrange + 1
      if data.loc[i, 'Label'] == 0: #class 0
          trues = trues + 1
    else:
      continue
  
  try:
    return (trues/inrange)*100, inrange
  except:
    return trues, inrange


def ExtractBounds(rule):
  rule = rule[1:-1].split('^')

  dictt = {}

  for i in range(len(rule)):
    if rule[i].find('>') != -1:

      dictt[rule[i][:rule[i].find('=')]] = [float(rule[i][rule[i].find('=')+2 : len(rule[i])]), "", "greater"]

    elif rule[i].find('<') != -1:

      dictt[rule[i][:rule[i].find('=')]] = [float(rule[i][rule[i].find('<')+1 : len(rule[i])]), "", "smaller"]

    elif rule[i].find('-') != -1:

      for k in range(rule[i].find('=')+2, len(rule[i])):
        if rule[i][k] == '-':
          a = rule[i][rule[i].find('=')+1: k]
          k = k
          break

      dictt[rule[i][:rule[i].find('=')]] = [float(a), float(rule[i][k+1:len(rule[i])]), "between"]

    elif rule[i].find('=') != -1:

      dictt[rule[i][:rule[i].find('=')]] = [float(rule[i][rule[i].find('=')+1 : len(rule[i])]), "", "equal"]      
  
  return dictt



rules= []
rulesresults = pd.DataFrame(columns = ['RUN'+str(i) for i in range(1, 21)])

for i in range(1, 21):

  dataens = pd.read_csv("autopilot_R1_6_regression_3500_ensemble_"+str(i)+".csv") #change the file name 
  

  X_train, y_train = dataens.loc[:,["AP_Eng1", "AP_Eng2", "AP_Eng3", "Pwheel1", "Pwheel2", "Pwheel3",  "TurnK1", "TurnK2", "TurnK3", "ALT_Mode1", "ALT_Mode2", "ALT_Mode3"]], dataens.loc[:,'Label']

  drmodel = BayesSearchCV(lw.RIPPER(random_state = 0), {'prune_size': [0.1, 0.8], 'k': [1, 3]}, n_iter = 100, optimizer_kwargs = {'acq_func': 'EI'})


  try:
    drmodel.fit(X_train, y_train, pos_class=0)
    params = drmodel.best_params_

    clf = lw.RIPPER(**params)
    
    X_train, y_train = dataens.loc[:,["AP_Eng1", "AP_Eng2", "AP_Eng3", "Pwheel1", "Pwheel2", "Pwheel3",  "TurnK1", "TurnK2", "TurnK3", "ALT_Mode1", "ALT_Mode2", "ALT_Mode3"]], dataens.loc[:,'Label']
    clf.fit(X_train, y_train)
    

  except:
    clf = lw.RIPPER(random_state = 0) #if bayesian cannot find a good model, i just use default
    X_train, y_train = dataens.loc[:,["AP_Eng1", "AP_Eng2", "AP_Eng3", "Pwheel1", "Pwheel2", "Pwheel3",  "TurnK1", "TurnK2", "TurnK3", "ALT_Mode1", "ALT_Mode2", "ALT_Mode3"]], dataens.loc[:,'Label']
    clf.fit(X_train, y_train, pos_class = 0)
    
  clf.ruleset_.out_pretty()

  rules.append(clf.ruleset_)


lastindex = rulesresults.shape[0]

for k in range(len(rules)):
  listt = []
  for j in range(len(rules[k])):

    listt.append(str(rules[k][j]))

    c = str(rules[k][j])
    dictt = ExtractBounds(c)

    conf, samples = GetConfSamp(dictt, k, ["AP_Eng1", "AP_Eng2", "AP_Eng3", "Pwheel1", "Pwheel2", "Pwheel3",  "TurnK1", "TurnK2", "TurnK3", "ALT_Mode1", "ALT_Mode2", "ALT_Mode3"])

    rulesresults.loc[j + lastindex, 'RUN'+str(k+1)] = str((conf, samples))


  rulesresults.loc[j+lastindex+1, 'RUN'+str(k+1)] = str(listt)


rulesresults.to_excel("rulesresults.xlsx")


