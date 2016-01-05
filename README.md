# 1Sheeld Firmware [![Build Status](https://travis-ci.org/Integreight/1Sheeld-Firmware.svg?branch=master)](https://travis-ci.org/Integreight/1Sheeld-Firmware)#

## Overview ##

This is the source code of the firmware shipped with your 1Sheeld board. It is an implementation of a custom version of the Firmata protocal ported to ATmega162. It supports some of Firmata's messages like digital messages but lacks capability query, analog, I2C and servos messages.

## Building ##

The project is an generic C project, and can be built using any port of *avr-gcc* for either Microsoft Windows, Linux, or Mac OSX. Just make sure you have both *GNU Make* and *avr-gcc* tool chain installed on your platform then run ``` make ``` on the repo's root directory and both the debug and release versions will be built in a new subdirectory called *build*.

## Fuse Bits ##

- Low Value: 0xFD
- High Value: 0xD8
- Extended Value: 0xFB

Click [here](http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega162&LOW=FD&HIGH=D8&EXTENDED=FB&LOCKBIT=CC) for a description of the enabled fuse bits.

## Uploading ##

The ICSP pins are exposed with a 6-pin header on the bottom of your 1Sheeld board. You can easily connect any ATmega programmer and upload your own version of the firmware.

If you are using any [USBasp programmer](http://www.fischl.de/usbasp/) and have *avrdude* installed, after building using ``` make ``` you can flash your board by running either ``` make flashdebug ``` or ``` make flashrelease ``` on the repo's root directory. Make sure you erase the chip first by running ``` make erase ```.

Note: 1Sheeld is equipped with a bootloader that enables us to push updates to the firmware from the app. If you attempt to upload this firmware and erased the chip, you will lose this functionality.

## Documentation ##

You can generate the documentation by installing and running ``` doxygen ``` on the repo's root directory.

## Contribution ##

Contributions are welcomed, please follow this pattern:
- Fork the repo.
- Open an issue with your proposed feature or bug fix.
- Commit and push code to a new branch in your forked repo.
- Submit a pull request to our *development* branch.

Don't forget to drop us an email, post on our forum, or mention us on Twitter or Facebook about what you have built using 1Sheeld, we would love to hear about it.

## Changelog ##

To see what has changed in recent versions of 1Sheeld Firmware, see the [Change Log](CHANGELOG.md).

## License and Copyright ##

```
This code is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 3 only, as
published by the Free Software Foundation.

This code is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
version 3 for more details (a copy is included in the LICENSE file that
accompanied this code).

Please contact Integreight, Inc. at info@integreight.com or post on our
support forums www.1sheeld.com/forum if you need additional information
or have any questions.
```
