// Register the pins
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR0
#define ledPin 4 // attach pin D4 to LED

bool calibrated = false;
int averageDistance;

bool on = false;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(ledPin, OUTPUT) // Set ledPin as OUTPUT
  
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  calibrateSensor()
}

void calibrateSensor(int s = 10, int cps = 10;) // s is the amount of seconds to calibrate for, the higher it gets, the more accurate. // cps is the checks per second.
{
 // take 10 seconds to calibrare the sensor by calculating the average distance. In order for it to be accurate at this time, there should be no movement.
  int total = 0;
  for (int i = 1; i < s; i++)
  {
      for(int j = 1; j < cps; j++) // run 10 checks per one second, each waiting 100 miliseconds between, 1000 miliseconds in a second, meaning 10 checks separated by 100 miliseconds
      {
        total = total + getDistance();
        delay(1000 / cps);
      }
  }
  averageDistance = total / s*cps;
  
  Serial.println("Calibrated average (cm): ");
  Serial.print(average);
}

int getDistance() // helper function to get the distance measured by the ultrasonic sensor
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

bool checkDistance(int p = 35) // p is the percentage of tolerance, if the difference is more than 35% of the average, this will return true
{
   int d1 = getDistance();
   delay(1000);
   int d2 = getDistance();

   float diff = d1 - d2;

   float threshold = average * (p / 100);

   return diff >= threshold;
}

void loop() {
  if(calibrated = true) // only run this if calibration is complete.
  {
    // every second checks the difference of distances.
    if(checkDistance())
    {
       on = !on;
    }
    if(on == true)
    {
      digitalWrite(ledPin, HIGH);
    }
    else 
    {
      digitalWrite(ledPin, LOW);
    }
  } 
}
