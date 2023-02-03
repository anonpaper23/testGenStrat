import pandas as pd
import numpy as np
import sys
import os
import glob
from sklearn.metrics import f1_score,accuracy_score,recall_score,precision_score
from sklearn.model_selection import train_test_split
from sklearn.metrics import precision_score, recall_score,accuracy_score
from sklearn.tree import DecisionTreeRegressor
from sklearn import tree
from sklearn.preprocessing import StandardScaler
import glob
from sklearn.model_selection import RepeatedKFold,cross_validate,RepeatedStratifiedKFold
from sklearn.model_selection import cross_val_predict
from sklearn.metrics import fbeta_score, make_scorer,accuracy_score
from sklearn.tree import _tree
import matplotlib.pyplot as plt
import random
from skopt import BayesSearchCV
import pickle

def extractRanges(data_file_location,verify_file_location):
    #Locate the CSV file in which test generation data is logged.(created by the MATLAB script)
    row = []
    print('============================================================================')
    fname = verify_file_location.split("\\") 
    filename = str(fname[-1])
    print(filename)
    mdata = filename.split('_')
    simName = mdata[0]
    print(simName)
    row.append(simName)

    fname_path = "..\\..\\Results\\"+simName+"\\verify\\"
    sourcefile_path = "..\\..\\Results\\"+simName+"\\"
    sourcefile = filename.replace("verify", "regression")
    print(sourcefile)

    location = fname_path+filename #location of verify dataset
    print(location)

    srclocation = sourcefile_path+sourcefile #location of regression dataset
    print(srclocation)

    #Define a dataframe from the test suite csv file
    actual_df = pd.read_csv(srclocation)

    #Drop columns that are not required
    actual_df = actual_df.drop(['Type','Label'],axis=1)
    if 'TrainDelta' in actual_df.columns:
        actual_df = actual_df.drop(['TrainDelta'],axis=1)
    if 'TestDelta' in actual_df.columns:
        actual_df = actual_df.drop(['TestDelta'],axis=1)
    #############################################################################
    #Code to build models only after 20 points added to the dataset.
    # Different models are represented by the change in the subset of train data. 
    print(actual_df.shape[0])
    if actual_df.shape[0] % 20 == 0:
        print('Using new model')
        actual_X = actual_df.iloc[:,1:]
        actual_Y = actual_df.iloc[:,0]
    else:
        print('Using prev model')
        reminderToBeSubtracted = actual_df.shape[0] % 20
        indexAfterSub = actual_df.shape[0] - reminderToBeSubtracted
        actual_X = actual_df.iloc[:indexAfterSub,1:]
        actual_Y = actual_df.iloc[:indexAfterSub,0]
    #############################################################################
    print(actual_df.shape[0])
    #Save the trained model as a pickle file
    filename = 'tuned_rtmodel.sav'
    if (actual_df.shape[0] == 301):
        #Both tune and train the first time in second loop. I.e After preprocessing stage
        params = tune_dt(actual_X,actual_Y)
        reg_clf = DecisionTreeRegressor(**params) # 
        reg_clf = reg_clf.fit(actual_X,actual_Y)
        # save the model to disk
        pickle.dump(reg_clf, open(filename, 'wb'))
    else:
        #Only train
        reg_clf = pickle.load(open(filename, 'rb'))

    print('DT Training complete!')

    #Extract paths related to boundary data points.
    paths = get_rules(reg_clf,actual_df.columns[1:])     
    boundaryPaths = []
    #Sort the paths based on the leaf node value
    sortedPaths = sorted(paths,key=lambda x: x[-1][0])

    for i in range(len(sortedPaths)-1):
        if simName == 'reg':
            print('correct Path')
            #Find the boundary paths i.e. Path  with leaf value around 0. 
            # More specifically, two consecutive paths with one having leaf value less than 0 AND one with leaf value greater than 0.
            if sortedPaths[i][-1][0] == 0 and sortedPaths[i+1][-1][0] > 0:
                bpath1 = sortedPaths[i]
                bpath2 = sortedPaths[i+1]
                boundaryPaths.append(bpath1)
                boundaryPaths.append(bpath2)
            else:
                pass
        else:
            if sortedPaths[i][-1][0] < 0 and sortedPaths[i+1][-1][0] >= 0:
                bpath1 = sortedPaths[i]
                bpath2 = sortedPaths[i+1]
                boundaryPaths.append(bpath1)
                boundaryPaths.append(bpath2)
            else:
                pass

    #Each path from regression tree contains additional information such as leaf node value. getDecisionPath removes it 
    #and returns just the path. 
    bpath1 = getDecisionPath(bpath1)
    bpath2 = getDecisionPath(bpath2)

    print(bpath1)
    print(bpath2)

    #Append both the paths
    for elem in bpath2:
        bpath1.append(elem)

    #Appended path is passed to modifyPath function
    listOfBpaths = modifyPath(bpath1,simName)

    # print(listOfBpaths)

    return listOfBpaths


def getDecisionPath(bpath):
    decisionPathDepth = len(bpath)
    if decisionPathDepth > 2:
        decisionPath = bpath[:-1] #i.e. Without number of instances and probability details of the path itself
    if decisionPathDepth == 2:
        decisionPath = bpath[0]

    return decisionPath

#Returns list of ranges that is passed to MATLAB code.
def modifyPath(decisionPath,simName):
    print(decisionPath)
    tempArr = []
    for nodes in decisionPath:
        print('Printing conds')
        print(nodes[0])
        tempArr.append(nodes[0])
    setWithVarNames = set(tempArr)

    rangeForEachVar = []
    for varName in setWithVarNames:
        lowArr = []
        highArr = []
        for nodes in decisionPath:
            if nodes[0] == varName:
                if nodes[1] == "<=":
                    highArr.append(nodes[2])
                else:
                    lowArr.append(nodes[2])
        # print(lowArr)
        # print(highArr)
        if len(lowArr) == 0:
            low = None
        else:
            low = max(lowArr)
        if len(highArr) == 0:
            high = None
        else:
            high = min(highArr)
        rangeForEachVar.append([varName,low,high])
    print(rangeForEachVar)

    finalRangeForEachVar = []
    #Parameter set to create a buffer around the two bounds of a range. One of these buffer ranges are used to continue the search
    epsilon = 0.05
    for nodes in rangeForEachVar:
        varName = nodes[0]
        defaultLow = getLow(simName,varName)
        defaultHigh = getHigh(simName,varName)
        if nodes[1] == None: # Lower bound missing
            l1 = None
            l2 = None

            h1 = nodes[2] - (abs(epsilon*defaultHigh))
            h2 = nodes[2] + (abs(epsilon*defaultHigh))
        elif nodes[2] == None:
            h1 = None
            h2 = None

            l1 = nodes[1] - (abs(epsilon*defaultLow))
            l2 = nodes[1] + (abs(epsilon*defaultLow))

        elif (nodes[1] == None and nodes[2] == None):
            l1 = None
            l2 = None
            h1 = None
            h2 = None
        else:
            l1 = nodes[1] - (abs(epsilon*defaultLow))
            l2 = nodes[1] + (abs(epsilon*defaultLow))

            h1 = nodes[2] - (abs(epsilon*defaultHigh))
            h2 = nodes[2] + (abs(epsilon*defaultHigh))

            tempLow = (l1,l2)
            tempHigh = (h1,h2)
            temp = [tempLow,tempHigh]
            tempLen = len(temp)
            randIndex = random.randint(0,tempLen-1)

            finalRangeForEachVar.append([varName,temp[randIndex]])
    print([finalRangeForEachVar])
    return finalRangeForEachVar

#Function to perform hyperparameter tuning using Bayesian Optimization
def tune_dt(X,y):
    print("TUNING....")
    param_dist_dt = {"max_depth": [2,14],
                "max_features": [1,10],
                "min_samples_leaf": [1,10]
                }
    tree_clf = BayesSearchCV(tree.DecisionTreeRegressor(), param_dist_dt, cv=5, random_state=0)
    search = tree_clf.fit(X,y)
    print(search.best_params_)
    return search.best_params_

cv = RepeatedStratifiedKFold(n_splits=10, n_repeats=10, random_state=1)

#Function to parse the regression tree and extact all the paths
def get_rules(tree, feature_names, class_names=None):
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
            p1 += [[name,'<=',np.round(threshold, 3)]]
            recurse(tree_.children_left[node], p1, paths)
            p2 += [[name,'>',np.round(threshold, 3)]]
            recurse(tree_.children_right[node], p2, paths)
        else:
            path += [[tree_.value[node], tree_.n_node_samples[node]]]
            paths += [path]

            
    recurse(0, path, paths)

    # sort by samples count
    samples_count = [p[-1][1] for p in paths]
    ii = list(np.argsort(samples_count))
    paths = [paths[i] for i in reversed(ii)]
                
    return paths

#Default upper and lower bounds of an input range
#The function is created such that a few initial characters of the input name is used
#as an unique identifier and the default bounds are allocated to each input. 
#For example, in case of Tustin, there are 14 inputs, Xin1 to Xin11, TL,BL and IC.
#The upper bound of Xin1 to Xin11,TL, BL and IC is 20 and lower bound is -20 for Xin1 to Xin11 and -10 for TL,BL and IC.
def getHigh(simulink, VarName):
    high = None
    if(simulink == 'tustin' or simulink == 'tustinr4'):
        if(VarName[:3] == 'Xin' or VarName[:2] == 'TL' or VarName[:2] == 'BL' or VarName[:2] == "IC"):
            high = 20
    if(simulink == 'reg'):
        high = 5
    if(simulink == 'autopilot'):
        if(VarName[:4] == 'Turn'):
            high = 45
        if(VarName[:7] == 'HDG_Ref'):
            high = 180
        if(VarName[:7] == 'ALT_Ref'):
            high = 1000
        if(VarName[:6] == 'Pwheel'):
            high = 30
        if(VarName[:8] == 'Throttle'):
            high = 1
        if(VarName[:6] == 'AP_Eng'):
            high = 1
        if(VarName[:8] == 'HDG_Mode'):
            high = 1
        if(VarName[:8] == 'ALT_Mode'):
            high = 1
    if(simulink == 'nlguidance'):
        high = 100
    return high

def getLow(simulink, VarName):
    low = None
    if(simulink == 'tustin' or simulink == 'tustinr4'):
        if(VarName[:3] == 'Xin' or VarName[:2] == "IC"):
            low = -20
        if(VarName[:2] == 'TL' or VarName[:2] == 'BL'):
            low = -10
    if(simulink == 'reg'):
        low = 0
    if(simulink == 'autopilot'):
        if(VarName[:4] == 'Turn'):
            low = 0
        if(VarName[:7] == 'HDG_Ref'):
            low = -180
        if(VarName[:7] == 'ALT_Ref'):
            low = 0
        if(VarName[:6] == 'Pwheel'):
            low = -30
        if(VarName[:8] == 'Throttle'):
            low = 0
        if(VarName[:6] == 'AP_Eng'):
            low = 1
        if(VarName[:8] == 'HDG_Mode'):
            low = 1
        if(VarName[:8] == 'ALT_Mode'):
            low = 0
    if(simulink == 'nlguidance'):
        if(VarName == 'r'):
            low = 0
        else:
            low = -100
    return low

listOfBpaths = extractRanges(data_file_location,verify_file_location)
