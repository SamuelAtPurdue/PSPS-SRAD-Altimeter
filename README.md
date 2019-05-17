# psps_srad_altimeter
<div>
  <h2>Description</h2>
  <br>
  <p>
    PSPS SRAD Altimeter is the backup flight computer for Purdue Space Program Solids 2018/2019. It records altitude, pressure tempurature, and status information to help diagnose problems with opperation and control the recovery system. The altimeter uses breakout boards from Adafruit and Sparkfun and a custom PCB. 
  </p>
</div>

<div>
  <h2>Hardware</h2>
  <br>
  <p>
    This Project uses a custom PCB which is included as eagle files. Hardware list for assembling the PCB can be found in the documentation. The other hardware required are listed below:
    <ul>
      <li> <a href = "https://www.adafruit.com/product/2651">Adafruit BMP280 Breakout</a></li>
      <li> <a href = "https://www.adafruit.com/product/254">Adafruit micro SD Card Breakout</a></li>
      <li> <a href = "https://www.adafruit.com/product/1897">Adafruit SPI FRAM Breakout</a></li>
      <li> <a href = "https://www.sparkfun.com/products/10255">Sparkfun Lipower Boost Converter</a></li>
      <li> <a href = "https://www.sparkfun.com/products/13777">Sparkfun Battery Babysitter</a></li>
    </ul>
  </p>
</div>

<div>
  <h2>Libraries</h2>
  <p>
    This Projects requires some modifications to Adafruit's BMP280 Libraries and the SD Library to work properly. These modified libraries as well as the other ones used are included. Links to the original Libraries below:
    <ul>
      <li> <a href ="https://github.com/adafruit/Adafruit_BMP280_Library">Adafruit BMP280</a></li>
      <li> <a href = "https://github.com/adafruit/SD">SD</a></li>
      <li> <a href = "https://github.com/adafruit/Adafruit_FRAM_SPI">Adafruit SPI FRAM</a></li>
      <li> <a href = "https://github.com/adafruit/Adafruit_Sensor">Adafruit Sensor</a></li>
      <li> <a href = "https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library">Battery Babysitter</a></li>
    </ul>
  </p>
</div>
