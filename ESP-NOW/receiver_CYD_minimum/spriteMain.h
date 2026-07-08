/** YellowPrinter-Esp32-Arduino                            *** spriteMain.h ***
 * 
 * Представить на экране дисплея поступившие строки и уложенные в спрайт
 * 
 * v1.0.0, 08.07.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.04.2024
**/

#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class TSprite_Main 
{
  public:

  // Построить объект (конструктор класса)
  TSprite_Main(int iQueueSize=4); 

  void clrScr(bool color); 



  private:

  int t;

  /*
  // Определяем счетчик прерываний от таймера и общее их количество до
  // события отправки одного из четырех сообщений = чуть быстрее четверти секунды 
  unsigned int cntr = 0;                      // счетчик тиков таймера
  volatile unsigned int timerToggle = 21000;  // 62499 -> 1 секунда;
  volatile bool OneSecondFlag = false;        // истечение 1 сек для запуска трассировок
  volatile bool Motor1_Flag = false;          // истечение 1 четверти сек для отправки данных по мотору
  volatile bool Vcc2_Flag = false;            // истечение 2 четверти сек для отправки данных по напряжению батареи
  volatile bool Act3_Flag = false;            // истечение 3 четверти сек для подтверждения крайней команды
  volatile byte quatr = 0;                    // счетчик четвертей секунды
  */
};


  TSprite_Main::TSprite_Main(int iQueueSize) 
  {
    TFT_eSPI tft = TFT_eSPI();
    t=iQueueSize;
  };

  void TSprite_Main::clrScr(bool color) 
  {
    bool c=color;
  }


  // *********************************************************** spriteMain.h ***
