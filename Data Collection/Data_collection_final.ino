#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
// above mentoned are the libraries needed for data collection
Adafruit_MPU6050 mpu; // an instance of mpu6050 sensor

unsigned long startTime;
int sampleRate = 100;
int duration = 60000;
bool isRunning = true;
int i = 0;
// above mentioned all global variables in millisecond
void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);  // SDA = D2, SCL = D1
  delay(10);
  /*
    Serial.begin(115200);: Starts serial communication at 115200 baud for quick data transfer.
Wire.begin(D2, D1);: Sets up I2C communication on NodeMCU pins D2 (SDA) and D1 (SCL).
delay(10);: A short delay to allow for setup completion
  */
  /* The baud rate is the speed at which data is transmitted over a communication channel, such as a serial connection between a microcontroller (like an Arduino or NodeMCU) and a computer. 
  It's measured in bits per second (bps) and indicates how many bits of data are sent or received per second*/

  Serial.println("Initializing MPU6050...");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
 /*
   setAccelerometerRange(MPU6050_RANGE_8_G);: Sets the accelerometer sensitivity to ±8g.
setGyroRange(MPU6050_RANGE_500_DEG);: Sets the gyroscope sensitivity to ±500 degrees per second.
setFilterBandwidth(MPU6050_BAND_5_HZ);: Sets a low-pass filter to reduce noise
  */
  // these are all some of the parameters defined in order to move forward for predictions
  Serial.println("ax,ay,az,gx,gy,gz,temperature");

  startTime = millis(); //startTime = millis();: Records the current time as the starting point for data collection.
}

void loop() {
  if (!isRunning) {
    return;
  }

  if (millis() - startTime >= duration) { // calculates the elapsed time
    Serial.println("Data collection complete.");
    isRunning = false;
    return;
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  // mpu.getEvent(&a, &g, &temp);: Fetches data from the sensor and stores it in the variables.

  Serial.print(i-45); Serial.print(",");
  Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print(g.gyro.x); Serial.print(",");
  Serial.print(g.gyro.y); Serial.print(",");
  Serial.print(g.gyro.z); Serial.print(",");
  Serial.println("1");
  i=i+1;
  // above code is for printing

  delay(sampleRate); //  waits for 100ms delay between each data input taken
}
