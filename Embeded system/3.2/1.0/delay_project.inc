;
; Filename: delay_project.inc
;
;this library is used for LCD ddelay and others
;
;Author: Yihuan Zhang
;
;Changelog:
;1v0 01/28/19 - include time_delay
; 
;
;


DELAY4clk:
	nop	;1 clk
	dec R16	;1 clk
	brne DELAY4clk	;2 clk
	ret	;1clk

DELAY1520uS:	;380loop * 4clk
	ldi R27,$1
	ldi R26,$7c
	delay_loop1:
		sbiw R27:R26,1	;2 clk
		brne delay_loop1	;2 clk
	ret	;1clk

DELAY40mS:;380loop * 4clk
	ldi R27,$27
	ldi R26,$10
	delay_loop2:
		sbiw R27:R26,1	;2 clk
		brne delay_loop2	;2 clk
	ret	;1clk

DELAY5mS:
	ldi R27,$04
	ldi R26,$E2
	delay_loop3:
		sbiw R27:R26,1	;2 clk
		brne delay_loop3	;2 clk
	ret	;1clk

DELAY1S:	;actually 1.05s
	ldi R27,$FF
	ldi R26,$FF
	delay_loop4_1:	;655350 clk
		nop nop nop nop nop nop
		sbiw R27:R26,1	;2 clk
		brne delay_loop4_1	;2 clk
	delay_loop4_2:	;393210 clk
		nop	;1 clk
		adiw R27:R26,1	;2 clk
		cpi R27,$FF	;1 clk
		brne delay_loop4_2	;2clk
	ret	;1clk