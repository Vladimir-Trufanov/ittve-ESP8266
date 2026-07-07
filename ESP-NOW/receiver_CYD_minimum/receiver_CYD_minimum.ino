// receiver_bare_minimum.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

#include <esp_now.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite stext3 = TFT_eSprite(&tft); // Sprite object stext3

uint16_t i=0;


void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
  //tft.println("\nПрием: ");
  Serial.println("\nПрием: ");
  for(int i=0; i<len; i++)
  {
    Serial.print((char)incomingData[i]);
    tft.print(((char)incomingData[i]));
  }
  Serial.println();
  //tft.println();

  saysprite();
}

void saysprite()
{
    // Чистим буфер памяти, выделенный под спрайт, заданным цветом
    //stext3.fillSprite(TFT_BLACK);

    // Так как предполагается вывод всего 2 строк, то 
    // вначале выводим прежнее значение в нижней части

    // Устанавливаем позицию курсора для вывода текста в заданной позиции спрайта
    stext3.setCursor(10,55);
    // Определяем цвет текста с прозрачным фоном  
    //stext3.setTextColor(TFT_LIGHTGREY,TFT_BLACK,true); 
    stext3.print("                                                                                          ");

    stext3.setCursor(10,55);
    stext3.print("Чи"); stext3.print(i); stext3.println("!"); stext3.print("   ");

    
    // Выталкмваем спрайт на дисплей с заданной позиции
    // stext3.pushSprite(20, 60);
    // Иммитируем перетекание текста на нижнюю строку
    // delay(200);

    // Выше складываем новое значение
    i++;
    stext3.setCursor(10,10);
    //stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    stext3.print("Чи"); stext3.print(i); stext3.print("!"); stext3.print("   ");
    stext3.pushSprite(20, 60);
}


void setup()
{
  Serial.begin(115200);
  delay(3000); // uncomment if your serial monitor is empty
   
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  
  //tft.loadFont("nasalization48"); // загрузка в память шрифта
  tft.loadFont("HuaweiSans16");     // загрузка в память шрифта
  tft.setCursor(0,0);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK,true);
  //tft.unloadFont();               // выгрузка шрифта из памяти

  Serial.println("\nПриложение запущено!");
  tft.println("\nПриложение запущено!");
 
  // Задаем среднюю глубину цвета и создаем спрайт
  // заданного размера в пикселах в оперативной памяти
  stext3.setColorDepth(8);
  stext3.createSprite(200, 110);
  // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
  stext3.fillSprite(TFT_BLACK);
  // Отключаем перенос текста и по горизонтали и по вертикали 
  stext3.setTextWrap(false, false);
  // Определяем цвет текста с прозрачным фоном  
  stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
  // Загружаем шрифт в память спрайта
  //stext3.loadFont("HuaweiSans16");     // загрузка в память шрифта
  stext3.loadFont("nasalization48");     
  
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(messageReceived);
}

void loop(){}

/*
TFT_BLACK (0, 0, 0) 0x0000
TFT_NAVY  (0, 0, 128) 0x0008
TFT_DARKGREEN (0, 128, 0) 0x0080
TFT_DARKCYAN  (0, 128, 128) 0x0080
TFT_MAROON  (128, 0, 0) 0x8000
TFT_PURPLE  (128, 0, 128) 0x800080
TFT_OLIVE (128, 128, 0) 0x808000
TFT_LIGHTGREY (192, 192, 192) 0xC0C0C0
TFT_DARKGREY  (128, 128, 128) 0x808080
TFT_BLUE  (0, 0, 255) 0x0000FF
TFT_GREEN (0, 255, 0) 0x00FF00
TFT_CYAN  (0, 255, 255) 0x00FFFF
TFT_RED         (255, 0, 0) 0xFF0000
TFT_MAGENTA     (255, 0, 255) 0xFF00FF
TFT_YELLOW      (255, 255, 0) 0xFFFF00
TFT_WHITE       (255, 255, 255) 0xFFFFFF
TFT_ORANGE      (255, 165, 0) 0xFFA500
TFT_GREENYELLOW (173, 255, 47)  0xADFF2F
TFT_PINK        (255, 192, 203) 0xFFC0CB


Параметр в методе setColorDepth() определяет, сколько бит будет использоваться для представления одного пикселя. 
Допустимые значения: 1, 8 или 16. 
1 бит (монохромный режим): экономит память, но сильно ограничивает количество отображаемых цветов.
8 бит: хороший баланс между качеством изображения и расходом памяти.
16 бит: максимальное качество, но требует больше ресурсов.
 
Важный нюанс: метод нужно вызвать до создания самого спрайта с помощью createSprite().
Если сделать это после, поведение программы может быть непредсказуемым. 

*/
 
