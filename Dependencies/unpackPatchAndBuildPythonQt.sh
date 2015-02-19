#!/bin/sh
# $Id$

# Shell script building HopsaGUI dependency PythonQt automatically
# Author: Peter Nordin peter.nordin@liu.se
# Date:   2012-03-29


pyversion="2.7"
basepwd=`pwd`
E_BADARGS=65

if [ $# -lt 1 ]; then
  echo "Error: To few input arguments!"
  echo "Usage: `basename $0` {release, debug} [pyversion]"
  echo "       pyversion is optional, (2.7 default)"
  exit $E_BADARGS
fi

if [ "$2" != "" ]; then
  pyversion="$2"  
fi

#ubuntuversion=$(echo `lsb_release -rs` | sed 's|\.||')

pythonqtname="PythonQt3.0"

# Abort if dir already exist. When running release build script we dont want to build twice
if [ -d $pythonqtname ]; then
echo "Directory $pythonqtname already exist. Remove it if you want (re)build using this script."
exit 0
fi

rm -rf $pythonqtname
unzip -q $pythonqtname.zip

cd $pythonqtname
echo "Applying Hopsan fixes to code"

# Apply patch to remove qt all extensions and other things
if [ -f ../$pythonqtname\_ubuntu.patch ]; then
patch -p0 < ../$pythonqtname\_ubuntu.patch
fi

# Remove tests and examples, always (test would not build at all for r209)
#sed "s|tests examples||" -i PythonQt.pro
cd ..


cd $pythonqtname

# Set build mode
if [ "$1" != "release" ]; then
  sed "s|#CONFIG += debug_and_release build_all|CONFIG += debug_and_release build_all|" -i build/common.prf
else
  #Remove tests and examples in release build
  sed "s|tests examples||" -i PythonQt.pro
fi

# Set python version
sed "s|unix:PYTHON_VERSION=2.6|unix:PYTHON_VERSION=$pyversion|" -i build/python.prf

qmake PythonQt.pro -r -spec linux-g++
make -j4 -w
cd $basepwd

