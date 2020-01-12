import getopt
import sys
from datetime import datetime
from random import seed
from random import randint
# seed random number generator

import serial

import crc as crc8
import package as pkg


def main(argv):
    baudrate = 9600
    #serila_port = "/dev/ttyUSB0"
    serila_port = "/dev/cu.usbserial-00000000"
    try:
        opts, args = getopt.getopt(argv,"hp:b:",["port=","baudrate="])
    except getopt.GetoptError:
        print ('master.py -p: --port= <serial port> -b: --baudrate= <serial baudrate>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('master.py -p: --port= <serial port> -b: --baudrate= <serial baudrate>')
            sys.exit()
        elif opt in ("-p", "--port"):
            serila_port = arg
        elif opt in ("-b", "--baudrate"):
            baudrate = arg
    ser = serial.Serial()
    ser.baudrate = baudrate
    ser.port = serila_port
    try:
        ser.open()
    except:
        print("Failed to open port")
    if not ser.is_open:
        print("Serial port not available")
        sys.exit()
    ser.timeout = pkg.DATAREAD_TIMEOUT
    m_pkg = pkg.Frame()
    m_pkg.setSerPort(ser)

    log = open("./log.csv","w")
    seed(1)

    #PING    
    m_pkg.preBuild_Ping(pkg.SLAVE1_ADDRESS)
    m_pkg.send()
    result = ser.read(1)
    if len(result) == 0:
        print("Timeout. No response for PING frame")
        logger(log, "", "", "PING", "FAILED", "Timeout")
    else:
        result = bytearray(result)
        if result[0] != pkg.SLAVE1_ADDRESS:
            #Print the received and expected values
            print("Wrong data.\nReceived: {recv:#02x}".format(recv = result[0]))
            logger(log, "", "", "PING", "FAILED", "Wrong data")
        else:
            print("Ping Succesfull")
            logger(log, "", "", "PING", "SUCCESSFULL", "")
    #BAT_V
    m_pkg.preBuild_BatV(pkg.SLAVE1_ADDRESS)
    m_pkg.dump("BAT_V")
    m_pkg.send()
    result = ser.read(5)
    if len(result) != 5:
        print("Timeout. No response for BAT_V frame")
        logger(log, "", "", "BAT_V", "FAILED", "Timeout")
    else:
        result = bytearray(result)
        pkg_response = pkg.Frame()
        pkg_response.pkgBuilder(result[0], result[1], ((result[2] << 8) | (result[3] & 0xFF)))
        pkg_response.dump("BAT_V Response")
        if pkg_response.crc != result[4]:
            pkg_response.crcErr("BAT_V", result[4])
            logger(log, "", "", "BAT_V", "FAILED" "CRC error")
        else:
            print("\nBattery voltage is {batV} V".format(batV = pkg_response.data))
            logger(log, "", "", "BAT_V", "SUCCESSFULL", "")
    while True:
        # Bytes to send
        rand_len = randint(1, 10)
        data = bytearray(rand_len)
        for i in range(0, rand_len):
            data[i] = randint(0, 255)
        # Send junk data
        ser.write(data)
        str = "".join('{:#02X} '.format(i) for i in data)

        # Send valid data
        m_pkg.preBuild_BatV(pkg.SLAVE1_ADDRESS)
        m_pkg.send()
        result = ser.read(5)
        if len(result) != 5:
            print("Timeout. No response for BAT_V frame")
            logger(log, str, "", "BAT_V", "FAILED", "Timeout")
        else:
            result = bytearray(result)
            pkg_response = pkg.Frame()
            pkg_response.pkgBuilder(result[0], result[1], ((result[2] << 8) | (result[3] & 0xFF)))
            if pkg_response.crc != result[4]:
                pkg_response.crcErr("BAT_V", result[4])
                logger(log, str, "", "BAT_V", "FAILED" "CRC error")
            else:
                logger(log, str, "", "BAT_V", "SUCCESSFULL", "")
        

def logger(file, junk, baudrate, package, result, description):
    file.write( datetime.now().strftime("%m/%d/%Y, %H:%M:%S") + ";" + junk + ";" + baudrate + ";" + package + ";" + result + ";" + description + "\n" )

if __name__ == "__main__":
    main(sys.argv[1:])
