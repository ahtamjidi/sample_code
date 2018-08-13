# sample_code

1. odeint_car
This is a small unfinished project. I was trying to use boost ode library to simulate a car that accepts different steering commands contaminated with noise. The car model, integration and noise generation works fine. I intend to add an observer and implement kalman filter as well. The code has a plotting function which produces the the plot for the car trajectory. It then saves the result into a png file. Look intto the `doc` folder for formula derivation.

To compile the code run
```
g++ -std=c++11 -O2 -Wall -pedantic  estimate.cpp -o estimate -I/usr/include/python2.7 -lpython2.7
```

2. python_sample
This folder contains a sample of my python code. This is a script that I use to visualize the results of localization at work.
`localization_viz.py` contains the code
`sample_output.ipynb` is the notebook that uses the same code to analyze localization

3. PCL sample project
This is a copy of the PCL tutorial on NDT taken from [here](http://pointclouds.org/documentation/tutorials/normal_distributions_transform.php#normal-distributions-transform)

I just made a very small change to its CMakeLists to put the executable in the right place. The screenshot from the output is saved in a file named
`Screenshot from 2018-08-13 01:09:34.png`
I wanted to demonstrate that I can install, compile and run PCL code. 
To compile the code 
```
cd build
cmake ..
make
cd ..
./ndt_tutorial
```
