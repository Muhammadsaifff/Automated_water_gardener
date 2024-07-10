const int motorPin = 13;               // Digital pin connected to the transistor
const unsigned long onDuration = 40000; // 10 seconds in milliseconds
const unsigned long offDuration = 10000; // 30 seconds in milliseconds
unsigned long lastActivationTime = 0;
boolean motorState = HIGH;              // HIGH for OFF, LOW for ON
const int trigPin = 3;                  // Trig pin of the ultrasonic sensor
const int echoPin = 4;
const float waterLevelThreshold = 60.96; // 2 feet in centimeters

void setup() {
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, motorState);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo duration and calculate distance
  long duration = pulseIn(echoPin, HIGH);
  float distance_cm = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  // Get the current time
  unsigned long currentTime = millis();
  Serial.println(currentTime / 1000); // Print time in seconds

  // Check if it's time to toggle the motor state
  if (currentTime - lastActivationTime >= (motorState == LOW ? onDuration : offDuration)) {
    // Toggle the motor state
    motorState = !motorState;

    // Check distance only when turning on the motor
    if (motorState == LOW && distance_cm > waterLevelThreshold) {
      digitalWrite(motorPin, HIGH); // If distance is too high, turn off the motor
      motorState = HIGH; // Update the motor state
      Serial.println("Motor OFF (Distance Condition)");
    } else {
      digitalWrite(motorPin, motorState); // Otherwise, set the motor state accordingly
      lastActivationTime = currentTime;    // Update the last activation time
      Serial.println(motorState == LOW ? "Motor ON" : "Motor OFF");
    }
  }

  delay(1000); // Adjust the delay as needed for other code
}