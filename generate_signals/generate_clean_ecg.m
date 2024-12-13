function ecg_signal = generate_clean_ecg(BPM, peak, Ns)
    % Generate time vector
    x = linspace(0.01, 2, Ns);
    
    % Calculate interval
    li = 30/BPM;
    
    % Wave parameters - scaled by peak
    a_pwav = 0.25 * peak;
    d_pwav = 0.09;
    t_pwav = 0.16;
    
    a_qwav = 0.025 * peak;
    d_qwav = 0.066;
    t_qwav = 0.166;
    
    a_qrswav = 1.6 * peak;
    d_qrswav = 0.11;
    
    a_swav = 0.25 * peak;
    d_swav = 0.066;
    t_swav = 0.09;
    
    a_twav = 0.35 * peak;
    d_twav = 0.142;
    t_twav = 0.2;
    
    a_uwav = 0.035 * peak;
    d_uwav = 0.0476;
    t_uwav = 0.433;
    
    % Generate waves
    pwav = p_wav(x, a_pwav, d_pwav, t_pwav, li);
    qwav = q_wav(x, a_qwav, d_qwav, t_qwav, li);
    qrswav = qrs_wav(x, a_qrswav, d_qrswav, li);
    swav = s_wav(x, a_swav, d_swav, t_swav, li);
    twav = t_wav(x, a_twav, d_twav, t_twav, li);
    uwav = u_wav(x, a_uwav, d_uwav, t_uwav, li);
    
    % Combine waves
    ecg_signal = pwav + qrswav + twav + swav + qwav + uwav;
end