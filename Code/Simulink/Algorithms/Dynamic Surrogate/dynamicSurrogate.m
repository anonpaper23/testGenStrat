%This file implements the SA-DYN (dynamic surrogate) approach

function PopObj = dynamicSurrogate(run,element,mlmodel,initialSimNum,problem,req,evaluation)
            disp('Called dynamic approach');
            model_name = mlmodel;
            initialSimulationNum = initialSimNum; 
            
            p = '..\..\Results\';
            data_path = strcat(p,func2str(problem),'\');
            replace_dot = strrep(req,'.','_');
            data_file_location = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
            disp(data_file_location);
            high = getRange(func2str(problem),replace_dot);

            inputArray = [];
            X = element;
     
            text_file_location = strcat(data_path,func2str(problem),'_',replace_dot,'_ensemblelist','.csv');
            disp(text_file_location);

            T = readtable(data_file_location);
            
            if strcmp(func2str(problem), 'tustin')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','Xin1','Xin2','Xin3','Xin4','Xin5','Xin6','Xin7','Xin8','Xin9','Xin10','Xin11','TL','BL','IC'}; % names of columns
            end
            if strcmp(func2str(problem), 'tustinr4')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','TL','BL','IC'}; % names of columns
            end
            if strcmp(func2str(problem), 'autopilot')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','AP_Eng1','AP_Eng2','AP_Eng3','HDG_Mode1','HDG_Mode2','HDG_Mode3','ALT_Mode1','ALT_Mode2','ALT_Mode3','HDG_Ref1','HDG_Ref2','HDG_Ref3','TurnK1','TurnK2','TurnK3','ALT_Ref1','ALT_Ref2','ALT_Ref3','Pwheel1','Pwheel2','Pwheel3'}; % names of columns
            end
            if strcmp(func2str(problem), 'reg')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','A1','A2','B1','B2','C1','C2','D1','D2','E1','E2','F1','F2','G1','G2','H1','H2','I1','I2','J1','J2','K1','K2','L1','L2','M1','M2','N1','N2','O1','O2','P1','P2'};
            end
            if strcmp(func2str(problem), 'nlguidance')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','Xt1','Xt2','Xt3','Xv1','Xv2','Xv3','Vv1','Vv2','Vv3','Vt1','Vt2','Vt3','r'};
            end
            if strcmp(func2str(problem),'fsm')
                rhs = {'Fitness', 'Label','Type','TrainDelta','TestDelta','Standby1','Standby2','Standby3','Apfail1','Apfail2','Apfail3','Supported1','Supported2','Supported3','Limits1','Limits2','Limits3'};
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17]) = rhs;
            end
            H = height(T);
            disp('total no of rows:')
            disp(H);

            global d1;
            global m1;
            global d2;
            %Approach has three main stages. Preprocessing, training
            %surrogates and peforming predictions using surrogates

            %Condition to deal with the prediction stage
            if H > initialSimulationNum 
                idx = T.Type==0;
                simulationData = T(idx,:);
                percentTest = 0.20;
                initialData = simulationData(1:initialSimulationNum,:);
                initialDataHeight = height(initialData);
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %Train data and test data is created
                %simulationData is not capped to 300 points. It grows over
                %the iterations
                train_features = simulationData(round(percentTest*initialDataHeight):end, 6 : length(X)+5);
                Y = simulationData(round(percentTest*initialDataHeight):end,1);

                %initialData is capped to 300 points. The testing is done
                %always on 20% of 300 points.
                test_Y = initialData(1:round(percentTest*height(initialData)),1);
                test_features = initialData(1:round(percentTest*height(initialData)),6 : length(X)+5);
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %Convert target output table into array
                trainingData = [];
                %Condition to deal with the training stage
                if H == initialSimulationNum + 1
                    startTrain = tic;
                    
                    testData = simulationData(1:initialSimulationNum,:);
                    test_Y = testData(1:round(percentTest*height(initialData)),1);
                    test_features = testData(1:round(percentTest*height(initialData)),6 : length(X)+5);
                    disp('====================================')
%                     
                    predictorNames = getPredictorNames(func2str(problem));
                    %Ensemble model is trained by calling the trainEnsemble
                    %file
                    [model1,delta1,modelname1,traindelta1] = trainDynamic(train_features,Y,test_features,test_Y,X,text_file_location,predictorNames);
                    m1 = modelname1;
                    d1 = traindelta1;
                    d2 = delta1;
                    trainingTime = toc(startTrain);
                    trainingData(1) = trainingTime;

                    disp('initial training done!');
                    save('model1','model1');
                    inputArray(4) = traindelta1;
                    inputArray(5) = delta1;
                end

                    model1 = matfile('model1.mat');
                %Predict the fitness score using the trained surrogate
                %ensemble for each input
                [predictedFitness,uncertainty] = predictDynamic(train_features,Y,test_features,test_Y,X,model1.model1);

                fileID = fopen(text_file_location,'a');
                fprintf(fileID,'%d,%s,%f\n',H,m1,d1);
                fclose(fileID);

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %Check the reliablity of the surrogate's prediction
                reliable = reliability_check(predictedFitness,uncertainty,high,replace_dot);

                if reliable
                    type = 1;
                    PopObj = predictedFitness;
                else
                    type = 0;
                    PopObj=callSimulator(X,req,func2str(problem));
                    predictorNames = getPredictorNames(func2str(problem));
                    disp(height(train_features));
                    disp(height(Y));
                    [model1,delta1,modelname1,traindelta1] = trainDynamicSecondLoop(train_features,Y,test_features,test_Y,X,text_file_location,predictorNames);
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    m1 = modelname1;
                    d1 = traindelta1;
                    d2 = delta1;
                    save('model1','model1');
                end

                %label boundary or non boundary here
                label = labelPF(PopObj,high,replace_dot);

                inputArray(6:length(X)+5) = X;
                inputArray(1) = PopObj;
                inputArray(2) = label; %0 - F & 1 - P
                inputArray(3) = type; %0 - simulation & 1 - prediction
                inputArray(4) = d1;
                inputArray(5) = d2;
                
                path = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
                writematrix(inputArray,path,'WriteMode', 'append');
            else
                %Condition to handle data imbalance as part of
                %preprocessing
                if H == 150

                    path = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
                    smotelist = pyrunfile("smote.py","smotelist",path = path,problem = func2str(problem));
    
                    smoteMatrix = double(smotelist);
                    %Pass X and Label to get output from smote.py
                    for i  = 152:length(smoteMatrix)
                        eachInput = smoteMatrix(i,:);
                    %Simulating the predicted instances
                        actualPopObj=callSimulator(eachInput,req,func2str(problem));
                        label = labelPF(actualPopObj,high,replace_dot);
                        type=0;
            
                        inputArray(6:length(X)+5) = eachInput;
                        inputArray(1) = actualPopObj;
                        inputArray(2) = label; %0 - F & 1 - P
                        inputArray(3) = type; %0 - simulation & 1 - prediction
                        
                        path = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
                        writematrix(inputArray,path,'WriteMode', 'append');
                    end
                else
                    type = 0;
                    PopObj = callSimulator(X,req,func2str(problem));
                    %label boundary or non boundary here
                    label = labelPF(PopObj,high,replace_dot);
                    inputArray(6:length(X)+5) = X;
                    inputArray(1) = PopObj;
                    inputArray(2) = label; %0 - F & 1 - P
                    inputArray(3) = type; %0 - simulation & 1 - prediction
                    
                    path = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
                    writematrix(inputArray,path,'WriteMode', 'append');
                end
            end
            PopObj = -100;
end
