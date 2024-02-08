int led = 13;

int received = 0;

int i;

void setup() {

  Serial.begin(9600); 

  pinMode(led, OUTPUT);

}

 

void loop() 
{
  Serial.println("in loop");
  if (Serial.available() > 0) 
  {
    received = Serial.read();
    Serial.println("received");
    Serial.println(received);
    if (received == 'B')
    {
      digitalWrite(led, HIGH);
      delay(2000);
      digitalWrite(led, LOW);
    }

    else
    {
      for(i=0;i<5;i++)
      {
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
        delay(1000);
      }
    }

  } 

}

