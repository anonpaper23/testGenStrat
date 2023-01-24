function varargout = fsm(Operation,Global,element)
    switch Operation
        case 'init'
 
            Global.D        = 12; %4 inputs
            Global.lower    = zeros(1,Global.D);
            Global.upper    = ones(1,Global.D);
            Global.operator = @BFlipMutation;
            Global.localop = 150;
            
            PopDec = round(rand(1,Global.D)); % for boolean True or False; 1-0                       

            varargout = {PopDec};
            
        case 'random'  
            disp('Called value case'); 
            PopObj = randomSearch(Global.run,element,Global.problem,Global.requirement,Global.evaluation);
            varargout = {PopObj};

        case 'optimize_r'
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