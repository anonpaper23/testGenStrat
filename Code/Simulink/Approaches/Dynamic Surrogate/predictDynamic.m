function [predictedFitness,uncertainty] = predictEnsemble(train_features,train_Y,test_features,test_Y,X,model1)
            trainTableCheck = istable(train_Y);
            if trainTableCheck
                YArray = table2array(train_Y);
            end
            testTableCheck = istable(test_Y);
            if testTableCheck
                YTestArray = table2array(test_Y);
            end
            disp(model1);
            disp('Above model used for Prediction')
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            %Calculate error between predicted fitness and actual fitness
            %of test data i.e. Find test error

%             model1TrainPred = predict(model1,train_features);
            model1TestPred = predict(model1,test_features);
            model1PredictedFitness = predict(model1,X);
%             model1TrainError = mae(YArray - model1TrainPred);
            model1TestError = mae(YTestArray - model1TestPred);

            predictedFitness = model1PredictedFitness;
            uncertainty = model1TestError;
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
