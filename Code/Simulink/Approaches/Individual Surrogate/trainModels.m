%Train test approach

function model = trainModels(train_features,train_Y,X,model_name,predictorNames) 
        switch model_name
            case 'svr'
                disp('svm');
                svr = fitrsvm(train_features,train_Y,... %,"PredictorNames",predictorNames
                            'OptimizeHyperparameters','auto',...
                            'HyperparameterOptimizationOptions',struct('AcquisitionFunctionName',...
                            'expected-improvement-plus','MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
                model = svr;
    
            case 'regtree'
                disp('regtree');
                regtree = fitrtree(train_features,train_Y,... %,"PredictorNames",predictorNames
                'OptimizeHyperparameters','auto',...
                'HyperparameterOptimizationOptions',struct('AcquisitionFunctionName',...
                'expected-improvement-plus','MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
                model = regtree;
 
            case 'rf_ensemble'
                disp('rf');

                rf = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
            'Method','Bag','OptimizeHyperparameters',{'MinLeafSize','MaxNumSplits'}, ... %{'NumLearningCycles','MaxNumSplits','MinLeafSize'}
                'HyperparameterOptimizationOptions',struct('MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
                model = rf;

            case 'ls_boost'
                disp('ls');
                lsboost = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
                'Method','LSBoost','OptimizeHyperparameters',{'MinLeafSize','MaxNumSplits'}, ... %{'NumLearningCycles','MaxNumSplits','MinLeafSize'}
                'HyperparameterOptimizationOptions',struct('MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
                model = lsboost;

            case 'gpr'
                disp('gpr');
                gpr = fitrgp(train_features,train_Y,'Basis','linear',...
          'FitMethod','exact','PredictMethod','exact'); %,"PredictorNames",predictorNames
                model = gpr;

            case 'gpr_nonlinear'
                disp('gpr non linear');
                gprnl = fitrgp(train_features,train_Y,'KernelFunction','squaredexponential',... %,"PredictorNames",predictorNames
    'OptimizeHyperparameters','auto','HyperparameterOptimizationOptions',...
    struct('AcquisitionFunctionName','expected-improvement-plus','MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
                model = gprnl;

            case 'nn'
                disp('regression neural network');
                numNode = floor((2/3)*length(X));
                numNode2 = floor((2/3)*length(X));
                nn = fitrnet(train_features,train_Y,"Standardize",true, ... %,"PredictorNames",predictorNames
"LayerSizes",[numNode,numNode2],'OptimizeHyperparameters',{'Lambda','LayerWeightsInitializer','LayerBiasesInitializer','Activations','Standardize'}, ... %'Lambda',1e-2
    "HyperparameterOptimizationOptions",struct("AcquisitionFunctionName","lower-confidence-bound",'MaxObjectiveEvaluations',10,'ShowPlots',false)); %,'UseParallel', true
                model = nn;
        end

end