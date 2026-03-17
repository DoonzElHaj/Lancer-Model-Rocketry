// WARNING: SOME PARTS OF THIS CODE ARE AI GENERATED, LOGIC IS NOT TESTED.

#include <Wire.h>
#include <Servo.h>

// ---------------- SERVOS ----------------
Servo elevL;
Servo elevR;
Servo aileron;

int elevL_pin = 9;
int elevR_pin = 10;
int ail_pin   = 11;

// Neutral positions (adjust these)
int elev_neutral = 90;
int ail_neutral  = 90;

// ---------------- MPU6050 ----------------
const int MPU = 0x68;

void setup() {
  Serial.begin(9600);

  // Servos
  elevL.attach(elevL_pin);
  elevR.attach(elevR_pin);
  aileron.attach(ail_pin);

  elevL.write(elev_neutral);
  elevR.write(elev_neutral);
  aileron.write(ail_neutral);

  // MPU6050 init
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);
}

void loop() {
  int16_t ax, ay, az;

  // Read accelerometer
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  // Compute simple angles (no filtering)
  float pitch = atan2(ax, sqrt((long)ay * ay + (long)az * az)) * 57.3;
  float roll  = atan2(ay, sqrt((long)ax * ax + (long)az * az)) * 57.3;

  // Map angles to servo movement
  int elev_cmd = elev_neutral + pitch;   // pitch controls elevators
  int ail_cmd  = ail_neutral + roll;     // roll controls aileron

  // Limit servo range
  elev_cmd = constrain(elev_cmd, 0, 180);
  ail_cmd  = constrain(ail_cmd, 0, 180);

  // Write to servos
  elevL.write(elev_cmd);
  elevR.write(180 - elev_cmd);  // opposite direction
  aileron.write(ail_cmd);

  delay(20);
}