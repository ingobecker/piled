#ifdef _SIM_

#include <stdint.h>
#include <stdio.h>

#else
#include <avr/io.h>
#endif

/*
  TODO: at the moment, this will work as we don't support more than
        one real hardware-architectur. needs to be refactored that every
        backend supplies it's own types.h file.
*/
