function noisy_signal = add_noise(clean_signal, peak, fs, t)
    % Simple baseline wander
    baseline_freq = 0.3;  % Breathing rate
    baseline_amplitude = 0.3 * peak;
    baseline_wander = baseline_amplitude * sin(2*pi*baseline_freq*t);

    % Simple muscle artifacts
    muscle_amplitude = 0.1 * peak;
    muscle_noise = muscle_amplitude * randn(size(t));

    % Contact noise (simulates electrode-skin interface noise)
    contact_noise = zeros(size(t));
    burst_locations = rand(size(t)) > 0.99;  % Random bursts
    contact_noise(burst_locations) = 0.05 * peak * randn(sum(burst_locations), 1);
    [b_contact, a_contact] = butter(1, 50/(fs/2), 'low');  % Reduced order to 1
    contact_noise = filter(b_contact, a_contact, contact_noise);
    
    % Combine noise components
    noisy_signal = clean_signal + baseline_wander + muscle_noise + contact_noise;


end