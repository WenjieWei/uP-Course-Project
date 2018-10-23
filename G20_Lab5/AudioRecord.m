% % Record your voice for 5 seconds.
% recObj = audiorecorder;
% disp('Start speaking.')
% recordblocking(recObj, 5);
% disp('End of Recording.');
% 
% % Play back the recording.
% play(recObj);
% 
% % Store data in double-precision array.
% myRecording = getaudiodata(recObj);
% figure; plot(myRecording); % Plot the original waveform.
% 
% % cut the area you want and convert it into integers
% audioData = uint16((myRecording(4000:8999)+1)*1024/2); 
% csvwrite('AudioArray.csv',audioData');
% % Plot the modified waveform.
% figure;plot(audioData);

instrreset

s = serial('COM3', 'BaudRate', 11520);
fopen(s);

for i = 1:5000
    fwrite(s, audioData(i), 'uint16');
end
fclose(s);