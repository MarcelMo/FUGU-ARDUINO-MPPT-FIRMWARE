

void setupWiFi(){
  if(enableWiFi==1){
    WIFI = 1;
  }
  if(enableBluetooth==1){ initBluetooth(); }
}
#define SERVICE_UUID "dbdad7f5-a84f-4ab4-867c-e8104a3c02f3"

void initBluetooth(){
  // Create the BLE Device
  BLEDevice::init("MPPT SOLAR");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  int flags = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE;
  pCharacteristic_JSON = pService->createCharacteristic( "a8c8b8bc-7856-4a9b-8ac1-e4c21484d036",flags);

   // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic_JSON->addDescriptor(new BLE2902());
  
  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

unsigned long btLoopDelay=0;

void Wireless_Telemetry(){
  
  ////////// WIFI TELEMETRY ////////// 
  if(enableWiFi==1){
    int LED1, LED2, LED3, LED4;                      //Declare LED brightness variable 
    if(buckEnable==1)      {LED1=200;}else{LED1=0;}  //BATTERY CHARGING STATUS
    if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  //FULL CHARGE STATUS
    if(batteryPercent<=10) {LED3=200;}else{LED3=0;}  //LOW BATTERY STATUS
    if(IUV==0)             {LED4=200;}else{LED4=0;}  //PV INPUT PRESENCE STATUS
  }
  ////////// WIFI TELEMETRY ////////// 
  if(enableBluetooth==1){
    unsigned long m=millis();
    if (m>=btLoopDelay) {
      btLoopDelay=m+bluetoothUpdateMS; // update every n seconds
      bluetoothLoop();
    }
  }
} 


void jsonBuildB(String &json,String fname,bool value,bool firstElement){
    jsonBuildI(json,fname,value,firstElement);
}

void jsonBuildF(String &json,String fname,float value,bool firstElement){
    char buffer[50];
    if (!firstElement) json.concat(",");
    json.concat("\"");
    json.concat(fname);
    json.concat("\":");
    sprintf(buffer,"%f",value);
    json.concat(buffer);      
}

void jsonBuildI(String &json,String fname,int value,bool firstElement){
    char buffer[50];
    if (!firstElement) json.concat(",");
    json.concat("\"");
    json.concat(fname);
    json.concat("\":");
    sprintf(buffer,"%d",value);
    json.concat(buffer);      
}

char json_c[400];

void bluetoothLoop(){
  // notify changed value
  if (deviceConnected) {
      String json="{";
      // Battery %
      jsonBuildI(json,"battery",batteryPercent,true);
      jsonBuildF(json,"vin",voltageInput,false);
      jsonBuildF(json,"vout",voltageOutput,false);
      jsonBuildF(json,"cin",currentInput,false);
      jsonBuildF(json,"cout",currentOutput,false);
      jsonBuildF(json,"t",temperature,false);
      jsonBuildB(json,"ERR",ERR,false);
      jsonBuildB(json,"FLV",FLV,false);
      jsonBuildB(json,"BNC",BNC,false);
      jsonBuildB(json,"IUV",IUV,false);
      jsonBuildB(json,"OOV",OOV,false);
      jsonBuildB(json,"OOC",OOC,false);
      jsonBuildB(json,"OTE",OTE,false);
      jsonBuildB(json,"WIFI",WIFI,false);
      jsonBuildB(json,"REC",REC,false);
      jsonBuildB(json,"BYP",bypassEnable,false);
      jsonBuildB(json,"BUE",buckEnable,false);
      jsonBuildI(json,"FAN",fanPWM,false);
      jsonBuildI(json,"PWM",PWM,false);
      jsonBuildI(json,"PPWM",PPWM,false);

      json.concat("}");        
      json.toCharArray(json_c,sizeof(json_c));
      pCharacteristic_JSON->setValue(json_c);
      pCharacteristic_JSON->notify();
      Serial.println(json);

      delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      Serial.println("BLE start advertising");
      oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
  }
}
