% Function to filter ECG signal
function filtered_signal = filter_ecg(noisy_signal, fs)
    f1 = 0.5;
    f2 = 800;
    Wn = [f1 f2]/(fs/2);
    [b, a] = butter(4, Wn, 'bandpass');
    filtered_signal = filter(b, a, noisy_signal);
end