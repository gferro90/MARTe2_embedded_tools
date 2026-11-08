#!/bin/sh
cfgfile=$1
patched=${cfgfile%.*}_patched.cfg
cp ${cfgfile} ${patched}
sed -i 's/^[ \t]*//g' ${patched}
sed -i 's/ //g' ${patched}
./SendConfiguration.ex /dev/ttyACM$2 ${patched} 115200 64 1234 4321
rm ${patched}
