char* anti_theft = "OFF";
int blink = 0;
char* value;
int byphone = 0;
int activated = 0;


/*Main function of the anti-theft alarm, see subsequent functions*/
int anti_theft_alarm()
{
  anti_theft_setup();

  while (!display.getButtons(TSButtonLowerLeft))
  {
    anti_theft_loop();
  }
  delay(300);
  return 0;
}

/* Determine what to Setup for the Anti-theft alarm function*/
void anti_theft_setup()
{
  
  SerialMonitorInterface.begin(9600);

  default_display();
}


/*Bluetooth Transceiver Function for the Anti-Theft Alarm*/
void TX_BLE(){
  //Process any ACI commands or events from the main BLE handle
   aci_loop();
   
  // Check if data is available
  if (ble_rx_buffer_len) {
    SerialMonitorInterface.print(ble_rx_buffer_len);
    SerialMonitorInterface.print(" : ");
    value = (char*)ble_rx_buffer;
    SerialMonitorInterface.println(value);
    SerialMonitorInterface.println((char*)ble_rx_buffer);
    //clear afer reading
    ble_rx_buffer_len = 0;
  }  
}

/*Bluetooth Receiver Function for the Anti-Theft Alarm*/
void RX_BLE(){
        char* alert = "ALERT";
        uint8_t sendBuffer[21];
        uint8_t sendLength = 0;
        for (int i = 0; i < strlen(alert); i++)
        {
          SerialMonitorInterface.println(sendBuffer[i]);
          sendBuffer[i] = alert[i];
          sendLength++;
        }
        sendBuffer[sendLength] = '\0';

        //Send ALERT message to the phone via bluetooth
        lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer, sendLength); 
  
}

/*Main Function for integration to stealchain in loop()*/
void anti_theft_loop()
{
  
  TX_BLE(); //Start Transceiver Bluetooth 
  //When the status is ON
  if(blink)
  {
     //Read the motion through Accelerometer
     readUpdated();
     diffInReading();
     activated = willItBlink();

     //Start the Blinking for Alert
     if (activated)
     {
      RX_BLE(); //Reveive Bluetooth values 
        
     }
  }

  // Compare the value get from the bluetooth whether is "ON" or "OFF"
  if (strcmp(value, "ON") == 0 && byphone == 0)
  {
        delay(300);
        anti_theft = "ON";
        ON(value);
        blink = 1;
        byphone = 1;
        value = "";
        
  }else if(strcmp(value, "OFF") == 0 && byphone == 1)
  {
        SerialMonitorInterface.println(anti_theft);
        delay(300);
        display.clearScreen();
        anti_theft = "OFF";
        OFF(value);
        blink = 0;
        stopBlink();
        byphone = 0;
        value = "";
  }
  
  //Check which Button was Pressed
  switch (display.getButtons())
  {
    case TSButtonUpperLeft:
      
      if (strcmp(anti_theft, "OFF") == 0)
      {
         
        delay(300);
        display.clearScreen();
        anti_theft = "ON";
        ON(anti_theft);
        blink = 1;
        byphone = 1;
        
      }else
      {
        delay(300);
        display.clearScreen();
        anti_theft = "OFF";
        OFF(anti_theft);
        blink = 0;
        stopBlink();
        byphone = 0;
      }
      break;
      
    case TSButtonLowerLeft:
      if (blink)
      {
        delay(300);
        display.clearScreen();
        blink = 0;
        stopBlink();
        byphone = 0;
        anti_theft = "OFF";
        default_display();
      }else
      {
        back();
      }
      break;
      
    case TSButtonLowerRight:
       delay(300);
       display.clearScreen();
       blink = 0;
       stopBlink();
       byphone = 0;
       anti_theft = "OFF";
       default_display();
       break;
       
    case TSButtonUpperRight:
       delay(300);
       display.clearScreen();
       blink = 0;
       stopBlink();
       byphone = 0;
       anti_theft = "OFF";
       default_display();
       break;
  }
}

//Default display
void default_display()
{
  
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(0,0);
  display.fontColor(YELLOW,BLACK);
  display.print("< ON");

  display.fontColor(TS_8b_White,TS_8b_Black);
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.setCursor(20,20);
  display.print("Anti Theft");
  
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(35,35);
  display.fontColor(TS_8b_Red,TS_8b_Black);
  display.print(anti_theft);

  back();
}

//ON display
void ON(char* anti_theft){

    readOriginal();
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.setCursor(0,0);
    display.fontColor(YELLOW,BLACK);
    display.print("< OFF");
    display.fontColor(TS_8b_White,TS_8b_Black);
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.setCursor(20,20);
    display.print("Anti Theft");
    display.clearWindow(35,35,96,44);
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.setCursor(35,35);
    display.fontColor(TS_8b_Green,TS_8b_Black);
    display.print(anti_theft);
    back();
  
}

//OFF display
void OFF(char* anti_theft){
    default_display();
}


//Back display
int goBack = 0;
int back(){
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(0,48);
  display.fontColor(YELLOW,BLACK);
  display.print("< Back");
  goBack = 1;
  return goBack;
}
