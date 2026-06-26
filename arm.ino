#include <Servo.h>

Servo m1;
Servo m2;
Servo m3;
Servo m4;

int c1 = 90;
int c2 = 90;
int c3 = 90;
int c4 = 20;

int g1 = 90;
int g2 = 90;
int g3 = 90;
int g4 = 20;

int trg = 7;
int ech = 8;

unsigned long l_ms = 0;

long r_dst() {
  digitalWrite(trg, LOW);
  delayMicroseconds(2);
  digitalWrite(trg, HIGH);
  delayMicroseconds(10);
  digitalWrite(trg, LOW);
  long t = pulseIn(ech, HIGH);
  long d = t * 0.034 / 2;
  return d;
}

void setup() {
  Serial.begin(9600);
  
  m1.attach(3);
  m2.attach(5);
  m3.attach(6);
  m4.attach(9);
  
  pinMode(trg, OUTPUT);
  pinMode(ech, INPUT);
  
  m1.write(c1);
  m2.write(c2);
  m3.write(c3);
  m4.write(c4);
}

void loop() {
  if (Serial.available() > 0) {
    char hdr = Serial.read();
    if (hdr == '#') {
      g1 = Serial.parseInt();
      g2 = Serial.parseInt();
      g3 = Serial.parseInt();
      g4 = Serial.parseInt();
    }
  }

  long dist = r_dst();

  if (dist > 0 && dist < 8) {
    if (g4 < 80) {
      g4 = 90; 
    }
  }

  unsigned long c_ms = millis();
  if (c_ms - l_ms >= 25) {
    l_ms = c_ms;
    
    if (c1 < g1) c1 += 1;
    else if (c1 > g1) c1 -= 1;
    
    if (c2 < g2) c2 += 1;
    else if (c2 > g2) c2 -= 1;
    
    if (c3 < g3) c3 += 1;
    else if (c3 > g3) c3 -= 1;
    
    if (c4 < g4) c4 += 2;
    else if (c4 > g4) c4 -= 2;
    
    m1.write(c1);
    m2.write(c2);
    m3.write(c3);
    m4.write(c4);
  }
}