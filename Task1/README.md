UDP Loopback

Usage on linux cmd line:

* sudo chmod +x tap_create.sh
* sudo chmod +x tap_destroy.sh
* tap_create.sh (Create a tapbr0 tap0 and tap1 so we can talk to two RIOT-OS process)
* ./bin/native/UDP_Loopback.elf tap0
* Use ifconfig to get the RIOT-IPv6-addr
* echo "Message" | nc -6u RIOT-IPv6-addr%tapbr0 12345
