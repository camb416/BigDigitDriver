libBigDigitDriver
---

This Arduino library is designed to support building Electronic Circuits using the Big Digit Driver module designed by Cameron Browning for use in the AMNH Exhibition Dept in 2009. 

What is a BDD?

Thanks for asking. It's an electronic module that interfaces with some Kingbright LED 7-segment LED displays. Each module takes up to three displays and the modules can be daisy chained. Here's an image: http://www.flickr.com/photos/camb416/5518512448/

This Library

This library was abstracted out of some 2009 application code in 2011 by Mike Edwards and still has a lot of old, unnecessary bits that are specific to a particular application.

Next time any work is to be done on this library, the first thing to do will be to dump things like the analog pin parameter, and any other slide potentiometer references. There is also some references to Mass and Length. These were specific to a particular application and aren't relevant now that the library is abstracted.


