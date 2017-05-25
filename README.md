
# Social-Media-Fan
The code and repository of an interactive, social-powered installation designed by Bruce Sterling and produced by Casa Jasmina. <br />A Tribut to NBH.

## Description
This projects uses an Arduino MKR1000 as a hardware and [IFTTT](https://ifttt.com/discover) plus [dweet.io](http://dweet.io/) as a software. <br />
Every time an interaction you choose happens, IFTTT will make an HTTP post request to dweet.io which stores this data up to 5 requests (for more you will have to upgrade). <br />Every hour the MKR1000 will poll those data and will trigger the fan accordingly, to then come back in sleep mode in order to save battery power.

## Getting started with [dweet.io](http://dweet.io/)
The peculiarity of this service is that you don't need any registration and that it keeps an history of the post requests you sent.<br /><br />

All you have to do is to choose a name for your "thing", then you'll be able to <strong>POST</strong> using: <br />

< https://dweet.io:443/dweet/for/your-thing-name?something=value >
<br /><br />
And to <strong>GET</strong> using:<br /><br />
< https://dweet.io:443/get/dweets/for/your-thing-name? ><br /><br />

For this particular project we will be looking for the time the fan need to be boosted, so in order to trigger the fan for 3 seconds our POST request will be:<br /><br />
< https://dweet.io:443/dweet/for/your-thing-name?boost=3 >

## Getting started with [IFTTT](https://ifttt.com/discover)
IFTTT (if this than that) allows you to easily link external services to each other. In our case we want every tweet with a certain hashtag to trigger an HTTP POST to dweet.io.<br /><br />
First thing is to create an account on IFTTT, then create a <strong>New Applet</strong>. Select <strong>New tweet from search</strong> and choose whatever hashtag you want.<br />

![Step 1](/images/image1.PNG)<br /><br />Create the "trigger" and proceed selecting the action service: <strong>Maker Webhoocks</strong><br /><br />
![Step 2](/images/image2.PNG)<br /><br />Now you can fill in the <strong>URL field</strong> with the POST request we created before, make sure you selected the POST method.<br /><br />
![Step 3](/images/image3.PNG)

## Configuration
All is left to do now is to upload the [sketch](/Sketch/Social-Media-Fan/Social-Media-Fan.ino) on your MKR1000.<br /><br />Once you uploaded the code the board will be in <strong>access point</strong> mode, so that it will expose a network named <strong>"MKR1000AP"</strong>.<br />Connect to the network and go to http://192.168.1.1 on your browser.<br /> It will redirect you to a web page where you can set the needed credentials.<br /><br />
![Step 4](/images/image4.PNG)
<br /><br />Press enter and you are done!


### Libraries
In order to compile the sketch you have first to download some libraries:<br /><br />
*[WiFi101](https://github.com/arduino-libraries/WiFi101)<br />
*[Json library](https://github.com/bblanchon/ArduinoJson)<br />
LowPower
