#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Layer 1 weights and biases
float layer1_weights[6][8] = {
    { 0.78462327, -0.32597342,  0.2554183,   0.19654053, 0.06677119, 1.0981591,   0.93531567, -0.25750238 },
    { 0.08113842, -0.50687385,  0.04052312, -0.6097115,  -0.16283128, -0.72001106, -0.4564029,   0.3560051  },
    { 0.4100445,   0.27722964,  0.12634094,  0.76113826, -0.30487213, -0.93777573, 0.9929625,   0.06992777 },
    { 0.13458923, -0.28036457,  1.5574461,  -1.8544766,  -2.6837277,  2.5052087,   0.2707883,  -0.18374771 },
    {-0.9817152,   0.47101292,  0.861611,   -1.0375439,   0.25400653, 0.5143082,  -0.86608696, -0.64464706 },
    { 0.03936161,  0.3658349,  -2.0587544,  -0.17329265, -0.7381291, -0.19620721, 1.8978561,   2.2617548  }
};
float layer1_biases[8] = { -0.29214087, -0.106714, 0.5936492, -0.11158045, 1.5659496, 1.3908523, -0.6662114, -0.40597162 };

// Layer 2 weights and biases
float layer2_weights[8][8] = {
    {-0.5440013,  -0.9169726,  -0.2180752, -0.09622395, -0.65456206,  0.16186525, -0.18821049, -0.5356675 },
    {-0.66636544, -0.7983158,  -0.0645978,  0.13822736,  0.06322987,  0.06204018, -0.5261331,   0.35536867 },
    {-0.26927212, -0.0507001,   0.56865275, -0.6529656, -0.11507711, -1.1650108,  -0.53584886, -0.22027718 },
    {-0.67375135, -0.11011217,  0.44583952, -0.14808337, 1.2879108,  -0.047951,   -0.35618314, -0.5864505 },
    {-0.40698418, -0.626078,    1.1566278,   0.2673591,  2.0039597,   0.8159956,  -0.8242271,  -0.37319946 },
    {-0.56986815, -0.8892082,   0.6213091,  -0.24485621, 2.197023,    0.5030093,  -0.08692926,  0.39200726 },
    { 1.1452496,  -0.02376877,  1.6174401,  -0.00858498, 0.81892115, -0.07736612, -0.10714674,  0.11734617 },
    { 0.63317084,  0.82827383, -0.40774673, -0.4628377, -0.19549711,  0.77446485, -0.23334427, -0.10871518 }
};
float layer2_biases[8] = {-0.23306401, 0.21281181, 0.47227085, -0.5410545, 1.2502798, -0.739475, -0.14206547, -0.24632888 };

// Output layer weights and biases
float output_weights[8][3] = {
    { 0.12521179, -0.065676339, -2.12285829 },
    { 0.73852086, -0.71121919, -1.56009793 },
    { 0.38219431,  0.094565667, -3.34283638 },
    { 0.114301495, -0.34922516,  0.204273894 },
    {-0.3375009,   1.30974257,  -4.69843292 },
    { 0.95620948, -0.072357468, -0.331571728 },
    {-0.270101458, -0.0038105636,  0.496816218 },
    { 0.611204565, -0.266685516, -0.695840359 }
};
float output_biases[3] = {-0.5426275, -4.3591285, 4.9203157};

// Activation functions
float relu(float x) { return x > 0 ? x : 0; }
float softmax(float* x, int index, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += exp(x[i]);
    }
    return exp(x[index]) / sum;
}

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
    if (prediction == 0) {
        Serial.println("Walking");
    } else if (prediction == 1) {
        Serial.println("Jogging");
    } else {
        Serial.println("Standing");
    }

    delay(1000);
}

int predict(float* input) {
    // Layer 1 forward propagation
    float layer1_output[8];
    for (int i = 0; i < 8; i++) {
        layer1_output[i] = layer1_biases[i];
        for (int j = 0; j < 6; j++) {
            layer1_output[i] += input[j] * layer1_weights[j][i];
        }
        layer1_output[i] = relu(layer1_output[i]);
    }

    // Layer 2 forward propagation
    float layer2_output[8];
    for (int i = 0; i < 8; i++) {
        layer2_output[i] = layer2_biases[i];
        for (int j = 0; j < 8; j++) {
            layer2_output[i] += layer1_output[j] * layer2_weights[j][i];
        }
        layer2_output[i] = relu(layer2_output[i]);
    }

    // Output layer forward propagation with softmax
    float output[3];
    for (int i = 0; i < 3; i++) {
        output[i] = output_biases[i];
        for (int j = 0; j < 8; j++) {
            output[i] += layer2_output[j] * output_weights[j][i];
        }
    }

    // Softmax for classification
    int predicted_class = 0;
    float max_prob = softmax(output, 0, 3);
    for (int i = 0; i < 3; i++) {
        float prob = softmax(output, i, 3);
        if (prob > max_prob) {
            max_prob = prob;
            predicted_class = i;
        }
    }

    return predicted_class;
}
