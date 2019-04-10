/*
 *	This Source Code Form is subject to the terms of the Mozilla Public
 *	License, v. 2.0.If a copy of the MPL was not distributed with this
 *	file, You can obtain one at http ://mozilla.org/MPL/2.0/.
 */

/********
 * Fichier: AxisTest.h
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
#include <DynamixelWorkbench.h>
#include "Axis.h"
#include "Arduino.h"

#define testQty 6


class AxisTest
{
private:
    Axis* axis;

    void testCreateAxis(int testID, int ID);
    void testEnableDisableAxis(int testID);
    void testMovingAxis(int testID);
    void testHomingAxis(int testID);
    void testPositionAxis(int testID);
    void testSoftlimit(int testID);
public:
    AxisTest();
    ~AxisTest();

    void run(int ID);

    bool testDone[testQty];
    bool allTestDone;
};


