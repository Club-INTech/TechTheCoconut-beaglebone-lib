#include "eqep.h"
#include "hw/hw_eqep.h"
#include "hw/soc_AM335x.h"
#include <map>

#define EQEP0_REG_ADDR(addr) REG_OFFSET(SOC_EQEP_0_REGS  + addr)
#define EQEP1_REG_ADDR(addr) REG_OFFSET(SOC_EQEP_1_REGS  + addr)
#define EQEP2_REG_ADDR(addr) REG_OFFSET(SOC_EQEP_2_REGS  + addr)

namespace BeagleLib{

  struct EQEP_RegAddr{
    int32_t QPOSCNT;
    int32_t QPOSINIT;
    int32_t QPOSMAX;
    int32_t QEINT;
    int32_t QEPCTL;
    int32_t QDECCTL;
    int32_t QUPRD;
  };
  
  static struct EQEP_RegAddr EQEP_Addrs[]={
    {
      EQEP0_REG_ADDR(EQEP_QPOSCNT),
      EQEP0_REG_ADDR(EQEP_QPOSINIT),
      EQEP0_REG_ADDR(EQEP_QPOSMAX),
      EQEP0_REG_ADDR(EQEP_QEINT),
      EQEP0_REG_ADDR(EQEP_QEPCTL),
      EQEP0_REG_ADDR(EQEP_QDECCTL),
      EQEP0_REG_ADDR(EQEP_QUPRD)
    },
    
    {
      EQEP1_REG_ADDR(EQEP_QPOSCNT),
      EQEP1_REG_ADDR(EQEP_QPOSINIT),
      EQEP1_REG_ADDR(EQEP_QPOSMAX),
      EQEP1_REG_ADDR(EQEP_QEINT),
      EQEP1_REG_ADDR(EQEP_QEPCTL),
      EQEP1_REG_ADDR(EQEP_QDECCTL),
      EQEP1_REG_ADDR(EQEP_QUPRD)
    },
    
    {
      EQEP2_REG_ADDR(EQEP_QPOSCNT),
      EQEP2_REG_ADDR(EQEP_QPOSINIT),
      EQEP2_REG_ADDR(EQEP_QPOSMAX),
      EQEP2_REG_ADDR(EQEP_QEINT),
      EQEP2_REG_ADDR(EQEP_QEPCTL),
      EQEP2_REG_ADDR(EQEP_QDECCTL),
      EQEP2_REG_ADDR(EQEP_QUPRD)
    }
    
  };
  
  EQEP::EQEP(EQEP_ID id) : id_(id)
  {
    EQEP_RegAddr& regs = EQEP_Addrs[id_];
    
    set_reg(regs.QPOSINIT, 0x0);
    set_reg(regs.QPOSMAX, 0xffffffff);
  }

  int32_t EQEP::value()
  {
    return get_reg(EQEP_Addrs[id_].QPOSCNT);
  }

  
}