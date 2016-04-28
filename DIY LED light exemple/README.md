DIY 60W (3*20W) grow LED bar
============================

Disclaimer
----------
This section is only an example of what can be done. Consider this guide as an illustration or just as a way for me to consolidate all useful information on my setup for future use. No support provided!
It will give you general directions but you will probably want to customize it to fit your grow space, greenhouse, etc.
This setup is currently working so if you tried to recreate it on your side using the same components from the same source and following all steps and it's not working then it's probably because:
- One of the component is defective.
- You did something wrong.

Components
----------
Total cost in parts is ~$95 for a *real* 60W LED light with a spectrum optimized for plant growth and photosynthesis (no green).
Most of the commercial solutions are extremely deceptive on their real output an power usage. In this example, as we are using 3*50W chips, this would typically be advertised as a 150W light.
This is totally wrong as only 60W of power will be used. It is not advised to run a LED chip at its nominal power as it will generate a lot of heat and will fail quickly!

To create this 60W LED fixture you will need the following materials:
- 3*50W full spectrum 45mil COB LED ($30, [~$10](http://www.ebay.com/itm/321822518554) each).
Can be bought on [ebay](http://www.ebay.com/itm/321822518554), [aliexpress](http://www.aliexpress.com/store/product/5pcs-400nm-840nm-Full-Spectrum-led-grow-chip-10W-20W-30W-50W-100W-30mil-45mil-led/1393057_32597667622.html), etc.
![LED Chips](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_1.jpg)

- 3*heatsinks able to cool down your LED modules ($30, [~$10 each](http://www.ebay.com/itm/231185535248)).
In this case we will be using a [DC-HS11132 heatsink](http://store3.sure-electronics.com/dc-hs11132) for passive cooling.
Heatsink come with 4 to 16 M3 screws and a pack of thermal paste for improved heat dissipation.
Using a smaller heatsink, or running more power in the LEDs would require a fan. Using this setup, the whole assembly is at ~40°C while running which is safe for the plants even at a close distance.
This specific heatsink can be mounted on a 35mm DIN rail which will allow to link the 3 LEDs on a single bar fixture.
![Heatsinks](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_2.jpg)

- 1*mounting track, 35mm DIN rail Type, 1m length, ([~$7](http://www.mouser.com/ProductDetail/Schneider-Electric/16-700DIN/)).
I used a [16-700DIN](http://magnecraft.thomasnet.com/item/all-categories/accessories/pn-3042) from [Schneider Electric](http://www.serelays.com).
Heatsink will slide on the rail and then can be bolted to it using the existing holes in the rail.

- 1*60W LED driver to power the LEDs ([~$16](https://www.jameco.com/webapp/wcs/stores/servlet/Product_10001_10001_2100540_-1)).
I use a [LPC-60-1750](http://www.meanwell.com/mw_search/LPC-60/LPC-60-SPEC.PDF) from [Mean Well](http://www.meanwell.com) that will provide 34V at 1750mA. LEDs will be wired in serial.
As this unit will be in charge of converting your main power (110-230V) to ~34V for more than 12h a day, every day, in potentially humid conditions it is very important to choose a power supply you can trust. You don't really want it to catch fire.
I would not recommend buying the power supply from ebay or directly from china as you may end up with a counterfeit unit unless you are sure of the seller.

- 1*PCB DIN rail mounting bracket ([~$2](http://www.ebay.com/itm/181561948236))
Will be used to mount the PCB to the rail. This is optional, if you prefer using a timer, a switch or have another way to control your light you can use this LED fixture as a standalone without the MySGrowLED part. But in this case I wonder why you are on this page.

- Misc (~$10)
You will also need some basic supplies that you may already have, or will find at your local hardware store. You will also need to solder the wires to the LED chip.
In this case you will need ~3m of [solid 2-conductor cable](http://www.lowes.com/pd_70086-295-64162101_1z0v13b__?productId=4294345&pl=1) and ~40cm of [solid 3-conductor cable](http://www.lowes.com/pd_70087-295-64168802_1z0v13b__?productId=3379310&pl=1) (optional if you are not using MySGrowLED).
I used thermostat cable that can easily bought in roll or cut at your size at your local store.
You will also need some wire terminal blocks to connect the wires, hex bolts and nuts to secure the heatsinks to the rail, some rope, hooks or any other way to suspend the LED fixture to your grow area.

Assembly
--------
The whole process should not take too much time, probably around 1 hour without rushing it.

### LED assembly
Start by removing the heatsinks from their package. They should come with at least 4 screws and a small pack of thermal paste.
![Heatsinks unboxed](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_3.jpg)

Grab a LED chip and lay it face down on a soft nonabrasive surface.
![Chip back](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_4.jpg)

Locate the thermal paste and carefully tear it open.
![Thermal paste packet](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_5.jpg)

Empty the whole packet content over the LED metal back...
![Paste on chip](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_6.jpg)

... and spread it evenly over all the back surface.
![Spread](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_7.jpg)

Grab the LED chip and using the provided screws mount it on the heatsink. Do not screw tight each screw one by one but do it in multiple steps to spread the paste evenly.
![Screw](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_8.jpg)

Repeat for the 2 other LED chips and heatsinks.
![Repeat](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_9.jpg)

### Soldering
Cut your 2-conductor cable in ~1m segments. Decide which color will be the positive wire (red in my case) and negative (white).
Locate the positive and negative contacts on the LED. They are the tiny metal wings on each side of the LED. Usually the positive side is perforated with 2 holes while the negative side only have 1 hole. You may want to check this using a multimeter. If at the end your lights don't light up it's probably reversed on your LEDs. Don't worry, just switch the wires on the terminal connector (see below).

Remove the plastic insulator over ~1cm and solder the wire directly to the LED chip connector. Use as much solder as possible to create a good electric contact.
![Soldered](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_10.jpg)

Repeat the process for each LED unit.

### Final assembly
Now that all units are ready you need to create the fixture using the rail.
Bend the cable around the heatsink to have it sandwiched between the heatsink and the rail. All wires should be oriented the same way, toward your power supply output.

Slide all heatsinks to their definitive position and secure them to the rail.
![Secured](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_11.jpg)

On the end where all your wires are converging, thread the 2-connector cables into the second DIN rail hole. Thread the 3-conductor in the first hole (if using MySGrowLED) toward the second heatsink.
Use a terminal connector to link all 2-connector wires together according to their polarity. On the other side link the 3-conductor using the same colors, leaving the extra color alone.
Secure the terminal to the rail using a zip-tie.
If you are using MySGrowLED link the positive output of the power supply to the positive side of your LEDs (red wire in my case) and the negative output of the power supply *to the extra color of the 3-conductor wire* (green in my case). If you are using a different control circuit link the negative output of the power supply output directly to the negative side of your LEDs (would be white wires in this case). If your lights are not working, probably because of a polarity error, switch the wires at the terminal, no need to desolder the wires!
![Wiring](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_12.jpg)

Add some rope, chain, hooks or other to suspend the LED fixture to your grow space.
![Rope and hook](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_13.jpg)

Mount everything (in my case suspended to a [$25 IKEA OMAR shelving unit](http://www.ikea.com/us/en/catalog/products/00069768/#/10069763))
![LEDs mounted](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_14.jpg)

Plug the power supply to the electrical main and congratulations, you now have a LED grow light!
![LEDs on](https://raw.githubusercontent.com/emc2cube/MySGrowLED/master/DIY%20LED%20light%20exemple/img/DIY_LED_15.jpg)