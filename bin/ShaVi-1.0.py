# Python program to create
# a file explorer in Tkinter
  
# import all components
# from the tkinter library
from tkinter import *
import tkinter.font as font
from tkinter import filedialog
import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter.ttk as ttk
from tktooltip import ToolTip
import numpy as np
import matplotlib.pyplot as plt
import sys
import os
from PIL import Image
import threading
import time
from tkinter.ttk import Progressbar
from tkinter.filedialog import asksaveasfile
from tkinter import messagebox
import xlsxwriter
import pandas as pd
import webbrowser
import shutil

font_size = ("Aerial", 13, "bold")

open('progress.txt', 'w')
open('status.txt', 'w')
open('../output/inverted/inverted_velocity.txt', 'w')
open('acq_temp.xlsx', 'w')

def save_file():
    save_inv = filedialog.asksaveasfile(initialfile = 'Untitled.txt',
defaultextension=".txt",filetypes=[("All Files","*.*"),("Text Documents","*.txt")])
    shutil.move("../output/inverted/inverted_velocity.txt", os.path.abspath(save_inv.name))

def restart():
    root.destroy()
    os.startfile("GUI_11.py")


def quit_win():
    root.quit()

def donothing():
   helpwin = Toplevel(root)
   button = Button(helpwin, text="Do nothing button", command = help)
   button.grid(row = 0, column = 0, sticky = 'nsew')
   

def clearToTextInput():
        T.delete("1.0","end")
def display():

    if os.stat('progress.txt').st_size == 0:
        messagebox.showinfo("Please Wait...", "Current progress status is updating. ")
    else:
        with open('status.txt', 'r') as file:
            data = file.read()   
        with open('progress.txt', 'r') as itr_file:
            itr = itr_file.read()

        T.insert(END, data)
        file.close()
        T.after(10000,display)
        T.after(9999,clearToTextInput)  
        progressBar['value'] = itr
        style.configure('text.Horizontal.TProgressbar',
                        text='Progress' + " " + itr + '%')

def tick_mark(no_grid, grid_size, no_tick):
    tick = []
    tick_label = []
    for i in range(0, no_grid+1, int(no_grid / no_tick)):
        tick.append(i)
        tick_label.append(str(i * grid_size))
    return tick, tick_label 

def inversion():
    if ("obsr_data" and "src_amp") and ("src_loc" and "rcr_loc") and ("in_mdl" and "par_file") in globals():
        #os.system("start D:\\fwi_version\\GUI_v1\\bin\\fwi.exe D:\\fwi_version\\GUI_v1\\example_1\\parameter_new.txt D:\\fwi_version\\GUI_v1\\example_1\\source_amp.txt D:\\fwi_version\\GUI_v1\\example_1\\initial_model.txt D:\\fwi_version\\GUI_v1\\example_1\\observe.txt D:\\fwi_version\\GUI_v1\\example_1\\source.txt D:\\fwi_version\\GUI_v1\\example_1\\receiver.txt")
        os.system("start D:\\fwi_version\\GUI_v1\\bin\\fwi.exe" +" " + par_file  + " " + src_amp + " " + in_mdl + " " +  obsr_data + " " + src_loc + " " + rcr_loc)
        #os.system("/media/vikas/Data/GUI/FWI/src/a.out /media/vikas/Data/GUI/FWI/input/parameter_new.txt /media/vikas/Data/GUI/FWI/input/source_amp.txt /media/vikas/Data/GUI/FWI/input/initial_model.txt /media/vikas/Data/GUI/FWI/input/observe.txt /media/vikas/Data/GUI/FWI/input/source.txt /media/vikas/Data/GUI/FWI/input/receiver.txt")
        #os.system("/media/vikas/Data/GUI/FWI/src/a.out" + " " + par_file  + " " + src_amp + " " + in_mdl + " " +  obsr_data + " " + src_loc + " " + rcr_loc + "&")
        #print("/media/vikas/Data/GUI/FWI/src/a.out" + " " + par_file  + " " + src_amp + " " + in_mdl + " " +  obsr_data + " " + src_loc + " " + rcr_loc + "&")
    else:
        messagebox.showerror("Error", "Please load the required files")
        

def browse_obsr_data():  
    global obsr_data  
    obsr_data = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("Text files",
                                                        "*.txt*"),
                                                       ("all files",
                                                        "*.*")))
      
    # Change label contents
    label_obsr_data.configure(text="File Opened: "+ obsr_data)    
    

def browse_in_mdl(): 
    global in_mdl   
    in_mdl = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("Text files",
                                                        "*.txt*"),
                                                       ("all files",
                                                        "*.*")))
      
    # Change label contents
    label_in_mdl.configure(text="File Opened: "+ in_mdl)    
    
def browse_src_amp():
    global src_amp    
    src_amp = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("Text files",
                                                        "*.txt*"),
                                                       ("all files",
                                                        "*.*")))
      
    # Change label contents
    label_src_amp.configure(text="File Opened: "+ src_amp)              

def browse_par_file():
    global par_file    
    global grid_size_plt
    global model_size_plt
    par_file = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("Text files",
                                                        "*.txt*"),
                                                       ("all files",
                                                        "*.*")))
      
    # Change label contents
    label_par_file.configure(text="File Opened: "+ par_file)    
    temp = open(par_file)
    read_lines = temp.read()
    para_list = read_lines.split()
    grid_size_plt = int(float(para_list[9]))
    model_size_plt = int(float(para_list[7]))

def browse_src_loc():    
    global src_loc
    src_loc = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("Text files",
                                                        "*.txt*"),
                                                       ("all files",
                                                        "*.*")))
      
    # Change label contents
    label_src_loc.configure(text="File Opened: "+ src_loc)        

def browse_rcr_loc():
    global rcr_loc    
    rcr_loc = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("Text files",
                                                        "*.txt*"),
                                                       ("all files",
                                                        "*.*")))
      
    # Change label contents
    label_rcr_loc.configure(text="File Opened: "+ rcr_loc)                 


def plt_init():
    if "in_mdl" and "par_file" in globals():
        v_init = np.loadtxt(in_mdl)
        if os.stat('../output/inverted/inverted_velocity.txt').st_size == 0:
            plot_data(v_init)
            plot_slice(v_init, v_init)
        else:
            v_inv = np.loadtxt("../output/inverted/inverted_velocity.txt")
            plot_data(v_init)
            plot_slice(v_inv, v_init)
    else:
        messagebox.showerror("Error", "Please load the required files")

def plt_rslt():
    if "in_mdl" and "par_file" in globals():
        v_init = np.loadtxt(in_mdl)
        #v_inv = np.loadtxt("../output/inverted/inverted_velocity.txt")
        if os.stat('../output/inverted/inverted_velocity.txt').st_size == 0:
            messagebox.showinfo("Information", "Inversion is in progress.")
        else:
            v_inv = np.loadtxt("../output/inverted/inverted_velocity.txt")
            plot_data(v_inv)
            plot_slice(v_inv, v_init)
    else:
        messagebox.showerror("Error", "Please load the required files")


def plot_data(v_plot):

    slice_pos = int(int(p1.get()) / grid_size_plt)

    if slice_pos > model_size_plt:
        messagebox.showerror("Error", "1-D slider is out of model dimension")

    else:
        matplotlib.use("TkAgg")
        # Create a figure of specific size
        fig1 = plt.figure()
        plt.imshow(v_plot)
        plt.axvline(x =  slice_pos, color = 'black')
        fig1.suptitle('Velocity Model', fontsize=20, y =0.95, fontweight="bold")
        plt.xlabel('Distance (m)', fontsize=10)
        plt.ylabel('Depth(m)', fontsize=10)
        
        #tick_mark(no_grid, grid_size, no_tick):
        tick, tick_label = tick_mark(len(v_plot[0]), grid_size_plt, 6)
        plt.gca().set_xticks(tick)
        plt.gca().set_xticklabels(tick_label)

        tick, tick_label = tick_mark(len(v_plot), grid_size_plt, 3)
        plt.gca().set_yticks(tick)
        plt.gca().set_yticklabels(tick_label)


        plt.colorbar(label="Velocity (m/s)", aspect=50, orientation="horizontal")
        canvas = FigureCanvasTkAgg(fig1, frame6 )
        canvas.get_tk_widget().grid(row=1, column=0, columnspan = 2, sticky = 'nsew')
        frame6.rowconfigure(1, weight = 1)
        frame6.columnconfigure(0, weight = 1)
        plt.close()

def plot_slice(v_inv, v_init):
    matplotlib.use("TkAgg")
    fig2 = plt.figure(figsize=(1, 7.6))
    fig2.suptitle('1-D Velocity Profile', fontsize=16, y =0.95, fontweight="bold")
    plt.xlabel('Velocity(m/s)', fontsize=10)
    plt.ylabel('Depth (m)', fontsize=10)
    slice_pos = int(int(p1.get()) / grid_size_plt)
    x_inv = v_inv[:,slice_pos]  # X-axis points
    x_init = v_init[:, slice_pos]
    y_inv =  range(0, len(v_inv)) # Y-axis points
    y_init = range(0, len(v_init))
    plt.plot(x_inv, y_inv, color="blue")
    plt.plot(x_init, y_init, color = 'red')
    plt.autoscale(enable=True, axis='y', tight=True)
    plt.gca().invert_yaxis()

    tick, tick_label = tick_mark(len(v_inv), 10, 10)
    plt.gca().set_yticks(tick)
    plt.gca().set_yticklabels(tick_label)
    
    plt.gca().tick_params(axis="y",direction="in", pad=-20)
    plt.legend(['Inverted', 'Initial'], loc = 'upper right')
    canvas = FigureCanvasTkAgg(fig2, frame8 )
    canvas.get_tk_widget().grid(row = 1, column=0, columnspan = 2, sticky = 'nsew')
    frame8.rowconfigure(1, weight = 1)
    frame8.columnconfigure(0, weight = 1)
    plt.close()



    

def para_window():
    global paratextarray
    global paraindex

    paratextarray = []
    paraindex = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17']

    para_frame = Toplevel(root)
    para_frame.title('Parameters')
    left_frame = Frame(para_frame, relief=GROOVE, borderwidth=2)
    left_frame.grid(row=0, column=0, rowspan = 2,  padx = 20, pady = (20,20),sticky = 'nsew')
    
    right_frame = Frame(para_frame, relief=GROOVE, borderwidth=2)
    right_frame.grid(row=0, column=1, padx = 20, pady = (20,0),sticky = 'nsew')

    btn_para = Frame(para_frame,relief=GROOVE, borderwidth=2)
    btn_para.grid(row=1, column=1, padx = 20, pady = (0,20),sticky = 'nsew')

    para_frame.rowconfigure(0,weight=1)
    para_frame.columnconfigure(0,weight=1)
    para_frame.rowconfigure(0,weight=1)
    para_frame.columnconfigure(1,weight=1)
    para_frame.rowconfigure(1,weight=1)
    para_frame.columnconfigure(1,weight=1)

    def sel1_2d():
        selection1_2d = str(inputtxt1_2d.get(1.0, "end-1c"))
        paraindex[0] = inputtxt1_2d.get(1.0, "end-1c")

    def sel2_2d():
        selection2_2d = str(inputtxt2_2d.get(1.0, "end-1c"))
        paraindex[1] = inputtxt2_2d.get(1.0, "end-1c")
      
    def sel3_2d():
        selection3_2d = str(inputtxt3_2d.get(1.0, "end-1c"))
        paraindex[2] = inputtxt3_2d.get(1.0, "end-1c")

    def sel4_2d():
        selection4_2d = str(inputtxt4_2d.get(1.0, "end-1c"))
        paraindex[3] = inputtxt4_2d.get(1.0, "end-1c")

    def sel5_2d():
        selection5_2d = str(inputtxt5_2d.get(1.0, "end-1c"))
        paraindex[4] = inputtxt5_2d.get(1.0, "end-1c")

    def sel6_2d():
        selection6_2d = str(inputtxt6_2d.get(1.0, "end-1c"))
        paraindex[5] = inputtxt6_2d.get(1.0, "end-1c")

    def sel7_2d():
        selection7_2d = str(inputtxt7_2d.get(1.0, "end-1c"))
        paraindex[6] = inputtxt7_2d.get(1.0, "end-1c")

    def sel8_2d():
        selection8_2d = str(inputtxt8_2d.get(1.0, "end-1c"))
        paraindex[7] = inputtxt8_2d.get(1.0, "end-1c")

    def sel9_2d():
        selection9_2d = str(inputtxt9_2d.get(1.0, "end-1c"))
        paraindex[8] = inputtxt9_2d.get(1.0, "end-1c")

    def sel10_2d():
        selection10_2d = str(inputtxt10_2d.get(1.0, "end-1c"))
        paraindex[9] = inputtxt10_2d.get(1.0, "end-1c")

    def sel11_2d():
        selection11_2d = str(inputtxt11_2d.get(1.0, "end-1c"))
        paraindex[10] = inputtxt11_2d.get(1.0, "end-1c")

    def sel12_2d():
        selection12_2d = str(inputtxt12_2d.get(1.0, "end-1c"))
        paraindex[11] = inputtxt12_2d.get(1.0, "end-1c")

    def sel13_2d():
        selection13_2d = str(inputtxt13_2d.get(1.0, "end-1c"))
        paraindex[12] = inputtxt13_2d.get(1.0, "end-1c")


    def sel14_2d():
        selection14_2d = str(inputtxt14_2d.get(1.0, "end-1c"))
        paraindex[13] = inputtxt14_2d.get(1.0, "end-1c")


    def sel15_2d():
        selection15_2d = str(inputtxt15_2d.get(1.0, "end-1c"))
        paraindex[14] = inputtxt15_2d.get(1.0, "end-1c")


    def sel16_2d():
        selection16_2d = str(inputtxt16_2d.get(1.0, "end-1c"))
        paraindex[15] = inputtxt16_2d.get(1.0, "end-1c")


    def sel17_2d():
        selection17_2d = str(inputtxt17_2d.get(1.0, "end-1c"))
        paraindex[16] = inputtxt17_2d.get(1.0, "end-1c")


    def sel_save_2d():
        filename = filedialog.asksaveasfilename(initialfile = 'Untitled.txt',
defaultextension=".txt",filetypes=[("All Files","*.*"),("Text Documents","*.txt")])
        save_parameters(filename)

    def save_parameters(filename):
        global paraindex
        sel1_2d()
        sel2_2d()
        sel3_2d()
        sel4_2d()
        sel5_2d()
        sel6_2d()
        sel7_2d()
        sel8_2d()
        sel9_2d()
        sel10_2d()
        sel11_2d()
        sel12_2d()
        sel13_2d()
        sel14_2d()
        sel15_2d()
        sel16_2d()
        sel17_2d()

        a_file_2d = open(filename, "w")
        x = ["number_sample", "sampling_intrvl", "depth_model", "lateral_model","grid_space", "depth_source", "depth_receiver", "number_src",
              "number_rcr", "processors", "max_iteration", "boundary_key","absorb_lay", "obj_fun_l2_1", "upper_avoid", "bound_avoid", "data_order_rm_1"]
    
        for i in range(0, 17):
            a_file_2d.write(x[i] + "\t" + str(paraindex[i]) + '\n')    

        a_file_2d.close()


    def sel_load_2d():
        filename = filedialog.askopenfilename(initialdir="/",
                                          title="Select a File",
                                          filetypes=(("Text files",
                                                      "*.txt*"),
                                                     ("all files",
                                                      "*.*")))

        with open(filename) as f:
            plist = list(zip(*(line.strip().split('\t') for line in f)))

            #print(plist)

        for i in range(17):
            paratextarray[i].delete(1.0, 'end')
            paratextarray[i].insert('end-1c', plist[1][i])


    var1_2D = DoubleVar()
    inputtxt1_2d = Text(left_frame,height =1, width = 10)
    inputtxt1_2d.grid(row = 0, column = 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Number of Samples", state=DISABLED, command=sel1_2d,height =1, width = 20, font=font_size)
    button.grid(row = 0, column=0, sticky='nsew')
    left_frame.rowconfigure(0,weight=1)
    paratextarray.append(inputtxt1_2d)
   

    var2 = DoubleVar()
    inputtxt2_2d = Text(left_frame,height =1, width = 10)
    inputtxt2_2d.grid(row = 1, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Sampling Interval", state=DISABLED, command=sel2_2d,height =1, width = 20, font=font_size)
    button.grid(row = 1, column= 0, sticky='nsew')
    ToolTip(button, msg="milli-sec")
    left_frame.rowconfigure(1,weight=1)
    paratextarray.append(inputtxt2_2d)

    var3 = DoubleVar()
    inputtxt3_2d = Text(left_frame,height =1, width = 10)
    inputtxt3_2d.grid(row = 2, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Depth Model", state=DISABLED, command=sel3_2d,height =1, width = 20, font=font_size)
    button.grid(row =2 , column= 0, sticky='nsew')
    ToolTip(button, msg="Number of grids")
    left_frame.rowconfigure(2,weight=1)
    paratextarray.append(inputtxt3_2d)

    var4 = DoubleVar()
    inputtxt4_2d = Text(left_frame,height =1, width = 10)
    inputtxt4_2d.grid(row = 3, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Horizontal Model", state=DISABLED, command=sel4_2d,height =1, width = 20, font=font_size)
    button.grid(row = 3, column= 0, sticky='nsew')
    ToolTip(button, msg="Number of grids")
    left_frame.rowconfigure(3, weight=1)
    paratextarray.append(inputtxt4_2d)

    var5 = DoubleVar()
    inputtxt5_2d = Text(left_frame,height =1, width = 10)
    inputtxt5_2d.grid(row = 4, column=1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Grid Size", state=DISABLED, command=sel5_2d,height =1, width = 20, font=font_size)
    button.grid(row = 4, column= 0, sticky='nsew')
    ToolTip(button, msg="metre")
    left_frame.rowconfigure(4,weight=1)
    paratextarray.append(inputtxt5_2d)

    var6 = DoubleVar()
    inputtxt6_2d = Text(left_frame,height =1, width = 10)
    inputtxt6_2d.grid(row = 5, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame , text="Depth of Source", state=DISABLED, command=sel6_2d,height =1, width = 20, font=font_size)
    button.grid(row = 5, column= 0, sticky='nsew')
    ToolTip(button, msg="Grid numbers")
    left_frame.rowconfigure(5,weight=1)
    paratextarray.append(inputtxt6_2d)

    var7 = DoubleVar()
    inputtxt7_2d = Text(left_frame,height =1, width = 10)
    inputtxt7_2d.grid(row = 6, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Depth of Receiver", state=DISABLED, command=sel7_2d,height =1, width = 20, font=font_size)
    button.grid(row = 6, column= 0, sticky='nsew')
    ToolTip(button, msg="Grid numbers")
    left_frame.rowconfigure(6,weight=1)
    paratextarray.append(inputtxt7_2d)

    var8_2D = DoubleVar()
    inputtxt8_2d = Text(left_frame,height =1, width = 10)
    inputtxt8_2d.grid(row = 7, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Number of Source", state=DISABLED, command=sel8_2d,height =1, width = 20, font=font_size)
    button.grid(row = 7, column= 0, sticky='nsew')
    left_frame.rowconfigure(7,weight=1)
    paratextarray.append(inputtxt8_2d)

    var2 = DoubleVar()
    inputtxt9_2d = Text(left_frame,height =1, width = 10)
    inputtxt9_2d.grid(row = 8, column= 1, padx=10, sticky = 'nsew')
    button = Button(left_frame, text="Number of Receiver", state=DISABLED, command=sel9_2d,height =1, width = 20, font=font_size)
    button.grid(row = 8, column= 0, sticky='nsew')
    left_frame.rowconfigure(8,weight=1)
    paratextarray.append(inputtxt9_2d)

    left_frame.columnconfigure(1,weight=1)
    left_frame.columnconfigure(0,weight=1)

    var1_2D = DoubleVar()
    inputtxt10_2d = Text(right_frame,height =1, width = 10)
    inputtxt10_2d.grid(row =0 , column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Processors", state=DISABLED, command=sel10_2d,height =1, width = 20, font=font_size)
    button.grid(row = 0, column= 0, sticky='nsew')
    ToolTip(button, msg="Factor of shots")
    right_frame.rowconfigure(0,weight=1)
    paratextarray.append(inputtxt10_2d)

    var2 = DoubleVar()
    inputtxt11_2d = Text(right_frame,height =1, width = 10)
    inputtxt11_2d.grid(row = 1, column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Iterations", state=DISABLED, command=sel11_2d,height =1, width = 20, font=font_size)
    button.grid(row = 1, column= 0, sticky='nsew')
    ToolTip(button, msg="Maximum number of iterations")
    right_frame.rowconfigure(1,weight=1)
    paratextarray.append(inputtxt11_2d)

    var3 = DoubleVar()
    inputtxt12_2d = Text(right_frame,height =1, width = 10)
    inputtxt12_2d.grid(row = 2, column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Boundary Key", state=DISABLED, command=sel12_2d,height =1, width = 20, font=font_size)
    button.grid(row = 2, column= 0, sticky='nsew')
    ToolTip(button, msg="On: 1 \n Off: 0")
    right_frame.rowconfigure(2,weight=1)
    paratextarray.append(inputtxt12_2d)

    var4 = DoubleVar()
    inputtxt13_2d = Text(right_frame,height =1, width = 10)
    inputtxt13_2d.grid(row =3 , column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Absorbing Layer", state=DISABLED, command=sel13_2d,height =1, width = 20, font=font_size)
    button.grid(row = 3, column= 0, sticky='nsew')
    ToolTip(button, msg="Grids reserved for boundary")
    right_frame.rowconfigure(3,weight=1)
    paratextarray.append(inputtxt13_2d)

    var5 = DoubleVar()
    inputtxt14_2d = Text(right_frame,height =1, width = 10)
    inputtxt14_2d.grid(row = 4, column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Objective Function", state=DISABLED, command=sel14_2d,height =1, width = 20, font=font_size)
    button.grid(row = 4, column= 0, sticky='nsew')
    ToolTip(button, msg="L2_norm: 1 \n Cross-correlation: 0")
    right_frame.rowconfigure(4,weight=1)
    paratextarray.append(inputtxt14_2d)

    var6 = DoubleVar()
    inputtxt15_2d = Text(right_frame,height =1, width = 10)
    inputtxt15_2d.grid(row = 5, column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Mute Upper Layer", state=DISABLED, command=sel15_2d,height =1, width = 20, font=font_size)
    button.grid(row = 5, column= 0, sticky='nsew')
    ToolTip(button, msg="Grid numbers")
    right_frame.rowconfigure(5,weight=1)
    paratextarray.append(inputtxt15_2d)

    var7 = DoubleVar()
    inputtxt16_2d = Text(right_frame,height =1, width = 10)
    inputtxt16_2d.grid(row =6, column=1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Mute Boudary Layer", state=DISABLED, command=sel16_2d,height =1, width = 20, font=font_size)
    button.grid(row = 6, column= 0, sticky='nsew')
    ToolTip(button, msg="Grid numbers")
    right_frame.rowconfigure(6,weight=1)
    paratextarray.append(inputtxt16_2d)

    var8_2D = DoubleVar()
    inputtxt17_2d = Text(right_frame,height =1, width = 10)
    inputtxt17_2d.grid(row = 7, column= 1, padx=10, sticky='nsew')
    button = Button(right_frame, text="Data Order", state=DISABLED, command=sel17_2d,height =1, width = 20, font=font_size)
    button.grid(row = 7, column= 0, sticky='nsew')
    ToolTip(button, msg="Row major: 1 \n Column Major: 0")
    right_frame.rowconfigure(7,weight=1)
    paratextarray.append(inputtxt17_2d)

    right_frame.columnconfigure(1,weight=1)
    right_frame.columnconfigure(0,weight=1)

    var_save = DoubleVar()
    button = Button(btn_para,  text="Save Parameters", command=sel_save_2d, bg = 'green',height =1, width = 15, font=font_size)
    button.grid(row = 0, column= 0, sticky='nsew')
    button = Button(btn_para,  text="Load Parameters", command=sel_load_2d, bg = 'green',height =1, width = 15, font=font_size)
    button.grid(row = 0, column= 1, sticky='nsew')
    btn_para.rowconfigure(0,weight=1)
    btn_para.columnconfigure(0,weight=1)
    btn_para.columnconfigure(1,weight=1)
    # Toplevel object which will
    # be treated as a new window
    
 
    # sets the title of the
    # Toplevel widget

 
    # A Label widget to show in toplevel

def acq_geo():
    acq_frame = Toplevel(root)
    acq_frame.title('Acquisition Geometry')
    workbook = xlsxwriter.Workbook('acq_temp.xlsx')
    worksheet = workbook.add_worksheet()

    def open_spread():
        global n_s
        global n_r
        global u_b
        global l_b
        n_s = number_src.get()
        n_r = number_rcr.get()
        u_b = number_up.get()
        l_b = number_low.get()
        for i in range (int(n_s)):
            worksheet.write(0,i+ 1, str('S#' + str(i)))

        for i in range (int(n_r)):
            worksheet.write(i+ 2, 0,  str('R#' + str(i)))
        
        workbook.close()
        #os.startfile('acq_temp.xlsx')


    def generate_src_file():
        if os.stat('acq_temp.xlsx').st_size == 0:
            messagebox.showinfo("Information", "Please generate the spreadsheet first.")
        else:
            src_dataframe = pd.read_excel('acq_temp.xlsx', usecols=range(1, int(n_s) + 1))
            src = src_dataframe.drop(range(1, int(n_r) + 1))
            src_string = src.to_string(header=False, index=False)
            File = filedialog.asksaveasfile(initialfile = 'Untitled.txt',
defaultextension=".txt",filetypes=[("All Files","*.*"),("Text Documents","*.txt")])
            File.write(src_string)
            File.close()
            temp = np.loadtxt(os.path.abspath(File.name))
            if np.any(temp > int(u_b)) or np.any(temp < int(l_b)) == True:
                messagebox.showwarning("Warning", "Source position is out of bound")

    def generate_rcr_file():
        if os.stat('acq_temp.xlsx').st_size == 0:
            messagebox.showinfo("Information", "Please generate the spreadsheet first.")
        else:
            rcr_dataframe = pd.read_excel('acq_temp.xlsx', usecols=range(1, int(n_s)  + 1))
            rcr = rcr_dataframe.drop([0])
            rcr_string = rcr.to_string(header=False, index=False)
            myFile = filedialog.asksaveasfile(initialfile = 'Untitled.txt',
defaultextension=".txt",filetypes=[("All Files","*.*"),("Text Documents","*.txt")])
            myFile.write(rcr_string)
            myFile.close()
            temp = np.loadtxt(os.path.abspath(myFile.name))
            if np.any(temp > int(u_b)) or np.any(temp < int(l_b)) == True:
                messagebox.showwarning("Warning", "Receiver position is out of bound")

    def browse_src():
        global src_file
        src_file = filedialog.askopenfilename(initialdir = "/",
                                            title = "Select a File",
                                            filetypes = (("Text files",
                                                            "*.txt*"),
                                                        ("all files",
                                                            "*.*")))
        
        # Change label contents
        label_src_file.configure(text="File Opened: "+ src_file, font=font_size)

    

    def browse_rcr():
        global rcr_file
        rcr_file = filedialog.askopenfilename(initialdir = "/",
                                            title = "Select a File",
                                            filetypes = (("Text files",
                                                            "*.txt*"),
                                                        ("all files",
                                                            "*.*")))
        
        # Change label contents
        label_rcr_file.configure(text="File Opened: "+ rcr_file, font=font_size)


    def plot_geometry():
        fig3 = plt.figure()
        src_data = np.loadtxt(src_file, dtype = 'int')
        rcr_data = np.loadtxt(rcr_file, dtype = 'int')
        for i in range(len(rcr_data[0])):
            rcr_temp = rcr_data[:, i]
            y_temp = 1 + i * np.ones(len(rcr_temp))
            plt.scatter(rcr_temp, y_temp, color = 'green', marker = '^')          
            plt.scatter(src_data[i], i + 1, color = 'red', marker = '*')
            plt.title("Acquisition geometry")
            plt.legend(["Receiver", "Source"])
            plt.xlabel("Gird number")
            plt.ylabel("Number of shots")
        plt.show()
        

    frame_acq1 = Frame(acq_frame, relief = 'ridge', borderwidth=1, border = 1)
    frame_acq1.grid(row = 0, column = 0, sticky='nsew')
    acq_frame.rowconfigure(0, weight = 1)
    acq_frame.columnconfigure(0, weight = 1)

    frame_acq2 = Frame(acq_frame, relief = 'ridge', borderwidth=1, border=1)
    frame_acq2.grid(row = 0, column = 1, sticky = 'nsew')
    acq_frame.columnconfigure(0, weight = 1)

    frame_acq3 = Frame(acq_frame,relief = 'ridge', borderwidth=1, border=1)
    frame_acq3.grid(row = 1, column = 0, columnspan = 2)
    acq_frame.rowconfigure(1, weight = 1)

    label_src = Label(frame_acq1, text ="Number of sources", font=font_size)
    label_src.grid(row = 0, column = 0, padx = (5,0), pady = 2)
    number_src = Entry(frame_acq1, width = 7, font=font_size)
    number_src.grid(row = 0, column = 1, padx = (0,5), pady = 2)

    label_rcr = Label(frame_acq1, text ="Number of receivers", font=font_size)
    label_rcr.grid(row = 1, column = 0, padx = (5,0), pady = 2)
    number_rcr = Entry(frame_acq1, width = 7, font=font_size)
    number_rcr.grid(row = 1, column = 1, padx = (0,5), pady = 2)

    label_low = Label(frame_acq1, text ="Lower bound", font=font_size)
    label_low.grid(row = 2, column = 0, padx = (5,0), pady = 2)
    number_low = Entry(frame_acq1, width = 7, font=font_size)
    number_low.grid(row = 2, column = 1, padx = (0,5), pady = 2)

    label_up = Label(frame_acq1, text ="Upper bound", font=font_size)
    label_up.grid(row = 3, column = 0, padx = (5,0), pady = 2)
    number_up = Entry(frame_acq1, width = 7, font=font_size)
    number_up.grid(row = 3, column = 1, padx = (0,5), pady = 2)

    btn = Button(frame_acq2, text= "Open spreadsheet", command= open_spread, font=font_size)
    btn.grid(row = 0, column = 0, sticky = 'nsew', padx = 10, pady = 5)

    btn = Button(frame_acq2, text= "Generate source file", command= generate_src_file, font=font_size)
    btn.grid(row = 1, column = 0, sticky = 'nsew', padx = 10, pady = (0, 5))

    btn = Button(frame_acq2, text= "Generate receiver file", command= generate_rcr_file, font=font_size)
    btn.grid(row = 2, column = 0, sticky = 'nsew', padx = 10, pady = (0, 5))

    btn = Button(frame_acq3, text = 'Source file', command = browse_src, font=font_size)
    btn.grid(row = 0, column = 0, sticky = 'nsew', pady = 5)
    label_src_file = Label(frame_acq3, text = 'Source location:', width = 44, height = 1, fg = "blue", bg = 'white', font = font_size)
    label_src_file.grid(row = 0, column = 1, sticky = 'nsew', padx = 10, pady = 5)

    btn = Button(frame_acq3, text = 'Receiver file', command = browse_rcr, font=font_size)
    btn.grid(row = 1, column = 0, sticky = 'nsew')
    label_rcr_file = Label(frame_acq3, text = 'Receiver location:', width = 44, height = 1, fg = "blue", bg = 'white', font = font_size)
    label_rcr_file.grid(row = 1, column = 1, sticky = 'nsew', padx = 10)

    btn = Button(frame_acq3, text = 'Geometry plot', command = plot_geometry, font=font_size)
    btn.grid(row = 2, column = 0, columnspan = 2, sticky = 'w', pady = 5)

def help_menu():
    webbrowser.open_new(r"https://github.com/vatsvikki/Manual-FWI")

def about_menu():
    webbrowser.open_new(r"https://github.com/vatsvikki")



    # Create the root root
root = Tk()

    # Set root title
root.title('Full waveform inversion')
    
    # Set root size
root.geometry('1670x850')
    
    #Set root background color
root.config()




'''
frame1 = Frame(master=root,relief=RAISED,borderwidth=1)
frame1.grid(row=0, column=0)
label1 = Label(master=frame1,  text=f"Row {0}\nColumn {0}")
label1.grid()

frame2 = Frame(master=root,relief=RAISED,borderwidth=1)
frame2.grid(row=0, column=1, columnspan=2)
label2 = Label(master=frame2, text=f"Row {0}\nColumn {1}")
label2.grid()

frame3 = Frame(master=root,relief=RAISED,borderwidth=1)
frame3.grid(row=0, column=2)
label3 = Label(master=frame3, text=f"Row {0}\nColumn {2}")
label3.grid()

frame4 = Frame(master=root,relief=RAISED,borderwidth=1)
frame4.grid(row=0, column=3)
label4 = Label(master=frame4, text=f"Row {0}\nColumn {3}")
label4.grid()
'''
frame5 = Frame(master=root,relief=GROOVE,borderwidth=10)
frame5.grid(row=0, column=0, sticky = 'nsew')
label5 = Label(master=frame5, text="Input Data", font=font_size)
label5.grid(row=0, column=0, sticky = 'nw')




frame6 = Frame(master=root,relief=SUNKEN,borderwidth=5)
frame6.grid(row=0, column=1, rowspan = 2, columnspan= 2, sticky = 'nsew')
label6 = Label(master=frame6)
label6.grid(sticky = 'nsew')


'''
frame7 = tk.Frame(master=root,relief=tk.RAISED,borderwidth=1)
frame7.grid(row=1, column=2)
label7 = tk.Label(master=frame7, text=f"Row {1}\nColumn {2}")
label7.grid()
'''
frame8 = Frame(master=root, relief = SUNKEN, borderwidth=5)
frame8.grid(row=0, column=3, rowspan=3, sticky = 'nsew')
label8 = Label(master=frame8)
label8.grid(sticky = 'nsew')



frame9 = Frame(master=root,relief=GROOVE,borderwidth=10)
frame9.grid(row=1, column=0, sticky = 'nsew')
label9 = Label(master=frame9, text="Acquisition Geometry",  font=font_size)
label9.grid(row=0, column=0, sticky = 'nw')
'''
frame10 = tk.Frame(master=root,relief=tk.RAISED,borderwidth=1)
frame10.grid(row=2, column=1)
label10 = tk.Label(master=frame10, text=f"Row {2}\nColumn {1}")
label10.grid()

frame11 = tk.Frame(master=root,relief=tk.RAISED,borderwidth=1)
frame11.grid(row=2, column=2)
label11 = tk.Label(master=frame11, text=f"Row {2}\nColumn {2}")
label11.grid()
'''
'''
frame12 = tk.Frame(master=root,relief=tk.RAISED,borderwidth=1)
frame12.grid(row=2, column=3)
label12 = tk.Label(master=frame12, text=f"Row {2}\nColumn {3}")
label12.grid()
'''
frame13 = Frame(master=root,relief=GROOVE,borderwidth=10)
frame13.grid(row=2, column=0, sticky = 'nsew')
label13 = Label(master=frame13, text="Input Parameters", font=font_size)
label13.grid(row=0, column=0, sticky = 'nw')


frame14 = Frame(master=root, relief=SUNKEN,borderwidth=5)
frame14.grid(row=2, column=1, columnspan=2, sticky = 'nsew')

'''
frame15 = Frame(master=root,relief=RAISED,borderwidth=1)
frame15.grid(row=3, column=2)
label15 = Label(master=frame15, text=f"Row {3}\nColumn {2}")
label15.grid()
'''
'''
frame16 = tk.Frame(master=root,relief=tk.RAISED,borderwidth=1)
frame16.grid(row=3, column=3)
label16 = tk.Label(master=frame16, text=f"Row {3}\nColumn {3}")
label16.grid()
'''
root.rowconfigure(1, weight = 1)
root.rowconfigure(0, weight = 1)
root.columnconfigure(0, weight = 1)
root.columnconfigure(1, weight = 1)
root.columnconfigure(3, weight = 1)
root.rowconfigure(1, weight = 1)
root.rowconfigure(2, weight = 1)

# Create a File Explorer label
label_obsr_data = Label(frame5,
                            text = "Observe Data:",
                            width= 30, height= 1,
                            fg = "blue", bg = 'white', font=font_size)

button_obsr_data = Button(frame5,
                        text = "Browse", 
                        
                        command = browse_obsr_data, font=font_size)
  
# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_obsr_data.grid(row = 1, column = 0, padx = 20, pady = (20, 0), sticky = 'n')  
#button_obsr_data.grid(row = 2, column = 0, sticky = 'w')  
button_obsr_data.place(x = 10, y = 70)


# Create a File Explorer label
label_in_mdl = Label(frame5,
                            text = "Initial Model:",
                            width= 30, height= 1,
                            fg = "blue", bg = 'white', font=font_size)

button_in_mdl = Button(frame5,
                        text = "Browse",                         
                        command = browse_in_mdl, font=font_size)

# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_in_mdl.grid(row = 3, column = 0, padx = 20, pady = (60, 0),sticky = 'n')  
#button_in_mdl.grid(row = 4, column = 0, sticky = 'w')  
button_in_mdl.place(x = 10, y = 155)


# Create a File Explorer label
label_src_amp = Label(frame5,
                            text = "Source Amplitude:",
                            width= 30, height= 1,
                            fg = "blue", bg = 'white', font=font_size)

button_src_amp = Button(frame5,
                        text = "Browse", 
                       
                        command = browse_src_amp, font=font_size)
  
# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_src_amp.grid(row = 5, column = 0, padx = 20, pady = (60, 0),sticky = 'n')  
#button_src_amp.grid(row = 6, column = 0, sticky = 'w')  
button_src_amp.place(x = 10, y = 240)
frame5.columnconfigure(0, weight = 1)

  
# Create a File Explorer label
label_par_file = Label(frame13,
                            text = "Parameter File:",
                            width= 30, height= 1,
                            fg = "blue", bg = 'white',  font=font_size)

button_par_file = Button(frame13,
                        text = "Browse", 

                        command = browse_par_file, font=font_size)
  
# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_par_file.grid(row = 1, column = 0, padx = 20, pady = (20, 0),sticky = 'n')   
#button_par_file.grid(row = 2, column = 0, sticky = 'w') 
button_par_file.place(x = 10, y = 70)
frame13.columnconfigure(0, weight = 1)


  
# Create a File Explorer label
label_src_loc = Label(frame9,
                            text = "Source Location:",
                            width= 30, height= 1,
                            fg = "blue", bg = 'white', font=font_size)

button_src_loc = Button(frame9,
                        text = "Browse", 
                        
                        command = browse_src_loc, font=font_size)
  
# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_src_loc.grid(row = 1, column = 0, padx = 20, pady = (20, 0),sticky = 'n')  
#button_src_loc.grid(row = 2, column = 0, sticky = 'w')  
button_src_loc.place(x = 10, y = 70)

# Create a File Explorer label
label_rcr_loc = Label(frame9,
                            text = "Receiver Location:",
                            width= 30, height= 1,
                            fg = "blue", bg = 'white', font=font_size)

button_rcr_loc = Button(frame9,
                        text = "Browse", 
                       
                        command = browse_rcr_loc, font=font_size)
  
# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_rcr_loc.grid(row = 3, column = 0, padx = 20, pady = (60, 0),sticky = 'n')  
#button_rcr_loc.grid(row = 4, column = 0,  sticky = 'w')  
button_rcr_loc.place(x = 10, y = 155)
frame9.columnconfigure(0, weight = 1)



menubar = Menu(root)
filemenu = Menu(menubar, tearoff=0)
filemenu.add_command(label="Exit", command=root.quit)
menubar.add_cascade(label="File", menu=filemenu)

toolbox = Menu(menubar, tearoff=0)
toolbox.add_command(label="Parameters", command=para_window)
toolbox.add_command(label="Acquisition geometry", command=acq_geo)
menubar.add_cascade(label="Toolbox", menu=toolbox)

helpmenu = Menu(menubar, tearoff=0)
helpmenu.add_command(label="Help Index", command=help_menu)
helpmenu.add_command(label="About...", command=about_menu)
menubar.add_cascade(label="Help", menu=helpmenu)

root.config(menu=menubar)


T = Text(frame14, width=120, height=5)
T.grid(row = 1, column = 0, columnspan = 6, sticky = 'nsew')

photo_status = PhotoImage(file = 'status.png')
btn_sts = Button(frame14, image = photo_status,  borderwidth=0, command=display, font=font_size)
btn_sts.grid(row = 0, column = 0, sticky = 'w')
ToolTip(btn_sts, msg="Live status")

photo_save = PhotoImage(file = 'save.png')
btn_save = Button(frame14, image = photo_save,  borderwidth=0, command = save_file)
btn_save.grid(row = 0, column = 3)
ToolTip(btn_save, msg="Save inverted data")

photo_restart = PhotoImage(file = 'restart.png')
btn_rstrt = Button(frame14, image = photo_restart,  borderwidth=0, command = restart)
btn_rstrt.grid(row = 0, column = 4)
ToolTip(btn_rstrt, msg="Restart")

photo_quit = PhotoImage(file = 'quit.png')
btn_qt = Button(frame14, image = photo_quit,  borderwidth=0,  command=quit_win, font=font_size)
btn_qt.grid(row = 0, column = 5, sticky = 'e')
ToolTip(btn_qt, msg="Quit")



style = ttk.Style(frame14)
style.layout('text.Horizontal.TProgressbar',
             [('Horizontal.Progressbar.trough',
               {'children': [('Horizontal.Progressbar.pbar',
                              {'side': 'left', 'sticky': 'ns'})],
                'sticky': 'nswe'}),
              ('Horizontal.Progressbar.label', {'sticky': ''})])
              # ,lightcolor=None,bordercolo=None,darkcolor=None
style.configure('text.Horizontal.TProgressbar', text='Progress 0 %')

progressBar = Progressbar(frame14,style='text.Horizontal.TProgressbar', length=700,   value=0,)
progressBar.place(x = 80, y = 6)



frame14.rowconfigure(1, weight = 1)
frame14.columnconfigure(0, weight = 1)



photo_run = PhotoImage(file = 'run.png')
btn_run = Button(frame14, image = photo_run,  borderwidth=0, command=threading.Thread(target=inversion).start, font=font_size)
btn_run.grid(row = 0, column = 2, sticky = 'w')
ToolTip(btn_run, msg="Run")


lbl_vel_pr = Label(frame8, text="1-D Profile horizontal distance(m)", font=font_size)
lbl_vel_pr.grid(row = 0, column = 0, sticky = 'w')
p1=Entry(frame8, width = 6, bd=3, font =font_size)
p1.grid(row = 0, column = 1, sticky = 'w')


b2 = Button(frame6, text="Initial Model",  command=plt_init, font=font_size)
b2.grid(row=0, column=0,  sticky = 'w')
b1 = Button(frame6, text="Inverted Model", command=plt_rslt, font=font_size)
b1.grid(row = 0, column=1)


matplotlib.use("TkAgg")
# Create a figure of specific size
ovr_thrst = np.loadtxt("real_overthrust.txt")
#fig1 = plt.figure()
fig1 = plt.figure()
plt.imshow(ovr_thrst)
p1.insert(0, 10 * int(len(ovr_thrst[0])/2))
plt.axvline(x =  int(len(ovr_thrst[0])/2), color = 'black', label = 'axvline - full height')
fig1.suptitle('Overthrust Model', fontsize=20, y =0.9, fontweight="bold")
plt.xlabel('Distance (m)', fontsize=10)
plt.ylabel('Depth(m)', fontsize=10)

#tick_mark(no_grid, grid_size, no_tick):
tick, tick_label = tick_mark(len(ovr_thrst[0]), 10, 6)
plt.gca().set_xticks(tick)
plt.gca().set_xticklabels(tick_label)

tick, tick_label = tick_mark(len(ovr_thrst), 10, 3)
plt.gca().set_yticks(tick)
plt.gca().set_yticklabels(tick_label)


plt.colorbar(label="Velocity (m/s)", aspect=30, orientation="horizontal")
canvas = FigureCanvasTkAgg(fig1, frame6 )
canvas.get_tk_widget().grid(row=1, column=0, columnspan = 2, sticky = 'nsew')
frame6.rowconfigure(1, weight = 1)
frame6.columnconfigure(0, weight = 1)
plt.close()

fig2 = plt.figure(figsize=(1, 7.6))
fig2.suptitle('1-D Velocity Profile', fontsize=16, y =0.95, fontweight="bold")
plt.xlabel('Velocity(m/s)', fontsize=10)
plt.ylabel('Depth (m)', fontsize=10)
x_o = ovr_thrst[:, int(len(ovr_thrst[0])/2)]  # X-axis points
y_o = range(0, len(ovr_thrst))
plt.plot(x_o, y_o, color="blue")
plt.autoscale(enable=True, axis='y', tight=True)
plt.gca().invert_yaxis()
tick, tick_label = tick_mark(len(ovr_thrst), 10, 10)
plt.gca().set_yticks(tick)
plt.gca().set_yticklabels(tick_label)
plt.gca().tick_params(axis="y",direction="in", pad=-20)
plt.legend(['Velocity'], loc = 'best')
canvas = FigureCanvasTkAgg(fig2, frame8 )
canvas.get_tk_widget().grid(row = 1, column=0, columnspan = 2, sticky = 'nsew')
frame8.rowconfigure(1, weight = 1)
frame8.columnconfigure(0, weight = 1)
plt.close()



#Create a label
# Let the root wait for any events
root.protocol("WM_DELETE_WINDOW", sys.exit)
root.mainloop()
