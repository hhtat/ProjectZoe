#!/bin/sh

if [ "$( id -u )" != "0" ] ; then
  echo "not root"
  exit 1
fi

sudo cp bin/am335x-bone.dtb /boot/uboot/dtbs/  
