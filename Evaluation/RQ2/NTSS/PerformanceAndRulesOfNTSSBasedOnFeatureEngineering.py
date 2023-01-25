import pandas as pd
import wittgenstein as lw
from skopt import BayesSearchCV
from sklearn.metrics import accuracy_score, confusion_matrix, classification_report, balanced_accuracy_score
import pandas as pd

def FeatureEngineering():

  tins = ['TIN'+str(i) for i in range(8)]


  features = []

  for k in range(len(tins) - 1): #2
      for h in range(k+1, len(tins)):

          feature = tins[k] + '+' + tins[h]
          features.append(feature)

  for k in range(len(features)): #3

      try:
          indexx = int(features[k][-1])+1
          for h in range(indexx, len(tins)):
            feature = features[k] + '+' + tins[h]
            features.append(feature)

      except:
        continue

  for k in range(len(features)): #4

      if features[k].count('+') == 2:

          try:
              indexx = int(features[k][-1])+1
              for h in range(indexx, len(tins)):
                feature = features[k] + '+' + tins[h]
                features.append(feature)


          except:
            continue

  for k in range(len(features)): #5

      if features[k].count('+') == 3:

          try:
              indexx = int(features[k][-1])+1
              for h in range(indexx, len(tins)):
                feature = features[k] + '+' + tins[h]
                features.append(feature)

          except:
            continue


  for k in range(len(features)): #6

      if features[k].count('+') == 4:

          try:
              indexx = int(features[k][-1])+1
              for h in range(indexx, len(tins)):
                feature = features[k] + '+' + tins[h]
                features.append(feature)


          except:
            continue

  for k in range(len(features)): #7

      if features[k].count('+') == 5:

          try:
              indexx = int(features[k][-1])+1
              for h in range(indexx, len(tins)):
                feature = features[k] + '+' + tins[h]
                features.append(feature)

          except:
            continue

  for k in range(len(features)): #8

      if features[k].count('+') == 6:

          try:
              indexx = int(features[k][-1])+1
              for h in range(indexx, len(tins)):
                feature = features[k] + '+' + tins[h]
                features.append(feature)

          except:
            continue

  return features




def Preprocess(data):

  tins = ['TIN'+str(i) for i in range(8)]

  for i in range(len(data.index)):
      features = []

      data.loc[i, 'TIN0'] = data.loc[i, 'TIN 0 Req-BW']
      data.loc[i, 'TIN1'] = data.loc[i, 'TIN 1 Req-BW']
      data.loc[i, 'TIN2'] = data.loc[i, 'TIN 2 Req-BW']
      data.loc[i, 'TIN3'] = data.loc[i, 'TIN 3 Req-BW']
      data.loc[i, 'TIN4'] = data.loc[i, 'TIN 4 Req-BW']
      data.loc[i, 'TIN5'] = data.loc[i, 'TIN 5 Req-BW']
      data.loc[i, 'TIN6'] = data.loc[i, 'TIN 6 Req-BW']
      data.loc[i, 'TIN7'] = data.loc[i, 'TIN 7 Req-BW']

      for k in range(len(tins) - 1): #2
          for h in range(k+1, len(tins)):

              feature = tins[k] + '+' + tins[h]
              features.append(feature)

              data.loc[i, feature] = data.loc[i, tins[k]] + data.loc[i, tins[h]]

      for k in range(len(features)): #3

          try:
              indexx = int(features[k][-1])+1
              for h in range(indexx, len(tins)):
                feature = features[k] + '+' + tins[h]
                features.append(feature)

                data.loc[i, feature] = data.loc[i, features[k]] + data.loc[i, tins[h]]
          except:
            continue

      for k in range(len(features)): #4

          if features[k].count('+') == 2:

              try:
                  indexx = int(features[k][-1])+1
                  for h in range(indexx, len(tins)):
                    feature = features[k] + '+' + tins[h]
                    features.append(feature)

                    data.loc[i, feature] = data.loc[i, features[k]] + data.loc[i, tins[h]]
              except:
                continue

      for k in range(len(features)): #5

          if features[k].count('+') == 3:

              try:
                  indexx = int(features[k][-1])+1
                  for h in range(indexx, len(tins)):
                    feature = features[k] + '+' + tins[h]
                    features.append(feature)

                    data.loc[i, feature] = data.loc[i, features[k]] + data.loc[i, tins[h]]
              except:
                continue


      for k in range(len(features)): #6

          if features[k].count('+') == 4:

              try:
                  indexx = int(features[k][-1])+1
                  for h in range(indexx, len(tins)):
                    feature = features[k] + '+' + tins[h]
                    features.append(feature)

                    data.loc[i, feature] = data.loc[i, features[k]] + data.loc[i, tins[h]]
              except:
                continue

      for k in range(len(features)): #7

          if features[k].count('+') == 5:

              try:
                  indexx = int(features[k][-1])+1
                  for h in range(indexx, len(tins)):
                    feature = features[k] + '+' + tins[h]
                    features.append(feature)

                    data.loc[i, feature] = data.loc[i, features[k]] + data.loc[i, tins[h]]
              except:
                continue

      for k in range(len(features)): #8

          if features[k].count('+') == 6:

              try:
                  indexx = int(features[k][-1])+1
                  for h in range(indexx, len(tins)):
                    feature = features[k] + '+' + tins[h]
                    features.append(feature)

                    data.loc[i, feature] = data.loc[i, features[k]] + data.loc[i, tins[h]]
              except:
                continue

  return data


def GetConfSamp(a, b, con, indexx, feat):

  data = pd.read_excel("ensrun"+str(indexx+1)+".xlsx")

  data = Preprocess(data)

  if con == "between":

    count = 0
    trues = 0
    for i in range(len(data.index)):

      if  a<= data.loc[i, feat]<=b:
        count = count + 1
        if data.loc[i, 'Label'] == 0: #class 0
          trues = trues + 1

    return (trues/count)*100, count

  elif con == "greater":

    count = 0
    trues = 0
    for i in range(len(data.index)):

      if  data.loc[i, feat]>=a:
        count = count + 1
        if data.loc[i, 'Label'] == 0: #class 0
          trues = trues + 1

    return (trues/count)*100, count

  elif con == "smaller":

    count = 0
    trues = 0
    for i in range(len(data.index)):

      if  data.loc[i, feat]<=a:
        count = count + 1
        if data.loc[i, 'Label'] == 0: #class 0
          trues = trues + 1

    return (trues/count)*100, count


def ExtractBounds(rule):

  if rule.find('>') != -1:

    return float(rule[rule.find('=')+2:-1]), "", "greater"

  elif rule.find('<') != -1:

    return float(rule[rule.find('<')+1 : -1]), "", "smaller"

  else:

    return float(rule[rule.find('=')+1:rule.find('-')]), float(rule[rule.find('-')+1:-1]), "between"


def CalculateMetrics(clf, data1, column):

  X_train, y_train = data1.loc[:,column], data1.loc[:,'Label']

  try:
    clf.fit(X_train, y_train, pos_class = 1)
  except:
    clf = lw.RIPPER(random_state = 0)
    clf.fit(X_train, y_train, pos_class = 1)

  testdata = pd.read_excel('testsetfromrandom.xlsx')

  testdata = Preprocess(testdata)

  ypreds = []

  for l in range(len(testdata.index)):

    X_test, y_test = testdata.loc[l:l, column], testdata.loc[l:l, 'Label']

    y_pred = clf.predict(X_test)

    ypreds.append(y_pred)


  y_test = testdata.loc[:, 'Label']

  accuracy = accuracy_score(y_test, ypreds)

  tn, fp, fn, tp = confusion_matrix(y_test, ypreds).ravel()

  precisionpass = (tp) / (tp + fp)
  recallpass = (tp) / (tp + fn)

  precisionfail = (tn) / (tn + fn)
  recallfail = (tn) / (tn + fp)

  return accuracy, precisionfail, recallfail, precisionpass, recallpass


###############MAIN LOOP########################################

metricsresults = pd.DataFrame(columns= ['accuracySA', 'precisionfailSA', 'recallfailSA', 'precisionpassSA', 'recallpassSA',
                                        'accuracyRand', 'precisionfailRand', 'recallfailRand', 'precisionpassRand', 'recallpassRand',
                                        'accuracyRegTree', 'precisionfailRegTree', 'recallfailRegTree', 'precisionpassRegTree', 'recallpassRegTree',
                                        'accuracyLogReg', 'precisionfailLogReg', 'recallfailLogReg', 'precisionpassLogReg', 'recallpassLogReg',])

rulesresults = pd.DataFrame(columns = ['RUN'+str(i) for i in range(1, 11)])

columns = FeatureEngineering()

for column in columns:


  print('new column is: ', column)
  rules= []

  for i in range(1, 11):

    ensdata = pd.read_excel("ensrun"+str(i)+".xlsx")

    ensdata = Preprocess(ensdata)

    randdata = pd.read_excel("randrun"+str(i)+".xlsx")

    randdata = randdata[:150]

    randdata = Preprocess(randdata)

    regtreedata = pd.read_excel("regtreerun"+str(i)+".xlsx")

    regtreedata = regtreedata.drop(columns = 'Unnamed: 0', axis = 1)

    regtreedata = Preprocess(regtreedata)

    logregdata = pd.read_excel("logregrun"+str(i)+".xlsx")

    logregdata = logregdata.drop(columns = 'Unnamed: 0', axis = 1)

    logregdata = Preprocess(logregdata)

    alldata = ensdata.append(randdata)
    alldata = alldata.append(regtreedata)
    alldata = alldata.append(logregdata)

    X_train, y_train = alldata.loc[:,column], alldata.loc[:,'Label']

    drmodel = BayesSearchCV(lw.RIPPER(random_state = 0), {'prune_size': [0.1, 0.8], 'k': [1, 3]}, optimizer_kwargs = {'acq_func': 'EI'})

    try:
      drmodel.fit(X_train, y_train, pos_class = 1)
      params = drmodel.best_params_
      clf = lw.RIPPER(**params)
    except:
      clf = lw.RIPPER(random_state = 0) #if bayesian cannot find a good model, i just use default



    accuracySA, precisionfailSA, recallfailSA, precisionpassSA, recallpassSA = CalculateMetrics(clf, ensdata, column) #get metrics for SA

    accuracyRAND, precisionfailRAND, recallfailRAND, precisionpassRAND, recallpassRAND = CalculateMetrics(clf, randdata, column) #get metrics for RAND

    accuracyREGTREE, precisionfailREGTREE, recallfailREGTREE, precisionpassREGTREE, recallpassREGTREE = CalculateMetrics(clf, regtreedata, column) #get metrics for REGTREE

    accuracyLOGREG, precisionfailLOGREG, recallfailLOGREG, precisionpassLOGREG, recallpassLOGREG = CalculateMetrics(clf, logregdata, column) #get metrics for LOGREG

    metricsresults.loc[metricsresults.shape[0]] = [accuracySA, precisionfailSA, recallfailSA, precisionpassSA, recallpassSA,
                                                   accuracyRAND, precisionfailRAND, recallfailRAND, precisionpassRAND, recallpassRAND,
                                                   accuracyREGTREE, precisionfailREGTREE, recallfailREGTREE, precisionpassREGTREE, recallpassREGTREE,
                                                   accuracyLOGREG, precisionfailLOGREG, recallfailLOGREG, precisionpassLOGREG, recallpassLOGREG]


    ###NOW GET THE RULES OF SA#######

    X_train, y_train = ensdata.loc[:,column], ensdata.loc[:,'Label']
    clf.fit(X_train, y_train, pos_class= 0)

    clf.ruleset_.out_pretty()

    rules.append(clf.ruleset_)



  lastindex = rulesresults.shape[0]

  for k in range(len(rules)):
    listt = []
    for j in range(len(rules[k])):

      listt.append(str(rules[k][j]))

      c = str(rules[k][j])
      a, b, con = ExtractBounds(c)

      conf, samples = GetConfSamp(a, b, con, k, column)

      rulesresults.loc[j + lastindex, 'RUN'+str(k+1)] = str((conf, samples))

    rulesresults.loc[j+lastindex+1, 'RUN'+str(k+1)] = str(listt)

metricsresults.to_excel("metricsresults.xlsx")
rulesresults.to_excel("rulesresults.xlsx")
