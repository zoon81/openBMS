import serial
import sys
import crc as crc8

MASTER_ADDRESS = 0x11
SLAVE1_ADDRESS = 0x035

<<<<<<< HEAD
DATAREAD_TIMEOUT = 5

=======
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
PACKET_CMD_BAT_V      = 0x33
PACKET_CMD_BAT_V_FAST = 0x35
PACKET_CMD_TEMP       = 0xC6
PACKET_CMD_BALLANCE   = 0x55
PACKET_CMD_PING       = 0xE9

<<<<<<< HEAD
class Frame:
=======
class Package:
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48

    def pkgBuilder(self, address, command, data):
        self.address = address
        self.command = command
        self.data = data
<<<<<<< HEAD
        self.crc = self.genCRC8()
=======
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48

    def genCRC8(self):
        crc = 0
        crc = crc8.UpdateCrc(crc, self.address)
        crc = crc8.UpdateCrc(crc, self.command)
<<<<<<< HEAD
        crc = crc8.UpdateCrc(crc, self.data >> 8)
        crc = crc8.UpdateCrc(crc, self.data & 0xFF)
        return crc
=======
        crc = crc8.UpdateCrc(crc, self.data)
        self.crc = crc
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48

    def setSerPort(self, ser_port):
        self.serport = ser_port
    
    def send(self):
<<<<<<< HEAD
        data = bytearray([self.address, self.command, self.data >> 8, self.data & 0xFF, self.crc])
        self.serport.write(data)
    
    def dump(self, framename = ""):
        print("\nFRAME DUMP: {frame_name}".format(frame_name = framename ))
        print("ADDRESS: {addr:#02x}\nCOMMAND: {comm:#02x}\nDATA: {data:#04x}\nCRC: {crc:#02x}".format(addr = self.address, comm = self.command, data = self.data, crc = self.crc))
    
    def crcErr(self, framename, get_crc):
        print("CRC erron in {frame_name} frame! \nExpected:\t{exp_crc:#02x}\nGet:\t{get_crc:#02}".format(frame_name = framename, exp_crc = self.crc, get_crc = get_crc))

    def preBuild_Ping(self, address):
        self.pkgBuilder(address, PACKET_CMD_PING, 0)

    def preBuild_BatV(self, address):
        self.pkgBuilder(address, PACKET_CMD_BAT_V, 0)

    def preBuild_BatV_fast(self, address):
       self.pkgBuilder(address, PACKET_CMD_BAT_V_FAST, 0)

    def preBuild_Ballance(self, address, ballance):
        self.pkgBuilder(address, PACKET_CMD_BALLANCE, ballance)

    def preBuild_Temp(self, address):
        self.pkgBuilder(address, PACKET_CMD_TEMP, 0)


=======
        data = bytearray([self.address, self.command, self.data, self.crc])
        self.serport.write(data)


    
>>>>>>> 0af11edb2c3301e8d9ec7259e5c7c1e7211edd48
