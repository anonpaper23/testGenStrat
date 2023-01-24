function reliable = reliability_check(predictedFitness, delta,high, req)

        isreliable = false;

        disp('predicted fitness:');
        disp(predictedFitness);
        disp('delta');
        disp(delta);
%         disp('low');
%         disp(low);
        disp('high');
        disp(high);

        %Only for certain requirements. 
        if (strcmp(req,'R1b')) || (strcmp(req,'R1e')) || (strcmp(req,'R2a')) || (strcmp(req,'R2b')) || (strcmp(req,'R7'))
            if (predictedFitness + delta >= high) && (predictedFitness - delta >= high) && (predictedFitness >= high)
                isreliable = true;
            end
            if (predictedFitness + delta < high) && (predictedFitness - delta < high) && (predictedFitness < high)
                isreliable = true;
            end
        else
            if (predictedFitness + delta >= 0) && (predictedFitness - delta >= 0) && (predictedFitness >= 0)
                isreliable = true;
            end
            if (predictedFitness + delta < 0) && (predictedFitness - delta < 0) && (predictedFitness < 0)
                isreliable = true;
            end

        end

        if (isreliable)
            reliable = true;
            disp('prediction is reliable')
        else
            disp('prediction not reliable');
            reliable = false;
        end
end