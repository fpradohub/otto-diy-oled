

#include <Servo.h>

Servo servo1,servo2,servo3,servo4;  

int pos = 90;    // variable to store the servo position

void setup() {
  servo1.attach(4);  // attaches the servo on pin 4 to the servo object
  servo2.attach(5);  // attaches the servo on pin 5 to the servo object
  servo3.attach(6);  // attaches the servo on pin 6 to the servo object
  servo4.attach(7);  // attaches the servo on pin 7 to the servo object


}

void loop() {
  
    servo1.write(pos);              // tell servo to go to position in variable 'pos'
    servo2.write(pos);
    servo3.write(pos);         
    servo4.write(pos);

                  
}
