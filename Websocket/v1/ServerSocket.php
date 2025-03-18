<?php
require_once 'WebSocketServer.php';

$server = new WebSocketServer(8898);

// слушаем входящие соединения
while (false !== ($activity = $server->listen()))
{
  // каждую секунду читаем поступающие данные от клиентов
  foreach (array_keys($server->clients) as $address) 
  {
    if ($data = $server->readFrom($address))
    {
      // эмулируем работу эхо-сервера, отправляем полученные данные клиенту
      $server->sendTo($address, var_export($data, true));
    }
  }
  sleep(1);
}


