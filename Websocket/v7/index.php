<?php

declare(strict_types = 1);

require_once __DIR__ . DIRECTORY_SEPARATOR . 'WorkerSocket.php';

$w = new WorkerSocket(
    __DIR__ . DIRECTORY_SEPARATOR . 'server.sock'
);

$w->connect(__DIR__ . '/' . time() . '-client.sock');

$i = 10;
while ($i > 0) {
    [$buf, $from] = $w->sendMessage('Hi number: ' . --$i);
    echo '[DEBUG]' . ' Received ' . $buf . ' from ' . $from . PHP_EOL;
    sleep(1);
}
$w->sendMessage('STOP');
?>