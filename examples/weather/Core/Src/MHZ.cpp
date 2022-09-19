/* MHZ library

    By Tobias Schürg
*/

#include "MHZ.h"
#include "main.h"

MHZ::MHZ(UART_HandleTypeDef * ss) {
	_serial = ss;
}

int MHZ::readCO2UART() {
	uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
	uint8_t response[9];  // for answer

	HAL_UART_Transmit(_serial,(uint8_t *)cmd,9,3000);  // request PPM CO2

	// clear the buffer
	//memset(response, 0, 9);

	HAL_UART_Receive(_serial, (uint8_t *)response, 9, 1000);
	while ( ! ((response[0] == 0xff) & (response[1] == 0x86)) ) {
	  ///_serial->flush();
	  //memset(response, 0, 9);
	  HAL_Delay(100);  // wait a short moment
	  HAL_UART_Transmit(_serial,(uint8_t *)cmd,9,3000);  // request PPM CO2
	  HAL_Delay(100);  // wait a short moment
	  HAL_UART_Receive(_serial, (uint8_t *)response, 9, 1000);
	}

	int ppm_uart = 256 * (int)response[2] + response[3];

	///_serial->flush();
	return ppm_uart;
}

int MHZ::getLastTemperature() {
	return temperature;
}
