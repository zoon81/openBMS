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
    m_pkg = pkg.Package()
    m_pkg.pkgBuilder(pkg.SLAVE1_ADDRESS, pkg.PACKET_CMD_PING,0)
    m_pkg.genCRC8()
    m_pkg.setSerPort(ser)
    m_pkg.send()

if __name__ == "__main__":
    main(sys.argv[1:])
