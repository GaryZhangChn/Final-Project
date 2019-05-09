;
; Filename: lcdio_project.inc
;
;this library is used for LCD display
;
;Author: Yihuan Zhang
;
;Changelog:
;1v0 01/28/19 - include lcd init, dis_char
;1v1 01/31/10 - banner msg 
;
;
init_lcd:
	rcall DELAY40mS
	;Funtion set
	ldi R17,$38
	sts ADR_LCD_CMD,R17	;sending data
	ldi R16,9	;37us=9loop * 4clk
	rcall DELAY4clk	
	;Function set
	ldi R17,$38
	sts ADR_LCD_CMD,R17	
	ldi R16,9	;37us
	rcall DELAY4clk
	;Display ON/OFF control
	ldi R17,$0C
	sts ADR_LCD_CMD,R17
	ldi R16,9	;37us
	rcall DELAY4clk
	;Display clear
	ldi R17,$01
	sts ADR_LCD_CMD,R17	
	rcall DELAY1520uS
	;Entry mode set
	ldi R17,$06
	sts ADR_LCD_CMD,R17
	ldi R16,9	;37us
	rcall DELAY4clk
	ret
clear_lcd:
	rcall DELAY1520uS
	ldi R17,$01	;P17 of datasheet
	sts ADR_LCD_CMD,R17
	rcall DELAY1520uS
	ret
clear_second_line:	;filling the line with blank
	rcall dis_second_line
	ldi R31,high(blank<<1)
	ldi R30,low(blank<<1)
	rcall dis_loop
	blank: .db "                ",NUL
	rcall dis_second_line
	ret

dis_second_line:
	ldi R17,$C0	;DDRAM
	ldi R16,9	;37us
	rcall DELAY4clk
	sts ADR_LCD_CMD,R17
	ldi R16,9	;37us
	rcall DELAY4clk
	ret


dis_loop:
		lpm R22, Z+
		cpi R22,NUL
		breq dis_loop_end
		rcall dis_char
		rjmp dis_loop
	dis_loop_end:
		ret
	
dis_char:	;load R22 into LCD to display
	push R17	;for time delay
	sts ADR_LCD_DATA,R22
	ldi R17, $FA	;1mS = 250loop * 4clk
	rcall DELAY4clk
	pop R17
	ret


banner_lcd:
;to init
	ldi R16,9	;37us
	rcall DELAY4clk
	rcall dis_lcd_banr	;first line
	ldi R17,$C0	;DDRAM
	ldi R16,9	;37us
	rcall DELAY4clk
	sts ADR_LCD_CMD,R17
	ldi R16,9	;37us
	rcall DELAY4clk
	ret
dis_lcd_banr:	;display the message in the first line
	ldi R31, high(lcd_banr<<1)
	ldi R30,low(lcd_banr<<1)
	rcall dis_loop
	lcd_banr: .db "N.A.M.System",NUL
	ret
dis_checksum:
	ldi R16,9
	rcall DELAY4clk
	ldi R31,high(dis_cks<<1)
	ldi R30,low(dis_cks<<1)
	dis_cks: .db "Checksum=$",NUL
	ret
dis_svsr:	;display the message in the second line
	ldi R16,9	;37us
	rcall DELAY4clk
	ldi R31, high(lcd_svsr<<1)
	ldi R30,low(lcd_svsr<<1)
	rcall dis_loop
lcd_svsr: .db "Monit. Mode",NUL
	ret
dis_node:	;display the message in the second line
	ldi R16,9	;37us
	rcall DELAY4clk
	ldi R31, high(lcd_node<<1)
	ldi R30,low(lcd_node<<1)
	rcall dis_loop
lcd_node: .db "Config. Mode",NUL
	ret
dis_alert1:
	ldi R16,9
	rcall DELAY4clk
	ldi R31,high(alert1<<1)
	ldi R30,low(alert1<<1)
	alert1: .db " !Unauthorized",NUL
	ret
dis_alert2:
	ldi R16,9
	rcall DELAY4clk
	ldi R31,high(alert2<<1)
	ldi R30,low(alert2<<1)
	alert2: .db "Enter:",NUL
	ret
dis_alert3:
	ldi R16,9
	rcall DELAY4clk
	ldi R31,high(alert3<<1)
	ldi R30,low(alert3<<1)
	alert3: .db " times",NUL
	ret

dis_data1:
	ldi R16,9
	rcall DELAY4clk
	ldi R31,high(data1<<1)
	ldi R30,low(data1<<1)
	data1: .db "Humidity ",NUL
	ret
dis_data2:
	ldi R16,9
	rcall DELAY4clk
	ldi R31,high(data2<<1)
	ldi R30,low(data2<<1)
	data2: .db "Temperature ",NUL
	ret
dis_HL:	;take R1 R3 and display in H:L
	ldi R31,high(highval<<1)
	ldi R30,low(highval<<1)
	rcall dis_loop
	highval: .db " H:",NUL
	;load H data
	mov R16,R1
	rcall dis_value		
	ldi R22,$20
	rcall dis_char
	ldi R31,high(lowval<<1)
	ldi R30,low(lowval<<1)
	rcall dis_loop
	lowval: .db " L:",NUL
	;load L data
	mov R16,R3
	rcall dis_value
	ret

	