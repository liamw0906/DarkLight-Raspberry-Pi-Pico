#include <SPI.h>
#include "menus.h"
#include "DACcontrols.h"
#include "adc.h"
String response = "";
 
void setup() { 
  pinMode(2, OUTPUT);     //ADC SCLK
  pinMode(3, OUTPUT);     //ADC DIN
  pinMode(4, INPUT);      //ADC DOUT
  pinMode(6, OUTPUT);     //ADC CS
  pinMode(7, OUTPUT);     //DAC1 CS
  pinMode(8, OUTPUT);     //DAC2 CS
  pinMode(10, OUTPUT);    //LED1
  pinMode(11, OUTPUT);    //LED2
  pinMode(12, OUTPUT);    //LED3
  pinMode(13, OUTPUT);    //LED4
  SPI.setSCK(2);          //Sets pin GP2 as SCK
  SPI.setTX(3);           //Sets pin GP3 as transmitter (DIN)
  digitalWrite(2, HIGH);  //Initialize SCK as high
  digitalWrite(3, HIGH);  //Initialize TX as high
  digitalWrite(6, HIGH);  //Initialize ADC CS High
  digitalWrite(7, HIGH);  //Initialize DAC1 CS high
  digitalWrite(8, HIGH);  //Initilize DAC2 CS high
  Serial.begin(9600);     //Start serial
  SPI.begin(false);       //Use spi
}

void loop() {
 
  displayUI();                         //Displays User Interface prompting user for inputs
  wait();                              //Waits for user input
  
  response = Serial.readString();      //Input response and print
  Serial.println("User Input: " +response);            //Return user input
  response.trim();                     //Trim response to exclude any null spaces and/or lines

  if (response == "L") {               //Active LED function to alter LED states
    LED();     
  }      
    
  else if (response == "S") {                 //Set Voltages of DAC 1 or 2
      
    Serial.println("Enter DAC to Adjust: 1 or 2");    //Choose between DAC 1 or 2    
    wait();                                   //Wait for response
    incomingbyte = Serial.parseInt();         //Read incoming decision
    Serial.readString();
    Serial.println("User Input: " +String(incomingbyte) + "\n");
    
    if (incomingbyte < 3 && incomingbyte > 0) {
      commandList();
      wait();                                   //Wait for response
      response = Serial.readString();           //Input response and print
      Serial.println("User Input " +response + "\n");     
      response.trim();

      if (response == "C") {                  //Turn on DAC
        commandDef();                         //Prompts user with command options, requests input
        addressDef();                         //Prompts user with address options, requests input
        requestVoltage();                     //Requests voltage input, converts to binary data string, also formats properly
        transferData();                       //Transfer command, address, and voltage data
      }

      if (response == "P") {                                       //Generalized Power On Command 
        DACPower();                           
        Serial.println("DAC "+ String(incomingbyte) + " Is On");        
      }

      if (response == "H") {                                       //Raise pin A high to 5 V
        writeDACHigh();
        Serial.println("DAC "+ String(incomingbyte) +" OUTPUT HIGH");
      }

      if (response == "L") {                                       //Set pin A low to 5 V
        writeDACLow();
        Serial.println("DAC "+ String(incomingbyte) +" OUTPUT IS LOW");
      }       
    }  
  }
    
  if (response == "A") {    //Display ADC UI -- TO BE COMPLETED
      adcUI();
      adccom();
    }        
  

  if (response == "H") {  //Display help menu
    helpmenu();           //General Help Menu
    ledhelp();            //LED Help Menu
    dachelp();            //DAC Help Menu
    adchelp();            //ADC Help Menu
  }
  
  else {
    Serial.println("Command Invalid or Process Finished Executing");        //Returns if unknown input command is sent or if process finishes
    Serial.println("Returning to User Interface\n");
  }  
  delay(200);       //Causes loop to wait, doesn't make much of a difference just less CPU intensive
  
}

  