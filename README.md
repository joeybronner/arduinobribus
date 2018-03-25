## ARDUINOBRIBUS 🚌

*Build your own shelter display using some Arduino boards & 3D printing parts*

<center>
    <a target="_blank" href="http://youtube.fr">
        <img src="images/prototype.png" />
    </a>
</center>

### Step 1: Mission ID

Find the **id** related to the mission, in the file located under **json/bus_missions.json**\
Mission object looks like:

    {
      "id":"100100020",
      "name":"Gare de Lyon / Gare Saint-Lazare",
      "shortName":"B20",
      "image":"b20.gif"
    }

Change the **sMissionId** value in **arduino/arduino.ino** with the id of your mission.\
*Example: 100100099 in my case*

### Step 2: Station ID

Open the following URL with your own mission ID: http://restratpws.azurewebsites.net/api/stations/<YOUR_MISSION_ID> and get the **id** of your station.\
Station object looks like:

    {
        "id":"PC3_1039_1091",
        "name":"Camille Flammarion"
    }

Change the **sStationId** value in **arduino/arduino.ino** with the id of your station.\
*Example: PC3_1047_1074 in my case*

### Step 3: Way (A/R)

The last value is the way of the bus ("a" or "r").\
To know the way, open the following URL with your own http://restratpws.azurewebsites.net/api/directions/<YOUR_MISSION_ID>.\
Direction object looks like:

    [
        {
            "way":"A",
            "name":"Porte Maillot"
        },
        {
            "way":"R",
            "name":"Skanderbeg"
        }
    ]

Change the **sWay** value in **arduino/arduino.ino** with the way you want to display.\
*Example: "a" in my case*

### Links

 - [GitHub - REST API for RATP](https://github.com/ferreirix/restratp)
 - [Fiche PC3](https://www.ratp.fr/sites/default/files/fiches-horaires/busratp/pc3.pdf)
 - [Img to logo for OLED](http://www.instructables.com/id/How-to-use-OLED-display-arduino-module/)
 - [3D Abribus](https://www.thingiverse.com/thing:2379938)
 - [3D OLED 0.96](https://www.thingiverse.com/thing:2176764)
 - [3D Box Wemos + OLED](https://www.thingiverse.com/thing:2798623)
 - [3D Standard OLED box with place for board](https://www.thingiverse.com/thing:2662079)
