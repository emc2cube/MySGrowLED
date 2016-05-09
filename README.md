MySGrowLED
==========

![KiCad 3D view](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/MySGrowLED_3D.jpg)

Description
-----------

![KiCad traces](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/MySGrowLED_traces.jpg)

[MySensors](http://www.mysensors.org) module designed for wireless greenhouse automation (nRF24L01, 2.4GHz).

This module allow you to:
- control a primary LED light (5-40V input can be used to power the module)
- control a secondary circuit designed for a CPU PWM fan but can be used for a second light, humidifier, relay or anything else using 12V
- monitor temperature using an optional onboard DS18b20 sensor
- monitor external temperature by plugging external OneWire sensors
- monitor any additional parameters using additional I2C modules such as temperature, humidity, light levels, barometric pressure, etc...

Ordering
--------
Gerber files are included, so you can order these PCB at your favorite PCB fab house.
For an easy ordering process you can directly order these PCB from [DirtyPCB](http://dirtypcbs.com/view.php?share=19108&accesskey=2ba6d152bdb819f09c5eb2b8370af154) or [OSH Park](https://oshpark.com/shared_projects/yTV72xyU) without having to do anything.
Components are highly customizable and the default list may not be the best for your specific use, but it will work for most of the cases. Double check that the power supplies, voltage regulators and mosfets are compatible with your project.


Options
-------
This board is designed to be compatible with [MySensors](http://www.mysensors.org) but can be used on its own (timer or sensors to control the lights) or another system using 2.4GHz communication if you adapt the arduino code.

### ATSHA204 module
For security reason you can add a CryptAuthEE SHA256 chip. This will allow you to sign messages and will secure communications between the node and your gateway (this is not encryption, just signing).
If you only control lights, fan, or other non-essential hardware you probably don't need to bother with this chip.
Signing can also be done at the software level, without the chip if you decide to add this function later.

### Eeprom module
This module is only used to perform OTA updates on the node. If you don't plan to use this feature you can also skip this chip.
You will also need to burn a compatible bootloader to your arduino (DualOptiBoot)


Assembly
--------

Once you received your dirty package of PCBs (or any other Fab house) is received start inspecting it carefully to look for scratches, dents, or anything that seems wrong. Do not use the board if you have any concerns.
![DirtyPackage](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB1.jpg)

Markings are a little off, but nothing wrong on this one, good to go!
![PCB RV](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB2.jpg)

Start by soldering the optional chips, if you plan to use them.
![Eeprom soldering](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB3.jpg)
![ATSHA soldering](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB4.jpg)

Then add all other surface mount components. Don't be affraid by their little size, they are actually easy to solder. If you never soldered SMD components before, be sure to look for a few video tutorials first.
![SMD soldering](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB5.jpg)

Finish by adding the regular through-hole components, starting by the smaller ones.
![Components and terminals](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB6.jpg)

Plug your arduino module and radio and you are good to go.
![Arduino and radio](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB7.jpg)


Testing
-------

You can upload MySGrowPCB_test to your arduino to test the setup:
- It will look for an ATSHA204 and return its serial number if detected.
- It will look for an eeprom and return its manufacturer ID if detected.
- It will alternatively switch the LED and Fan circuits ON and OFF for 10 seconds.


Usage examples
--------------

This module can be used on the DIY LED light exemple included (PCB compared to breadboard design)
![PCB vs BreadBoard](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB9.jpg)
![MySGrowLED](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB9.jpg)

Another possible use to control a germination chamber inside a plastic container (PCB compared to previous protoboard design) 
![PCB vs ProtoBoard](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB10.jpg)
![SeedBox](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/img/PCB11.jpg)

Revision history
----------------

Version 1.0: Initial release.