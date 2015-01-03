
import time, datetime, sys
import gspread
from tkinter import *

sys.path.append("/home/pi/quick2wire-python-api")

from subprocess import Popen, PIPE
from quick2wire.i2c import I2CMaster, writing_bytes, reading

address = 0x28
Tekst = [ 0x54, 0x65, 0x6d, 0x70, 0x31, 0x3d ]
Tekst2 = [ 0xfe, 0x47, 0x01, 0x02, 0x54, 0x65, 0x6d, 0x70, 0x32, 0x3d ]
ROMCODE1 = [ 0x28, 0xc3, 0xc2, 0x9d, 0x04, 0x00, 0x00, 0x9b]
sRomCode1 = "40 195 194 157 4 0 0 155"
ROMCODE5 = [ 0x28, 0xe8, 0xd4, 0x45, 0x05, 0x00, 0x00, 0x83]
sRomCode5 = "40 232 212 69 5 0 0 131"

def I2CWrite(value1) :
    I2CMaster().transaction( writing_bytes(address, value1) )
    time.sleep(0.1)
    return 1

def I2CRead():
    time.sleep(1)
    read = I2CMaster().transaction( reading(address, 1) )[0]
    time.sleep(1)
    return read

def GoogleSubmit(value1, value2):
    try:
        gs = gspread.login('jesperbirkp@gmail.com', 'zxkfdgmtpslbqpzg')
    except:
        print('fail')
        sys.exit
    wks = gs.open("TempLog1").sheet1
    
    #writing to Google sheet
    values = [ datetime.datetime.now(), 'sensor', value1, value2]
    wks.append_row(values)

print('starting...')    #printing in prompt
root = Tk()

path = "./DS18b20read.a "
path1 = path + sRomCode1
path2 = path + sRomCode5
print(path1)
print(path2)
 
#proc = Popen(["sudo ./DS18b20read.a "],
proc = Popen(path1 ,
             shell=True, 
            stdout=PIPE)
stdoutvalue = proc.communicate()
TempRead1 = eval(stdoutvalue[0])
print("current temperature 1 is ", TempRead1, "degree Celsius" )

proc = Popen(path2 ,
             shell=True, 
            stdout=PIPE)
stdoutvalue = proc.communicate()
TempRead2 = eval(stdoutvalue[0])
print("current temperature 2 is ", TempRead2, "degree Celsius" )

#GoogleSubmit(TempRead1, TempRead2)

#write to GUI
label_1 = Label(root, text="Temperatur 1")
label_2 = Label(root, text="Temperatur 2")
label_3 = Label(root, text=TempRead1)
label_4 = Label(root, text=TempRead2)
entry_1 = Entry(root)
entry_2 = Entry(root)
label_5 = Label(root, text="Logged at :")
label_6 = Label(root, text=datetime.datetime.now() )
label_1.grid(row=0, sticky=E)
label_2.grid(row=1, sticky=E)
label_3.grid(row=0, column=1)
label_4.grid(row=1, column=1)
entry_1.grid(row=0, column=2)
entry_2.grid(row=1, column=2)
label_5.grid(row=2, column=0)
label_6.grid(row=2, column=1)

c = Checkbutton(root, text="checkbox ")
c.grid(row=3, columnspan=2)
b = Button(root, text="Submit to Google Sheet", command=GoogleSubmit(TempRead1,TempRead2))
b.grid.(row=4)

# Write to LCD - clear displa and move cursor to start
I2CWrite(0xFE)
I2CWrite(0x58)

# writing LCD line1 with temp1
for a in range(len(Tekst)):
    I2CWrite(Tekst[a])
I2CWrite(0x30+(int(TempRead1/10)))      #I2CWrite(0x30+(int(eval(stdoutvalue[0])/10)))
I2CWrite(0x30+TempRead1%10)             #I2CWrite(0x30+eval(stdoutvalue[0])%10)

# writing LCD line 2 with Temp2
for a in range(len(Tekst2)):
    I2CWrite(Tekst2[a])
I2CWrite(0x30+(int(TempRead2/10)))
I2CWrite(0x30+TempRead2%10)

var=0
root.mainloop()
print( "Goodbye")
