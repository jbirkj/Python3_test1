
import time, datetime, sys
import gspread

sys.path.append("/home/pi/quick2wire-python-api")

from subprocess import Popen, PIPE
from quick2wire.i2c import I2CMaster, writing_bytes, reading

address = 0x28
Tekst = [ 0x54, 0x65, 0x6d, 0x70, 0x3d ]
ROMCODE1 = [ 0x28, 0xc3, 0xc2, 0x9d, 0x04, 0x00, 0x00, 0x9b]
sRomCode1 = "40 195 194 157 4 0 0 155"

def I2CWrite(value1) :
    I2CMaster().transaction( writing_bytes(address, value1) )
    time.sleep(0.1)
    return 1

def I2CRead():
    time.sleep(1)
    read = I2CMaster().transaction( reading(address, 1) )[0]
    time.sleep(1)
    return read

print('starting...')

try:
    gs = gspread.login('jesperbirkp@gmail.com', 'zxkfdgmtpslbqpzg')
except:
    print('fail')
    sys.exit

wks = gs.open("TempLog1").sheet1

path = "./DS18b20read.a " + sRomCode1
print(path)
 
#proc = Popen(["sudo ./DS18b20read.a "],
proc = Popen(path ,
             shell=True, 
            stdout=PIPE)
stdoutvalue = proc.communicate()
TempRead1 = eval(stdoutvalue[0])
print("current temperature is ", TempRead1, "degree Celsius" )

#writing to Google sheet
values = [ datetime.datetime.now(), 'sensor1',
           eval(stdoutvalue[0]), eval(stdoutvalue[0])+5 ]
wks.append_row(values)


# clear displa and move cursor to start
I2CWrite(0xFE)
I2CWrite(0x58)

for a in range(len(Tekst)):
    I2CWrite(Tekst[a])

I2CWrite(0x30+(int(eval(stdoutvalue[0])/10)))
I2CWrite(0x30+eval(stdoutvalue[0])%10)
         

print( "Goodbye")
var=0
