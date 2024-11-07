import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam

# Load and preprocess the data
walking_df = pd.read_csv('/content/walking.csv')
jogging_df = pd.read_csv('/content/jogging.csv')
standing_df = pd.read_csv('/content/standing.csv')

# Drop unnecessary columns if needed
walking_df = walking_df.drop(walking_df.columns[0], axis=1)
jogging_df = jogging_df.drop(jogging_df.columns[0], axis=1)
standing_df = standing_df.drop(standing_df.columns[0], axis=1)

# Combine and label data (0 for walking, 1 for jogging, 2 for standing)
walking_df['label'] = 0
jogging_df['label'] = 1
standing_df['label'] = 2

# Concatenate all the dataframes
data = pd.concat([walking_df, jogging_df, standing_df], ignore_index=True)

# Features and labels
X = data[['Ax', 'Ay', 'Az', 'Gx', 'Gy', 'Gz']]
y = data['label']  # 0 for walking, 1 for jogging, 2 for standing

# Convert labels to categorical (one-hot encoding)
y = pd.get_dummies(y, drop_first=False)

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Define a neural network model for multiclass classification
model = Sequential([
    Dense(8, activation='relu', input_shape=(6,)),   # First hidden layer with 8 neurons
    Dense(8, activation='relu'),                     # Second hidden layer with 8 neurons
    Dense(3, activation='softmax')                   # Output layer with 3 neurons for 3 classes
])

# Compile the model
model.compile(optimizer=Adam(learning_rate=0.01), loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(X_train, y_train, epochs=20, batch_size=8, validation_data=(X_test, y_test))

# Get model weights and biases for deployment
for layer in model.layers:
    weights, biases = layer.get_weights()
    print(f"Weights: {weights}")
    print(f"Biases: {biases}")
