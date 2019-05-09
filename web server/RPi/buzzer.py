from gpiozero import Buzzer
from time import sleep

buz = Buzzer(2)
while 1:
	buz.beep(0.5,0.5,1)
