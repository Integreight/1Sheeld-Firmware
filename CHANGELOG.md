## v1.6 (Major:1, Minor:6) (April 10, 2016)
 - Supported the new plus board. (Sending chunks of 20 bytes every 15ms)
 - Made the repo a generic avr-gcc project that can be built with a make command.
 - Made the board testable using a special frame.
 - Added the ability to rename the bluetooth module.
 - Added the ability to query and change the board's communication baud rate.

## v1.5 (Major:1, Minor:5) (November 8, 2015)
 - Prepared the firmware to receive the new disconnection frame.
 
## v1.4 (Major:1, Minor:4) (October 13, 2015)
 - Increased the RX1 buffer size to 256 instead of 64.
 - Made some required checks before processing app frames.

## v1.3 (Major:1, Minor:3) (April 30, 2015)
 - Added error checking bytes to the incoming BT reset frame.

## v1.2 (Major:1, Minor:2) (December 24, 2014)
 - Send all pin status when queried.
 - Notify Arduino if BT disconnected.

## v1.1 (Major:1, Minor:1) (June 23, 2014)
 - Bluetooth reset enhancement on MC reset.
 - Adjusted prescaler of timers.
 - Enhanced stability.

## v1.0 (Major:1, Minor:0) (April 7, 2014)
 - Initial Release.