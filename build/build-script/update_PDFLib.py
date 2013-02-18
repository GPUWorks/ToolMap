#!/usr/bin/env python
# encoding: utf-8
"""
update_PDFLib.py

Created by Lucien Schreiber on 2011-12-05.
Copyright (c) 2011 CREALP. All rights reserved.
"""

import sys
import os
import shutil
import zipfile
import subprocess
import stat
import multiprocessing


def askUserWithCheck (msg, allowedval=['Y', 'y', 'N', 'n'], errormsg = "Unsupported value"):
  "Ask user for entering a value with validy check, looping until correct value is entered"
  while(1):
    myValue = input(msg)
    if (myValue.isdigit()):
      myValue = int(myValue)
    if (myValue in allowedval):
      if(type(myValue) != type(int())):
        return myValue.upper()
      return myValue
    print (errormsg)


def patchFileMac(libdir):
  """patch wxpdfdoc for mac unicode support"""
  myFile = libdir + os.sep + "src" + os.sep + "pdffontdatacore.cpp"
  print ("Patching file:", myFile)
  
  with open (myFile, "r") as rf:
    lines = rf.readlines()
    
    # small verification
    mycheckline = "      wxUint32 cc = (unsigned char) (*ch);\n"
    if (lines[186-1] == mycheckline):
      print ("Ok line correct... patching")
    else:
      print ("Error patching, line 186 should be:\n{}\nbut is instead\n{}".format(mycheckline, lines[186-1]))
      exit()
    
    with open(myFile, "w") as wf:
      # patch definition
      patchdict = {185 : "      // wxUint32 cc = (unsigned char) (*ch);\n", 186 : "      charIter = (*convMap).find(*ch);\n"}
    
      #patching
      for index, line in enumerate(lines):
        if index in patchdict:
          linepatched = patchdict[index]
          print (index, "-", linepatched[:-1])
          wf.write(linepatched)
        else:
          wf.write(lines[index])
  pass



def createEmptyDirs(bindir):
  """Creating vroomtwin, vroomloader, vroomgistests if not existing"""
  try:
    os.makedirs(bindir)
  except:
    print ("Error creating '{}', dir exists!".format(bindir))



def Windows():
  """docstring for Windows"""
  print ("Building for Windows 7")
  libpath = "D:\\LIB\\"
  libname = "wxpdfdoc-0.9.3"
  libprefix = ""
  libwx = "D:\\LIB\\wxWIDGETS-SVN"
  return libpath, libname, libprefix, libwx

  
def MacBook():
  """docstring for MacBook"""
  print ("Building for MacBook")
  libpath = "/Users/lucien/DATA/PROGRAMATION/_LIB/64/"
  libname = "wxpdfdoc-0.9.3"
  libprefix = "/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBPDF"
  libwx = "/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBWXSVN"
  return libpath, libname, libprefix, libwx

def LinuxHome():
  """docstring for Linux Home"""
  print ("Building for Linux Home")
  libpath = "/home/lucien/programmation/LIB/"
  libname = "wxpdfdoc-0.9.3"
  libprefix = "/home/lucien/programmation/LIB/_LIBPDF"
  libwx = "/home/lucien/programmation/LIB/_LIBWX"
  return libpath, libname, libprefix, libwx


def MacPro():
  """docstring for MacPro"""
  print ("Building for MacPro")
  libpath = "/Users/lucien/Documents/PROGRAMMATION/64/"
  libname = "wxpdfdoc-0.9.3"
  libprefix = "/Users/lucien/Documents/PROGRAMMATION/64/_LIBPDF"
  libwx = "/Users/lucien/Documents/PROGRAMMATION/64/_LIBWXSVN"
  return libpath, libname, libprefix, libwx 


def MacConfigure(libpath, libprefix, libwx):
  """docstring for MacBookConfigure"""
  try:
    os.remove(libpath+os.sep+"configure")
    os.remove(libpath+os.sep+"Makefile.in")
    os.rename(libpath+os.sep+"configure29", libpath+os.sep+"configure")
    try:
      os.lchmod(libpath+os.sep+"configure", 0o777)
    except:
      try:
        mymode = stat.S_IWRITE | stat.S_IREAD | stat.S_IEXEC | stat.S_IRGRP | stat.S_IWGRP | stat.S_IXGRP | stat.S_IROTH | stat.S_IWOTH | stat.S_IXOTH
        os.chmod(libpath+os.sep+"configure", mymode)
      except:
        print("Unable to change file rights! configure will not work!")
        exit()
    os.rename(libpath+os.sep+"Makefile29.in", libpath+os.sep+"Makefile.in")
  except:
    print ("Error renaming file!")
    exit()
  try:
    shutil.rmtree(libpath+os.sep+"build")
    shutil.move (libpath+os.sep+"build29", libpath+os.sep+"build")
  except:
    print("Error moving build folder!")
    exit()
  print ("Configuring now!")  
  myconfig = ['./configure']
  myconfig.append("--prefix=" + libprefix)
  myconfig.append("--with-wxdir=" + libwx)
  myconfig.append("--with-wx-config=" + libwx + os.sep + "bin" + os.sep + "wx-config")
  myconfig.append("--enable-unicode")
  myconfig.append("--disable-shared")
  print (" ".join(myconfig))
  try:
    myProcess = subprocess.Popen(myconfig, 0, None, None, None,  None, None, False, False, libpath+os.sep)
    myProcess.wait()
  except:
    print("Error configuring!")
    exit()
  
  
  
def MacBuild(libpath, libprefix, cpunumber):
  """docstring for MacBookBuild"""
  # CLEAN libprefix
  try:
    shutil.rmtree(libprefix)
  except:
    print("Error removing:", libprefix)
  createEmptyDirs(libprefix)  
  
  # MAKE
  mybuild = ['make']
  mybuild.append("--jobs=" + str(cpunumber))
  #mybuild.append(";")
  #mybuild.append("make")
  #mybuild.append(" install")
  print (" ".join(mybuild))
  try:
    myProcess = subprocess.Popen(mybuild, 0, None, None, None,  None, None, False, False, libpath+os.sep)
    myProcess.wait()
  except:
    print("Error Building!")
    exit()
  
  # INSTALL
  try:
    myProcess = subprocess.Popen(['make', 'install'], 0, None, None, None,  None, None, False, False, libpath+os.sep)
    myProcess.wait()
  except:
    print ("Error installing!")
    exit()


def WindowBuild (libpath, libprefix, cpunumber):
  print("building on Windows")
  print (libpath, libprefix, cpunumber)
  mycommand = ['nmake']
  mycommand.append("-f")
  mycommand.append("makefile.vc")
  mycommand.append("WX_DIR={}".format(libprefix))
  mycommand.append("WX_MONOLITHIC=1")
  
  mycmddebug  = mycommand[:]
  mycmddebug.append("LDFLAGS=/debug")
  mycmddebug.append("WX_DEBUG=1")
  mycmddebug.append('CPPFLAGS=/nologo /MTd /MP /EHsc /Zi /W4 /DDEBUG')
  
  mycmdrel  = mycommand[:]
  mycmdrel.append("WX_DEBUG=0")
  mycmdrel.append('CPPFLAGS=/nologo /MT /MP /EHsc /Ox /W3')
  print (" ".join(mycmddebug))
  print ("Compiling debug version")
  
  try:
    p = subprocess.Popen(mycmddebug, shell=False, cwd=libpath+os.sep+"build29")
    p.wait()
  except:
    print ("Error creating debug version")
    exit()
  
  try:
    p = subprocess.Popen(mycmdrel, shell=False, cwd=libpath+os.sep+"build29")
    p.wait()
  except:
    print ("Error building release version")
    exit()
  

  
    

if __name__ == '__main__':
  class Namespace: pass
  print ("Build wxPDFDocument library")
  print ("---------------------------")
  plateforms = ('MacBook','MacPro', 'Window 7', 'linux home')
  ns = Namespace()
  ns.libpath = ""
  ns.libname = ""
  ns.libprefix = ""
  ns.libwx = ""
  for enum, item in enumerate(plateforms):
    print ("{0}) {1}".format(enum, item))
  myValue = input ("Select Plateform: ")
  
  
  # GETTING VARIABLE NAMES
  if(plateforms[int(myValue)] == 'MacBook'):
    ns.libpath, ns.libname, ns.libprefix, ns.libwx = MacBook()
  elif (plateforms[int(myValue)] == 'Window 7'):
    ns.libpath, ns.libname, ns.libprefix, ns.libwx = Windows()
  elif (plateforms[int(myValue)] == 'MacPro'):
    ns.libpath, ns.libname, ns.libprefix, ns.libwx = MacPro()
  elif (plateforms[int(myValue)] == 'linux home'):
    ns.libpath, ns.libname, ns.libprefix, ns.libwx = LinuxHome()
  else:
    print ("Unsupported plateform")
    exit()
  print (ns.libpath, ns.libname)
  
  # REMOVING FOLDER AND EXTRACTING ZIP
  print ("removing folder if exists!")
  if (ns.libname != "" and ns.libpath != ""):
    try:
      shutil.rmtree(ns.libpath+ns.libname)
    except:
      print("unable to remove: {0}".format(ns.libpath+ns.libname))
  print ("Extracting ZIP file to {}".format(ns.libpath+ns.libname))
  myzipfile = zipfile.ZipFile(ns.libpath+ns.libname+".zip")
  myzipfile.extractall(ns.libpath)
  
  # PERFORMING PATCH
  #if(plateforms[int(myValue)] == 'MacBook' or plateforms[int(myValue)] == 'MacPro'):
  # patchFileMac(ns.libpath+ns.libname)
  #elif(plateforms[int (myValue)] == 'linux home'):
  # patchFileMac(ns.libpath+ns.libname)
  mypatchsrc = os.path.join(ns.libpath, "wxpdfpatch")
  mypatchdest = os.path.join(ns.libpath, ns.libname, "include", "wx")
  for filename in os.listdir(mypatchsrc):
    print ("Copying patch file:", filename)
    shutil.copy(os.path.join(mypatchsrc, filename), os.path.join(mypatchdest, filename))
  
  print ("Patching DONE")
  
  # PERFORMING CONFIGURE
  if(plateforms[int(myValue)] == 'MacBook' or plateforms[int(myValue)] == 'MacPro'):
    MacConfigure(ns.libpath+ns.libname, ns.libprefix, ns.libwx)
  elif  (plateforms[int(myValue)] == 'linux home'):
    MacConfigure(ns.libpath+ns.libname, ns.libprefix, ns.libwx)
  elif (plateforms[int(myValue)] == 'Window 7'):
    print ("No configuration needed!")
  else:
    print ("Unsupported plateform")
  
  
  # PERFORMING BUILD
  myNumberofProc = multiprocessing.cpu_count()
  if(plateforms[int(myValue)] == 'MacBook' or plateforms[int(myValue)] == 'MacPro'):
    MacBuild(ns.libpath+ns.libname, ns.libprefix, myNumberofProc)
  elif  (plateforms[int(myValue)] == 'linux home'):
    MacBuild(ns.libpath+ns.libname, ns.libprefix, myNumberofProc)
  elif(plateforms[int(myValue)] == 'Window 7'):
    WindowBuild(ns.libpath+ns.libname, ns.libwx, myNumberofProc)
  else:
    print ("Unsupported plateform")
  
  print ("---------------------------------")
  print ("wxPDFDocument build successfully!")
  print ("---------------------------------")
  
  
