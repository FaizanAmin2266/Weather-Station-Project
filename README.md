# Weather-Station-Project

This project utilizes a PIC24 microcontroller, a DHT11 temperature/humidity sensor, and a MAX7219 matrix display. Data is received from the DHT using the proprietary Aosong 1 wire serial bus. This data is processed by the PIC24, and converted into a custom format used to display. The formatted data is sent to the 4 MAX chips using the SPI interface, and displayed on the matrix screen. <br>
![weather_station_schem](https://github.com/user-attachments/assets/0b643618-0590-4bcc-aab0-3e4be8fcd1f6)
Project Schematic. PIC24 is connected to an MPLAB SNAP, which is not shown. The matrix display is 4 8x8 panels chained together. Both the display and DHT11 sensor are connected to 5V power, while the PIC is on 3.3V.

<br><br>

![20241209_001207](https://github.com/user-attachments/assets/80053d03-04d6-4c68-8cbd-cfd7beced0e9)
Display showing realtime temperature and humidity.
