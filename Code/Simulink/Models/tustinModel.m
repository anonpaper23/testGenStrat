
%% Main
function FR = tustinModel(X,req)  
TimeSteps = 11;  
nbrInputs = 6;
    
curPath=fileparts(which('tustin.m')); 
addpath(curPath);
% Folder = fullfile(curPath, '../..');
% addpath(genpath(Folder));

modelpath = strrep(curPath,'Code\Simulink\Models','Benchmark\Simulink Models\2_tustin\');%
addpath(modelpath);
open_system(strcat(modelpath,'tustinintegrator.slx'));
cs = getActiveConfigSet('tustinintegrator');
model_cs = cs.copy;
set_param(model_cs, 'MaxDataPoints' ,' off', 'LimitDataPoints', 'off', 'ReturnWorkspaceOutputs','on','SaveOutput','on', 'OutputSaveName', 'varout');        

var.time = 0:1:10;

% Signals initialization 
    for j = 1:nbrInputs
        var.signals(j).values = zeros(TimeSteps,1);
        var.signals(j).dimensions =  1;
    end
                                  
    for i = 1:TimeSteps
        var.signals(1).values(i) =  X(i);
        var.signals(2).values(i)=  1;
        var.signals(3).values(i) =  1;
        var.signals(4).values(i) =  X(14);
        var.signals(5).values(i) =  X(12);
        var.signals(6).values(i) =  X(13);
    end
    
    % load data file
    save var_tustinR.mat var;
    load('var_tustinR.mat', 'var');

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
    
    Xin = var.signals(1).values;
    TL = var.signals(5).values(1);
    BL = var.signals(6).values(1);
    reset = var.signals(2).values(1);
    ic = var.signals(4).values(1);
    
    F = 100 * ones(size(t,1), 1); 
     
    switch req
        case 'R1a'            
        for i = 1:size(t,1)       
 %% Requirement #1a;         
        Ft = R01aObj(y(i), TL ,BL ,ic);
        F(i) = Ft;
        end    
        
        case 'R1b'            
        for i = 1:size(t,1)       
 %% Requirement #1b;         
        Ft = R01bObj(y(i), TL ,BL ,ic);
        F(i) = Ft;
        end
        
        case 'R1c'            
        for i = 1:size(t,1)       
 %% Requirement #1c;         
        Ft = R01cObj(y(i), TL ,BL ,ic);
        F(i) = Ft;
        end        
        
        case 'R1d'            
        for i = 1:size(t,1)       
 %% Requirement #1d;         
        Ft = R01dObj(y(i), TL ,BL ,ic);
        F(i) = Ft;
        end
        
        case 'R1e'            
        for i = 1:size(t,1)       
 %% Requirement #1d;         
        Ft = R01eObj(y(i), TL ,BL ,ic);
        F(i) = Ft;
        end
        
        case 'R2a'            
        for i = 1:size(t,1)       
 %% Requirement #2a;         
        Ft = R02aObj(y(i),TL, BL);
        F(i) = Ft;
        end    
        
        case 'R2b'            
        for i = 1:size(t,1)       
 %% Requirement #2b;         
        Ft = R02bObj(y(i),TL, BL);
        F(i) = Ft;
        end       
                             
    end
    
    FR = min(F);
    
end

%% Requirement #1;     
% If ic is bounded by TL and BL, yout = ic
%((reset and ic<=TL and ic>=BL) impl yout == ic); #1a 

% If ic is not bounded by TL and BL, yout = Saturation Point
%((reset and ic>=TL and TL>=BL) impl yout == TL); #1b 
%((reset and ic<=BL and TL>=BL) impl yout == BL); #1c
%((reset and ic>=BL and TL<BL) impl yout == BL); #1d
%((reset and ic<=TL and TL<BL) impl yout == TL); #1e 

%Violation: (reset will not be included since it is set as "1" by default.
%((reset and ic<=TL and ic>=BL) AND yout <> ic); #1a 
%((reset and ic>=TL and TL>=BL) AND yout <> TL); #1b 
%((reset and ic<=BL and TL>=BL) AND yout <> BL); #1c
%((reset and ic>=BL and TL<BL) AND yout <> BL); #1d
%((reset and ic<=TL and TL<BL) AND yout <> TL); #1e 
     
function R01a = R01aObj(y, TL ,BL ,ic)
  
%((reset and ic<=TL and ic>=BL) AND yout <> ic); #1a 
    term1 = round(ic - TL, 4);
    term2 = round(BL - ic, 4);
    term3 = 0.0001 - abs(round(y - ic, 4));
    
    R01a = max(max(term1, term2), term3);
end

function R01b = R01bObj(y, TL ,BL ,ic)
%((reset and ic>=TL and TL>=BL) AND yout <> TL); #1b 
    term4 = round(TL - ic, 4);
    term5 = round(BL - TL, 4);
    term6 = 0.0001 - abs(round(y - TL, 4));
    
    R01b = max(max(term4, term5), term6);
end

function R01c = R01cObj(y, TL ,BL ,ic)
%((reset and ic<=BL and TL>=BL) AND yout <> BL); #1c
    term7 = round(ic - BL, 4);
    term8 = round(BL - TL, 4);
    term9 = 0.0001 - abs(round(y - BL, 4));
    
    R01c = max(max(term7, term8), term9);
end

function R01d = R01dObj(y, TL ,BL ,ic)
%((reset and ic>=BL and TL<BL) AND yout <> BL); #1d
    term10 = round(BL - ic, 4);
    term11 = 0.0001 + round(TL - BL, 4);
    term12 = 0.0001 - abs(round(y - BL, 4));
    
    R01d = max(max(term10, term11), term12);
end

function R01e = R01eObj(y, TL ,BL ,ic)
%((reset and ic<=TL and TL<BL) AND yout <> TL); #1e 
    term13 = round(ic - TL, 4);
    term14 = 0.0001 + round(TL - BL, 4);
    term15 = 0.0001 - abs(round(y - TL, 4));
    
    R01e = max(max(term13, term14), term15);    
end
%  
%% Requirement #2; 
%(TL>=BL) impl (BL<=yout and yout<=TL); #2a
%(TL<BL) impl (TL<=yout and yout<=BL); #2b

%Violation:
%(TL>=BL) AND (BL>yout OR yout>TL); #2a
%(TL<BL) AND (TL>yout OR yout>BL); #2b
function R02a = R02aObj(y, TL ,BL)

%(TL>=BL) AND (BL>yout OR yout>TL); #2a
    term1 = round(BL - TL, 4);
    term2 = min(0.0001 - round(BL - y, 4), 0.0001 - round(y - TL, 4));    
    R02a = max(term1, term2);
end

function R02b = R02bObj(y, TL ,BL) 
 %(TL<BL) AND (TL>yout OR yout>BL); #2b       
    term1 = 0.0001 + round(TL - BL, 4);        
    term2 = min(0.0001 - round(TL - y, 4), 0.0001 - round(y - BL, 4));
    R02b = max(term1, term2);
          
end