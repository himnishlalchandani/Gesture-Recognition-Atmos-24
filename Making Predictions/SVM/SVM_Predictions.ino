#include <Wire.h>
#include <MPU6050.h>
/*
  #include <Wire.h>: Imports the Wire library, which allows the microcontroller to communicate over I2C 
#include <MPU6050.h>: Imports the MPU6050 library, which contains functions to initialize 
and read data from the MPU6050 sensor (an accelerometer and gyroscope sensor)
*/

MPU6050 mpu; //MPU6050 mpu;: Creates an instance of the MPU6050 sensor, allowing access to sensor functions like initialization and data retrieval.

float weights[] = { 0.5190259938983779, -0.6734625681675306, -0.35391766885335985, 
                    -0.8417342657728066, 0.21518534317613736, -0.2222962189137609 };
float intercept = 3.571519649657513;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.initialize();
}
/*
  Serial.begin(115200);: Initializes serial communication at a baud rate of 115200 bps for outputting predictions to 
  the Serial Monitor.
Wire.begin();: Begins I2C communication, which is needed to communicate with the MPU6050 sensor.
mpu.initialize();: Initializes the MPU6050 sensor, getting it ready to provide data.
  */

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  /*
    int16_t ax, ay, az, gx, gy, gz;: Declares six variables for accelerometer (ax, ay, az) and gyroscope (gx, gy, gz) data.
    int16_t is a 16-bit integer type to match the sensor data format.
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);: Reads raw accelerometer and gyroscope values from the MPU6050 and stores 
them in the respective variables
  */

    float features[] = { static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(az),
                         static_cast<float>(gx), static_cast<float>(gy), static_cast<float>(gz) };
  // conversion to float data type

    int prediction = predict(features);
    
    // Print either "Walking" or "Jogging" based on the prediction
    if (prediction == 1) {
        Serial.println("Jogging");
    } else if (prediction == 0) {
        Serial.println("Walking");
    } else {
        Serial.println("Unknown prediction");
    }

    delay(1000);
}

int predict(float* features) {
    float sum = 0.0;
    for (int i = 0; i < 6; i++) {
        sum += weights[i] * features[i];
    }
    sum += intercept;
  /* 
    float sum = 0.0;: Initializes sum to accumulate the weighted sum of features.
Weighted Sum Calculation:
for (int i = 0; i < 6; i++): Loops through each feature.
sum += weights[i] * features[i];: Multiplies each feature by its corresponding weight and adds it to sum.
sum += intercept;: Adds the intercept to the sum, adjusting the final score.
return (sum > 0) ? 1 : 0;: Returns 1 if sum is greater than 0 (indicating "Jogging") 
  and 0 if sum is less than or equal to 0 (indicating "Walking").
  */

    return (sum > 0) ? 1 : 0;
}
