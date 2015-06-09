function fir_data(plot_title)

load FIR.txt

figure()
plot(FIR)
ylabel('FIR Filtered Z Accel Data')
xlabel('Data Points')
title(plot_title)

f=fft(FIR);
f(1)=[];
n_f=length(f);
power=abs(f(1:floor(n_f/2))).^2;
nyquist=1/2;
freq=(1:n_f/2)/(n_f/2)*nyquist;
figure()
plot(freq,power)
ylabel('Power')
xlabel('Frequency')
title('FFT FIR Data')

end