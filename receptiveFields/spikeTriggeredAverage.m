clear all;
close all;
load FlyH1.mat;


% creating an averaged "snapshot" of the receptive fields energy 400ms
% preceding spike events, called STA "spike triggered average";

% variables for the for loop and a count for the number of spikes;
numIterations = length(stim);
numSpikes = 0;

% vector for the snapshot, with 200 indices because of the temporal
% resolution of 2ms and a desired 400ms snapshot;
spikeVelocityVector = zeros(1, 200); % each index will represent an instance;

% for loop, starting at 200 to represent time needed to obtain snapshot;
for iterationNum = 200:1:numIterations
    if (rho(iterationNum) == 1)
        % add to the STA the stimulus values preceding this spike;
        spikeVelocityVector = spikeVelocityVector ...
            + stim(iterationNum:-1:(iterationNum-200+1))';
        numSpikes = numSpikes + 1;
    end
end
% check overlap, add check against it to maintain that the data is
% independent for generalized field


% the vector contains the sum of all the snapshots, average them;
averageSpikeTrigger = spikeVelocityVector / numSpikes;

% % to plot the values, reverse x-axis to show time leading and then plot;
% axes('XDir','reverse')
% figure(1)
% plot(averageSpikeTrigger)



% initializing the convolution vector, given that it has a length of vec1 + vec2
lenConv = length(averageSpikeTrigger) + length(stim);
convolvedVector = zeros(1, lenConv);

% creating new temporary vectors for the STA and the stimulus data, which will be of the same
% length to allow for convolution without going past their bounds and causing an error. will be
% filled with zeroes
lenSTA = length(averageSpikeTrigger);
lenStim = length(stim);


% the outer for loop iterating through the indices of the convolution vector itself, not the
% component adjusted STA and stim vectors; this nested loop will be insanely demanding though,
% given that it is nm; will amend as needed in lab
for i = lenSTA:-1:1
    for j = 1:lenStim
        % checking back on what convolving does mathematically, the convolution vector being
        % created will hold by index the sum of every product value of the two vectors by time;
        % convolution usually does things through integration, but Riemann sums will do the
        % trick here, with each index of the final vector holding a summation of the previously
        % mentioned values
        % in terms of implementation, that means interating over every possible value and summing
        % them up if the condition that the indices don't take the arrays/vectors out of bounds.
        % MATLAB has the first vector start at 1 and doesn't have >= conditional, so instead check
        % if i - j + 1 > 0 instead of i - j ≥ 0
        convolvedVector(i + j - 1) = convolvedVector(i + j -1) + averageSpikeTrigger(i) * stim(j);
    end
end

% % plotting the values
% figure(2)
% plot(convolvedVector);


normalized_vector = convolvedVector/max(convolvedVector);
lenConv = length(convolvedVector);

% this is the first strategy I used. normalize the convolved vector by
% dividing by its max, which I don't think is a sound strategy anymore, as
% there are large values that may be aberations, and taking the max one can
% introduce error into the analysis. maybe take a weighted average? 
poissonPrediction = zeros(1, lenConv);
poisFires = 0;

% this is the number of test runs we want to evaluate the poisson
% prediction by
numIterations = 10;
for i = 1:numIterations
    for j = 1:lenConv
        if(normalized_vector(j) > rand) 
            poissonPrediction(j) = poissonPrediction(j) + 1;     
            poisFires = poisFires + 1;
        end
    end
end

averagePoissionPrediction = poissonPrediction / numIterations;
averagePoisFires = poisFires / numIterations;

mean(averagePoissionPrediction)
mean(rho)
        
% next step, create a kernel function which uses the STA to find the relative likelihood of the
% receptive field firing during an input stimulus through convolving the STA with the stimulus.
% Might also want to also potentially add the ability to update the STA with the values in the
% new stimulus if it includes when it fires, maybe comparing accuracy before and after assimilating
% the values in the functionality.

% notes:
% divide STA by euclidian norm, ro normalize the convolved vector, make more orderly
% next: take the vector of spikes from data, turn into rates based on
% convolution
% or take the stimulus vector, and turn into predicted spikes
