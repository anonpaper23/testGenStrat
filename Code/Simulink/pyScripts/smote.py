import pandas as pd
from imblearn.over_sampling import SMOTE
import os
import numpy as np
  
def smote(path,problem):
    originaldata = pd.read_csv(path)

    #convert Pandas to numpy array
    oversample = SMOTE(sampling_strategy='minority',k_neighbors=2)
    X = originaldata.drop(['Fitness','Label','Type','TrainDelta','TestDelta'],axis=1)
    y = originaldata.iloc[:,1]

    X, y = oversample.fit_resample(X, y)
    X = X.values.tolist()
    X = np.array(X)
    return X

smotelist = smote(path,problem)