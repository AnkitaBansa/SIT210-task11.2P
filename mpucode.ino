#include <Arduino_LSM6DS3.h>

// Constants for fall detection thresholds
const float GyroThreshold = 150.0;  // Adjust this threshold as needed
const int DetectionDelay = 5000;      // Delay (in milliseconds) to avoid false positives

bool fallDetected = false;
unsigned long lastDetectionTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");
}

void loop() {
  float x, y, z;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);

    // Serial.print(x);
    // Serial.print('\t');
    // Serial.print(y);
    // Serial.print('\t');
    // Serial.println(z);

    // Calculate the magnitude of angular velocity
    float gyroMagnitude = sqrt(x * x + y * y + z * z);
    // Serial.println(gyroMagnitude);

    // Check if the gyroscope magnitude exceeds the threshold
    if (gyroMagnitude > GyroThreshold) {
      // Check if enough time has passed since the last detection to avoid false positives
      unsigned long currentTime = millis();
      if (currentTime - lastDetectionTime > DetectionDelay) {
        // Fall detected
        Serial.println("Fall detected!");
        fallDetected = true;
        lastDetectionTime = currentTime;
      }
    }
  }

  // You can add additional logic here to handle the fall detected state as needed
  // For example, trigger an alarm, send a notification, etc.
}