<?php
// php.exe -f v8Server.php

$host = '127.0.0.1';
$port = 9999;

// Create a new socket
$socket = socket_create(AF_INET, SOCK_STREAM, 0);

// Bind the socket to an address and port
socket_bind($socket, $host, $port);

// Listen for incoming connections
socket_listen($socket);

// Accept an incoming connection
$client = socket_accept($socket);

// Read data from the client
$data = socket_read($client, 1024);

// Write data back to the client
socket_write($client, 'Server: ' . $data);

// Close the connection
socket_close($client);
socket_close($socket);
