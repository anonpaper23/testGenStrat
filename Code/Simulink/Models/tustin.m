function varargout = tustin(Operation,Global,element)
    switch Operation
        case 'init'
            disp('Called init case');
            urangeXmax         = 20;    %Input Xin max. value
            urangeXmin         = -20;   %Input Xin min. value
            urangeTLBLMax         = 10;   %Input TL, BL value 
            urangeTLBLMin         = -10;   %Input TL, BL  value 
            Global.D        = 14; 
            Global.lower    = [urangeXmin * ones(1,11),urangeTLBLMin * ones(1,2), urangeXmin];
            Global.upper    = [urangeXmax * ones(1,11),urangeTLBLMax * ones(1,2), urangeXmax];
            Global.operator = @GMutation;
            Global.localop = 244; %for adaptive walk
            Global.law = 34;         % for hcrr alg
            
            NoXin = 11;
            
            PopDec = rand(1,Global.D);
            
            for i = 1:NoXin   
            PopDec(:,i) = (PopDec(:,i) * (urangeXmax - urangeXmin)) + urangeXmin;  % for Xin
            end
            
          for i = NoXin+1:Global.D-1  
            PopDec(:,i) = (PopDec(:,i) * (urangeTLBLMax - urangeTLBLMin)) + urangeTLBLMin;  % for TL and BL      
          end
            
            PopDec(:,14) =  (PopDec(:,14 ) * (urangeXmax - urangeXmin)) + urangeXmin; % for initial condition(ic)
            %PopDec(:,15) =  round(rand); % for reset booelan True or False; 1-0
            varargout = {PopDec};

        case 'random'  
            disp('Called Random Search');
            PopObj = randomSearch(Global.run,element,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'optimize_r'
            %4
            disp('Called regression approach');
            PopObj = individualSurrogate(Global.run,element,Global.mlmodel,Global.initialSimNum,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'ensemble'
            disp('Called ensemble approach');
            PopObj = dynamicSurrogate(Global.run,element,Global.mlmodel,Global.initialSimNum,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'verify'      
            disp('Called verify case');
            PopObj = verify(Global.run,element,Global.mlmodel,Global.problem,Global.requirement);
            varargout = {PopObj};
            
        case 'generate'
            disp('Called generate case');
            PopObj = regressionTree(Global.run,element,Global.mlmodel,Global.initialSimNum,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'generateLog'
            disp('Called generate case');
            PopObj = logisticRegression(Global.run,element,Global.mlmodel,Global.initialSimNum,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};
    end
end

%         case 'ensembleVoting'  
%            
%             switch Global.requirement
%                 case 'R1a'
%                     % R1a
%                     low = -5.1682;
%                     high = 5.76204;
%                 case 'R1b'
%                     % R1b
%                     low = -0.004;
%                     high = 2.3548;
%                 case 'R1c'
%                     %R1c
%                     low = -0.0305;
%                     high = 1.54631;
%                 case 'R1d'    
%                     %R1d
%                     low = -0.04587;
%                     high = 2.322075;
%                 case 'R1e'
%                     %R1e
%                     low = -0.0345;
%                     high = 1.74958;
%                 case 'R2a'
%                     %R2a
%                     low = -0.00301;
%                     high = 0.157414;
%                 case 'R2b'
%                     %R2b
%                     low = -0.00323;
%                     high = 0.168535;
%                 case 'R4a'
%                     %R4a
%                     low = -1.6255;
%                     high = 1.79086;
%                 case 'R4b'
%                     %R4b
%                     low = 0.049249;
%                     high = 0.050116;
%             end
% 
%            disp('Called ensemble voting approach');
%             initialSimulationNum = Global.initialSimNum;
%             model_name = Global.mlmodel;
% 
%             inputArray = [];
%             overfitData = [];
%             X = element;
%            
%             %5
%             p = 'C:\Python-Workspace\thesis\FSE2019_latest\Results\ModelTestingResults\exp4\';
%             data_path = strcat(p,func2str(Global.problem),'\');
%             replace_dot = strrep(Global.requirement,'.','_');
%             data_file_location = strcat(data_path,func2str(Global.problem),'_',replace_dot,'_regression_',string(Global.evaluation),'_',model_name,'.csv');
%             disp(data_file_location);
% 
%             T = readtable(data_file_location);
%             T.Properties.VariableNames([1 2 3 4 5]) = {'Fitness', 'Label','Type','Delta','Run'}; % names of columns
%             H = height(T);
%             disp('total no of rows:')
%             disp(H);
% 
%             if H > initialSimulationNum %Hyperparameter
%                 idx = T.Type==0;
%                 simulationData = T(idx,:);
%                 simHeight = height(simulationData);
%                 train_features = simulationData(:, 6 : length(X)+5);
%                 Y = simulationData(:,1);
%                 %Convert target output table into array
%                 
%                 [lbl1,lbl2,lbl3] = trainEnsembleVoting(train_features,Y,X,low,high);
%                 if (lbl1 == 1 && lbl2 == 1 && lbl3 == 1) || (lbl1 == 0 && lbl2 == 0 && lbl3 == 0)
%                     reliable = 1;
%                 else
%                     reliable = 0;
%                 end
%                 if reliable
%                     type = 1;
%                     PopObj = -100;
%                 else
%                     type = 0;
%                     PopObj=simtstn(X,Global.requirement);
%                 end
% 
% %                 inputArray(4) = uncertainty; %Training loss that was used in each iteration
% %                 if (H == Global.evaluation-1) || (simHeight == Global.maxSimNum-1)
% %                     %Read csv file
% %                     base_path = strcat('C:\Python-Workspace\thesis\FSE2019_latest\Results\MLExperimentResults\');
% %                     testFileName = strcat(func2str(Global.problem),'_',replace_dot,'.csv');
% %                     testData = strcat(base_path,testFileName);
% %                     test = readtable(testData);
% %                     trainInput = test(:, 6 : length(X)+5);
% %                     target = test(:,1);
% %                     targetArray = table2array(target); %Y and array_actual_Y are same
% %                     [testPrediction,predictedFitness] = trainModels(trainInput,target,X,model_name);
% %                     [predictedFitness,uncertainty] = trainEnsemble(trainInput,targetArray,X);
% %                     %Convert target output table into array
% %                     
% %                     testError = evaluate(targetArray,testPrediction);
% %                     disp('Test error:')
% %                     disp(testError);
% %                     overfitData(1) = testError;
% % %                     overfitData(2) = func2str(Global.problem);
% % %                     overfitData(3) = replace_dot;
% % %                     overfitData(4) = model_name;
% %                     p = strcat(data_path,'overfit.csv');
% %                     writematrix(overfitData,p,'WriteMode', 'append');
% % 
% %                 end
%             else
%                 type = 0;
%                 PopObj=simtstn(X,Global.requirement);
%             end
% 
%             %label boundary or non boundary here
%             label = labelBNB(PopObj,low,high);
% 
%             inputArray(6:length(X)+5) = X;
%             inputArray(1) = PopObj;
%             inputArray(2) = label; %0 - NB & 1 - B
%             inputArray(3) = type; %0 - simulation & 1 - prediction
%             
%             path = strcat(data_path,func2str(Global.problem),'_',replace_dot,'_regression_',string(Global.evaluation),'_',model_name,'.csv');
%             writematrix(inputArray,path,'WriteMode', 'append');
%             
%             varargout = {PopObj};


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


