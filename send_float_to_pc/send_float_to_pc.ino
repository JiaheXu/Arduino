

//=============

void setup() {
  Serial.begin(9600);
}

//=============

void loop() {

  replyToPC();

}


float state_float_buffer[ 4 ];
byte state_byte_buffer[ 4*4 ];


void replyToPC() {
  float a[]={1.1, -2.2, 3.3, -4.4};
    state_float_buffer[0] = a[0];
    state_float_buffer[1] = a[1];
    state_float_buffer[2] = a[2];
    state_float_buffer[3] = a[3];    
    Serial.print("<");
    memcpy( state_byte_buffer, state_float_buffer, 4*4);
    Serial.write(state_byte_buffer, 4*4);  
    Serial.print(">");
    delay(1000);
}

