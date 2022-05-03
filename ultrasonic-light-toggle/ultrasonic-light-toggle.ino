// Register the pins
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR0
#define ledPin 4 // attach pin D4 to LED

bool calibrated = false;
float average;

bool on = false;

void calibrateSensor(int s = 10, int cps = 10) // s is the amount of seconds to calibrate for, the higher it gets, the more accurate. // cps is the checks per second.
{
 // take 15 seconds to calibrare the sensor by calculating the average distance. In order for it to be accurate at this time, there should be no movement.
 delay(5000); // wait to stabilize
  float total = 0;
  for (int i = 1; i < s; i++)
  {
      for(int j = 1; j < cps; j++) // run 10 checks per one second, each waiting 100 miliseconds between, 1000 miliseconds in a second, meaning 10 checks separated by 100 miliseconds
      {
        total = total + getDistance();
        delay(1000 / cps);
      }
  }  
  float iterations = s*cps;
  float a = total / iterations;
  average = a;
  
  Serial.print("Calibrated average: ");
  Serial.print(average);
  Serial.println(" cm.");
  calibrated = true;
}

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(ledPin, OUTPUT); // Set ledPin as OUTPUT
  
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  calibrateSensor();
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
  float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back). Measured in centimetres.

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm.");
  
  return distance;
}

bool checkDistance(float p = 35) // p is the percentage of tolerance, if the difference is more than 35% of the average, this will return true
{
   float d1 = getDistance();

   float diff = average - d1;  
   if(diff < 0) // if diff is negative make it positive
   {
    diff = fabsf(diff);
   }

   float dPercent = p / 100.0;
   float threshold = average * dPercent;
    Serial.println(diff);
    Serial.println(threshold);
   return diff >= threshold;
}

void loop() {
  if(calibrated == true) // only run this if calibration is complete.
  {
    // refreshes every 0.1 seconds.
    delay(100); 
    if(checkDistance())
    {
       on = !on;
       if(on == true)
       {
           digitalWrite(ledPin, HIGH);
       }
       else 
       {
          digitalWrite(ledPin, LOW);
       }
       delay(1000); // a 1 second delay before checking again
    }
    
    // turn the led on or off based on if on is true or false
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
