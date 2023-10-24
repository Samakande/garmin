# garmin
The solution has two parts
MATLAB
Designed a low pass filter in matlab to fileter the noise of the uart signal. The low pass filter has a bandpass of 57600Hz. Filtering was done using matlab script function filter().  
The filtered data is stored in excel file.

C/Embedded application
The decoding of the filtered data was done in C to run on an STM32nucleo-l152re board. The decoded data is sent via uart and with the help of a serial terminal (Putty) it was printed on the screen

Assumptions
No parit bit
2 stop bits
The number of buffered samples per bit duration alternated between 8 and 9 to account for rounding errors of (Fs/Baudrate). This was enabled by using a circular shift mechanism.
The voting mechanism to decide either a low or high bit used only the average of the 4 middle bits. The code can be further optimised by using 2 or 1 middle bit provided the filtered signal is smooth enough
I also assumed a high should be greater than 1.6V otherwise its a low. 
IDE
Used STMCubeide version 1.13.2
