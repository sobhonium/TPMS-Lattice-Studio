from subprocess import list2cmdline
import pyvista as pv
from genericpath import isfile
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QApplication, QCheckBox, QColorDialog,
                             QFileDialog, QGridLayout, QGroupBox, QHBoxLayout,
                             QLabel, QPushButton, QRadioButton, QSlider, QScrollArea,           
                             QTabWidget, QTextEdit, QVBoxLayout, QWidget, QPlainTextEdit, QTableView, QSpinBox, QDoubleSpinBox, QFileDialog, QTableWidget, QTableWidgetItem,QTextEdit,QAbstractScrollArea, QListWidget, QListWidgetItem)
from pyvistaqt import QtInteractor
from PyQt5.QtGui import QColor
import PyQt5.QtGui
import os

#TODO(SBN): 


class   ShowTabUI(QTabWidget):
    def __init__(self):
        QTabWidget.__init__(self, parent=None)
        # initial colors        
        self.show_tab_UI()
    
    def msg_print(self, string_value):
        """a sort of msg from the program to user to tell him how his 
           latest operation is done by the program.
        Args:
            string_value ([string]): a msg from program to user
        """
        
        msg = "msg: " + string_value
        self.lbl_msg.setText(msg) 
        print(msg)



    def clear_screen(self):
        """simply, this function clears the screen at once"""
        self.plotter.clear()
        self.btn_clear.setEnabled(False)
        self.msg_print("Screen Cleaned.")


    def slider_on_progress(self):
        """if any of the sliders are changed this function is called 
            automatically"""
        values =""
        # there are 6 parameters to look at
        for i in range(1, 7+5 ):
            # print(self.slider[i].value())
            values= values + " " + str(self.slider[i].value())
            self.lbl_x[i].setText(str(self.slider[i].value()))
        self.msg_print("parameters are changed to:" + values)
    

    def plt_on_screen(self):
        """ If the plot button is clicked, this function looks at the checkbox
           buttons and dispalys the customized output """
        # first off clean the screen
        # self.clear_screen()

        list_param = [str(self.slider[i].value()) for i in range(1, 6+1)]
        patt = '-'.join(list_param)
        
        if self.chbox_surf_point.isChecked():
            surf_path = self.chosen_path + patt + '.obj'
            if isfile(surf_path):
                mesh_surf = pv.read(surf_path)
                self.plotter.add_mesh(mesh_surf, show_edges=True, 
                                      color=self.surf_point_color, 
                                      label='surf point')
                # self.plotter.reset_camera()
                self.plotter.add_axes()
                self.msg_print("file "+ surf_path + " is found." )
                self.btn_clear.setEnabled(True)
                
            else:
                self.msg_print("There is no such file by the name "+ surf_path)

        if self.chbox_vol.isChecked():
            vol_path = self.chosen_path + patt + '-vol.obj'
            if isfile(vol_path):
                mesh_vol = pv.read(vol_path)
                self.plotter.add_mesh(mesh_vol, show_edges=True, 
                                      color=self.vol_color, label='Volume')
                # self.plotter.reset_camera()
                self.plotter.add_axes()
                self.msg_print("file "+ vol_path+ " is found ")
                self.btn_clear.setEnabled(True)
                
            else:
                self.msg_print("There is no such file by the name "+ vol_path )
        
        if self.chbox_surf.isChecked():
            surf_path = self.chosen_path + patt + '-surface.obj'
            if isfile(surf_path):
                mesh_vol = pv.read(surf_path)
                #  if the mesh checkbox is ticked this one shows the edge lines
                #     on the surface
                self.plotter.add_mesh(
                    mesh_vol, show_edges=self.chbox_surf_edge.isChecked(), 
                    color=self.surf_color, label='Surf', 
                     lighting=True)
                # self.plotter.reset_camera()
                self.plotter.add_axes()
                self.msg_print("file "+ surf_path+ " is found. ")
                self.btn_clear.setEnabled(True)

                 
                
            else:
                self.msg_print("There is no such file by the name "+ surf_path )
        
        if self.chbox_grid.isChecked():
            self.plotter.show_grid()
        

    def which_path(self):
        """ This function gives the user a front end tool to choose a path for
              loading 
        """
        self.chosen_path = QFileDialog.getExistingDirectory(self,"Choose Path",
                                                          self.chosen_path) + "/"
        self.msg_print("path is changed into: " + self.chosen_path )
       

    def show_tab_UI(self):
        """Create the Show UI."""
        
        # for future use in order to deactivate them all at once
        # active_actors = []
     
        self.chosen_path = "./"  

        self.surf_point_color = "#bfc9ca"    
        self.vol_color = "#6600ff"    
        self.surf_color = "#0cc9cc"
        
        
        main_layout  = QHBoxLayout()
        right_layout = QGridLayout()
        left_layout  = QGridLayout()
        
        self.setLayout(main_layout)
        

        # show_Tab = QWidget()
        # show_Tab.setLayout(main_layout)
        
        main_layout.addLayout(left_layout )
        main_layout.addLayout(right_layout)

    

        self.plotter = QtInteractor(self)
        self.lbl_msg = QLabel("msg: ")
        left_layout  . addWidget(self.lbl_msg, 1,0)
        left_layout.addWidget(self.plotter.interactor, 0,0)
        

        #  add a sphere to the pyqt frame ,initally
        sphere = pv.Sphere()
        
        self.actor1 = self.plotter.add_mesh(sphere, show_edges=False, 
                                            color= self.surf_color, style='wireframe')
        # active_actors += "self.actor1"
        # self.plotter.reset_camera()

    
    


        self.btn_path = QPushButton("Path")
        self.btn_path.clicked.connect(self.which_path)
        right_layout.addWidget(self.btn_path,0,0)
        
        
        self.gbox = QGroupBox ("x param")
        self.gbox_layout = QGridLayout()
        self.gbox.setLayout(self.gbox_layout)

        right_layout.addWidget(self.gbox,2,0)
        

        # checks if any of params changes
        self.slider = [1,1,1,1,1,1,1, 120, 120, 120, 120, 120] 
        self.lbl_x_stick = [1,1,1,1,1,1,1, 120, 120, 120, 120, 120] 
        self.lbl_x = [1,1,1,1,1,1,1, 120, 120, 120, 120, 120] 
        for i in range(1, 6+1):
            self.slider[i] = QSlider()
            self.slider[i].setOrientation(Qt.Vertical)
            self.slider[i].setTickPosition(QSlider.TicksBelow)
            self.slider[i].setTickInterval(1)
            self.slider[i].setMinimum(0)
            self.slider[i].setMaximum(4)
            self.gbox_layout.addWidget(self.slider[i],1,0+i)
            self.slider[i].valueChanged[int].connect( self.slider_on_progress)

            self.lbl_x_stick[i] = QLabel("x"+str(i))
            self.gbox_layout.addWidget(self.lbl_x_stick[i],2,0+i)

            self.lbl_x[i] = QLabel("1")
            self.gbox_layout.addWidget(self.lbl_x[i],0,0+i)

        for i in range(7, 7+5):
            self.slider[i] = QSlider()
            self.slider[i].setOrientation(Qt.Vertical)
            # self.slider[i].setTickPosition(QSlider.TicksBelow)
            # self.slider[i].setTickInterval(1)
            self.slider[i].setMinimum(0)
            self.slider[i].setMaximum(360)
            self.gbox_layout.addWidget(self.slider[i],1,0+i)
            self.slider[i].valueChanged[int].connect( self.slider_on_progress)

            self.lbl_x_stick[i] = QLabel("t"+str(i-6))
            self.gbox_layout.addWidget(self.lbl_x_stick[i],2,0+i)

            self.lbl_x[i] = QLabel("0")
            self.gbox_layout.addWidget(self.lbl_x[i],0,0+i)
        # inital geomtery of window
        



        self.chbox_grid = QCheckBox("Add Grid Ruler")
        right_layout.addWidget(self.chbox_grid, 4,0)


        self.chbox_surf_edge = QCheckBox("Mesh edge")
        self.chbox_surf_edge.setChecked(True)
        right_layout.addWidget(self.chbox_surf_edge, 5,0)
        # self.gbox_surf_layout.addWidget(self.chbox_surf_edge, 3,1)

        self.r_btn = QRadioButton("Background")
        self.r_btn.clicked.connect(self.change_background)
        right_layout.addWidget(self.r_btn, 6,0)

        self.r_btn2 = QCheckBox("Reset Camera")
        self.r_btn2.clicked.connect(self.reset_camera)
        right_layout.addWidget(self.r_btn2, 7,0)
    

        self.gbox_surf = QGroupBox(" ")
        self.gbox_surf_layout = QGridLayout()
        self.gbox_surf.setLayout(self.gbox_surf_layout)

        right_layout.addWidget(self.gbox_surf , 8,0)
        self.chbox_surf_point = QCheckBox("Add Surf points")
        self.gbox_surf_layout.addWidget(self.chbox_surf_point, 0,0)

        self.btn_surf_point_color = QPushButton("")
        self.gbox_surf_layout.addWidget(self.btn_surf_point_color, 0,1)
        self.btn_surf_point_color.clicked.connect(self.change_color_surf_point)
        self.btn_surf_point_color.setStyleSheet("background-color:"+self.surf_point_color)
        
        self.chbox_vol = QCheckBox("Add Vol points")
        self.gbox_surf_layout.addWidget(self.chbox_vol, 1,0)

        self.btn_vol_color = QPushButton("")
        self.gbox_surf_layout.addWidget(self.btn_vol_color, 1,1)
        self.btn_vol_color.clicked.connect(self.change_color_vol)
        self.btn_vol_color.setStyleSheet("background-color:"+self.vol_color)

# 
        self.chbox_surf = QCheckBox("Add Surf mesh")
        self.gbox_surf_layout.addWidget(self.chbox_surf, 2,0)

 

        self.btn_surf_color = QPushButton("")
        self.gbox_surf_layout.addWidget(self.btn_surf_color, 2,1)
        self.btn_surf_color.clicked.connect(self.change_color_surf)
        self.btn_surf_color.setStyleSheet("background-color:"+self.surf_color)
        
        self.btn_clear = QPushButton("Clear")
        self.btn_clear.clicked.connect(self.clear_screen)
        right_layout.addWidget(self.btn_clear,9,0)

        self.btn_plt = QPushButton("Plot")
        self.btn_plt.clicked.connect(self.plt_on_screen)
        right_layout.addWidget(self.btn_plt,10,0)

        self.lbl_state = QLabel("State: Plot obj")
        right_layout.addWidget(self.lbl_state,11,0)
      


        # return show_Tab

    def  reset_camera(self):
        """ reseting the camera position in the screen """
        self.plotter.reset_camera()

    def change_background(self):
        """If the radio button is ticked, the screen's bg should be changed """
        if self.r_btn.isChecked():
            self.plotter.set_background("royalblue", top="aliceblue")
        else:
            self.plotter.set_background('black', top='white')

    def change_color_surf(self):
        """if the surface color is selected to be changed this function is done """
        col=QColorDialog.getColor()
        self.surf_color = col.name()
        self.btn_surf_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("Surf color changed to: "+ col.name())

    def change_color_surf_point(self):
        """ The surf point color is determined in this function"""

        col=QColorDialog.getColor()
        self.surf_point_color = col.name()
        self.btn_surf_point_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("Surf points' color changed to: "+ col.name())

    def change_color_vol(self):
        """ sepcifies the vol color """
        
        col=QColorDialog.getColor()
        self.vol_color = col.name()
        self.btn_vol_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("Vol color changed to: "+ col.name())                


        