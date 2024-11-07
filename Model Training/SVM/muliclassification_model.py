import pandas as pd
import numpy as np
from sklearn.svm import LinearSVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, confusion_matrix

# Load and process walking, jogging, standing, and sleeping data
walking_df = pd.read_csv('/content/walking.csv')
jogging_df = pd.read_csv('/content/jogging.csv')
standing_df = pd.read_csv('/content/standing.csv')
sleeping_df = pd.read_csv('/content/sleeping.csv')

# Drop unnecessary columns if needed (e.g., the first column if it's an index)
walking_df = walking_df.drop(walking_df.columns[0], axis=1)
jogging_df = jogging_df.drop(jogging_df.columns[0], axis=1)
standing_df = standing_df.drop(standing_df.columns[0], axis=1)
sleeping_df = sleeping_df.drop(sleeping_df.columns[0], axis=1)

# Concatenate all data into one DataFrame
data = pd.concat([walking_df, jogging_df, standing_df, sleeping_df], ignore_index=True)

# Extract features and labels
X = data[['Ax', 'Ay', 'Az', 'Gx', 'Gy', 'Gz']]
y = data['label']  # Ensure label column contains values 0 (walking), 1 (jogging), 2 (standing), 3 (sleeping)

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Initialize and train the LinearSVC model
model = LinearSVC()
model.fit(X_train, y_train)

# Make predictions on the test set
y_pred = model.predict(X_test)

# Evaluate the model
print("Confusion Matrix:")
print(confusion_matrix(y_test, y_pred))
print("\nClassification Report:")
print(classification_report(y_test, y_pred))

# Get the model weights (coefficients) and intercept for each class
weights = model.coef_
intercept = model.intercept_

# Print out the weights for each class
for i, (class_weights, class_intercept) in enumerate(zip(weights, intercept)):
    print(f"\nClass {i} Weights: {class_weights}")
    print(f"Class {i} Intercept: {class_intercept}")
