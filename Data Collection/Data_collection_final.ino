#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
/* 
1. Includes the Adafruit MPU6050 library, which provides functions to interact with the MPU6050 sensor. 
This library allows reading accelerometer, gyroscope, and temperature data from the sensor 
2. Includes the Adafruit Sensor library, which provides a unified interface for sensor data, 
making it easier to read data in a standardized format
3. Includes the Wire library, which allows communication over I2C (Inter-Integrated Circuit) protocol.
The MPU6050 sensor communicates with the NodeMCU through I2C, so this library is essential for data transmission*/
Adafruit_MPU6050 mpu;

unsigned long startTime; // Declares a variable to store the start time of data collection. It will be set when the sensor begins collecting data
int sampleRate = 100; //Sets the interval between data samples in milliseconds. Here, 100 milliseconds means data will be read approximately 10 times per second.
int duration = 60000; // Defines how long data collection will last, in milliseconds. Here, 60000 milliseconds (1 minute) is the total duration for data collection.
bool isRunning = true;
int i = 0;
void setup() { // function creation
  Serial.begin(115200);
  /* known as baud rate
  The baud rate is the speed at which data is transmitted over a communication channel, 
  measured in bits per second (bps). In the context of serial communication, 
  like between a microcontroller (such as a NodeMCU or Arduino) and a computer, the baud rate determines
  how fast data bits are sent and received.
  For example, setting Serial.begin(9600); in Arduino code means the microcontroller will communicate with the
  computer at a speed of 9600 bits per second.*/
  Wire.begin(D2, D1);  // SDA = D2, SCL = D1
  delay(10); // Pauses the execution for 10 milliseconds, allowing the I2C communication to stabilize before continuing

  Serial.println("Initializing MPU6050...");

  if (!mpu.begin()) { // basically condition for checking proper connections
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
/*
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);: Sets the accelerometer’s sensitivity range to ±8g (where "g" refers to gravitational force).
mpu.setGyroRange(MPU6050_RANGE_500_DEG);: Sets the gyroscope’s sensitivity to ±500 degrees per second.
  */
  Serial.println("ax,ay,az,gx,gy,gz,temperature");

  startTime = millis();
}

void loop() {
  if (!isRunning) {
    return;
  }

  if (millis() - startTime >= duration) {  // compares the elapsed time
    Serial.println("Data collection complete.");
    isRunning = false;
    return;
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
/* sensors_event_t a, g, temp;: Declares three sensors_event_t objects to hold sensor readings for acceleration (a), gyroscope (g), and temperature (temp).
mpu.getEvent(&a, &g, &temp);: Calls getEvent() to read data from the sensor and stores it in the a, g, and temp variables
  */
  Serial.print(i-45); Serial.print(",");
  Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print(g.gyro.x); Serial.print(",");
  Serial.print(g.gyro.y); Serial.print(",");
  Serial.print(g.gyro.z); Serial.print(",");
  Serial.println("1");
  // all the above lines are for printing acceleration and rotational motion 
  i=i+1;

  delay(sampleRate);
}
