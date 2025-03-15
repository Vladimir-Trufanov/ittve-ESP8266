<?php
/**
 * Простой websocket-echo сервер
 */
class ZN_WebSocket
{
	/**
	 * IP адрес, на котором запущен сервер
	 * 
	 * @var string
	 */
	private $_ip;

	/**
	 * TCP порт, на котором запущен сервер
	 * 
	 * @var int 
	 */
	private $_port;
	
	/**
	 * Ресурс главного сокета
	 * 
	 * @var resource
	 */
	private $_socket_master;
	
	/**
	 * Массив с сокетами клиентов
	 * 
	 * @var array
	 */
	private $_socket_client;
	
	/**
	 * Таймаут между прошслушиваниями соединения
	 * 
	 * @var int
	 */
	private $_socket_select_usleep = 200000;

	/**
	 * Конструктор
	 * 
	 * @param string $ip
	 * @param int $port
	 */
	public function __construct($ip, $port) 
	{
		$this->_ip = $ip;
		$this->_port = $port;
	}

	/**
	 * Запустить сервер вебсокета
	 */
	public function run()
	{
		/**
		  * Создаём сокет
		  * AF_INET - семейство адресов - IPv4
		  * SOCK_STREAM - тип сокета - передача потока данных с предварительной установкой соединения.
		  * SOL_TCP - протокол TCP
		*/
		$this->_socket_master = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

		/* Связываем сокет с адресом */
		socket_bind($this->_socket_master, $this->_ip, $this->_port);

		/* Создаём очередь запросов на соединение */
		socket_listen($this->_socket_master);

		/* Делаем сокет неблокирующий */
		socket_set_nonblock($this->_socket_master);
		
		do
		{
			/* Данные для socket_select */
			$read = $this->_socket_client;			/* Слушаем клиентов */
			$read[] = $this->_socket_master;		/* Слушаем мастер сокет */
			$write = null;
			$error = null;

			/* Слушаем сокеты на поступление данных. После socket_select из массива read удаляются не читаемые сокеты */
			if (socket_select($read, $write, $error, 0, $this->_socket_select_usleep) < 1)
			{
				continue;
			}

			/* Пришли данные на главный сокет (рукопожатие) */
			if (in_array($this->_socket_master, $read))
			{
				/* Создаём читающий сокет */
				$sock = socket_accept($this->_socket_master);

				/* Делаем рукопожатие */
				$this->_handshake($sock);
			}

			/* Читаем сокеты клиентов */
			foreach ($this->_socket_client as $key => $sock)
			{
				if (in_array($sock, $read))
				{
					/* Читаем данные по сокету */
					$str = $this->_socket_read_all($sock);
					
					/* Раскодируем */
					$frame = $this->decode($str);
					
					switch ($frame['type'])
					{
						/* Текстовое сообщение */
						case "text":
						{
							echo "Пришло сообщение: «{$frame['message']}»\n";
							
							$msg = $this->encode($frame['message']);
							socket_write($sock, $msg);
						}
						break;
					
						/* Фрейм закрыть соединение */
						case "close":
						{
							echo "Закрываем соединение.\n";
							
							socket_close($sock);
							unset($this->_socket_client[$key]);
						}
						break;
					}
					
					
				}
			}
		}
		while (true);
	}

	/**
	 * Декодирование строки
	 * 
	 * @param string $str
	 * @param boolean $message_only
	 * @return array | string
	 */
	public function decode($str, $message_only = false)
	{
		/* Разбираем заголовок */
		$header = unpack("n", $str)[1];			/* Рапаковываем первые 16 бит, порядок байт «big endian», т.к. сетевой протокол */
		$data = [];
		$data['fin']			= (bool) (($header >> (16 - 1))  & 0b1);		/* Финальный фрейм. Если сообщение нефрагмантированное, то всегда 1, если фрагмантированно, то у последнего 1 у остальных 0 */
		$data['rsv1']			= (bool) (($header >> (16 - 2))  & 0b1);		/* Флаги RSV1, RSV2, RSV3 служат для расширений протокола, почти всегда в false */
		$data['rsv2']			= (bool) (($header >> (16 - 3))  & 0b1);
		$data['rsv3']			= (bool) (($header >> (16 - 4))  & 0b1);
		$data['opcode']			= (int)  (($header >> (16 - 8))  & 0b1111);		/* Тип фрейма */
		$data['is_mask']		= (bool) (($header >> (16 - 9))  & 0b1);		/* Замаскированы ли фреймы */
		$data['length_prev']	= (int)  (($header >> (16 - 16)) & 0b1111111);	/* Предварительная длина фрейма */
		
		/* Определяем тип фрейма */
		$data['type'] = $this->_get_type_frame($data['opcode']);
		
		/* Определяем длину фрейма */
		if ($data['length_prev'] < 126)
		{
			$data['length'] = $data['length_prev'];
		}
		elseif ($data['length_prev'] === 126)
		{
			$data['length'] = unpack("x2/n", $str)[1];
		}
		elseif ($data['length_prev'] > 126)
		{
//			$data['length'] = unpack("x2/J", $str)[1];
			$data['length'] = unpack("x2/x4/N", $str)[1];
		}
				
		/* Маска */
		if ($data['is_mask'])
		{
			if ($data['length_prev'] < 126)
			{
				$mask = substr($str, 2, 4);
			}
			elseif ($data['length_prev'] === 126)
			{
				$mask = substr($str, 2 + 2, 4);
			}
			elseif ($data['length_prev'] > 126)
			{
				$mask = substr($str, 2 + 8, 4);
			}
		}
		
		/* Тело запроса */
		$message_start = 2;
		if ($data['length_prev'] < 126)
		{
			
		}
		elseif ($data['length_prev'] === 126)
		{
			$message_start += 2;
		}
		elseif ($data['length_prev'] > 126)
		{
			$message_start += 8;
		}
		
		if ($data['is_mask'])
		{
			$message_start += 4;
		}
		$data['message_start'] = $message_start;
		
		$message = substr($str, $message_start, $data['length']);
		
		/* Размаскируем сообщение */
		if ($data['is_mask'])
		{
			$length = strlen($message);
			
			for ($i = 0; $i < $length; $i++) 
			{
				$message[$i] = $message[$i] ^ $mask[$i % 4];
			}
		}
		
		$data['message'] = $message;
		
		/* Возвращаем сообщение */
		if ($message_only === false)
		{
			return $data;
		}
		else
		{
			return $data['message'];
		}
	}
	
	/**
	 * Закодировать строку
	 * 
	 * @param string $str
	 * @param boolean $is_mask
	 * @return string
	 */
	public function encode($str, $is_mask = false)
	{
		$bin = "";
		
		/* Основные флаги */
		$data = 
		[
			"fin" => 0b1,
			"rsv1" => 0b0,
			"rsv2" => 0b0,
			"rsv3" => 0b0,
			"opcode" => 0x1,
			"is_mask" => $is_mask === true ? 0b1 : 0b0,
			"length" => strlen($str)
		];
		
		/* Предварительная длина */
		if ($data['length'] < 126)
		{
			$data['length_prev'] = $data['length'];
		}
		elseif ($data['length'] < 65536)
		{
			$data['length_prev'] = 126;
		}
		else
		{
			$data['length_prev'] = 127;
		}
		
		/* Заголовок */
		$header = $data['fin'];
		$header = $header << 1 | $data['rsv1'];
		$header = $header << 1 | $data['rsv2'];
		$header = $header << 1 | $data['rsv3'];
		$header = $header << 4 | $data['opcode'];
		$header = $header << 1 | $data['is_mask'];
		$header = $header << 7 | $data['length_prev'];
		$bin .= pack("n", $header);
		
		/* Расширенная длина тела */
		if ($data['length_prev'] === 126)
		{
			$bin .= pack("n", $data['length']);
		}
		elseif ($data['length_prev'] > 126)
		{
			$bin .= pack("x4N", $data['length']);
		}
		
		/* Маскировать сообщение */
		if ($is_mask)
		{
			$mask = substr(md5(microtime()), 0, 4);
			$bin .= $mask;

			$length = strlen($str);
			for ($i = 0; $i < $length; $i++) 
			{
				$str[$i] = $str[$i] ^ $mask[$i % 4];
			}
		}

		$bin .= $str;
		
		return $bin;
	}

	/**
	 * Рукопожатие
	 * 
	 * @param resource $sock
	 */
	private function _handshake($sock)
	{
		/* Считываем все заголовки */
		$header = $this->_socket_read_all($sock);
		$header = $this->_http_parse_headers($header);

		try
		{
			/* Проверяем наличие необходимых заголовков */
			$header_need = ['Host', 'Upgrade', 'Connection', 'Origin', 'Sec-WebSocket-Key', 'Sec-WebSocket-Version'];
			if (!empty(array_diff($header_need, array_keys($header))))
			{
				throw new Exception("Отсутствуют необходимые заголовки.");
			}

			/* Формируем необходимый ключ */
			$guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
			$sec_websocket_accept = $header['Sec-WebSocket-Key'] . $guid;
			$sec_websocket_accept = sha1($sec_websocket_accept, true);
			$sec_websocket_accept = base64_encode($sec_websocket_accept);

			/* Отвечаем на рукопожатие */
			$answer = 
				"HTTP/1.1 101 Switching Protocols\r\n" .
				"Upgrade: websocket\r\n" .
				"Connection: Upgrade\r\n" .
				"Sec-WebSocket-Accept: {$sec_websocket_accept}\r\n".
				"Sec-WebSocket-Version: 13\r\n\r\n";
			socket_write($sock, $answer);

			/* Помещаем в массив сокетов клиентов */
			$this->_socket_client[] = $sock;
			
			return true;
		}
		catch (Exception $e)
		{
			echo $e->getMessage() . "\n";
			socket_close($sock);
			
			return false;
		}
	}

	/**
	 * Читаем все данные пришедшие с сокета
	 * 
	 * @param resource $sock
	 * @return string
	 */
	private function _socket_read_all($sock)
	{
		$length = 2048;
		$str = "";
		do
		{
			$buf = socket_read($sock, $length);
			$str .= $buf;
		}
		while (strlen($buf) === $length);

		return $str;
	}

	/**
	 * Определить тип фрейма по опкоду
	 * 
	 * @param int $opcode
	 * @return string
	 */
	private function _get_type_frame($opcode)
	{
		/* Продолжение фрейма (фрагматированный фрейм) */
		if ($opcode === 0x0)
		{
			return "cont";
		}
		/* Текстовой фрейм */
		elseif ($opcode === 0x1)
		{
			return "text";
		}
		/* Двоичный фрейм */
		elseif	($opcode === 0x2)
		{
			return "binary";
		}
		/* Другие фреймы с данными (зарезервировано) */
		elseif ($opcode <= 0x3 and $opcode >= 0x7)
		{
			return "other_data";
		}
		/* Закрытие соединения */
		elseif ($opcode === 0x8)
		{
			return "close";
		}
		/* PING */
		elseif ($opcode === 0x9)
		{
			return "ping";
		}
		/* PONG */
		elseif ($opcode === 0xA)
		{
			return "pong";
		}
		/* Другие управляющие фреймы (зарезервировано) */
		elseif ($opcode <= 0xB and $opcode >= 0xF)
		{
			return "other_control";
		}
	}
	
	/**
	 * Парсим заголовки HTTP
	 * 
	 * @param string $str
	 * @return array
	 */
	private function _http_parse_headers($str)
	{
		if (function_exists('http_parse_headers'))
		{
			return http_parse_headers($str);
		}
		else
		{
			$header = [];
			$key = "";

			foreach(explode("\n", $str) as $i => $h)
			{
				$h = explode(":", $h, 2);

				if (isset($h[1]))
				{
					if (!isset($header[$h[0]]))
					{
						$header[$h[0]] = trim($h[1]);
					}
					elseif (is_array($header[$h[0]]))
					{
						$header[$h[0]] = array_merge($header[$h[0]], [trim($h[1])]);
					}
					else
					{
						$header[$h[0]] = array_merge([$header[$h[0]]], [trim($h[1])]);
					}

					$key = $h[0];
				}
				else
				{
					if (substr($h[0], 0, 1) == "\t")
					{
						$header[$key] .= "\r\n\t" . trim($h[0]);
					}
					elseif (!$key)
					{
						$header[0] = trim($h[0]);
					}
				}
			}

			return $header;
		}
	}
}


?>