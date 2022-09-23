#include <stdio.h>
#include <string.h>
#include "main.h"

#include "usb_device.h"
#include "fatfs.h"

#include "bmp280.h"
#include "MHZ.h"
#include "ssd1306.h"

extern RTC_HandleTypeDef hrtc;


extern "C" void main_cpp(UART_HandleTypeDef * mhz, I2C_HandleTypeDef * hi2c)
{
	BMP280_HandleTypedef bmp280;
	MHZ MHZ(mhz);

	float pressure, temperature, humidity;
	u_int8_t buffer[256];
	int size;

    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    int lastMinutes=1;

	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = hi2c;

	while (!bmp280_init(&bmp280, &bmp280.params)) {
		size = sprintf((char *)buffer, "BMP280 initialization failed\r\n");
		//HAL_UART_Transmit(con, buffer, size, 1000);
		HAL_Delay(2000);
	}
	bool bme280p = bmp280.id == BME280_CHIP_ID;
	size = sprintf((char *)buffer, "BMP280: found %s\r\n", bme280p ? "BME280" : "BMP280");
	//HAL_UART_Transmit(con, buffer, size, 1000);

    ssd1306_Init();

    HAL_Delay(100);

    while(1) {

    	while (!bmp280_read_float(&bmp280, &temperature, &pressure, &humidity)) {
			size = sprintf((char *)buffer, "Temperature/pressure reading failed\r\n");
			//HAL_UART_Transmit(con, buffer, size, 1000);
			HAL_Delay(2000);
		}
		int co2 = MHZ.readCO2UART();

		size = sprintf((char *)buffer,"Temperature: %5.2f C, Humidity: %5.2f %%, Pressure: %7.2f hPa, CO2 %4d ppm\r\n", temperature, humidity, pressure/100, co2);
		//HAL_UART_Transmit(con, buffer, size, 1000);

	    ssd1306_Fill(Black);
	    sprintf((char *)buffer,"%5.2f C", temperature);
	    ssd1306_SetCursor(2, 0);
	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
		sprintf((char *)buffer,"%5.2f %%", humidity);
	    ssd1306_SetCursor(2, 11);
	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
		sprintf((char *)buffer,"%7.2f hPa", pressure/100);
	    ssd1306_SetCursor(2, 22);
	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
		sprintf((char *)buffer,"%4d", co2);
	    ssd1306_SetCursor(62, 0);
	    ssd1306_WriteString((char*)buffer, Font_11x18, White);
		sprintf((char *)buffer,"ppm");
	    ssd1306_SetCursor(107, 7);
	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
	    //ssd1306_UpdateScreen();

	    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		if(((sTime.Minutes % 5)==0) && (sTime.Minutes != lastMinutes)){// 5min
			lastMinutes = sTime.Minutes;
    		size = sprintf((char *)buffer,"%4d-%02d-%02d, %02d:%02d\r\n",
    				sDate.Year+2000, sDate.Month, sDate.Date, sTime.Hours, sTime.Minutes);
    		//HAL_UART_Transmit(con, buffer, size, 1000);
    		size = sprintf((char *)buffer,"Saving...");
    		//HAL_UART_Transmit(con, buffer, size, 1000);

    		sprintf((char *)buffer,">");
    	    ssd1306_SetCursor(107, 22);
    	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
    	    ssd1306_UpdateScreen();


        	uint32_t byteswritten;
        	while(f_mount(&USERFatFS, USERPath, 1) != FR_OK) {
        		HAL_Delay(1000);
        	}
        	if(f_open(&USERFile, "weather.csv", FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
        		Error_Handler();
        	}
        	else
        	{

        		size = sprintf((char *)buffer,"%4d-%02d-%02d, %02d:%02d, %5.2f, %5.2f , %7.2f, %4d\r\n",
        				sDate.Year+2000, sDate.Month, sDate.Date, sTime.Hours, sTime.Minutes,
						temperature, humidity, pressure/100, co2);

        		retUSER = f_lseek(&USERFile, f_size(&USERFile));
        		retUSER = f_write(&USERFile, buffer, size, (UINT*)&byteswritten);

        		if((byteswritten == 0) || (retUSER != FR_OK))
        		{
        			Error_Handler();
        		}
        		else
        		{
        			f_close(&USERFile);
        		}
        	}
    		size = sprintf((char *)buffer,"Done\r\n");
    		//HAL_UART_Transmit(con, buffer, size, 1000);

    		sprintf((char *)buffer,">>");
    	    ssd1306_SetCursor(114, 22);
    	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
    	    ssd1306_UpdateScreen();
    	} else{
    		size = sprintf((char *)buffer,"%02d:%02d", sTime.Hours, sTime.Minutes);
    	    ssd1306_SetCursor(93, 22);
    	    ssd1306_WriteString((char*)buffer, Font_7x10, White);
    	    ssd1306_UpdateScreen();
    	}
    	HAL_Delay(10*1000);     // 10sec
    }
}
