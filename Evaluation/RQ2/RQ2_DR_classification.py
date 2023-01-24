#!/usr/bin/env python
# coding: utf-8

# In[ ]:

import pandas as pd
import numpy as np
import wittgenstein as lw
from skopt import BayesSearchCV
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report,balanced_accuracy_score,accuracy_score
import wittgenstein as lw

import warnings
warnings.filterwarnings('ignore')
#Enter the Simulink model for which RQ2 evaluation needs to be performed. 
sim_name = 'fsm'
fname_path = '../Results/'+sim_name+'/'
#runs, reqs are used to create the filename which is used for training. 
#Therefore, pass the same run values that was generated. I.e that was passed in MATLAB executeXXnewHCR file. 
# Specifically in the "runs" list in MATLAB.
runs = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
#Mention the appropriate req
reqs = ['R1_1']#
data = []

#Function to tune RIPPER using  Bayesian Search
def tune_dr(X,y):
    ripper_clf = lw.RIPPER()
    opt = BayesSearchCV(ripper_clf,{"prune_size": [0.1, 0.8],"k": [1, 3]},random_state=0,verbose=1)
    _ = opt.fit(X, y)
    return opt.best_params_


#For each requirement and for each run,
for req in reqs:
    for run in runs:
        print(req,run)
        #Build the filename including path for each algorithm
        ensfilename = sim_name+'_'+req+'_regression_3500_ensemble_'+str(run)+'.csv'
        rsfilename = sim_name+'_'+req+'_regression_3500_value_'+str(run)+'.csv'
        rslogregfilename = sim_name+'_'+req+'_regression_3500_genLog_'+str(run)+'.csv'
        rsregtreefilename = sim_name+'_'+req+'_regression_3500_genRegTree_'+str(run)+'.csv'
    
        ensPath = fname_path+ensfilename
        rsPath = fname_path+rsfilename
        rslogregPath = fname_path+rslogregfilename
        rsregtreePath = fname_path+rsregtreefilename
        
        row = []
        
        row.append(sim_name)
        row.append(req)
        row.append(run)

        #Read train data for each algorithm
        
        ensData = pd.read_csv(ensPath)
        simData = ensData[ensData['Type'] == 0]
        rsData = pd.read_csv(rsPath)
        rslogregData = pd.read_csv(rslogregPath)
        rsregtreeData = pd.read_csv(rsregtreePath)
        
        #Aggregate data from different algorithms for tuning RIPPER
        frames = [ensData,rsData,rslogregData,rsregtreeData]
        aggregateData = pd.concat(frames)
        
        print(aggregateData.shape)
        
        #Set Test data location. 
        testlocation = '../Data/RQ2TestData/'
        sim600DataFile = sim_name+'_'+req+'_verify_newnew.csv'
        sim600DataLocation = testlocation+sim600DataFile
        #Read test data 
        sim600Data = pd.read_csv(sim600DataLocation)
        
        ens_X = ensData.iloc[:,5:]
        ens_Y = ensData.iloc[:,1]
        
        sim_X = simData.iloc[:,5:]
        sim_Y = simData.iloc[:,1]

        rs_X = rsData.iloc[:,5:] 
        rs_Y = rsData.iloc[:,1]

        rslogreg_X = rslogregData.iloc[:,5:]
        rslogreg_Y = rslogregData.iloc[:,1]
        
        rsregtree_X = rsregtreeData.iloc[:,5:]
        rsregtree_Y = rsregtreeData.iloc[:,1]

        test_X = sim600Data.iloc[:,2:]
        test_Y = sim600Data.iloc[:,1]
        
         #######DR tuning here!#######
        params = tune_dr(aggregateData.iloc[:,5:],aggregateData.iloc[:,1]) #
        print(params)

# =======================================================================
        #Extract num of pass instances for SA-DYN, RS, LR and RT
        pIndex1 = np.where((ensData['Label']==1))
        row.append(ensData.loc[pIndex1].shape[0])
        
        pIndex2 = np.where((rsData['Label']==1)) 
        row.append(rsData.loc[pIndex2].shape[0])
        
        pIndex3 = np.where((rslogregData['Label']==1)) 
        row.append(rslogregData.loc[pIndex3].shape[0])
        
        pIndex4 = np.where((rsregtreeData['Label']==1)) 
        row.append(rsregtreeData.loc[pIndex4].shape[0])

# =======================================================================
        #Extract num of fail instances for SA-DYN, RS, LR and RT
        fIndex1 = np.where((ensData['Label']==0))
        row.append(ensData.loc[fIndex1].shape[0])
        
        fIndex2 = np.where((rsData['Label']==0)) 
        row.append(rsData.loc[fIndex2].shape[0])
        
        fIndex3 = np.where((rslogregData['Label']==0)) 
        row.append(rslogregData.loc[fIndex3].shape[0])
        
        fIndex4 = np.where((rsregtreeData['Label']==0)) 
        row.append(rsregtreeData.loc[fIndex4].shape[0])

# =======================================================================
        #Extract num of pass and fail in test data
        
        testfIndex = np.where((sim600Data['Label']==0))
        row.append(sim600Data.loc[testfIndex].shape[0])
        
        testpIndex = np.where((sim600Data['Label']==1))
        row.append(sim600Data.loc[testpIndex].shape[0])
# =======================================================================

        #Build RIPPER model for each algorithm
         
        ensDR = lw.RIPPER(**params) 
        simDR = lw.RIPPER(**params) 
        rsDR = lw.RIPPER(**params)
        rslogregDR = lw.RIPPER(**params)
        rsregtreeDR = lw.RIPPER(**params)
        
        #Fit RIPPER for each algorithm
        ensDR.fit(ens_X,ens_Y)
        simDR.fit(sim_X,sim_Y)
        rsDR.fit(rs_X,rs_Y)
        rslogregDR.fit(rslogreg_X,rslogreg_Y)
        rsregtreeDR.fit(rsregtree_X,rsregtree_Y)
        # ======================================================================
        #Predict on test data
        ens_preds = ensDR.predict(test_X)
        ens_report = classification_report(test_Y, ens_preds,output_dict=True)
        ens_0recall = ens_report['0']['recall']
        ens_0precision = ens_report['0']['precision']
        ens_1recall = ens_report['1']['recall']
        ens_1precision = ens_report['1']['precision']
        ens_acc = accuracy_score(test_Y, ens_preds)
        ens_balacc = balanced_accuracy_score(test_Y,ens_preds)

        row.append(ens_0recall)
        row.append(ens_0precision)
        row.append(ens_1recall)
        row.append(ens_1precision)
        row.append(ens_acc)
        row.append(ens_balacc)
 
        sim_preds = simDR.predict(test_X)
        sim_report = classification_report(test_Y, sim_preds,output_dict=True)
        sim_0recall = sim_report['0']['recall']
        sim_0precision = sim_report['0']['precision']
        sim_1recall = sim_report['1']['recall']
        sim_1precision = sim_report['1']['precision']
        sim_acc = accuracy_score(test_Y, sim_preds)
        sim_balacc = balanced_accuracy_score(test_Y,sim_preds)
        
        row.append(sim_0recall)
        row.append(sim_0precision)
        row.append(sim_1recall)
        row.append(sim_1precision)
        row.append(sim_acc)
        row.append(sim_balacc)
        
    
        rs_preds = rsDR.predict(test_X)

        rs_report = classification_report(test_Y, rs_preds,output_dict=True)
        rs_0recall = rs_report['0']['recall']
        rs_0precision = rs_report['0']['precision']
        rs_1recall = rs_report['1']['recall']
        rs_1precision = rs_report['1']['precision']
        rs_acc = accuracy_score(test_Y,rs_preds)
        rs_balacc = balanced_accuracy_score(test_Y,rs_preds)

        row.append(rs_0recall)
        row.append(rs_0precision)
        row.append(rs_1recall)
        row.append(rs_1precision)
        row.append(rs_acc)
        row.append(rs_balacc)
          
        rslogreg_preds = rslogregDR.predict(test_X)
        rslogreg_report = classification_report(test_Y, rslogreg_preds,output_dict=True)
        rslogreg_0recall = rslogreg_report['0']['recall']
        rslogreg_0precision = rslogreg_report['0']['precision']
        rslogreg_1recall = rslogreg_report['1']['recall']
        rslogreg_1precision = rslogreg_report['1']['precision']
        rslogreg_acc = accuracy_score(test_Y,rslogreg_preds)
        rslogreg_balacc = balanced_accuracy_score(test_Y,rslogreg_preds)

        row.append(rslogreg_0recall)
        row.append(rslogreg_0precision)
        row.append(rslogreg_1recall)
        row.append(rslogreg_1precision)
        row.append(rslogreg_acc)
        row.append(rslogreg_balacc)
 
        rsregtree_preds = rsregtreeDR.predict(test_X)
        rsregtree_report = classification_report(test_Y, rsregtree_preds,output_dict=True)
        rsregtree_0recall = rsregtree_report['0']['recall']
        rsregtree_0precision = rsregtree_report['0']['precision']
        rsregtree_1recall = rsregtree_report['1']['recall']
        rsregtree_1precision = rsregtree_report['1']['precision']
        rsregtree_acc = accuracy_score(test_Y,rsregtree_preds)
        rsregtree_balacc = balanced_accuracy_score(test_Y,rsregtree_preds)

        row.append(rsregtree_0recall)
        row.append(rsregtree_0precision)
        row.append(rsregtree_1recall)
        row.append(rsregtree_1precision)
        row.append(rsregtree_acc)
        row.append(rsregtree_balacc)

        # =======================================================================
        data.append(row)

        #Log to CSV file
        
rippertraintest = pd.DataFrame(data,columns = ["SimulinkModel","Requirement","Run","p1","p2","p3","p4","EnsFailNum","RSFailNum","RSLogRegFailNum","RSRegTreeFailNum","testf","testp","FailRecallSA","FailPrecisionSA","PassRecallSA","PassPrecisionSA","AccuracySA","BalAccSA","FailRecallSASimOnly","FailPrecisionSASimOnly","PassRecallSASimOnly","PassPrecisionSASimOnly","AccuracySASimOnly","BalAccSASimOnly","FailRecallRS","FailPrecisionRS","PassRecallRS","PassPrecisionRS","AccuracyRS","BalAccRS","FailRecallLR","FailPrecisionLR","PassRecallLR","PassPrecisionLR","AccuracyLR","BalAccLR","FailRecallRT","FailPrecisionRT","PassRecallRT","PassPrecisionRT","AccuracyRT","BalAccRT"])
rippertraintest.to_csv(fname_path+'DRresult.csv')
