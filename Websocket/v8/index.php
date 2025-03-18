<?php

// v8index.php 

$host = '127.0.0.1';
$port = 9999;

// Create a new socket
$socket = socket_create(AF_INET, SOCK_STREAM, 0);

// Connect to the server
socket_connect($socket, $host, $port);

// Write data to the server
socket_write($socket, 'Hello, Server!');

// Read data from the server
$data = socket_read($socket, 1024);

echo "Received from server: $data\n";

// Close the connection
socket_close($socket);

