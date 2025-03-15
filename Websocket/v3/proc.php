<?php
header("Content-type: text/plain; charset=UTF-8");

require "zn_websocket.php";

$ws = new ZN_WebSocket("127.0.0.1", 3333);
$ws->run();
?>