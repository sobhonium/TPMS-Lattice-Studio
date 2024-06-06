''' 
   Copyright: ---,
   License:   MIT,
   Author:    SBN,
   Purpose:   Lattice Generators for TPMS structures,
   Version:   2.0,
   Date:      23, 05, 2021,
'''

import pyvista as pv
from genericpath import isfile
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QApplication, QCheckBox, QColorDialog,
                             QFileDialog, QGridLayout, QGroupBox, QHBoxLayout,
                             QLabel, QPushButton, QRadioButton, QSlider,
                             QTabWidget, QTextEdit, QVBoxLayout, QWidget, QPlainTextEdit, QTableView)
from pyvistaqt import QtInteractor
from pathlib import Path

import PyQt5.QtGui
import os

class ComputeTabUI(QTabWidget):
    def __init__(self):
        QTabWidget.__init__(self, parent=None)
        # compute_Tab = QTabWidget()
        self.fileName_source = './plotList.txt'
        main_layout =   QGridLayout()
        dialog_layout = QVBoxLayout()
        report_layout = QVBoxLayout() 
        config_layout = QGridLayout()
        main_layout.addLayout(dialog_layout, 0,0)
        main_layout.addLayout(report_layout, 0,1)
        main_layout.addLayout(config_layout, 1,0)
        self.setLayout(main_layout)

        self.rb1 = QRadioButton("plot a of list surfaces in folder", self)
        self.rb2 = QRadioButton("plot a list of volumes in folder ", self)
        self.rb_surf = QRadioButton("Compute surface points from a given list", self)
        self.rb_vol = QRadioButton("Compute volume points from a given list", self)
        self.rb_surfvol = QRadioButton("Compute surface/volume points from a given list", self)
        
        # rb_vol = QRadioButton("Compute volume points from a given list", self)

        dialog_layout.addWidget(self.rb1,0)
        dialog_layout.addWidget(self.rb2,1)
        dialog_layout.addWidget(self.rb_surf,2)
        dialog_layout.addWidget(self.rb_vol,3)
        dialog_layout.addWidget(self.rb_surfvol,4)
        
 
        self.text_edit_report = QPlainTextEdit()

        report_layout.addWidget(self.text_edit_report,0)
        self.chosen_path = "./"
        
        self.btn_source_file =  QPushButton("Browse")
        self.lbl_source_file =  QLabel("srouce:   ./plotList.txt")
        self.btn_save_folder =  QPushButton("Browse")
        self.lbl_save_folder =  QLabel("save in folder:"+self.chosen_path)
        
        self.btn_source_file.clicked.connect(self.add_file)
        self.btn_save_folder.clicked.connect(self.add_folder)
        
        self.btn_action = QPushButton("Action")
        self.btn_action.clicked.connect(self.do_action)

        self.chbox_reconst_surf = QCheckBox("reconst surf")
        self.chbox_obj = QCheckBox(".obj")

        config_layout.addWidget(self.lbl_source_file, 0,0 )
        config_layout.addWidget(self.btn_source_file, 0,3 )

        config_layout.addWidget(self.lbl_save_folder, 1,0 )
        config_layout.addWidget(self.btn_save_folder, 1,3 )

        config_layout.addWidget(self.chbox_reconst_surf, 2,0 )
        config_layout.addWidget(self.chbox_obj, 2,1 )

        config_layout.addWidget(self.btn_action, 3,3 )
    

        
    def add_file(self):
        fl = QFileDialog()
        fl.setNameFilters(["Text files (*.txt)"])
        self.fileName_source, _ =fl.getOpenFileName()
        self.lbl_source_file.setText("Source file:  " + self.fileName_source)
    
    
    def which_path(self):
        """ This function gives the user a front end tool to choose a path for
              loading 
        """
        self.chosen_path = QFileDialog.getExistingDirectory(self,"Choose Path",
                                                          self.chosen_path) + "/"
        # self.msg_print("path is changed into: " + self.chosen_path )
       

        
    def add_folder(self):
        self.which_path()
        self.lbl_save_folder.setText("Save in folder:  " + self.chosen_path)


    def do_pre_surf_stuff(self):
        self.pad_name_right=''
        self.text_edit_report.appendPlainText("----Surfaces are selected to be computed from list at: " +  self.fileName_source + "-----")
            
        f = open('surf_file.txt', 'w')
        f.write("1")
        f.close()   
    
    def do_pre_vol_stuff(self):
        self.pad_name_right='-vol' 
        self.text_edit_report.appendPlainText("Volumes are selected to be computed from list at: " +  self.fileName_source)
            
        f = open('surf_file.txt', 'w')
        f.write("0")
        f.close() 
       
    def do_action(self):
        if self.rb_surf.isChecked():
            self.do_pre_surf_stuff()
            self.computing()
            
        elif self.rb_vol.isChecked():
            self.do_pre_vol_stuff()
            self.computing()
                
        elif self.rb_surfvol.isChecked():
            self.do_pre_surf_stuff()
            self.computing()
            self.do_pre_vol_stuff()
            self.computing()
           
            
    def computing(self):            
        with open(self.fileName_source, 'r') as f:
                lines = f.readlines()
                # print(lines)
                for line in lines:    
                    x_param = [st for st in line.split(' ')]
                    print("==> ",x_param)

                    # + 
                    # Rather than using a thead, I end up finding out that PyQt5.QtWidgets.qApp.processEvents()
                    # resolves it...! Damn it...
                    ste = [f'{float(elm):.1f}' for elm in x_param[0:15]] # after 15 place there are spaces
                    # print(ste)
                    stor_f_name = '-'.join(ste) + self.pad_name_right + '.xyz'  
                    self.text_edit_report.appendPlainText("==> "+ stor_f_name+' is being added to folder' )
                    import PyQt5.QtWidgets
                    PyQt5.QtWidgets.qApp.processEvents()
                

                    self.writing_parameters_in_file(x_param)
                    self.running_core_program(1)
                
                    self.text_edit_report.appendPlainText("added." )
                    old_name = '../src/demofile.txt'
                    new_name = self.chosen_path+ stor_f_name
                    os.rename(old_name, new_name)

    def writing_parameters_in_file(self,  X_param = 0):
        # self.text_edit_report.append("==> " )
        #f = open("X.txt", "w")
        with open("../src/X.txt", "w") as file_:

            if X_param == 0:
                file_.write(self.param1.get() + " ")    
                file_.write(self.param2.get() + " ")
                file_.write(self.param3.get() + " ")    
                file_.write(self.param4.get() + " ")  
                file_.write(self.param5.get() + " ")    
                file_.write(self.param6.get() + " ")  
            else: # if the parameters are given from list to plot them all, this else 
                # is called   
                for elem in X_param:
                    file_.write(str(elem) + " ")    



    def running_core_program(self, backend_num):
        
        # os.popen("g++ -fno-math-errno  main.cpp  linalgebra.cpp testclass.cpp;  ./a.out `cat X.txt`").read()
        if backend_num == 1:
            os.popen("g++  -fno-math-errno  ../src/main7.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()
        if backend_num == 2:
            os.popen("g++  -fno-math-errno  ../src/main7.cpp  ../include/linalgebra4.cpp ../include/TPMSLattice.cpp -o ../src/a.out;  ../src/a.out `cat ../src/X.txt`").read()
