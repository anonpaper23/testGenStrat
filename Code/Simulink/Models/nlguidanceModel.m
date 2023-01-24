%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%    Main
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

function FR = nlguidanceModel(X,req) 
                  
    TimeSteps = 501;
    nbrInputs = 4;
    
    curPath=fileparts(which('nlguidance.m'));
    addpath(curPath);
    modelpath = strrep(curPath,'Code\Simulink\Models','Benchmark\Simulink Models\4_nlguide\');
    addpath(modelpath);    
    open_system(strcat(modelpath,'NLGuidance.slx'));
    
    var.time = 0:0.02:10; 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%    Inputs
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%     
    for s = 1:nbrInputs
        var.signals(s).values = zeros(TimeSteps,3);              % Signals initialization
        var.signals(s).dimensions = 3; 
    end
    % Signals preprocessing
        mv = 2 * rand(TimeSteps,3) - 1;            % matrix of vehicle velocities in [0 1]       
        %mt = sort(2 * rand(501,3) - 1,'descend');                % Target is fix       
        d = 1;
        for j = 1:3
            var.signals(1).values(1,j) = X(1,d);         % initial positions
            var.signals(2).values(1,j) = X(1,d+1);
            d = d+2;
        end
        for s = 3:4
            for j = 1:3
                var.signals(s).values(:,j) = X(1,d);         % initial Velocities
                d = d+1;
            end
        end
        var.signals(3).values = var.signals(3).values + mv;     % Final vehicle velocity        
        var.signals(4).values(:) = 0;
        var.signals(5).values = X(1,d)*ones(1,TimeSteps)';                                                      
        var.signals(5).dimensions = 1; 
        
        % Compute positions having velocities.
        for j = 1:3
            for i = 2:TimeSteps
                %var.signals(5).values(i,1) = 5*i;
                var.signals(1).values(i,j) = var.signals(1).values(i-1,j)+var.signals(4).values(i,j);
                var.signals(2).values(i,j) = var.signals(2).values(i-1,j)+var.signals(3).values(i,j);        
            end
        end
        
        var.signals(1).values(:,3) = abs(var.signals(1).values(:,3));       % Altitudes should be positive 
        var.signals(2).values(:,3) = abs(var.signals(2).values(:,3));       
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%    Simulation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%        
        
        hws = get_param('NLGuidance', 'modelworkspace');
        list = whos;       
        N = length(list);
        for  i = 1:N
             hws.assignin(list(i).name,eval(list(i).name));
        end
        
        % Run simulation and get simOut
        simOut = sim(fullfile(modelpath,'NLGuidance.slx'),'ReturnWorkspaceOutputs','on','SaveOutput','on','OutputSaveName','varout');                    
        varout = simOut.get('varout');
        cmd1 = simOut.get('cmd1');
        cmd2 = simOut.get('cmd2');
        Xap1 = simOut.get('Xap1');
        Xap2 = simOut.get('Xap2');

                
        Xtarg = var.signals(1).values;      % Target position vector
        Xv = var.signals(2).values;         % Vehicle position vector
        Vv = var.signals(3).values;         % Vehicle velocity vector                             
        Vt = var.signals(4).values;         % Target velocity vector
        yout = varout.signals.values;       % Aim point Position vector  
        
        Ft = 1000 * ones(TimeSteps,1); 
        switch req
            case 'R6'     
                for t = 2:size(Xap2,1)
                    Ft(t-1) = R6Obj(yout(t-1,1:3),yout(t,1:3),Xap2(t-1,1:3),Xap2(t,1:3), Vv(t,1:3),Vt(t,1:3));            
                end
        end     
        FR=  min(Ft);

end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Requirements
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%% Requirement #6; 

% Req:        G((yout = Xap2) AND (youtp = Xap2p)) ==> 
%             G((yout-youtp)^2 - 0.0004 * (Vv + Vt)^2) <= 0)
% Violation:   F( (yout = Xap2) AND (youtp = Xap2p) AND
%               (yout-youtp)^2 - 0.0004 (Vv + Vt)^2 ) > 0 )


% diff = (yout-youtp)^2 - 0.0004 (Vv + Vt)^2  

function R6 = R6Obj(youtp,yout,Xap2p,Xap2,Vv,Vt)
    sumOut = 0; sumVl  = 0; 
    for i= 1:3
        sumOut = sumOut + (yout(i)-youtp(i))^2;
        sumVl  = sumVl  + (Vv(i)+Vt(i))^2; 
    end 
    diff = sumOut - (0.0004 * sumVl);

    term1 = 0.0001 - diff;
    max1 = 0; max2 = 0;
    for i= 1:3
        max1 = max(max1, abs(yout(i) - Xap2(i)));
        max2  = max(max2, abs(youtp(i) - Xap2p(i))); 
    end 
    max3 = max(max1,max2);
    term = max(max3,term1);
    if (term>=0)
        term5 = term/(term+1);
    else
        term5 = (-1)*abs(term)/(abs(term)+1);
    end
    if (term1>=0)
        term6 = term1/(term1+1);
    else
        term6 = (-1)*abs(term1)/(abs(term1)+1);
    end
    if (term == 0)
        v = term6;
    else
        v = term5;
    end
    R6 = v;
        
end        
