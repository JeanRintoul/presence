Presence
========

This is intended to be about breaking the skin barrier between yourself and others and better-exposing your autonomic self and all that entails.

Currently a simple, fun, naive project, but with hopefully interesting directions.

Hardware
--------

The hardware this drives is:

- [Polar ECG chest strap](http://www.amazon.com/Polar-T31-Coded-Transmitter-Belt/dp/B000ARDPPU) (transmits wirelessly to)
- [Polar wireless receiver](http://danjuliodesigns.com/sparkfun/sparkfun.html) (connects via i2c with)
- [Any arduino board](http://www.arduino.cc/) (connects via digital output ports to)
- [El Escudo Dos](https://www.sparkfun.com/products/10878) 8-channel el-wire switch (connects directly to)
- Up to 8 el-wires

The specific configuration of pins and el-wire characteristics is declared at the top of the file. For that see the comment "This holds the pin numbers for the various length".


Note: alternate version with LCD screen was also made in parallel but was woefully undocumented(yes, I know). However, a youtube video was made! The reason an LCD screen with the heart rate beat per minute number(BPM) was used was because it was hard to see whether a person's heart rate was increasing or decreasing by noticing the synced blinking lights. Also, the Derby integration went with my outfit. 

<iframe width="854" height="510" src="//www.youtube.com/embed/NSAjIEhVMR8" frameborder="0" allowfullscreen></iframe>

An extra level of communication, beyond words, is to know someone else's heart rate, and to know how that rhythm is changing. There are many devices out there that measure heart rate with a chest strap, and transmit this information wirelessly to a receiver. This project, converted a derby into a display of heart rate. Initially it was going to just be a light display, but after noticing that just seeing something blink at a different 
