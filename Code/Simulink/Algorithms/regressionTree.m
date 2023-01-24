%This function implements guided search with RT (Regression Tree) approach
%The preprocessing step is similar to the surrogate-assisted (SA) approach.

function PopObj = regressionTree(run,element,mlmodel,initialSimNum,problem,req,evaluation)

            model_name = mlmodel;
            initialSimulationNum = initialSimNum; %300
%             p = 'C:\Python-Workspace\thesis\FSE2023\Results\';
            p = '..\..\Results\';
            data_path = strcat(p,func2str(problem),'\');
            replace_dot = strrep(req,'.','_');
            data_file_location = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
            disp(data_file_location);
            
            verify_file_location = strcat(data_path,func2str(problem),'_',replace_dot,'_verify_',string(evaluation),'_',model_name,'_',string(run),'.csv');
            %Upper limit of boundary range used only in the certain
            %requirement cases where there are no negative values
            high = getRange(func2str(problem),replace_dot);

            inputArray = [];    
            X = element;

            T = readtable(data_file_location);
            
            if strcmp(func2str(problem), 'tustin')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','Xin1','Xin2','Xin3','Xin4','Xin5','Xin6','Xin7','Xin8','Xin9','Xin10','Xin11','TL','BL','IC'}; % names of columns
            end
            if strcmp(func2str(problem), 'tustinr4')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','TL','BL','IC'}; % names of columns
            end
            if strcmp(func2str(problem), 'autopilot')
                if strcmp(req,'R12.1')
                    T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','AP_Eng1','AP_Eng2','AP_Eng3','HDG_Mode1','HDG_Mode2','HDG_Mode3','ALT_Mode1','ALT_Mode2','ALT_Mode3','HDG_Ref1','HDG_Ref2','HDG_Ref3','TurnK1','TurnK2','TurnK3','ALT_Ref1','ALT_Ref2','ALT_Ref3','Pwheel1','Pwheel2','Pwheel3','Throttle1','Throttle2','Throttle3'}; % names of columns
                else
                    T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','AP_Eng1','AP_Eng2','AP_Eng3','HDG_Mode1','HDG_Mode2','HDG_Mode3','ALT_Mode1','ALT_Mode2','ALT_Mode3','HDG_Ref1','HDG_Ref2','HDG_Ref3','TurnK1','TurnK2','TurnK3','ALT_Ref1','ALT_Ref2','ALT_Ref3','Pwheel1','Pwheel2','Pwheel3'}; % names of columns
                end
            end
            if strcmp(func2str(problem), 'reg')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','A1','A2','B1','B2','C1','C2','D1','D2','E1','E2','F1','F2','G1','G2','H1','H2','I1','I2','J1','J2','K1','K2','L1','L2','M1','M2','N1','N2','O1','O2','P1','P2'};
            end
            if strcmp(func2str(problem), 'nlguidance')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','Xt1','Xt2','Xt3','Xv1','Xv2','Xv3','Vv1','Vv2','Vv3','Vt1','Vt2','Vt3','r'};
            end
            if strcmp(func2str(problem),'fsm')
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','Standby1','Standby2','Standby3','Apfail1','Apfail2','Apfail3','Supported1','Supported2','Supported3','Limits1','Limits2','Limits3'};
            end
            %H is reads the length of the dataset after generating the
            %input each iteration to keep track of the simulation budget.
            H = height(T);
            disp('total no of rows:')
            disp(H);
            %Second loop
            if H > initialSimulationNum
                disp('Second loop');
                trainingData = [];
                disp(data_file_location);
                %Python file with regression tree logic is called which
                %returns the refined ranges for inputs which is used for
                %guiding the search algorithm towards failure-inducing
                %inputs
                listOfBpaths = pyrunfile("extractRangesEpsilon.py","listOfBpaths",data_file_location = data_file_location,verify_file_location = verify_file_location); %"finalListOfRanges"
                cP = cell(listOfBpaths);

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %The ranges obtained from the python file is stored in a
                %.mat file which is saved in the workspace memory while the
                %process is performed.
                if H == initialSimulationNum+1
                    if strcmp(func2str(problem),'tustin')
                        [Xin1Max,Xin2Max,Xin3Max,Xin4Max,Xin5Max,Xin6Max,Xin7Max,Xin8Max,Xin9Max,Xin10Max,Xin11Max] = deal(20);
                        [Xin1Min,Xin2Min,Xin3Min,Xin4Min,Xin5Min,Xin6Min,Xin7Min,Xin8Min,Xin9Min,Xin10Min,Xin11Min] = deal(-20);
                        [TLMax,BLMax,ICMax] = deal(10);
                        [TLMin,BLMin,ICMin] = deal(-10);
                        prevTemplateArray = {{Xin1Min,Xin1Max},{Xin2Min,Xin2Max},{Xin3Min,Xin3Max},{Xin4Min,Xin4Max},{Xin5Min,Xin5Max},{Xin6Min,Xin6Max},...
                            {Xin7Min,Xin7Max},{Xin8Min,Xin8Max},{Xin9Min,Xin9Max},{Xin10Min,Xin10Max},{Xin11Min,Xin11Max},{TLMin,TLMax},{BLMin,BLMax},{ICMin,ICMax}};
                        save('.\Utilities\Ranges','prevTemplateArray');
                    end
                    if strcmp(func2str(problem),'tustinr4')
                        [TLMax,BLMax,ICMax] = deal(10);
                        [TLMin,BLMin,ICMin] = deal(-10);
                        prevTemplateArray = {{TLMin,TLMax},{BLMin,BLMax},{ICMin,ICMax}};
        %                 disp(templateArray{1}{2});
                        save('.\Utilities\Ranges','prevTemplateArray');
                    end
                    if strcmp(func2str(problem),'reg')
                        [A1Max,A2Max,B1Max,B2Max,C1Max,C2Max,D1Max,D2Max,E1Max,E2Max,F1Max,F2Max,G1Max,G2Max,H1Max,H2Max,I1Max,I2Max,J1Max,J2Max,K1Max,K2Max,L1Max,L2Max,M1Max,M2Max,N1Max,N2Max,O1Max,O2Max,P1Max,P2Max] = deal(5);
                        [A1Min,A2Min,B1Min,B2Min,C1Min,C2Min,D1Min,D2Min,E1Min,E2Min,F1Min,F2Min,G1Min,G2Min,H1Min,H2Min,I1Min,I2Min,J1Min,J2Min,K1Min,K2Min,L1Min,L2Min,M1Min,M2Min,N1Min,N2Min,O1Min,O2Min,P1Min,P2Min] = deal(0);
                        prevTemplateArray =  {{A1Max,A1Min},{A2Max,A2Min},{B1Max,B1Min},{B2Max,B2Min},{C1Max,C1Min},{C2Max,C2Min},{D1Max,D1Min},{D2Max,D2Min},{E1Max,E1Min},{E2Max,E2Min},{F1Max,F1Min},...
                            {F2Max,F2Min},{G1Max,G1Min},{G2Max,G2Min},{H1Max,H1Min},{H2Max,H2Min},{I1Max,I1Min},{I2Max,I2Min},{J1Max,J1Min},{J2Max,J2Min},{K1Max,K1Min},{K2Max,K2Min},...
                            {L1Max,L1Min},{L2Max,L2Min},{M1Max,M1Min},{M2Max,M2Min},{N1Max,N1Min},{N2Max,N2Min},{O1Max,O1Min},{O2Max,O2Min},{P1Max,P1Min},{P2Max,P2Min}};
                        save('.\Utilities\Ranges','prevTemplateArray');
                    end
                    if strcmp(func2str(problem),'autopilot')
                        if strcmp(req,'R12.1')
                            [AP_Eng1Max,AP_Eng2Max,AP_Eng3Max,HDG_Mode1Max,HDG_Mode2Max,HDG_Mode3Max,ALT_Mode1Max,ALT_Mode2Max,ALT_Mode3Max,Throttle1Max,Throttle2Max,Throttle3Max] = deal(1);
                            [AP_Eng1Min,AP_Eng2Min,AP_Eng3Min,HDG_Mode1Min,HDG_Mode2Min,HDG_Mode3Min,ALT_Mode1Min,ALT_Mode2Min,ALT_Mode3Min,Throttle1Min,Throttle2Min,Throttle3Min] = deal(0);
                        else
                            [AP_Eng1Max,AP_Eng2Max,AP_Eng3Max,HDG_Mode1Max,HDG_Mode2Max,HDG_Mode3Max,ALT_Mode1Max,ALT_Mode2Max,ALT_Mode3Max] = deal(1);
                            [AP_Eng1Min,AP_Eng2Min,AP_Eng3Min,HDG_Mode1Min,HDG_Mode2Min,HDG_Mode3Min,ALT_Mode1Min,ALT_Mode2Min,ALT_Mode3Min] = deal(0);
                        end
                        [HDG_Ref1Max,HDG_Ref2Max,HDG_Ref3Max] = deal(180);
                        [HDG_Ref1Min,HDG_Ref2Min,HDG_Ref3Min] = deal(-180);
                        [TurnK1Max,TurnK2Max,TurnK3Max] = deal(45);
                        [TurnK1Min,TurnK2Min,TurnK3Min] = deal(0);
                        [ALT_Ref1Max,ALT_Ref2Max,ALT_Ref3Max] = deal(1000);
                        [ALT_Ref1Min,ALT_Ref2Min,ALT_Ref3Min] = deal(0);
                        [Pwheel1Max,Pwheel2Max,Pwheel3Max] = deal(30);
                        [Pwheel1Min,Pwheel2Min,Pwheel3Min] = deal(-30);
                        
                        if strcmp(req,'R12.1')
                            prevTemplateArray = {{AP_Eng1Max,AP_Eng1Min},{AP_Eng2Max,AP_Eng2Min},{AP_Eng3Max,AP_Eng3Min},{HDG_Mode1Max,HDG_Mode1Min},{HDG_Mode2Max,HDG_Mode2Min},{HDG_Mode3Max,HDG_Mode3Min},...
                                {ALT_Mode1Max,ALT_Mode1Min},{ALT_Mode2Max,ALT_Mode2Min},{ALT_Mode3Max,ALT_Mode3Min},{HDG_Ref1Max,HDG_Ref1Min},{HDG_Ref2Max,HDG_Ref2Min},{HDG_Ref3Max,HDG_Ref3Min},...
                                {TurnK1Max,TurnK1Min},{TurnK2Max,TurnK2Min},{TurnK3Max,TurnK3Min},{ALT_Ref1Max,ALT_Ref1Min},{ALT_Ref2Max,ALT_Ref2Min},{ALT_Ref3Max,ALT_Ref3Min},...
                                {Pwheel1Max,Pwheel1Min},{Pwheel2Max,Pwheel2Min},{Pwheel3Max,Pwheel3Min},{Throttle1Max,Throttle1Min},{Throttle2Max,Throttle2Min},{Throttle3Max,Throttle3Min}};
                            save('.\Utilities\Ranges','prevTemplateArray');
                        else
                            prevTemplateArray = {{AP_Eng1Max,AP_Eng1Min},{AP_Eng2Max,AP_Eng2Min},{AP_Eng3Max,AP_Eng3Min},{HDG_Mode1Max,HDG_Mode1Min},{HDG_Mode2Max,HDG_Mode2Min},{HDG_Mode3Max,HDG_Mode3Min},...
                                {ALT_Mode1Max,ALT_Mode1Min},{ALT_Mode2Max,ALT_Mode2Min},{ALT_Mode3Max,ALT_Mode3Min},{HDG_Ref1Max,HDG_Ref1Min},{HDG_Ref2Max,HDG_Ref2Min},{HDG_Ref3Max,HDG_Ref3Min},...
                                {TurnK1Max,TurnK1Min},{TurnK2Max,TurnK2Min},{TurnK3Max,TurnK3Min},{ALT_Ref1Max,ALT_Ref1Min},{ALT_Ref2Max,ALT_Ref2Min},{ALT_Ref3Max,ALT_Ref3Min},...
                                {Pwheel1Max,Pwheel1Min},{Pwheel2Max,Pwheel2Min},{Pwheel3Max,Pwheel3Min}};
                            save('.\Utilities\Ranges','prevTemplateArray');
                        end
                    end
                    if strcmp(func2str(problem),'nlguidance')
                        [Xt1Max,Xt2Max,Xt3Max,Xv1Max,Xv2Max,Xv3Max,Vv1Max,Vv2Max,Vv3Max,Vt1Max,Vt2Max,Vt3Max,rMax] = deal(100);
                        [Xt1Min,Xt2Min,Xt3Min,Xv1Min,Xv2Min,Xv3Min,Vv1Min,Vv2Min,Vv3Min,Vt1Min,Vt2Min,Vt3Min] = deal(-100);
                        rMin = 0;

                        prevTemplateArray =  {{Xt1Max,Xt1Min},{Xt2Max,Xt2Min},{Xt3Max,Xt3Min},{Xv1Max,Xv1Min},{Xv2Max,Xv2Min},{Xv3Max,Xv3Min},{Vv1Max,Vv1Min},{Vv2Max,Vv2Min},{Vv3Max,Vv3Min},{Vt1Max,Vt1Min},{Vt2Max,Vt2Min},...
                            {Vt3Max,Vt3Min},{rMax,rMin}};
                        disp(prevTemplateArray);
                        save('.\Utilities\Ranges','prevTemplateArray');
                    end
                    if strcmp(func2str(problem),'fsm')
                        [Standby1Max,Standby2Max,Standby3Max,Apfail1Max,Apfail2Max,Apfail3Max,Supported1Max,Supported2Max,Supported3Max,Limits1Max,Limits2Max,Limits3Max] = deal(1);
                        [Standby1Min,Standby2Min,Standby3Min,Apfail1Min,Apfail2Min,Apfail3Min,Supported1Min,Supported2Min,Supported3Min,Limits1Min,Limits2Min,Limits3Min] = deal(0);

                        prevTemplateArray =  {{Standby1Max,Standby1Min},{Standby2Max,Standby2Min},{Standby3Max,Standby3Min},{Apfail1Max,Apfail1Min},{Apfail2Max,Apfail2Min},{Apfail3Max,Apfail3Min},{Supported1Max,Supported1Min},{Supported2Max,Supported2Min},{Supported3Max,Supported3Min},{Limits1Max,Limits1Min},{Limits2Max,Limits2Min},...
                            {Limits3Max,Limits3Min}};
                        disp(prevTemplateArray);
                        save('.\Utilities\Ranges','prevTemplateArray');
                    end
                end
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %Load the .mat file containing the information of input ranges
                prevTemplateArray = load(".\Utilities\Ranges.mat");

                path = cP;
%                 disp(path{1}{1});
%                 disp(path{1}{2}{1});
%                 disp(path{1}{2}{2});
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                %For each node for which a refined range has been found
                %from fitting the regression tree, parse the lower bound
                %and upper bound of the range namely "lowRange" and
                %"highRange". Then, update the .mat file with the new
                %ranges.

                %NOTE: In each iteration, regression tree returns
                %constraints only for a subset of inputs. Therefore, .mat
                %file is updated such that only the inputs with new ranges
                %are updated. For other inputs, the ranges from previous
                %range is used. 
                for i = 1: length(path)
                    varName = path{i}{1};
                    lowRange = path{i}{2}{1};
                    highRange = path{i}{2}{2};
                    
                    disp(varName);
                    disp(lowRange);
                    disp(highRange);

                    inputVars = [];
                    if strcmp(func2str(problem),'tustin')
                        inputVars = ["Xin1","Xin2","Xin3","Xin4","Xin5","Xin6","Xin7","Xin8","Xin9","Xin10","Xin11","TL","BL","IC"];
                    elseif strcmp(func2str(problem),"tustinr4")
                        inputVars = ["TL","BL","IC"];
                    elseif strcmp(func2str(problem),"reg")
                        inputVars = ["A1","A2","B1","B2","C1","C2","D1","D2","E1","E2","F1","F2","G1","G2","H1","H2","I1","I2","J1","J2","K1","K2","L1","L2","M1","M2","N1","N2","O1","O2","P1","P2"];
                    elseif strcmp(func2str(problem),"autopilot")
                        if strcmp(req,'R12.1')
                            inputVars = ["AP_Eng1","AP_Eng2","AP_Eng3","HDG_Mode1","HDG_Mode2","HDG_Mode3","ALT_Mode1","ALT_Mode2","ALT_Mode3","HDG_Ref1","HDG_Ref2","HDG_Ref3","TurnK1","TurnK2","TurnK3","ALT_Ref1","ALT_Ref2","ALT_Ref3","Pwheel1","Pwheel2","Pwheel3","Throttle1","Throttle2","Throttle3"];
                        else
                            inputVars = ["AP_Eng1","AP_Eng2","AP_Eng3","HDG_Mode1","HDG_Mode2","HDG_Mode3","ALT_Mode1","ALT_Mode2","ALT_Mode3","HDG_Ref1","HDG_Ref2","HDG_Ref3","TurnK1","TurnK2","TurnK3","ALT_Ref1","ALT_Ref2","ALT_Ref3","Pwheel1","Pwheel2","Pwheel3"];
                        end
                    elseif strcmp(func2str(problem),"nlguidance")
                        inputVars = ["Xt1","Xt2","Xt3","Xv1","Xv2","Xv3","Vv1","Vv2","Vv3","Vt1","Vt2","Vt3","r"];
                    elseif strcmp(func2str(problem),"fsm")
                        inputVars = ["Standby1","Standby2","Standby3","Apfail1","Apfail2","Apfail3","Supported1","Supported2","Supported3","Limits1","Limits2","Limits3"];
                    end

                    disp(inputVars);
                    disp(length(inputVars))
                    for v = 1:length(inputVars)
                        if lowRange > prevTemplateArray.prevTemplateArray{v}{1}
                        prevTemplateArray.prevTemplateArray{v}{1} = lowRange;
                        end
                        if highRange < prevTemplateArray.prevTemplateArray{v}{2}
                            prevTemplateArray.prevTemplateArray{v}{2} = highRange;
                        end
                    end
                end
    
                %Loop to generate n i.e. 5 number of inputs
        
                for j = 1: 5
                    disp('Generating points');
                    disp(j);
                    prevTemplateArray = load('.\Utilities\Ranges.mat');
                    PopDec = rand(1,length(inputVars));
                    disp(req)
                    if strcmp(req,"R12.1")
                        for iter = 1:9                
                            PopDec(:,iter) = round(rand);
                        end         
                        for k = 10:15 %1 -> 10 for AUTOPILOT
                            PopDec(:,k) =  (PopDec(:,k ) * (prevTemplateArray.prevTemplateArray{k}{2} - prevTemplateArray.prevTemplateArray{k}{1})) + prevTemplateArray.prevTemplateArray{k}{1};
                        end
                        for k = 16:18
                            PopDec(:,k) = 6700;
                        end
                        for k = 19:length(inputVars)
                            PopDec(:,k) =  (PopDec(:,k ) * (prevTemplateArray.prevTemplateArray{k}{2} - prevTemplateArray.prevTemplateArray{k}{1})) + prevTemplateArray.prevTemplateArray{k}{1};
                        end
                    elseif strcmp(req, 'R1_4_1')
                        for k = 1:9
                            PopDec(:,k) = round(rand);
                        end
                        for k = 10:length(inputVars)
                            PopDec(:,k) =  (PopDec(:,k ) * (prevTemplateArray.prevTemplateArray{k}{2} - prevTemplateArray.prevTemplateArray{k}{1})) + prevTemplateArray.prevTemplateArray{k}{1};
                        end
                    elseif strcmp(req, 'R1_6')
                        for k = 1:9
                            PopDec(:,k) = round(rand);
                        end
                        for k = 10:length(inputVars)
                            PopDec(:,k) =  (PopDec(:,k ) * (prevTemplateArray.prevTemplateArray{k}{2} - prevTemplateArray.prevTemplateArray{k}{1})) + prevTemplateArray.prevTemplateArray{k}{1};
                        end
                    else
                        for k = 1:length(inputVars)
                            PopDec(:,k) =  (PopDec(:,k ) * (prevTemplateArray.prevTemplateArray{k}{2} - prevTemplateArray.prevTemplateArray{k}{1})) + prevTemplateArray.prevTemplateArray{k}{1};
                        end
                    end

        %           Simulating the predicted instances
                    actualPopObj = callSimulator(PopDec,req,func2str(problem));
                    high = getRange(func2str(problem),replace_dot);
                    actualLabel = labelPF(actualPopObj,high,replace_dot);
                    
                    %Write to a csv file
                    inputArray(6:length(X)+5) = PopDec;
                    inputArray(1) = actualPopObj;
                    inputArray(2) = actualLabel; %0 - F & 1 - P
                    inputArray(3) = 0; %0 - simulation
                    
                    path = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
                    writematrix(inputArray,path,'WriteMode', 'append');
                end

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