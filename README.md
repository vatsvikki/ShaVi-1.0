# ShaVi-1.0

## Description

Full waveform inversion (FWI) is a technique used in studying underground structures through seismic data. 
It helps create a detailed image of what’s beneath the
Earth’s surface. However, implementing FWI can be tough due to its complex math
and high computational demands. To make it easier, we made ShaVi-1.0, a 2D acoustic FWI tool. It uses OpenMP API to handle computations and has a user-friendly
interface for easy use on regular computers.


ShaVi-1.0’s core code is in C language, designed so users can adjust parts as needed.
The Graphical User Interface (GUI), made in Python, comes with handy features like
real-time plotting and updates on the process status. It also has message and warning
boxes for a smooth experience. The GUI includes a toolbox for generating acquisition
geometry and plotting source-receiver locations. This GUI not only makes using the
tool easier but also lets you analyze results in real-time.

## Table of Contents 

- [Installation](#installation)
- [Usage](#usage)
- [Tests](#tests)
- [Contribution](#contribution)
- [Support](#support)

## Installation
#### Prerequisites
1. [GNU Compiler Collection (GCC)](https://www.guru99.com/c-gcc-install.html)
2. [Python 3.0.0](https://www.python.org/downloads/)
3. [pip](https://pip.pypa.io/en/stable/installation/)


### Access the command prompt or terminal and proceed with the following steps.
## Setup

#### Step: 1
##### Clone the repository
```
git clone https://github.com/vatsvikki/ShaVi-1.0.git
```
#### Step: 2
##### Module installation
```
cd <path_to_repository>/packages
pip install -r dependencies.txt
```
## Usage

#### Step: 3
Navigate to the `src` folder and compile the `main.c`

```
gcc main.c -lm -fopenmp -o ../bin/fwi
```
An executable file named `fwi` will be generated in the `bin` folder. 

#### Step: 4

Execute the `ShaVi-1.0.py` file to launch the interface by following the command below:

For Windows OS:
```
python ShaVi_win-1.0.py
```
For Linux OS:
```
python ShaVi_linux-1.0.py
```
This Command will launch the interface of Shavi-1.0 as shown in figure.
    
   [ ![figure_1](https://github.com/vatsvikki/ShaVi-1.0/assets/50227308/91cabea1-3046-493a-868a-a06413278b7c)](https://github.com/vatsvikki/ShaVi-1.0/issues/1#issue-2057942027)
    

## Features

The package consists of four primary windows and buttons to interact with the package, as marked in the figure of interface, each offering distinct utilities to enhance the user’s implementation experience.
These windows are outlined below:

#### Data Input: Window 1

To commence the inversion process, crucial data (seismic information, initial model, and source signature) in ".txt" format is supplied by navigating through the designated path.

#### Visualization of Initial and Inverted Models: Window 2

This interface provides real-time visualization of both initial and inverted models, enabling users to track inversion progress.

#### Live Progress Updates and Misfit Calculation: Window 3

 It incorporates a progress bar indicating the process's completion percentage and provides information on receivers, sources, and the objective function, offering valuable insights into the inversion process's convergence rate.

 #### 1D Velocity-Depth Profile Visualization: Window 4

 In this package, Window 4 functions as a tool to plot the 1D velocity-depth profile for both initial and inverted models, enabling a comparative analysis. Input the horizontal distance on the model's axis in Window 4 to select the profile location. Refresh the "inverted model" button in Window 2 to update and visualize the chosen profile.

 #### Menu Bar
 
This package includes a user-friendly menu bar, augmenting its functionality. Users can seamlessly generate the parameter file using the dedicated "Parameters" and "Acquisition Geometry" tools.


#### Buttons Overview:

Initial Model: Generates a visual representation of the starting point for inversion.

Inverted Model: Plots the evolving subsurface structure during the inversion process.

Progress (Hourglass): Updates real-time information on the inversion process status.

Run: Initiates computational procedures for the inversion process.

Save: Manually saves the inverted velocity model; auto-saved for convenience.

Restart: Resets the inversion process for adjustments or a fresh start.

Quit: Closes the package interface, concluding the session.

For user support, a dedicated help section in the menu bar offers a tutorial video for step-by-step guidance, enhancing the user experience and promoting effective utilization of the software's capabilities.

## Tests
Synthetic geological models — Three Blocks, Marmosui, and Overthrust — are employed in test cases with different dimensions and specified source and receiver configurations. 

#### Case 1: Three Block Model

![block_model](https://github.com/vatsvikki/ShaVi-1.0/assets/50227308/5fa4cefb-4165-4632-ab85-fc60ec901327)

The Three-Block model serves as a straightforward test case for evaluating this
package. The model’s dimensions are 500 x 1000 meters, as illustrated in figure,
partitioned into small square grids with dimensions of 10 meters each. This model
features three low-velocity blocks, each with a velocity of 1800 m/s, situated at different
depths, having a uniform background velocity of 2000 m/s. The recovery process for
this true model, commences with an initial model, depicted in
figure.

Note: The peak frequency of the observed data and source signature is 10 Hz.


The necessary data for the inversion process is located in the folder labeled
"test/case_1_block_model". Details of every file are given in the following table.

|       Data       |      File name     |
|------------------|--------------------|
|   Observe Data   |    observe.txt     |
|   Initial Model  | initial_model_block.txt|
| Source Signature | source_amp.txt     |
| Source Locations | source.txt         |
|Receiver Locations| receiver.txt       |
|   Parameters     |    parameter.txt   |

#### Case 2: Marmousi Model

![marmousi_model](https://github.com/vatsvikki/ShaVi-1.0/assets/50227308/02e7d247-a4f1-4836-8f82-74323428ab43)

Refer to the manual for detailed information on data specifications.

#### Case 3: Overthrust Model

![overthrust_model](https://github.com/vatsvikki/ShaVi-1.0/assets/50227308/fcfde493-a8bf-48d4-99b6-be3f655bc03f)

Refer to the manual for detailed information on data specifications.

## Contribute

We're excited to invite you to contribute the ShaVi-1.0.

##### Steps to contribute

1. Fork the Repository

2. Clone Your Fork

3. Create a Branch

4. Make Changes

5. Commit Changes

6. Push Changes

7. Submit a Pull Request: Open a Pull Request on the original repository. Provide a clear description of your changes, and we'll review it.

##### What Can You Contribute?

1. Code improvements or bug fixes
2. New features or enhancements
3. Documentation updates
4. Bug reports or issue discussions

## Support

For assistance regarding the package, our team is available to address your inquiries and technical needs.

You can contact us at vvats@iitk.ac.in or vikasvats17293@gmail.com
