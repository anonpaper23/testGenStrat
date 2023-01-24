function model = trainModelsSecondLoop(train_features,train_Y,X,model_name,predictorNames)
        model = matfile('model.mat');
        
        switch model_name
            case 'svr'
                disp('svm');
                m = model.model;
                params = m.ModelParameters;
                disp(params);
                svr = fitrsvm(train_features,train_Y,"BoxConstraint",params.BoxConstraint,... %,"PredictorNames",predictorNames
                    "CacheSize",params.CacheSize,"DeltaGradientTolerance",params.DeltaGradientTolerance,...
                    "Epsilon",params.Epsilon,"GapTolerance",params.GapTolerance,"KernelFunction",params.KernelFunction,"KernelScale",params.KernelScale,...
                    "Solver",params.Solver);
                model = svr;
    
            case 'regtree'
                disp('regtree');
                m = model.model;
                params = m.ModelParameters;
                disp(params);
                regtree = fitrtree(train_features,train_Y,"MaxNumSplits",params.MaxSplits,... %,"PredictorNames",predictorNames
                    "MinParentSize",params.MinParent,"MinLeafSize",params.MinLeaf,"MergeLeaves",params.MergeLeaves,"PruneCriterion","mse",QuadraticErrorTolerance=params.QEToler);
                model = regtree;
 
            case 'rf_ensemble'
                disp('rf');
                m = model.model;
                params = m.ModelParameters;
                treeParams = params.LearnerTemplates{1, 1};
                disp(treeParams);

                rf = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
                'Method','Bag');
                model = rf;

            case 'ls_boost'
                disp('ls');
                m = model.model;
                params = m.ModelParameters;
                treeParams = params.LearnerTemplates{1, 1};

                lsboost = fitrensemble(train_features,train_Y,... %,"PredictorNames",predictorNames
                'Method','LSBoost');
                model = lsboost;

            case 'gpr'
                disp('gpr');
                gpr = fitrgp(train_features,train_Y,'Basis','linear',...
          'FitMethod','exact','PredictMethod','exact'); %,"PredictorNames",predictorNames
                model = gpr;

            case 'gpr_nonlinear'
                disp('gpr non linear');
                m = model.model;
                params = m.ModelParameters;
                gprnl = fitrgp(train_features,train_Y,'KernelFunction','squaredexponential',... %"PredictorNames",predictorNames,
'BasisFunction',params.BasisFunction,"Beta",params.Beta,"Sigma",params.Sigma,FitMethod="exact",PredictMethod="exact",...
ConstantSigma=params.ConstantSigma);
                model = gprnl;

            case 'nn'
                m = model.model;
                params = m.ModelParameters;
                disp('regression neural network');
                numNode = m.ModelParameters.LayerSizes(1);
                numNode2 = m.ModelParameters.LayerSizes(2);
                nn = fitrnet(train_features,train_Y,"Standardize",true, ... 
                    "LayerSizes",[numNode,numNode2],"Activations",m.ModelParameters.Activations,...
                    "LayerWeightsInitializer",m.ModelParameters.LayerWeightsInitializer,"LayerBiasesInitializer",m.ModelParameters.LayerBiasesInitializer,...
                    "IterationLimit",m.ModelParameters.IterationLimit,"GradientTolerance",m.ModelParameters.GradientTolerance,...
                    "LossTolerance",m.ModelParameters.LossTolerance,"StepTolerance",m.ModelParameters.StepTolerance,...
                    "ValidationPatience",m.ModelParameters.ValidationPatience,"ValidationFrequency",m.ModelParameters.ValidationFrequency,...
                    "Lambda",m.ModelParameters.Lambda); %
                model = nn;
        end

end