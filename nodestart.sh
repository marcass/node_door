#!/bin/sh
#/etc/init.d/nodestart.sh

export PATH=$PATH:/usr/local/bin
export NODE_PATH=$NODE_PATH:/usr/local/lib/node_modules

case "$1" in
  start)
  cd /home/chip/git/node_door
  exec forever --sourceDir=/home/chip/git/node_door -p /home/chip/node_forever_logs garagedoor.js
  ;;
stop)
  exec forever stop --sourceDir=/home/chip/git/node_door garagedoor.js
  ;;
*)
  echo "Usage: /etc/init.d/nodeup {start|stop}"
  exit 1
  ;;
esac

exit 0

#########Scipt manipulation:        ###############
# 1. Move and make the script executable:
#chmod 755 /etc/init.d/nodestart.sh
# 2. Set it to go up and down with the system via Debian's update-rc.d:
	#update-rc.d nodestart.sh defaults
	#Stop it from coming up with:
	#update-rc.d -f nodestart.sh remove
###################################################

