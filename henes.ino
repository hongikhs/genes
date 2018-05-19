#include <SoftwareSerial.h>
#define BT_RX 12
#define BT_TX 13
#define M2_DIR 4
#define M2_PWM 5
#define M1_PWM 6
#define M1_DIR 7
#define H_LEFT 871
#define H_RIGHT 119
#define S_LEFT 830
#define S_RIGHT 220
SoftwareSerial bt(BT_RX,BT_TX); // RX, TX
int mode = 1; // 0 for BT, 1 for BABY
int b1,b2,b3,b4;
int pdir,pvel;
int dir,vel;
int handle,ah,steer,handle_pwm;

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
  // put your main code here, to run repeatedly:

  if (mode) {
    //Serial.println("BABY mode");
    handle = analogRead(A0);
    ah = map(handle, H_RIGHT, H_LEFT, S_RIGHT, S_LEFT);
    steer = analogRead(A1);
    handle_pwm = abs(ah-steer);
    if (ah < steer) {
      digitalWrite(M2_DIR,HIGH);
    } else {
      digitalWrite(M2_DIR,LOW);
    }
    analogWrite(M2_PWM,handle_pwm);
    Serial.print(" / handle : ");
    Serial.print(handle);
    Serial.print(" / ah : ");
    Serial.print(ah);
    Serial.print(" / steer : ");
    Serial.print(steer);
    Serial.print(" / PWM : ");
    Serial.println(handle_pwm);
    delay(100);
  } else {
    bt.println("connected");
    while (!bt.available());
    b1=bt.read();
    if (b1==201) {
      while (!bt.available());
      b2=bt.read();
      while (!bt.available());
      b3=bt.read();
      Serial.print("(BT : ");
      Serial.print(b1);
      Serial.print(" ");
      Serial.print(b2);
      Serial.print(" ");
      Serial.print(b3);
      //if (b2<200) dir=b2;
      //if (b3<200) vel=b3;
      //Serial.print("\tdir : ");
      //Serial.print(dir);
      //Serial.print("\tpdir : ");
      //Serial.print(pdir);
      //Serial.print("\tvel : ");
      //Serial.print(vel);
      
    }
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
    Serial.print(" \tah : ");
    Serial.print(ah);
    Serial.print(" \tsteer : ");
    Serial.print(steer);
    Serial.print(" \tPWM : ");    
    Serial.print(handle_pwm);

    if (b3<200) {
      digitalWrite(M1_DIR,LOW);
      analogWrite(M1_PWM,b3);
    } else {
      digitalWrite(M1_DIR,HIGH);
      analogWrite(M1_PWM,b3-100);
    }

  }
  Serial.println(")");
}
