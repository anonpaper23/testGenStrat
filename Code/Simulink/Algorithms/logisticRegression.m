function PopObj = logisticRegression(run,element,mlmodel,initialSimNum,problem,req,evaluation)
            model_name = mlmodel;
            initialSimulationNum = initialSimNum;
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
                T.Properties.VariableNames([1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29]) = {'Fitness', 'Label','Type','TrainDelta','TestDelta','AP_Eng1','AP_Eng2','AP_Eng3','HDG_Mode1','HDG_Mode2','HDG_Mode3','ALT_Mode1','ALT_Mode2','ALT_Mode3','HDG_Ref1','HDG_Ref2','HDG_Ref3','TurnK1','TurnK2','TurnK3','ALT_Ref1','ALT_Ref2','ALT_Ref3','Pwheel1','Pwheel2','Pwheel3','Throttle1','Throttle2','Throttle3'}; % names of columns
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
            H = height(T);
            disp('total no of rows:')
            disp(H);

            if H > initialSimulationNum %Hyperparameter
                disp('Second logistic reg');

                %Generate x number of random points
                x = 5;
                points = [];
                for i = 1: x
                    inputVars = [];
                    if strcmp(func2str(problem),'tustin')
                        inputVars = ["Xin1","Xin2","Xin3","Xin4","Xin5","Xin6","Xin7","Xin8","Xin9","Xin10","Xin11","TL","BL","IC"];
                    elseif strcmp(func2str(problem),"tustinr4")
                        inputVars = ["TL","BL","IC"];
                    elseif strcmp(func2str(problem),"reg")
                        inputVars = ["A1","A2","B1","B2","C1","C2","D1","D2","E1","E2","F1","F2","G1","G2","H1","H2","I1","I2","J1","J2","K1","K2","L1","L2","M1","M2","N1","N2","O1","O2","P1","P2"];
                    elseif strcmp(func2str(problem),"autopilot")
                        inputVars = ["AP_Eng1","AP_Eng2","AP_Eng3","HDG_Mode1","HDG_Mode2","HDG_Mode3","ALT_Mode1","ALT_Mode2","ALT_Mode3","HDG_Ref1","HDG_Ref2","HDG_Ref3","TurnK1","TurnK2","TurnK3","ALT_Ref1","ALT_Ref2","ALT_Ref3","Pwheel1","Pwheel2","Pwheel3",'Throttle1','Throttle2','Throttle3'];
                    elseif strcmp(func2str(problem),"nlguidance")
                        inputVars = ["Xt1","Xt2","Xt3","Xv1","Xv2","Xv3","Vv1","Vv2","Vv3","Vt1","Vt2","Vt3","r"];
                    elseif strcmp(func2str(problem),"fsm")
                        inputVars = ["Standby1","Standby2","Standby3","Apfail1","Apfail2","Apfail3","Supported1","Supported2","Supported3","Limits1","Limits2","Limits3"];
                    end
                    disp(inputVars);
                    disp(length(inputVars))
        
                    %Tustin default range
                    PopDec = rand(1,length(inputVars));
                    if strcmp(func2str(problem),'tustin')
                        urangeXmax         = 20;    %Input Xin max. value
                        urangeXmin         = -20;   %Input Xin min. value
                        urangeTLBLMax         = 10;   %Input TL, BL value 
                        urangeTLBLMin         = -10;   %Input TL, BL  value 
                        for i = 1:11
                        PopDec(:,i) = (PopDec(:,i) * (urangeXmax - urangeXmin)) + urangeXmin;  % for Xin
                        end
                        
                        for i = 12:length(inputVars)-1  
                            PopDec(:,i) = (PopDec(:,i) * (urangeTLBLMax - urangeTLBLMin)) + urangeTLBLMin;  % for TL and BL      
                        end
                        
                        PopDec(:,14) =  (PopDec(:,14 ) * (urangeXmax - urangeXmin)) + urangeXmin; % for initial condition(ic)
                    end
                    if strcmp(func2str(problem),'tustinr4')
                        urangeXmax         = 20;    %Input Xin max. value
                        urangeXmin         = -20;   %Input Xin min. value
                        urangeTLBLMax         = 10;   %Input TL, BL value 
                        urangeTLBLMin         = -10;   %Input TL, BL  value 
                        for i = 1:length(inputVars)-1  
                            PopDec(:,i) = (PopDec(:,i) * (urangeTLBLMax - urangeTLBLMin)) + urangeTLBLMin;  % for TL and BL      
                        end
                        
                        PopDec(:,3) =  (PopDec(:,3 ) * (urangeXmax - urangeXmin)) + urangeXmin; % for initial condition(ic)
    
                    end
                    if strcmp(func2str(problem),'reg')
                        urangemin       = 0;
                        urangemax       = 5; 
                        PopDec = (rand(1,length(inputVars))* (urangemax - urangemin)) + urangemin;
    
                    end
                    if strcmp(func2str(problem),'autopilot')
                        urangeTurnKmax      = 45;           %Input Turn Knob value 
                        urangeTurnKmin      = 0;          %Input Turn Knob  value
                        urangeHDGRefmax     = 180;          %Input HDG Ref value 
                        urangeHDGRefmin     = -180;         %Input HDG Ref value        
                        urangeALTRefmin     = 0;            %Input ALT Ref value 
                        urangeALTRefmax     = 1000;         %Input ALT Ref value
                        urangePitchWmax     = 30;           %Input Pitch wheel value 
                        urangePitchWmin     = -30;          %Input Pitch wheel value
    
                        for i = 1:9                
                            PopDec(:,i) = round(rand);
                        end            
                        for i = 10:12                 
                            PopDec(:,i) = (PopDec(:,i) * (urangeHDGRefmax - urangeHDGRefmin)) + urangeHDGRefmin;  
                        end
                        for i = 13:15                 
                            PopDec(:,i) = (PopDec(:,i) * (urangeTurnKmax - urangeTurnKmin)) + urangeTurnKmin; 
                        end
                        for i = 16:18                
                            PopDec(:,i) = (PopDec(:,i) * (urangeALTRefmax - urangeALTRefmin)) + urangeALTRefmin;  
                        end
                        for i = 19:21                 
                            PopDec(:,i) = (PopDec(:,i) * (urangePitchWmax - urangePitchWmin)) + urangePitchWmin;  
                        end
                    end
                    if strcmp(func2str(problem),'nlguidance')
                        urangeMin      = -100;          
                        urangeMax      = 100;         
                        urangeRmin      = 0;
                        urangeRmax      = 100;
                        PopDec = rand(1,13);
                                
                        for i = 1:12                  
                            PopDec(:,i) = (PopDec(:,i) * (urangeMax - urangeMin)) + urangeMin;  
                        end   
                        PopDec(:,13) = (PopDec(:,13) * (urangeRmax - urangeRmin)) + urangeRmin;
                    end

                    if strcmp(func2str(problem),'fsm')
                        N = 12;
                        PopDec = round(rand(1,N)); % for booelan True or False; 1-0      

                    end
                    points = [points; [PopDec]];
                end
                npA = py.numpy.array(points);
                disp(npA);
                trainingData = [];
                finalList = pyrunfile("logGen.py","finalList",data_file_location = data_file_location,verify_file_location = verify_file_location,points=npA); %"finalListOfRanges"
                
                final = py.numpy.array(finalList);
                cP = double(final);
%                 disp(cP);
                %Number of closest points to select
                c = 1;
                
                for itr = 1:c
                    eachClosestPoint = cP(itr,:);
                    disp(eachClosestPoint)

% Only for AUTOPILOT REQ 12.1
%                     for i = 1:9                
%                         eachClosestPoint(:,i) = round(rand);
%                     end
% 
%                     for k = 16:18
%                         eachClosestPoint(:,k) = 6700;
%                     end
%ELSE eachClosestPoint is not modified.
                    %Simulating the predicted instances
                    actualPopObj = callSimulator(eachClosestPoint,req,func2str(problem));
                    high = getRange(func2str(problem),replace_dot);
                    actualLabel = labelPF(actualPopObj,high,replace_dot);
                    
                    %Write to a csv file
                    inputArray(6:length(X)+5) = eachClosestPoint;
                    inputArray(1) = actualPopObj;
                    inputArray(2) = actualLabel; %0 - F & 1 - P
                    inputArray(3) = 0; %0 - simulation
                    
                    path = strcat(data_path,func2str(problem),'_',replace_dot,'_regression_',string(evaluation),'_',model_name,'_',string(run),'.csv');
                    writematrix(inputArray,path,'WriteMode', 'append');
                end

            else
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
