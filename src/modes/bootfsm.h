#ifndef _BOOTFSM_H_
#define _BOOTFSM_H_

#include "state/mstate.h"
#include "project_config.h"

// #ifdef NEWBOOT



namespace MBoot
{
  class MStart : public MState
  {       
    public:
      MStart(MTools * Tools);
      MState * fsm() override;
  };


#ifdef OLDBOOT

  class MTxPowerStop : public MState
  {
    public:   
      MTxPowerStop(MTools * Tools);
      MState * fsm() override;
  };

  class MTxGetTreaty : public MState
  {
    public:   
      MTxGetTreaty(MTools * Tools);
      MState * fsm() override;
  };

  class MTxsetFactorV : public MState
  {
    public:   
      MTxsetFactorV(MTools * Tools);
      MState * fsm() override;
    private:
      static constexpr unsigned short fixed = MPrj::factor_v_default;   //0x5326;
  };

  class MTxSmoothV : public MState
  {
    public:   
      MTxSmoothV(MTools * Tools);
      MState * fsm() override;
    private:
      static constexpr unsigned short fixed = MPrj::smooth_v_default;
  };

  class MTxShiftV : public MState
  {
    public:   
      MTxShiftV(MTools * Tools);
      MState * fsm() override;
    private:
      static constexpr short fixed = MPrj::shift_v_default;
  };

  class MTxFactorI : public MState
  {
    public:   
      MTxFactorI(MTools * Tools);
      MState * fsm() override;
    private:
      static constexpr unsigned short fixed = MPrj::factor_i_default; //0x7918;
  };

  class MTxSmoothI : public MState
  {
    public:   
      MTxSmoothI(MTools * Tools);
      MState * fsm() override;
    private:
      static constexpr unsigned short fixed = MPrj::smooth_i_default;
  };
  
  class MTxShiftI : public MState
  {
    public:   
      MTxShiftI(MTools * Tools);
      MState * fsm() override;
    private:
      static constexpr short fixed = MPrj::shift_i_default;
  };

  class MDel : public MState
  {
    public:
      MDel(MTools * Tools);
      MState * fsm() override;
  };

#endif // OLDBOOT

  class MExit : public MState
  {
    public:
      MExit(MTools * Tools);
      MState * fsm() override;
  };

};

#endif  //_BOOTFSM_H_
