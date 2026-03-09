
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "STM32_CAN.h"

Adafruit_MCP4725 dac;

#define THR PA1
#define REF PA0
#define RELAY PB8
#define ALERT PC13
#define SIG_1 PB9
#define SIG_2 PB10

#define ECO PB3
#define FWD PB4
#define REV PB5


STM32_CAN Can(CAN1, DEF);


String inputString = "";
bool stringComplete = false;
float targetVolt = 0.0;

static float currentVolt = 0.0;
static bool rampActive = false;
static unsigned long lastStepTime = 0;

int motor_rpm = 0;
int batt_vol = 0;
int batt_cur = 0;
int throttle_pct = 0;

uint8_t eco = 0, boost = 0, reverse = 0;
uint8_t brake_any = 0, motor_error = 0, mcu_error = 0;

static CAN_message_t CAN_RX_msg;
uint32_t ts = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Can.begin();
  Can.setBaudRate(500000);

  dac.begin(0x62);
  dac.setVoltage(0, false);

  pinMode(RELAY, OUTPUT);
  pinMode(ALERT, OUTPUT);
  pinMode(SIG_1, OUTPUT);
  pinMode(SIG_2, OUTPUT);

  pinMode(ECO, INPUT);
  pinMode(FWD, INPUT);
  pinMode(REV, INPUT);

  digitalWrite(ALERT, LOW);

  //Serial.println("System Ready..");
}

void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') stringComplete = true;
    else inputString += c;
  }
}


void rampDAC() {

  if (!rampActive) return;

  if (millis() - lastStepTime >= 1000) {
    lastStepTime = millis();

    uint16_t dacValue = (uint16_t)((currentVolt * 4095.0f) / 5.0f);
    dac.setVoltage(dacValue, false);

    Serial.printf("DAC: %.2fV\n", currentVolt);

    currentVolt += 0.10;     // step increase

    if (currentVolt > targetVolt) {
      rampActive = false;
      Serial.println("Ramp Completed ");
    }
  }
}

void loop() {

  rampDAC();
  if (stringComplete) {

    if (inputString.startsWith("DAC")) {
      inputString.remove(0,3);
      inputString.trim();

      float v = inputString.toFloat();

      if (v >= 0.0 && v <= 4.2) {
        targetVolt = v;
        currentVolt = 0.0;   // restart from zero
        rampActive = true;   // start ramping
        lastStepTime = millis();
        dac.setVoltage(0, false);
        Serial.printf("Ramping to %.2fV...\n", targetVolt);
      }
      else {
        Serial.println("Enter 0.0 to 4.2V only!"); 
      }
    }

    inputString = "";
    stringComplete = false;
  }


  if (Can.read(CAN_RX_msg)) {

    if (CAN_RX_msg.id == 0x14520902 && !CAN_RX_msg.flags.remote) {
      int16_t raw_rpm = (int16_t)((CAN_RX_msg.buf[1] << 8) | CAN_RX_msg.buf[0]);
      uint16_t raw_vol = (CAN_RX_msg.buf[5] << 8) | CAN_RX_msg.buf[4];
      uint16_t raw_current = (CAN_RX_msg.buf[7] << 8) | CAN_RX_msg.buf[6];

      motor_rpm = (int)(raw_rpm * 8.35);
      batt_vol  = (int)(raw_vol * 0.1);
      batt_cur  = (int)(raw_current * 0.01);
    }

    }
  }

  if (millis() - ts > 1000) {
    Serial.printf("*%d,%d,%d,%d,%d,%d,%d,%d,%d,%d#\n",
                  motor_rpm,
                  batt_vol,
                  throttle_pct,
                  batt_cur,
                  eco,
                  boost,
                  reverse,
                  brake_any,
                  motor_error,
                  mcu_error);
    ts = millis();
  }
}
