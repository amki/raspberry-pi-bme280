try:
    import usocket as _socket
except:
    import _socket
try:
    import ussl as ssl
except:
    import ssl
import ubinascii
import machine
from time import sleep
import BME280

# ESP32 - Pin assignment


def main(use_stream=True):
    mac = ubinascii.hexlify(machine.unique_id()).decode("utf-8")
    chipId = mac[4]+mac[5]+mac[2]+mac[3]+mac[0]+mac[1]
    print("chipId is ", chipId)
    i2c = machine.I2C(scl=machine.Pin(22), sda=machine.Pin(21), freq=10000)
    bme = BME280.BME280(i2c=i2c)
    print("ARE WE CONNECTED MON? ", sta_if.isconnected())
    
    ai = _socket.getaddrinfo("env.epow0.org", 443)
    print("Address infos:", ai)
    addr = ai[0][-1]

    print("Connect address:", addr)

    while True:
        data = "{\"i\":\""+chipId+"\",\"t\":"+str(bme.temperature)+",\"p\":"+str(bme.pressure)+",\"h\":"+str(bme.humidity)+"}"
        s = _socket.socket()
        s.connect(addr)
        s = ssl.wrap_socket(s)
        s.write(b"POST /api/bme280 HTTP/1.1\r\nHost: env.epow0.org\r\nConnection: close\r\nContent-Length: "+str(len(data))+"\r\nContent-Type: application/json\r\n\r\n"+data+"\r\n\r\n")
        print(s.read(4096))
        s.close()
        #temp = bme.temperature
        #hum = bme.humidity
        #pres = bme.pressure
        # uncomment for temperature in Fahrenheit
        #temp = (bme.read_temperature()/100) * (9/5) + 32
        #temp = str(round(temp, 2)) + 'F'
        #print('Temperature: ', temp)
        #print('Humidity: ', hum)
        #print('Pressure: ', pres)
        print("Socket data ",data)

        sleep(5)

main()
