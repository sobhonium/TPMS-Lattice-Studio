import os
from pathlib import Path
from subprocess import list2cmdline

import PyQt5.QtGui
import pyvista as pv
from genericpath import isfile
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import (QAbstractScrollArea, QApplication, QCheckBox,
                             QColorDialog, QDoubleSpinBox, QFileDialog,
                             QGridLayout, QGroupBox, QHBoxLayout, QLabel,
                             QListWidget, QListWidgetItem, QPlainTextEdit,
                             QPushButton, QRadioButton, QScrollArea, QSlider,
                             QSpinBox, QTableView, QTableWidget,
                             QTableWidgetItem, QTabWidget, QTextEdit,
                             QVBoxLayout, QWidget)
from pyvistaqt import QtInteractor

#TODO(SBN): Some of the methods in class should be changed into @staticmethods or 
#           @classmethods
#           descriptors for some of the classes 
#           some are needed to be decorated
#           use itertools for some operations.
#           use pathlib for cross platform purposes
#           instead of conda env export, simply use requirements.txt

#FIXIT(SBN): in main7.cpp main8.cpp and main10.cpp low_x and other coordinates must be updated... 


class   PlotTabUI(QTabWidget):
    def __init__(self):
        QTabWidget.__init__(self, parent=None)

        self.setStyleSheet('''
    
    
                        QGroupBox {
                            border: 2px solid gray;
                            border-radius: 5px;
                            margin-top: 1ex; /* leave space at the top for the title */
                        }
                            '''
                           )
        self.add_component()
        
    def add_component(self):    
        
        self.coloring_btn_stylesheet = '''QPushButton{
                                                    background-color: white;
                                                    border-style: outset;
                                                    border-radius: 10px;
                                                    border-color: beige;
                                                    min-width: 1em;
                                                    padding: 1px;}'''
        
        main_layout  = QHBoxLayout()
        right_layout = QGridLayout()
        left_layout  = QGridLayout()
        
        self.color_bg_counter = 0    
        # show_Tab = QWidget()
        self.setLayout(main_layout)
        
        # left is twice large as right
        main_layout.addLayout(left_layout, 3)
        main_layout.addLayout(right_layout)



        self.plotter = QtInteractor(self)
        self.lbl_msg = QLabel("msg: ")
        left_layout  . addWidget(self.lbl_msg, 1,0)
        left_layout.addWidget(self.plotter.interactor, 0,0)
    

        import numpy as np
        p1 = np.loadtxt(Path('p1.txt'))
        p2 = np.loadtxt(Path('p2.txt'))
      
        self.line_width_sklt1 = 3
        self.line_width_sklt2 = 3
              
        points1 = p1
        points2 = p2 

        lines1 =  [[2, i,i+1] for i in range(0, len(p1), 2)]
        lines2 =  [[2, i,i+1] for i in range(0, len(p2), 2)]

        pdata1 = pv.PolyData(points1)
        pdata1.lines = lines1


        pdata2 = pv.PolyData(points2)
        pdata2.lines = lines2
        
        self.surf_skelt1_color = 'white'
        self.surf_skelt2_color = 'grey'
        self.surf_color = 'white'
        self.vol_color = 'grey'
        self.import_color = 'grey'
        
        self.plotter.add_mesh(pdata1, color=self.surf_skelt1_color,line_width=self.line_width_sklt1) 
        self.plotter.add_mesh(pdata2, color=self.surf_skelt2_color,line_width=self.line_width_sklt2) 

# General right groupbox
        self.gen_rit_gbox = QGroupBox()
        gen_rit_gbox_layout = QVBoxLayout()
        self.gen_rit_gbox.setLayout(gen_rit_gbox_layout)
        right_layout.addWidget(self.gen_rit_gbox)


        self.scroll1 = QScrollArea()           
        self.scroll1.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.scroll1.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.scroll1.setWidgetResizable(True)
        
        self.scroll1.setWidget(self.gen_rit_gbox)
        main_layout.addWidget(self.scroll1) 




        self.gbox = QGroupBox ("X and Theta parameters")
        self.gbox_layout = QGridLayout()
        self.gbox.setLayout(self.gbox_layout)

        gen_rit_gbox_layout.addWidget(self.gbox)
        

        # checks if any of params changes
        self.slider = [1,1,1,1,1,1,1, 120, 120, 120, 120, 120] 
        self.lbl_x_stick = [1,1,1,1,1,1,1, 120, 120, 120, 120, 120] 
        self.lbl_x = [1,1,1,1,1,1,1, 120, 120, 120, 120, 120] 
        for i in range(1, 6+1):
            self.slider[i] = QSlider()
            self.slider[i].setOrientation(Qt.Horizontal)
            self.slider[i].setTickPosition(QSlider.TicksBelow)
            self.slider[i].setTickInterval(1)
            self.slider[i].setMinimum(0)
            self.slider[i].setValue(1)
            self.slider[i].setMaximum(4)
            self.gbox_layout.addWidget(self.slider[i],0+i, 1)
            self.slider[i].valueChanged[int].connect( self.slider_on_progress)

            self.lbl_x_stick[i] = QLabel("x"+str(i))
            self.gbox_layout.addWidget(self.lbl_x_stick[i],0+i, 0)

            self.lbl_x[i] = QLabel("1")
            self.gbox_layout.addWidget(self.lbl_x[i],0+i, 2)

        for i in range(7, 7+5):
            self.slider[i] = QSlider()
            self.slider[i].setOrientation(Qt.Horizontal)
            # self.slider[i].setTickPosition(QSlider.TicksBelow)
            # self.slider[i].setTickInterval(1)
            self.slider[i].setMinimum(0)
            self.slider[i].setMaximum(360)
            self.slider[i].setValue(120)
            self.gbox_layout.addWidget(self.slider[i],0+i, 1)
            self.slider[i].valueChanged[int].connect( self.slider_on_progress)

            self.lbl_x_stick[i] = QLabel("t"+str(i-6))
            self.gbox_layout.addWidget(self.lbl_x_stick[i],0+i,0)

            self.lbl_x[i] = QLabel("0")
            self.gbox_layout.addWidget(self.lbl_x[i],0+i, 2)
            self.setWindowTitle("Testing Parameters Studio")
            # self.show()

# Box gruop
        self.gbox1 = QGroupBox("Skeleton")
        self.gbox1_layout = QGridLayout()
        self.gbox1.setLayout(self.gbox1_layout)
        
        #for making gbox1 checkable 
        # self.gbox1.setCheckable(True)
        # self.gbox1.setChecked(False)
      
        

        gen_rit_gbox_layout.addWidget(self.gbox1)
        


        self.chbox_skeleton = QCheckBox("Skeleton")
        self.gbox1_layout.addWidget(self.chbox_skeleton, 0,0)

        self.btn_sklt1_point_color = QPushButton("Skt 1")
        self.gbox1_layout.addWidget(self.btn_sklt1_point_color, 0,1)
        self.btn_sklt1_point_color.clicked.connect(self.change_color_skt1_point)
        self.btn_sklt1_point_color.setStyleSheet("background-color:"+self.surf_skelt1_color)
        # self.btn_sklt1_point_color.setStyleSheet(self.coloring_btn_stylesheet) 
      
        self.btn_sklt2_point_color = QPushButton("Skt 2")
        self.gbox1_layout.addWidget(self.btn_sklt2_point_color, 0,2)
        self.btn_sklt2_point_color.clicked.connect(self.change_color_skt2_point)
        self.btn_sklt2_point_color.setStyleSheet("background-color:"+self.surf_skelt2_color)


        self.spin_sklt_dim = QSpinBox(self)
        self.gbox1_layout.addWidget(self.spin_sklt_dim, 0, 3)
        self.spin_sklt_dim.setRange(1, 5)
        self.spin_sklt_dim.setValue(1)
        self.spin_sklt_dim.setPrefix('dim: ')
        
        self.spin_sklt_line_with = QSpinBox(self)
        self.gbox1_layout.addWidget(self.spin_sklt_line_with, 0, 4)
        self.spin_sklt_line_with.setRange(1, 15)
        self.spin_sklt_line_with.setValue(3)
        self.spin_sklt_line_with.setPrefix('tck: ')
    
# Box group
        self.gbox2 = QGroupBox("Surf/Volm")
        self.gbox2_layout = QGridLayout()
        self.gbox2.setLayout(self.gbox2_layout)
        gen_rit_gbox_layout.addWidget(self.gbox2)

        self.chbox_surface_comp = QCheckBox("Surf")
        self.gbox2_layout.addWidget(self.chbox_surface_comp, 0,0)
        self.btn_surf_point_color = QPushButton("")
        self.gbox2_layout.addWidget(self.btn_surf_point_color, 0,1)
        self.btn_surf_point_color.clicked.connect(self.change_color_surf_point)
        self.btn_surf_point_color.setStyleSheet("background-color:"+self.surf_color)
        
        self.spin_surf_dim1 = QSpinBox(self)
        self.gbox2_layout.addWidget(self.spin_surf_dim1, 0, 2)
        self.spin_surf_dim1.setRange(1, 5)
        self.spin_surf_dim1.setValue(1)
        self.spin_surf_dim1.setPrefix('d1: ')
        
        self.spin_surf_dim2 = QSpinBox(self)
        self.gbox2_layout.addWidget(self.spin_surf_dim2, 0, 3)
        self.spin_surf_dim2.setRange(1, 5)
        self.spin_surf_dim2.setValue(1)
        self.spin_surf_dim2.setPrefix('d2: ')
        
        self.spin_surf_dim3 = QSpinBox(self)
        self.gbox2_layout.addWidget(self.spin_surf_dim3, 0, 4)
        self.spin_surf_dim3.setRange(1, 5)
        self.spin_surf_dim3.setValue(1)
        self.spin_surf_dim3.setPrefix('d3: ')
        
        self.percion_surf_spin = QDoubleSpinBox(self)
        self.gbox2_layout.addWidget(self.percion_surf_spin, 0, 5)
        self.percion_surf_spin.setRange(0, 1)
        self.percion_surf_spin.setValue(0.01)
        self.percion_surf_spin.setDecimals(3)
        self.percion_surf_spin.setSingleStep(0.001)
        self.percion_surf_spin.setPrefix('per: ')
        

        self.chbox_vol_comp = QCheckBox("Vol")
        self.gbox2_layout.addWidget(self.chbox_vol_comp, 1,0)
        self.btn_vol_point_color = QPushButton("")
        self.gbox2_layout.addWidget(self.btn_vol_point_color, 1,1)
        self.btn_vol_point_color.clicked.connect(self.change_color_vol_point)
        self.btn_vol_point_color.setStyleSheet("background-color:"+self.vol_color)
        
        self.spin_vol_dim1 = QSpinBox(self)
        self.gbox2_layout.addWidget(self.spin_vol_dim1, 1, 2)
        self.spin_vol_dim1.setRange(1, 5)
        self.spin_vol_dim1.setValue(1)
        self.spin_vol_dim1.setPrefix('d1: ')
        
        self.spin_vol_dim2 = QSpinBox(self)
        self.gbox2_layout.addWidget(self.spin_vol_dim2, 1, 3)
        self.spin_vol_dim2.setRange(1, 5)
        self.spin_vol_dim2.setValue(1)
        self.spin_vol_dim2.setPrefix('d2: ')
        
        self.spin_vol_dim3 = QSpinBox(self)
        self.gbox2_layout.addWidget(self.spin_vol_dim3, 1, 4)
        self.spin_vol_dim3.setRange(1, 5)
        self.spin_vol_dim3.setValue(1)
        self.spin_vol_dim3.setPrefix('d3: ')
                
        self.percion_vol_spin = QDoubleSpinBox(self)
        self.gbox2_layout.addWidget(self.percion_vol_spin, 1, 5)
        self.percion_vol_spin.setRange(0, 1)        
        self.percion_vol_spin.setValue(0.01)
        self.percion_vol_spin.setDecimals(3)
        self.percion_vol_spin.setSingleStep(0.001)
        self.percion_vol_spin.setPrefix('per: ')

        self.gboxrdbtn = QGroupBox("Through?")
        self.gboxrdbtn_layout = QVBoxLayout()
        self.gboxrdbtn.setLayout(self.gboxrdbtn_layout)
        gen_rit_gbox_layout.addWidget(self.gboxrdbtn)
        # self.gbox2_layout.addWidget.addWidget(self.gboxrdbtn)
        
        self.through_generator_cordin_rbtn = QRadioButton(" Through Generator Axis ")
        self.through_generator_cordin_rbtn.setChecked(True)
        self.gboxrdbtn_layout.addWidget(self.through_generator_cordin_rbtn)
        
        self.through_stand_cordin_rbtn = QRadioButton(" Through Standard Axis ")
        self.through_stand_cordin_rbtn.setChecked(False)
        self.gboxrdbtn_layout.addWidget(self.through_stand_cordin_rbtn)

        self.btn_gbox = QGroupBox("Action")
        btngbox_layout = QGridLayout()
        self.btn_gbox.setLayout(btngbox_layout)
        gen_rit_gbox_layout.addWidget(self.btn_gbox)
        
        self.plt_btn = QPushButton("Plot")
        btngbox_layout.addWidget(self.plt_btn, 0,0)
        self.plt_btn.clicked.connect(self.plt_on_screen)

        

        self.cln_btn = QPushButton("Clear")
        btngbox_layout.addWidget(self.cln_btn, 0,1)
        self.cln_btn.clicked.connect(self.clear_screen)

        self.btn_plt_imp = QPushButton("Import")
        self.btn_plt_imp.clicked.connect(self.plt_import)
        btngbox_layout.addWidget(self.btn_plt_imp,0,2)
            
        self.btn_impot_color = QPushButton("impt")
        btngbox_layout.addWidget(self.btn_impot_color, 0,3)
        self.btn_impot_color.clicked.connect(self.change_color_import)
        self.btn_impot_color.setStyleSheet("background-color:"+self.import_color)
            
        
        
        self.btn_grid = QCheckBox("Add Grid")
        gen_rit_gbox_layout.addWidget(self.btn_grid)
        self.btn_grid.clicked.connect(self.add_grid)
        
        self.btn_bound = QCheckBox("Add Bound")
        gen_rit_gbox_layout.addWidget(self.btn_bound)
        self.btn_bound.clicked.connect(self.add_bound)
        
        self.btn_resetCam = QCheckBox("Reset Camera")
        gen_rit_gbox_layout.addWidget(self.btn_resetCam)
        self.btn_resetCam.clicked.connect(self.reset_camera)

        self.btn_bg = QCheckBox("Background")
        self.btn_bg.clicked.connect(self.change_background)
        gen_rit_gbox_layout.addWidget(self.btn_bg)

        self.genertor_cbox = QCheckBox("Generators Axis")
        self.genertor_cbox.clicked.connect(self.add_generator)
        gen_rit_gbox_layout.addWidget(self.genertor_cbox)

        self.stand_cbox = QCheckBox("x,y,z Axis")
        self.stand_cbox.clicked.connect(self.add_standard_axis)
        gen_rit_gbox_layout.addWidget(self.stand_cbox)

        self.cylinder_cbox = QCheckBox("cylinder volume")
        self.cylinder_cbox.clicked.connect(self.add_cylinder)
        gen_rit_gbox_layout.addWidget(self.cylinder_cbox)

       
        self.layer_gbox = QGroupBox ("Layers")
        self.layer_gbox_layout = QGridLayout()
        self.layer_gbox.setLayout(self.layer_gbox_layout)
        gen_rit_gbox_layout.addWidget(self.layer_gbox)
        
        self.lyr_chbox=[0 for _ in range(50)]
        self.lyr_list_count = 0
        self.db = []
        
        self.btn_refresh = QPushButton("Refresh")
        self.layer_gbox_layout.addWidget(self.btn_refresh)
        self.btn_refresh.clicked.connect(self.refresh_screen)

        # self.listWidget = QListWidget()
        # layer_gbox_layout.addWidget(self.listWidget)
        # self.listitems = {"item0": (True,'/'), 
        #                   "item1": (False, '/')}
        # # self.listitems = []
        # for  item, (check, addr) in self.listitems.items():
        #     qitem = QListWidgetItem()
        #     qitem.setText(item)
        #     qitem.setCheckState(Qt.Checked if check else Qt.Unchecked)
        #     self.listWidget.addItem(qitem)
        #     print(item, check, addr)
        
        self.btn_renderxy = QPushButton("Render xy")
        gen_rit_gbox_layout.addWidget(self.btn_renderxy)        
        self.btn_renderxy.clicked.connect(lambda: self.rendering(0))
        
        self.btn_renderxz = QPushButton("Render xz")
        gen_rit_gbox_layout.addWidget(self.btn_renderxz)        
        self.btn_renderxz.clicked.connect(lambda: self.rendering(1))
        
        self.btn_renderyz = QPushButton("Render yz")
        gen_rit_gbox_layout.addWidget(self.btn_renderyz)        
        self.btn_renderyz.clicked.connect(lambda: self.rendering(2))
        
    def rendering(self, direction):
        self.plotter.set_background("black")
        import numpy as np
        import math
        g = np.loadtxt('../src/generators.txt')
        
        
        cent = np.array([0,0,0])
        m = 5
        
        tolx= math.sqrt(g[0][0]**2+ g[0][1]**2 + g[0][2]**2)
        toly= math.sqrt(g[1][0]**2+ g[1][1]**2 + g[1][2]**2)
        tolz= math.sqrt(g[2][0]**2+ g[2][1]**2 + g[2][2]**2)
        
        deltaz = (tolz)/m
        for i in range(m+1):
            print(i*deltaz)
            
             
        print('g0= ',g[0], ' ', g[0][0])
        # for i1 in range(g[0][0]):
        # self.plotter.add_arrows(cent, g[0])
        # self.plotter.add_arrows(cent, g[1])
        # self.plotter.add_arrows(cent, g[2])
        # self.plotter.view_vector(g[0])
        # self.plotter.set_position([3, 0, 0])
        
        # if direction == 0:
        #     self.plotter.view_xy()
        #     self.plotter.screenshot('xy.png')

        # if direction == 2:
        #     self.plotter.view_yz()
        #     self.plotter.screenshot('yz.png')
        
        # if direction == 1:
        #     self.plotter.view_xz()
        #     self.plotter.screenshot('xz.png')
        
        
        # if direction == 0:
        #     self.plotter.view_vector(g[0])
        #     self.plotter.screenshot('g1.png')

        # if direction == 2:
        #     self.plotter.view_vector(g[1])
        #     self.plotter.screenshot('g2.png')
        
        # if direction == 1:
        #     self.plotter.view_vector(g[2])
        #     self.plotter.screenshot('g3.png')
          
    def update_itemlist(self): 
        self.listWidget.clear()
        for  item, (check,addr) in self.listitems.items():
            qitem = QListWidgetItem()
            qitem.setText(item)
            # qitem.setBackgroundColor(self.surf_color)
            qitem.setTextColor (QColor("red"))
            qitem.setCheckState(Qt.Checked if check else Qt.Unchecked)
            self.listWidget.addItem(qitem)
            print(item, check, addr)


    def plt_import(self):
        filename, _ = QFileDialog.getOpenFileName(self, "Open file", "", "Text files (*.txt *.xyz)")
        # file = open(filename,'r')
        # with open(filename, 'r') as f:

    
        import numpy as np

        try:
                spoints = np.loadtxt(filename)
                pdata = pv.PolyData(spoints)     
                self.plotter.add_mesh(pdata, color=self.import_color)
                              
                self.msg_print(filename + ' is added.')
                
                self.updatelayter(filename, self.import_color, pdata) 
                # filename_to_show_inlist   = filename[filename.rfind('/')+1::] 
                # self.listitems[filename_to_show_inlist]= (True, filename)
                # print(self.listitems)
                # self.update_itemlist()
                self.cln_btn.setEnabled(True)

        except:
            print('excrption happened')
           
   
    def updatelayter(self, filename, color, data):
        
        self.db += [(filename, data, color)]
        print((filename, data, color))
        
        self.lyr_chbox[self.lyr_list_count]=QCheckBox(filename)
        self.lyr_chbox[self.lyr_list_count].setCheckState(2)
        self.lyr_chbox[self.lyr_list_count].setStyleSheet("QCheckBox { color:"+color+ "}");
        self.layer_gbox_layout.addWidget(self.lyr_chbox[self.lyr_list_count])
        # self.lyr_chbox[self.lyr_list_count].stateChanged.connect(self.checking_check_layer)
        self.lyr_list_count += 1
        
###     
    def refresh_screen(self):
        if self.lyr_list_count == 0:
            return 
        print(self.db[0][0])
        self.clear_screen()
        # self.plotter.add_mesh(self.db.data, color=self.import_color)
        for i in range(self.lyr_list_count):
            chb = self.lyr_chbox[i]
            if chb.isChecked():
                self.plotter.add_mesh(self.db[i][1], color=self.db[i][2])
            # print(chb.isChecked())
            # if chb == 2:
            #     print(self.lyr_chbox[self.lyr_list_count] )
            
    

    
        
 

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
        self.cln_btn.setEnabled(False)
        self.msg_print("Screen Cleaned.")  
    

        
    
    def slider_on_progress(self):
        """if any of the sliders are changed this function is called 
            automatically"""
        values =""
        # there are 6 parameters to look at
        for i in range(1, 7+5):
            # print(self.slider[i].value())
            values= values + " " + str(self.slider[i].value())
            self.lbl_x[i].setText(str(self.slider[i].value()))
        self.msg_print("parameters are changed to:" + values)

    def change_color_skt1_point(self):
        """ The sk1 color is determined in this function"""
        col=QColorDialog.getColor()
        self.surf_skelt1_color =col.name()
        self.btn_sklt1_point_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("skl1 color changed to: "+ col.name())
        
    def change_color_import(self):
        col=QColorDialog.getColor()
        self.import_color =col.name()
        self.btn_impot_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("import color changed to: "+ col.name())
        
        
        
    def change_color_surf_point(self):
        """ The surf color is determined in this function"""

        col=QColorDialog.getColor()
        self.surf_color =col.name()
        self.btn_surf_point_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("surf color changed to: "+ col.name())

    def change_color_vol_point(self):
        """ The vol color is determined in this function"""

        col=QColorDialog.getColor()
        self.vol_color =col.name()
        self.btn_vol_point_color.setStyleSheet("background-color:" +col.name())
        self.msg_print("vol color changed to: "+ col.name())

    def change_color_skt2_point(self):
        """ The skt2 color is determined in this function"""

        col=QColorDialog.getColor()
        self.surf_skelt2_color =col.name()
        self.btn_sklt2_point_color.setStyleSheet("background-color:" +col.name())
       
        self.msg_print("skt2 color changed to: "+ col.name())
    
    def change_background(self):
        """If the radio button is ticked, the screen's bg should be changed """
        color_list = ["black", "blue", "red", "green", "royalblue", "white", "aliceblue"]
        # if self.r_btn.isChecked():
        self.color_bg_counter +=1
        colr = color_list[self.color_bg_counter%len(color_list)]
        self.plotter.set_background(colr)
        # else:
        # self.plotter.set_background(colr, top='white')
    def add_bound(self):
        if self.btn_bound.isChecked():
            self.plotter.show_bounds(grid='front',location='outer',all_edges=True, color='black')
        else:
            self.plotter.show_bounds(show_xaxis=False, show_yaxis=False, show_zaxis=False,grid=False)
            self.plotter.remove_bounding_box()     
    
    
    def add_grid(self):
        if self.btn_grid.isChecked():
            self.plotter.show_grid(color='black')
        else:
            self.plotter.show_grid(grid=False) 
            # self.plotter.show_bounds(show_xaxis=False, show_yaxis=False, show_zaxis=False,grid=False)   
    
    def reset_camera(self):
        self.btn_resetCam.setCheckState(False)
        self.plotter.reset_camera()        
    
    def add_generator(self):
        import numpy as np
        g = np.loadtxt('../src/generators.txt')
    
        cent = np.array([0,0,0])
        self.plotter.add_arrows(cent, g[0])
        self.plotter.add_arrows(cent, g[1])
        self.plotter.add_arrows(cent, g[2])
        
    def add_standard_axis(self):
        import numpy as np
        points = np.array([[-3,0,0], [3,0,0], [0,-3,0], [0, 3,0], [0,0,-3], [0,0,3]])
        
        x_axix= [2, 0, 1]
        y_axix= [2, 0, 1]
        z_axix= [2, 0, 1]


        pdata1 = pv.PolyData(points[0:2])
        pdata1.lines = x_axix
        self.plotter.add_mesh(pdata1, color='red') 
        
        pdata2 = pv.PolyData(points[2:4])
        pdata2.lines = y_axix
        self.plotter.add_mesh(pdata2, color='green')
        
        pdata3 = pv.PolyData(points[4:6])
        pdata3.lines = z_axix
        self.plotter.add_mesh(pdata3, color='blue') 
         
        
        
    def add_cylinder(self):
        import numpy as np
        p1 = np.loadtxt(Path('p1.txt'))
        # p2 = np.loadtxt('p2.txt')


        from math import sqrt
        for i in range(0, len(p1), 2):
            center_= ((p1[i][0]+p1[i+1][0])/2, (p1[i][1]+p1[i+1][1])/2, (p1[i][2]+p1[i+1][2])/2)
            direction_= (p1[i][0]-p1[i+1][0], p1[i][1]-p1[i+1][1], p1[i][2]-p1[i+1][2])
            height_ = sqrt((p1[i][0]-p1[i+1][0])**2+ (p1[i][1]-p1[i+1][1])**2+ (p1[i][2]-p1[i+1][2])**2)
            cy1 = pv.Cylinder( center=center_, direction=direction_, radius=0.15, height=height_, resolution=50, capping=True)
            
            sphere1 = pv.Sphere(radius=0.15, center=p1[i])
            sphere2 = pv.Sphere(radius=0.15, center=p1[i+1])

            self.plotter.add_mesh(cy1, color= self.surf_skelt1_color)
            self.plotter.add_mesh(sphere1, color= self.surf_skelt1_color)
            self.plotter.add_mesh(sphere2, color= self.surf_skelt1_color)

    def plt_on_screen(self):
        """ If the plot button is clicked, this function looks at the checkbox
           buttons and dispalys the customized output """
        # first off clean the screen
        # self.clear_screen()
        
        self.msg_print('Skeleton Computing...')
        
        list_param = [str(self.slider[i].value()) for i in range(1, 7 + 5)]
        patt = '-'.join(list_param)
        



        if self.chbox_skeleton.isChecked():
            
            self.msg_print('Skeleton Computing...')
            import PyQt5.QtWidgets
            PyQt5.QtWidgets.qApp.processEvents()
            
            
            with open("../src/X.txt", "w") as file_:
                list_param = [str(self.slider[i].value()) for i in range(1, 7+5)]
                print(list_param)
                # print('len o list_param =', len(list_param))
                # 60/360 = ?/2pi
                for i in range(6, 7+5-1):
                    # print(i, type( list_param[i]))
                    list_param[i] = str(float(list_param[i])*2* 3.14159265359/360)
                parm_all = ' '.join(list_param)
                print(parm_all)
                file_.write(parm_all)
                # print('spin val==',self.spin_sklt_dim.value())  
                            
                dim=str(self.spin_sklt_dim.value())
                self.msg_print('Dimension chaned to ' + dim)  
                file_.write(" " + dim)    
            # this main function just computes skeletals points in p1.txt and p2.txt
            print('main8.cpp running') 
            os.popen("g++  -fno-math-errno  ../src/main8.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()


            self.line_width_sklt1 = self.spin_sklt_line_with.value()
            self.line_width_sklt2 = self.spin_sklt_line_with.value()
            
            import numpy as np
            p1 = np.loadtxt('p1.txt')
            p2 = np.loadtxt('p2.txt')
            
            # print(p1)
            # lines = [p1[0], p1[1], p1[0], p1[2], p1[0], p1[3]]
            # lines = [[0, 1], [0, 2], [0, 3], [1, 4], [2, 5], [3, 6]]
            # line2 = [[0, 1], [0, 2], [0, 3], [1, 4], [2, 5], [3, 6]]

            
            points1 = p1
            points2 = p2 
            print('number of points: ', len(p1))
            lines1 =  [[2, i,i+1] for i in range(0, len(p1), 2)]
            lines2 =  [[2, i,i+1] for i in range(0, len(p2), 2)]

            pdata1 = pv.PolyData(points1)
            pdata1.lines = lines1


            pdata2 = pv.PolyData(points2)
            pdata2.lines = lines2
            
            self.plotter.add_mesh(pdata1, color=self.surf_skelt1_color,  line_width=self.line_width_sklt1) 
            self.plotter.add_mesh(pdata2, color=self.surf_skelt2_color,   line_width=self.line_width_sklt2)
            
            s= "[" + ' '.join(list_param[:6])+"],["
        
            # i/2*pi = ?/360
            for i in list_param[6::]:
                s+=  str(int(float(i)/3.1415*180))+"-"
            s+="]"    
           
                
            self.updatelayter("Skt1: "+ s, self.surf_skelt1_color , pdata1) 
            self.updatelayter("Skt2: " +s, self.surf_skelt2_color , pdata2) 
            
            self.cln_btn.setEnabled(True)
            self.msg_print('Skeletal Computation Done!')

        if self.chbox_surface_comp.isChecked():
            
            self.msg_print('Surface point Computing...')
            import PyQt5.QtWidgets
            PyQt5.QtWidgets.qApp.processEvents()
            
            
            f = open('surf_file.txt', 'w')
            f.write("1")
            f.close()    
            
            with open("../src/X.txt", "w") as file_:
                list_param = [str(self.slider[i].value()) for i in range(1, 7+5)]
                print(list_param)
                # print('len o list_param =', len(list_param))
                # 60/360 = ?/2pi
                for i in range(6, 7+5-1):
                    # print(i, type( list_param[i]))
                    list_param[i] = str(float(list_param[i])*2* 3.14159265359/360)
                parm_all = ' '.join(list_param)
                print(parm_all)
                file_.write(parm_all)
                # print('spin val==',self.spin_sklt_dim.value())  
                            
                dim1=str(self.spin_surf_dim1.value())
                dim2=str(self.spin_surf_dim2.value())
                dim3=str(self.spin_surf_dim3.value())
                
                self.msg_print('Dimension chaned to: ' + dim1+ ' '+ dim2+ ' ' + dim3)  
                file_.write(" " + dim1 + " " + dim2 + " " + dim3)
           
                file_.write(" "+str(self.percion_surf_spin.value()))
             

            # os.popen("g++  -fno-math-errno  main10.cpp  linalgebra4.cpp testclass4__.cpp;  ./a.out `cat X.txt`").read()
            if (self.through_generator_cordin_rbtn.isChecked()):
                os.popen("g++  -fno-math-errno  ../src/main10.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()
            else:# through standad cordinate is checked.
                os.popen("g++  -fno-math-errno  ../src/main12.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()     

            import numpy as np
            points = np.loadtxt('../src/demofile.txt')
            pdata = pv.PolyData(points)
            try: 
                self.plotter.add_mesh(pdata, color=self.surf_color)
                              
                s= "[" + ' '.join(list_param[:6])+"],["
            
                # i/2*pi = ?/360
                for i in list_param[6::]:
                    s+=  str(int(float(i)/3.1415*180))+"-"
                s+="]"    
                self.updatelayter("Surf: "+s, self.surf_color , pdata) 
                
                
                self.msg_print('Surface Computation Done.')
                print(list_param)
            except:    
                self.msg_print('No point to plot. percion is big! Try small one.')

        if self.chbox_vol_comp.isChecked():
            
            self.msg_print('Point Volume Computing...')
            import PyQt5.QtWidgets
            PyQt5.QtWidgets.qApp.processEvents()
            self.msg_print('Point Volume Computing...')

            f = open('surf_file.txt', 'w')
            f.write("0")
            f.close()    
            
            with open("../src/X.txt", "w") as file_:
                list_param = [str(self.slider[i].value()) for i in range(1, 7+5)]
                print(list_param)
                # print('len o list_param =', len(list_param))
                # 60/360 = ?/2pi
                for i in range(6, 7+5-1):
                    # print(i, type( list_param[i]))
                    list_param[i] = str(float(list_param[i])*2* 3.14159265359/360)
                parm_all = ' '.join(list_param)
                print(parm_all)
                file_.write(parm_all)
                
                            
                dim1=str(self.spin_vol_dim1.value())
                dim2=str(self.spin_vol_dim2.value())
                dim3=str(self.spin_vol_dim3.value())
                self.msg_print("Dimension chaned to: " + dim1+" "+dim2+" "+dim3+" ")  
                file_.write(" "+ dim1+" "+dim2+" "+dim3)
           
                file_.write(" "+str(self.percion_vol_spin.value()))

            # os.popen("g++  -fno-math-errno  main9.cpp  linalgebra4.cpp testclass4__.cpp;  ./a.out `cat X.txt`").read()
            if (self.through_generator_cordin_rbtn.isChecked()):
                os.popen("g++  -fno-math-errno  ../src/main10.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()
            else:
                os.popen("g++  -fno-math-errno  ../src/main12.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()     
            


            import numpy as np
            points = np.loadtxt('../src/demofile.txt')
            pdata = pv.PolyData(points)
            # try: 
            self.plotter.add_mesh(pdata, color=self.vol_color)
            
            # surf = self.plotter.delaunay_2d()
            # surf.plot(show_edges=True)

            s= "[" + ' '.join(list_param[:6])+"],["
        
            # i/2*pi = ?/360
            for i in list_param[6::]:
                s+=  str(int(float(i)/3.1415*180))+"-"
            s+="]"    
            self.updatelayter("Vol: "+s, self.vol_color , pdata) 
            
            self.msg_print('Point volume Computation Done.')
            # except:    
                # self.msg_print('No point to plot. percion is big! Try small one.')

