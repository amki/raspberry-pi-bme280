pip install esptool

Read Chip ID esptool.py --chip esp32 --port COM4 chip_id
Erase Flash  esptool.py --chip esp32 --port COM4 erase_flash
Write micropython fw esptool.py --chip esp32 --port COM4 --baud 460800 write_flash -z 0x1000 esp32-idf4-20210202-v1.14.bin
