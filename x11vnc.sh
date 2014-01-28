#!/bin/sh

killall x11vnc

export DISPLAY=:0

x11vnc &

