%Entry point function to run test generation for Finite State Model (FSM) Simulink model
function executeFSMnewHCR()
            curPath=fileparts(which('executeFSMnewHCR.m')); 
            mainpath = strrep(curPath,'Scripts/HCvsRandom','');    
            addpath(mainpath);
            cHeader = {'Model','Requirement','Translation','Algorithm','Run', 'Iteration', 'Best Fitness', 'Current Fitness'};
            commaHeader = [cHeader;repmat({','},1,numel(cHeader))]; 
            commaHeader = commaHeader(:)';
            textHeader = cell2mat(commaHeader);
            fid = fopen('test_FSMnewHCR.csv','wt');
            fprintf(fid,'%s\n',textHeader);            
            fclose(fid);  

%% Finite State Machine-Test Generation %%        
        %Define path to log time data of each run in a CSV file
            timePath = '..\Results\fsm\';            
            %"runs" is an array containing integers that represents each
            %run of an approach. For example runs = [1,2,3,4,5] calls
            %eachRuns 5 times with the approach defined in "models" array.
            runs = [0];
            %Possible values: "ensemble","genLog","genRegTree","random","rf_ensemble","regtree","gpr","gpr_nonlinear","ls_boost","svr","nn"
            models = ["value"]; 
            %Specify a valid requirement to test for test generation
            %Possible values: "R1.4.1","R1.6","R12.1"
            req = ["R1.1"]; 
          
            for r = 1: length(runs)
                for i = 1: length(models)
                    for j = 1: length(req)
                    
                        timeData = [];
                        startTime = tic;
                        if strcmp(models(i),'ensemble')
                            %Ensemble 
%Parameter Definition of eachRun(Test generation startegy,Search Algorithm, Translation, Simulink model, Requirement, MaxIteration, Runs, Test generation startegy, First loop simulation budget, Total simulation budget)  
                            eachRun('ensemble',@ars,'R','FSM',req(j),1200,runs(r), models(i),300,320);
                        elseif strcmp(models(i),'genLog')
                            %Guided search - Logistic Regression
                            eachRun('generateLog',@ars,'R','FSM',req(j),1200,runs(r), models(i),300,338);
                        elseif strcmp(models(i),'genRegTree')
                            %Guided search - Regression Tree
                            eachRun('generate',@ars,'R','FSM',req(j),1200,runs(r), models(i),300,351);
                        elseif strcmp(models(i),"random")
                            %Random Search
                            eachRun('random',@ars,'R','FSM',req(j),1200,runs(r), models(i),300,354);
                        else
                            %Individual surrogates i.e nn, svr, rf, gpr,
                            %gprnl etc.
                            eachRun('optimize_r',@ars,'R','FSM',req(j),1200,runs(r), models(i),300,600);

                        end
                        endTime = toc(startTime);
                        %Log time data in milliseconds
                        timeData(1) = endTime;
                        tPath = strcat(timePath,models(i),'_timeTaken.csv');
                        writematrix(timeData,tPath,'WriteMode', 'append');
                        %Uncomment this line to evaluate the surrogate
                        %approach. I.e Uncomment if models array contains
                        %"ensemble" or any other individual surrogate
                        %models such as nn, svr, gpr, rf etc. 
%                         eachRun('verify',@ars,'R','FSM',req(j),1,runs(r), models(i),300,600);
                    end
                end
            end

end


function eachRun(simType,algorithm,translation,model,requirement,iteration,counter,mlmodel,initialSimNum,maxSimNum)
   
    if (translation == 'R')
        switch model 
            case 'AP'
                Vmodel = 'autopilot';
            case 'TT'
                Vmodel = 'twotanks';
            case 'NN'
                Vmodel = 'nn';
            case 'TU'
                Vmodel = 'tustin';
            case 'TU3'
                Vmodel = 'tustinr3';
            case 'TU4'
                Vmodel = 'tustinr4';
            case 'FSM'
                Vmodel = 'fsm';
        end
        Vproblem = strcat('@',Vmodel);
        Vproblem1 = str2func(Vproblem);
    end
    
    Vreq = requirement;
    
    curPath=fileparts(which('executeFSMnewHCR.m')); 
    mainpath = strrep(curPath,'Scripts/HCvsRandom','');    
    addpath(mainpath);
    index = 0;
    createCSV(counter,Vmodel,requirement,iteration,mlmodel);

    main('run',counter ,'-simType',simType,'-algorithm', algorithm, '-problem', Vproblem1,'-requirement',Vreq,'-operator', @GMutation, '-evaluation', iteration,'-mlmodel', mlmodel, 'initialSimNum',initialSimNum,'maxSimNum',maxSimNum);
      
    for i=1:iteration
        index = index+1;  
           
        m = matfile('fitmat.mat');
        BestFit = m.bestfit;
        CurrFit = m.curfit;
        C = {Vmodel,requirement,translation,strtok(func2str(algorithm),'@'),counter,index, BestFit(index),CurrFit(index)};       

        C = C.';    
        fid = fopen('test_APnewHCR.csv','a');
        fprintf(fid,'%s,%s,%s,%s,%d,%d,%4f,%4f\n',C{:});
        fclose(fid);
    end
end 

