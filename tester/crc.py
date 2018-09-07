def UpdateCrc(crc,data):
    crc ^= data
    for bitnumber in range(0,8):
        if crc & 0x01:
            crc ^= 0x91
        crc = ( crc >> 1 )
    # print hex(crc & 0xFF)
    return crc & 0xFF