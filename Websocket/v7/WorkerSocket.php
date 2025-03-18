<?php

declare(strict_types = 1);

/**
 * @license UNLICENSE
 * @co-author William Desportes <williamdes [at] wdes [dot] fr>
 * @source https://www.php.net/manual/fr/function.socket-create.php#108535
 */
class WorkerSocket
{

    /**
     * @var string
     */
    private $serverSocketFile;

    /**
     * @var resource
     */
    private $openSocket;

    /**
     * @var string
     */
    private $openSocketFile;

    /**
     * @var bool
     */
    private $listen = true;

    public function __construct(string $serverSocketFile)
    {
        if (! extension_loaded('sockets')) {
            echo '[ERROR]' . ' The sockets extension is not loaded.' . PHP_EOL;
            exit(1);
        }
        $this->serverSocketFile = $serverSocketFile;
    }

    public function __destruct()
    {
        $this->disconnect();
    }

    public function connect(?string $clientSocket = null): void
    {
        $this->openSocketFile = $clientSocket === null ? $this->serverSocketFile : $clientSocket;
        // create unix tcp socket
        $socket = socket_create(AF_INET, SOCK_RAW, 0);
        if ($socket === false) {
            echo '[ERROR]' . ' Unable to create AF_UNIX socket.' . PHP_EOL;
            exit(1);
        }
        $this->openSocket = $socket;

        // same socket will be used in recv_from and send_to
        if (! socket_bind($this->openSocket, $this->openSocketFile)) {
            echo '[ERROR]' . ' Unable to bind to ' . $this->openSocketFile . PHP_EOL;
            exit(1);
        }
    }

    public function disconnect(): void
    {
        $this->listen = false;
        // close socket and delete own .sock file
        if (is_resource($this->openSocket)) {
            socket_close($this->openSocket);
        }
        if (file_exists($this->openSocketFile)) {
            unlink($this->openSocketFile);
        }
        $this->logger->debug('Disconnecting');
    }

    public function listen(): void
    {
        while ($this->listen) { // server never exits unless a magic command is sent
            // receive query
            if (!socket_set_block($this->openSocket)) {
                echo '[ERROR]' . ' Unable to set blocking mode for socket' . PHP_EOL;
                return;
            }
            echo '[DEBUG]' . ' Ready to receive...' . PHP_EOL;
            // will block to wait client query
            [$buf, $from] = $this->receive();

            // NOTE: remove this code to disable the magic STOP command
            if ($buf === 'STOP') {
                $this->listen = false;
            }

            echo '[DEBUG]' . ' Received ' . $buf . ' from ' . $from . PHP_EOL;

            $buf .= '->Response'; // process client query here

            // send response
            if (!socket_set_nonblock($this->openSocket)) {
                echo '[ERROR]' . ' Unable to set nonblocking mode for socket' . PHP_EOL;
                return;
            }

            // client side socket filename is known from client request: $from
            if (! $this->send($buf, $from)) {
                echo '[ERROR]' . ' Sending failed' . PHP_EOL;
                continue;
            }
            echo '[DEBUG]' . ' Request processed' . PHP_EOL;
        }
    }

    public function sendMessage(string $message): ?array
    {
        if (! $this->send($message, $this->serverSocketFile)) {
            return null;
        }

        // use socket to receive data
        if (!socket_set_block($this->openSocket)) {
            echo '[ERROR]' . ' Unable to set blocking mode for socket' . PHP_EOL;
            return null;
        }
        // will block to wait server response
        return $this->receive();
    }

    private function send(string $msg, string $receivingAddress): bool
    {
        $len = strlen($msg);
        // at this point 'server' process must be running and bound to receive from server.sock
        $bytes_sent = socket_sendto($this->openSocket, $msg, $len, 0, $receivingAddress);
        if ($bytes_sent === -1) {
            echo '[ERROR]' . ' An error occurred while sending to the socket' . PHP_EOL;
            return false;
        }
        if ($bytes_sent !== $len) {
            echo '[ERROR]' . $bytes_sent . ' bytes have been sent instead of the ' . $len . ' bytes expected' . PHP_EOL;
            return false;
        }
        return true;
    }

    /**
     * @return string[]
     */
    private function receive(): array
    {
        $buf  = '';
        $from = '';
        // will block to wait server response
        $bytes_received = socket_recvfrom($this->openSocket, $buf, MSG_WAITFORONE, 0, $from);
        if ($bytes_received === -1) {
            echo '[ERROR]' . ' An error occurred while receiving from the socket' . PHP_EOL;
            return [$buf, $from];
        }
        return [$buf, $from];
    }

}