% Function to generate LUT from signal
function lut = generate_lut(signal, RES)
    % Scale to DAC resolution
    signal = signal - min(signal);
    signal = signal / max(signal) * (2^RES - 1);
    lut = round(signal);
    
    % Ensure bounds
    lut(lut > 2^RES-1) = 2^RES-1;
    lut(lut < 0) = 0;
end