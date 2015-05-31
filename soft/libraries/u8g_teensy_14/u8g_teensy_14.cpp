#include <Arduino.h>
#include "u8glib.h"
#include <spi4teensy3_14.h>

#define _DC  6
#define _RST 7
#define _CS  8

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      //STOP THE DEVICE
    break;

    case U8G_COM_MSG_INIT:
		//INIT HARDWARE INTERFACES, TIMERS, GPIOS...
		pinMode(_CS, OUTPUT);
		pinMode(_RST, OUTPUT);
		pinMode(_DC, OUTPUT);
		//For hardware SPI
		
        spi4teensy3::init();
		
		digitalWrite(_RST, HIGH);
		delay(1);
		// bring reset low
		digitalWrite(_RST, LOW);
		// wait 10ms
		delay(10);
		// bring out of reset
		digitalWrite(_RST, HIGH);
		

		
		
    break;

    case U8G_COM_MSG_ADDRESS:  
      //SWITCH FROM DATA TO COMMAND MODE (arg_val == 0 for command mode)
	  if (arg_val != 0)
      {
          digitalWriteFast(_DC, HIGH);
      }
      else
      {
          digitalWriteFast(_DC, LOW);
      }

    break;
	
	case U8G_COM_MSG_CHIP_SELECT:
		if(arg_val == 0)
		{
			digitalWriteFast(_CS, HIGH);
		}
		else{
			digitalWriteFast(_CS, LOW);
		}
	break;

    case U8G_COM_MSG_RESET:
      //TOGGLE THE RESET PIN ON THE DISPLAY BY THE VALUE IN arg_val
	  digitalWriteFast(_RST, arg_val);
    break;

    case U8G_COM_MSG_WRITE_BYTE:
      //WRITE BYTE TO DEVICE
      uint8_t data[1];
      data[0] = arg_val;
      spi4teensy3::send(data, 1);	
      // SPI.transfer(arg_val);

    break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
	{
      //WRITE A SEQUENCE OF BYTES TO THE DEVICE
	  register uint8_t *ptr = static_cast<uint8_t *>(arg_ptr);
	  while(arg_val > 0){
		spi4teensy3::send(ptr++, 1);
		arg_val--;
	  }
	 }
    break;

  }
  return 1;
}