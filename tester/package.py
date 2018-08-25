import serial
import sys
import crc as crc8

MASTER_ADDRESS = 0x11
SLAVE1_ADDRESS = 0x035

PACKET_CMD_BAT_V      = 0x33
PACKET_CMD_BAT_V_FAST = 0x35
PACKET_CMD_TEMP       = 0xC6
PACKET_CMD_BALLANCE   = 0x55
PACKET_CMD_PING       = 0xE9

class Package:

    def pkgBuilder(self, address, command, data):
        self.address = address
        self.command = command
        self.data = data

    def genCRC8(self):
        crc = 0
        crc = crc8.UpdateCrc(crc, self.address)
        crc = crc8.UpdateCrc(crc, self.command)
        crc = crc8.UpdateCrc(crc, self.data)
        self.crc = crc

    def setSerPort(self, ser_port):
        self.serport = ser_port
    
    def send(self):
        data = bytearray([self.address, self.command, self.data, self.crc])
        self.serport.write(data)


    