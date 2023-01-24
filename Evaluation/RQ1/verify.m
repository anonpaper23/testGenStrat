function PopObj = verify(run,element,mlmodel,problem,req)

            X = element;
            replace_dot = strrep(req,'.','_');
            maxIterationsNum = 3500;
            high = getBoundaryRange(func2str(problem),replace_dot);
            inputArray = [];
            file_name = strcat(func2str(problem),'_',replace_dot,'_regression_',string(maxIterationsNum),'_',mlmodel,'_',string(run),'.csv');

%             source_path = 'C:\Python-Workspace\thesis\FSE2023\Results\';
            source_path = '../../Results';

            data_path = strcat(source_path,func2str(problem),'\');
            data_file_location = strcat(data_path,file_name);
            T = readmatrix(data_file_location);
            row_indices = find(T(:,3)==1); 
            predictedData = T(T(:,3) == 1,:);
            disp(length(X));
            inputData = predictedData(:,6:length(X)+5); 
            heightInputData = height(inputData);
            disp(heightInputData);

            verify_path = strcat(source_path,func2str(problem),'\verify\');
            path = strcat(verify_path,func2str(problem),'_',replace_dot,'_verify_',string(maxIterationsNum),'_',mlmodel,'_',string(run),'.csv');    
            verifyData = readmatrix(path);
            counter = height(verifyData);

              numChange = 0;
              numSame = 0;
              actualPopObj=-1;
              for i = counter : heightInputData
                 disp('number of iterations completed:');
                 eachInput = inputData(i,:);
                 disp(eachInput);
                 predictedFitness = predictedData(i,1); 
                 predictedLabel = predictedData(i,2); 

%                  Simulating the predicted instances
                 actualPopObj=callSimulator(eachInput,req,func2str(problem));
                 actualLabel = labelBNB(actualPopObj,high,replace_dot);
                
                  flag = 0;
                 if predictedLabel == actualLabel
                     numSame = numSame + 1;
                     flag = 1;
                 else
                     flag = 0;
                     numChange = numChange + 1;
                 end 

                 disp('Num of prediction labels changed after sim');
                 disp(numChange);
                 disp('Num of prediction labels same after sim')
                 disp(numSame);
                
                inputArray(1) = predictedFitness;
                inputArray(2) = predictedLabel;
                inputArray(3) = actualPopObj;
                inputArray(4) = actualLabel;
                inputArray(5) = flag;
                inputArray(6) = row_indices(i); 
                inputArray(7:length(X)+6) = eachInput;
            
            writematrix(inputArray,path,'WriteMode', 'append');
              end
            PopObj = -100;