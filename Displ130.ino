#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  // I2C / TWI
 const uint64_t pipe = 0xF0F1F2F3F4LL; //первый идентификатор передачи данных.
  RF24 radio(9, 10); //указываем пины подключения передатчика.
   float data [3];
   float temper0;
   float temper1;
   float temper2;
void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void text_Prob() {
  u8g_prepare();
  //u8g.setScale();
  u8g.drawStr(0, 0, "temper0");
  u8g.drawStr(60, 0, (temper1));  //60 исправить
                                 //u8g.setScale2x2();
  u8g.drawStr(0, 10, temper2);
  // u8g.undoScale();
  u8g.drawStr(0, 20, "WAV -- 33");
  // u8g.undoScale();
}

uint8_t draw_state = 0;

void draw(void) {
  u8g_prepare();
  text_Prob();
}

void setup(void) {
Serial.begin(9600); //открываем пот обмена данными.
  radio.begin(); //делаем инициализацию.
  delay(2); //ждем реакции модуля передачи данных.
  radio.setChannel(9); // канал для передачи данных (0-127).
  radio.setDataRate(RF24_1MBPS); //указываем на какой скорости будут общаться модули.
  radio.setPALevel(RF24_PA_HIGH); //указываем мощность передатчика. 
 
  radio.openReadingPipe(1,pipe); //Можно открыть все трубы разом.
  radio.startListening(); //включаем приемник и начинаем слушать трубу.
  // dislpl.init();
    // заголовки на дисплей
//   dislpl.backlight();
  // flip screen, if required
  //u8g.setRot180();
#if defined(ARDUINO)
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
#endif
}

void loop(void) {
//---------------------Radio--------------------------
 //float data [3];  //создаем массив из 3-х переменных.
  if (radio.available()){ //проверяем не пришло ли чего в буфер.
  radio.read(&data, sizeof(data)); //читаем массив и указываем сколько пришло.
 //  dislpl.setCursor(0,0);
       // dislpl.print(data[0]);
       // dislpl.setCursor(0,1);
       // dislpl.print(data[1]);
       // dislpl.setCursor(6,1); // нужно 7,1
       // dislpl.print(data[2]);
        temper0 = data[0];
        temper1 = data[1];
        temper2 = data[2];
  /* Serial.println(data[0]); //выводим данные в сериал порт для проверки.
   Serial.println(data[1]); //выводим данные в сериал порт для проверки.
   Serial.println(data[2]); //выводим данные в сериал порт для проверки.
   Serial.println(); //пробел. */
  }
//-----------------------------------------------
  u8g.firstPage();
  // picture loop
  delay(1000);
  do {
    draw();
  } while (u8g.nextPage());

  // increase the state
  draw_state++;
  if (draw_state >= 9 * 8)
    draw_state = 0;

  // rebuild the picture after some delay
  //delay(150);
}
