#define MENU_PAGE_SUPPLY_ALGORYTHM 0
#define MENU_PAGE_MODE 1
#define MENU_PAGE_MAX_BATTERY 2
#define MENU_PAGE_MIN_BATTERY 3
#define MENU_PAGE_CHARGING_CURRENT 4
#define MENU_PAGE_FAN_ENABLE 5
#define MENU_PAGE_FAN_LOW_TEMP 6
#define MENU_PAGE_FAN_HIGH_TEMP 7
#define MENU_PAGE_SHUTDOWN_TEMP 8
#define MENU_PAGE_WIFI 9
#define MENU_PAGE_AUTOLOAD 10
#define MENU_PAGE_BACKLIGHT_SLEEP 111
#define MENU_PAGE_FACTORY_RESET 12
#define MENU_PAGE_FIRMWARE 13
#define MENU_PAGE_MAXIMUM 13

bool displaySleeping = false;
boolean lcdBacklight_Wake(){
  if (displaySleeping) {
    lcd.setBacklight(HIGH);
    prevLCDBackLMillis = millis();
    displaySleeping=false;
    return(true);
  } else {
    return(false);
  }
}

int waitForRelease = 0;
bool keyDown(int button) {
  if (waitForRelease!=0) { // Waiting for keyup ?
    if (digitalRead(waitForRelease)==1) { // Still pressed ?
      return(false);
    } else {                              // Continue normal operation
      waitForRelease=0;
    }
  }
  bool isPressed = digitalRead(button)==1;
  if (isPressed) {
    waitForRelease = button;
    return(!lcdBacklight_Wake());
  } else {
    return(false);
  }
}

bool keyHold(int button) {
  return(digitalRead(button)==1);
}

void lcdBacklight(){
  unsigned long backLightInterval;
  if(backlightSleepMode==0){prevLCDBackLMillis = millis();}                 //Set backlight var to sleep never
  else if(backlightSleepMode==1){backLightInterval=10000;}                  //Set backlight var to sleep after 10 seconds without keypress 
  else if(backlightSleepMode==2){backLightInterval=300000;}                 //Set backlight var to sleep after 5 minutes without keypress 
  else if(backlightSleepMode==3){backLightInterval=3600000;}                //Set backlight var to sleep after 1 hour without keypress  
  else if(backlightSleepMode==4){backLightInterval=21600000;}               //Set backlight var to sleep after 6 hours without keypress    
  else if(backlightSleepMode==5){backLightInterval=43200000;}               //Set backlight var to sleep after 12 hours without keypress   
  else if(backlightSleepMode==6){backLightInterval=86400000;}               //Set backlight var to sleep after 1 day without keypress 
  else if(backlightSleepMode==7){backLightInterval=259200000;}              //Set backlight var to sleep after 3 days without keypress 
  else if(backlightSleepMode==8){backLightInterval=604800000;}              //Set backlight var to sleep after 1 week without keypress  
  else if(backlightSleepMode==9){backLightInterval=2419200000;}             //Set backlight var to sleep after 1 month without keypress    

  if(backlightSleepMode>0 && settingMode==0){
    currentLCDBackLMillis = millis();
    if(currentLCDBackLMillis-prevLCDBackLMillis>=backLightInterval){        //Run routine every millisRoutineInterval (ms)
      prevLCDBackLMillis = currentLCDBackLMillis;                           //Store previous time
      lcd.setBacklight(LOW);                                                //Increment time counter
    } 
  }  
}
void padding100(int padVar){
  if(padVar<10){lcd.print("  ");}
  else if(padVar<100){lcd.print(" ");}
}
void padding10(int padVar){
  if(padVar<10){lcd.print(" ");}
}
void displayConfig1(){
  lcd.setCursor(0,0);lcd.print(powerInput,0);lcd.print("W");padding100(powerInput);      
  lcd.setCursor(5,0);
  if(Wh<10){lcd.print(Wh,3);lcd.print("Wh ");}                 //9.999Wh_
  else if(Wh<100){lcd.print(Wh,2);lcd.print("Wh ");}           //99.99Wh_
  else if(Wh<1000){lcd.print(Wh,1);lcd.print("Wh ");}          //999.9Wh_
  else if(Wh<10000){lcd.print(kWh,2);lcd.print("kWh ");}       //9.99kWh_
  else if(Wh<100000){lcd.print(kWh,1);lcd.print("kWh ");}      //99.9kWh_
  else if(Wh<1000000){lcd.print(kWh,0);lcd.print("kWh  ");}    //999kWh__
  else if(Wh<10000000){lcd.print(MWh,2);lcd.print("MWh ");}    //9.99MWh_
  else if(Wh<100000000){lcd.print(MWh,1);lcd.print("MWh ");}   //99.9MWh_ 
  else if(Wh<1000000000){lcd.print(MWh,0);lcd.print("MWh  ");} //999MWh__
  lcd.setCursor(13,0);lcd.print(daysRunning,0); 
  lcd.setCursor(0,1);lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent);
  if(BNC==0){lcd.setCursor(5,1); lcd.print(voltageOutput,1);lcd.print("V");padding10(voltageOutput);}
  else{lcd.setCursor(5,1);lcd.print("NOBAT ");}          
  lcd.setCursor(11,1);lcd.print(currentOutput,1);lcd.print("A");padding10(currentOutput);     
}
void displayConfig2(){
  lcd.setCursor(0,0); lcd.print(powerInput,0);  lcd.print("W");padding100(powerInput);    
  lcd.setCursor(5,0); lcd.print(voltageInput,1);lcd.print("V");padding10(voltageInput);            
  lcd.setCursor(11,0);lcd.print(currentInput,1);lcd.print("A");padding10(currentInput);    
  lcd.setCursor(0,1); lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent); 
  if(BNC==0){lcd.setCursor(5,1); lcd.print(voltageOutput,1);lcd.print("V");padding10(voltageOutput);}
  else{lcd.setCursor(5,1);lcd.print("NOBAT");}
  lcd.setCursor(11,1);lcd.print(currentOutput,1);lcd.print("A");padding10(currentOutput);    
}
void displayConfig3(){
  lcd.setCursor(0,0);  lcd.print(powerInput,0);lcd.print("W");padding100(powerInput); 
  lcd.setCursor(5,0);
  if(Wh<10){lcd.print(Wh,2);lcd.print("Wh ");}                 //9.99Wh_
  else if(Wh<100){lcd.print(Wh,1);lcd.print("Wh ");}           //99.9Wh_
  else if(Wh<1000){lcd.print(Wh,0);lcd.print("Wh  ");}         //999Wh__
  else if(Wh<10000){lcd.print(kWh,1);lcd.print("kWh ");}       //9.9kWh_
  else if(Wh<100000){lcd.print(kWh,0);lcd.print("kWh  ");}     //99kWh__
  else if(Wh<1000000){lcd.print(kWh,0);lcd.print("kWh ");}     //999kWh_
  else if(Wh<10000000){lcd.print(MWh,1);lcd.print("MWh ");}    //9.9MWh_
  else if(Wh<100000000){lcd.print(MWh,0);lcd.print("MWh  ");}  //99MWh__
  else if(Wh<1000000000){lcd.print(MWh,0);lcd.print("MWh ");}  //999Mwh_
  lcd.setCursor(12,0);lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent);
  int batteryPercentBars;
  batteryPercentBars = batteryPercent/6.18; //6.25 proper value
  lcd.setCursor(0,1);
  for(int i=0;i<batteryPercentBars;i++){lcd.print((char)255);} //Battery Bar Blocks    
  for(int i=0;i<16-batteryPercentBars;i++){lcd.print(" ");}    //Battery Blanks
}
void displayConfig4(){
  lcd.setCursor(0,0);lcd.print("TEMPERATURE STAT");
  lcd.setCursor(0,1);lcd.print(temperature);lcd.print((char)223);lcd.print("C");padding100(temperature);
  lcd.setCursor(8,1);lcd.print("FAN");
  lcd.setCursor(12,1);
  if(fanStatus==1){lcd.print("ON ");}
  else{lcd.print("OFF");}
}
void displayConfig5(){
  lcd.setCursor(0,0);lcd.print(" SETTINGS MENU  ");
  lcd.setCursor(0,1);lcd.print("--PRESS SELECT--");
}

void factoryResetMessageLCD(){
  lcd.setCursor(0,0);lcd.print("  FACTORY RESET ");
  lcd.setCursor(0,1);lcd.print("   SUCCESSFUL   ");
  delay(1000);
}
void savedMessageLCD(){
//  lcd.setCursor(0,0);lcd.print(" SETTINGS SAVED ");
//  lcd.setCursor(0,1);lcd.print(" SUCCESSFULLY   ");
//  delay(500);
//  lcd.clear();
}
void cancelledMessageLCD(){
//  lcd.setCursor(0,0);lcd.print(" SETTINGS       ");
//  lcd.setCursor(0,1);lcd.print(" CANCELLED      ");
//  delay(500);
//  lcd.clear();
}

////////////////////////////////////////////  MAIN LCD MENU CODE /////////////////////////////////////////////
void LCD_Menu(){
  int 
  menuPages          = 4,
  subMenuPages       = 12,
  longPressTime      = 3000,
  longPressInterval  = 500,
  shortPressInterval = 100;

  //SETTINGS MENU
  if(settingMode==1){
    chargingPause = 1;

    //BUTTON KEYPRESS
    if(setMenuPage==MENU_PAGE_SUPPLY_ALGORYTHM){
      if(keyDown(buttonRight)){subMenuPage++;}
      if(keyDown(buttonLeft)) {subMenuPage--;}
      if(keyDown(buttonBack)) {settingMode=0;subMenuPage=0;}  //bool engage, main menu int page
      if(keyDown(buttonSelect)){setMenuPage=1;} //enter sub menu settings - bool engage 
    } 
    //SUB MENU PAGE CYCLING
    if(subMenuPage>subMenuPages){subMenuPage=0;}                     
    else if(subMenuPage<0){subMenuPage=subMenuPages;}  
    //--------------------------- SETTINGS MENU PAGES: ---------------------------// 
    ///// SETTINGS MENU ITEM: SUPPLY ALGORITHM SELECT /////
    if(subMenuPage==0){
      lcd.setCursor(0,0);lcd.print("SUPPLY ALGORITHM");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(MPPT_Mode==1){lcd.print("MPPT + CC-CV  ");}
      else{lcd.print("CC-CV ONLY    ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = MPPT_Mode;}
      else{
        if(keyDown(buttonRight)||keyDown(buttonLeft)){
          if(MPPT_Mode==1){MPPT_Mode=0;}else{MPPT_Mode=1;}
        }
        if(keyDown(buttonBack)){MPPT_Mode = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}
      }     
    }

    ///// SETTINGS MENU ITEM: CHARER/PSU MODE /////
    else if(subMenuPage==MENU_PAGE_MODE){
      lcd.setCursor(0,0);lcd.print("CHARGER/PSU MODE");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(output_Mode==1){lcd.print("CHARGER MODE  ");}
      else{lcd.print("PSU MODE      ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = output_Mode;}
      else{
        if(keyDown(buttonRight)||keyDown(buttonLeft)){
          if(output_Mode==1){output_Mode=0;}else{output_Mode=1;}
        }
        if(keyDown(buttonBack)){output_Mode = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}
      }     
    }

    
    ///// SETTINGS MENU ITEM: MAX BATTERY V /////
    else if(subMenuPage==MENU_PAGE_MAX_BATTERY){
      lcd.setCursor(0,0);lcd.print("MAX BATTERY V   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);lcd.print("V");  
      lcd.print("                ");  

      //SET MENU - FLOATTYPE
      if(setMenuPage==0){floatTemp = voltageBatteryMax;}
      else{
        if(keyDown(buttonBack)){voltageBatteryMax = floatTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}     
        currentMenuSetMillis = millis();
        if(keyDown(buttonRight)){                                                    //Right button press (increments setting values)
          while(keyHold(buttonRight)){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMax += 1.00;                                                    //Increment by 1
              voltageBatteryMax = constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                           //Short Press  
              voltageBatteryMax += 0.01;                                                    //Increment by 0.01
              voltageBatteryMax = constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(shortPressInterval);  //Display settings data                            
            }  
            lcd.print("V   ");                                                              //Display unit
          } 
        }
        else if(keyDown(buttonLeft)){                                                //Left button press (decrements setting values)
          while(keyHold(buttonLeft)){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMax -= 1.00;                                                    //Increment by 1
              voltageBatteryMax = constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                            //Short Press  
              voltageBatteryMax -= 0.01;                                                     //Increment by 0.01
              voltageBatteryMax =  constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(shortPressInterval);   //Display settings data                                      
            } 
            lcd.print("V   ");                                                               //Display unit
          } 
        }
      }    
    }
    ///// SETTINGS MENU ITEM: MIN BATTERY V /////
    else if(subMenuPage==MENU_PAGE_MIN_BATTERY){
      lcd.setCursor(0,0);lcd.print("MIN BATTERY V   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);lcd.print("V");  
      lcd.print("                ");  

      //SET MENU - FLOATTYPE
      if(setMenuPage==0){floatTemp = voltageBatteryMin;}
      else{
        if(keyDown(buttonBack)){while(keyHold(buttonBack)){}voltageBatteryMin = floatTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)==1){while(keyHold(buttonSelect)){}saveSettings();setMenuPage=0;savedMessageLCD();}     
        currentMenuSetMillis = millis();
        if(keyDown(buttonRight)){                                                    //Right button press (increments setting values)
          while(keyHold(buttonRight)){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMin += 1.00;                                                    //Increment by 1
              voltageBatteryMin = constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                           //Short Press  
              voltageBatteryMin += 0.01;                                                    //Increment by 0.01
              voltageBatteryMin = constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(shortPressInterval);  //Display settings data                            
            }  
            lcd.print("V   ");                                                              //Display unit
          } 
        }
        else if(keyDown(buttonLeft)){                                                //Left button press (decrements setting values)
          while(keyHold(buttonLeft)){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMin -= 1.00;                                                    //Increment by 1
              voltageBatteryMin = constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                            //Short Press  
              voltageBatteryMin -= 0.01;                                                     //Increment by 0.01
              voltageBatteryMin =  constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(shortPressInterval);   //Display settings data                                      
            } 
            lcd.print("V   ");                                                               //Display unit
          } 
        }
      }    
    }
    ///// SETTINGS MENU ITEM: CHARGING CURRENT /////
    else if(subMenuPage==MENU_PAGE_CHARGING_CURRENT){
      lcd.setCursor(0,0);lcd.print("CHARGING CURRENT");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(currentCharging,2);lcd.print("A");  
      lcd.print("                ");  

      //SET MENU - FLOATTYPE
      if(setMenuPage==0){floatTemp = currentCharging;}
      else{
        if(keyDown(buttonBack)){currentCharging = floatTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}     
        currentMenuSetMillis = millis();
        if(keyDown(buttonRight)){                                                  //Right button press (increments setting values)
          while(keyHold(buttonRight)){
            if(millis()-currentMenuSetMillis>longPressTime){                              //Long Press     
              currentCharging += 1.00;                                                    //Increment by 1
              currentCharging = constrain(currentCharging,0.0,cOutSystemMax);             //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                         //Short Press  
              currentCharging += 0.01;                                                    //Increment by 0.01
              currentCharging = constrain(currentCharging,0.0,cOutSystemMax);             //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(shortPressInterval);  //Display settings data                            
            }  
            lcd.print("A   ");                                                            //Display unit
          } 
        }
        else if(keyDown(buttonLeft)){                                              //Left button press (decrements setting values)
          while(keyHold(buttonLeft)){
            if(millis()-currentMenuSetMillis>longPressTime){                              //Long Press     
              currentCharging -= 1.00;                                                    //Increment by 1
              currentCharging = constrain(currentCharging,0.0,cOutSystemMax);             //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                         //Short Press  
              currentCharging -= 0.01;                                                    //Increment by 0.01
              currentCharging =  constrain(currentCharging,0.0,cOutSystemMax);            //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(shortPressInterval);  //Display settings data                                      
            } 
            lcd.print("A   ");                                                            //Display unit
          } 
        }
      } 
    }
    ///// SETTINGS MENU ITEM: COOLING FAN /////
    else if(subMenuPage==MENU_PAGE_FAN_ENABLE){
      lcd.setCursor(0,0);lcd.print("COOLING FAN     ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(enableFan==1){lcd.print("ENABLED       ");}
      else{lcd.print("DISABLE         ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = enableFan;}
      else{
        if(keyDown(buttonRight)||keyDown(buttonLeft)){
          if(enableFan==1){enableFan=0;}else{enableFan=1;}
        }
        if(keyDown(buttonBack)){enableFan = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}
      } 
    }
    ///// SETTINGS MENU ITEM: FAN TRIG TEMP /////
    else if(subMenuPage==MENU_PAGE_FAN_LOW_TEMP){
      lcd.setCursor(0,0);lcd.print("FAN TRIGGER TEMP");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(temperatureFan);
      lcd.print((char)223);lcd.print("C");lcd.print("                ");  

      //SET MENU - INTTYPE
      if(setMenuPage==0){intTemp = temperatureFan;}
      else{
        if(keyDown(buttonBack)){temperatureFan = intTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}     
        if(keyDown(buttonRight)){                                              //Right button press (increments setting values)
          while(keyHold(buttonRight)){   
            temperatureFan++;                                                       //Increment by 1
            temperatureFan = constrain(temperatureFan,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureFan);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
        else if(keyDown(buttonLeft)){                                        //Left button press (decrements setting values)
          while(keyHold(buttonLeft)){ 
            temperatureFan--;                                                       //Increment by 1
            temperatureFan = constrain(temperatureFan,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureFan);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
      }         
    }
    ///// SETTINGS MENU ITEM: FAN TRIG TEMP /////
    else if(subMenuPage==MENU_PAGE_FAN_HIGH_TEMP){
      lcd.setCursor(0,0);lcd.print("FAN HIGH TEMP");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(temperatureFanMax);
      lcd.print((char)223);lcd.print("C");lcd.print("                ");  

      //SET MENU - INTTYPE
      if(setMenuPage==0){intTemp = temperatureFanMax;}
      else{
        if(keyDown(buttonBack)){temperatureFanMax = intTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}     
        if(keyDown(buttonRight)){                                              //Right button press (increments setting values)
          while(keyHold(buttonRight)){   
            temperatureFanMax++;                                                       //Increment by 1
            temperatureFanMax = constrain(temperatureFanMax,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureFanMax);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
        else if(keyDown(buttonLeft)){                                        //Left button press (decrements setting values)
          while(keyHold(buttonLeft)){ 
            temperatureFanMax--;                                                       //Increment by 1
            temperatureFanMax = constrain(temperatureFanMax,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureFanMax);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
      }         
    }
    ///// SETTINGS MENU ITEM: SHUTDOWN TEMP /////
    else if(subMenuPage==MENU_PAGE_SHUTDOWN_TEMP){
      lcd.setCursor(0,0);lcd.print("SHUTDOWN TEMP   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(temperatureMax);
      lcd.print((char)223);lcd.print("C");lcd.print("                ");  

      //SET MENU - INTTYPE
      if(setMenuPage==0){intTemp = temperatureMax;}
      else{
        if(keyDown(buttonBack)){temperatureMax = intTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}     
        if(keyDown(buttonRight)){                                              //Right button press (increments setting values)
          while(keyHold(buttonRight)){   
            temperatureMax++;                                                       //Increment by 1
            temperatureMax = constrain(temperatureMax,0,120);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureMax);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
        else if(keyDown(buttonLeft)){                                        //Left button press (decrements setting values)
          while(keyHold(buttonLeft)){ 
            temperatureMax--;                                                       //Increment by 1
            temperatureMax = constrain(temperatureMax,0,120);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureMax);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
      }       
    }
    ///// SETTINGS MENU ITEM: WIFI FEATURE /////
    else if(subMenuPage==MENU_PAGE_WIFI){
      lcd.setCursor(0,0);lcd.print("WIFI FEATURE    ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(enableWiFi==1){lcd.print("ENABLED       ");}
      else{lcd.print("DISABLED      ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = enableWiFi;}
      else{
        if(keyDown(buttonRight)||keyDown(buttonLeft)){
          if(enableWiFi==1){enableWiFi=0;}else{enableWiFi=1;}
        }
        if(keyDown(buttonBack)==1){enableWiFi = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)==1){saveSettings();setMenuPage=0;savedMessageLCD();}
      }       
    }

    ///// SETTINGS MENU ITEM: AUTOLOAD /////
    else if(subMenuPage==MENU_PAGE_AUTOLOAD){
      lcd.setCursor(0,0);lcd.print("AUTOLOAD FEATURE");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(flashMemLoad==1){lcd.print("ENABLED       ");}
      else{lcd.print("DISABLED      ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = flashMemLoad;}
      else{
        if(keyDown(buttonRight)||keyDown(buttonLeft)){
          if(flashMemLoad==1){flashMemLoad=0;}else{flashMemLoad=1;}
        }
        if(keyDown(buttonBack)){flashMemLoad = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveAutoloadSettings();setMenuPage=0;savedMessageLCD();}
      }       
    }
    ///// SETTINGS MENU ITEM: BACKLIGHT SLEEP /////
    else if(subMenuPage==MENU_PAGE_BACKLIGHT_SLEEP){
      lcd.setCursor(0,0);lcd.print("BACKLIGHT SLEEP ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);
      if(backlightSleepMode==1)     {lcd.print("10 SECONDS    ");}
      else if(backlightSleepMode==2){lcd.print("5 MINUTES     ");}
      else if(backlightSleepMode==3){lcd.print("1 HOUR        ");}
      else if(backlightSleepMode==4){lcd.print("6 HOURS       ");}  
      else if(backlightSleepMode==5){lcd.print("12 HOURS      ");}  
      else if(backlightSleepMode==6){lcd.print("1 DAY         ");}
      else if(backlightSleepMode==7){lcd.print("3 DAYS        ");}
      else if(backlightSleepMode==8){lcd.print("1 WEEK        ");}
      else if(backlightSleepMode==9){lcd.print("1 MONTH       ");}     
      else{lcd.print("NEVER         ");}    
      
      //SET MENU - INTMODETYPE
      if(setMenuPage==0){intTemp = backlightSleepMode;}
      else{
        if(keyDown(buttonBack)){backlightSleepMode = intTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(keyDown(buttonSelect)){saveSettings();setMenuPage=0;savedMessageLCD();}     
        if(keyDown(buttonRight)){                                                    //Right button press (increments setting values)
          backlightSleepMode++;                                                           //Increment by 1
          backlightSleepMode = constrain(backlightSleepMode,0,9);                         //Limit settings values to a range
          lcd.setCursor(2,1);
          if(backlightSleepMode==1)     {lcd.print("10 SECONDS    ");}
          else if(backlightSleepMode==2){lcd.print("5 MINUTES     ");}
          else if(backlightSleepMode==3){lcd.print("1 HOUR        ");}
          else if(backlightSleepMode==4){lcd.print("6 HOURS       ");}  
          else if(backlightSleepMode==5){lcd.print("12 HOURS      ");}  
          else if(backlightSleepMode==6){lcd.print("1 DAY         ");}
          else if(backlightSleepMode==7){lcd.print("3 DAYS        ");}
          else if(backlightSleepMode==8){lcd.print("1 WEEK        ");}
          else if(backlightSleepMode==9){lcd.print("1 MONTH       ");}     
          else{lcd.print("NEVER         ");}    
        }
        else if(keyDown(buttonLeft)==1){                                              //Left button press (decrements setting values)
          backlightSleepMode--;                                                           //Increment by 1
          backlightSleepMode = constrain(backlightSleepMode,0,9);                         //Limit settings values to a range
          lcd.setCursor(2,1);
          if(backlightSleepMode==1)     {lcd.print("10 SECONDS    ");}
          else if(backlightSleepMode==2){lcd.print("5 MINUTES     ");}
          else if(backlightSleepMode==3){lcd.print("1 HOUR        ");}
          else if(backlightSleepMode==4){lcd.print("6 HOURS       ");}  
          else if(backlightSleepMode==5){lcd.print("12 HOURS      ");}  
          else if(backlightSleepMode==6){lcd.print("1 DAY         ");}
          else if(backlightSleepMode==7){lcd.print("3 DAYS        ");}
          else if(backlightSleepMode==8){lcd.print("1 WEEK        ");}
          else if(backlightSleepMode==9){lcd.print("1 MONTH       ");}     
          else{lcd.print("NEVER         ");}    
        }
      }         
    }
    ///// SETTINGS MENU ITEM: FACTORY RESET /////
    else if(subMenuPage==MENU_PAGE_FACTORY_RESET){
      if(setMenuPage==0){
        lcd.setCursor(0,0);lcd.print("FACTORY RESET   ");
        lcd.setCursor(0,1);lcd.print("> PRESS SELECT  ");  
      }
      else{
        if(confirmationMenu==0){lcd.setCursor(0,0);lcd.print(" ARE YOU SURE?  ");lcd.setCursor(0,1);lcd.print("  >NO      YES  ");}  // Display ">No"
        else{lcd.setCursor(0,0);lcd.print(" ARE YOU SURE?  ");lcd.setCursor(0,1);lcd.print("   NO     >YES  ");}                     // Display ">YES"
        if(keyDown(buttonRight)||keyDown(buttonLeft)){if(confirmationMenu==0){confirmationMenu=1;}else{confirmationMenu=0;}}  //Cycle Yes NO
        if(keyDown(buttonBack)){cancelledMessageLCD();setMenuPage=0;confirmationMenu=0;} //Cancel
        if(keyDown(buttonSelect)){if(confirmationMenu==1){factoryReset();factoryResetMessageLCD();}setMenuPage=0;confirmationMenu=0;subMenuPage=0;}
      } 
    }  
    ///// SETTINGS MENU ITEM: FIRMWARE VERSION /////
    else if(subMenuPage==MENU_PAGE_FIRMWARE){
      if(setMenuPage==0){
        lcd.setCursor(0,0);lcd.print("FIRMWARE VERSION");
        lcd.setCursor(0,1);lcd.print(firmwareInfo);    
        lcd.setCursor(8,1);lcd.print(firmwareDate); 

        
      }
      else{
        lcd.setCursor(0,0);lcd.print(firmwareContactR1);
        lcd.setCursor(0,1);lcd.print(firmwareContactR2);          
        if(keyDown(buttonBack)||keyDown(buttonSelect)){setMenuPage=0;} //Cancel
      } 
    }  
  }
  //MAIN MENU
  else if(settingMode==0){
    chargingPause = 0;

    //LCD BACKLIGHT SLEEP
    lcdBacklight();
   
    currentLCDMillis = millis();
    if(currentLCDMillis-prevLCDMillis>=millisLCDInterval&&enableLCD==1){   //Run routine every millisLCDInterval (ms)
      prevLCDMillis = currentLCDMillis;     

      //MENU PAGE BUTTON ACTION
      if(keyDown(buttonRight))      {buttonRightCommand=0;menuPage++;lcd.clear();}
      else if(keyDown(buttonLeft))  {buttonLeftCommand=0;menuPage--;lcd.clear();}
      else if(keyDown(buttonBack))  {buttonBackCommand=0;menuPage=0;lcd.clear();}
      else if(keyDown(buttonSelect) &&menuPage==4){buttonSelectCommand=0;settingMode=1;lcd.clear();}
      if(menuPage>menuPages){menuPage=0;}
      else if(menuPage<0){menuPage=menuPages;}  

      if(menuPage==0)     {displayConfig1();}
      else if(menuPage==1){displayConfig2();}
      else if(menuPage==2){displayConfig3();}
      else if(menuPage==3){displayConfig4();}
      else if(menuPage==4){displayConfig5();}
    }    
  }
}
