 /* 
  * Copyright 2019 Étienne Villemure
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <DynamixelWorkbench.h>
#include "p_monitor.h"

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif

String cmd[64];

bool verbose_mode;       // Print additionnal data to serial
void setup()
{
    // Initialisation
    Serial.begin(115200);
    while(!Serial);     // Wait until the serial is ready
    
    cmd[0]="help";

    verbose_mode = false;
}

void loop() 
{
    // Read serial message if available
    if(Serial.available()>0)
    {
        String read_string = Serial.readStringUntil('\n');
        if(verbose_mode == true)
        {
          Serial.println("[CMD] : " + String(read_string));
        }
        
        read_string.trim();
  
        split(read_string, ' ', cmd);
  
        /** Interpreting commands **/
        dynamixel_command(cmd);       //Motor commands
       
        if (cmd[0] == "verbose")      // Verbose mode
        {
          if (cmd [1] == "on")
          {
              verbose_mode = true;
          }
          else if (cmd [1] == "off")
          {
            verbose_mode = false;
          }
          
        }
      
     }
}
