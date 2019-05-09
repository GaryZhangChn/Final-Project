import serial
import re
ser = serial.Serial("/dev/ttyS0", baudrate = 2400, timeout = 100)

with ser:

    line = ser.readline()
    
    line = line.decode("utf-8")

    content = re.findall('\((.*?)\)', line)
    dic = {}
    for val in content:
        tup = val.split(":")
        dic[tup[0]] = tup[1]

    dic['Humidity'] = (int(dic['Humidity'],16)*0.0196-0.958)/0.0308

    print("Humidity:%2.1f%% " % float(dic["Humidity"])) #Humidity:<XX.X>%
    
    
    dic['Temperature'] = (int(dic['Temperature'],16))
    degree_sign = u'\N{DEGREE SIGN}'
    print('Temperature:%2d \u2103' % int(dic["Temperature"]))

