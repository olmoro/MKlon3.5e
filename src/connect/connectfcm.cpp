/*
  Сonnections state machine 
*/

#include "connectfsm.h"
#include "fsbrowser.h"
#include "mtools.h"
#include "board/mboard.h"
#include "board/msupervisor.h"
#include <Arduino.h>

namespace ConnectFsm
{
    MState * MInit::fsm()
    {
        initFSBrowser();
        return new MExe(Tools);
    };

// Что будем отдавать на отображение браузеру (округляет до 0.010, потому в милли)
    MState * MExe::fsm()
    {
    //  short v = Tools->getMilliVolt();
    //  short i = Tools->getMilliAmper();
    //        if(v > 15000)   {  Serial.print("\nV="); Serial.print(v);  }
    //        if(i >  5000)   {  Serial.print("\nI="); Serial.print(i);  }

        runFSBrowser(
              Tools->getMilliVolt(),        // Tools->getRealVoltage(),
              Tools->getMilliAmper(),       // Tools->getRealCurrent(),
              Board->getCelsius(),
              Tools->getAhCharge(), 
              Board->Supervisor->getFanPwm() );
        return this;
    };

};
