//#include <Ultrasonic.h>
#include <NewPing.h>
#include <IRremote.h>

/*Declare pinout for the sonar and create sonar object*/
#define TRIGGER_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 100

//Variables for the remote
const int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;


// NewPing setup of pins and maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
//Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

/*Declare pinout for motors*/
// Motor A
int ENA = 2;
int IN1 = 4;
int IN2 = 5;

// Motor B
int ENB = 3;
int IN3 = 6;
int IN4 = 7;

int vel = 200;

int colorSensor = 8; // Declaration of the sensor input pin

/*Declare basic movements of the robot*/
void Adelante ()
{
  //Direccion motor A
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, LOW);
  digitalWrite (IN4, HIGH);
  analogWrite (ENB, vel); //Velocidad motor A
}
void Atras ()
{
  //Direccion motor A
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, HIGH);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, vel); //Velocidad motor B
}
void Derecha ()
{
  //Direccion motor A
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, HIGH);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, LOW);
  digitalWrite (IN4, HIGH);
  analogWrite (ENB, vel); //Velocidad motor A
}

void Izquierda ()
{
  //Direccion motor A

  //Direccion motor A
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, vel); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, vel); //Velocidad motor B
}
void Parar ()
{
  //Direccion motor A
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, 0); //Velocidad motor A
  //Direccion motor B
  digitalWrite (IN3, LOW);
  digitalWrite (IN4, LOW);
  analogWrite (ENB, 0); //Velocidad motor A
}

void sweepLeft()
{
  unsigned int distance = 100;

  while (distance > 35 )
  {
    distance = sonar.ping_cm();
    Izquierda();

  }

  Parar();
}

void nntLeft()
{
  for (long i = 0; i < 16666; i++)
  {
    Izquierda();
  }
  Parar();
}

void nntRight()
{
  for (long i = 0; i < 16666; i++)
  {
    Derecha();
  }
  Parar();
}

void turnLeft(long dist)
{
  for (long i = 0; i < (16666 + dist); i++)
  {
    Izquierda();
  }
  Parar();
}

void turnRight(long dist)
{
  for (long i = 0; i < 16666 + dist; i++)
  {
    Derecha();
  }
  Parar();
}

void specialRight()
{
  for (long i = 0; i < 1666; i++)
  {
    Derecha();
  }

  for (long i = 0; i < 1781; i++)
  {
    Adelante();
  }
}

void distanceAdv (long dist)
{
  for (long i = 0; i < dist; i++)
  {
    Adelante();
  }
  Parar();
}

int SonarRight()
{
  unsigned int distance = 100;

  Serial.print(distance);
  while (distance > 35)
  {
    distance = sonar.ping_cm();
    Serial.print(distance);
    Serial.print("\n");
    Derecha();
    if(distance == 0)
    {
      distance = 100;
    } 
  }

  Parar();

  return distance;
}


int SonarLeft()
{
  unsigned int distance = 100;

  Serial.print(distance);
  while (distance > 35)
  {
    distance = sonar.ping_cm();
    Serial.print(distance);
    Serial.print("\n");
    Izquierda();
    if(distance == 0)
    {
      distance = 100;
    } 
  }

  Parar();

  return distance;
}






void setup()
{
  Serial.begin(9600) ;
  // Declaramos todos los pines como salidas
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}
void runToBlack() {
  bool inBlack = LOW;
  while (inBlack == LOW) {
    inBlack = digitalRead (colorSensor);
    Adelante();
  }

}
void loop()
{
  unsigned int distance = sonar.ping_cm();
  int objDist;
  bool black = digitalRead (colorSensor);
  bool detected = false;
  bool started = false;

  if (irrecv.decode(&results))
  {
    if(results.value == 0xFF6897)
    {
      if (black == HIGH)
      {
        Parar();
      } else {
        //1781 == 1cm
        delay(5000);
        Serial.print("parte SONAR");
        Serial.print("\n");
        objDist = SonarLeft();
        Serial.print("Detected at:");
        Serial.print("\n");
        Serial.print(objDist);
        Serial.print("\n");
        delay(2000);
        Serial.print("parte LEFT");
        Serial.print("\n");
        turnLeft(-(8500));
        Serial.print("parte BLACK");
        Serial.print("\n");
        runToBlack();
        Serial.print("parte SWEEP RIGHT");
        Serial.print("\n");
        objDist = SonarRight();
        Serial.print("\n");
        delay(2000);
        specialRight();
        runToBlack();
      }
    }
    if(results.value == 0xFF18E7)
    {
      if (black == HIGH)
      {
        Parar();
      } else {
        //1781 == 1cm
        delay(5000);
        Serial.print("parte SONAR");
        Serial.print("\n");
        objDist = SonarLeft();
        specialRight();
        runToBlack();
      }
    }
    key_value = results.value;
    irrecv.resume(); 
  }
}    
