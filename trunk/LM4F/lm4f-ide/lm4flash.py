
'''

    @filename: lm4flash.py
    @project : LM4F-GCC-ARM-IDE

    PhilRobotics | Philippine Electronics and Robotics Enthusiasts Club
    http://philrobotics.com | http://philrobotics.com/forum | http://facebook.com/philrobotics
    phirobotics.core@philrobotics.com

    Copyright (C) 2013  Julius Constante

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

import os, subprocess
from PyQt4 import QtCore

DEFAULT_LM4FLASH_WIN32 = 'tools\\lm4tools\\lm4flash.exe'
DEFAULT_LM4FLASH_LINUX = 'tools/lm4tools/lm4flash'

class LM4FlashThread(QtCore.QThread):
    def __init__(self, parent=None):
        QtCore.QThread.__init__(self, parent)
        self.parent = parent
        
        self.BinFile = ''
        self.LogList = QtCore.QStringList()
        self.LoaderProcess = None # todo: use QtCore.QProcess class instead
        
    def run(self):
        
        self.LogList.clear()
        
        if os.sys.platform == 'win32':
            command = [DEFAULT_LM4FLASH_WIN32]
        elif os.sys.platform == 'linux2':
            command = [DEFAULT_LM4FLASH_LINUX]
        #elif os.sys.platform == 'darwin':
        #    command = [DEFAULT_LM4FLASH_OSX]
        else:
            return
        
        command.append('-v')
        command.append('-f')
        command.append(self.BinFile)
        
        try:
            self.LoaderProcess = subprocess.Popen( command,
                         stdin=subprocess.PIPE,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT,
                         shell= (os.sys.platform == 'win32'))
            error_count = 0
            while True:
                self.msleep(20)
                if not self.LoaderProcess:
                    break
                # read single lines    
                buff = self.LoaderProcess.stdout.readline()
                if buff == '': # got nothing
                    if self.LoaderProcess.poll() != None: # process exited
                        del self.LoaderProcess
                        self.LoaderProcess = None
                        # print 'compiler process finished.'
                        break
                else:
                    msg = str(buff).strip()
                    msg_lowered = msg.lower()
                    # string to QString
                    if msg_lowered.find("warning:") >= 0:
                        self.LogList.append( "<font color=orange>%s</font>" % msg )
                    # todo: other error messages
                    elif msg_lowered.find("error:") >= 0 \
                            or msg_lowered.find("error initializing") >= 0 \
                            or msg_lowered.find("error while matching") >= 0 \
                            or msg_lowered.find("error opening") >= 0 \
                            or msg_lowered.find("error claiming") >= 0 \
                            or msg_lowered.find("unable to find") >= 0 \
                            or msg_lowered.find("unable to get") >= 0 \
                            or msg_lowered.find("unable to open") >= 0 \
                            or msg_lowered.find("no icdi device") >= 0 \
                            or msg_lowered.find("found multiple icdi") >= 0 \
                            or msg_lowered.find("found icdi serial number collision") >= 0 \
                            or msg_lowered.find("not recognized") >= 0 \
                            or msg_lowered.find("operable program") >= 0:
                        self.LogList.append( "<font color=red>%s</font>" % msg )
                        error_count += 1
                    else:
                        self.LogList.append( "<font color=green>%s</font>" % msg )
        except:
            print 'got errors in flash loader thread!'
            self.LogList.append("<font size=4 color=red>error: Failed to load program!</font>")
            self.LoaderProcess = None
        
        if not error_count:
            self.LogList.append( "<font size=4 color=cyan>done.</font>" )
        else:
            self.LogList.append( "<font size=4 color=red>done with error(s) !</font>"  )
        
        print 'flash loader thread done.'
    
    
    def programDevice(self, binFile=None):
        if self.isRunning():
            return False, "busy"
        if not binFile:
            return False, "program what?"
        
        if not os.path.isfile(binFile): # file not found
            print 'binary file not found: ', binFile
            return False, "No *.bin file found! (re)build first the project."
        
        self.BinFile = binFile
        self.start()
        
        return True, "lm4flash tool running. please wait..."
    
    
    def pollBootLoadProcess(self):
        if self.isRunning() or self.LogList.count()>0:
            if self.LogList.count():
                return True, str(self.LogList.takeFirst())
            else:
                return True, ''
        else:
            return False, "process not running"
    
    
    
    