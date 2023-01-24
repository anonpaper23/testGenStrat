function varargout = nlguidance(Operation,Global,element)
% <problem> <4_nlguide>
% Nonlinear Guidance
% operator --- EAreal
% Test scenario when the target is fix

    switch Operation
        case 'init'
            urangeMin      = -100;          
            urangeMax      = 100;         
            urangeRmin      = 0;
            urangeRmax      = 100;
            Global.D        = 13;                   % Size of X: positions of V and T: initial points randomly generated 
            Global.lower    = zeros(1,Global.D) - 100 * ones(1,Global.D);
            Global.upper    = 100 * ones(1,Global.D);
            Global.operator = @GMutation;
            Global.localop = 78;
            Global.law = 63;         % for hcrr alg
            PopDec = rand(1,Global.D);
            
                                          
            for i = 1:12                  
                PopDec(:,i) = (PopDec(:,i) * (urangeMax - urangeMin)) + urangeMin;  
            end   
            PopDec(:,13) = (PopDec(:,13) * (urangeRmax - urangeRmin)) + urangeRmin;
            
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
            disp('Called dynamic approach');
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

        