from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import random
import numpy as np
import sys
import Tkinter as tk
import time

def function1(fig, ax):
  ax.cla()
  color_grade_classes = ['#80FF00','#FFFF00','#FF8000', '#FF0000']
  varsi =  random.randint(1, 100)

  for colors, rows  in zip(color_grade_classes, [3,2,1,0] ):  
    indexs = np.arange(5)
    heights = [varsi,varsi/2,varsi/3,0,0]
    ax.bar(indexs, heights, zs = rows,  zdir='y', color=colors, alpha=0.8)
  return fig

class App():
    def __init__(self):
        self.root = tk.Tk()
        self.root.wm_title("Embedding in TK")

        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.fig = function1(self.fig, self.ax)

        self.canvas = FigureCanvasTkAgg(self.fig, master=self.root)
        self.toolbar = NavigationToolbar2TkAgg( self.canvas, self.root )
        self.toolbar.update()
        self.canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=1)

        self.label = tk.Label(text="")
        self.label.pack()
        self.update_clock()
        self.root.mainloop()

    def update_clock(self):
        self.fig = function1(self.fig,self.ax)
        self.canvas.show()
        self.canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=1)
        now = time.strftime("%H:%M:%S")
        self.label.configure(text=now)
        self.root.after(1000, self.update_clock)

app=App()
