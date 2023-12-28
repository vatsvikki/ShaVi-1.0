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
    
   [ ![figure_1](https://github.com/vatsvikki/ShaVi-1.0/assets/50227308/91cabea1-3046-493a-868a-a06413278b7c)](https://github.com/vatsvikki/ShaVi-1.0/issues/1#issue-2057942027)
    

## Features

If your project has a lot of features, list them here.

## Tests

Go the extra mile and write tests for your application. Then provide examples on how to run them here.

## Credits

List your collaborators, if any, with links to their GitHub profiles.

If you used any third-party assets that require attribution, list the creators with links to their primary web presence in this section.

If you followed tutorials, include links to those here as well.

## How to Contribute

If you created an application or package and would like other developers to contribute it, you can include guidelines for how to do so. The [Contributor Covenant](https://www.contributor-covenant.org/) is an industry standard, but you can always write your own if you'd prefer.
