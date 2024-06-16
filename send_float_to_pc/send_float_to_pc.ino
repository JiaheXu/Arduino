

//=============

void setup() {
  Serial.begin(9600);
}

//=============

void loop() {

  replyToPC();

}

void replyToPC() {
  float a[]={1.1, 2.2, 3.3, 4.4};

    Serial.print("<");
    Serial.print(a[0]);
    Serial.print(a[1]);   
    Serial.print(a[2]);   
    Serial.print(a[3]);   
    Serial.print(">");
    delay(1000);
}

