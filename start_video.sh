#!/bin/sh

if [ "$( id -u )" != "0" ] ; then
  echo "not root"
  exit 1
fi

killall Xvfb

Xvfb -screen 0 560x160x8 &
sleep 2
export DISPLAY=:0

cd bin
./xzoe &
cd ..

evilwm -fn fixed &

