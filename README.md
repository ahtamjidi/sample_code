# sample_code

This is a small unfinished project. I was trying to use boost ode library to simulate a car that accepts different steering commands contaminated with noise. The car model, integration and noise generation works fine. I intend to add an observer and implement kalman filter as well. The code has a plotting function which produces the the plot for the car trajectory. It then saves the result into a png file. Look intto the `doc` folder for formula derivation.

To compile the code run
```
g++ -std=c++11 -O2 -Wall -pedantic  estimate.cpp -o estimate -I/usr/include/python2.7 -lpython2.7
```