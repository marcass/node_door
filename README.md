# node_door
Use mosquitto broker to manage alerts for an opened door via MQTT 

## Materials needed
* C.H.I.P.  mini computer https://getchip.com/ (i'm running a debian headless image on it)
* speakers plugged into chip
* broker machine - this can be run on the chip or a server doing other things

## Steps to implement

1. Install image on chip using chrome extension or command line
2. Setup mosquitto broker
3. Install and test hardware on door (using a reed switch)
4. Update ESP8266 module (esptool)
5. Flash arduino and connect I/0
6. Test software
