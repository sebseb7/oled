
This file is part of a ZIP archive that was downloaded from
<http://www.idleloop.com/robotics/OLEDsample/>

The code provided here comes with no warranty or support.
It is distributed under the GNU General Public License.
See GNU-GPL.txt or http://www.gnu.org/copyleft/gpl.html for details.


This project is intended as an example of how to communicate with
the SparkFun OLED. It assumes that you are familiar with programming
the Atmel AVR family of microcontrollers. If you are not, it would
be a good idea to find another project as a learning tool.

This code was written for the ATmega644. If you use a different
microcontroller, you'll need to change the makefile (mcu target,
avrdude part). Other things like the timer code will likely need
to be updated as well.

Resources
 - Timer2 used by Timer code
 - USART0 used by Comm code
   connect uC Tx to PC
   38.4k, 8-N-1
 - OLED connections documented in OLED.cpp

Files:
 - main.cpp -- calls the OLED functions
 - OLED.cpp/h -- communications with the OLED
     see the header comment for details
 - Timer.cpp/h -- simple timer
 - Comm.cpp/h -- basic serial transmit
 - Util.h -- useful defines
 - makefile -- building and downloading (and setting uC fuse bits)
