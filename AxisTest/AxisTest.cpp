/*
 *	This Source Code Form is subject to the terms of the Mozilla Public
 *	License, v. 2.0.If a copy of the MPL was not distributed with this
 *	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
 */

/********
 * Fichier: AxisTest.cpp
 * Auteurs: M.-A Martel
 * Date: 8 Avril 2019 (creation)
 * Description:
********/

// ****************************************************************
// ****************************************************************
// ******** WARNING : Use a motor that is not on the robot ********
// ****************************************************************
// ****************************************************************

using namespace std;
#include "Axis.h"
#include "AxisTest.h"
#include <DynamixelWorkbench.h>
#include "Arduino.h"

AxisTest::AxisTest()
{
    allTestDone = 0;
    for(int i = 0; i < testQty; i++)
    {
        testDone[i] = 0;
    }
}

AxisTest::~AxisTest()
{
    delete axis;
}

void AxisTest::run(int ID)
{
    Serial.println("Beginning of Axis class tests");
    Serial.println();
    Serial.println();
    Serial.println("***********************************************************************************");
    Serial.println();

    testCreateAxis(0,ID);
    testEnableDisableAxis(1);
    testMovingAxis(2);
    testHomingAxis(3);
    testPositionAxis(4);
    testSoftlimit(5);

    allTestDone = 1;

    for(int i = 0; i < testQty; i++)
    {
        if(!testDone[i])
        {
            allTestDone = 0;
        }
    }

    if (allTestDone)
    {
        Serial.println("End of Axis class tests");
        Serial.println();
        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();
    }
    
}

void AxisTest::testCreateAxis(int testID, int ID)
{
    if(!testDone[testID])
    {
        Serial.println("We create an object of type Axis and make sure we have a connection");
        Serial.println("The ID should be 1");
        Serial.println();

        axis = new Axis(ID,57600,250,0,180); // Creation of the object
        
        if(axis->ID == 1)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.print("Actual axis ID is : ");
            Serial.println(axis->ID);
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.print("Actual axis ID is : ");
            Serial.println(axis->ID);
        }

        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();

        testDone[testID] = 1;
    }
}

void AxisTest::testEnableDisableAxis(int testID)
{
    if(!testDone[testID])
    {
        Serial.println("Test the Enable fonction");
        Serial.println();
        axis->Enable();
        if(axis->Sts_Enabled)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis enabled successfuly");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Axis did not enable, verify connection or ID");
        }

        Serial.println();
        Serial.println("Test the Disable fonction");
        Serial.println();

        axis->Disable();
        if(!axis->Sts_Enabled)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis disabled successfully");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Axis did not disable, verify connection or ID");
        }

        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();

        testDone[testID] = 1;
    }
}

void AxisTest::testMovingAxis(int testID)
{
    if(!testDone[testID])
    {
        Serial.println("Test the moveAtSpeed function, the stopCmd function and the moving status");
        Serial.println("The motor will run forward, then stop and will do the samething backward");
        Serial.println();

        axis->moveAtSpeed(String(50));

        if(axis->Sts_Moving)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis is moving forward");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("The controler does not see the axis moving forward");
        }

        delay(1000);

        if (axis->Sts_Moving) 
        {
            axis->stopCmd();

            if(!axis->Sts_Moving)
            {
                Serial.println("--------------- SUCCESS ---------------");
                Serial.println("Axis stopped successfully");
            }
            else
            {
                Serial.println("--------------- ERROR ---------------");
                Serial.println("The controler can not stop the axis");
            }
        }

        axis->moveAtSpeed(String(-50));

        if(axis->Sts_Moving)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis is moving backward");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("The controler does not see the axis moving backward");
        }

        delay(1000);

        if (axis->Sts_Moving) 
        {
            axis->stopCmd();

            if(!axis->Sts_Moving)
            {
                Serial.println("--------------- SUCCESS ---------------");
                Serial.println("Axis stopped successfully");
            }
            else
            {
                Serial.println("--------------- ERROR ---------------");
                Serial.println("The controler can not stop the axis");
            }
        }

        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();

        testDone[testID] = 1;
    }
}

void AxisTest::testHomingAxis(int testID)
{
    if(!testDone[testID])
    {
        Serial.println("Test the homing fonction");
        Serial.println("The motor will run backward until it reach the home position switch");
        Serial.println();

        long startTime = millis();
        long actualTime = startTime;
        long maxDelay = 15000;

        while(!axis->Sts_Homed && (actualTime - startTime) < maxDelay)
        {
            pinMode(8,INPUT);
            bool HomeSwitch = digitalRead(8);
            
            long actualTime = millis();

            axis->HomeRequest(&HomeSwitch);
        }

        if(axis->Sts_Homed)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis homed successfully");
        }
        else if((actualTime - startTime) < maxDelay)
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Axis did not home within 15 seconds, verify it can reach home position switch");
        }

        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();

        testDone[testID] = 1;
    }
}

void AxisTest::testPositionAxis(int testID)
{
    if(!testDone[testID])
    {

        long startTime = millis();
        long actualTime = startTime;
        long maxDelay = 15000;

        Serial.println("Test the moveTo function and the position status");
        Serial.println("The motor will move to a specific position and verify that position changes");
        Serial.println();

        
        Serial.print("Actual position is : ");
        Serial.println(axis->getPosition());

        int goalPos = 90;
        axis->Moveto(goalPos);

        if(axis->Sts_Moving)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis is moving to position");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("The controler does not see the axis moving to position");
        }
        
        while(axis->Sts_Moving && (actualTime - startTime) < maxDelay)
        {
            long actualTime = millis();
            axis->verifGoalAchieve();
        }

        Serial.print("New actual position is : ");
        Serial.println(axis->getPosition());

        if(goalPos == axis->getPosition())
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis reached goal position");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Axis did not reach goal position, verify that the motor moves or that the getPosition function works");
        }

        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();

        testDone[testID] = 1;
    }
}

void AxisTest::testSoftlimit(int testID)
{
    if(!testDone[testID])
    {
        long startTime = millis();
        long actualTime = startTime;
        long maxDelay = 15000;

        float goalPos;

        Serial.println("Test the soft limits functions");
        Serial.println("The soft limits will be changed, and attempt to go further than the limit will be made");
        Serial.println();

        Serial.print("Actual minimum soft limit is : ");
        Serial.println(axis->MinSoftlimit);
        int lastMinSoft = axis->MinSoftlimit;
        Serial.print("Actual maximum soft limit is : ");
        Serial.println(axis->MaxSoftlimit);
        int lastMaxSoft = axis->MaxSoftlimit;

        axis->setMinSoftlimit(String(lastMinSoft+10));
        axis->setMinSoftlimit(String(lastMaxSoft+10));

        if(axis->MinSoftlimit != lastMinSoft)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Min soft limit has been changed");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Min soft limit has not been changed");
        }
        
        if(axis->MaxSoftlimit != lastMaxSoft)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Max soft limit has been changed");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Max soft limit has not been changed");
        }

        Serial.println("Trying to go further than minimum soft limit");
        goalPos = axis->MinSoftlimit - 5;
        axis->Moveto(goalPos);
        
        while(axis->Sts_Moving && (actualTime - startTime) < maxDelay)
        {
            long actualTime = millis();
            axis->verifGoalAchieve();
        }

        if(goalPos < axis->getPosition()-2)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis stopped at minimum soft limit switch");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Axis went further than minimum soft limit, verify the moveto fonction");
        }

        Serial.println("Trying to go further than maximum soft limit");
        goalPos = axis->MaxSoftlimit + 5;
        axis->Moveto(goalPos);
        
        while(axis->Sts_Moving && (actualTime - startTime) < maxDelay)
        {
            long actualTime = millis();
            axis->verifGoalAchieve();
        }

        if(goalPos > axis->getPosition()+2)
        {
            Serial.println("--------------- SUCCESS ---------------");
            Serial.println("Axis stopped at maximum soft limit switch");
        }
        else
        {
            Serial.println("--------------- ERROR ---------------");
            Serial.println("Axis went further than maximum soft limit, verify the moveto fonction");
        }

        Serial.println();
        Serial.println("***********************************************************************************");
        Serial.println();

        testDone[testID] = 1;
    }
}