clear;
clc;
close all;

pi = 3.14159;
sampleTime = 2;
frequency = 440;
sineWave = zeros(16000 * sampleTime, 1);

for i = 1 : 32000
    rad=2*pi*frequency*i/16000;
    sineWave(i) = (sin(rad) + 1) * 2048;
end
% 
% for i = 1 : 32000
%     rad=2*pi*frequency*i;
%     sineWave(i) = sin(rad);
% end

plot(sineWave);