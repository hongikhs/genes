#include <SoftwareSerial.h>
#define BT_RX 12
#define BT_TX 13
#define M2_DIR 4  //handle
#define M2_PWM 5
#define M1_PWM 6  //move
#define M1_DIR 7
#define H_LEFT 871
#define H_RIGHT 119
#define S_LEFT 830
#define S_RIGHT 220
#define BREAK_H 630
#define BREAK_L 0
#define ACCEL_H 858
#define ACCEL_L 175
SoftwareSerial bt(BT_RX,BT_TX); // RX, TX
int drive_mode = 0; // 0 for BT, 1 for BABY
int b1,b2,b3,b4;
int pdir,pvel;
int dir,vel;
int handle,steer,handle_pwm,pd_accel,pd_break;
int aa,ab,ah;
int move_pwm = 0;

void self_drive() {
  handle = analogRead(A0);
  steer = analogRead(A1);
  pd_accel = analogRead(A3);
  pd_break = analogRead(A2);
  aa = map(pd_accel, ACCEL_H, ACCEL_L, 200, 0);
  ab = map(pd_break, BREAK_H, BREAK_L, 200, 0);
  ah = map(handle, H_RIGHT, H_LEFT, S_RIGHT, S_LEFT);
  handle_pwm = abs(ah-steer);
  if (ah < steer) {
    digitalWrite(M2_DIR,HIGH);
  } else {
    digitalWrite(M2_DIR,LOW);
  }
  analogWrite(M2_PWM,handle_pwm);
  move_pwm = aa - ab;
  move_pwm = constrain(move_pwm,0,200);
  digitalWrite(M1_DIR,LOW);
  analogWrite(M1_PWM,move_pwm);
  //Serial.print("handle : ");
  //Serial.print(handle);
  //Serial.print(" / ah : ");
  //Serial.print(ah);
  //Serial.print(" / steer : ");
  //Serial.print(steer);
  //Serial.print(" / H_PWM : ");
  //Serial.print(handle_pwm);
  //Serial.print(" / accel : ");
  //Serial.print(pd_accel);
  //Serial.print(" / break : ");
  //Serial.print(pd_break);
  //Serial.print(" / M_PWM : ");
  //Serial.println(move_pwm);
  //Serial.println(")");
  delay(50);
  //if (bt.read()==201) drive_mode = 0; //bluetooth control mode
}

void bt_control() {
  while (!bt.available());
  b1=bt.read();
  if (b1>=251&&b1<=253) {
    while (!bt.available());
    b2=bt.read();
    while (!bt.available());
    b3=bt.read();
    Serial.print("BT: ");
    Serial.print(b1);
    Serial.print(",");
    Serial.print(b2);
    Serial.print(",");
    Serial.print(b3);

    ////////// FRONT WHEEL - DIRECTION //////////
    
    ah = map(b2, 180, 0, S_RIGHT, S_LEFT);
    steer = analogRead(A1);
    handle_pwm = abs(ah-steer);
    if (ah < steer) {
      digitalWrite(M2_DIR,HIGH);
    } else {
      digitalWrite(M2_DIR,LOW);
    }
    if (handle_pwm<10) handle_pwm=0;
    analogWrite(M2_PWM,handle_pwm);
    //Serial.print(" \tah : ");
    //Serial.print(ah);
    //Serial.print(" \tsteer : ");
    //Serial.print(steer);
    //Serial.print(" \tPWM : ");    
    //Serial.print(handle_pwm);

    ////////// REAR WHEEL - MOVE //////////

    if (b1==251) digitalWrite(M1_DIR,LOW);        // forward
    else if (b1==253) digitalWrite(M1_DIR,HIGH);  // backward

    if (b3==0) {  // break
      while (move_pwm >= 10) {
        move_pwm /= 2;
        Serial.println(move_pwm);
        delay(100);
      }
      move_pwm = 0;
      //Serial.println(move_pwm);
    }
    // increase
    else if (b3>move_pwm) move_pwm = constrain(move_pwm+4,50,b3);
    // decrease
    else if (b3<move_pwm) move_pwm = constrain(move_pwm-4,b3,250);
    
    analogWrite(M1_PWM,move_pwm);
    Serial.print(" -> ");
    Serial.print(move_pwm);
    Serial.println();
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(M1_DIR,OUTPUT);
  pinMode(M2_DIR,OUTPUT);
  Serial.begin(9600);
  bt.begin(9600);
  pdir=90;
  pvel=100;
}

void loop() {
  if (drive_mode) {
    self_drive();
  } else {
    bt_control();
  }
}
