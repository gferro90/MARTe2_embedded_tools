#/bin/sh
export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):/home/pc/stlink/stsw-link007/stlink/build/Release
/home/pc/stlink/stsw-link007/stlink/build/Release/st-flash --reset write output.bin 0x08000000
