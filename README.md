# ShaVi-1.0
There are two way to execute this inversion code to reprodue the model parameters:
First: To access the GUI of this code execute the Shavi-1.0.py script which is placed in bin file. 
There is a turorial link for its execution in help section of the GUI which can make its implimentation
easy and a "GUI_tutorial.pdf" is also provided in the tutorial folder.

Second: Another option is to directly access the code by using the executable (fwi.exe) placed in bin file. 
Command to run the excecutable for test model is:
./fwi.exe ../input/parameter.txt ../input/source_amp.txt ../input/initial_model_block.txt ../input/observe.txt ../acquisition/source.txt ../acquisition/receiver.txt


It will require the necessary file (parameter, source information, acquisition geometry,
and observe data)
Test files are already prepared which are placed as follows:

input folder contains the information of 
   Data				  File Name
Source amplitude ---------------- "/input/source_amp.txt", 
Initial model ------------------- "/input/initial_model_block.txt"
Parameter ------------------------"/input/parameter.txt"
Observe data ---------------------"/input/observe.txt"
Source location ------------------"/acquisition/source.txt"
Receiver location ----------------"/acquisition/receiver.txt"


The ouput is saved in the output folder provided in this package.

A folder is also provided with the name synthetic data generator which can help to produce the synthetic data for synthtic models.
