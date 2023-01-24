function varargout = tustinr4(Operation,Global,element)
    switch Operation
        case 'init'
            urangeXmax         = 20;    %Input Xin max. value
            urangeXmin         = -20;   %Input Xin min. value
            urangeTLBLMax         = 10;   %Input TL, BL value 
            urangeTLBLMin         = -10;   %Input TL, BL  value 
            Global.D        = 3; 
            Global.lower    = [urangeTLBLMin * ones(1,2), urangeXmin];
            Global.upper    = [urangeTLBLMax * ones(1,2), urangeXmax];
            Global.operator = @GMutation;
            Global.localop = 244; %for adaptive walk
            Global.law = 34;         % for hcrr alg
            
            PopDec = rand(1,Global.D);
            
            for i = 1:Global.D-1  
            PopDec(:,i) = (PopDec(:,i) * (urangeTLBLMax - urangeTLBLMin)) + urangeTLBLMin;  % for TL and BL      
            end
            
            PopDec(:,3) =  (PopDec(:,3 ) * (urangeXmax - urangeXmin)) + urangeXmin; % for initial condition(ic)

            varargout = {PopDec};

        case 'random'  
            disp('Called Random Search');
            PopObj = randomSearch(Global.run,element,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'optimize_r'
            disp('Called regression approach');
            PopObj = individualSurrogate(Global.run,element,Global.mlmodel,Global.initialSimNum,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'ensemble'
            disp('Called ensemble approach');
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