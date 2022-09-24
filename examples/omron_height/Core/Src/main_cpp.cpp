#include <stdio.h>
#include <string.h>
#include "main.h"
#include "usb_device.h"
#include "fatfs.h"

#include "Omron2SMPB02E.hpp"

extern "C" void main_cpp(UART_HandleTypeDef * huart2, I2C_HandleTypeDef * hi2c1, I2C_HandleTypeDef * hi2c2, I2C_HandleTypeDef * hi2c3, char *fname)
{
    uint8_t SDO;
    SDO = 1;
    Omron2SMPB02E prs0(hi2c1, SDO);
    Omron2SMPB02E prs1(hi2c2, SDO);
    SDO = 0;
    Omron2SMPB02E prs2(hi2c1, SDO);
    Omron2SMPB02E prs3(hi2c2, SDO);

    u_int8_t buffer[40*50+10];

    HAL_Delay(100);
    prs0.begin();
    prs0.set_mode(MODE_NORMAL);
    prs0.set_average(AVG_8, AVG_32);
    prs0.set_filter(FILTER_8);

    prs1.begin();
    prs1.set_mode(MODE_NORMAL);
    prs1.set_average(AVG_8, AVG_32);
    prs1.set_filter(FILTER_8);

    prs2.begin();
    prs2.set_mode(MODE_NORMAL);
    prs2.set_average(AVG_8, AVG_32);
    prs2.set_filter(FILTER_8);

    prs3.begin();
    prs3.set_mode(MODE_NORMAL);
    prs3.set_average(AVG_8, AVG_32);
    prs3.set_filter(FILTER_8);

    for(int i = 0; 1; i++ ){
        int numbuf = 40*(i%50);
        prs0.read_sensor();
        prs1.read_sensor();
        prs2.read_sensor();
        prs3.read_sensor();
        int height0 = prs0.get_height()*100;
        int height1 = prs1.get_height()*100;
        int height2 = prs2.get_height()*100;
        int height3 = prs3.get_height()*100;
        sprintf((char *)&buffer[numbuf], "%8d, %8d, %8d, %8d\r\n", height0, height1, height2, height3);
        HAL_UART_Transmit(huart2,(uint8_t *)&buffer[numbuf], strlen((char *)&buffer[numbuf]), 3000);

        if((i%50)==49){
        	uint32_t byteswritten;
        	while(f_mount(&USERFatFS, USERPath, 1) != FR_OK) {
        		HAL_Delay(1000);
        	}
        	if(f_open(&USERFile, fname, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
        		Error_Handler();
        	}
        	else
        	{
        		retUSER = f_lseek(&USERFile, f_size(&USERFile));
        		retUSER = f_write(&USERFile, buffer, strlen((const char*)buffer), (UINT*)&byteswritten);
        		if((byteswritten == 0) || (retUSER != FR_OK))
        		{
        			Error_Handler();
        		}
        		else
        		{
        			f_close(&USERFile);
        		}
        	}
            sprintf((char *)buffer, "%d line written\r\n", i+1);
            HAL_UART_Transmit(huart2,(uint8_t *)buffer, strlen((char *)buffer), 3000);
        }
	    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	    HAL_Delay(200);
    }
}
