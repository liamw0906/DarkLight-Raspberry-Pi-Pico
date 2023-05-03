
void adcUI() {                      //This whole thing has yet to be done
    Serial.println("ADC Command List: Enter Number Corresponding to Command to Select");
}

void adccom() {                     //TO BE COMPLETED
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    digitalWrite(6, LOW);
    SPI.transfer(0);
    SPI.transfer(0);

    SPI.endTransaction();
}