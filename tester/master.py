import getopt
import sys

import serial

import crc as crc8
import package as pkg


def main(argv):
    baudrate = 9600
    serila_port = "/dev/ttyUSB0"
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
    #PING    
    m_pkg.preBuild_Ping(pkg.SLAVE1_ADDRESS)
    m_pkg.send()
    result = ser.read(1)
    if len(result) == 0:
        print("Timeout. No response for PING frame")
    else:
        result = bytearray(result)
        if result[0] != pkg.SLAVE1_ADDRESS:
            #Print the received and expected values
            print("Wrong data.\nReceived: {recv:#02x}".format(recv = result[0]))
        else:
            print("Ping Succesfull")
    #BAT_V
    m_pkg.preBuild_BatV(pkg.SLAVE1_ADDRESS)
    m_pkg.dump("BAT_V")
    m_pkg.send()
    result = ser.read(5)
    if len(result) != 5:
        print("Timeout. No response for BAT_V frame")
    else:
        result = bytearray(result)
        pkg_response = pkg.Frame()
        pkg_response.pkgBuilder(result[0], result[1], ((result[2] << 8) | (result[3] & 0xFF)))
        pkg_response.dump("BAT_V Response")
        if pkg_response.crc != result[4]:
            pkg_response.crcErr("BAT_V", result[4])
        else:
            print("\nBattery voltage is {batV} V".format(batV = pkg_response.data))

if __name__ == "__main__":
    main(sys.argv[1:])
