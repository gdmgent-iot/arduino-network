#include <Servo.h>

Servo marcel;
int servoPin = 10;

void setup() {
  // put your setup code here, to run once:
  marcel.attach(servoPin);
}

void loop() {

  for(int pos=0; pos <=180; pos++) {
    marcel.write(pos);
    delay(8);
  }

  for(int pos=180; pos >= 0; pos--) {
    marcel.write(pos);
    delay(8);
  }

}
