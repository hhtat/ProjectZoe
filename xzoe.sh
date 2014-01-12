#!/bin/sh

if [ "$( id -u )" != "0" ] ; then
  echo "not root"
  exit 1
fi

killall xzoe

export DISPLAY=:0

cd bin
./xzoe
cd ..

