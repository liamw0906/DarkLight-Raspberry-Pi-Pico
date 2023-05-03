int bit0, bit1, bit2, adv, vbyte, D, j;                                                    
float Vout;
String dbin = "", firstfour = "";

void DACPower()   {                                                    //function to turn on DAC
    SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));  //begin transaction using default settings
    digitalWrite(6 + incomingbyte, LOW);                               //write pin low to begin
    SPI.transfer(4);                                                   //command bit to turn on DAC
    SPI.transfer(0);                                                   //address bits and 4 data
    SPI.transfer(0);                                                   //all data bits
    SPI.transfer(0);                                                   //all dummy
    digitalWrite(6 + incomingbyte, HIGH);                              //write pin high to end
    SPI.endTransaction();                                              //end transaction
}


void writeDACHigh() {                                                  //function to write DAC pin A high
    SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));  //begins SPI transaction using clk speed @ 50MHz, MSB first and mode 0
    digitalWrite(7, LOW);                               //begins write sequence of DAC1 by bringing SYNC low   
    SPI.transfer(3);                                                   //first 8 bits- 4 dummy followed by 4 command bits
    SPI.transfer(15);                                                  //second 8 bits- 4 address followed by first 4 of data
    SPI.transfer(255);                                                 //third 8 bits - all data
    SPI.transfer(0);                                                   //last 8 bits - all dummy
    digitalWrite(6 + incomingbyte, HIGH);                              //ends write sequence of DAC1 by bringing SYNC HIGH 
    SPI.endTransaction();                                              //end transaction
}

void writeDACLow() {                                                   //function to write DAC low
    SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));  //begins SPI transaction using default settings
    digitalWrite(6 + incomingbyte, LOW);                               //begins write sequence of DAC1 by bringing SYNC low
    SPI.transfer(3);                                                   //first 8 bits- 4 dummy followed by 4 command bits
    SPI.transfer(0);                                                   //second 8 bits- 4 address followed by first 4 of data
    SPI.transfer(0);                                                   //third 8 bits - all data
    SPI.transfer(0);                                                   //last 8 bits - all dummy
    digitalWrite(6 + incomingbyte, HIGH);                              //ends write sequence of DAC1 by bringing SYNC HIGH
    SPI.endTransaction();                                              //end transaction
}

void requestVoltage() {                           //calls for voltage input
    dbin = "";
    Serial.println("Input Voltage in Volts");     //request voltage input from user
    wait();                                       //wait for response
    Vout = Serial.parseFloat();                   //read response
    Serial.readString();
    Serial.println("User Input: " +String(Vout)+ "V\n");                         //print back response to user
    D = Vout / (5.00122) * 4096;                 //convert output voltage to desired decimal 0-4095

    for (int j = 11; j >= 0; j--) {               //convert decimal number to binary
        int anotherbit = ((D >> j) & 1);
        dbin += anotherbit;
    }
    j = 11;
    Serial.println("Voltage Command in Binary:" +dbin + "\n"); //outputs 12 bit voltage string in binary
    
    String firstfour = dbin.substring(0,4);         //takes first four bits of voltage string
    Serial.println("First Four Binary Bits of Data: " +firstfour);
    String lasteight = dbin.substring(4,12);        //takes last eight bits of voltage string
     
    String advoltbin = addressbin + firstfour;      //smushes together address bit and first four of voltage
    Serial.println("4 Address Bits with First Four Data Bits: " +advoltbin);
    Serial.println("Last Eight of Data Bits: " +lasteight);

    adv = 0;
    vbyte = 0;

    for (int k = 0; k < advoltbin.length(); k++) {                                   //convert address bit and first four into decimal
        adv += (advoltbin.charAt(k) - '0') * pow(2, (advoltbin.length() -1 - k));
    }
    Serial.println("Address Bits with First Four Data In Decimal: " +String(adv));

    for (int l = 0; l < lasteight.length(); l++) {
        vbyte += (lasteight.charAt(l) - '0') * pow(2, (lasteight.length() -1 - l));     //convert last eight of voltage into decimal
    }
    Serial.println("Last 8 Data Bits in Decimal: " +String(vbyte)+ "\n");   
    return;
}

void transferData() {
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  digitalWrite(6 + incomingbyte, LOW);
  SPI.transfer(commandbyte);
  SPI.transfer(adv);
  SPI.transfer(vbyte);
  SPI.transfer(0);
  digitalWrite(6 + incomingbyte, HIGH);
  SPI.endTransaction();
  Serial.println("Command Written\n");
}

