#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float weights[] = { 0.5190259938983779, -0.6734625681675306, -0.35391766885335985, 
                    -0.8417342657728066, 0.21518534317613736, -0.2222962189137609 };
float intercept = 3.571519649657513;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.initialize();
}

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float features[] = { static_cast<float>(ax), static_cast<float>(ay), static_cast<float>(az),
                         static_cast<float>(gx), static_cast<float>(gy), static_cast<float>(gz) };

    int prediction = predict(features);
    
    Serial.println(prediction); // 0 for walking, 1 for jogging

    delay(1000);
}

int predict(float* features) {
    float sum = 0.0;
    for (int i = 0; i < 6; i++) {
        sum += weights[i] * features[i];
    }
    sum += intercept;

    return (sum > 0) ? 1 : 0;
}
