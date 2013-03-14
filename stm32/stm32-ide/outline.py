
'''

    @filename: outline.py
    @project : PhilRoboKit IDE

    PhilRobotics | Philippine Electronics and Robotics Enthusiasts Club
    http://philrobotics.com | http://philrobotics.com/forum | http://facebook.com/philrobotics
    phirobotics.core@philrobotics.com

    Copyright (C) 2012  Julius Constante

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses

'''

import clang.cindex as clang
from PyQt4 import QtGui, QtCore

class CLangParserThread(QtCore.QThread):
    def __init__(self, parent=None):
        QtCore.QThread.__init__(self, parent)
        self.parent = parent        
        self.queue = False
        self.nodes = []
        self.index = clang.Index.create()
    
    def find_typerefs(self, node, fname=None, level=0):
        if str(node.location.file)==str(fname):
            self.nodes.append((node, level))
        for c in node.get_children():
            self.find_typerefs(c, fname, level+1)
    
    def run(self):
        self.msleep(200)
        del self.nodes[:]
        #print 'started parsing...'
        tu = self.index.parse('a.cpp', [], [('a.cpp', self.parent.getRawContent())], 13)
        self.find_typerefs(tu.cursor, tu.spelling)
        #print 'done parsing.'
        self.msleep(200)
        
        if self.queue:
            self.queue = False
            self.run()
            return
        print 'CLangParserThread done.'
        
    def setQueue(self):
        if self.queue:
            return
        self.queue = True;
        
    def getNodes(self):
        if self.isRunning():
            return []
        return self.nodes
    

class OutLineView(QtGui.QDockWidget):

    def __init__(self, parent=None):
        super(OutLineView, self).__init__("Outline", parent)
        
        self.parser = CLangParserThread(self)
        self.content = ""
        self.updated = True
        
        self.treeWidget = QtGui.QTreeWidget(self)
        self.treeWidget.setHeaderHidden(True)
        self.setWidget(self.treeWidget)
        
        self.startTimer(2000)
        
    def update(self, str_to_parse=""):
        self.updated = False
        self.content = str_to_parse
        if self.parser.isRunning():
            self.parser.setQueue()
            return
        self.parser.start()
        
    def getRawContent(self):
        return str(self.content)
        
    def timerEvent(self, *args, **kwargs):
        if not self.updated:
            previous_parent = None
            previous_item = None
            previous_level = 0
            nodes = self.parser.getNodes()
            if nodes:
                self.treeWidget.clear()
                for node, level in nodes:
                    if node.spelling:
                        label = str(node.spelling)                        
                    elif node.displayname:
                        label = str(node.displayname)
                    else:
                        continue
                    
                    print level, label, node.kind, node.location.line
                    try:
                        if level==1:
                            previous_item = QtGui.QTreeWidgetItem(self.treeWidget, [label])
                            self.treeWidget.addTopLevelItem(previous_item)                        
                        elif previous_level!=level:
                            print 'add child...'
                            previous_parent = previous_item
                            previous_parent.addChild( QtGui.QTreeWidgetItem([label]) )
                        else:
                            print 'add siblings..'
                            previous_parent.addChild( QtGui.QTreeWidgetItem([label]) )
                    except:
                        pass
                    previous_level = level
                        
                self.updated = True
            
        return QtGui.QDockWidget.timerEvent(self, *args, **kwargs)
        
    