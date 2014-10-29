
import time
from subprocess import Popen, PIPE
from quick2wire.i2c import I2CMaster, writing_bytes, reading

address = 0x28
Tekst = [ 0x54, 0x65, 0x6d, 0x70, 0x3d ]
ROMCODE1 = [ 0x28, 0xc3, 0xc2, 0x9d, 0x04, 0x00, 0x00, 0x9b]
iROMCODE1 = [ 40, 195, 194, 157, 4, 0, 0, 155]

def I2CWrite(value1) :
    I2CMaster().transaction( writing_bytes(address, value1) )
    time.sleep(0.1)
    return 1

def I2CRead():
    time.sleep(1)
    read = I2CMaster().transaction( reading(address, 1) )[0]
    time.sleep(1)
    return read

print('starting....')

path = "./DS18b20read.a "
parse = path + repr(ROMCODE1)

for a in range(0, 8 ):
    path += str(iROMCODE1[a])
    path += " "


print(path)

#proc = Popen(["sudo ./DS18b20read.a "],
proc = Popen(path ,
             shell=True, 
            stdout=PIPE)

stdoutvalue = proc.communicate()

print("current temperature is ", eval(stdoutvalue[0]), "degree Celsius" )

# clear displa and move cursor to start
I2CWrite(0xFE)
I2CWrite(0x58)

for a in range(len(Tekst)):
    I2CWrite(Tekst[a])

I2CWrite(0x30+(int(eval(stdoutvalue[0])/10)))
I2CWrite(0x30+eval(stdoutvalue[0])%10)
         

print( "Goodbye")
var=0
