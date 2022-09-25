#include <SPI.h>
#include "config.h"

// using two incompatible SPI devices, A and B. Incompatible means that they need different SPI_MODE

const int slaveAPin = 21;

// set up the speed, data order and data mode
unsigned long Valor_actual;
unsigned long Valor_anterior;
unsigned long millis();
const int tiempo = 15000;
int enviar = 0;

SPISettings settingsA(4000000, MSBFIRST, SPI_MODE1);
AdafruitIO_Feed *seconds_1 = io.feed("seconds");
AdafruitIO_Feed *minutes_1 = io.feed("minutes");
AdafruitIO_Feed *hours_1 = io.feed("hours");
AdafruitIO_Feed *PUSH1_1 = io.feed("PUSH1");
AdafruitIO_Feed *PUSH2_1 = io.feed("PUSH2");
AdafruitIO_Feed *POT1_1 = io.feed("POT1");
AdafruitIO_Feed *temperatura_1 = io.feed("temperatura");

void setup() {

  // set the Slave Select Pins as outputs:

  pinMode (slaveAPin, OUTPUT);

  // initialize SPI:
  SPI.begin();
  Serial.begin(115200);

  Serial.print("Connecting to WiFi");
  WiFi.begin("UVG", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // wait for serial monitor to open
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
}

uint8_t seconds, minutes, hours, POT1, PUSH1, PUSH2, temperatura;

void loop() {
  io.run();
  // read three bytes from device A

  SPI.beginTransaction(settingsA);
  
  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  seconds = SPI.transfer(1);
  seconds = seconds/2;
  delay(10);
 
  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  minutes = SPI.transfer(2);
  minutes = minutes/2;
  delay(10);

  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  hours = SPI.transfer(3);
  hours = hours/2;
  delay(10);

  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  POT1 = SPI.transfer(4);
  POT1 = POT1/2;
  delay(10);

  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  PUSH1 = SPI.transfer(5);
  PUSH1 = PUSH1/2;
  delay(10);

  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  PUSH2 = SPI.transfer(6);
  PUSH2 = PUSH2/2;
  delay(10);

  digitalWrite (slaveAPin, HIGH);
  delay(10);
  digitalWrite (slaveAPin, LOW);
  delay(10);
  temperatura = SPI.transfer(0);
  temperatura = temperatura/2;
  delay(10);
  
  SPI.endTransaction();


  Valor_actual = millis();
  if (Valor_actual - Valor_anterior >= tiempo) {
    Valor_anterior = Valor_actual;
    enviar = 1;
  }

  if (enviar == 1) {
    Serial.print("sending seconds -> ");
    Serial.println(seconds);
    seconds_1->save(seconds);
    Serial.print("sending minutes -> ");
    Serial.println(minutes);
    minutes_1->save(minutes);
    Serial.print("sending hours -> ");
    Serial.println(hours);
    hours_1->save(hours);
    Serial.print("sending PUSH1 -> ");
    Serial.println(PUSH1);
    PUSH1_1->save(PUSH1);
    Serial.print("sending PUSH2 -> ");
    Serial.println(PUSH2);
    PUSH2_1->save(PUSH2);
    Serial.print("sending POT1 -> ");
    Serial.println(POT1);
    POT1_1->save(POT1);
    Serial.print("sending temperatura -> ");
    Serial.println(temperatura);
    temperatura_1->save(temperatura);
    
    enviar = 0;
  }
}
