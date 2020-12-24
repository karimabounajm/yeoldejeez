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
axes('XDir','reverse')
hold on 
plot(averageSpikeTrigger)

potato = conv(averageSpikeTrigger, stim);
% 
% % zero padding
% for iterationNum = 200:1:numIterations
%     if (rho(iterationNum) == 1)
%         % add to the STA the stimulus values preceding this spike;
%         spikeVelocityVector = conv(averageSpikeTrigger, stim(iterationNum:-1:(iterationNum-200+1))'); 
%         numSpikes = numSpikes + 1;
%     end
% end
% I think I need to find every instance of firing, convolve every instance
% before a spike with the STA, then average that out?