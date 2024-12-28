# Weather-Station-Project

This project utilizes a PIC24 microcontroller, a DHT11 temperature/humidity sensor, and a MAX7219 matrix display. Data is received from the DHT using the proprietary Aosong 1 wire serial bus. This data is processed by the PIC24, and converted into a custom format used to display. The formatted data is sent to the 4 MAX chips using the SPI interface, and displayed on the matrix screen.
