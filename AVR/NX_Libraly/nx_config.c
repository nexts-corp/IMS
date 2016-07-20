#include <mega2560.h>  
#include <stdio.h>           
#include <string.h>
#include <ctype.h>         
#include <delay.h>

#include "nx_config.h"

void config_mcu()
{
      // Declare your local variables here

      // Crystal Oscillator division factor: 1
      #pragma optsize-
      CLKPR=0x80;
      CLKPR=0x00;
      #ifdef _OPTIMIZE_SIZE_
      #pragma optsize+
      #endif

      // Reset Source checking
      if (MCUSR & 1)
      {
            // Power-on Reset
            MCUSR&=0xE0;
            // Place your code here

      }
      else if (MCUSR & 2)
      {
            // External Reset
            MCUSR&=0xE0;
            // Place your code here

      }
      else if (MCUSR & 4)
      {
            // Brown-Out Reset
            MCUSR&=0xE0;
            // Place your code here

      }
      else if (MCUSR & 8)
      {
            // Watchdog Reset
            MCUSR&=0xE0;
            // Place your code here

      }
      else if (MCUSR & 0x10)
      {
            // JTAG Reset
            MCUSR&=0xE0;
            // Place your code here

      }
      DIDR1=0xff;
      DIDR0=0xff;
}