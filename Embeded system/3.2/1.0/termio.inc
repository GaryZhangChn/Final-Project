;
; Filename: termio.inc
; 
; The following code supplied to the Fall 2018 243-513-DW students for educational/study
; purposes. The use of these routines, in whole or in part, without proper reference to 
; origin is a violation of ISEP regarding Academic Integrity and Plagerism.
;
;Description: Subroutine framework for use in the project (do not modify) 
;
;original Author: Mr. Markou
;
;Changelog:
;0v9 11/04/17 - beta version of serialio.asm
;1v0 11/10/17 - finalized and commented version 
;2v0 11/11/18 - forked into this version with extra routines
;2v1 11/12/18 - restructured for multi device usability e.g.UART/LCD
;2v2 11/20/18 - added some bells and whistles, and cookie (find it).
;3v0 01/28/19 - modified by Gary 1630783 for project purpose
;3v1 01/31/19 - changed the banner msg
.equ  UBRR  = (FCPU_L /(16 * BAUD)) -1   ;see p.138 (important)	
.equ  FRAME = $86      ;8N1 standard frame
.equ  TXE = $18        ;Transmit & receive enable     
.equ  LF = $0A		   ;ASCII line feed
.equ  CR = $0D		   ;ASCII carriage return
.equ  NUL = 0	       ;string terminating value
.equ  EOL = 0	       ;string terminating value
.equ  EOT = $04	       ;string terminating value

;
; Standard USART init which logically belongs here
; 
init_uart:                 
	ldi R16, 0	       ;always zero (mostly)
	out UBRRH, R16    
	ldi R16, UBRR	 
	out UBRRL, R16     ;config. the rate of data tx 
	ldi R16, TXE      
	out UCSRB, R16     ;enable port tx (see p.158)
	ldi R16, FRAME     ;defined in calling     
	out UCSRC, R16     ;config. frame elements 
	ret
;
;*gets - asm workalike C routine which inputs from UART RxD
; Entry: Z reg must point to buffer  
;        R17 must contain the size of the buffer.
gets:
	dec   R17	       ;leave room for NUL w/o exceeding buffer limit
gtx: 
    rcall getche
	st	  Z+, R16
	cpi	  R16, CR      ;check for returm
	breq  gty
	dec	  R17
	brne  gtx
gty:
	ldi   R16, NUL     ;place end of string
	st    Z, R16	
	ret
;
;*getch - asm workalike C routine to receive char from UART
; Exit:  R16 contain rx char
getch:
	in    R16,UCSRA
	andi  R16, $80
	breq  getch		   ;poll status for key press
	in	  R16,UDR      ;get char from UART 
	ret
;
;*getche - like above but w/echo
; Exit:  R16 contain rx char
getche:
	rcall getch
	push  R16          ;save R16 on stack
	rcall putchar
	pop  R16           ;and retrieve it back
	ret
;
;*putchar - char tx UART routine 
; Entry: R16  char. to send
putchar:	
    out  UDR,R16	   ;txmt char. out the TxD 
putc1:		
    in   R16, UCSRA    ;poll status reg
	andi R16, $20      ;check for tx complete
	breq putc1
	ret	
;
;*puts - asm workalike routine to puts() in C
; Entry: Z index points to an NUL terminated string in cseg or dseg
puts:
	lpm   R16, Z+      ;get char from memory
	cpi   R16, NUL     
	breq  px		   ;this is the end, my only friend, the end
	rcall putchar      ;send it to uart
	rjmp  puts
px: ret
;
;*newl - issues a new line (CR&LF) which comes in handy 
; Entry: R16
newl:
	ldi   R16, LF	    ;nothing new here
    rcall putchar
	ldi   R16, CR	 
    rcall putchar
	ret	

;
;//insert following two directives to main program a line before .exit
;.nolist
;.include "termio.inc"   ;append library subroutines from same folder


;===========terminal subroutines===========
banner_terminal:
	rcall newl			  
	ldi   R30, low(banr<<1)
	ldi   R31, high(banr<<1)
    rcall puts
	ret
banr: .db "E.R.Monitoring System V2.0",LF,CR,"By Gary",LF,CR,"1630783",LF,CR,NUL


terminal_menu_conf:
	rcall newl		   
	ldi   R30, low(node_menu<<1)
	ldi   R31, high(node_menu<<1)
    rcall puts
	ret
node_menu: .db LF,CR,"Config. Mode:",LF,CR,"T - Temerature",LF,CR,"H - Humidity",LF,CR,"S XX - Sets a new trim level",NUL

;terminal_menu_monit:
;	rcall newl		   
;	ldi   R30, low(svsr_menu<<1)
;	ldi   R31, high(svsr_menu<<1)
;    rcall puts
;	ret
;svsr_menu: .db LF,CR,"Monit. Mode:",LF,CR,"T - Temerature",LF,CR,"H - Humidity",LF,CR,"A - Allert mode",LF,CR,"C - Counter",NUL

unknown:
	rcall newl			  
	ldi   R30, low(idk<<1)
	ldi   R31, high(idk<<1)
    rcall puts
	ret
idk: .db "?",LF,CR,NUL

terminal_get_monit:
	rcall newl
	ldi   R30, low(term_svsr<<1)   
	ldi   R31, high(term_svsr<<1)
    rcall puts

	ldi	  R17, MAX        ;5 char max include CR
	ldi   R30, low(buff)  ;point Z-reg and buffer
	ldi   R31, high(buff)
	rcall gets
	ret
term_svsr: .db "Monitor>",NUL
	
terminal_get_conf:
	rcall newl
	ldi   R30, low(term_node<<1)   
	ldi   R31, high(term_node<<1)
    rcall puts

	ldi	  R17, MAX        ;5 char max include CR
	ldi   R30, low(buff)  ;point Z-reg and buffer
	ldi   R31, high(buff)
	rcall gets
	ldi R30,low(buff)
	ldi R31,high(buff)
	ld	R16,Z	;input saved in R16
	ret
term_node: .db "Config#",NUL


;===========terminal results===========
terminal_data:	;data1 saved in R2, data2 saved in R4
;Humidity = XX
;temperature = XX

	rcall newl	
	rcall adc_get_data1			
   	ldi   R30, low(data1<<1)
	ldi   R31, high(data1<<1)
    rcall puts 

	ldi R16,$3D	; space
	rcall putchar
	ldi R16,$20	; "="
	rcall putchar

	mov R16,R18
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar

	rcall newl

	rcall adc_get_data2	
   	ldi   R30, low(data2<<1)
	ldi   R31, high(data2<<1)
    rcall puts 

	ldi R16,$3D	; space
	rcall putchar
	ldi R16,$20	; "="
	rcall putchar

	mov R16,R18
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar
	ret


;===========terminal monit data print===========
print_part_1:
	ldi R31,high(data_print1<<1)
	ldi R30,low(data_print1<<1)
	rcall puts 
	ret
data_print1: .db $5B,$28,"Humidity",$3A,NUL;[(data1:

print_part_2:
	ldi R31,high(data_print2<<1)
	ldi R30,low(data_print2<<1)
	rcall puts 
	ret
data_print2: .db $29,$2C,$28,"Temperature",$3A,NUL	;),(data2:

print_part_3:
	ldi R31,high(data_print3<<1)
	ldi R30,low(data_print3<<1)
	rcall puts 
	ret
data_print3: .db $29,$5D,LF,CR,NUL	;)] /n
;===========terminal config input check===========
check_input_conf:
	cpi R16,$54	;T
	breq acquire_temperature
	cpi R16,$48	;H
	breq acquire_humidity
	cpi R16,$53
	breq set_trim
	rcall unknown
	ret

acquire_temperature:
;------display the data in hex------
	rcall newl
	ldi R31,high(data2<<1)
	ldi R30,low(data2<<1)
	rcall puts
	rcall adc_get_data2
	mov R16,R18
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar
	rcall terminal_HL
	ret
;------display the H and L val------	
terminal_HL:
	rcall newl
	ldi R31,high(highval<<1)
	ldi R30,low(highval<<1)
	rcall puts
	mov R16,R1
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar
	ldi R31,high(lowval<<1)
	ldi R30,low(lowval<<1)
	rcall puts
	mov R16,R3
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar
	ret
acquire_humidity:
	rcall newl
	ldi R31,high(data1<<1)
	ldi R30,low(data1<<1)
	rcall puts
	rcall adc_get_data1
	mov R16,R18
	rcall hex2asc
	rcall putchar
	mov R16,R17
	rcall putchar
	rcall terminal_HL
	ret

display_stats:
	rcall clear_lcd
	rcall DELAY1520uS
	rcall checksum
	rcall terminal_data
	rcall checkdata1
	rcall DELAY1S
	rcall checkdata2
	rcall DELAY1S
	ret
set_trim:
	ld R16,Z+
	ld R16,Z+	;shift to the second byte
	cpi R16,$20	;check if it's "space"
	breq trim_level
	;rcall unknown
	ret
trim_level:
	clr R17	;init for LSB
	ld R16,Z+	;MSB
	cpi R16,$3A
	brsh trim_level_end
	cpi R16,$30
	brlo trim_level_end
	rcall asc2hex
	lsl R16
	lsl R16
	lsl R16
	lsl R16
	
	ld R17,Z+	;LSB
	cpi R17,$3A
	brsh trim_level_end
	cpi R17,$30
	brlo trim_level_end
	rcall asc2hex

	and R16,R17
	mov R19,R16
	trim_level_end:
	ret

asc2hex:	;0~9, no mask needed
	subi R16,$30
	
	ret