/* MHZ library

    By Tobias Schürg
*/
#ifndef MHZ_H
#define MHZ_H

#include "main.h"

#// status codes
extern const int STATUS_INCOMPLETE;

class MHZ {
 public:
  MHZ(UART_HandleTypeDef * ss);

  int readCO2UART();
  int getLastTemperature();

 private:
  uint8_t temperature;

  UART_HandleTypeDef * _serial;
};

#endif
