# Arduino Library for u-blox SARA R410M

## Usage
<div align="justify">
This library enables an [Arduino MKR NB 1500](https://docs.arduino.cc/hardware/mkr-nb-1500) to configure, connect to, and publish data to an MQTT broker utilizing the u-blox [SARA-R4 series](https://www.u-blox.com/en/product/sara-r4-series?legacy=Current#Documentation-&-resources) AT command set.

It functions as an extension, and depends on the [MKRNB library](https://github.com/arduino-libraries/MKRNB) for core functionality.

The library has been tested with the SARA-R410M-02B module running the following firmware version:
- Modem: L0.0.00.00.05.12
- Application: A.02.21

Note: Due to known firmware limitations, MQTTS does not function properly [u-blox id 3869].

## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
</div>
