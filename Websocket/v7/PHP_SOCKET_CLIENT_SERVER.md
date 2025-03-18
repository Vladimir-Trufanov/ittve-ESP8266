# PHP Socket client server example

With this example you will be able to use modern PHP code and have multiple clients connected.

The code is inspired and sourced from the of the posted notes here.

If you CTRL-C stopped the server, run the script twice. The first time will clean up the left over socket. Or just remove the socket file before starting the server.

There is a "magic" command (STOP) in the code to stop the server, you can easily remove it.

## server console
```sh
$ php -f server.php 
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 9 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 8 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 7 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 6 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 5 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 4 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 3 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 2 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 1 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received Hi number: 0 from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Ready to receive...
[DEBUG] Received STOP from /mnt/Dev/@williamdes/php/1620432655-client.sock
[DEBUG] Request processed
[DEBUG] Disconnecting
```

## worker 1 console

```sh
$ php -f client.php 
[DEBUG] Received Hi number: 9->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 8->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 7->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 6->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 5->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 4->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 3->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 2->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 1->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Received Hi number: 0->Response from /mnt/Dev/@williamdes/php/server.sock
[DEBUG] Disconnecting
```