%model an LGN neuron receiving a target bar of light preceded by two
%masking bars of light that, if appropriately positioned in space and
%appropriately timed, will make the target bar appear less visible!
clear all;
close all;

%FILTER PARAMETERS
%spatial filter parameters
sigma_c = 1.4; %width of center portion of spatial r.f. [degrees]
sigma_s = 2.1; %determines width of surround portion of spatial r.f. [deg]
A_c = 1; %strength of center portion of spatial r.f.
A_s = 0.9; %strength of surround portion of spatial r.f.
dx = 0.05; %resolution of spatial grid
% Should this reflect the (average) neuronal density?

%Defining and plotting the receptive field
x_min = -4*sigma_s; %x-value roughly giving -x border of r.f.
x_max = 4*sigma_s; %x-value roughly giving +x border of r.f.
x_vect = x_min:dx:x_max; %values of x over which to compute D_x

%defining spatial kernel D_x, which determines the weight given to each
%spatial location. 
D_x_center = A_c * exp(-(x_vect.^2)/(2*(sigma_c^2))) / sqrt(2*pi*sigma_c^2);
D_x_surround = A_s * exp(-(x_vect.^2)/(2*(sigma_s^2))) / sqrt(2*pi*sigma_s^2);
D_x_vect = D_x_center - D_x_surround;

%Plotting the center distribution (excitatory), the surrounding
%distribution (inhibatory), and the finally the cell's spatial
%receptive field.
figure(1)
subplot(3,1,1)
% plot(x_vect, D_x_center, 'r--') %plot center with red dashed lines
hold on
% plot(x_vect, D_x_surround, 'k--') %plot surround with black dashed lines
plot(x_vect, D_x_vect) %plot full receptive field with solid blue line
xlabel('x (deg)') %ask Goldman what this means
ylabel('D_x')

%temporal filter parameters
alpha = 1/10; %determines length of temporal filter [ms^-1]
dt = 1; %ms
tau_vect_max = 20/alpha; %tau value giving border of temporal r.f.
%means that the signal will decay to zero by this time
tau_vect = 0:dt:tau_vect_max; %value of tau over which to compute D_t

%define and then plot temporal kernel D_t
D_t_vect = alpha*exp(-alpha*tau_vect).*((alpha*tau_vect).^5/(5*4*3*2) ...
    - (alpha*tau_vect).^7/(7*6*5*4*3*2));
subplot(3,1,2)
plot(tau_vect,D_t_vect)
xlabel('tau (ms)')
ylabel('D_t')
set(gca,'XDir','reverse')

%define and then plot the full spatio-temporal kernel D(x,tau)
D_xt_mat = D_x_vect'*D_t_vect; 
%full 2-D r.f., with x as 1st dimension & t as 2nd dimension

%creating a contour plot of the data
subplot(3,1,3)
contour(tau_vect,x_vect,D_xt_mat,12); %makes a contour plot of the data
colorbar
set(gca,'Xdir','reverse')
xlabel('tau (ms)')
ylabel('x (deg)')


%STIMULUS PARAMETERS
%spatial parameters & plot of spatial locations of stimuli
Target_LeftEnd = -0.5; %position of start of bar [deg]
Target_RightEnd = 0.5; %position of end of bar [deg]
Target_x_vect = [zeros(1,(Target_LeftEnd - x_min)/dx)... %nothing flashed here
 ones(1,((Target_RightEnd - Target_LeftEnd)/dx)+1)... %Target position
 zeros(1,(x_max - Target_RightEnd)/dx)]; %nothing flashed here
% note, this is simply a vector denoting a light beam, with 0s representing no
% light and 1s representing light. Uses the parameters in the receptive field 

%a new set of graphs/figurino
figure(2)
subplot(3,1,1)
plot(x_vect,Target_x_vect,'o')
xlabel('x (deg)')
ylabel('Target(1=Lt,0=Dk)')

%temporal parameters & plot of temporal locations of stimuli
tmax = 800; %ms
Target_on = 400; %ms
Target_off = 600; %ms
t_vect = 0:dt:tmax;
Target_t_vect = [zeros(1,Target_on/dt) ... %bar initially off from t=0 to t=Target_on-dt
 ones(1,(Target_off-Target_on)/dt) ... %then Target on
 zeros(1,((tmax-Target_off)/dt)+1)]; %then Target off again
subplot(3,1,2)
plot(t_vect,Target_t_vect)
xlabel('t (ms)')
ylabel('Target')

%define and plot stimulus in both space & time
Target_xt_mat = Target_x_vect'*Target_t_vect;
subplot(3,1,3)
contourf(t_vect,x_vect,Target_xt_mat); %makes a contour plot of the data
colorbar
xlabel('t (ms)')
ylabel('x (deg)')

%RUN MODEL
r0 = 0; %background rate (ms^-1)
% Target_L_x = sum(dx*D_x_vect.*Target_x_vect) 
%spatial linear filter integral
% assume there is zero background rate to these neurons. 
% thespatial integral Lx  simply becomes the sum of the product dx*Dx(x)*sx(x), 
% i.e. just multiply Dx(x) by sx(x) element by element, then multiply by dx
% and then sum over all the spatial points for which neither of these are zero. 
% (Technically, we could just multiply over x corresponding to the width of the 
% target bar, but to keep our code general, we’ll multiply over all
% elements of x for which the kernel Dx(x) is non-negligible). 

% the definition of Target_L_x can also be achieved by taking the dot
% product of the two vectors, or matrix multiplying them
Target_L_x = dx*D_x_vect*Target_x_vect'; %spatial linear filter integral

%prepare to do temporal filter integral
Target_L_t_vect = zeros(1,length(t_vect)); %set up vector to hold temporal filter values

Stim_t_NegTimes = zeros(1,tau_vect_max+1); %need to add stimulus values at t<0 so
 %can get what influenced cell at times t<tau_vect_max
Target_t_vect_long = [Stim_t_NegTimes Target_t_vect]; %includes Stimulus at times t<0

i = 0;
for t=0:dt:tmax
    i = i+1;
    Target_L_t_vect(i)=dt * D_t_vect * Target_t_vect_long(i+tau_vect_max:-1:i)';
end

Target_r_vect_NoThresh = 1000*(r0 + Target_L_x*Target_L_t_vect); 
%1000 converts to Hz 

Target_r_vect = max(Target_r_vect_NoThresh,0); 
%thresholding, is the maximum of the value of the first argument to the function and zero

%plot model results
figure(3)
subplot(3,1,1)
contourf(tau_vect,x_vect,D_xt_mat,12); %makes a contour plot of the data
colorbar
set(gca,'Xdir','reverse')
xlabel('tau (ms)')
ylabel('x (deg)')
subplot(3,1,2)
contourf(t_vect,x_vect,Target_xt_mat); %makes a contour plot of the data
colorbar
xlabel('t (ms)')
ylabel('x (deg)')
subplot(3,1,3)
plot(t_vect, Target_r_vect_NoThresh,':')
hold on;
plot(t_vect, Target_r_vect)
xlabel('time (ms)');
ylabel('rate (Hz)');
legend('No threshold', 'thresholded')
hold off;