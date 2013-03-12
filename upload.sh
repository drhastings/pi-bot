#! /bin/sh

make all && sudo avrdude -pm328p -carduino -P /dev/ttyAMA0 -b 115200 -U flash:w:main.hex && cat /dev/ttyAMA0
#make clean && make all && sudo avrdude -pm328p -carduino -P /dev/ttyAMA0 -b 115200 -U flash:w:main.hex
