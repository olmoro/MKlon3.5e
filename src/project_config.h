/* Константы, общие для проекта MKlon3.5E
    27.12.2023
*/

#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

namespace MPrj
{
/* Согласованные параметры ПИД-регуляторов SAMD21, которые
  могут быть изменены только в случае синхронного изменения
  соответствующих параметров контроллера силового блока. */

  /* - множитель и максимальное значение параметра, передаваемого 
    по последовательному интерфейсу; */
  static constexpr unsigned short par_mult  = 0x0080;
  static constexpr unsigned short par_max   = 0x01FF;
  /* - максимальное значение для задания коэффициентов ПИД-регуляторов; */
  static constexpr      float par_float_max =  5.11f;
  /* - частота ПИД-регулятора, в этом проекте фиксированная, 1 кГц; */
  static constexpr unsigned short pid_hz    = 1000u;
  /* - пределы регулирования DC-DC заряда; частота преобразования
    проверялась на двух значениях: 190 и 480 кГц; */
  static constexpr unsigned short pwm_min   =     0u;
  static constexpr unsigned short pwm_max   =   200u;   // 480kHz
  //static constexpr unsigned short pwm_max =   500u;   // 190kHz
  /* - пределы регулирования DC-DC разряда; */
  static constexpr unsigned short dac_min   = 0x0020;
  static constexpr unsigned short dac_max   = 0x03FF;
//=========================================================================

/* Параметры, автоматически загружаемые по последовательному интерфейсу 
  в качестве "заводских" если иное не установлено в режиме DEVICE. */
  
  /* - множители преобразования результатов измерения в физические 
    величины: милливольты и миллиамперы; */
  static constexpr unsigned short factor_v_default = 0x5326;
  static constexpr unsigned short factor_i_default = 0x7918;
  /* - коэффициенты фильтрации измерителей; */
  static constexpr unsigned short smooth_v_default = 4u;
  static constexpr unsigned short smooth_i_default = 4u;
  /* - приборные смещения измерителей в милливольтах и миллиамперах; */
  static constexpr short shift_v_default           = 0;
  static constexpr short shift_i_default           = 0;
  /* - коэффициенты ПИД-регулятора заряда; */
  static constexpr float kp_c_default = 1.20f;
  static constexpr float ki_c_default = 0.45f;
  static constexpr float kd_c_default = 0.00f;
  /* - коэффициенты ПИД-регулятора разряда; */
  static constexpr float kp_d_default = 0.55f;
  static constexpr float ki_d_default = 0.50f;
  static constexpr float kd_d_default = 0.00f;
  /* - предельные значения тока и напряжения при заряде в данном
    проекте не реализованы; */
  /* - предельные значения тока и напряжения при разряде в данном
    проекте не реализованы; */
  static constexpr short discurrent_default_min  = 50;
  static constexpr short discurrent_default_max  = 5000;
  static constexpr short voltage_off_default_min = 50;
  static constexpr short voltage_off_default_max = 16200;
//=========================================================================

/* Параметры, использующиеся в качестве "заводских" (fixed), если иное 
  не установлено в режиме OPTIONS.*/
  /* - задержка включения в часах; */
  static constexpr unsigned short postpone_fixed  = 0u;
  /* - выдержка заряда в третьей фазе в часах; */
  static constexpr unsigned short timeout_fixed   = 5u;
  /* - номинальное напряжение заряжаемой батареи в милливольтах; */
  static constexpr unsigned short nominal_v_fixed = 12000u;
  /* - номинальная емкость батареи в ампер-часах; */
  static constexpr unsigned short capacity_fixed  = 55u;
  /* - выбор типа батареи по номеру в списке */
  static constexpr unsigned short tec_fixed       = 0u;
//=========================================================================

/* Прочие константы проекта. */

  /* Минимальный ток заряда, "вилка" значений для разных емкостей батареи */
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
