This is the source code of the firmware that is shipped with your 1Sheeld. It is an implementation of a custom version of the Firmata protocal ported to ATmega162. It supports some of Firmata's messages like digital messages but lacks capability query, analog, I2C and servos messages.

The repo is an Atmel Studio project, but we have plans to make it as a generic C project so you can build it easliy on platforms other than Microsoft Windows with a standard MakeFile.

You can generate the documentation by installing and running doxygen on the repo's root directory.

The ICSP pins are exposed with a 6-pin header on the bottom of your 1Sheeld, so you can easily connect any ATmega programmer and upload your own version of the firmware. We are adding some tutorials regarding that on our website soon.

Note: 1Sheeld is equipped with a bootloader that enables us to push updates to the firmware from the app. If you attempt to upload this firmware and erased the chip, you will lose this functionality.

If you need more help, don't hesitate posting a question on our [website's forum](http://www.1sheeld.com/forum).

If you want to contribute, please discuss your proposed feature on our forum first so that you can coordinate with the other developers working on the firmware.

Required Fuse and Lock bits:
- Low Value: 0xFD
- High Value: 0xD8
- Extended Value: 0xFB
Click (here)[http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega162&LOW=FD&HIGH=D8&EXTENDED=FB&LOCKBIT=CC] for a description of the enabled fuse bits.

Change Log:

--------- Version 1.2 (Major:1, Minor: 2) -----------

- Send all pin status when queried.
- Notify Arduino if BT disconnected.

--------- Version 1.1 (Major:1, Minor: 1) -----------

- Bluetooth reset enhancement on MC reset.
- Adjusted prescaler of timers.
- Enhanced stability.

--------- Version 1.0 (Major:1, Minor: 0) -----------

- Initial Release

1Sheeld Firmware by Integreight, Inc. is licensed under GNU General Public License v3.0 (GNU GPL v3.0).