function varargout = reg(Operation,Global,element)
    switch Operation
        case 'init'          
            urangemin       = 0;
            urangemax       = 5;            
            Global.D        = 32;                   % Size of X: Randomly generated (initially 32)
            Global.lower    = urangemin;
            Global.upper    = urangemax;
            Global.operator = @GMutation;
	
            PopDec = (rand(1,Global.D)* (urangemax - urangemin)) + urangemin;
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