#include "sense.h"

char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

int spi_cs0_fd;
int spi_cs1_fd;
unsigned char spi_bitsPerWord = 8;
unsigned int spi_speed = 1000000;

struct bme280_t bme280;
bme280data data;

std::string hexStr(unsigned char *data, int len)
{
    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

int spi_init(int spi_device) {
    int status = -1;
    int *spi_cs_fd;

    unsigned char spi_mode = SPI_MODE_0;

    if(spi_device) {
        spi_cs_fd = &spi_cs1_fd;
        *spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    } else {
        spi_cs_fd = &spi_cs0_fd;
        *spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);
    }
    
    if(*spi_cs_fd < 0) {
        std::cerr << "ERROR: Could not open spi device..." << std::endl;
        return 1;
    }

    status = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status < 0) {
        std::cerr << "ERROR: Could not set WR mode..." << std::endl;
        return 1;
    }
    
    status = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status < 0) {
        std::cerr << "ERROR: Could not set RD mode..." << std::endl;
        return 1;
    }
    
    status = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status < 0) {
        std::cerr << "ERROR: Could not set WR bpw..." << std::endl;
        return 1;
    }
    
    status = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status < 0) {
        std::cerr << "ERROR: Could not set RD bpw..." << std::endl;
        return 1;
    }
    
    status = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status < 0) {
        std::cerr << "ERROR: Could not set WR speed" << std::endl;
        return 1;
    }
    
    status = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status < 0) {
        std::cerr << "ERROR: Could not set RD speed" << std::endl;
        return 1;
    }
    return status;
}

int spi_rw(int spi_device, unsigned char *data, int length) {
    //data      Bytes to write.  Contents is overwritten with bytes read.
    struct spi_ioc_transfer spi;
    int *spi_cs_fd;
    int retVal = -1;

    if (spi_device)
        spi_cs_fd = &spi_cs1_fd;
    else
        spi_cs_fd = &spi_cs0_fd;

    memset(&spi, 0, sizeof (spi));
    spi.tx_buf        = (unsigned long)data; // transmit from "data"
    spi.rx_buf        = (unsigned long)data; // receive into "data"
    spi.len           = length;
    spi.delay_usecs   = 1000;
    spi.speed_hz      = spi_speed;
    spi.bits_per_word = spi_bitsPerWord;
    //spi[i].cs_change = 0;

    retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(1), &spi) ;

    if(retVal < 0)
    {
        std::cerr << "Error - Problem transmitting spi data..";
        return 1;
    }

    return retVal;
}

s8 BME280_SPI_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
    std::cout << "SPI: Writing reg " << hexStr(&reg_addr,1) << " data: " << hexStr(reg_data,cnt) << std::endl;
    s32 iError = BME280_INIT_VALUE;
    u8 buf[SPI_BUFFER_LEN * BME280_ADDRESS_INDEX];
    buf[BME280_INIT_VALUE] = reg_addr;
    u8 index = BME280_INIT_VALUE;

    for(int i=0;i<cnt;++i) {
        index = i * BME280_ADDRESS_INDEX;
        buf[index] = (reg_addr++) &SPI_WRITE;
        buf[index + BME280_DATA_INDEX] = *(reg_data + i);
    }

    iError = spi_rw(dev_addr, buf, SPI_BUFFER_LEN);
    return iError;
}

s8 BME280_SPI_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
    std::cout << "SPI: Reading reg " << hexStr(&reg_addr,1);
    s32 iError = BME280_INIT_VALUE;
    u8 buf[SPI_BUFFER_LEN] = {0,};

    buf[BME280_INIT_VALUE] = reg_addr|SPI_READ;

    iError = spi_rw(dev_addr, buf, SPI_BUFFER_LEN);
    for(int i = 0;i < cnt; i++) {
        *(reg_data + i) = buf[i+BME280_DATA_INDEX];
    }
    std::cout << " Answer: " << hexStr(reg_data,cnt) << std::endl;

    return iError;
}

void BME280_delay_msek(u32 msek) {
    std::cout << "DELAAAY for " << msek << std::endl;
    usleep(msek*1000);
}

void bme280_debug_read() {
    s32 com_rslt = ERROR;
    while(true) {
        // Read data
        /* API is used to read the uncompensated temperature*/
        com_rslt += bme280_read_uncomp_temperature(&data.v_data_uncomp_temp_s32);

        /* API is used to read the uncompensated pressure*/
        com_rslt += bme280_read_uncomp_pressure(&data.v_data_uncomp_pres_s32);

        /* API is used to read the uncompensated humidity*/
        com_rslt += bme280_read_uncomp_humidity(&data.v_data_uncomp_hum_s32);
        /* API is used to compute the compensated temperature*/
        data.v_comp_temp_s32[0] = bme280_compensate_temperature_int32(data.v_data_uncomp_temp_s32);

        /* API is used to compute the compensated pressure*/
        data.v_comp_press_u32[0] = bme280_compensate_pressure_int32(data.v_data_uncomp_pres_s32);

        /* API is used to compute the compensated humidity*/
        data.v_comp_humidity_u32[0] = bme280_compensate_humidity_int32(data.v_data_uncomp_hum_s32);

        std::cout << "Temperature is: " << ((float)data.v_comp_temp_s32[0])/100 << std::endl;
        std::cout << "Pressure is: " << ((float)data.v_comp_press_u32[0])/256 << std::endl;
        std::cout << "Humidity is: " << ((float)data.v_comp_humidity_u32[0])/1024 << std::endl;
        usleep(3000000);
    }
}

void init_bme280() {
    spi_init(0);
    bme280.bus_write = BME280_SPI_bus_write;
    bme280.bus_read = BME280_SPI_bus_read;
    bme280.delay_msec = BME280_delay_msek;
    /* The variable used to assign the standby time*/
    u8 v_stand_by_time_u8 = BME280_INIT_VALUE;
    s32 com_rslt = ERROR;
    com_rslt = bme280_init(&bme280);

    com_rslt += bme280_set_power_mode(BME280_NORMAL_MODE);
    com_rslt += bme280_set_oversamp_humidity(BME280_OVERSAMP_4X);
    com_rslt += bme280_set_oversamp_pressure(BME280_OVERSAMP_4X);
    com_rslt += bme280_set_oversamp_temperature(BME280_OVERSAMP_4X);

    com_rslt += bme280_set_standby_durn(BME280_STANDBY_TIME_1_MS);

    com_rslt += bme280_get_standby_durn(&v_stand_by_time_u8);
    std::cout << "BME280 initialized, rslt: " << com_rslt << std::endl;

    bme280_debug_read();
}

