;OCM19264模块测试程序
D_I	EQU	P3.0				;Data or Instrument Select,H:写数据,L:写指令
R_W	EQU	P3.1				;Write or Read,H:read,L:write
E	EQU	P3.2				;读写使能
CS1	EQU	P3.3				;CS1=L,选择左边屏
RESET	EQU	P3.4				;Lcm reset,低有效
CS2	EQU	P3.5				;CS2=L,选择中间屏
CS3	EQU	P3.6				;CS2=L,选择右边屏
	
DISP_X	EQU	30H
DISP_Y	EQU	31H
DISP_Z	EQU	32H
LONG	EQU	33H
WIDE	EQU	34H
DATA1_WR	EQU	35H
DATA2_WR	EQU	36H
COUNTER1	EQU	37H
COUNTER2	EQU	38H
;**********************************************
		ORG	0000H
		AJMP	MAIN
		ORG	0100H
MAIN:		MOV	SP,#5FH
		CLR	RESET
		NOP
		NOP
		NOP
		SETB	RESET
		MOV	DISP_Z,#0C0H
;**********************************************
LGS1:		CLR	CS1
		SETB	CS2
		SETB	CS3
		MOV	DISP_X,#0B8H
		MOV	DISP_Y,#40H
		MOV	LONG,#40H
		MOV	WIDE,#02H
		MOV	DPTR,#HZ11
		LCALL	CHN_DISP
		MOV	DISP_X,#0BAH
		MOV	DPTR,#HZ21
		LCALL	CHN_DISP
		MOV	DISP_X,#0BCH
		MOV	DPTR,#HZ31
		LCALL	CHN_DISP
		MOV	DISP_X,#0BEH
		MOV	DPTR,#HZ41
		LCALL	CHN_DISP
		
		SETB	CS1
		CLR	CS2
		SETB	CS3
		MOV	DISP_X,#0B8H
		MOV	DISP_Y,#40H
		MOV	LONG,#40H
		MOV	WIDE,#02H
		MOV	DPTR,#HZ12
		LCALL	CHN_DISP
		MOV	DISP_X,#0BAH
		MOV	DPTR,#HZ22
		LCALL	CHN_DISP
		MOV	DISP_X,#0BCH
		MOV	DPTR,#HZ32
		LCALL	CHN_DISP
		MOV	DISP_X,#0BEH
		MOV	DPTR,#HZ42
		LCALL	CHN_DISP
		
		SETB	CS1
		SETB	CS2
		CLR	CS3
		MOV	DISP_X,#0B8H
		MOV	DISP_Y,#40H
		MOV	LONG,#40H
		MOV	WIDE,#02H
		MOV	DPTR,#HZ13
		LCALL	CHN_DISP
		MOV	DISP_X,#0BAH
		MOV	DPTR,#HZ23
		LCALL	CHN_DISP
		MOV	DISP_X,#0BCH
		MOV	DPTR,#HZ33
		LCALL	CHN_DISP
		MOV	DISP_X,#0BEH
		MOV	DPTR,#HZ43
		LCALL	CHN_DISP
		
		LCALL	DELAY1
;----------------------------------------------
LGS5:		MOV	DPTR,#HOCO
		LCALL	PHO_DISP
		LCALL	DELAY1
;----------------------------------------------
LGS4:		CLR	CS1
		CLR	CS2
		CLR	CS3
		MOV	DATA1_WR,#0FFH
		MOV	DATA2_WR,#00H
		LCALL	WRLATTICE
		LCALL	DELAY1
		
		MOV	DATA1_WR,#00H
		MOV	DATA2_WR,#0FFH
		LCALL	WRLATTICE
		LCALL	DELAY1
;----------------------------------------------
;LGS2:		CLR	CS1
;		CLR	CS2
;		CLR	CS3
;		MOV	DATA1_WR,#0FFH
;		MOV	DATA2_WR,#0FFH
;		LCALL	WRLATTICE
;		LCALL	DELAY1
;----------------------------------------------
;LGS3:		CLR	CS1
;		CLR	CS2
;		CLR	CS3
;		MOV	DISP_X,#0B8H
;		MOV	DISP_Y,#40H
;		MOV	LONG,#20H
;		MOV	WIDE,#04H
;		MOV	DPTR,#GPTB1
;		LCALL	CHN_DISP
;		MOV	DISP_X,#0B8H
;		MOV	DISP_Y,#60H
;		MOV	DPTR,#GPTB2
;		LCALL	CHN_DISP
		
;		MOV	DISP_X,#0BCH
;		MOV	DISP_Y,#40H
;		MOV	DPTR,#GPTB2
;		LCALL	CHN_DISP
;		MOV	DISP_X,#0BCH
;		MOV	DISP_Y,#60H
;		MOV	DPTR,#GPTB1
;		LCALL	CHN_DISP
		
;		LCALL	DELAY1
;----------------------------------------------
LGS6:		CLR	CS1
		CLR	CS2
		CLR	CS3
		MOV	DATA1_WR,#55H
		MOV	DATA2_WR,#55H
		LCALL	WRLATTICE
		LCALL	DELAY1
		
		MOV	DATA1_WR,#0AAH
		MOV	DATA2_WR,#0AAH
		LCALL	WRLATTICE
		LCALL	DELAY1
;----------------------------------------------
		LJMP	LGS1
;**********************************************
WRLATTICE:	MOV	COUNTER1,#08H		;写点阵
		MOV	DISP_X,#0B8H
		MOV	DISP_Z,#0C0H
		MOV	DISP_Y,#40H
		LCALL	DISP
LATTICE1:	LCALL	WR_ZB
		MOV	COUNTER2,#20H
LATTICE2:	MOV	A,DATA1_WR
		LCALL	WR_DATA
		MOV	A,DATA2_WR
		LCALL	WR_DATA
		DJNZ	COUNTER2,LATTICE2
		INC	DISP_X
		DJNZ	COUNTER1,LATTICE1
		RET
;----------------------------------------------
PHO_DISP:	MOV	COUNTER1,#08H		;全屏显示图形
		MOV	DISP_X,#0B8H
		MOV	DISP_Z,#0C0H
		MOV	DISP_Y,#40H
PHO_DISP1:	CLR	CS1
		SETB	CS2
		SETB	CS3
		LCALL	DISP
		LCALL	WR_ZB
		MOV	COUNTER2,#40H
PHO_DISP11:	CLR	A
		MOVC	A,@A+DPTR
		LCALL	WR_DATA
		INC	DPTR
		DJNZ	COUNTER2,PHO_DISP11
PHO_DISP2:	SETB	CS1
		CLR	CS2
		SETB	CS3
		LCALL	DISP
		LCALL	WR_ZB
		MOV	COUNTER2,#40H
PHO_DISP21:	CLR	A
		MOVC	A,@A+DPTR
		LCALL	WR_DATA
		INC	DPTR
		DJNZ	COUNTER2,PHO_DISP21
PHO_DISP3:	SETB	CS1
		SETB	CS2
		CLR	CS3
		LCALL	DISP
		LCALL	WR_ZB
		MOV	COUNTER2,#40H
PHO_DISP31:	CLR	A
		MOVC	A,@A+DPTR
		LCALL	WR_DATA
		INC	DPTR
		DJNZ	COUNTER2,PHO_DISP31
		INC	DISP_X
		DJNZ	COUNTER1,PHO_DISP1
		RET
;----------------------------------------------
CHN_DISP:	LCALL	DISP			;显示LONGxWIDE大小的图形
		MOV	COUNTER1,WIDE
CHN_DISP1:	LCALL	WR_ZB
		MOV	COUNTER2,LONG
CHN_DISP2:	CLR	A
		MOVC	A,@A+DPTR
		LCALL	WR_DATA
		INC	DPTR
		DJNZ	COUNTER2,CHN_DISP2
		INC	DISP_X
		DJNZ	COUNTER1,CHN_DISP1
		RET
;----------------------------------------------
WR_ZB:		MOV	A,DISP_X
		LCALL	WR_INT
		MOV	A,DISP_Z
		LCALL	WR_INT
		MOV	A,DISP_Y
		LCALL	WR_INT
		RET
;----------------------------------------------
DISP:		MOV	A,#3EH
		LCALL	WR_INT
		MOV	A,#3FH
		LCALL	WR_INT
		RET
;**********************************************
WR_INT:		LCALL	CHE_BUSY		;WRITE	INSTRUCTION
		CLR	D_I
		CLR	R_W
		MOV	P1,A
		NOP
		SETB	E
		NOP
		NOP
		CLR	E
		RET
;----------------------------------------------
WR_DATA:	LCALL	CHE_BUSY		;WRITE	DATA
		SETB	D_I
		CLR	R_W
		MOV	P1,A
		NOP
		SETB	E
		NOP
		NOP
		CLR	E
		RET
;----------------------------------------------
CHE_BUSY:	CLR	D_I			;CHECK	STATIU
		SETB	R_W
		ANL	P1,#0B0H
		ORL	P1,#80H
		NOP
		SETB	E
		JB	P1.7,$
		JB	P1.4,$
		CLR	E
		RET
;**********************************************
DELAY1:		MOV	R7,#10H
DEL11:		MOV	R6,#0FFH
DEL12:		MOV	R5,#0FFH
DEL13:		DJNZ	R5,DEL13
		DJNZ	R6,DEL12
		DJNZ	R7,DEL11
		RET
DELAY2:		MOV	R7,#04H
DEL21:		MOV	R6,#23H
DEL22:		DJNZ	R6,DEL22
		DJNZ	R7,DEL21
		RET
;**********************************************
HZ11:
;--  调入了一幅图像：E:\广.bmp  --
;--  宽度x高度=64x16  --
DB  000H,000H,0FCH,004H,004H,004H,004H,005H,006H,004H,004H,004H,004H,004H,004H,000H
DB  000H,004H,004H,0C4H,0B4H,08CH,087H,084H,0F4H,084H,084H,084H,084H,004H,000H,000H
DB  080H,060H,01CH,095H,096H,094H,09CH,0C8H,0A4H,0AFH,094H,094H,02CH,044H,044H,000H
DB  000H,000H,0FCH,044H,044H,044H,044H,0C5H,07EH,0C4H,044H,044H,044H,044H,044H,000H
DB  040H,030H,00FH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB  000H,000H,020H,018H,00EH,004H,020H,040H,0FFH,000H,002H,004H,018H,030H,000H,000H
DB  020H,022H,02AH,02AH,02AH,02AH,02AH,0FFH,02AH,02AH,02AH,02FH,022H,022H,020H,000H
DB  040H,030H,00FH,040H,020H,010H,00CH,003H,000H,001H,006H,018H,030H,060H,020H,000H
HZ12:
;--  调入了一幅图像：E:\金.bmp  --
;--  宽度x高度=64x16  --
DB  040H,040H,020H,020H,050H,048H,044H,0C3H,044H,048H,050H,050H,020H,060H,020H,000H
DB  000H,0FEH,012H,0FEH,000H,0FEH,012H,0FEH,000H,0FCH,00EH,035H,044H,07CH,000H,000H
DB  010H,012H,092H,072H,0FEH,051H,091H,000H,022H,0CCH,000H,000H,0FFH,000H,000H,000H
DB  008H,008H,088H,0FFH,048H,028H,000H,0C8H,048H,048H,07FH,048H,0C8H,048H,008H,000H
DB  040H,040H,042H,042H,04AH,072H,042H,07FH,042H,062H,05AH,042H,042H,040H,040H,000H
DB  020H,01FH,021H,07FH,020H,01FH,041H,07FH,008H,009H,009H,029H,049H,021H,01FH,000H
DB  004H,002H,001H,000H,0FFH,000H,004H,004H,004H,002H,002H,002H,0FFH,001H,001H,000H
DB  001H,041H,080H,07FH,000H,040H,040H,020H,013H,00CH,00CH,012H,021H,060H,020H,000H
HZ13:
;--  调入了一幅图像：E:\有.bmp  --
;--  宽度x高度=64x16  --
DB  000H,004H,084H,044H,0E4H,034H,02CH,027H,024H,024H,024H,0E4H,004H,004H,004H,000H
DB  0FEH,002H,032H,04EH,082H,000H,0FEH,04AH,0CAH,04AH,04AH,04AH,07EH,000H,000H,000H
DB  000H,000H,080H,040H,030H,00EH,084H,000H,000H,00EH,010H,060H,0C0H,080H,080H,000H
DB  000H,010H,092H,092H,092H,092H,092H,092H,092H,092H,012H,002H,002H,0FEH,000H,000H
DB  002H,001H,000H,000H,0FFH,009H,009H,009H,029H,049H,0C9H,07FH,000H,000H,000H,000H
DB  0FFH,000H,002H,004H,003H,000H,0FFH,040H,020H,003H,00CH,012H,021H,060H,020H,000H
DB  000H,001H,020H,070H,028H,024H,023H,031H,010H,010H,014H,078H,030H,001H,000H,000H
DB  000H,000H,01FH,004H,004H,004H,004H,004H,004H,00FH,000H,020H,040H,03FH,000H,000H
HZ21:
;--  调入了一幅图像：E:\奥.bmp  --
;--  宽度x高度=64x16  --
DB  0FFH,0FFH,001H,0DDH,0D5H,04DH,09CH,001H,09DH,04DH,055H,0DDH,001H,0FFH,0FFH,0FFH
DB  0FFH,0FDH,0FDH,00DH,0EDH,0EDH,0EDH,0EDH,00DH,0FDH,0FDH,0FDH,001H,0FDH,0FDH,0FFH
DB  0EFH,0EFH,0EFH,000H,06FH,0AFH,0FFH,0B7H,076H,0F5H,0F1H,0F7H,017H,0B7H,0F7H,0FFH
DB  0EFH,09EH,0F9H,01FH,0E7H,07BH,01BH,0E3H,07BH,09AH,041H,0DBH,05BH,09BH,0FBH,0FFH
DB  07FH,07BH,0BAH,0BBH,0DAH,0EBH,0F3H,0F8H,0F3H,0EBH,0DAH,0DBH,0BAH,03BH,0BBH,0FFH
DB  0FFH,0FFH,0FFH,0F8H,0FDH,0FDH,0FDH,0FDH,0F8H,0EFH,0DFH,0BFH,0C0H,0FFH,0FFH,0FFH
DB  0FDH,0BDH,07EH,080H,0FFH,0DFH,0DFH,0DFH,0DEH,0C1H,0DFH,0C3H,0DCH,0DFH,0DFH,0FFH
DB  0FBH,0FBH,000H,0FFH,0FEH,0FFH,000H,0BEH,0DEH,0EDH,0F3H,0E4H,09EH,03FH,0BFH,0FFH
HZ22:
;--  调入了一幅图像：E:\晶.bmp  --
;--  宽度x高度=64x16  --
DB  0FFH,0FFH,0FFH,0FFH,081H,0D5H,0D5H,0D5H,0D5H,0D5H,0D5H,081H,0FFH,0FFH,0FFH,0FFH
DB  0FFH,0FFH,0FFH,0C1H,0D5H,015H,0D5H,0D5H,0D5H,015H,0D5H,0C1H,0FFH,0FFH,0FFH,0FFH
DB  0FFH,0DFH,0DFH,0DDH,0DDH,0DDH,0DDH,01DH,0DDH,0DDH,0DDH,0DDH,0DDH,0DFH,0DFH,0FFH
DB  0BFH,0BFH,0B0H,0B6H,0B6H,036H,030H,08FH,03FH,030H,0B6H,0A6H,096H,0B0H,0FFH,0FFH
DB  0FFH,080H,0DAH,0DAH,0DAH,0DAH,080H,0FFH,0FFH,080H,0DAH,0DAH,0DAH,0DAH,080H,0FFH
DB  0DFH,0DEH,0DDH,0D3H,0DFH,0C0H,0DFH,0DFH,0DFH,0C0H,0D7H,0DBH,0DCH,0DFH,0DFH,0FFH
DB  0EFH,0F7H,0FBH,0FCH,0FFH,0BFH,07FH,080H,0FFH,0FFH,0FEH,0FDH,0F3H,0E7H,0FFH,0FFH
DB  0FDH,0FDH,081H,0BAH,0BAH,0BBH,083H,0FFH,083H,0BBH,0BAH,0BAH,081H,0F9H,0FDH,0FFH
HZ23:
;--  调入了一幅图像：E:\有.bmp  --
;--  宽度x高度=64x16  --
DB  0FFH,0FBH,07BH,0BBH,01BH,0CBH,0D3H,0D8H,0DBH,0DBH,0DBH,01BH,0FBH,0FBH,0FBH,0FFH
DB  001H,0FDH,0CDH,0B1H,07DH,0FFH,001H,0B5H,035H,0B5H,0B5H,0B5H,081H,0FFH,0FFH,0FFH
DB  0FFH,0FFH,07FH,0BFH,0CFH,0F1H,07BH,0FFH,0FFH,0F1H,0EFH,09FH,03FH,07FH,07FH,0FFH
DB  0FFH,0EFH,06DH,06DH,06DH,06DH,06DH,06DH,06DH,06DH,0EDH,0FDH,0FDH,001H,0FFH,0FFH
DB  0FDH,0FEH,0FFH,0FFH,000H,0F6H,0F6H,0F6H,0D6H,0B6H,036H,080H,0FFH,0FFH,0FFH,0FFH
DB  000H,0FFH,0FDH,0FBH,0FCH,0FFH,000H,0BFH,0DFH,0FCH,0F3H,0EDH,0DEH,09FH,0DFH,0FFH
DB  0FFH,0FEH,0DFH,08FH,0D7H,0DBH,0DCH,0CEH,0EFH,0EFH,0EBH,087H,0CFH,0FEH,0FFH,0FFH
DB  0FFH,0FFH,0E0H,0FBH,0FBH,0FBH,0FBH,0FBH,0FBH,0F0H,0FFH,0DFH,0BFH,0C0H,0FFH,0FFH
HZ31:
;--  调入了一幅图像：E:\h.bmp  --
;--  宽度x高度=64x16  --
DB  008H,0F8H,000H,080H,080H,080H,000H,000H,000H,080H,080H,0E0H,080H,080H,000H,000H
DB  000H,080H,080H,0E0H,080H,080H,000H,000H,080H,080H,000H,080H,080H,000H,000H,000H
DB  000H,000H,000H,0C0H,0C0H,000H,000H,000H,000H,000H,000H,000H,080H,060H,018H,004H
DB  000H,000H,000H,000H,080H,060H,018H,004H,080H,080H,000H,080H,000H,080H,080H,080H
DB  020H,03FH,021H,000H,000H,020H,03FH,020H,000H,000H,000H,01FH,020H,020H,000H,000H
DB  000H,000H,000H,01FH,020H,020H,000H,000H,080H,0FFH,0A1H,020H,020H,011H,00EH,000H
DB  000H,000H,000H,030H,030H,000H,000H,000H,000H,060H,018H,006H,001H,000H,000H,000H
DB  000H,060H,018H,006H,001H,000H,000H,000H,00FH,030H,00CH,003H,00CH,030H,00FH,000H
HZ32:
;--  调入了一幅图像：E:\w.bmp  --
;--  宽度x高度=64x16  --
DB  080H,080H,000H,080H,000H,080H,080H,080H,080H,080H,000H,080H,000H,080H,080H,080H
DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,080H,080H,080H,080H,080H,000H
DB  080H,080H,000H,080H,080H,000H,000H,000H,000H,080H,080H,0E0H,080H,080H,000H,000H
DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,080H,080H,080H,000H,000H
DB  00FH,030H,00CH,003H,00CH,030H,00FH,000H,00FH,030H,00CH,003H,00CH,030H,00FH,000H
DB  000H,030H,030H,000H,000H,000H,000H,000H,000H,06BH,094H,094H,094H,093H,060H,000H
DB  080H,0FFH,0A1H,020H,020H,011H,00EH,000H,000H,000H,000H,01FH,020H,020H,000H,000H
DB  000H,030H,030H,000H,000H,000H,000H,000H,000H,00EH,011H,020H,020H,020H,011H,000H
HZ33:
;--  调入了一幅图像：E:\o.bmp  --
;--  宽度x高度=64x16  --
DB  000H,000H,080H,080H,080H,080H,000H,000H,080H,080H,080H,080H,080H,080H,080H,000H
DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,080H,080H,080H,000H,000H
DB  080H,080H,000H,080H,080H,080H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB  000H,01FH,020H,020H,020H,020H,01FH,000H,020H,03FH,020H,000H,03FH,020H,000H,03FH
DB  000H,030H,030H,000H,000H,000H,000H,000H,000H,00EH,011H,020H,020H,020H,011H,000H
DB  020H,03FH,021H,000H,000H,020H,03FH,020H,000H,000H,000H,000H,000H,000H,000H,000H
DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
HZ41:
;--  调入了一幅图像：E:\j.bmp  --
;--  宽度x高度=64x16  --
DB  000H,000H,000H,080H,098H,098H,000H,000H,080H,080H,080H,080H,080H,080H,080H,000H
DB  000H,000H,080H,080H,080H,080H,080H,000H,080H,080H,000H,080H,080H,000H,000H,000H
DB  000H,000H,000H,080H,080H,080H,000H,000H,0C0H,030H,0C8H,028H,0E8H,010H,0E0H,000H
DB  080H,080H,000H,080H,080H,000H,000H,000H,080H,080H,000H,000H,000H,080H,080H,000H
DB  000H,0C0H,080H,080H,080H,07FH,000H,000H,020H,03FH,020H,000H,03FH,020H,000H,03FH
DB  000H,06BH,094H,094H,094H,093H,060H,000H,080H,0FFH,0A1H,020H,020H,011H,00EH,000H
DB  000H,00EH,011H,020H,020H,020H,011H,000H,007H,018H,027H,024H,023H,014H,00BH,000H
DB  080H,0FFH,0A1H,020H,020H,011H,00EH,000H,000H,01FH,020H,020H,020H,010H,03FH,020H
HZ42:
;--  调入了一幅图像：E:\b.bmp  --
;--  宽度x高度=64x16  --
DB  008H,0F8H,000H,080H,080H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB  000H,080H,080H,080H,080H,080H,080H,000H,008H,0F8H,000H,080H,080H,080H,000H,000H
DB  000H,000H,080H,080H,080H,080H,000H,000H,000H,000H,080H,080H,080H,080H,000H,000H
DB  000H,000H,000H,080H,080H,080H,080H,000H,000H,080H,098H,098H,000H,000H,000H,000H
DB  000H,03FH,011H,020H,020H,011H,00EH,000H,000H,030H,030H,000H,000H,000H,000H,000H
DB  000H,021H,030H,02CH,022H,021H,030H,000H,020H,03FH,021H,000H,000H,020H,03FH,020H
DB  000H,019H,024H,022H,022H,022H,03FH,020H,000H,01FH,020H,020H,020H,020H,01FH,000H
DB  000H,00EH,011H,020H,020H,0A0H,0FFH,080H,000H,020H,020H,03FH,020H,020H,000H,000H
HZ43:
;--  调入了一幅图像：E:\n.bmp  --
;--  宽度x高度=64x16  --
DB  080H,080H,000H,080H,080H,080H,000H,000H,000H,000H,080H,080H,080H,080H,080H,000H
DB  000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,080H,080H,080H,080H,080H,000H
DB  000H,000H,000H,080H,080H,088H,0F8H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB  000H,000H,000H,080H,080H,080H,000H,000H,080H,080H,000H,080H,080H,080H,000H,000H
DB  020H,03FH,021H,000H,000H,020H,03FH,020H,000H,06BH,094H,094H,094H,093H,060H,000H
DB  000H,030H,030H,000H,000H,000H,000H,000H,000H,06BH,094H,094H,094H,093H,060H,000H
DB  000H,00EH,011H,020H,020H,010H,03FH,020H,000H,030H,030H,000H,000H,000H,000H,000H
DB  000H,00EH,011H,020H,020H,020H,011H,000H,020H,03FH,021H,000H,000H,020H,03FH,020H
GPTB1:
;--  调入了一幅图像：F:\梁\画图\GPTB.bmp  --
;--  宽度x高度=32x32  --
DB  000H,000H,000H,000H,000H,080H,040H,020H,010H,048H,0D4H,0B4H,062H,0C2H,082H,002H
DB  002H,002H,002H,084H,0C4H,0E0H,0A0H,0E0H,060H,020H,020H,000H,000H,000H,000H,000H
DB  000H,000H,000H,000H,0FEH,001H,000H,004H,00DH,01BH,036H,06DH,0DBH,0F6H,0EDH,0FBH
DB  0FEH,0FEH,0FFH,07FH,01FH,007H,001H,000H,000H,000H,0F0H,000H,000H,000H,000H,000H
DB  000H,000H,000H,000H,000H,003H,004H,008H,010H,020H,040H,040H,080H,083H,0FFH,0FFH
DB  0FFH,0FFH,083H,040H,040H,020H,010H,008H,004H,003H,000H,000H,000H,000H,000H,000H
DB  07CH,044H,0ECH,000H,07CH,044H,07CH,000H,07CH,040H,040H,000H,07CH,044H,038H,000H
DB  07CH,014H,01CH,000H,07CH,040H,040H,000H,078H,024H,078H,000H,07CH,018H,018H,07CH
GPTB2:
;--  调入了一幅图像：F:\梁\画图\GPTB.bmp  --
;--  宽度x高度=32x32  --
DB  0FFH,0FFH,0FFH,0FFH,0FFH,07FH,0BFH,0DFH,0EFH,0B7H,02BH,04BH,09DH,03DH,07DH,0FDH
DB  0FDH,0FDH,0FDH,07BH,03BH,01FH,05FH,01FH,09FH,0DFH,0DFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB  0FFH,0FFH,0FFH,0FFH,001H,0FEH,0FFH,0FBH,0F2H,0E4H,0C9H,092H,024H,009H,012H,004H
DB  001H,001H,000H,080H,0E0H,0F8H,0FEH,0FFH,0FFH,0FFH,00FH,0FFH,0FFH,0FFH,0FFH,0FFH
DB  0FFH,0FFH,0FFH,0FFH,0FFH,0FCH,0FBH,0F7H,0EFH,0DFH,0BFH,0BFH,07FH,07CH,000H,000H
DB  000H,000H,07CH,0BFH,0BFH,0DFH,0EFH,0F7H,0FBH,0FCH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB  083H,0BBH,013H,0FFH,083H,0BBH,083H,0FFH,083H,0BFH,0BFH,0FFH,083H,0BBH,0C7H,0FFH
DB  083H,0EBH,0E3H,0FFH,083H,0BFH,0BFH,0FFH,087H,0DBH,087H,0FFH,083H,0E7H,0E7H,083H
HOCO:
;-------------------------------------------------------------------------------
;  源文件 / 文字 : G:\梁\画图\hoco19264.bmp
;  宽×高（像素）: 192×64
;-------------------------------------------------------------------------------
DB 0FFH,003H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,081H,0C1H
DB 0E1H,0E1H,0F1H,0F9H,0F9H,0F9H,0F9H,0FDH,0FDH,0FDH,0FDH,0FDH,0FDH,0FDH,0FDH,0F9H
DB 0F1H,0F1H,0E1H,081H,081H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H
DB 001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H
DB 001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H
DB 001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H
DB 001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,081H,001H
DB 001H,081H,0C1H,0E1H,0A1H,081H,0C1H,0C1H,0E1H,0C1H,0C1H,0C1H,0C1H,061H,071H,001H
DB 001H,001H,081H,081H,0C1H,0C1H,061H,031H,001H,001H,001H,001H,001H,001H,001H,001H
DB 001H,001H,081H,081H,0C1H,0C1H,0C1H,0C1H,0E1H,0E1H,0E1H,0F1H,0F1H,0F1H,0F1H,0F1H
DB 0F1H,0F1H,0F1H,0F1H,0F1H,0F1H,0F1H,0E1H,0E1H,0E1H,0E1H,0C1H,0C1H,0C1H,081H,081H
DB 001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,001H,003H,0FFH
DB 0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,01FH,0B5H,05FH
DB 0FFH,0FFH,0CFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FEH,0F8H,0F0H,080H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0E0H,070H,07FH,07FH
DB 0FEH,0F7H,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0BFH,03FH,01FH,01FH,01FH,00FH
DB 00FH,007H,003H,001H,001H,000H,000H,000H,000H,0C0H,0E0H,0F0H,0F8H,0FCH,0FCH,0FEH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FDH,0FDH,0DDH,0DDH,0DCH,0DCH,0DCH
DB 0DCH,09CH,09CH,09CH,01CH,038H,038H,038H,071H,071H,0E1H,0E3H,0C3H,083H,007H,00FH
DB 00FH,03EH,07CH,0F0H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0FFH
DB 0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,01FH,03FH,03FH,027H,01FH,000H,006H,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FCH,0F8H,0F8H,0F8H,0F8H,0F0H,0F0H
DB 0F0H,0F0H,0F0H,0F0H,0F8H,0F8H,0F8H,0F8H,0F8H,0F8H,0F8H,0F8H,0F8H,0F8H,0F8H,0F0H
DB 0F0H,0F0H,0E0H,0C0H,080H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,0C0H,0E0H,0F0H,0F8H,0FCH,0FEH,0FEH,0FEH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FCH,000H,000H
DB 000H,000H,000H,000H,000H,0E0H,0F8H,0FEH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FEH,0FEH,0F8H,0F0H,0C0H,001H,003H,08FH,0FFH,07CH
DB 000H,000H,000H,0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0FFH
DB 0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,080H,08FH,09FH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FEH,0F0H,0C0H,080H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,03FH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FEH,0FEH
DB 0FCH,0FCH,0FCH,0FEH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0BFH
DB 07FH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0CFH,03FH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,007H,03FH,0FFH,03FH,000H,000H,001H,000H,000H
DB 000H,002H,001H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0FFH
DB 0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 0FCH,0FFH,0FFH,01FH,00FH,007H,007H,007H,007H,007H,007H,007H,007H,003H,087H,0FFH
DB 0FFH,0FFH,0FFH,07FH,00FH,00FH,00FH,00FH,00FH,00FH,00FH,00FH,00FH,00FH,00FH,00FH
DB 00FH,00FH,00FH,00FH,03FH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,07FH,01FH,00FH,03FH,0FFH,0FFH,0FFH,0FEH,0F0H,0E0H,0C0H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,001H,003H,007H,007H,01FH,01FH,03FH,07FH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH
DB 0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,0FFH,07FH,03FH
DB 00FH,000H,0C3H,0FFH,0FFH,0FFH,07FH,03FH,0FFH,0FFH,0FFH,0FFH,07FH,03FH,007H,000H
DB 001H,07FH,03FH,01FH,007H,003H,000H,000H,0FEH,001H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0FFH
DB 0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,03CH
DB 07FH,0FFH,0FEH,0F8H,0F0H,000H,000H,000H,000H,000H,000H,000H,000H,0FCH,0FFH,07FH
DB 03FH,00FH,001H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,003H,007H,00FH,01FH,03FH,0FFH,0FFH,0FEH,0F0H,0C7H
DB 00FH,01FH,0FFH,0FFH,0FFH,0F8H,0E0H,080H,000H,001H,003H,0FFH,0FFH,0FFH,0E3H,0C7H
DB 08EH,01CH,038H,070H,0C0H,080H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,001H,003H,003H,007H,007H,00FH,00FH,01FH,0FFH,0FFH,07FH,03FH,09FH,00FH,00FH
DB 01FH,03FH,07FH,07FH,0FFH,0FFH,0FFH,0FFH,0FFH,0FDH,03FH,003H,001H,007H,008H,000H
DB 000H,00EH,007H,003H,003H,001H,000H,007H,003H,003H,001H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0FFH
DB 0FFH,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,080H,0C0H,0C0H,0E0H,0F0H,0FCH,07FH,03FH,01FH,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,080H,0F0H,0F8H,0FFH,0FFH,03FH,01FH
DB 00FH,007H,007H,003H,003H,001H,001H,000H,000H,000H,000H,001H,00FH,01FH,01FH,03FH
DB 031H,021H,042H,000H,000H,00FH,010H,020H,040H,080H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,0C0H,030H,008H,09CH,007H,001H,040H,040H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,024H,01CH,009H,007H,00AH,002H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H
DB 000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,000H,0FFH
DB 0FFH,0C0H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,081H,081H,081H,081H,081H,081H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,0B0H,0B8H,0BCH,0BEH,0BFH,0BFH,087H,083H,081H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,084H,084H,084H,084H,084H
DB 08CH,086H,082H,083H,081H,084H,080H,080H,083H,080H,080H,081H,080H,080H,082H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H
DB 080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,080H,0C0H,0FFH

		END
		