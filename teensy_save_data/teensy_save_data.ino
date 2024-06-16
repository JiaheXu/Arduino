//  String of floats input routine.  Format description at End Of File below.
// Oct 3, 2020, not working as expected.

#include <Arduino.h>
#include <EEPROM.h>
String inCommand = "";
char inChar;
IntervalTimer Timer_1S;  // Just to blink the TeensyLED.
  
void setup() {
  Serial.begin(115200);
  while(!Serial) ;
  pinMode(13, OUTPUT);          digitalWrite(13, HIGH);
  Timer_1S.begin(INT_Timer, 200000);   
}

void INT_Timer() {  
  if (digitalRead(13) != HIGH)
  {
    digitalWrite(13, HIGH); 
  }
  else
  {    
    digitalWrite(13, LOW);
  }
}

float freadFromEEPROM(int fp)
{ 
  int n; 
  float floatReturn;
  unsigned int fromEEPROM[4] = {0,0,0,0};  
  for ( n=0; n<4; n++)
  {
    fromEEPROM[4] = EEPROM.read(fp+n);
  }
  floatReturn = fromEEPROM[0] | fromEEPROM[1] << 8 | fromEEPROM[2] << 16 | fromEEPROM[3] << 24; 
  return floatReturn;
}


float fwriteToEEPROM(float floatToWrite, int fp)
{
  int n;
  unsigned char toEEPROM;
  for ( n=0; n<4; n++)
  {
    toEEPROM = (char)(floatToWrite >> n*8) & 0xFF;
    EEPROM.write(toEEPROM, fp+n); delay(4);
  }
  return freadFromEEPROM(int fp);  // Return what was written to confirm.
} 


void loop() {
  char bufTemp[64]="";
  int StrLength, DollarLoc, CommaLoc, AsteriskLoc, floatsToRead, destAddress;
  double myFloat = 0;       //123456789012.123456789;
  double floatArray[16];    // 
  int n,pointIntoString; 
  if (Serial.available() > 0){
    inCommand = "";
    while(Serial.available() ){  // Read the hwole thing in, assume it's complete to the cr-lf.
      inChar = Serial.read();
      if( (inChar != '\r') && (inChar != '\n'))  inCommand += inChar;  
    }
    Serial.println(inCommand);  
    StrLength = inCommand.length();           //Serial.println(StrLength);    
    DollarLoc = inCommand.indexOf('$');       //Serial.println(DollarLoc);   
    CommaLoc = inCommand.indexOf(',');        //Serial.println(CommaLoc);    //This is the first of several commas.
    AsteriskLoc = inCommand.indexOf('*');     //Serial.println(AsteriskLoc); 
    /* int CRCpassFail = GetCRCandCheck();   // To Do: get the CRC from the two characters following the '*'.    if (CRCpassFail == true){          }else{      Serial.println("CRC fail message");    }  */
    if( (DollarLoc == 0) && (CommaLoc == (DollarLoc+3)) && (AsteriskLoc > CommaLoc ) ){  // AND between them is 'IS', then read the number of floats to read (between the first and second comma.
      Serial.print("Before remove  = ");  Serial.println(inCommand);
      inCommand.remove(0,CommaLoc+1);
      Serial.print("After  remove  = ");  Serial.println(inCommand);        
      inCommand.toCharArray(bufTemp, inCommand.indexOf(',')+1);
      floatsToRead = atoi(bufTemp);                                         //  Serial.print("After sprintf  = ");  Serial.println(bufTemp);    //    Serial.print("tempOutBuf     = ");  Serial.println(TempOutBuf);
      Serial.print("FloatsToRead   = ");  Serial.println(floatsToRead);
      inCommand.remove(0,inCommand.indexOf(',')+1);
      inCommand.toCharArray(bufTemp, inCommand.indexOf(',')+1);
      destAddress = atoi(bufTemp);                                          //  Serial.print("After sprintf  = ");  Serial.println(bufTemp);    //    Serial.print("tempOutBuf     = ");  Serial.println(TempOutBuf);
      Serial.print("destAddress = ");  Serial.println(destAddress);
      inCommand.remove(0,inCommand.indexOf(',')+1);                         //  Serial.print("Stripped   = ");  Serial.println(inCommand);
      
      for( n=0; n < floatsToRead; n++){     //CommaLoc = inCommand.indexOf(',');   Serial.print("CommaLoc = ");         Serial.println(CommaLoc);    
        CommaLoc = inCommand.indexOf(','); 
        if (CommaLoc != -1){
          inCommand.toCharArray(bufTemp, CommaLoc+1);
          inCommand.remove(0,CommaLoc+1);
        }else{
          inCommand.toCharArray(bufTemp, inCommand.indexOf('*')+1);
          inCommand.remove(0,inCommand.indexOf('*')+1);
        }
        Serial.print("bufTemp = "); Serial.println(bufTemp);        // Serial.print("    inCommand = "); Serial.print(inCommand);   
        floatArray[n] = atof(bufTemp); // inCommand.toFloat();      //Serial.print("      Float "); Serial.print(n); Serial.print(" = ");  Serial.print(floatArray[n]);
        sprintf(bufTemp, "%24.6f", floatArray[n]);
        Serial.print("     Formatted = "); Serial.println(bufTemp);
        GotAGoodString = true;
      }
    }else{
      Serial.println (" Didn't have the right $ and , and * locations ");  
    }
  }
  if (GotAGoodString == true){  
    GotAGoodString = false;
    for (n=0; n<floatsToRead-1; n++){
      Serial.print("element "); Serial.print(n); Serial.print(" = "); 
      Serial.println(fwriteToEEPROM(floatArray[n], n));
    }
  }
}


 
 







// Some test data strings.......
//  $IS,5,7,1234.3456,98765.56789,252525.525252,4576,5467.6748*HI
//  $IS,05,07,01234.3456,98765.567890,252525.525252,04576,5467.6748*HI
//  $IS,016,07,01234.3456,98765.567890,252525.525252,04576,5467.6748*HI

/* Format 
 *  $IS,nn,aa,D1.E1,D2.E2,D3.E3,...   ...,Dn.En*CRC
 *  $  = new data identifier (carryover from LTI format)
 *  IS = string identifier mnemonic, "Insert String"
 *  nn = number of comma delimited data elements in string (following address)...  
 *        .... typically an int type.
 *  aa = address (destination of string, to be used by receiving device typically as an array index)... 
 *        ... int type, and arbitrary limit address range to 0-255.    
 *  D1.E1 = a floating point number, like 12345.67.   (limit is 32 bits (4294967296) integer, 6 digits fractional) 
 *  Dn.En = the last float in the string  
 *    
 *  '*' signifies the end of data and the start of the CRC bytes 
 *  CRC is a two byte CRC from the '$' to the '*', inclusive.  Not implemented as of Oct 3, 2020
 */



 