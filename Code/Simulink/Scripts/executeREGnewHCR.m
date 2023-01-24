%Entry point function to run test generation for Regulator Simulink model
function executeREGnewHCR()
            curPath=fileparts(which('executeREGnewHCR.m')); 
            mainpath = strrep(curPath,'Scripts/HCvsRandom','');    
            addpath(mainpath);
            cHeader = {'Model','Requirement','Translation','Algorithm','Run', 'Iteration', 'Best Fitness', 'Current Fitness'};
            commaHeader = [cHeader;repmat({','},1,numel(cHeader))]; 
            commaHeader = commaHeader(:)';
            textHeader = cell2mat(commaHeader);
            fid = fopen('test_TUnewHCR.csv','wt');
            fprintf(fid,'%s\n',textHeader);            
            fclose(fid);  
 
%% TUSTIN-HC-Robustness  
            %Define path to log time data of each run in a CSV file
            timePath = '..\Results\reg\';
            %"runs" is an array containing integers that represents each
            %run of an approach. For example runs = [1,2,3,4,5] calls
            %eachRuns 5 times with the approach defined in "models" array.
            runs = [0];
            %Specify a valid requirement to test for test generation
            %Possible values: "R7"
            req = ["R7"];

            %Possible values: "ensemble","genLog","genRegTree","random","rf_ensemble","regtree","gpr","gpr_nonlinear","ls_boost","svr","nn"
            models = ["ensemble"]; 
            for r = 1:length(runs)
                for i = 1:length(models)
                    for j = 1:length(req)
                        timeData = [];
                        startTime = tic;
                        if strcmp(models(i),'genLog')
                            %Guided search - Logistic Regression
%Parameter Definition of eachRun(Test generation startegy,Search Algorithm, Translation, Simulink model, Requirement, MaxIteration, Runs, Test generation startegy, First loop simulation budget, Total simulation budget)  
                            eachRun('generateLog',@ars,'R','REG',req(j),3500,runs(r), models(i),300,608);
                        elseif strcmp(models(i),'genRegTree')
                            %Guided search - Regression Tree
                            eachRun('generate',@ars,'R','REG',req(j),3500,runs(r), models(i),300,617);
                        elseif strcmp(models(i),'ensemble')
                            %Ensemble 
                            eachRun('ensemble',@ars,'R','REG',req(j),3500,runs(r), models(i),300,600);
                        elseif strcmp(models(i),"random")
                            eachRun('random',@ars,'R','REG',req(j),3500,runs(r), models(i),300,600);
                        else
                            %Individual surrogates i.e nn, svr, rf, gpr,
                            %gprnl etc.
                            eachRun('optimize_r',@ars,'R','REG',req(j),3500,runs(r), models(i),300,600);
                        end
                        endTime = toc(startTime);
                        timeData(1) = endTime;
                        tPath = strcat(timePath,models(i),'_timeTaken.csv');
                        writematrix(timeData,tPath,'WriteMode','append');
                        %Uncomment this line to evaluate the surrogate
                        %approach. I.e Uncomment if models array contains
                        %"ensemble" or any other individual surrogate
                        %models such as nn, svr, gpr, rf etc. 

                        eachRun('verify',@ars,'R','REG',req(j),1,runs(r), models(i),300,600);
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
            case 'REG'
                Vmodel = 'reg';
        end
        Vproblem = strcat('@',Vmodel);
        Vproblem1 = str2func(Vproblem);
    end
    
    Vreq = requirement;
    
    curPath=fileparts(which('executeAPnewHCR.m')); 
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
