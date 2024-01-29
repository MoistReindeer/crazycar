% CREATE A FINE POLYNOME
clc; clear all; hold off; close all;

table_size = 212;
lut_factor = 1700 / table_size;
sensorOutput = [2.75 2.55 2 1.55 1.25 1.05 0.9 0.8 0.725 0.65 0.6 0.55 0.5 0.495 0.49];        % Just an initial characteristic curve of the infrared sensor
ADCchange = 4096 / max(2.75);
%adcOutput_front = [4095 3833 3378 2940 2521 2197 1918 1735 1562 1402 1315 1207 1117,...
%    1062 979 922 867 812 786 720 693 635 579 541 495 463]./lut_factor;                         % MEASURED ADC Values of your measured distances

% adcOutput_front = [400 500 600 800 1000 1200 1400 1500 1600]
% adcOutput_front = [400 540.51 679.22 817.93 997.19 1184.19 1362.11 1483.75 1600 1700 1800 1900];
adcOutput_front = [ 400 519 674.95 817.93 1000 1192 1356 1496 1600];
% relatedDistance_mm = [0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 110 120 130 140 150] * 10; % HERE you could store your measured distance values
% relatedDistance_mm = [30 31.5 35 45 55 65 75 77 80];
% relatedDistance_mm = [30 32.86 38.47 46.04 56.7 68.49 74.66 76.9 78 80 80 80];
relatedDistance_mm = [30 32.58 37.63 46.61 60 70.45 74.94 76.9 80];

[p_front,s_front] = polyfit(adcOutput_front, relatedDistance_mm, 8);          % Fits a polynom of order-n to your input values
x_front = linspace(0,floor(max(adcOutput_front)),table_size);                       % Just create ADC Values in order to plot the results of the polynome

f_front = floor(polyval(p_front,x_front));                                          % Calculates your results, taking into account the measured values and the already stored polynome

% f_front(f_front>512) = 512;                                                       % Just uses ADC lower or equal to 4096
%f_front(x_front<floor(min(adcOutput_front))) = max(relatedDistance_mm);     % THERE ARE NO NEGATIVE ADC results
f_front(x_front<min(adcOutput_front)) = min(relatedDistance_mm);
f_front(f_front<min(relatedDistance_mm)) = min(relatedDistance_mm);
f_front(f_front>max(relatedDistance_mm)) = max(relatedDistance_mm);
f_front(x_front>1700) = max(relatedDistance_mm);
%f_front(f_front>max(relatedDistance_mm)) = min(relatedDistance_mm);

%f_front(f_front<min(relatedDistance_mm)) = max(relatedDistance_mm);

figure(1)
subplot(1,2,1)
hold on;
plot(adcOutput_front,relatedDistance_mm);
%plot(x_front,f_front,'o')

title('Front Sensor output value');
xlabel('Scaled ADC Values (samples)');
ylabel('Distance (mm)');
grid on; grid minor;

subplot(1,2,2)
hold on;
plot(adcOutput_front,relatedDistance_mm);
plot(x_front,f_front,'o')

legend('Measured Sensor Values','Approximated Sensor Values');

title('Front Sensor output value');
xlabel('Scaled ADC Values (samples)');
ylabel('Distance (mm)');
grid on; grid minor;
X_front = table(x_front',f_front');

fprintf('unsigned short ir_front[%i] = {', table_size);
for idx = 1:table_size
    fprintf(num2str(f_front(idx)));
    if idx < table_size
        fprintf(',');
    end
end
fprintf('};\n');

