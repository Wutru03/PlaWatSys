# PlaWatSys

PlaWatSys stands for plant watering system and is one of the projects I built for my computer science course.

## Sensors and actuators
The system measures the moisture of the soil with a capacitive soil sensor.<br>
The watering happens with a small submersible 5v waterpump.

<p>
  <img src="https://cdn.shopify.com/s/files/1/0300/6424/6919/products/IMG_4512_1200x1200.jpg?v=1648451999" width=300>
  <img src="https://imgaz1.staticbg.com/thumb/view/oaupload/banggood/images/CE/B5/1a1b0a77-9a85-408c-bcdf-101cebed4118.jpg" width=300>
</p>

## Controllers
The sensor and pump are controlled by an Arduino Uno. The Arduino is controlled by a Raspberry Pi.
For these two to communicate I wrote a custom protocol over the GPIO pins in which 
the Raspberry Pi can command the Arduino to turn on the pump or to send back the analog data of the sensor
by bit banging the value over one of the lines.

<p>
  <img src="https://m.media-amazon.com/images/I/71z22cRPeeL._AC_SL1000_.jpg" width=300>
  <img src="https://media.distrelec.com/Web/WebShopImages/landscape_large/9-/01/Raspberry%20Pi-PI4%20MODEL%20B_1GB-30152779-01.jpg" width=300>
</p>

## Daemons
The Raspberry Pi runs two C daemons.<br><br>
The first one is run every 15 minutes and gets the sensor data and stores it in a MariaDB database, if the soil is too dry it will also automatically start and stop the pump.<br><br>

The second one is a C socket server from which you can request to measure and water the plant manually.

## Website
The website shows all the data in the database in a nice and intuitive way, this data is retrieved by running a python script to retrieve the data from the database.<br>
The website also features two buttons to manually water the plant using the socket server or to toggle a plant to be enabled or disabled in the database.
