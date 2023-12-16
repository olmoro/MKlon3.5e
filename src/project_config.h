/* Константы, общие для проекта MKlon3.5E
    15.12.2023
*/

#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

namespace MPrj
{
    // Приборные ограничения:

  // Согласованные параметры измерителей SAMD21:
  static constexpr unsigned short factor_v_default = 0x5326;
  static constexpr unsigned short factor_i_default = 0x7918;
  static constexpr unsigned short smooth_v_default = 4u;
  static constexpr unsigned short smooth_i_default = 4u;
  static constexpr short shift_v_default           = 0;
  static constexpr short shift_i_default           = 0;

  // Параметры разряда
  static constexpr short discurrent_default_min    = 50;
  static constexpr short discurrent_default_max    = 5000;
  static constexpr short voltage_off_default_min   = 50;
  static constexpr short voltage_off_default_max   = 16200;

  
  // Согласованные параметры ПИД-регуляторов SAMD21 (частный случай):
  static constexpr unsigned short par_mult         = 0x8000u;    // pMult
  static constexpr unsigned short par_max          = 0x0001u;    // pMax
  static constexpr          float par_float_max    = 1.0f;
  static constexpr unsigned short pid_hz           = 1000u;
  
  static constexpr float kp_c_default = 0.50f;
  static constexpr float ki_c_default = 0.45f;
  static constexpr float kd_c_default = 0.00f;
   
  static constexpr float kp_d_default = 0.55f;
  static constexpr float ki_d_default = 0.50f;
  static constexpr float kd_d_default = 0.00f;

  // Дефолтные для ввода пользовательских параметров
  static constexpr unsigned short postpone_fixed  =  0u;
  static constexpr unsigned short timeout_fixed   =  5u;
  static constexpr unsigned short nominal_v_fixed = 12000u;
  static constexpr unsigned short capacity_fixed  = 55u;
  static constexpr unsigned short tec_fixed       = 0u;



  // Минимальный ток заряда, "вилка" значений для разных емкостей батареи
  static constexpr short cur_min_lo      = 620u;    // 20u;     // Минимальный ток заряда, mA от
  static constexpr short cur_min_hi      = 700u;    //200u;     // и до

  static constexpr short max_v_fixed     = nominal_v_fixed * 2100 / 2000;
  static constexpr short min_v_fixed     = nominal_v_fixed * 1930 / 2000;
  static constexpr short max_i_fixed     = capacity_fixed * 10 * 10;
  static constexpr short min_i_fixed     = cur_min_lo;
        
  enum ROLES
  {
    RS = 0,    // режим прямого регулирования
    RU,        // режим управления напряжением
    RI,        // режим управления током
    RD         // режим управления током разряда
  };

  // Температурный порог
static constexpr float cool_set_point = 36.6f;

};

#endif //_PROJECT_CONFIG_H_
