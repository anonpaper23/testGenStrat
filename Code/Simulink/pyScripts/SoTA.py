import pandas as pd
import numpy as np
import sys
import os
import glob
from sklearn.metrics import f1_score,accuracy_score,recall_score,precision_score
from sklearn.model_selection import train_test_split
from sklearn.metrics import precision_score, recall_score,accuracy_score
from sklearn.tree import DecisionTreeRegressor, DecisionTreeClassifier
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

def extractRanges(data_file_location,verify_file_location, run):
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

    fname_path = "...\\Code\\Results\\"+simName+"\\verify\\"
    sourcefile_path = "...\\Code\\Results\\"+simName+"\\"
    sourcefile = filename.replace("verify", "regression")
    print(sourcefile)

    location = fname_path+filename #location of verify dataset
    print(location)

    srclocation = sourcefile_path+sourcefile #location of regression dataset
    print(srclocation)

    #Define a dataframe from the test suite csv file
    actual_df = pd.read_csv(srclocation)

    #Drop columns that are not required
    actual_df = actual_df.drop(['Fitness', 'Type'],axis=1)
    if 'TrainDelta' in actual_df.columns:
        actual_df = actual_df.drop(['TrainDelta'],axis=1)
    if 'TestDelta' in actual_df.columns:
        actual_df = actual_df.drop(['TestDelta'],axis=1)

    if data_file_location.find('R12_1') == -1: # we are checking R1.4.1 and R1.6
        if 'Throttle1' in actual_df.columns:
            actual_df = actual_df.drop(['Throttle1'], axis = 1)
            actual_df = actual_df.drop(['Throttle2'], axis=1)
            actual_df = actual_df.drop(['Throttle3'], axis=1)

    #############################################################################
    #Code to build models only after 20 points added to the dataset.
    # Different models are represented by the change in the subset of train data. 
    print(actual_df.shape[0])
    
    print('Using new model')
    actual_X = actual_df.iloc[:,1:]
    actual_Y = actual_df.iloc[:,0]
    
    #############################################################################
    print(actual_df.shape[0])
    #Save the trained model as a pickle file
    filename = '...\\Code\\Results\\tuned_rtmodel'+str(run)+'.pickle'
    
    #Both tune and train the first time in second loop. I.e After preprocessing stage
    print(np.version.version)
    print("i am types ")
    print(type(actual_X), type(actual_Y))
    numberoffails = actual_df['Label'].value_counts()[0]
    datasetsize = len(actual_df.index)
    reg_clf = DecisionTreeClassifier(min_samples_leaf = 1, min_samples_split =2, max_features = None, max_depth = 5, random_state = 0, class_weight = {0 : (1/numberoffails), 1 : 1/(datasetsize - numberoffails)}) #
    reg_clf = reg_clf.fit(actual_X,actual_Y)
    # save the model to disk
    pickle.dump(reg_clf, open(filename, 'wb'))
    

    print('DT Training complete!')

    #Extract paths related to boundary data points.
    paths = get_rules(reg_clf,actual_df.columns[1:])     
    boundaryPaths = []
    #Sort the paths based on the leaf node value
    #sortedPaths = sorted(paths,key=lambda x: x[-1][0])

    #Each path from regression tree contains additional information such as leaf node value. getDecisionPath removes it 
    #and returns just the path.

    #Appended path is passed to modifyPath function
    allrangesfromallpaths = []
    for r in paths:
        print("this is r", r)
        r = r[:-1]
        print("thisssssssss is r ", r)
        listOfBpaths = modifyPath(r,simName)
        allrangesfromallpaths.append(listOfBpaths)

    return allrangesfromallpaths


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

    return rangeForEachVar

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

listOfBpaths = extractRanges(data_file_location,verify_file_location, run)
