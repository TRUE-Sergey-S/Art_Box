#include <FastLED.h>
#define NUM_LEDS 300
#define DATA_PIN 5
#define LED_TYPE WS2812B
#define DELAY_TIME 1
CRGB leds[NUM_LEDS];
short randNumber;
short randomIncteaseLight;
asd
int sendData[3];
// [0] - тип устройста
// [1] - код
// [2] - проверочное число ([0]+[1])
#include <EEPROM.h>
#include <SPI.h>                            // Подключаем библиотеку для работы с шиной SPI.
#include <nRF24L01.h>                       // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                           // библиотека для работы с nRF24L01+ https://yadi.sk/d/nX_akvzEusyGz
RF24 radio(9, 10);                          // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN).
byte pipe;

void SendBoxStatus(int statusWalue) {
  //********** коды *************
  //1 Тихий режим
  //2 Огненный
  //3 Кислотный
  //4 Електрический

  sendData[1] = statusWalue;
  sendData[2] = sendData[0] + sendData[1];
  if (radio.write(&sendData, sizeof(sendData))) {
    Serial.print("Отправка прошла успешно, значение: ");
    Serial.print(sendData[0]);
    Serial.print(sendData[1]);
    Serial.print(sendData[2]);
    Serial.println("                                                ");
  } else {
    Serial.print("Ошибка отправки, значение: ");
    Serial.println(sendData[1]);
  }
}
void printBoxStatus() {
  switch (EEPROM.read(0)) {
    case 1:
      SendBoxStatus(1);
      break;
    case 2:
      SendBoxStatus(2);
      break;
    case 3:
      SendBoxStatus(3);
      break;
    case 4:
      SendBoxStatus(4);
      break;
    case 5:
      SendBoxStatus(5);
      break;
    case 6:
      SendBoxStatus(6);
      break;
  }
};

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);

  sendData[0] = 1;//1-контейнер для артефактов
  radio.begin();                            // Инициируем работу nRF24L01+.
  radio.setChannel(5);                      // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц.(на одном канале можно, одновременно иметь "1 передатчик-рот-труба" или "6 приёмников-ушей-трубы"
  radio.setDataRate     (RF24_250KBPS);     // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS). При скорости 2 Мб/с, задействуются сразу два канала (выбранный и следующий за ним). (самая дальнобойная 250KBPS, но скорость меньше)
  radio.setPALevel      (RF24_PA_HIGH);     // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm).
  radio.openWritingPipe (0xAABBCCDD11LL);
  //radio.openWritingPipe (0xAABBCCDD22LL);
  //radio.openWritingPipe (0xAABBCCDD33LL);
  //radio.openWritingPipe (0xAABBCCDD44LL);
  //radio.openWritingPipe (0xAABBCCDD55LL);
  //radio.openWritingPipe (0xAABBCCDD66LL);

  radio.openReadingPipe (1, 0xAABBCCDD12LL);
  Serial.print("Статус контейнера: ");
  Serial.println(EEPROM.read(0));
  printBoxStatus();
  Serial.println("Запущен");
  delay(3000);
}

unsigned long timing;
void loop() {
  RGBcontrol(EEPROM.read(0));
  printBoxStatus();
  if (millis() - timing > 300) {
    Serial.println("Слушаю радио эфир");
    timing = millis();
    delay (20);
    radio.startListening  ();
    delay (50);
    if (radio.available(&pipe)) {
      Serial.print("принял что то: ");
      byte radioInput;
      radio.read(&radioInput, sizeof(radioInput));
      Serial.println(radioInput);
      EEPROM.write(0, radioInput);
    }
    delay (20);
    radio.stopListening();
    Serial.println("Не слушаю радио эфир");
    delay (50);
  }
}

void RGBcontrol(int mode) {
  // 1 - выключить
  // 2 - Огонь
  // 3 - Кислота
  // 4 - Електро
  switch (mode)
  {
    case 1: // выключить
      FastLED.clear();
      FastLED.show();
      break;
    case 2: // огонь
      FastLED.clear();
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 1, 0);
      }
      for (int i = 0; i < NUM_LEDS; i++) {
        randNumber = random(0, 300);
        if (randNumber == 0 ) {
          for (int x = 0; x < NUM_LEDS; x++) {
            leds[x].setRGB(0, 255, 0);
          }
          FastLED.show();
          delay(DELAY_TIME * 20);
          for (int x = 0; x < NUM_LEDS; x++) {
            leds[x].setRGB(0, 100, 0);
          }
          FastLED.show();
          delay(DELAY_TIME * 20);
          for (int x = 0; x < NUM_LEDS; x++) {
            leds[x].setRGB(0, 50, 0);
          }
          FastLED.show();
          delay(DELAY_TIME * 20);
          for (int x = 0; x < NUM_LEDS; x++) {
            leds[x].setRGB(0, 1, 0);
          }
          FastLED.show();
        }
        leds[i].setRGB(50, 255, 0);
        if (i >= 1) {
          leds[i - 1].setRGB(30, 200, 0);
        }
        if (i >= 2) {
          leds[i - 2].setRGB(20, 150, 0);
        }
        if (i >= 3) {
          leds[i - 3].setRGB(10, 100, 0);
        }
        if (i >= 4) {
          leds[i - 4].setRGB(5, 50, 0);
        }
        if (i >= 5) {
          leds[i - 5].setRGB(2, 40, 0);
        }
        if (i >= 6) {
          leds[i - 6].setRGB(1, 30, 0);
        }
        if (i >= 7) {
          leds[i - 7].setRGB(0, 20, 0);
        }
        if (i >= 8) {
          leds[i - 8].setRGB(0, 15, 0);
        }
        if (i >= 9) {
          leds[i - 9].setRGB(0, 10, 0);
        }
        if (i >= 10) {
          leds[i - 10].setRGB(0, 5, 0);
        }
        if (i >= 11) {
          leds[i - 10].setRGB(0, 2, 0);
        }
        if (i >= 12) {
          leds[i - 10].setRGB(0, 1, 0);
        }
        FastLED.show();
        delay(DELAY_TIME * 10);
      }
      break;
    case 3: // Кислота
      FastLED.clear();
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(1, 0, 0);
      }
      FastLED.show();
      randNumber = random(0 + 10, NUM_LEDS - 10);
      for (int i = 10; i <= 255; i++) {
        leds[randNumber].setRGB(i, 0, 0);
        for (int x = 1; x <= 10; x++) {
          if (i > 10) {
            leds[randNumber + x].setRGB(i / (x * 3), 0, 0);
          }
        }
        for (int x = 1; x <= 10; x++) {
          if (i > 10) {
            leds[randNumber - x].setRGB(i / (x * 3), 0, 0);
          }
        }
        if (i == 255) {
          leds[randNumber - 5].setRGB(255, 0, 0);
          leds[randNumber - 4].setRGB(255, 5, 5);
          leds[randNumber - 3].setRGB(255, 10, 10);
          leds[randNumber - 2].setRGB(255, 20, 20);
          leds[randNumber - 1].setRGB(255, 50, 50);
          leds[randNumber].setRGB(255, 255, 255);
          leds[randNumber + 1].setRGB(255, 50, 50);
          leds[randNumber + 2].setRGB(255, 20, 20);
          leds[randNumber + 3].setRGB(255, 10, 10);
          leds[randNumber + 5].setRGB(255, 5, 5);
          leds[randNumber + 5].setRGB(255, 0, 0);
          FastLED.show();
          delay(DELAY_TIME * 50);
        }
        FastLED.show();
        delay(DELAY_TIME * 5);
      }
      break;
    case 4: // електро
      FastLED.clear();
      randNumber = random(11, NUM_LEDS - 11);
      for (int i = randNumber - 10; i <= randNumber + 10; i++) {
        leds[i].setRGB(255, 255, 255);
      }
      FastLED.show();
      delay(DELAY_TIME * 50);
      for (int i = randNumber-10; i <= randNumber+10; i++) {
        leds[i].setRGB(0, 0, 1);
      }
      FastLED.show();
      for (int i = randNumber; i <= NUM_LEDS; i++) {
        leds[i].setRGB(0, 0, 255);
        if (i >= 2) {
          leds[i - 1].setRGB(0, 0, 255);
        }
        if (i >= 3) {
          randomIncteaseLight = random(200, 255);
          leds[i - 2].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 4) {
          randomIncteaseLight = random(150, 255);
          leds[i - 3].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 5) {
          randomIncteaseLight = random(100, 255);
          leds[i - 4].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 6) {
          randomIncteaseLight = random(50, 255);
          leds[i - 5].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 7) {
          randomIncteaseLight = random(40, 255);
          leds[i - 6].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 8) {
          randomIncteaseLight = random(30, 255);
          leds[i - 7].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 9) {
          randomIncteaseLight = random(20, 255);
          leds[i - 8].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 10) {
          randomIncteaseLight = random(10, 255);
          leds[i - 9].setRGB(0, 0, randomIncteaseLight);
        }
        if (i >= 11) {
          randomIncteaseLight = random(1, 10);
          leds[i - 10].setRGB(0, 0, randomIncteaseLight);
        }
        FastLED.show();
        delay(DELAY_TIME * 1);
        randomIncteaseLight = random(0, 255);
        if (randomIncteaseLight <= 10) {
          leds[i - 3].setRGB(255, 255, 255);
          leds[i - 2].setRGB(255, 255, 255);
          leds[i - 1].setRGB(255, 255, 255);
          leds[i].setRGB(255, 255, 255);
          leds[i + 1].setRGB(255, 255, 255);
          leds[i + 2].setRGB(255, 255, 255);
          leds[i + 3].setRGB(255, 255, 255);
          FastLED.show();
          delay(DELAY_TIME * 5); +
          leds[i - 5].setRGB(0, 0, 1);
          leds[i - 4].setRGB(0, 0, 1);
          leds[i - 3].setRGB(0, 0, 1);
          leds[i - 2].setRGB(0, 0, 1);
          leds[i - 1].setRGB(0, 0, 1);
          leds[i].setRGB(0, 0, 1);
          leds[i + 1].setRGB(0, 0, 1);
          leds[i + 2].setRGB(0, 0, 1);
          leds[i + 3].setRGB(0, 0, 1);
          leds[i + 4].setRGB(0, 0, 1);
          leds[i + 5].setRGB(0, 0, 1);
        }
        if (randomIncteaseLight >= 245) {
          break;
        }
      }
      break;
  }
}
