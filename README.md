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