# neuprog
Raspberrypi code based on pigpio to programm the Neutrogena Activator to 99 uses. Batteries still need to be exchanged after about 30 uses.

The initial reengineering was done by Ed Rosten and I used his work as a base for this Raspberry version of it.

https://deathandthepenguinblog.wordpress.com/2018/01/03/hacking-the-neutrogena-visible-light-therapy-system-to-get-99-lives/


The git repo contains the code I used to reset the counter to 99, it is using pigpio to bitbang the SPI. Raspberry could also use SPI directly, anyways since the code was already there it was easier to just use Eds code with adations to the RPi.
It also contains pictures from the instrumented PCB and a cool tool I ordered for the future, a clip for S0-8, this way there is no soldering when reseting the counter.


