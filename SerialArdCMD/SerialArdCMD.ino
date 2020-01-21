/*************************
   SerialArdCMD
   Written and tested by Adrian Keating (C) 2019
   adrian.keating at uwa.edu.au
  This routine allows any functions to be added and then called from a command line interface.
  The Serial port is continually polled so that the called function can be stopped at any time.
  Importantly, the function "ReturnFloat" provides a simple way to pass command line parameters
  to the arduino, including negative number and decimal value.  This is usefull when setting
  averages of data or updating a calibration value within the Arduino.
*/
#define VERSION 1.0
boolean Verbose;
unsigned long TIME0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  Verbose = LOW;
  pinMode(LED_BUILTIN, OUTPUT);
  splash();
}


void loop() {
  int Bytes2Read = 0;
  int myinteger = 0;
  float myfloat = 0;
  boolean logdataflag = false;
  Bytes2Read = Serial.available();
  if (Bytes2Read > 0) {
    int inByte = Serial.read();

    if (inByte > 96) inByte = inByte - 32; //convert to upper case
    switch (inByte) {
      case 'R':
        break;
      case '?':
        splash();
        break;
      case 'V':
        Verbose = HIGH;
        break;
      case 'S':
        Verbose = LOW;
        break;
      case 'I':
        myfloat = ReturnFloat(Bytes2Read);
        if (Verbose) {Serial.print("Float = ");Serial.println(myfloat, 3);}
        break;
      case 'H':
        myfloat = ReturnFloat(Bytes2Read);
        myinteger = ((int) myfloat);
        if (Verbose) {Serial.print("Integer = ");Serial.println(myinteger, DEC);}
        if (myinteger == 1) TurnLEDon();
        if (myinteger == 0) TurnLEDoff();
        break;
      case 'A':
        break;

      case 'L':
        // turn logging on
        logdataflag = true;
        TIME0 = millis(); // zero the time
        break;
      case 'X':
        // Turn logging off
        if (Verbose) {Serial.print ("Time(s)= ");Serial.println(((float) (millis () - TIME0)) / 1000, 2);}
        logdataflag = false;
        break;
      case 'C':
        // Run calibration  if required
        break;
        //default:
        // do nothing
    }
    Serial.flush();
  }
  if (logdataflag) {
    logdata();
  }

}

void logdata() {
  unsigned long time;
  static unsigned long timesent;
  time = millis() - TIME0;
  //************** DATA COLLECTION ROUTINE HERE *****************//
}

void TurnLEDon() {
  digitalWrite(LED_BUILTIN, HIGH);
}
void TurnLEDoff() {
  digitalWrite(LED_BUILTIN, LOW);
}


void splash (void) {
  Serial.print(F("Data Logging ................. By Adrian Keating (C) 2017, VERSION:"));
  Serial.println (VERSION, 2);
  Serial.println(F("Command Set (use either upper orlower case, commands with numbers end in ; or CR):"));
  Serial.println(F("? - Help"));
  Serial.println(F("In - Function requiring float input, x.x=float"));
  Serial.println(F("Hx - Function requiring integer input, n=integer, n=0: turn LED off, n=0:turn LED on"));
  Serial.println(F("C - Calibration Routine"));
  Serial.println(F("L - Start Data Logging"));
  Serial.println(F("X - Stop Data Logging"));
  Serial.println(F("V - Verbose mode"));
  Serial.println(F("S - Silent mode (no labels on data)"));
}

float ReturnFloat (int BytesAtSerialPort) {
  // returns a float from entered data, allows white space
  int NumBytesAvaiable;
  int NextByte;
  boolean ExitFlag = false;
  long val = 0;
  float valfloat;
  float scale;
  int chval = 0;
  int indexponent = 0;
  int ind = 0;
  int sign = 1;
  while (ExitFlag == false)
  {
    chval = Serial.read ();
    if (chval == '-' & ind == 0)
    { //  loop as many times as required to cheak for minus
      // do nothing
      sign = sign * -1;;
    }
    else {
      if (chval == '.' & indexponent == 0)
      { // note the location of a decimal pt
        // the check & indexponent==0 ensure that is a 2nd decimal point is entered, it is ignored
        ind = ind + 1;
        indexponent = ind;
      }
      else
      {
        if (chval >= '0' && chval <= '9')
        { // is ch a number?
          val = val * 10 + chval - '0'; // yes, accumulate the value
          ind = ind + 1;
        }
        else if (chval == ';' || chval == 13 || chval == 10)
        { // is ch an ; or a CR
          if (ind == 0)
            valfloat = val; // traps case when only ; entered and no number
          else
          {
            if (indexponent == 0) indexponent = ind;
            scale = pow (10, (ind - indexponent));
            valfloat = ((float) sign * val) / scale;
          }
          ExitFlag = true;
        }     // end check for decimal pt
      }   // check for leading + or - sign
    }
  }
  return valfloat;
}


