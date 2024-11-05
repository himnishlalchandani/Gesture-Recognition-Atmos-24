#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

unsigned long startTime;
int sampleRate = 100;  // 100 ms delay for 10 readings per second
int duration = 60000;  // Total data collection time: 1 minute (60000 ms)
bool isRunning = true; // Variable to control when to stop
int i = 0;
void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);  // SDA = D2, SCL = D1
  delay(10);

  Serial.println("Initializing MPU6050...");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }
  Serial.println("MPU6050 Found!");

  // Setting ranges
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  // Print headers for CSV
  Serial.println("ax,ay,az,gx,gy,gz,temperature");

  startTime = millis();  // Start the timer
}

void loop() {
  if (!isRunning) {
    return; // Stop collecting data once duration is reached
  }

  if (millis() - startTime >= duration) {
    Serial.println("Data collection complete.");
    isRunning = false; // Stop further readings
    return;
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  // Print data in CSV format
  Serial.print(i-45); Serial.print(",");
  Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print(g.gyro.x); Serial.print(",");
  Serial.print(g.gyro.y); Serial.print(",");
  Serial.print(g.gyro.z); Serial.print(",");
  Serial.println("1");
  i=i+1;

  delay(sampleRate);
}
