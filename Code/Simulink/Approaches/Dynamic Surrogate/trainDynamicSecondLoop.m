%Function to retrain surrogate models without tuning i.e. Training the
%surrogate model with the same hyperparameters but with the newly updated
%dataset
function [model1,delta1,modelname1,traindelta1] = trainDynamicSecondLoop(train_features,train_Y,test_features,test_Y,X,text_file_location,predictorNames)
            trainTableCheck = istable(train_Y);
            if trainTableCheck
                train_Y = table2array(train_Y);
            end
            testTableCheck = istable(test_Y);
            if testTableCheck
                test_Y = table2array(test_Y);
            end

            svrParams = matfile('svr.mat');
            svr_m = svrParams.svr;
            svr_params = svr_m.ModelParameters;

            svr = fitrsvm(train_features,train_Y,"BoxConstraint",svr_params.BoxConstraint,... %,"PredictorNames",predictorNames
                "CacheSize",svr_params.CacheSize,"DeltaGradientTolerance",svr_params.DeltaGradientTolerance,...
                "Epsilon",svr_params.Epsilon,"GapTolerance",svr_params.GapTolerance,"KernelFunction",svr_params.KernelFunction,"KernelScale",svr_params.KernelScale,...
                "Solver",svr_params.Solver);
            svrTrainPred = predict(svr,train_features); %svr predicted fitness
            svrTrainError = mae(train_Y - svrTrainPred);
            
            svrTestPred = predict(svr,test_features);
            svrTestError = mae(test_Y - svrTestPred);
            disp("SVR Test Error");
            disp(svrTestError);

            regParams = matfile('regtree.mat');
            reg_m = regParams.regtree;
            reg_params = reg_m.ModelParameters;

            regtree = fitrtree(train_features,train_Y,"MaxNumSplits",reg_params.MaxSplits,... %,"PredictorNames",predictorNames
                "MinParentSize",reg_params.MinParent,"MinLeafSize",reg_params.MinLeaf,"MergeLeaves",reg_params.MergeLeaves,"PruneCriterion","mse",QuadraticErrorTolerance=reg_params.QEToler);
            regTrainPred = predict(regtree,train_features); %reg pred fitness
            regTrainError = mae(train_Y - regTrainPred);
            regTestPred = predict(regtree,test_features);
            regTestError = mae(test_Y - regTestPred);
            disp("Reg Test Error");
            disp(regTestError);

            rfParams = matfile('rf.mat');
            rf_m = rfParams.rf;
            rf_params = rf_m.ModelParameters;
            rf = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
            'Method','Bag');
            rfTrainPred = predict(rf,train_features);
            rfTrainError = mae(train_Y - rfTrainPred);

            rfTestPred = predict(rf,test_features);
            rfTestError = mae(test_Y - rfTestPred);
            disp("Rf Test Error");
            disp(rfTestError);

            lsParams = matfile('lsboost.mat');
            ls_m = lsParams.lsboost;
            ls_params = ls_m.ModelParameters;

            lsboost = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
            'Method','LSBoost');
            lsTrainPred = predict(lsboost,train_features);
            lsTrainError = mae(train_Y - lsTrainPred);

            lsTestPred = predict(lsboost,test_features);
            lsTestError = mae(test_Y - lsTestPred);

            disp("LS Test Error");
            disp(lsTestError);

            nnParams = matfile('nn.mat');
            nn_m = nnParams.nn;
            nn_params = nn_m.ModelParameters;
            numNode = nn_m.ModelParameters.LayerSizes(1);
            numNode2 = nn_m.ModelParameters.LayerSizes(2);
            nn = fitrnet(train_features,train_Y,"Standardize",true, ... %,"PredictorNames",predictorNames
                "LayerSizes",[numNode,numNode2],"Activations",nn_m.ModelParameters.Activations,...
                "LayerWeightsInitializer",nn_m.ModelParameters.LayerWeightsInitializer,"LayerBiasesInitializer",nn_m.ModelParameters.LayerBiasesInitializer,...
                "IterationLimit",nn_m.ModelParameters.IterationLimit,"GradientTolerance",nn_m.ModelParameters.GradientTolerance,...
                "LossTolerance",nn_m.ModelParameters.LossTolerance,"StepTolerance",nn_m.ModelParameters.StepTolerance,...
                "ValidationPatience",nn_m.ModelParameters.ValidationPatience,"ValidationFrequency",nn_m.ModelParameters.ValidationFrequency,...
                "Lambda",nn_m.ModelParameters.Lambda); %
            nnTrainPred = predict(nn,train_features);
            nnTrainError = mae(train_Y - nnTrainPred);

            nnTestPred = predict(nn,test_features);
            nnTestError = mae(test_Y - nnTestPred);

            disp("NN Test Error");
            disp(nnTestError);

            gprnlParams = matfile('gprnl.mat');
            gprnl_m = gprnlParams.gprnl;
            gprnlparams = gprnl_m.ModelParameters;
            disp('gpr non linear');
            gprnl = fitrgp(train_features,train_Y,'KernelFunction','squaredexponential',... %,"PredictorNames",predictorNames
'BasisFunction',gprnlparams.BasisFunction,"Beta",gprnlparams.Beta,"Sigma",gprnlparams.Sigma,FitMethod="exact",PredictMethod="exact",...
ConstantSigma=gprnlparams.ConstantSigma);
            gprnlTrainPred = predict(gprnl,train_features);
            gprnlTrainError = mae(train_Y - gprnlTrainPred);

            gprnlTestPred = predict(gprnl,test_features);
            gprnlTestError = mae(test_Y - gprnlTestPred);

            disp("GPRNL Test Error");
            disp(gprnlTestError);

            gpr = fitrgp(train_features,train_Y,'Basis','linear',"PredictorNames",predictorNames,...
                      'FitMethod','exact','PredictMethod','exact');
            gprTrainPred = predict(gpr,train_features);
            gprTrainError = mae(train_Y - gprTrainPred);

            gprTestPred = predict(gpr,test_features);
            gprTestError = mae(test_Y - gprTestPred);

            %Added gpr
            deltas = [svrTestError,regTestError,rfTestError,lsTestError,nnTestError,gprnlTestError,gprTestError];
            deltasTrain = [svrTrainError,regTrainError,rfTrainError,lsTrainError,nnTrainError,gprnlTrainError,gprTrainError];
            names = ['s','t','r','l','n','g','p']; %Initials of svr, regtree, rf, lsboost, nn, gprnl and gpr respectively
            for i = 1:7
                models(i).name = names(i);
                models(i).delta = deltas(i);
                models(i).traindelta = deltasTrain(i);
            end

            T = struct2table(models); % convert the struct array to a table
            sortedT = sortrows(T, 'delta'); % sort the table by 'delta'
            sortedS = table2struct(sortedT); % change it back to struct array if necessary
            
            modelname1 = sortedS(1).name;
            modelname2 = sortedS(2).name;
            modelname3 = sortedS(3).name;

            delta1 = sortedS(1).delta;
            delta2 = sortedS(2).delta;
            delta3 = sortedS(3).delta;

            traindelta1 = sortedS(1).traindelta;
            disp(text_file_location);

            model1 = chooseModel(sortedS,svr,regtree,rf,lsboost,nn,gprnl,gpr,1); 
            model2 = chooseModel(sortedS,svr,regtree,rf,lsboost,nn,gprnl,gpr,2); 
            model3 = chooseModel(sortedS,svr,regtree,rf,lsboost,nn,gprnl,gpr,3); 
end

function model = chooseModel(sortedS,svr,regtree,rf,lsboost,nn,gprnl,gpr,index)
    if sortedS(index).name == 's'
        model = svr;
    end
    if sortedS(index).name == 't'
        model = regtree;
    end
    if sortedS(index).name == 'r'
        model = rf;
    end
    if sortedS(index).name == 'l'
        model = lsboost;
    end
    if sortedS(index).name == 'n'
        model = nn;
    end
    if sortedS(index).name == 'g'
        model = gprnl;
    end
    if sortedS(index).name == 'p'
        model = gpr;
    end
end
