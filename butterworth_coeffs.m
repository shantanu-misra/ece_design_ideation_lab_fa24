fs = 4000;  % Sampling frequency
% Lower cutoff should be lower to remove only baseline wander
f1 = 0.5;   % 0.5 Hz to remove baseline wander but keep P and T waves
f2 = 800;   % Upper cutoff to remove high-freq noise

% Normalize frequencies
Wn = [f1 f2]/(fs/2);
% Keep 2nd order for stability
[b, a] = butter(2, Wn, 'bandpass');

% Print coefficients
fprintf('float32_t b[] = {%.6ff, %.6ff, %.6ff, %.6ff, %.6ff};\n', b);
fprintf('float32_t a[] = {%.6ff, %.6ff, %.6ff, %.6ff, %.6ff};\n', a);

% Print coefficients
fprintf('// Coefficients for %d-%d Hz bandpass at %d Hz sampling\n', f1, f2, fs);
fprintf('float32_t b[] = {');
fprintf('%.6ff, ', b(1:end-1));
fprintf('%.6ff};\n', b(end));

fprintf('float32_t a[] = {');
fprintf('%.6ff, ', a(1:end-1));
fprintf('%.6ff};\n', a(end));

% Plot frequency response
freqz(b, a, 1024, fs);
grid on;
title(sprintf('Butterworth Bandpass Filter (%d-%d Hz)', f1, f2));

% save with higher resolution
print('figures/bandpass_filter', '-dpng', '-r300')  % 300 DPI resolution