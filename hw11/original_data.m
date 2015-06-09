function original_data(plot_title)

load accels.txt

figure()
plot(accels)
ylabel('Raw Z Accel Data')
xlabel('Data Points')
title(plot_title)

f=fft(accels);
f(1)=[];
n_f=length(f);
power=abs(f(1:floor(n_f/2))).^2;
nyquist=1/2;
freq=(1:n_f/2)/(n_f/2)*nyquist;
figure()
plot(freq,power)
ylabel('Power')
xlabel('Frequency')
title('FFT Original Data')

end