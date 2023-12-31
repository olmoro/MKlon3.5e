/*
    Набор методов, доступных разработчику для программирования собственных
 режимов работы прибора.

    20231216          
*/

#include "mtools.h"
#include "project_config.h"
#include "mcmd.h"
#include "driver/mcommands.h"
#include "board/mboard.h"
#include "mdispatcher.h"
#include "display/mdisplay.h"
#include "driver/mcommands.h"
#include <Preferences.h>
#include "Arduino.h"

    
MTools::MTools(MBoard * board, MDisplay * display) : 
  Board(board),
  Display(display),
  qPreferences(new Preferences) {}

MTools::~MTools()
{
  delete qPreferences;
}
  //analog:
  //MTools::MTools(MBoard * board, MDisplay * display, MKeyboard * keyboard) 
  //{
  //      Board = board;
  //      Display = display; 
  //      Keyboard = keyboard;
  //}

    // Флаг блокировки обмена с драйвером на время его рестарта
bool MTools::getBlocking()                      {return blocking;}
void MTools::setBlocking(bool bl)               {blocking = bl;}

    // Флаг выбора режима коррекции смещения АЦП
bool MTools::getTuningAdc()                     {return tuningAdc;}
void MTools::setTuningAdc(bool tu)              {tuningAdc = tu;}

void  MTools::setPostpone(unsigned short hour)  {postpone = hour;}
unsigned short MTools::getPostpone()            {return postpone;}

  // Напряжение и ток без преобразования
short beautyV = 2; 

void MTools::setMilliVolt(short val)
{
  // (sum += val - (sum >> beauty)) >> beauty
  static int32_t sum = 0;
  //beautyV = 2;
  val = (sum += val - (sum >> beautyV)) >> beautyV;

  milliVolt = val;
  voltage = (float)val / 1000;
}

short MTools::getMilliVolt() {return milliVolt;}

short beautyI = 2; 

void MTools::setMilliAmper(short val)
{
  // (sum += val - (sum >> beauty)) >> beauty
  static int32_t sum = 0;
  //beautyI = 2;
  val = (sum += val - (sum >> beautyI)) >> beautyI;
 
  milliAmper = val;
  current = (float)val / 1000;
}


short   MTools::getMilliAmper()                 {return milliAmper;}

  // Текущие целочисленные в мВ и мА напряжение и ток преобразуются в вольты и амперы 
void    MTools::setVoltageVolt(short val)       {voltage = (float)val / 1000;}
float   MTools::getVoltageVolt()                {return voltage;}
void    MTools::setCurrentAmper(short val)      {current = (float)val / 1000;}
float   MTools::getCurrentAmper()               {return current;}

void    MTools::setCelsius(short val)           {celsius = val;}

void  MTools::setState(unsigned short val)      {state = val;}
unsigned short MTools::getState()               {return state;}

void  MTools::setErr(short err)                 {error = err;}
short MTools::getErr()                          {return error;}

void  MTools::setReady(short value)             {ready = value;}
short MTools::getReady()                        {return ready;}

  // Диспетчер
void MTools::aboutMode(short modeSelection)
{
  char sMode[20] = {0};           // до 26?
  char sHelp[20] = {0};
  char sVers[20] = {0};

  switch(modeSelection)
  {
    case MDispatcher::BOOT:
      sprintf(sMode, "BOOT" );
      sprintf(sHelp, ". . . wait . . ." );
    break;

    case MDispatcher::OPTIONS:
      sprintf(sMode, "OPTION" );
      sprintf(sHelp, "User parameters" );
    break;

    case MDispatcher::SAMPLE:
      sprintf(sMode, "SAMPLE" );
      sprintf(sHelp, "Example" );
    break;

    case MDispatcher::CCCV:
      sprintf(sMode, "CCCV" );
      sprintf(sHelp, "Main charge" );
    break;

    case MDispatcher::DISCHARGE:
      sprintf(sMode, "DISCHARGE:");
      sprintf(sHelp, "Testing");
    break;

    case MDispatcher::DEVICE:
      sprintf(sMode, "DEVICE");   // Настройки с доступом (?) разработчика (заводской доступ)
      sprintf(sHelp, "Calibrations");
    break;

    default:
      sprintf(sMode, "ERROR" );
      sprintf(sHelp, "UNIDENTIFIED" );
    break;
  }

  // Данные о выбираемом режиме на дисплей
  Display->drawLabel( sMode, 2 );           // Вывод названия режима
  Display->drawLabel( sHelp, 3 );           // Добавление краткого описания режима

  Display->drawLabel( sVers, 7 );           // Версия
    // Остальные строки очищаются
  Display->clearLine( 0, 1 ); 
  Display->clearLine( 4, 6 ); 
}

// Вариант загрузчика
void MTools::boot()
{
  setBlocking(true);                    // Блокировать обмен
  vTaskDelay(1000/portTICK_PERIOD_MS);  // Не беспокоим драйвер (3) секунды после рестарта 
  setBlocking(false);                   // Разблокировать обмен
//  txGetPidTreaty();                     // 0x47 : Get short, short, short   nu
  txSetFactorU(readNvsShort("device", "factorV", MPrj::factor_v_default));  // 0x31 : Set short
  txSetSmoothU(readNvsShort("device", "smoothV", MPrj::smooth_v_default));  // 0x34 : Set byte
  txSetShiftU(readNvsShort ("device", "offsetV", MPrj::shift_v_default));   // 0x36 : Set short
  txSetFactorI(readNvsShort("device", "factorI", MPrj::factor_i_default));  // 0x39 : Set short
  txSetSmoothI(readNvsShort("device", "smoothI", MPrj::smooth_i_default));  // 0x3C : Set byte
  txSetShiftI(readNvsShort ("device", "offsetI", MPrj::shift_i_default));   // 0x3E : Set short
  txSetPidReconfig( MPrj::RU, 
                    readNvsFloat("device", "kpC", MPrj::kp_c_default),
                    readNvsFloat("device", "kiC", MPrj::ki_c_default),
                    readNvsFloat("device", "kdC", MPrj::kd_c_default),
                    MPrj::pwm_min, MPrj::pwm_max );                         // 0x43
  txSetPidReconfig( MPrj::RD,
                    readNvsFloat("device", "kpD", MPrj::kp_d_default),
                    readNvsFloat("device", "kiD", MPrj::ki_d_default),
                    readNvsFloat("device", "kdD", MPrj::kd_d_default),
                    MPrj::dac_min, MPrj::dac_max );                         // 0x43
}




bool  MTools::getAP()                           {return false;}

float MTools::getRealVoltage()                  {return voltage;}  //-21
float MTools::getRealCurrent()                  {return current;}  //-21

  // Подсчет ампер-часов
void  MTools::zeroAhCounter()         {timeCounter = 0; ahCharge = 0.0f;} // Обнуление счетчика ампер-часов заряда
int   MTools::getChargeTimeCounter()  { return chargeTimeCounter;}
float MTools::getAhCharge()           {return ahCharge;}
void  MTools::chargeCalculations()
{
  timeCounter++;
  chargeTimeCounter = ((int)timeCounter / 10);
  ahCharge += current / 36000.0;
}

// // Предвычисления коэффициентов пид-регуляторов.
// unsigned short MTools::kpToShort(float p)
//   {return (unsigned short)(p * MPrj::par_mult);}
// unsigned short MTools::kiToShort(float i)
//   {return (unsigned short)(((i * MPrj::par_mult) + (MPrj::pid_hz >> 1)) / MPrj::pid_hz);}
// unsigned short MTools::kdToShort(float d)
//   {return (unsigned short)((d * MPrj::par_mult) *  MPrj::pid_hz);}

// Предвычисления коэффициентов пид-регуляторов.
// unsigned short MTools::kpToShort(float p) {return (unsigned short)(p * MPrj::par_mult);}
// unsigned short MTools::kiToShort(float i) {return (unsigned short)(i * MPrj::par_mult);}
// unsigned short MTools::kdToShort(float d) {return (unsigned short)(d * MPrj::par_mult);}




// void MTools::calkKpKiKd(float p, float i, float d)
// {
// #ifdef KIKD
//   // ki вычисляется с округлением
//   kp = kpToShort(p);  // (unsigned short)(p * MPrj::par_mult);
//   ki = kiToShort(i);  //(unsigned short)(((i * MPrj::par_mult) + (MPrj::pid_hz >> 1)) / MPrj::pid_hz);
//   kd = kdToShort(d);  // (unsigned short)((d * MPrj::par_mult) * MPrj::pid_hz);
// #else
//   kp = (unsigned short)(p * MPrj::par_mult);
//   ki = (unsigned short)(i * MPrj::par_mult);
//   kd = (unsigned short)(d * MPrj::par_mult);
// #endif
// }


// ==================================== Nvs read ====================================

bool MTools::readNvsBool(const char * name, const char * key, const bool defaultValue)
{
  qPreferences->begin(name, true);                      // R-mode (second parameter has to be true).
  bool val = qPreferences->getBool(key, defaultValue);
  qPreferences->end();                                  // Close the Preferences
  return val;  
}

short MTools::readNvsShort(const char * name, const char * key, const short defaultValue)
{
  qPreferences->begin(name, true);
  short val = qPreferences->getShort(key, defaultValue);
  qPreferences->end();
  return val;  
}

int MTools::readNvsInt(const char * name, const char * key, const int defaultValue)
{
  qPreferences->begin(name, true);
  int val = qPreferences->getInt(key, defaultValue);
  qPreferences->end();
  return val;  
}

float MTools::readNvsFloat(const char * name, const char * key, const float defaultValue)
{
  qPreferences->begin(name, true);
  float val = qPreferences->getFloat(key, defaultValue);
  qPreferences->end();
  return val;
}

// ==================================== Nvs write ====================================
void MTools::writeNvsBool(const char * name, const char * key, const bool bValue)
{
  qPreferences->begin(name, false);                     // RW-mode (second parameter has to be false).
  qPreferences->putBool(key, bValue);
  qPreferences->end();
}

void MTools::writeNvsShort(const char * name, const char * key, const short sValue)
{
  qPreferences->begin(name, false);
  qPreferences->putShort(key, sValue);
  qPreferences->end();
}

void MTools::writeNvsInt(const char * name, const char * key, const int iValue)
{
  qPreferences->begin(name, false);
  qPreferences->putInt(key, iValue);
  qPreferences->end();
}

void MTools::writeNvsFloat(const char * name, const char * key, const float fValue)
{
  qPreferences->begin(name, false);
  qPreferences->putFloat(key, fValue);
  qPreferences->end();
}

// ================================= Nvs copy/move ===================================
float MTools::copyNvsFloat (const char * src, const char * dst, const char * key, const float defaultValue )
{
  qPreferences->begin(src, false);
  float val = qPreferences->getFloat(key, defaultValue);

  qPreferences->begin(dst, false);
  qPreferences->putFloat(key, val);
  qPreferences->end();
  return val;
}

float MTools::copyNvsFloat (const char * src, const char * dst, const char * key, const char * newkey, const float defaultValue )
{
  qPreferences->begin(src, false);
  float val = qPreferences->getFloat(key, defaultValue);

  qPreferences->begin(dst, false);
  qPreferences->putFloat(newkey, val);
  qPreferences->end();
  return val;
}

// ===================== Clear all keys in opened preferences ========================
bool MTools::clearAllKeys(const char * name)
{
  qPreferences->begin(name, false);                     // RW-mode (second parameter has to be false).
  bool err = qPreferences->clear();
  qPreferences->end();
  return err;
}

// =================================  Remove a key ===================================
void MTools::removeKey(const char * name, const char * key)
{
  qPreferences->begin(name, false);
  qPreferences->remove(key);
  qPreferences->end();
}


void MTools::clrTimeCounter() { timeCounter = 0; }
void MTools::clrChargeTimeCounter() { chargeTimeCounter = 0; }
void MTools::clrAhCharge() { ahCharge = 0; }
int  MTools::getTimeCounter() { return timeCounter; }
void MTools::setTimeCounter( int ivalue ) { timeCounter = ivalue; }


uint8_t buffCmd = MCmd::cmd_nop;             // 0x00 - нет операции

uint8_t MTools::getBuffCmd()            {return buffCmd;}
void    MTools::setBuffCmd(uint8_t cmd) {buffCmd = cmd;}


short MTools::getCooler() {return cool;}
void  MTools::setCooler(short val) {cool = val;}

void  MTools::setAdcV( short val ) { adcV = val; }
void  MTools::setAdcI( short val ) { adcI = val; }

short MTools::getAdcOffset() {return offsetAdc;}
void  MTools::setAdcOffset(short val) {offsetAdc = val;}

short MTools::getAdcV() {return adcV;}
short MTools::getAdcI() {return adcI;}

void MTools::setSync( bool _sync) { sync = _sync; }
bool MTools::getSync() { return sync; }



short MTools::getLtV() {return ltV;}
short MTools::getUpV() {return upV;}
short MTools::getLtI() {return ltI;}
short MTools::getUpI() {return upI;}

// ======================== ЦЕЛЕВЫЕ КОМАНДЫ УПРАВЛЕНИЯ ДРАЙВЕРОМ SAMD21 MO MINI ========================
  //Команды чтения результатов измерений:
void MTools::txReadUIS()            {buffCmd = MCmd::cmd_get_uis;}      // 0x10;
void MTools::txGetU()               {buffCmd = MCmd::cmd_get_u;}        // 0x11 Чтение напряжения (мВ)
void MTools::txGetI()               {buffCmd = MCmd::cmd_get_i;}        // 0x12 Чтение тока (мА)
void MTools::txGetUI()              {buffCmd = MCmd::cmd_get_ui;}       // 0x13 Чтение напряжения (мВ) и тока (мА)
void MTools::txGetState()           {buffCmd = MCmd::cmd_get_state;}    // 0x14 Чтение состояния

void MTools::txReady(short value)
{
  ready = value;
  buffCmd = MCmd::cmd_ready;  // 0x15 Параметры согласованы
  vTaskDelay(80 / portTICK_PERIOD_MS);
}

void MTools::txPowerAuto(short spV, short spI)
{
  setpointU = spV;
  setpointI = spI;
  buffCmd   = MCmd::cmd_power_auto;
  vTaskDelay(80 / portTICK_PERIOD_MS);
} 

  // 0x2B*
void MTools::txAutoCurrentUp(short spV, short spI, short dI)
{
  setpointU = spV;
  setpointI = spI;
  deltaI    = dI;
  buffCmd   = MCmd::cmd_auto_current_up;
  vTaskDelay(80 / portTICK_PERIOD_MS);
} 




  // Команда перевода в безопасный режим (выключение)               // 0x21 
void MTools::txPowerStop()
{
  buffCmd = MCmd::cmd_power_stop;
  vTaskDelay(80 / portTICK_PERIOD_MS);
}


void MTools::txPowerMode(short spV, short spI, uint8_t mode)
{
  setpointU = spV;
  setpointI = spI;
  pidMode   = mode;                   // Выбор режима старта
  buffCmd   = MCmd::cmd_power_mode;
}

void MTools::txPowerVGo(short spV, short spI) {txPowerMode(spV, spI, MPrj::RU);}   // 0x22

void MTools::txPowerIGo(short spV, short spI) {txPowerMode(spV, spI, MPrj::RI);}   // 0x22

//void MTools::txDischargeGo(short spI) {setpointI = spI; buffCmd = MCmd::cmd_discharge_go;} // 0x24
void MTools::txDischargeGo(short spD) {setpointD = spD; buffCmd = MCmd::cmd_discharge_go;} // 0x24

void MTools::txVoltageAdj(short spV) {setpointU = spV; buffCmd = MCmd::cmd_voltage_adj;}    // 0x25
void MTools::txCurrentAdj(short spI) {setpointI = spI; buffCmd = MCmd::cmd_current_adj;}    // 0x26
void MTools::txDiscurrentAdj(short spD) {setpointD = spD; buffCmd = MCmd::cmd_discurrent_adj;} // 0x27


//   // 0x28
// void MTools::txPowerOn() 
// {
//   buffCmd = MCmd::cmd_power_on;
// }

  // 0x29*
void MTools::txPowerOff() 
{
  buffCmd = MCmd::cmd_power_off;
  vTaskDelay(80 / portTICK_PERIOD_MS);
}

  // 0x2A*  Пока это эквивалент 0x29
void MTools::txIdle() 
{
  buffCmd = MCmd::cmd_idle;
  vTaskDelay(80 / portTICK_PERIOD_MS);
}

  // Команды управления измерителями:
    // Множитель преобразования в милливольты
void MTools::txGetFactorU()                           {buffCmd = MCmd::cmd_read_factor_u;}            // 0x30 Чтение
void MTools::txSetFactorU(short val)
{
  factorV = val;   buffCmd = MCmd::cmd_write_factor_u;           // 0x31* Запись
  vTaskDelay(80 / portTICK_PERIOD_MS);
}
void MTools::txSetFactorDefaultU()                    {buffCmd = MCmd::cmd_write_factor_default_u;}   // 0x32 Возврат к заводскому
    // Параметр сглаживания по напряжению
void MTools::txGetSmoothU()                           {buffCmd = MCmd::cmd_read_smooth_u;}            // 0x33 Чтение
void MTools::txSetSmoothU(short val)
{
  smoothV = val;   buffCmd = MCmd::cmd_write_smooth_u;           // 0x34* Запись
  vTaskDelay(80 / portTICK_PERIOD_MS);
}
    // Приборное смещение по напряжению
void MTools::txGetShiftU()                            {buffCmd = MCmd::cmd_read_offset_u;}            // 0x35 Чтение
void MTools::txSetShiftU(short val)
{
  shiftV  = val;   buffCmd = MCmd::cmd_write_offset_u;           // 0x36* Запись
  vTaskDelay(80 / portTICK_PERIOD_MS);
}
    // Множитель преобразования в миллиамперы
void MTools::txGetFactorI()                           {buffCmd = MCmd::cmd_read_factor_i;}            // 0x38 Чтение
void MTools::txSetFactorI(short val)
{
  factorI = val;   buffCmd = MCmd::cmd_write_factor_i;           // 0x39* Запись
  vTaskDelay(80 / portTICK_PERIOD_MS);
}
void MTools::txSetFactorDefaultI()                    {buffCmd = MCmd::cmd_write_factor_default_i;}   // 0x3A Возврат к заводскому
    // Параметр сглаживания по току
void MTools::txGetSmoothI()                           {buffCmd = MCmd::cmd_read_smooth_i;}            // 0x3B Чтение
void MTools::txSetSmoothI(short val)
{
  smoothI = val;   buffCmd = MCmd::cmd_write_smooth_i;           // 0x3C* Запись
  vTaskDelay(80 / portTICK_PERIOD_MS);
}
    // Приборное смещение по току
void MTools::txGetShiftI()                            {buffCmd = MCmd::cmd_read_offset_i;}            // 0x3D Чтение
void MTools::txSetShiftI(short val)
{
  shiftI  = val;   buffCmd = MCmd::cmd_write_offset_i;           // 0x3E* Запись
  vTaskDelay(80 / portTICK_PERIOD_MS);
}

  // Команды работы с ПИД-регулятором (без проверки на max):

void MTools::txSetPidShortC()
{

}




void MTools::txSetPidConfig(uint8_t _m, float p, float i, float d,
                            uint16_t _minOut, uint16_t _maxOut)
{
  pidMode = _m;
  kp      = (unsigned short)(p * MPrj::par_mult);
  ki      = (unsigned short)(i * MPrj::par_mult);
  kd      = (unsigned short)(d * MPrj::par_mult);
  // //calkKpKiKd(p, i, d);
  // kp = kpToShort(p);
  // ki = kiToShort(i);
  // kd = kdToShort(d);
  minOut  = _minOut;
  maxOut  = _maxOut;
  buffCmd = MCmd::cmd_pid_configure;                                                   // 0x40 Запись
}

void MTools::txSetPidCoeff(unsigned short m, float p, float i, float d)    // 0x41 Запись
{
  pidMode = m;
    kp      = (unsigned short)(p * MPrj::par_mult);
    ki      = (unsigned short)(i * MPrj::par_mult);
    kd      = (unsigned short)(d * MPrj::par_mult);
  // //calkKpKiKd(p, i, d);
  // kp = kpToShort(p);  // (unsigned short)(p * MPrj::par_mult);
  // ki = kiToShort(i);  //(unsigned short)(((i * MPrj::par_mult) + (MPrj::pid_hz >> 1)) / MPrj::pid_hz);
  // kd = kdToShort(d);  // (unsigned short)((d * MPrj::par_mult) * MPrj::pid_hz);
  buffCmd = MCmd::cmd_pid_write_coefficients;                              // 0x41 Запись
}

  // 0x41* Запись
void MTools::txSetPidCoeffC(float p, float i, float d)
{
  pidMode = MPrj::RU;

  kp      = (unsigned short) (p * MPrj::par_mult);
  ki      = (unsigned short) (i * MPrj::par_mult);
  kd      = (unsigned short) (d * MPrj::par_mult);

  // //calkKpKiKd(p, i, d);
  // kp = kpToShort(p);
  // ki = kiToShort(i);
  // kd = kdToShort(d);
  buffCmd = MCmd::cmd_pid_write_coefficients;
  vTaskDelay(80 / portTICK_PERIOD_MS);
}


  // 0x41*
void MTools::txSetPidCoeffD(float p, float i, float d)
{
  pidMode = MPrj::RD;
  kp      = (unsigned short) (p * MPrj::par_mult);
  ki      = (unsigned short) (i * MPrj::par_mult);
  kd      = (unsigned short) (d * MPrj::par_mult);
  // //calkKpKiKd(p, i, d);
  // kp = kpToShort(p);
  // ki = kiToShort(i);
  // kd = kdToShort(d);
  buffCmd = MCmd::cmd_pid_write_coefficients;
  vTaskDelay(80 / portTICK_PERIOD_MS);

  // Serial.print("pMult=0x"); Serial.println(pMult, HEX);
  // Serial.print("kp=0x");    Serial.println(kp, HEX);
  // Serial.print("ki=0x");    Serial.println(ki, HEX);
  // Serial.print("kd=0x");    Serial.println(kd, HEX);

} // 0x41

void MTools::txSetPidOutputRange(uint8_t _m, uint16_t _minOut, uint16_t _maxOut)
{
    pidMode = _m;
    minOut  = _minOut;
    maxOut  = _maxOut;
    buffCmd = MCmd::cmd_pid_output_range;                                                             // 0x42 Запись
}

void MTools::txSetPidReconfig(uint8_t _m, float p, float i, float d, 
                              uint16_t _minOut, uint16_t _maxOut)
{
    pidMode = _m;
    kp      = (unsigned short)(p * MPrj::par_mult);
    ki      = (unsigned short)(i * MPrj::par_mult);
    kd      = (unsigned short)(d * MPrj::par_mult);
    //calkKpKiKd(p, i, d);
  // kp = kpToShort(p);
  // ki = kiToShort(i);
  // kd = kdToShort(d);
    minOut  = _minOut;
    maxOut  = _maxOut;
    buffCmd = MCmd::cmd_pid_reconfigure;                                    // 0x43 Запись
    vTaskDelay(80 / portTICK_PERIOD_MS);
}

void MTools::txPidClear()
{
  buffCmd = MCmd::cmd_pid_clear;               // 0x44*
  vTaskDelay(80 / portTICK_PERIOD_MS);
} 

void MTools::txGetPidTreaty()
{
  buffCmd = MCmd::cmd_pid_read_treaty;          // 0x47* Get shift, bits, hz
  vTaskDelay(80 / portTICK_PERIOD_MS);
}

void MTools::txGetPidConfig(uint8_t m)          // 0x48 Get (mode) kP, kI, kD по выбору
{
  // kp      = (unsigned short) (p * MPrj::par_mult);
  // ki      = (unsigned short) (i * MPrj::par_mult);
  // kd      = (unsigned short) (d * MPrj::par_mult);

  buffCmd = MCmd::cmd_pid_read_configure;
}

// // Ввод параметров PID-регулятора для синхронизации            0x4A (резерв)
// void MTools::txSetPidTreaty(unsigned short shift, unsigned short bits, unsigned short hz)
// {
//     // paramShift = shift;
//     // paramBits  = bits;
//     // pidHz      = hz;
//     buffCmd = MCmd::cmd_pid_write_treaty;                                                             // 0x4A Запись
// }

// #ifndef HZ1000
// // Ввод частоты PID-регулятора                                    0x4A
//   void MTools::txSetPidFrequency(unsigned short hz)
//   {
//     pidHz   = hz;
//     buffCmd = MCmd::cmd_pid_write_frequency;                // 0x4A Запись
//     vTaskDelay(80 / portTICK_PERIOD_MS);
//   }
// #endif

// void MTools::txSetCurrent(unsigned short val)     // 0x59
// {
//   pidMode   = MPrj::RI;
//   setpointI = val;
//   buffCmd = MCmd::cmd_write_current;              // 0x59
// }

// 0x4C*
void MTools::txSetPidChargeShort()
{
  buffCmd = MCmd::cmd_pid_set_charge_short;
  vTaskDelay(80 / portTICK_PERIOD_MS);
}

// 0x4D*
void MTools::txSetPidDischargeShort()
 {
  buffCmd = MCmd::cmd_pid_set_discharge_short;
  vTaskDelay(80 / portTICK_PERIOD_MS);
}



//   // 0x5A тестовая проверки регулятора разряда 20230210
// void MTools::txSetDiscurrent(uint8_t m, unsigned short val)
// {
//   pidMode   = m;
//   setpointD = val;
//   buffCmd = MCmd::cmd_write_discurrent;
// }  // doSetDiscurrent();

//   // 0x5A Установка тока разряда                20231022
// void MTools::txSetDiscurrent(unsigned short val)
// {
//   pidMode   = MPrj::RD;
//   setpointD = val;
//   buffCmd = MCmd::cmd_write_discurrent;   // 0x5A
// }  // doSetDiscurrent();


//   // Команды работы с АЦП
// void MTools::txGetProbes()                              {buffCmd = MCmd::cmd_adc_read_probes;}        // 0x50
// void MTools::txGetAdcOffset()                           {buffCmd = MCmd::cmd_adc_read_offset;}        // 0x51  
// void MTools::txSetAdcOffset(short val) {offsetAdc = val; buffCmd = MCmd::cmd_adc_write_offset;}       // 0x52
// void MTools::txAdcAutoOffset()                          {buffCmd = MCmd::cmd_adc_auto_offset;}        // 0x53 nu 

  // Команды управления тестовые

  // Команды задания порогов отключения
void MTools::txGetLtV()                            {buffCmd = MCmd::cmd_get_lt_v;}                    // 0x60
void MTools::txSetLtV(short val)        {ltV = val; buffCmd = MCmd::cmd_set_lt_v;}                    // 0x61
void MTools::txSetLtDefaultV(short val) {ltV = val; buffCmd = MCmd::cmd_set_lt_default_v;}            // 0x62
void MTools::txGetUpV()                            {buffCmd = MCmd::cmd_get_up_v;}                    // 0x63
void MTools::txSetUpV(short val)        {upV = val; buffCmd = MCmd::cmd_set_up_v;}                    // 0x64
void MTools::txSetUpDefaultV(short val) {upV = val; buffCmd = MCmd::cmd_set_up_default_v;}            // 0x65

void MTools::txGetLtI()                            {buffCmd = MCmd::cmd_get_lt_i;}                    // 0x68
void MTools::txSetLtI(short val)        {ltI = val; buffCmd = MCmd::cmd_set_lt_i;}                    // 0x69
void MTools::txSetLtDefaultI(short val) {ltI = val; buffCmd = MCmd::cmd_set_lt_default_i;}            // 0x6A
void MTools::txGetUpI()                            {buffCmd = MCmd::cmd_get_up_i;}                    // 0x6B
void MTools::txSetUpI(short val)        {upI = val; buffCmd = MCmd::cmd_set_up_i;}                    // 0x6C
void MTools::txSetUpDefaultI(short val) {upI = val; buffCmd = MCmd::cmd_set_up_default_i;}            // 0x6D


  // Подсчет задержки пуска
bool MTools::postponeCalculation()
{
    timeCounter--;
    chargeTimeCounter = timeCounter / 10;
    if( chargeTimeCounter == 0 ) return true;
    return false;
}

  // 202207 
short MTools::updnInt(short value, short below, short above, short additives)
{
    value += additives;
    if(value > above) return above;
    if(value < below) return below;
    return value;
}

float MTools::updnFloat(float value, float below, float above, float additives)
{
    value += additives;
    if(value > above) return above;
    if(value < below) return below;
    return value;
}


// Расчет частоты (резерв)
unsigned short MTools::calkPHz(unsigned short pidHz)
{
  return pidHz;
}


unsigned short MTools::getPHz()   {return pidHz;}


    // Дисплей 20230311

void MTools::showAmp(float amp, uint8_t pls)
{
  Display->showAmp(amp, pls);
}
 
void MTools::showAmp(float amp, uint8_t pls, short filtr)
{
  beautyI = filtr;
  Display->showAmp(amp, pls);
}

void MTools::showVolt(float volt, uint8_t pls)
{
  Display->showVolt(volt, pls);
}

void MTools::showVolt(float volt, uint8_t pls, short filtr)
{
  beautyV = filtr;
  Display->showVolt(volt, pls);
}


void MTools::ledStopGo()
{
  ((getState() == getStatusPidVoltage()) |
   (getState() == getStatusPidCurrent()) |
   (getState() == getStatusPidDiscurrent())) ?  /*Green : Red*/
                          Board->ledsGreen() : Board->ledsRed();
}

void MTools::autoStopGo(short spV, short spI)
{
  ((getState() == getStatusPidVoltage()) |
   (getState() == getStatusPidCurrent())) ?  /* 0x21 : 0x20 */
                            txPowerStop() : txPowerAuto(spV, spI);
}

void MTools::autoIdleGo(short spV, short spI)
{
  ((getState() == getStatusPidVoltage()) |
   (getState() == getStatusPidCurrent())) ?  /* 0x2A : 0x20 */
                                 txIdle() : txPowerAuto(spV, spI);
}



void MTools::dischargeStopGo(short spD)
{
  (getState() == getStatusPidDiscurrent()) ?
                             txPowerStop() : txDischargeGo(spD);
}
