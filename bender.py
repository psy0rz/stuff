#! /usr/bin/env python
#
# Example program using irc.client.py.
#
# This program is free without restrictions; do anything you like with
# it.
#
# Joel Rosdahl <joel@rosdahl.net>

import irc.client
import sys
import time


irc.DEBUG=1
irc.client.DEBUG=1

class IRCCat(irc.client.SimpleIRCClient):
    def __init__(self, target):
        self.powwake=time.time()
        self.last_cmd_time=time.time()-1000
        irc.client.SimpleIRCClient.__init__(self)
        self.target = target

    def on_welcome(self, connection, event):
        if irc.client.is_channel(self.target):
            connection.join(self.target)
        else:
            self.send_it()

    def on_join(self, connection, event):
	   pass    

    def on_disconnect(self, connection, event):
        time.sleep(10)
        c.connect(server, port, nickname)
#       sys.exit(0)
        
    def on_pubmsg(self, connection, event):
	
    
	#handle a bot command and call cmd_<commandname>
	if (event.arguments[0][0]=='!'):
                if (time.time()-self.last_cmd_time < 5):
                        print "flood protection"
                        return

                self.last_cmd_time=time.time()

		m = "cmd_" + event.arguments[0][1:]
		if hasattr(self, m):
			getattr(self, m)(connection, event)
		else:
			print "No handler for command: ",m

			
    def cmd_redisgek(self, c, e):
        c.privmsg(e.target, "KLOPT!")

    def cmd_wakker(self, c, e):
        if (time.time()-self.powwake< 3600):
                c.privmsg(e.target, "zak maar mooi in de stront..je ben net wakker")
        else:
                self.powwake=time.time()
                self.cmd_pOwtime(c,e)

    def cmd_pOwtime(self, c, e):
        now=time.time()
        diff=int(now-self.powwake)
        diff=diff+(10*3600)

        sec=diff%60
        min=(diff/60)%60
        hour=diff/3600

        c.privmsg(e.target, "pow time is {0:02}:{1:02}.{2:02}".format( hour, min, sec))

        
if len(sys.argv) != 4:
    print "Usage: irccat2 <server[:port]> <nickname> <target>"
    print "\ntarget is a nickname or a channel."
    sys.exit(1)

s = sys.argv[1].split(":", 1)
server = s[0]
if len(s) == 2:
    try:
        port = int(s[1])
    except ValueError:
        print "Error: Erroneous port."
        sys.exit(1)
else:
    port = 6667
nickname = sys.argv[2]
target = sys.argv[3]

c = IRCCat(target)
try:
    c.connect(server, port, nickname)
except irc.client.ServerConnectionError, x:
    print x
    sys.exit(1)
c.start()

