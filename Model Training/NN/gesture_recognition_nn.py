import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam

# Load and preprocess the data
walking_df = pd.read_csv('/content/walking.csv')
jogging_df = pd.read_csv('/content/jogging.csv')

# Drop unnecessary columns if needed
walking_df = walking_df.drop(walking_df.columns[0], axis=1)
jogging_df = jogging_df.drop(jogging_df.columns[0], axis=1)

# Combine and label data (assuming label column is the last)
data = pd.concat([walking_df, jogging_df], ignore_index=True)
X = data[['Ax', 'Ay', 'Az', 'Gx', 'Gy', 'Gz']]
y = data['label']  # 0 for walking, 1 for jogging

# Split data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Define a small neural network model
model = Sequential([
    Dense(4, activation='relu', input_shape=(6,)),   # First hidden layer with 4 neurons
    Dense(4, activation='relu'),                     # Second hidden layer with 4 neurons
    Dense(1, activation='sigmoid')                   # Output layer
])

# Compile the model
model.compile(optimizer=Adam(learning_rate=0.01), loss='binary_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(X_train, y_train, epochs=20, batch_size=8, validation_data=(X_test, y_test))

# Get model weights and biases for deployment
for layer in model.layers:
    weights, biases = layer.get_weights()
    print(f"Weights: {weights}")
    print(f"Biases: {biases}")
