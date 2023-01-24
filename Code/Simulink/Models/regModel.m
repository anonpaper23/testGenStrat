%% Main
function FR = regModel(X,req)

    curPath=fileparts(which('reg.m')); 
    addpath(curPath);
    modelpath = strrep(curPath,'Code\Simulink\Models','Benchmark\Simulink Models\3_regulators\');
    addpath(modelpath);
    cmd = load('-mat', 'regs_12B_data.mat');
    vars = fieldnames(cmd);
    for i = 1:length(vars)
        assignin('base', vars{i}, cmd.(vars{i}));
    end
    
    t= (0:0.01:10)';

    u= zeros(1,16);
    d=1;
    for j = 1:16
        u(1:500,j) = X(d);
        u(501:1001,j) = X(d+1);
        d = d +2;
    end
%     disp(u);
%     u(1:500,14) = 66;
%     u(501:1001,14) = 0;
%     u(1:500,8) = 0;
%     u(501:1001,8) = 66;
    save input.mat u;
    load('input.mat', 'u');
    open_system(strcat(modelpath,'regs_12B.slx'));
    cs = getActiveConfigSet('regs_12B');
    model_cs = cs.copy;
    set_param(model_cs, 'MaxDataPoints' ,' off', 'LimitDataPoints', 'off', 'ReturnWorkspaceOutputs','on','SaveOutput','on', 'OutputSaveName', 'varout');        
    % Solve the problem of unrecognized external input variable 'var'
    hws = get_param('regs_12B', 'modelworkspace');
    list = whos;
    N = length(list);
    for  i = 1:N
        hws.assignin(list(i).name,eval(list(i).name));
    end
    simOut = sim('regs_12B.slx', model_cs);
    varout=simOut.get('varout');
    lcvdt_cmd_fcs_dps2 = varout(:,1);
    mcvdt_cmd_fcs_dps2 = varout(:,2);
    ncvdt_cmd_fcs_dps2 = varout(:,3);
    xcvdt_cmd_fcs_dps2 = varout(:,4);
    hcvdt_cmd_fcs_dps2 = varout(:,5);
    
    F = 10000 * ones(size(lcvdt_cmd_fcs_dps2,1), 1);
    
    switch req
        case 'R1'       
            %% R1                     
            FR = R1Obj(lcvdt_cmd_fcs_dps2);
        case 'R2'
            %% R2                     
            FR = R2Obj(mcvdt_cmd_fcs_dps2);
        case 'R3'
            %% R3                     
            FR = R3Obj(ncvdt_cmd_fcs_dps2);
        case 'R4'
            %% R4                     
            FR = R4Obj(xcvdt_cmd_fcs_dps2);
        case 'R5'
            %% R5                     
            FR = R5Obj(hcvdt_cmd_fcs_dps2);
        case 'R6'
            %% R6 
            for i = 2:size(ncvdt_cmd_fcs_dps2,1)
                F(i-1) = R6Obj(lcvdt_cmd_fcs_dps2(i),lcvdt_cmd_fcs_dps2(i-1));
            end
            FR = min(F);    
        case 'R7'
            %% R7
            for i = 2:size(mcvdt_cmd_fcs_dps2,1)
                F(i-1) = R7Obj(mcvdt_cmd_fcs_dps2(i),mcvdt_cmd_fcs_dps2(i-1));
            end
            FR = min(F)*5; %Stretched the distribution here    
        case 'R8'
            %% R8 
            for i = 2:size(ncvdt_cmd_fcs_dps2,1)
                F(i-1) = R8Obj(ncvdt_cmd_fcs_dps2(i),ncvdt_cmd_fcs_dps2(i-1));
            end
            FR = min(F);
        case 'R9'
            %% R9                     
            for i = 2:size(xcvdt_cmd_fcs_dps2,1)
                F(i-1) = R9Obj(xcvdt_cmd_fcs_dps2(i),xcvdt_cmd_fcs_dps2(i-1));
            end
            FR = min(F);
        case 'R10'
            %% R10                     
            for i = 2:size(hcvdt_cmd_fcs_dps2,1)
                F(i-1) = R10Obj(hcvdt_cmd_fcs_dps2(i),hcvdt_cmd_fcs_dps2(i-1));
            end
            FR = min(F);
        otherwise % by default R1 
            %% R1                     
            FR = R1Obj(lcvdt_cmd_fcs_dps2);
    end
        
end

%% Requirement #1; 
function R1 = R1Obj(lcvdt_cmd_fcs_dps2) 
 
%     F{1:100}(|lcvdt_cmd_fcs_dps2(t)| <= 50) AND F{2:101}(|lcvdt_cmd_fcs_dps2(t)| <= 50) AND .. AND F{901:1001}(|lcvdt_cmd_fcs_dps2(t)| <= 50)
%     G{1:100}(|lcvdt_cmd_fcs_dps2(t)| > 50) OR G{2:101}(|lcvdt_cmd_fcs_dps2(t)| > 50) OR .. OR G{901:1001}(|lcvdt_cmd_fcs_dps2(t)| > 50)

    term1 = zeros(100,1);
    term= zeros(901,1);
    for t = 1:901
        for i= t:t+99
            if (abs(lcvdt_cmd_fcs_dps2(i)) - 50 > 0)
                term1(i) = 0;
            else
                term1(i) = (-1) * (abs(lcvdt_cmd_fcs_dps2(i)) - 50) + 1;
            end
        end
        term(t) = sum(term1);
    end
    
    R1 = min(term);  
end
%% Requirement #2;
function R2 = R2Obj(mcvdt_cmd_fcs_dps2) 
 
%Requirement     F{1:100}(|mcvdt_cmd_fcs_dps2(t)| <= 50) AND F{2:101}(|mcvdt_cmd_fcs_dps2(t)| <= 50) AND .. AND F{901:1001}(|mcvdt_cmd_fcs_dps2(t)| <= 50)
%Violtation     G{1:100}(|mcvdt_cmd_fcs_dps2(t)| > 50) OR G{2:101}(|mcvdt_cmd_fcs_dps2(t)| > 50) OR .. OR G{901:1001}(|mcvdt_cmd_fcs_dps2(t)| > 50)

    term1 = zeros(100,1);
    term= zeros(901,1);
    for t = 1:901
        for i= t:t+99
            if (abs(mcvdt_cmd_fcs_dps2(i)) - 50 > 0)
                term1(i) = 0;
            else
                term1(i) = (-1) * (abs(mcvdt_cmd_fcs_dps2(i)) - 50) + 1;
            end
        end
        term(t) = sum(term1);
    end
    
    R2 = min(term);  
end 
%% Requirement #3;
function R3 = R3Obj(ncvdt_cmd_fcs_dps2) 
 
%     F{1:100}(|ncvdt_cmd_fcs_dps2(t)| <= 50) AND F{2:101}(|ncvdt_cmd_fcs_dps2(t)| <= 50) AND .. AND F{901:1001}(|ncvdt_cmd_fcs_dps2(t)| <= 50)
%     G{1:100}(|ncvdt_cmd_fcs_dps2(t)| > 50) OR G{2:101}(|ncvdt_cmd_fcs_dps2(t)| > 50) OR .. OR G{901:1001}(|ncvdt_cmd_fcs_dps2(t)| > 50)

    term1 = zeros(100,1);
    term= zeros(901,1);
    for t = 1:901
        for i= t:t+99
            if (abs(ncvdt_cmd_fcs_dps2(i)) - 50 > 0)
                term1(i) = 0;
            else
                term1(i) = (-1) * (abs(ncvdt_cmd_fcs_dps2(i)) - 50) + 1;
            end
        end
        term(t) = sum(term1);
    end
    
    R3 = min(term);  
end
%% Requirement #4;
function R4 = R4Obj(xcvdt_cmd_fcs_dps2) 
 
%     F{1:100}(|xcvdt_cmd_fcs_dps2(t)| <= 32) AND F{2:101}(|xcvdt_cmd_fcs_dps2(t)| <= 32) AND .. AND F{901:1001}(|xcvdt_cmd_fcs_dps2(t)| <= 32)
%     G{1:100}(|xcvdt_cmd_fcs_dps2(t)| > 32) OR G{2:101}(|xcvdt_cmd_fcs_dps2(t)| > 32) OR .. OR G{901:1001}(|xcvdt_cmd_fcs_dps2(t)| > 32)

    term1 = zeros(100,1);
    term= zeros(901,1);
    for t = 1:901
        for i= t:t+99
            if (abs(xcvdt_cmd_fcs_dps2(i)) - 32 > 0)
                term1(i) = 0;
            else
                term1(i) = (-1) * (abs(xcvdt_cmd_fcs_dps2(i)) - 32) + 1;
            end
        end
        term(t) = sum(term1);
    end
    
    R4 = min(term);  
end
%% Requirement #5;
function R5 = R5Obj(hcvdt_cmd_fcs_dps2) 
 
%     F{1:100}(|hcvdt_cmd_fcs_dps2(t)| <= 32) AND F{2:101}(|hcvdt_cmd_fcs_dps2(t)| <= 32) AND .. AND F{901:1001}(|hcvdt_cmd_fcs_dps2(t)| <= 32)
%     G{1:100}(|hcvdt_cmd_fcs_dps2(t)| > 32) OR G{2:101}(|hcvdt_cmd_fcs_dps2(t)| > 32) OR .. OR G{901:1001}(|hcvdt_cmd_fcs_dps2(t)| > 32)

    term1 = zeros(100,1);
    term= zeros(901,1);
    for t = 1:901
        for i= t:t+99
            if (abs(hcvdt_cmd_fcs_dps2(i)) - 32 > 0)
                term1(i) = 0;
            else
                term1(i) = (-1) * (abs(hcvdt_cmd_fcs_dps2(i)) - 32) + 1;
            end
        end
        term(t) = sum(term1);
    end
    
    R5 = min(term);  
end
%% Requirement #6;
function R6 = R6Obj(lcvdt_cmd_fcs_dps2,lcvdt_cmd_fcs_dps2p) 
 
% Requirement: (lcvdt_cmd_fcs_dps2(t) - lcvdt_cmd_fcs_dps2(t-1)) <= 0.5
% Violation: (lcvdt_cmd_fcs_dps2(t) - lcvdt_cmd_fcs_dps2(t-1)) > 0.5

   if (lcvdt_cmd_fcs_dps2 - lcvdt_cmd_fcs_dps2p - 0.5 > 0)
        term = 0;
   else
        term = (-1) * (lcvdt_cmd_fcs_dps2 - lcvdt_cmd_fcs_dps2p - 0.5) + 1;
   end
    
    R6 = term;  
end
%% Requirement #7;
function R7 = R7Obj(mcvdt_cmd_fcs_dps2,mcvdt_cmd_fcs_dps2p) 
 
% Requirement: (mcvdt_cmd_fcs_dps2(t) - mcvdt_cmd_fcs_dps2(t-1)) <= 0.5
% Violation: (mcvdt_cmd_fcs_dps2(t) - mcvdt_cmd_fcs_dps2(t-1)) > 0.5

   if (mcvdt_cmd_fcs_dps2 - mcvdt_cmd_fcs_dps2p - 0.5 > 0)
        term = 0;
   else
        term = (-1) * (mcvdt_cmd_fcs_dps2 - mcvdt_cmd_fcs_dps2p - 0.5) + 1;
   end
    
    R7 = term;  
end
%% Requirement #8;
function R8 = R8Obj(ncvdt_cmd_fcs_dps2,ncvdt_cmd_fcs_dps2p) 
 
% Requirement: (ncvdt_cmd_fcs_dps2(t) - ncvdt_cmd_fcs_dps2(t-1)) <= 0.5
% Violation: (ncvdt_cmd_fcs_dps2(t) - ncvdt_cmd_fcs_dps2(t-1)) > 0.5

   if (ncvdt_cmd_fcs_dps2 - ncvdt_cmd_fcs_dps2p - 0.5 > 0)
        term = 0;
   else
        term = (-1) * (ncvdt_cmd_fcs_dps2 - ncvdt_cmd_fcs_dps2p - 0.5) + 1;
   end
    
    R8 = term;  
end
%% Requirement #9;
function R9 = R9Obj(xcvdt_cmd_fcs_dps2,xcvdt_cmd_fcs_dps2p) 
 
% Requirement: (xcvdt_cmd_fcs_dps2(t) - xcvdt_cmd_fcs_dps2(t-1)) <= 0.5
% Violation: (xcvdt_cmd_fcs_dps2(t) - xcvdt_cmd_fcs_dps2(t-1)) > 0.5

   if (xcvdt_cmd_fcs_dps2 - xcvdt_cmd_fcs_dps2p - 0.32 > 0)
        term = 0;
   else
        term = (-1) * (xcvdt_cmd_fcs_dps2 - xcvdt_cmd_fcs_dps2p - 0.32) + 1;
   end
    
    R9 = term;  
end
%% Requirement #10;
function R10 = R10Obj(hcvdt_cmd_fcs_dps2,hcvdt_cmd_fcs_dps2p) 
 
% Requirement: (hcvdt_cmd_fcs_dps2(t) - hcvdt_cmd_fcs_dps2(t-1)) <= 0.5
% Violation: (hcvdt_cmd_fcs_dps2(t) - hcvdt_cmd_fcs_dps2(t-1)) > 0.5

   if (hcvdt_cmd_fcs_dps2 - hcvdt_cmd_fcs_dps2p - 0.32 > 0)
       term = 0;
   else
        term = (-1) * (hcvdt_cmd_fcs_dps2 - hcvdt_cmd_fcs_dps2p - 0.32) + 1;
   end
    
    R10 = term;  
end
