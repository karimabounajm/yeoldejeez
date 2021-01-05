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

% the vector contains the sum of all the snapshots, average them;
averageSpikeTrigger = spikeVelocityVector / numSpikes;

% to plot the values, reverse x-axis to show time leading and then plot;
% axes('XDir','reverse')

% plotting in a figure, allowing for more diagrams to be plotted next to it
figure(1)
subplot(3, 1, 1)

hold on 
plot(averageSpikeTrigger)

xlabel('Time Away from Spike')
ylabel('Stimulus Potential')


% convolving the STA just found with the stimulus (without using conv)
% result would be an estimate of the membrane potential of the cell at each time-point during the 
% presentation of the provided stimulus. Since a rising membrane potential puts a cell closer to 
% its spiking threshold, this value can be somewhat related to firing rate because higher membrane 
% potential values are related to higher firing rates.

% initializing the convolution vector, given that it has a length of vec1 + vec2
lenConv = length(averageSpikeTrigger) + length(stim);
convolvedVector = zeros(1, lenConv);

% creating new temporary vectors for the STA and the stimulus data, which will be of the same 
% length to allow for convolution without going past their bounds and causing an error. will be 
% filled with zeroes
lenSTA = length(averageSpikeTrigger);
lenStim = length(stim);

% the outer for loop iterating through the indices of the convolution vector itself, not the 
% component adjusted STA and stim vectors; one more thing, zeros nearly gave me a migrana, it 
% forms vectors instead of array and is a pain. this nested loop will be insanely long though, 
% given that it is n^2 with size of 600200; will amend as needed in lab 
for i = 1:lenSTA
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

subplot(3, 1, 2)

hold on 
plot(convolvedVector);

xlabel('Time with Resolution of 2ms')
ylabel('Relative Likelyhood to Fire')
