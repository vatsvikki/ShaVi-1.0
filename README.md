# ShaVi-1.0

## Description

Full waveform inversion (FWI) is a technique used in studying underground struc
tures through seismic data. It helps create a detailed image of what’s beneath the
Earth’s surface. However, implementing FWI can be tough due to its complex math
and high computational demands. To make it easier, we made ShaVi-1.0, a 2D acous-
tic FWI tool. It uses OpenMP API to handle computations and has a user-friendly
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
- [Credits](#credits)
- [License](#license)

## Installation
#### Prerequisites
1. [GNU Compiler Collection (GCC)](https://www.guru99.com/c-gcc-install.html)
2. [Python 3.0.0](https://www.python.org/downloads/)
3. [pip](https://pip.pypa.io/en/stable/installation/)

Access the command prompt or terminal and proceed with the following steps.
#### Clone the repository
```
git clone https://github.com/vatsvikki/ShaVi-1.0.git
```
#### Module installation
```
cd path_to_repository/packages
pip install -r dependencies.txt
```
## Usage

Navigate to the `src` folder and compile the `main.c`
```
gcc main.c -lm -fopenmp -o ../bin/fwi
```
An executable file named `fwi` will be generated in the `bin` folder. 

Execute the `ShaVi-1.0.py` file to launch the interface by following the command below:
```
python ShaVi-1.0.py
```
This Command will launch the interface of Shavi-1.0 as shown in figure.
    
   [ ![figure_1](https://github.com/vatsvikki/ShaVi-1.0/assets/50227308/91cabea1-3046-493a-868a-a06413278b7c)](https://github.com/vatsvikki/ShaVi-1.0/issues/1#issue-2057942027)
    

## Features

package consists of
four primary windows and buttons to interact with the package, as marked in the figure of interface, each offering distinct utilities to enhance the user’s implementation experience.
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

Go the extra mile and write tests for your application. Then provide examples on how to run them here.

## Credits

List your collaborators, if any, with links to their GitHub profiles.

If you used any third-party assets that require attribution, list the creators with links to their primary web presence in this section.

If you followed tutorials, include links to those here as well.

## How to Contribute

If you created an application or package and would like other developers to contribute it, you can include guidelines for how to do so. The [Contributor Covenant](https://www.contributor-covenant.org/) is an industry standard, but you can always write your own if you'd prefer.
