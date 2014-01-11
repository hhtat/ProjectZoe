#!/bin/sh

Xvfb -screen 0 560x160x8 &
sleep 2
export DISPLAY=:0

cd bin
./xzoe &
cd ..

evilwm -fn fixed &

x11vnc &

