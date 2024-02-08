int led = 13;
int relay_pin = 10;
char received = 0;

int i;

void setup() {

  Serial.begin(9600); 

  pinMode(led, OUTPUT);

}

 
int relay = 0;
void loop() 
{  
  if(relay == 0)
  {
    digitalWrite(led, LOW);
    digitalWrite(relay_pin, LOW);
  }
  else
  {
    digitalWrite(led, HIGH);
    digitalWrite(relay_pin, HIGH);
  }
  // Serial.println("running");
  if (Serial.available() > 0) 
  {
    received = Serial.read();

    Serial.println("received: ");
    Serial.println(received);
    
    if (received == 't')
    {
      relay = 1;
    }

  }

}