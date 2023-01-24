
function [trainPrediction,testPrediction,predictedFitness] = predictModels(train_features,test_features,X,model_name,model)
        disp('separate file called');
 
        switch model_name
            case 'svr'
                disp('svm');

                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);
                disp("Predicted fitness value:")
                disp(predictedFitness);

    
            case 'regtree'
                disp('regtree');
                disp(train_features);
                disp(model);
                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);
                disp("Predicted fitness value:")
                disp(predictedFitness);
 
 
            case 'rf_ensemble'
                disp('rf');

                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);
                disp("Predicted fitness value:")
                disp(predictedFitness);


            case 'ls_boost'
                disp('ls');

                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);

            case 'gpr'
                disp('gpr');

                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);


            case 'gpr_nonlinear'
                disp('gpr non linear');

                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);


            case 'nn'
                disp('regression neural network');

                trainPrediction = predict(model,train_features);
                testPrediction = predict(model,test_features);
                predictedFitness = predict(model,X);

        end

end