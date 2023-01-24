%% Main
function FR = tustinr4Model(X,req)  
TimeSteps = 101;  
nbrInputs = 6;
    

curPath=fileparts(which('tustinr4.m'));
addpath(curPath);
% Folder = fullfile(curPath, '../..');
% addpath(genpath(Folder));
modelpath = strrep(curPath,'Code\Simulink\Models','Benchmark\Simulink Models\2_tustin\');    
addpath(modelpath);
open_system(strcat(modelpath,'tustinintegrator.slx'));
cs = getActiveConfigSet('tustinintegrator');
model_cs = cs.copy;
set_param(model_cs, 'MaxDataPoints' ,' off', 'LimitDataPoints', 'off', 'ReturnWorkspaceOutputs','on','SaveOutput','on', 'OutputSaveName', 'varout');        

var.time = 0:0.1:10;

% Signals initialization 
    for j = 1:nbrInputs
        var.signals(j).values = zeros(TimeSteps,1);
        var.signals(j).dimensions =  1;
    end                         
            
    for i = 1:TimeSteps
        %var.signals(1).values(i) =  1; % requirement 4-a
        var.signals(1).values(i) =  cos(var.time(i)); % requirement 4-b
        var.signals(2).values(i)=  0;
        var.signals(3).values(i) =  0.1;
        var.signals(4).values(i) =  X(3);
        var.signals(5).values(i) =  X(1);
        var.signals(6).values(i) =  X(2);
    end
    
    % load data file
    save var_tustin.mat var;
    load('var_tustin.mat', 'var');

    % Solve the problem of unrecognized external input variable 'var'
    hws = get_param('tustinintegrator', 'modelworkspace');
    list = whos;
    N = length(list);
    for  i = 1:N
        hws.assignin(list(i).name,eval(list(i).name));
    end
    
    simOut = sim('tustinintegrator.slx', model_cs);
    varout=simOut.get('varout');
    
    t = varout.time;
    y = varout.signals.values;
    %xin = var.signals(1).values(1); % requirement 4-a
    xin = var.signals(1).values; % requirement 4-b
    TL = var.signals(5).values(1);
    BL = var.signals(6).values(1);
    reset = var.signals(2).values(1);
    T = var.signals(3).values(1);
    ic = var.signals(4).values(1);
    F = 1000 * ones(size(t,1), 1);
  
    
        
 %% Requirement #4;  
%  The Output (yout) of this function shall approximate the Tustin 
%  integration of the value of the input signal (xin) over time within 
%  a specified tolerance, defined in the following subtests:
 
 %% Requirement #4a;

%   After 10 seconds of normal operation, the function computation 
%   at an execution frequency of 10 hz, yout should equal 10.0 within
%   a +/- 0.1 tolerance, for a constant xin equal to 1.0, and sample
%   delta time T equal to 0.1 seconds.

 %Requirement;
 %TL >= BL &&  y(i) >= BL  && y(i) <= TL )|| (BL >= TL && y(i) >= TL  && y(i) <= BL  )
 %=>( abs(yout{100}-10) <= 0.1 );
 %Violation;
 %TL >= BL &&  y(i) >= BL  && y(i) <= TL )|| (BL >= TL && y(i) >= TL  && y(i) <= BL  )
 %AND abs(yout{100}-10) > 0.1 
 
  %% Requirement #4b;
%  Over a 10 second computational duration at an execution frequency of 10 hz, 
%  the Output should equal the sine of time t, sin(t), where time is defined 
%  as a vector from 0 to 10 by increments of 0.1 seconds within a +/- 0.1 
%  tolerance for an input equal to the cosine of time t, cos(t), with the 
%  sample delta time T = 0.1 seconds when in normal mode of operation.     
 
 %Requirement;
 %TL >= BL &&  y(i) >= BL  && y(i) <= TL )|| (BL >= TL && y(i) >= TL  && y(i) <= BL  )
 %=>abs(y(i) - sin(t(i))) <= 0.1;
 
 %Violation;
 %TL >= BL &&  y(i) >= BL  && y(i) <= TL )|| (BL >= TL && y(i) >= TL  && y(i) <= BL  )
 % AND abs(y(i) - sin(t(i))) > 0.1;
 
    switch req
        case 'R4a'
            %Requirement #4a;  
            for i = 1:size(t,1)
                term1 = round(BL - TL, 4);
                term2 = round(BL - y(i), 4);
                term3 = round(y(i) - TL, 4);
        
                term4 = round(TL - BL, 4);
                term5 = round(TL - y(i), 4);
                term6 = round(y(i) - BL, 4);
                          
                term7 = 0.0001 - ((abs(y(i) - (i * T))) - 0.1); 
                Ft = max(min(max(max(term1, term2),term3),max(max(term4, term5),term6)),term7);
                F(i) = Ft;
            end
            

        case 'R4b'
            %Requirement #4b;
            for i = 1:size(t,1)
                term1 = round(BL - TL, 4);
                term2 = round(BL - y(i), 4);
                term3 = round(y(i) - TL, 4);
        
                term4 = round(TL - BL, 4);
                term5 = round(TL - y(i), 4);
                term6 = round(y(i) - BL, 4);
                                     
                term8 = 0.0001 - (abs(y(i) - sin(t(i))) - 0.1);
                Ft = max(min(max(max(term1, term2),term3),max(max(term4, term5),term6)),term8);
                F(i) = Ft;
            end  
    
    end 
          
    FR =  min(F); 
    
end 
