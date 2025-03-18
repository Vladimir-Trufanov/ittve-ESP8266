<?php

// php.exe -f v7Server.php

declare(strict_types = 1);

require_once __DIR__ . DIRECTORY_SEPARATOR . 'WorkerSocket.php';

$w = new WorkerSocket(
    __DIR__ . DIRECTORY_SEPARATOR . 'server.sock'
);

$w->connect();
$w->listen();

?>