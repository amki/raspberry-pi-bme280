#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
extern "C" {
#include "bme280.h"
}

#define SPI_READ    0x80
#define SPI_WRITE   0x7F
#define SPI_BUFFER_LEN 5
#define BME280_DATA_INDEX   1
#define BME280_ADDRESS_INDEX    2

struct bme280data {
    /* The variable used to assign the standby time*/
    u8 v_stand_by_time_u8 = BME280_INIT_VALUE;
    /* The variable used to read uncompensated temperature*/
    s32 v_data_uncomp_temp_s32 = BME280_INIT_VALUE;
    /* The variable used to read uncompensated pressure*/
    s32 v_data_uncomp_pres_s32 = BME280_INIT_VALUE;
    /* The variable used to read uncompensated pressure*/
    s32 v_data_uncomp_hum_s32 = BME280_INIT_VALUE;
    /* The variable used to read compensated temperature*/
    s32 v_comp_temp_s32[2] = {BME280_INIT_VALUE, BME280_INIT_VALUE};
    /* The variable used to read compensated pressure*/
    u32 v_comp_press_u32[2] = {BME280_INIT_VALUE, BME280_INIT_VALUE};
    /* The variable used to read compensated humidity*/
    u32 v_comp_humidity_u32[2] = {BME280_INIT_VALUE, BME280_INIT_VALUE};
};

extern bme280data data;

int spi_init();
int spi_rw();

void init_bme280();
