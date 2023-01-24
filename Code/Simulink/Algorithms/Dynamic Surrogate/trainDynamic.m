function [model1,delta1,modelname1,traindelta1] = trainEnsemble(train_features,train_Y,test_features,test_Y,X,text_file_location,predictorNames)
            trainTableCheck = istable(train_Y);
            if trainTableCheck
                train_Y = table2array(train_Y);
            end
            testTableCheck = istable(test_Y);
            if testTableCheck
                test_Y = table2array(test_Y);
            end
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % Tune using Bayesian Search and fit different types of machine learning models to the dataset. 
            svr = fitrsvm(train_features,train_Y,... %,"PredictorNames",predictorNames
                            'OptimizeHyperparameters','auto',...
                            'HyperparameterOptimizationOptions',struct('AcquisitionFunctionName',...
                            'expected-improvement-plus','MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
            svrTrainPred = predict(svr,train_features); %svr predicted fitness
            svrTrainError = mae(train_Y - svrTrainPred);
            
            svrTestPred = predict(svr,test_features);
            svrTestError = mae(test_Y - svrTestPred);
            disp("SVR Test Error");
            disp(svrTestError);
            save('svr',"svr");
            
            regtree = fitrtree(train_features,train_Y,... %,"PredictorNames",predictorNames
                'OptimizeHyperparameters','auto',...
                'HyperparameterOptimizationOptions',struct('AcquisitionFunctionName',...
                'expected-improvement-plus','MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
            regTrainPred = predict(regtree,train_features); %reg pred fitness
            regTrainError = mae(train_Y - regTrainPred);
            regTestPred = predict(regtree,test_features);
            regTestError = mae(test_Y - regTestPred);
            save('regtree',"regtree");
            
            rf = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
            'Method','Bag','OptimizeHyperparameters',{'MinLeafSize','MaxNumSplits'}, ... %{'NumLearningCycles','MaxNumSplits','MinLeafSize'}
                'HyperparameterOptimizationOptions',struct('MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
            rfTrainPred = predict(rf,train_features);
            rfTrainError = mae(train_Y - rfTrainPred);

            rfTestPred = predict(rf,test_features);
            rfTestError = mae(test_Y - rfTestPred);
            save('rf',"rf");
            
            lsboost = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
                'Method','LSBoost','OptimizeHyperparameters',{'MinLeafSize','MaxNumSplits'}, ... %{'NumLearningCycles','MaxNumSplits','MinLeafSize'}
                'HyperparameterOptimizationOptions',struct('MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
            lsTrainPred = predict(lsboost,train_features);
            lsTrainError = mae(train_Y - lsTrainPred);

            lsTestPred = predict(lsboost,test_features);
            lsTestError = mae(test_Y - lsTestPred);
            save('lsboost',"lsboost");

            gpr = fitrgp(train_features,train_Y,'Basis','linear',... %,"PredictorNames",predictorNames
                      'FitMethod','exact','PredictMethod','exact');
            gprTrainPred = predict(gpr,train_features);
            gprTrainError = mae(train_Y - gprTrainPred);

            gprTestPred = predict(gpr,test_features);
            gprTestError = mae(test_Y - gprTestPred);

            save("gpr","gpr");
            
            numNode = floor((2/3)*length(X));
            numNode2 = floor((2/3)*length(X));
             nn = fitrnet(train_features,train_Y,"Standardize",true,... %"PredictorNames",predictorNames
"LayerSizes",[numNode,numNode2],'OptimizeHyperparameters',{'Lambda','LayerWeightsInitializer','LayerBiasesInitializer','Activations','Standardize'}, ... %'Lambda',1e-2
    "HyperparameterOptimizationOptions",struct("AcquisitionFunctionName","lower-confidence-bound",'MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
            nnTrainPred = predict(nn,train_features);
            nnTrainError = mae(train_Y - nnTrainPred);

            nnTestPred = predict(nn,test_features);
            nnTestError = mae(test_Y - nnTestPred);

            save("nn","nn");

            
            disp('gpr non linear');
            gprnl = fitrgp(train_features,train_Y,'KernelFunction','squaredexponential',... %,"PredictorNames",predictorNames
'OptimizeHyperparameters','all','HyperparameterOptimizationOptions',...
struct('AcquisitionFunctionName','expected-improvement-plus','MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
            gprnlTrainPred = predict(gprnl,train_features);
            gprnlTrainError = mae(train_Y - gprnlTrainPred);

            gprnlTestPred = predict(gprnl,test_features);
            gprnlTestError = mae(test_Y - gprnlTestPred);

            save("gprnl","gprnl");
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            %Choose the best surrogate model. Lower the test error, better
            %the model. 
            deltas = [svrTestError,regTestError,rfTestError,lsTestError,nnTestError,gprnlTestError,gprTrainError];
            deltasTrain = [svrTrainError,regTrainError,rfTrainError,lsTrainError,nnTrainError,gprnlTrainError,gprTestError];
            names = ['s','t','r','l','n','g','p']; %Initials of svr, regtree, rf, lsboost, nn, gprnl and gpr respectively (Used p for gpr since g is already used for gprnl) 
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
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end

%Setting unique characters to identify different surrogate models in a map
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
