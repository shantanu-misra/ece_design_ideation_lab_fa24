# ece_design_ideation_lab_fa24
EKG jacket project with DSP on STM32

simulated ECG signals with true-to-life noise
![ecg_signals_hires](https://github.com/user-attachments/assets/006fec63-91b3-4e40-b1ed-74e95473230a)

compared bode plots of different filters to choose best one
![filter_comparison](https://github.com/user-attachments/assets/7cd47d7f-122e-4166-a16f-0bf36cb2e29a)

decided on a bandpass butterworth filter
![bandpass_filter](https://github.com/user-attachments/assets/5323c11a-ec6d-40ba-ab26-beca2d69a261)

wrote that code on stm32 to filter the simulated ecg signals
![noisy and filtered ecg](https://github.com/user-attachments/assets/3bf388b7-6ae0-4f4d-8379-8437ae48be78)



References:
1. karthik raviprakash (2024). ECG simulation using MATLAB (https://www.mathworks.com/matlabcentral/fileexchange/10858-ecg-simulation-using-matlab), MATLAB Central File Exchange. Retrieved December 13, 2024.
2. GE HealthCare, "A Guide to ECG Signal Filtering," Aug. 2023. [Online].
3. S. Sim, Z. Lendek, and P. Dobra, "Implementation and Testing of Digital Filters on STM32 Nucleo-64P," in Proc. 2022 IEEE Conference, 2022, pp. 1-6.
