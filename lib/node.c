#include "types.h"

/*
  glue for simulator

  just a quick hack to make the nodes compile for hardware-backends without
  modifications in node-code.

  TODO: check if this leads to "nop" due to optimization, if not
        choose another solution for this issue
*/

#ifndef _SIM_
sim_node_define_context(uint8_t cnt, ...){

}
#endif
