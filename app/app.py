''' 
   Copyright: ---,
   License:   MIT,
   Author:    SBN,
   Purpose:   Lattice Generators for TPMS structures,
   Version:   2.0,
   Date:      23, 05, 2021,
'''

import sys


from PyQt5.QtWidgets import (QApplication, QCheckBox, QColorDialog,
                             QFileDialog, QGridLayout, QGroupBox, QHBoxLayout,
                             QLabel, QMainWindow, QMenu, QMenuBar, QPushButton,
                             QRadioButton, QSlider, QTabWidget, QTextEdit,
                             QVBoxLayout, QWidget)


from computetab import ComputeTabUI
from plottab import PlotTabUI
from showtab import ShowTabUI

# TODO(Sobhan): - use pathlib package for link addressing and get rid of string
#                addresses
#               - add a checkbox for lighting=True or False for surf plot.   
#               - shutils for running c++ backend  in python code 
#               - loguru insteading printing logs





    

class CentralWindow(QWidget):
    """Creates the central window of the window application

    Args:
        QWidget : (superclass): all the plottings are put in an obj of this class 
    """

   
    
    def __init__(self, parent):
        super(CentralWindow, self).__init__(parent)

        self.setWindowTitle("QTabWidget Example")
        
        # Create a top-level layout
        layout = QGridLayout()
        self.setLayout(layout)

        # Create the tab widget with some tabs
        tabs = QTabWidget()
        # tabs.addTab(self.show_tab_UI(), "Show")
        

        self.show_tab=ShowTabUI()
        tabs.addTab(self.show_tab, "Show")
        
        self.comput_tab=ComputeTabUI()
        tabs.addTab(self.comput_tab, "Compute")

        
        self.plot_tab=PlotTabUI()
        tabs.addTab(self.plot_tab, "Plot")
        

        layout.addWidget(tabs)
        



class MainWindow(QMainWindow):
    '''
    This class used only just for providing the features of menues or other stuff
    which are cool. QMainwindow by its nature uses centralwidget, dockwidget, menubarwidget
    toolbar, statusbar which are pretty nice for development of a window
    '''
    
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        
        _widget =  QWidget()
        _layout =  QVBoxLayout(_widget)
        self.setLayout(_layout)
        menuBar = QMenuBar()
        _layout.addWidget(menuBar)

        fileMenu = QMenu("&File", self)
        menuBar.addMenu(fileMenu)
        # Creating menus using a title
        editMenu = menuBar.addMenu("&Edit")
        insertMenu = menuBar.addMenu("&Insert")
        helpMenu = menuBar.addMenu("&Help")
        
        self.central_widget = CentralWindow(self)
        
        self.setCentralWidget(_widget)
        self.resize(800, 600)
        _layout.addWidget(self.central_widget)
        
                
        

  


if __name__ == "__main__":

    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())



