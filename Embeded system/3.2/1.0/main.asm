
;
; 1.0.asm
;
; Created: 1/25/2019 12:59:17 PM
; Author : 1630783
;

.equ FCPU_L  = 1000000 ;used by termio rtn 
.equ BAUD  = 2400    ;desired baud rate
.EQU BUSON=$82	;BUS and INT
.EQU VAL2=$00
.EQU ADR_ADC1=$7000	;enable Y7' for CH0
.EQU ADR_ADC2=$7200	;enable Y7' for CH1
.EQU ADR_LCD_CMD=$2000	;enable Y2' and R/S for instruction
.EQU ADR_LCD_DATA=$2100	;enable Y2' and R/S for data
;.EQU ADR_BUZ=$4000
.EQU MAX = 5
.dseg
buff:  .byte MAX

.cseg
	reset:	
	rjmp init	
	rjmp isr0	;int
	.org $1E	;reserved for int
	clr R28	;clear Y reg
	clr R29
;=========stack pointer=========
init:
	ldi R16,low(ramend)	;stack pointer
	out spl,R16
	ldi R16,high(ramend)
	out sph,R16		
	rcall init_uart	;sbr in "termio.inc"
		
;=========init INT=========		
	init_0:
		ldi R16,$40	;enable int0
		out GICR,R16
;===========init bus,ddr,lcd===========
		ldi R17,BUSON
		out MCUCR,R17	;falling edge, on Datasheet P77
		sts ADR_ADC1,R17	;trigger ADC
		sts ADR_ADC2,R17	;trigger ADC
		ldi R17,2	;mask to enable first pin only
		out DDRB,R17	;initialize DDR
		rcall init_lcd
;===========below only executes once===========
		in R17,PINB	;selector
		andi R17,1	;mask
		cpi R17,NUL
		brne monit_mode
		rjmp config_mode

;===========below executes periodly===========	
monit_mode:

	rcall clear_lcd
	rcall DELAY1520uS
	rcall dis_svsr	;first line display Supervisor mode
	clr R25	;initialize counters
	clr R27
;	rcall terminal_menu_monit	;to be removed

monit_loop:
	rcall dis_second_line
	sei	;enable motion sensor
	cpi R25,2	;entering times limit
	brsh alt
;	in    R16,UCSRA	;check the flag of RX
;	andi  R16, $80
;	breq  monit_menu_end	;skip the terminal menu	
;	rcall terminal_get_monit	;*should put somewhere else
;	rcall checkdeta_monit
;--------print: [(data1:val),(data2:val)]
	rcall print_part_1

	rcall adc_get_data1	;data saved in R18
	mov R16,R18
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar

	rcall print_part_2

	rcall adc_get_data2
	mov R16,R18
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar

	rcall print_part_3
	rcall DELAY1S
	rcall DELAY1S
	;rcall DELAY1S
	rjmp monit_loop

alt:	;alert procedure
	ldi R17, 2
	out PORTB,R17	;turn on the buzzer signal
	rcall clear_lcd
	rcall DELAY1520uS
	rcall dis_alert1
	rcall dis_loop	
refresh:	;counting enter times until reset
	rcall lcd_alt
	rjmp refresh

config_mode:
	rcall banner_terminal
	rcall banner_lcd
	rcall dis_second_line
	rcall checksum	;on both lcd and terminal
	rcall DELAY1S	;delay for banner
	rcall DELAY1S	; and motion detector
	rcall DELAY1S
	rcall DELAY1S
	rcall DELAY1S
	cli	;disable sensor
	rcall clear_lcd
	rcall DELAY1520uS
	rcall dis_node	;first line dispaly Node mode
default:
	ldi R19,128	;set default trim level
	rcall terminal_data	;general value of T and H
	rcall dis_second_line
	;rcall terminal_config
background:	
	in    R16,UCSRA	;check the flag of RX
	andi  R16, $80
	brne  terminal_config	;skip the terminal menu
 	rcall checkdata1	;also display on LCD, only in configuring mode
	rcall DELAY1S
	rcall checkdata2
	rcall DELAY1S
	rjmp background
fini:
	rjmp fini

terminal_config:
	rcall terminal_menu_conf
	rcall terminal_get_conf	;data saved in R16
	rcall check_input_conf
	rjmp background
;===========Interrupt subroutine===========
isr0:
	inc R25
	rcall calc_int_time
	reti

calc_int_time:
	cpi R25,$64	;100 times
	brsh one_hundred
	ret
one_hundred:
	clr R25
	cpi R27,$64
	brsh cnt_reset
	inc R27
cnt_reset:
	ret
ten_thousands:
	clr R25
	rjmp cnt_reset
;===========Checksum===========
checksum:
	clr R20
	clr R21
	clr R0
	ldi R30,0
	ldi R31,0
	ldi R28,high(esum<<1)	;store the last address in Y reg
	ldi R29,low(esum<<1)
checksum_calc:
	lpm R19,Z+	;to hold the value
	add R20,R19	;add the number on the lsb
	adc R21,R0	;hold or add the carry on the msb
	sbiw R28:R29,1
	brne checksum_calc
	rcall dis_checksum
	rcall dis_loop
	mov R16,R21
	rcall hex2asc	;R17,R16
	rcall ascii_on_lcd
	rcall ascii_on_terminal
	mov R16,R20
	rcall hex2asc
	rcall ascii_on_lcd
	rcall ascii_on_terminal
	ret

;===========LCD results===========
checkdata1:
	rcall DELAY1520uS
	rcall dis_second_line
	rcall dis_data1	;"data1 = "
	rcall dis_loop
	rcall adc_get_data1	;aquire data in R18, record H L in R1 R2
	rcall percentage_humidity	;convert hex of R18 into percentage
	mov R16,R18
	rcall hex2bcd8
	rcall dis_value
	ldi R22,$25	;'%'
	rcall dis_char
	rcall DELAY1S
	rcall DELAY1S
	rcall DELAY1S
	rcall clear_second_line
	rcall dis_HL	;display H:L
	ret

checkdata2:
	rcall DELAY1520uS
	rcall dis_second_line
	rcall dis_data2
	rcall dis_loop
	rcall adc_get_data2
	;rcall percentage_trim
	mov R16,R18
	rcall hex2bcd8
	rcall dis_value
	ldi R22,$DF	;check P14 of "ST7066U"
	rcall dis_char
	ldi R22,$43	;'%'
	rcall dis_char
	rcall DELAY1S
	rcall DELAY1S
	rcall DELAY1S
	rcall clear_second_line
	rcall dis_HL	;display H:L
	ret

dis_value:	;put the displaying value in R16 in hex
		rcall hex2asc
		mov R22,R16
		rcall dis_char	;display MSB
		mov R22,R17
		rcall dis_char	;display LSB
		ret

lcd_alt:	;a counter 
	rcall dis_second_line
	rcall dis_alert2
	rcall dis_loop
;	mov R16,R27	;if more than a hundred
;	rcall hex2bcd8
;	rcall hex2asc
;	rcall ascii_on_lcd
	mov R16,R25
	rcall hex2bcd8
	rcall hex2asc
	rcall ascii_on_lcd
	rcall dis_alert3
	rcall dis_loop
	ret

;===========ADC subroutine===========
;R19 is the trim level;
;R2 stores the values exceeded
;R1 stores the values below

adc_get_data1:
	clr R3	;L
	clr R1	;H
	clr R18
	ldi R16,$64;100 samples
	data_loop1:
		lds R18,ADR_ADC1 ;read the data from ADC
		cp R18,R19
		brsh cnt_high1	;R1++
		inc R3	;R3++
	data_loop1_end:
		dec R16
		brne data_loop1	
	ret
cnt_high1:
	inc R1
	rjmp data_loop1_end

adc_get_data2:
	clr R3
	clr R1
	clr R18
	ldi R16,$64;100 samples
	data_loop2:
		lds R18,ADR_ADC2 ;read the data from ADC
		cp R18,R19
		brsh cnt_high2	;R1++
		inc R3	;R3++
		data_loop2_end:
		dec R16
		brne data_loop2
	ret
cnt_high2:
	inc R1
	rjmp data_loop2_end



;================

ascii_on_lcd:	;display two vals (R16,R17) on LCD (R22)
	push R16
	push R17
	mov R22,R16
	rcall dis_char	;display MSB
	mov R22,R17
	rcall dis_char	;display LSB
	pop R16
	pop R17
	ret
ascii_on_terminal:	;send R16,R17 to UDR (R16)
	push R16
	push R17
	mov R15,R16
	mov R16,R17
	rcall putchar
	mov R16,R15
	rcall putchar
	pop R16
	pop R17
	ret
;include files must be in the same folder
.include "termio.inc"   ;routines to do terminal io using AVR's USART
.include "numio.inc"    ;routines from same folder
.include "lcdio_project.inc" 
.include "delay_project.inc" 
esum: .db 0

.exit