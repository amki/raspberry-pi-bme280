try:
    import usocket as _socket
except:
    import _socket
try:
    import ussl as ssl
except:
    import ssl
import network
import ubinascii
import machine
from time import sleep
import BME280

# ESP32 - Pin assignment

def searchWifi(sta_if):
    print("Looking for Wifi")
    aps = sta_if.scan()
    picked = None
    for ap in aps:
        name = ap[0].decode("utf-8")
        if name.startswith("thanksfor"):
            if picked is None or picked[3] < ap[3]:
                picked = ap
    
    return picked

def reconnectWifi():
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)
    sta_if.disconnect()
    picked = None
    while picked == None:
        picked = searchWifi(sta_if)
        if picked == None:
            sleep(10)
    
    print("Connecting to ",picked)
    sta_if.connect(picked[0], "derkleinekrebskrebstnichtmehr", bssid=picked[1]) # Connect to an AP
    while not sta_if.isconnected():
        print("Wifi is in state: ",sta_if.status())
        sleep(5)


def main(use_stream=True):
    mac = ubinascii.hexlify(machine.unique_id()).decode("utf-8")
    chipId = mac[4]+mac[5]+mac[2]+mac[3]+mac[0]+mac[1]
    print("chipId is ", chipId)
    i2c = machine.I2C(scl=machine.Pin(22), sda=machine.Pin(21), freq=10000)
    bme = BME280.BME280(i2c=i2c)

    errors = 0
    
    ai = _socket.getaddrinfo("env.epow0.org", 443)
    print("Address infos:", ai)
    addr = ai[0][-1]

    print("Connect address:", addr)

    while True:
        data = "{\"i\":\""+chipId+"\",\"t\":"+str(bme.temperature)+",\"p\":"+str(bme.pressure)+",\"h\":"+str(bme.humidity)+"}"
        try:
            s = _socket.socket()
            s.connect(addr)
            s = ssl.wrap_socket(s)
            s.write(b"POST /api/bme280 HTTP/1.1\r\nHost: env.epow0.org\r\nConnection: close\r\nContent-Length: "+str(len(data))+"\r\nContent-Type: application/json\r\n\r\n"+data+"\r\n\r\n")
            #print(s.read(4096))
            s.close()
            print("Socket data ",data)
            errors = 0
            sleep(10)
        except:
            errors += 1
            print("Error ",errors)
            if errors > 9:
                reconnectWifi()
                errors = 0
            sleep(3)
        

reconnectWifi()
main()
