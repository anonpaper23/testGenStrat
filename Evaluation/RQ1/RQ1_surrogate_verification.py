#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import pandas as pd
import numpy as np
import os
sim_name = 'fsm'
#sourcefile_path = 'C:/Python-Workspace/thesis/FSE2023/Results/'+sim_name+'/'
sourcefile_path = '../../Results'+sim_name+'/'
data = []

for filename in os.listdir(sourcefile_path):
    rowArray = []
    f = os.path.join(sourcefile_path, filename)
    
    fname = filename
    vfile = fname.replace("regression", "verify")
    vfilepath = sourcefile_path+'verify/'
    location = vfilepath+vfile
    # checking if it is a file
    if os.path.isfile(f) & os.path.isfile(location):
        print(f)
        print(location)
        filename = filename.replace('.csv','')
        metadata = filename.split('_')
        simName = metadata[0]
        req = metadata[1]
        mlmodel = metadata[-2]
        runs = metadata[-1]
        rowArray.append(simName)
        rowArray.append(req)
        rowArray.append(mlmodel)
        rowArray.append(runs)
        
        actual_df = pd.read_csv(f,header=0)
        verify_df = pd.read_csv(location,header=0)
        
        rowArray.append(actual_df.shape[0])
        
        simIndex = np.where((actual_df['Type']==0))
        rowArray.append(actual_df.loc[simIndex].shape[0])
        
        predIndex = np.where((actual_df['Type']==1)) #resolve
        rowArray.append(actual_df.loc[predIndex].shape[0])
        
        #Find instances that switch from 0 to 1 i.e. NB in prediction to B in simulation (FN)
        idxA = np.where((verify_df['PredictedLabel']==0) & (verify_df['SimulatedLabel']==1))
        changeType1 = verify_df.loc[idxA].shape[0]
        rowArray.append(changeType1)


        #Find instances that switch from 1 to 0 i.e. B in prediction to NB in simulation (FP)
        idxB = np.where((verify_df['PredictedLabel']==1) & (verify_df['SimulatedLabel']==0))
        changeType2 = verify_df.loc[idxB].shape[0]
        rowArray.append(changeType2)

        #Total number of changes (Both 0 to 1 and 1 to 0)
        numChange = changeType1 + changeType2
        print(numChange)
        rowArray.append(numChange)

        idSamePass = np.where((verify_df['PredictedLabel']==1) & (verify_df['SimulatedLabel']==1))
        sameType1 = verify_df.loc[idSamePass].shape[0]
        rowArray.append(sameType1)

        idSameFail = np.where((verify_df['PredictedLabel']==0) & (verify_df['SimulatedLabel']==0))
        sameType2 = verify_df.loc[idSameFail].shape[0]
        rowArray.append(sameType2)

        #Total number of same labels
        idSame = sameType1+sameType2

        rowArray.append(idSame)

        #Number of predicted pass data points
        idxC = np.where((verify_df['PredictedLabel']==1))
        rowArray.append(verify_df.loc[idxC].shape[0])

        #Number of simulated pass data points
        idxD = np.where((verify_df['SimulatedLabel']==1))
        rowArray.append(verify_df.loc[idxD].shape[0])


        #Number of predicted fail data points
        idxE = np.where((verify_df['PredictedLabel']==0))
        rowArray.append(verify_df.loc[idxE].shape[0])

        #Number of simulated fail data points
        idxF = np.where((verify_df['SimulatedLabel']==0))
        rowArray.append(verify_df.loc[idxF].shape[0])

        meanPredicted = verify_df['PredictedFitness'].mean()
        meanSimulated = verify_df['SimulatedFitness'].mean()
        
        fitnessDiff = abs(meanSimulated - meanPredicted)
        rowArray.append(fitnessDiff)
        
        #delta value
        traindeltaAt300 = actual_df['TrainDelta'].iloc[303]
        traindeltaAtEnd = actual_df['TrainDelta'].iloc[-1]
        
        #delta value
        testdeltaAt300 = actual_df['TestDelta'].iloc[303]
        testdeltaAtEnd = actual_df['TestDelta'].iloc[-1]
        
        rowArray.append(traindeltaAt300)
        rowArray.append(testdeltaAt300)
        
        rowArray.append(traindeltaAtEnd)
        rowArray.append(testdeltaAtEnd)
        
        print(rowArray)
        
        data.append(rowArray)
        
regressionSummary = pd.DataFrame(data, columns = ["SimulinkModel","Requirement","mlModel","runs","DatasetSize","NumSim","NumPred","F2P","P2F","NumLabelsChange","sameP","sameF","NumLabelsSame","NumPredP","NumSimP","NumPredF","NumSimF","FitnessDifference","InitialTrainDelta","InitialTestDelta","TraindeltaAtEnd","TestdeltaAtEnd"])
regressionSummary.to_csv(sourcefile_path+'Summary.csv')

