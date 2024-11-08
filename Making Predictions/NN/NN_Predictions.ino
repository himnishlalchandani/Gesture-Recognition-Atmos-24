#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Layer 1 weights and biases
float layer1_weights[6][4] = {
    { 0.12988369,  0.8510138,  -0.3102076, -0.21291123 },
    { -0.7034654,  -0.3657693,  -0.58647233,  0.6129328 },
    { 0.92085254, -0.8431001,   0.47424665,  0.5535118 },
    { -1.870355,   1.5002494,  -0.49784935,  2.0672975 },
    { -0.2724311,  0.69231534, -0.20959646, -0.5452295 },
    { 0.3816481,   0.47190323, -0.7373124,   1.3051966 }
};
float layer1_biases[4] = { 0.7735471,  0.27232274, -0.20416549, -0.9169817 };

// Layer 2 weights and biases
float layer2_weights[4][4] = {
    { -0.5081072,  -0.30487055,  1.5152127,   0.24221998 },
    { -0.4311063,  -1.950772,    1.0172085,   -0.3962456 },
    { -0.48055515,  0.36365741, -0.29425088,  0.22027226 },
    { -0.2509939,   0.9354071,  -0.13977252, -0.39096758 }
};
float layer2_biases[4] = { 0.0, -0.19516565,  1.0911125,  -0.49834856 };

// Output layer weights and biases
float output_weights[4] = { 0.67689717, -1.3130871,  1.3608875, -0.32621548 };
float output_bias = 0.9523207;

// Activation functions
float relu(float x) { return x > 0 ? x : 0; }
float sigmoid(float x) { return 1.0 / (1.0 + exp(-x)); }

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.initialize();
}

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float input[] = { (float)ax, (float)ay, (float)az, (float)gx, (float)gy, (float)gz };

    int prediction = predict(input);
    if (prediction == 1) {
        Serial.println("Jogging");
    } else {
        Serial.println("Walking");
    }

    delay(1000);
}

int predict(float* input) {
    // Layer 1 forward propagation
    float layer1_output[4];
    for (int i = 0; i < 4; i++) {
        layer1_output[i] = layer1_biases[i];
        for (int j = 0; j < 6; j++) {
            layer1_output[i] += input[j] * layer1_weights[j][i];
        }
        layer1_output[i] = relu(layer1_output[i]);  // ReLU activation
    }

    // Layer 2 forward propagation
    float layer2_output[4];
    for (int i = 0; i < 4; i++) {
        layer2_output[i] = layer2_biases[i];
        for (int j = 0; j < 4; j++) {
            layer2_output[i] += layer1_output[j] * layer2_weights[j][i];
        }
        layer2_output[i] = relu(layer2_output[i]);  // ReLU activation
    }

    // Output layer forward propagation
    float output = output_bias;
    for (int i = 0; i < 4; i++) {
        output += layer2_output[i] * output_weights[i];
    }
    output = sigmoid(output);  // Sigmoid activation for binary classification

    // Return 1 if Jogging, 0 if Walking based on threshold
    return output > 0.5 ? 1 : 0;
}
