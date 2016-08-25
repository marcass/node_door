# node_door
Use node.js to control garage door using C.H.I.P mini computer

## Materials needed
* C.H.I.P.  mini computer https://getchip.com/ (i'm running a debian headless image on it)
* relay that can be switched with 3.3v
* some interface. I'm using openHab, but you may just want to operate it through a webpage interface
* some reed switches for figuring out door state (2x window/door reed switch form aliexpress with do the trick)

## Steps to implement

1. Install image on chip using chrome extension
2. Setup node https://nodejs.org/en/download/package-manager/
3. Deploy code from this git repo
4. Satisfy dependencies
5. Connect I/O pins to reed switches for state reporting (one end of reed switch to i/o pin, then other to ground)
6. Connect relay to button terminals on door opener (usually GND and PB in door opener circuit board)
7. Configure openHab

## Notes
I couldn't get the chip GPIO to drive an active high relay due to piddly amounts of current flowing from a GPIO pin pulled high - it wouldn't even saturate an NPN transistor! I could get an active low relay to work, but the problem with that is if power failure occured, it would trigger the relay during boot sequence upon resumption of power connection. Therefore I had to use a solid sate relay that has 2 connectors (- and 3-30v) and connect the CHIP 3.3v pin to the 3-30v pin and the GPIO pin to -, then I pull the GPIO pin down to make it sink current to ground activating the relay. I don't seem to get any unexpected relay trips during power down or power up now.

I also don't really understand why I can get away with floating GPIO pins with th CHIP! I should read the datasheet (that thing is complex)......

You could also use some form of authentication to your nodejs server and just request web pages for status updates or actions if you don't have or can't be bothered with openHab. In the case of the code posted it would be something like:
* http://<server address>:8080/status for door status
* http://<server address>:8080/action/open to open the door if closed (and replace open with close in the URL to close)
