## Method 1

1、Download the code from github: https://github.com/BETAFPV/LiteSilverware.git, and checkout to f042_1s_bl, open the project with KEIL MDK, then compile it, there should be no errors and no warnings

![image-20200421091953903](C:\Users\wenzh\AppData\Roaming\Typora\typora-user-images\image-20200421091953903.png)



2、Connect the board to computer with a ST-LINK v2, **SWDIO to DATA, SWDCLK to CLK, and GND to GND**, then power on the board and click the download button on keil to flash the firmwware. 

![image-20200421085826997](C:\Users\wenzh\AppData\Roaming\Typora\typora-user-images\image-20200421085826997.png)

3、Reboot the board and begin to calibrate the acc  with the following steps(show as the code above):

a. place the quadrotors on a horizontal surface

b. push the pitch rocker to the bottom and hold on 

c. then switchs aux 6 

d. wait for a moment



## Method 2

1、Use a hex file to flash the board directly with STM32 ST-LINK Utility