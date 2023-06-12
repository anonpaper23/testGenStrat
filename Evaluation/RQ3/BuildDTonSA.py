from sklearn.tree import DecisionTreeClassifier
import pandas as pd

finall = pd.DataFrame(columns = ['Accuracy', 'Fail recall', 'Fail precision'])

numofruns = 20
model = "autopilot_R12_1_regression_3500_ensemble_"  #for NTSS use "ensrun"
test = "autopilot_R12_1_verify_newnew.csv" #testset  for NTSS use "NTSS_testset.xlsx"

for i in range(1, numofruns):

  data= pd.read_csv(model+str(i)+'.csv')

  X_train, y_train = data.iloc[:, 5:], data.iloc[:, 1]
  testset = pd.read_csv(test)
  X_test , y_test = testset.iloc[:, 2:], testset.iloc[:, 1]
  numberoffails = data['Label'].value_counts()[0]
  datasetsize = len(data.index)
  clf = DecisionTreeClassifier(min_samples_leaf= 1, min_samples_split=2, max_depth = 5, max_features=None ,random_state = 0, class_weight= {0: 1/numberoffails ,
                                                                                                                                            1: 1/(datasetsize - numberoffails)})
  clf.fit(X_train, y_train)

  y_pred = clf.predict(X_test)
  sim_report = classification_report(y_test, y_pred,output_dict=True)
  sim_0recall = sim_report['0']['recall']
  sim_0precision = sim_report['0']['precision']
  sim_1recall = sim_report['1']['recall']
  sim_1precision = sim_report['1']['precision']
  sim_acc = accuracy_score(y_test, y_pred)
  sim_balacc = balanced_accuracy_score(y_test,y_pred)  

  filename = 'finalized_model'+str(i)+'.pickle'
  pickle.dump(clf, open(filename, 'wb'))
  
  print(sim_report)
  
  finall.loc[finall.shape[0]] = [sim_acc, sim_0recall, sim_0precision ]

finall.to_excel('final results.xlsx')
