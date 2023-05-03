#include <iostream>

int incomingbyte, commandbyte, addressbyte, i;     //Int used for pin input
String addressbin = "";     
                                
void wait() {                                                 //Function to wait for user response
   while (Serial.available() == 0) {}                         //While serial is not available, do nothing -> causes Serial to wait for an input
}

void displayUI() {                                            //User interface displaying available options
  Serial.println("USER INTERFACE\n");
  Serial.println("1. To Set LED Configuration, Press L");
  Serial.println("2. For Set Voltage, Press S");
  Serial.println("3. For ADC Control, Press A");
  Serial.println("4. For Help, press H");
}

void commandList() {                                                                     //Command list of current dac operations
  Serial.println("List of DAC Commands: Enter Number To Select\n");
  Serial.println("1. To Turn on DAC, Press P");
  Serial.println("2. To Set Pin A High, Press H");
  Serial.println("3. To Set Pin A Low, Press L");
  Serial.println("4. To Enter Custom Command, Press C");
}

void commandDef() {                                                                      //List of command bits for dac
  Serial.println("DAC Command List: Enter Number Corresponding to Command to Select\n");     //Prompts user to input command ID number
  Serial.println("0. Write to Input Register n");
  Serial.println("1. Update DAC Register n");
  Serial.println("2. Write to Input Register n, update all");
  Serial.println("3. Write to and update DAC Channel n");
  Serial.println("4. Power down/power up DAC");
  Serial.println("5. Load clear code register");
  Serial.println("6. Load LDAC register");
  Serial.println("7. Set up internal REF register");
  wait();                                                                                //Waits for user input
  commandbyte = Serial.parseInt();
  Serial.readString();                                                                   //Read in command
  Serial.println("User Input: " +String(commandbyte) + "\n");                                                           //Read back to user
}

String addresstoBin() {                                       //Used to convert the address decimal into 4 character binary string
  addressbin = "";                                            //Define empty address binary string
  for (int i = 3; i >= 0; i--) {                              //Convert into binary
    int abit = ((addressbyte >> i) & 1);
    addressbin += String(abit);
  }
  i = 3;                                                      //Reset i to 3
  Serial.println("Decimal in Binary: " +addressbin+ "\n");    //Print out four character binary string              
  return addressbin;                                          //Return string
}

void addressDef() {                                                                 //UI to ask for Address Bytes
  Serial.println("Address List: Enter Number Corresponding to Pin to Select\n");
  Serial.println("Input 0-7 For DAC Pins A-H");
  Serial.println("Input 8 to write to all DAC Pins\n");
  wait();
  addressbyte = Serial.parseInt();
  Serial.readString();
  Serial.println("User Input: " +String(addressbyte)+ "\n");                               
  addresstoBin();
}

void helpmenu() {                                 //Help menu UI
  Serial.println("Select Help Option:\n");
  Serial.println("1. LED Information");
  Serial.println("2. DAC Operation Information");
  Serial.println("3. ADC Operation Information");
  wait();
  incomingbyte = Serial.parseInt();
  Serial.readString();
  Serial.println("User Input: "+ String(incomingbyte)+ "\n");
}

void ledhelp() {                                  //Brief LED function help menu
  if (incomingbyte == 1) {
    Serial.println("The PCB consists of 4 onboard LEDs.");
    Serial.println("Access LED Control via 'L' in the User Interface.");
    Serial.println("Once LED Controls are accessed, use 1-4 to control LEDs.");
    Serial.println("The state of the LED will be swapped to the opposite state.\n");
  }
}

void dachelp() {                                  //Brief DAC function help menu 
  if (incomingbyte == 2) { 
    Serial.println("The PCB consists of 2 DACs used to set Analog Voltages.");
    Serial.println("The lower DAC is denoted as 1 and the upper as 2.");
    Serial.println("The DACs can be controlled by pressing 'S' in the User Interface.");
    Serial.println("Each pin of the DAC can be controlled individually and output 0-5 volts.\n");
  }
}

void adchelp() {                                  //Brief ADC function help menu
  if (incomingbyte == 3) {
    Serial.println("To Be Determined\n");         //PROJECT FOR FUTURE UNDERGRAD!!
  }
}

void LED() {                                                //command used to alter LED state
  Serial.println("Enter LED Number 1-4 To Alter");          //request led number
  wait();                                                   //wait for user response             
  incomingbyte = Serial.parseInt();                         //return integer response
  Serial.readString();                                      //read response into serial
  Serial.println("User Input: " +String(incomingbyte)+ "\n");                             //read input back to user
        
  if (incomingbyte > 0 && incomingbyte < 5 ) {              //swap current state of LEDs On or Off using ints 1-4
    if (digitalRead(incomingbyte + 9) == HIGH) {            //check current state of LED, high = off, low = on
      digitalWrite(incomingbyte + 9, LOW);                  //writes pin low if already high in order to turn on LED
      Serial.println("LED " +String(incomingbyte)+ " IS ON\n");                        //displays led status
    }
    else{                                                   //check if led on or off
      digitalWrite(incomingbyte + 9, HIGH);                 //turns off led
      Serial.println("LED " +String(incomingbyte)+ " IS OFF\n");                       //displays led status
    }            
  }
}