function high = getBoundaryRange(simModel, req)
    switch simModel
        case 'reg'
            if strcmp(req,'R7')
                low = -0.026; %-0.00257
                high = 0.1;%1.337 %1.288514
            end
        case 'autopilot'
            if strcmp(req,'R1_4_1')
                low = -23.019;
                high = 21.45;
            end
            if strcmp(req,'R1_6')
                low = -5.66;
                high = 4.46;
            end
            if strcmp(req,'R12_1')
                low = 0;
                high = 0;
            end
        case 'tustin'
            if strcmp(req,'R1a')
                % R1a
                low = -5.1682;
                high = 5.76204;
            end
            if strcmp(req,'R1b')
                % R1b
                low = -0.004;
                high = 2.3548;
            end
            if strcmp(req,'R1c')
                %R1c
                low = -0.0305;
                high = 1.54631;
            end
            if strcmp(req,'R1d')
                %R1d
                low = -0.04587;
                high = 2.322075;
            end
            if strcmp(req,'R1e')
                %R1e
                low = -0.0345;
                high = 1.74958;
            end
            if strcmp(req,'R2a')
                %R2a
                low = -0.92;
                high = 1.69;
            end
            if strcmp(req,'R2b')
                %R2b
                low = -0.81;
                high = 1.29;
            end
        case 'tustinr4'
            if strcmp(req,'R4a')
                low = -1.6255;
                high = 1.79086;
            end
            if strcmp(req,'R4b')
                low = 0.049249;
                high = 0.050116;
            end
        case 'nlguidance'
            if strcmp(req,'R6')
                low = 0;
                high = 0;
            end
        case 'fsm'
            if strcmp(req,'R1_1')
                high = 0.01;
            end
    end
end