#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Weights and intercepts for each class
float weights[4][6] = {
    {-0.04578527, 0.24942555, 0.02602638, 0.02995652, -0.12145042, -0.05982817}, // Class 0 (Walking)
    {0.16895413, -0.02775622, -0.0152711, -0.36518198, 0.1059486, -0.1250134},   // Class 1 (Jogging)
    {-0.07200122, 0.0750543, 0.00995641, 0.06958562, 0.05638641, 0.02940132},    // Class 2 (Standing)
};

float intercepts[4] = {
    -2.5444826636403066,   // Class 0 (Walking)
    -1.035210317330788,    // Class 1 (Jogging)
    -0.7581600497274124,   // Class 2 (Standing)
};

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
    
    // Print the predicted activity
    if (prediction == 0) {
        Serial.println("Walking");
    } else if (prediction == 1) {
        Serial.println("Jogging");
    } else if (prediction == 2) {
        Serial.println("Standing");
    }else {
        Serial.println("Unknown prediction");
    }

    delay(1000);
}

int predict(float* features) {
    float max_score = -INFINITY;
    int best_class = -1;

    // Compute the score for each class
    for (int i = 0; i < 3; i++) {
        float score = intercepts[i];
        for (int j = 0; j < 6; j++) {
            score += weights[i][j] * features[j];
        }

        // Keep track of the class with the highest score
        if (score > max_score) {
            max_score = score;
            best_class = i;
        }
    }

    return best_class;
}