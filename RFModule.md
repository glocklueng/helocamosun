# Easy Radio ERx00TRS-02 Transceiver. #

The ERx00TRS-02 has the following characteristics:
  * Half Duplex operation (RX and TX but not simultaneously)
  * Customizable Serial I/O baud rate (deigned to interface with uC UARTS
  * CMOS/TTL logic levels
  * 180 byte TX/RX Buffer
  * Modules available in 400MHz and 900Mhz ranges
  * Includes software for programming the module

Pros:
  * It will significantly reduce design time, and allow us to focus more on making the helicopter actually fly.

Cons:
  * This radio module has a maximum data throughput of 38.4kbps (see page 14 of the datasheet).  This is plenty for telemetry data, but too slow for video or photos.

Links:
  * [datasheet](http://www.radio-modules.com/products/radiodatamods/datasheets/er-datasheet-2.3-sept-05.pdf)
  * [website](http://www.radio-modules.com/products/radiodatamods/radiodatamods.asp)

# XBee-PRO™ ZigBee OEM RF Module #

Characteristics:
  * 60 mW (18 dBm), 100 mW EIRP power output (up to 1 mile range)
  * 250kbps RF data rate, up to 115.2kbps interface data rate
  * serial data interface: 3V CMOS UART - No configuration required

Pros:
  * Good range
  * fairly inexpensive ($32.00 usd)
  * off the shelf USB reciever available

Cons:
  * probably more difficult to set up than the Easy Radio module

Links
  * [Webpage](http://www.maxstream.net/products/xbee/xbee-pro-oem-rf-module-zigbee.php)
  * [Manual](http://www.maxstream.net/products/xbee/manual_xb_oem-rf-modules_zigbee.pdf)

# Spark Fun Nordic Transceiver - 2.4GHz with Built-In Antenna #

Characteristics:
  * Frequency: 2.4~2.524 GHz
  * Modulation type: GFSK
  * Operating Voltage: 3V
  * Output Power: +4dBm
  * Data Rate: 1Mbps; 250Kbps
  * Small footprint size: 20.0 x 36.7 x 2.4mm
  * Operating Temperature: -40 ~ +85 C
  * Long range: 280mts @250Kbps ; 150mts @1Mbps
  * Built-in antenna

Pros:
  * high data rate ( 250 to 1000 kbps)
  * inexpensive ($19.00 usd)

Cons:
  * short range (280 meters max)

Links
  * [Webpage](http://www.sparkfun.com/commerce/product_info.php?products_id=151)
  * [datasheet](http://www.sparkfun.com/datasheets/RF/RF-24G_datasheet.pdf)


