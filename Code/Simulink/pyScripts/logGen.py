import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline
from sklearn.model_selection import RepeatedKFold,cross_validate,RepeatedStratifiedKFold
from sklearn.metrics import fbeta_score, make_scorer,accuracy_score
import matplotlib.pyplot as plt
from math import log
from skopt import BayesSearchCV
import pickle

def logGen(data_file_location,verify_file_location,points):
    #Locate the CSV file in which test generation data is logged.(created by the MATLAB script)
    row = []
    print('============================================================================')
    fname = verify_file_location.split("\\")
    filename = str(fname[-1])
    mdata = filename.split('_')
    simName = mdata[0]
    row.append(simName)

    fname_path = "..\\..\\Results\\ModelTestingResults\\exp1\\"+simName+"\\verify\\"
    sourcefile_path = "..\\..\\Results\\ModelTestingResults\\exp1\\"+simName+"\\"
    sourcefile = filename.replace("verify", "regression")
    print(sourcefile)

    location = fname_path+filename #location of verify dataset
    print(location)

    srclocation = sourcefile_path+sourcefile #location of regression dataset
    print(srclocation)

    #Define a dataframe from the test suite csv file
    actual_df = pd.read_csv(srclocation)

    #Drop unnecessary columns
    actual_df = actual_df.drop(['Type','Fitness'],axis=1)
    if 'TrainDelta' in actual_df.columns:
        actual_df = actual_df.drop(['TrainDelta'],axis=1)
    if 'TestDelta' in actual_df.columns:
        actual_df = actual_df.drop(['TestDelta'],axis=1)

    print(actual_df.shape[0])
    if actual_df.shape[0] % 20 == 0:
        print('Using new model')
        X = actual_df.iloc[:,1:]
        y = actual_df.iloc[:,0]
    else:
        print('Using prev model')
        reminderToBeSubtracted = actual_df.shape[0] % 20
        indexAfterSub = actual_df.shape[0] - reminderToBeSubtracted
        X = actual_df.iloc[:indexAfterSub,1:]
        y = actual_df.iloc[:indexAfterSub,0]

    
    print(actual_df.shape[0])
    filename = 'tuned_logmodel.sav'
    if (actual_df.shape[0] == 301):
        #Tune and train LogReg for given dataset
        params = tune_log(X,y)
        log_clf = LogisticRegression(**params)
        log_clf = log_clf.fit(X,y)
        # save the model to disk
        pickle.dump(log_clf, open(filename, 'wb'))
    else:
        log_clf = pickle.load(open(filename, 'rb'))

    print('DT Training complete!')

    mergedList = []
    distForEachInput = []
    numOfInputs = len(points)

    #Find the number of positive labels
    pNum = actual_df[actual_df["Label"] == 1].shape[0]
    #Find the total dataset size
    dataSize = actual_df.shape[0]
    #p = positive labels num/ total dataset
    p = pNum/dataSize

    #For each input radnomly generated and obtained from the MATLAB script, find the euclidean 
    #distance between the hyperplane and each point. 
    
    for i in range(numOfInputs):
        lengthOfInput = len(points[0])
        odd = (p/(1-p))
        logodd = log(odd)
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
    
    #Sort the points based on the euclidean distance
    sortedMergedList = sorted(mergedList,key=lambda x: x[0])
    finalList = []
    for i in range(len(sortedMergedList)):
        finalList.append(sortedMergedList[i][0][1])
    print(finalList)
    return finalList

#Tuning Logistic Regression using Bayesian Optimization
def tune_log(X,y):
    param_dist_dt = {"C": [0.01,100]}
    log_clf = BayesSearchCV(LogisticRegression(), param_dist_dt, cv=10, random_state=0)
    search = log_clf.fit(X,y)
    print(search.best_params_)
    return search.best_params_

finalList = logGen(data_file_location,verify_file_location,points)
