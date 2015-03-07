# ArduinoProjects


A repo for miscellaneous multi-purpose Arduino Projects. These are a group of small projects that didn't fit anywhere else. They are listed along with a short description.

These projects come with no provided support or warranty, but I'd be happy to answer any quick questions you have. <a href="mailto:nick@nickmccomb.net">Email me</a> and I'll get back to you.

## AnalogLogger
 
 Originally designed for <a target="_blank" href="http://groups.engr.oregonstate.edu/osurc/">OSURC</a>, this program logs ADC data to an SD Card. It uses EEPROM to inteligently name files to allow for extended and individual tests. Optional RTC support.

#### Hardware Requirements
 * Arduino UNO
 * Arduino Ethernet Shield
 * SD Card (16 GB Used for Testing)
 * (Optional) <a target="_black" href="https://www.sparkfun.com/products/12708">Real Time Clock</a>

## SerialPassthrough

 Designed for testing serial-based devices. _Very_ simple passthrough script. Uses SoftwareSerial to interface the Hardware USART with a simulated one.
 
#### Hardware Requirements
 * Any Arduino
