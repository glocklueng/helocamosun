A CPU is required for the upper functions of the helicopter.  These functions are as follows.

  * Implementation of our custom communication protocol (to be defined)
  * GPS data parsing
  * monitoring of non-stability related helicopter functions (battery, GPS etc)
  * assigning tasks to the stability MCUs (eg. go to waypoint, ascend, descend)
  * photography/video
  * collision detection

We will likely use an operating system such as Linux, QNX or another RTOS variant.  A Realtime OS may not be necessary, since the stability MCUs will handle critical helicopter functions in real time. So far we have considered the following modules:

Both of these modules can run the Linux kernel developed by [the PXA Linux Project](http://pxa-linux.sourceforge.net/)

# Gumstix #

![http://gumstix.com/store/catalog/images/prd_bsx400xmbt.jpg](http://gumstix.com/store/catalog/images/prd_bsx400xmbt.jpg)

Features
  * Intel PXA255 400MHz Processor
  * Embedded linux, comes pre-packaged with a 2.6 kernel
  * 16Meg flash
  * wide array of expansion boards (at extra cost)
  * Probably the usual I/O

Pros
  * fairly simple to get started

Cons
  * Aging Processor, I'm not sure about support, and I cant even find a datasheet

Cost
$197.27 for the 400MHz high end model (CDN)

Links
  * [Webpage](http://gumstix.com/store/catalog/product_info.php?cPath=27&products_id=155)
  * I havent been able to find the PXA255 datasheet yet

# Toradex Intel PXA #

![http://www.toradex.com/img/Colibri_PXA320.jpg](http://www.toradex.com/img/Colibri_PXA320.jpg)

Features
  * CPU: Monahans XScale® 806MHz
  * 64MB DDR RAM (32 Bit)
  * 32MB FLASH (16 Bit)
  * Interfaces
    * 16 Bit External Bus
    * I2C
    * SPI
    * One-Wire
    * 2xUART
    * IrDA
    * Consumer IR unit
    * 100 MBit Ethernet
    * Memory Stick
    * SDCard
    * CompactFlash - PCMCIA
    * 1x USB Host
    * 1x USB Host - Device
    * UTMI for USB 2.0 Host
    * LCD (XGA, 1024x768)
    * Touch Screen
    * Audio I/O (16Bit Stereo)
    * CMOS/CCD image sensor
    * MSL (up to 192 Mbps)
    * up to 123 GPIOs
    * comes with WinCE installed
Pros
  * Fast
  * Processor is up to date and well supported by Intel
  * on board image processing hardware

Cons
  * does not come pre-packed with Linux, though a tested Linux image is available for the 312MHz and 500MHz models


Cost
$152.97 for the 312MHz module, $214.84 for the 806MHz module (CDN)

Links
  * Webpage
  * Datasheet