
;CodeVisionAVR C Compiler V2.05.3 Standard
;(C) Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega2560
;Program type             : Application
;Clock frequency          : 8.000000 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 1024 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;Global 'const' stored in FLASH     : No
;Enhanced function parameter passing: Yes
;Enhanced core instructions         : On
;Smart register allocation          : On
;Automatic register allocation      : On

	#pragma AVRPART ADMIN PART_NAME ATmega2560
	#pragma AVRPART MEMORY PROG_FLASH 262144
	#pragma AVRPART MEMORY EEPROM 4096
	#pragma AVRPART MEMORY INT_SRAM SIZE 8703
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x200

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU EECR=0x1F
	.EQU EEDR=0x20
	.EQU EEARL=0x21
	.EQU EEARH=0x22
	.EQU SPSR=0x2D
	.EQU SPDR=0x2E
	.EQU SMCR=0x33
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU WDTCSR=0x60
	.EQU UCSR0A=0xC0
	.EQU UDR0=0xC6
	.EQU RAMPZ=0x3B
	.EQU EIND=0x3C
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU XMCRA=0x74
	.EQU XMCRB=0x75
	.EQU GPIOR0=0x1E

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0200
	.EQU __SRAM_END=0x21FF
	.EQU __DSTACK_SIZE=0x0400
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __GETD1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X+
	LD   R22,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;GPIOR0 INITIALIZATION VALUE
	.EQU __GPIOR0_INIT=0x00

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

_tbl10_G100:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G100:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

_0x0:
	.DB  0x52,0x65,0x61,0x64,0x20,0x41,0x44,0x43
	.DB  0x20,0x43,0x48,0x30,0x20,0x3A,0x20,0x25
	.DB  0x64,0xD,0xA,0x0
_0x2060060:
	.DB  0x1
_0x2060000:
	.DB  0x2D,0x4E,0x41,0x4E,0x0,0x49,0x4E,0x46
	.DB  0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  __seed_G103
	.DW  _0x2060060*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30
	STS  XMCRA,R30
	STS  XMCRB,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	WDR
	IN   R26,MCUSR
	CBR  R26,8
	OUT  MCUSR,R26
	STS  WDTCSR,R31
	STS  WDTCSR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

	OUT  RAMPZ,R24

	OUT  EIND,R24

;GPIOR0 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x600

	.CSEG
;#include <mega2560.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.EQU __sm_adc_noise_red=0x02
	.SET power_ctrl_reg=smcr
	#endif
;#include <stdio.h>
;#include <string.h>
;#include <ctype.h>
;#include <delay.h>
;
;
;
;#include "nx_io.h"
;#include "nx_usart.h"
;#include "nx_config.h"
;
;void main(void)
; 0000 000E {

	.CSEG
_main:
; 0000 000F       int ADC=0;
; 0000 0010       int duty=0;
; 0000 0011       config_mcu(); //config ALL MCU AT mega 2560
;	ADC -> R16,R17
;	duty -> R18,R19
	__GETWRN 16,17,0
	__GETWRN 18,19,0
	CALL _config_mcu
; 0000 0012       init_portin(PF); //init port PF is input
	LDI  R30,LOW(0)
	OUT  0x10,R30
; 0000 0013       init_analogpin(A0); //init pin A0 is analogp input
	LDI  R26,LOW(0)
	LDI  R27,0
	RCALL _init_analogpin
; 0000 0014       init_uart(1,38400); //init Uart CH1 Baudrate 38400
	LDI  R30,LOW(1)
	ST   -Y,R30
	LDI  R26,LOW(38400)
	LDI  R27,HIGH(38400)
	CALL _init_uart
; 0000 0015 
; 0000 0016       while(1)
_0x3:
; 0000 0017       {
; 0000 0018            Set_PWM(1 ,duty); //Set output PWM CH1  = duty
	LDI  R30,LOW(1)
	ST   -Y,R30
	MOVW R26,R18
	RCALL _Set_PWM
; 0000 0019            Set_PWM(2 ,duty); //Set output PWM CH2  = duty
	LDI  R30,LOW(2)
	ST   -Y,R30
	MOVW R26,R18
	RCALL _Set_PWM
; 0000 001A 
; 0000 001B            ADC = analog_read(A0); //Read analogp input A0
	LDI  R26,LOW(0)
	RCALL _analog_read
	MOVW R16,R30
; 0000 001C            printDebug("Read ADC CH0 : %d\r\n",ADC);
	__POINTW1FN _0x0,0
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R16
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _printDebug
	ADIW R28,6
; 0000 001D            delay_ms(500);
	LDI  R26,LOW(500)
	LDI  R27,HIGH(500)
	CALL _delay_ms
; 0000 001E            duty=duty+50;
	__ADDWRN 18,19,50
; 0000 001F            if(duty>255)
	__CPWRN 18,19,256
	BRLT _0x6
; 0000 0020            {
; 0000 0021                 duty=0;
	__GETWRN 18,19,0
; 0000 0022            }
; 0000 0023 
; 0000 0024 
; 0000 0025       }
_0x6:
	RJMP _0x3
; 0000 0026 
; 0000 0027 }
_0x7:
	RJMP _0x7
;
;
;
;
;#include <mega2560.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.EQU __sm_adc_noise_red=0x02
	.SET power_ctrl_reg=smcr
	#endif
;#include <stdlib.h>
;#include <string.h>
;#include <delay.h>
;#include <stdio.h>
;#include "nx_io.h"
;
;#define ADC_VREF_TYPE 0x00
;
;// Read the ADC conversion result
;// Analog 0-1024 Port PF0 - PF8 (A0-A7)
;// Analog (-512)-512 Port PK0 - PK8 (A8-A15)
;// example
;// init_analogpin(A0);
;unsigned int analog_read(unsigned char adc_input)
; 0001 0010 {

	.CSEG
_analog_read:
; 0001 0011       ADMUX = (0<<REFS1)|(1<<REFS0)|adc_input;
	ST   -Y,R26
;	adc_input -> Y+0
	LD   R30,Y
	ORI  R30,0x40
	STS  124,R30
; 0001 0012       ADCSRA |=(1<<ADSC);
	LDS  R30,122
	ORI  R30,0x40
	STS  122,R30
; 0001 0013       while(!(ADCSRA&(1<<ADIF)));
_0x20003:
	LDS  R30,122
	ANDI R30,LOW(0x10)
	BREQ _0x20003
; 0001 0014       return ADCW;
	LDS  R30,120
	LDS  R31,120+1
	RJMP _0x20A0002
; 0001 0015 }
;
;void init_analogpin(unsigned int pin)
; 0001 0018 {
_init_analogpin:
; 0001 0019 
; 0001 001A       ACSR=0x80;
	ST   -Y,R27
	ST   -Y,R26
;	pin -> Y+0
	LDI  R30,LOW(128)
	OUT  0x30,R30
; 0001 001B       ADCSRB=0x00;
	LDI  R30,LOW(0)
	STS  123,R30
; 0001 001C       DIDR1=0x00;
	STS  127,R30
; 0001 001D       DIDR0=0x00;
	STS  126,R30
; 0001 001E       DIDR2=0x00;
	STS  125,R30
; 0001 001F       ADMUX=ADC_VREF_TYPE & 0xff;
	STS  124,R30
; 0001 0020       ADCSRA=0x83;
	LDI  R30,LOW(131)
	STS  122,R30
; 0001 0021 }
	ADIW R28,2
	RET
;
;void Set_PWM(char ch ,int duty )
; 0001 0024 {
_Set_PWM:
; 0001 0025     switch (ch)
	CALL SUBOPT_0x0
;	ch -> Y+2
;	duty -> Y+0
; 0001 0026     {
; 0001 0027         case 1: DDRB |= (1 << DDB7);
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x20009
	SBI  0x4,7
; 0001 0028                 //OCR0A = 50;
; 0001 0029                 TCCR0A |= (1 << COM0A1);
	IN   R30,0x24
	ORI  R30,0x80
	OUT  0x24,R30
; 0001 002A                 // set non-inverting mode
; 0001 002B                 TCCR0A |= (1 << WGM01) | (1 << WGM00);
	IN   R30,0x24
	ORI  R30,LOW(0x3)
	OUT  0x24,R30
; 0001 002C                 // set 10bit phase corrected PWM Mode
; 0001 002D                 TCCR0B |= (1 << CS01);
	IN   R30,0x25
	ORI  R30,2
	OUT  0x25,R30
; 0001 002E                 // set prescaler to 8 and starts PWM
; 0001 002F                 OCR0A = duty;
	LD   R30,Y
	OUT  0x27,R30
; 0001 0030                 break;
	RJMP _0x20008
; 0001 0031         case 2:
_0x20009:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x20008
; 0001 0032                 DDRB |= (1 << DDB4);
	SBI  0x4,4
; 0001 0033                  //OCR0A = 50;
; 0001 0034                 TCCR2A |= (1 << COM2A1);
	LDS  R30,176
	ORI  R30,0x80
	STS  176,R30
; 0001 0035                 // set non-inverting mode
; 0001 0036                 TCCR2A |= (1 << WGM21) | (1 << WGM20);
	LDS  R30,176
	ORI  R30,LOW(0x3)
	STS  176,R30
; 0001 0037                 // set 10bit phase corrected PWM Mode
; 0001 0038                 TCCR2B |= (1 << CS21);
	LDS  R30,177
	ORI  R30,2
	STS  177,R30
; 0001 0039                 OCR2A = duty;
	LD   R30,Y
	STS  179,R30
; 0001 003A                 // set prescaler to 8 and starts PWM
; 0001 003B                 break;
; 0001 003C     }
_0x20008:
; 0001 003D 
; 0001 003E }
	RJMP _0x20A0003
;
;#include <mega2560.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.EQU __sm_adc_noise_red=0x02
	.SET power_ctrl_reg=smcr
	#endif
;#include <stdio.h>
;#include <string.h>
;#include <ctype.h>
;#include <delay.h>
;
;
;#include "nx_usart.h"
;/*============================================================*/
;/* Baudrate set 9600 14400 38400                                                           */
;/*============================================================*/
;void init_uart(char port , int Baudrate)
; 0002 000D {     switch (port)

	.CSEG
_init_uart:
	CALL SUBOPT_0x0
;	port -> Y+2
;	Baudrate -> Y+0
; 0002 000E       {
; 0002 000F             case 0:
	SBIW R30,0
	BRNE _0x40006
; 0002 0010                   if(Baudrate==9600)
	CALL SUBOPT_0x1
	BRNE _0x40007
; 0002 0011                   {
; 0002 0012                         UCSR0A=0x00;
	CALL SUBOPT_0x2
; 0002 0013                         UCSR0B=0x98;
; 0002 0014                         UCSR0C=0x06;
; 0002 0015                         UBRR0H=0x00;
; 0002 0016                         UBRR0L=0x33;
	LDI  R30,LOW(51)
	RJMP _0x4003D
; 0002 0017                   }
; 0002 0018                   else if(Baudrate==14400)
_0x40007:
	CALL SUBOPT_0x3
	BRNE _0x40009
; 0002 0019                   {
; 0002 001A                         UCSR0A=0x00;
	CALL SUBOPT_0x2
; 0002 001B                         UCSR0B=0x98;
; 0002 001C                         UCSR0C=0x06;
; 0002 001D                         UBRR0H=0x00;
; 0002 001E                         UBRR0L=0x22;
	LDI  R30,LOW(34)
	RJMP _0x4003D
; 0002 001F                   }
; 0002 0020                   else if(Baudrate==38400)
_0x40009:
	CALL SUBOPT_0x4
	BRNE _0x4000B
; 0002 0021                   {
; 0002 0022                       UCSR0A=0x00;
	CALL SUBOPT_0x2
; 0002 0023                       UCSR0B=0x98;
; 0002 0024                       UCSR0C=0x06;
; 0002 0025                       UBRR0H=0x00;
; 0002 0026                       UBRR0L=0x0C;
	LDI  R30,LOW(12)
_0x4003D:
	STS  196,R30
; 0002 0027                   }
; 0002 0028                   break;
_0x4000B:
	RJMP _0x40005
; 0002 0029 
; 0002 002A             case 1:
_0x40006:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x4000C
; 0002 002B                   if(Baudrate==9600)
	CALL SUBOPT_0x1
	BRNE _0x4000D
; 0002 002C                   {
; 0002 002D                       UCSR1A=0x00;
	CALL SUBOPT_0x5
; 0002 002E                       UCSR1B=0x98;
; 0002 002F                       UCSR1C=0x06;
; 0002 0030                       UBRR1H=0x00;
; 0002 0031                       UBRR1L=0x33;
	LDI  R30,LOW(51)
	RJMP _0x4003E
; 0002 0032                   }
; 0002 0033                   else if(Baudrate==14400)
_0x4000D:
	CALL SUBOPT_0x3
	BRNE _0x4000F
; 0002 0034                   {
; 0002 0035                         UCSR1A=0x00;
	CALL SUBOPT_0x5
; 0002 0036                         UCSR1B=0x98;
; 0002 0037                         UCSR1C=0x06;
; 0002 0038                         UBRR1H=0x00;
; 0002 0039                         UBRR1L=0x22;
	LDI  R30,LOW(34)
	RJMP _0x4003E
; 0002 003A                   }
; 0002 003B                   else if(Baudrate==38400)
_0x4000F:
	CALL SUBOPT_0x4
	BRNE _0x40011
; 0002 003C                   {
; 0002 003D                       UCSR1A=0x00;
	CALL SUBOPT_0x5
; 0002 003E                       UCSR1B=0x98;
; 0002 003F                       UCSR1C=0x06;
; 0002 0040                       UBRR1H=0x00;
; 0002 0041                       UBRR1L=0x0C;
	LDI  R30,LOW(12)
_0x4003E:
	STS  204,R30
; 0002 0042                   }
; 0002 0043                   break;
_0x40011:
	RJMP _0x40005
; 0002 0044 
; 0002 0045             case 2:
_0x4000C:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x40012
; 0002 0046                    if(Baudrate==9600)
	CALL SUBOPT_0x1
	BRNE _0x40013
; 0002 0047                   {
; 0002 0048                       UCSR2A=0x00;
	CALL SUBOPT_0x6
; 0002 0049                       UCSR2B=0x98;
; 0002 004A                       UCSR2C=0x06;
; 0002 004B                       UBRR2H=0x00;
; 0002 004C                       UBRR2L=0x33;
	LDI  R30,LOW(51)
	RJMP _0x4003F
; 0002 004D                   }
; 0002 004E                   else if(Baudrate==14400)
_0x40013:
	CALL SUBOPT_0x3
	BRNE _0x40015
; 0002 004F                   {
; 0002 0050                         UCSR2A=0x00;
	CALL SUBOPT_0x6
; 0002 0051                         UCSR2B=0x98;
; 0002 0052                         UCSR2C=0x06;
; 0002 0053                         UBRR2H=0x00;
; 0002 0054                         UBRR2L=0x22;
	LDI  R30,LOW(34)
	RJMP _0x4003F
; 0002 0055                   }
; 0002 0056                   else if(Baudrate==38400)
_0x40015:
	CALL SUBOPT_0x4
	BRNE _0x40017
; 0002 0057                   {
; 0002 0058                       UCSR2A=0x00;
	CALL SUBOPT_0x6
; 0002 0059                       UCSR2B=0x98;
; 0002 005A                       UCSR2C=0x06;
; 0002 005B                       UBRR2H=0x00;
; 0002 005C                       UBRR2L=0x0C;
	LDI  R30,LOW(12)
_0x4003F:
	STS  212,R30
; 0002 005D                   }
; 0002 005E                   break;
_0x40017:
	RJMP _0x40005
; 0002 005F 
; 0002 0060             case 3:
_0x40012:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x40005
; 0002 0061                   if(Baudrate==9600)
	CALL SUBOPT_0x1
	BRNE _0x40019
; 0002 0062                   {
; 0002 0063                       UCSR3A=0x00;
	CALL SUBOPT_0x7
; 0002 0064                       UCSR3B=0x98;
; 0002 0065                       UCSR3C=0x06;
; 0002 0066                       UBRR3H=0x00;
; 0002 0067                       UBRR3L=0x33;
	LDI  R30,LOW(51)
	RJMP _0x40040
; 0002 0068                   }
; 0002 0069                   else if(Baudrate==14400)
_0x40019:
	CALL SUBOPT_0x3
	BRNE _0x4001B
; 0002 006A                   {
; 0002 006B                         UCSR3A=0x00;
	CALL SUBOPT_0x7
; 0002 006C                         UCSR3B=0x98;
; 0002 006D                         UCSR3C=0x06;
; 0002 006E                         UBRR3H=0x00;
; 0002 006F                         UBRR3L=0x22;
	LDI  R30,LOW(34)
	RJMP _0x40040
; 0002 0070                   }
; 0002 0071                   else if(Baudrate==38400)
_0x4001B:
	CALL SUBOPT_0x4
	BRNE _0x4001D
; 0002 0072                   {
; 0002 0073                       UCSR3A=0x00;
	CALL SUBOPT_0x7
; 0002 0074                       UCSR3B=0x98;
; 0002 0075                       UCSR3C=0x06;
; 0002 0076                       UBRR3H=0x00;
; 0002 0077                       UBRR3L=0x0C;
	LDI  R30,LOW(12)
_0x40040:
	STS  308,R30
; 0002 0078                   }
; 0002 0079                   break;
_0x4001D:
; 0002 007A       }
_0x40005:
; 0002 007B 
; 0002 007C }
_0x20A0003:
	ADIW R28,3
	RET
;
;/*============================================================*/
;#pragma used+
;void putchar0(char c)
; 0002 0081 {
_putchar0:
; 0002 0082     while ((UCSR0A & DATA_REGISTER_EMPTY)==0);
	ST   -Y,R26
;	c -> Y+0
_0x4001E:
	LDS  R30,192
	ANDI R30,LOW(0x20)
	BREQ _0x4001E
; 0002 0083     UDR0=c;
	LD   R30,Y
	STS  198,R30
; 0002 0084 }
	RJMP _0x20A0002
;#pragma used-
;#pragma used+
;void putchar1(char c)
; 0002 0088 {
_putchar1:
; 0002 0089     while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
	ST   -Y,R26
;	c -> Y+0
_0x40021:
	LDS  R30,200
	ANDI R30,LOW(0x20)
	BREQ _0x40021
; 0002 008A     UDR1=c;
	LD   R30,Y
	STS  206,R30
; 0002 008B }
	RJMP _0x20A0002
;#pragma used-
;#pragma used+
;void putchar2(char c)
; 0002 008F {
_putchar2:
; 0002 0090     while ((UCSR2A & DATA_REGISTER_EMPTY)==0);
	ST   -Y,R26
;	c -> Y+0
_0x40024:
	LDS  R30,208
	ANDI R30,LOW(0x20)
	BREQ _0x40024
; 0002 0091     UDR2=c;
	LD   R30,Y
	STS  214,R30
; 0002 0092 }
	RJMP _0x20A0002
;#pragma used-
;#pragma used+
;void putchar3(char c)
; 0002 0096 {
_putchar3:
; 0002 0097     while ((UCSR3A & DATA_REGISTER_EMPTY)==0);
	ST   -Y,R26
;	c -> Y+0
_0x40027:
	LDS  R30,304
	ANDI R30,LOW(0x20)
	BREQ _0x40027
; 0002 0098     UDR3=c;
	LD   R30,Y
	STS  310,R30
; 0002 0099 }
_0x20A0002:
	ADIW R28,1
	RET
;#pragma used-
;
;/*============================================================*/
;void printDebug(flash char *fmtstr, ...)
; 0002 009E {
_printDebug:
	PUSH R15
	MOV  R15,R24
; 0002 009F     char textBuffer[256];
; 0002 00A0     int num=0;
; 0002 00A1     va_list argptr;
; 0002 00A2 
; 0002 00A3     va_start(argptr, fmtstr);
	SUBI R29,1
	CALL __SAVELOCR4
;	*fmtstr -> Y+260
;	textBuffer -> Y+4
;	num -> R16,R17
;	*argptr -> R18,R19
	__GETWRN 16,17,0
	MOVW R26,R28
	SUBI R26,LOW(-(256))
	SBCI R27,HIGH(-(256))
	CALL __ADDW2R15
	MOVW R18,R26
; 0002 00A4     vsprintf(textBuffer,fmtstr,argptr);
	MOVW R30,R28
	ADIW R30,4
	ST   -Y,R31
	ST   -Y,R30
	MOVW R26,R28
	SUBI R26,LOW(-(262))
	SBCI R27,HIGH(-(262))
	CALL __ADDW2R15
	CALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	MOVW R26,R18
	CALL _vsprintf
; 0002 00A5 
; 0002 00A6     send_uart(1, textBuffer);
	LDI  R30,LOW(1)
	ST   -Y,R30
	MOVW R26,R28
	ADIW R26,5
	RCALL _send_uart
; 0002 00A7 
; 0002 00A8     va_end(argptr);
; 0002 00A9 
; 0002 00AA     return;
	CALL __LOADLOCR4
	ADIW R28,4
	SUBI R29,-1
	POP  R15
	RET
; 0002 00AB }
;/*============================================================*/
;
;/*============================================================*/
;void send_uart(char port,char *buffer)
; 0002 00B0 {
_send_uart:
; 0002 00B1     char i = 0;
; 0002 00B2 
; 0002 00B3     switch (port)
	ST   -Y,R27
	ST   -Y,R26
	ST   -Y,R17
;	port -> Y+3
;	*buffer -> Y+1
;	i -> R17
	LDI  R17,0
	LDD  R30,Y+3
	LDI  R31,0
; 0002 00B4     {
; 0002 00B5         case 0: while(buffer[i] != 0)
	SBIW R30,0
	BRNE _0x4002D
_0x4002E:
	CALL SUBOPT_0x8
	LD   R30,X
	CPI  R30,0
	BREQ _0x40030
; 0002 00B6                 {
; 0002 00B7                     putchar0(buffer[i]);
	CALL SUBOPT_0x8
	LD   R26,X
	RCALL _putchar0
; 0002 00B8                     i++;
	SUBI R17,-1
; 0002 00B9                 }
	RJMP _0x4002E
_0x40030:
; 0002 00BA                 break;
	RJMP _0x4002C
; 0002 00BB 
; 0002 00BC         case 1: while(buffer[i] != 0)
_0x4002D:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x40031
_0x40032:
	CALL SUBOPT_0x8
	LD   R30,X
	CPI  R30,0
	BREQ _0x40034
; 0002 00BD                 {
; 0002 00BE                     putchar1(buffer[i]);
	CALL SUBOPT_0x8
	LD   R26,X
	RCALL _putchar1
; 0002 00BF                     i++;
	SUBI R17,-1
; 0002 00C0                 }
	RJMP _0x40032
_0x40034:
; 0002 00C1                 break;
	RJMP _0x4002C
; 0002 00C2 
; 0002 00C3         case 2: while(buffer[i] != 0)
_0x40031:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x40035
_0x40036:
	CALL SUBOPT_0x8
	LD   R30,X
	CPI  R30,0
	BREQ _0x40038
; 0002 00C4                 {
; 0002 00C5                     putchar2(buffer[i]);
	CALL SUBOPT_0x8
	LD   R26,X
	RCALL _putchar2
; 0002 00C6                     i++;
	SUBI R17,-1
; 0002 00C7                 }
	RJMP _0x40036
_0x40038:
; 0002 00C8                 break;
	RJMP _0x4002C
; 0002 00C9 
; 0002 00CA         case 3: while(buffer[i] != 0)
_0x40035:
	CPI  R30,LOW(0x3)
	LDI  R26,HIGH(0x3)
	CPC  R31,R26
	BRNE _0x4002C
_0x4003A:
	CALL SUBOPT_0x8
	LD   R30,X
	CPI  R30,0
	BREQ _0x4003C
; 0002 00CB                 {
; 0002 00CC                     putchar3(buffer[i]);
	CALL SUBOPT_0x8
	LD   R26,X
	RCALL _putchar3
; 0002 00CD                     i++;
	SUBI R17,-1
; 0002 00CE                 }
	RJMP _0x4003A
_0x4003C:
; 0002 00CF                 break;
; 0002 00D0     }
_0x4002C:
; 0002 00D1 }
	LDD  R17,Y+0
	ADIW R28,4
	RET
;/*============================================================*/
;#include <mega2560.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.EQU __sm_adc_noise_red=0x02
	.SET power_ctrl_reg=smcr
	#endif
;#include <stdio.h>
;#include <string.h>
;#include <ctype.h>
;#include <delay.h>
;
;#include "nx_config.h"
;
;void config_mcu()
; 0003 000A {

	.CSEG
_config_mcu:
; 0003 000B       // Declare your local variables here
; 0003 000C 
; 0003 000D       // Crystal Oscillator division factor: 1
; 0003 000E       #pragma optsize-
; 0003 000F       CLKPR=0x80;
	LDI  R30,LOW(128)
	STS  97,R30
; 0003 0010       CLKPR=0x00;
	LDI  R30,LOW(0)
	STS  97,R30
; 0003 0011       #ifdef _OPTIMIZE_SIZE_
; 0003 0012       #pragma optsize+
; 0003 0013       #endif
; 0003 0014 
; 0003 0015       // Reset Source checking
; 0003 0016       if (MCUSR & 1)
	IN   R30,0x34
	SBRC R30,0
; 0003 0017       {
; 0003 0018             // Power-on Reset
; 0003 0019             MCUSR&=0xE0;
	RJMP _0x6000C
; 0003 001A             // Place your code here
; 0003 001B 
; 0003 001C       }
; 0003 001D       else if (MCUSR & 2)
	IN   R30,0x34
	SBRC R30,1
; 0003 001E       {
; 0003 001F             // External Reset
; 0003 0020             MCUSR&=0xE0;
	RJMP _0x6000C
; 0003 0021             // Place your code here
; 0003 0022 
; 0003 0023       }
; 0003 0024       else if (MCUSR & 4)
	IN   R30,0x34
	SBRC R30,2
; 0003 0025       {
; 0003 0026             // Brown-Out Reset
; 0003 0027             MCUSR&=0xE0;
	RJMP _0x6000C
; 0003 0028             // Place your code here
; 0003 0029 
; 0003 002A       }
; 0003 002B       else if (MCUSR & 8)
	IN   R30,0x34
	SBRC R30,3
; 0003 002C       {
; 0003 002D             // Watchdog Reset
; 0003 002E             MCUSR&=0xE0;
	RJMP _0x6000C
; 0003 002F             // Place your code here
; 0003 0030 
; 0003 0031       }
; 0003 0032       else if (MCUSR & 0x10)
	IN   R30,0x34
	SBRS R30,4
	RJMP _0x6000B
; 0003 0033       {
; 0003 0034             // JTAG Reset
; 0003 0035             MCUSR&=0xE0;
_0x6000C:
	IN   R30,0x34
	ANDI R30,LOW(0xE0)
	OUT  0x34,R30
; 0003 0036             // Place your code here
; 0003 0037 
; 0003 0038       }
; 0003 0039       DIDR1=0xff;
_0x6000B:
	LDI  R30,LOW(255)
	STS  127,R30
; 0003 003A       DIDR0=0xff;
	STS  126,R30
; 0003 003B }
	RET
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.EQU __sm_adc_noise_red=0x02
	.SET power_ctrl_reg=smcr
	#endif

	.CSEG
_put_buff_G100:
	ST   -Y,R27
	ST   -Y,R26
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,2
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x2000016
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,4
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x2000018
	__CPWRN 16,17,2
	BRLO _0x2000019
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	__PUTW1SNS 2,4
_0x2000018:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,2
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	LDD  R26,Y+4
	STD  Z+0,R26
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	TST  R31
	BRMI _0x200001A
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
_0x200001A:
_0x2000019:
	RJMP _0x200001B
_0x2000016:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	ST   X+,R30
	ST   X,R31
_0x200001B:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,5
	RET
__print_G100:
	ST   -Y,R27
	ST   -Y,R26
	SBIW R28,6
	CALL __SAVELOCR6
	LDI  R17,0
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   X+,R30
	ST   X,R31
_0x200001C:
	LDD  R30,Y+18
	LDD  R31,Y+18+1
	ADIW R30,1
	STD  Y+18,R30
	STD  Y+18+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x200001E
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x2000022
	CPI  R18,37
	BRNE _0x2000023
	LDI  R17,LOW(1)
	RJMP _0x2000024
_0x2000023:
	CALL SUBOPT_0x9
_0x2000024:
	RJMP _0x2000021
_0x2000022:
	CPI  R30,LOW(0x1)
	BRNE _0x2000025
	CPI  R18,37
	BRNE _0x2000026
	CALL SUBOPT_0x9
	RJMP _0x20000CF
_0x2000026:
	LDI  R17,LOW(2)
	LDI  R20,LOW(0)
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x2000027
	LDI  R16,LOW(1)
	RJMP _0x2000021
_0x2000027:
	CPI  R18,43
	BRNE _0x2000028
	LDI  R20,LOW(43)
	RJMP _0x2000021
_0x2000028:
	CPI  R18,32
	BRNE _0x2000029
	LDI  R20,LOW(32)
	RJMP _0x2000021
_0x2000029:
	RJMP _0x200002A
_0x2000025:
	CPI  R30,LOW(0x2)
	BRNE _0x200002B
_0x200002A:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x200002C
	ORI  R16,LOW(128)
	RJMP _0x2000021
_0x200002C:
	RJMP _0x200002D
_0x200002B:
	CPI  R30,LOW(0x3)
	BREQ PC+3
	JMP _0x2000021
_0x200002D:
	CPI  R18,48
	BRLO _0x2000030
	CPI  R18,58
	BRLO _0x2000031
_0x2000030:
	RJMP _0x200002F
_0x2000031:
	LDI  R26,LOW(10)
	MUL  R21,R26
	MOV  R21,R0
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x2000021
_0x200002F:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x2000035
	CALL SUBOPT_0xA
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	LDD  R26,Z+4
	ST   -Y,R26
	CALL SUBOPT_0xB
	RJMP _0x2000036
_0x2000035:
	CPI  R30,LOW(0x73)
	BRNE _0x2000038
	CALL SUBOPT_0xA
	CALL SUBOPT_0xC
	CALL _strlen
	MOV  R17,R30
	RJMP _0x2000039
_0x2000038:
	CPI  R30,LOW(0x70)
	BRNE _0x200003B
	CALL SUBOPT_0xA
	CALL SUBOPT_0xC
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x2000039:
	ORI  R16,LOW(2)
	ANDI R16,LOW(127)
	LDI  R19,LOW(0)
	RJMP _0x200003C
_0x200003B:
	CPI  R30,LOW(0x64)
	BREQ _0x200003F
	CPI  R30,LOW(0x69)
	BRNE _0x2000040
_0x200003F:
	ORI  R16,LOW(4)
	RJMP _0x2000041
_0x2000040:
	CPI  R30,LOW(0x75)
	BRNE _0x2000042
_0x2000041:
	LDI  R30,LOW(_tbl10_G100*2)
	LDI  R31,HIGH(_tbl10_G100*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(5)
	RJMP _0x2000043
_0x2000042:
	CPI  R30,LOW(0x58)
	BRNE _0x2000045
	ORI  R16,LOW(8)
	RJMP _0x2000046
_0x2000045:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x2000077
_0x2000046:
	LDI  R30,LOW(_tbl16_G100*2)
	LDI  R31,HIGH(_tbl16_G100*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(4)
_0x2000043:
	SBRS R16,2
	RJMP _0x2000048
	CALL SUBOPT_0xA
	CALL SUBOPT_0xD
	LDD  R26,Y+11
	TST  R26
	BRPL _0x2000049
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	CALL __ANEGW1
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDI  R20,LOW(45)
_0x2000049:
	CPI  R20,0
	BREQ _0x200004A
	SUBI R17,-LOW(1)
	RJMP _0x200004B
_0x200004A:
	ANDI R16,LOW(251)
_0x200004B:
	RJMP _0x200004C
_0x2000048:
	CALL SUBOPT_0xA
	CALL SUBOPT_0xD
_0x200004C:
_0x200003C:
	SBRC R16,0
	RJMP _0x200004D
_0x200004E:
	CP   R17,R21
	BRSH _0x2000050
	SBRS R16,7
	RJMP _0x2000051
	SBRS R16,2
	RJMP _0x2000052
	ANDI R16,LOW(251)
	MOV  R18,R20
	SUBI R17,LOW(1)
	RJMP _0x2000053
_0x2000052:
	LDI  R18,LOW(48)
_0x2000053:
	RJMP _0x2000054
_0x2000051:
	LDI  R18,LOW(32)
_0x2000054:
	CALL SUBOPT_0x9
	SUBI R21,LOW(1)
	RJMP _0x200004E
_0x2000050:
_0x200004D:
	MOV  R19,R17
	SBRS R16,1
	RJMP _0x2000055
_0x2000056:
	CPI  R19,0
	BREQ _0x2000058
	SBRS R16,3
	RJMP _0x2000059
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	LPM  R18,Z+
	STD  Y+6,R30
	STD  Y+6+1,R31
	RJMP _0x200005A
_0x2000059:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R18,X+
	STD  Y+6,R26
	STD  Y+6+1,R27
_0x200005A:
	CALL SUBOPT_0x9
	CPI  R21,0
	BREQ _0x200005B
	SUBI R21,LOW(1)
_0x200005B:
	SUBI R19,LOW(1)
	RJMP _0x2000056
_0x2000058:
	RJMP _0x200005C
_0x2000055:
_0x200005E:
	LDI  R18,LOW(48)
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	CALL __GETW1PF
	STD  Y+8,R30
	STD  Y+8+1,R31
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,2
	STD  Y+6,R30
	STD  Y+6+1,R31
_0x2000060:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	CP   R26,R30
	CPC  R27,R31
	BRLO _0x2000062
	SUBI R18,-LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	SUB  R30,R26
	SBC  R31,R27
	STD  Y+10,R30
	STD  Y+10+1,R31
	RJMP _0x2000060
_0x2000062:
	CPI  R18,58
	BRLO _0x2000063
	SBRS R16,3
	RJMP _0x2000064
	SUBI R18,-LOW(7)
	RJMP _0x2000065
_0x2000064:
	SUBI R18,-LOW(39)
_0x2000065:
_0x2000063:
	SBRC R16,4
	RJMP _0x2000067
	CPI  R18,49
	BRSH _0x2000069
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,1
	BRNE _0x2000068
_0x2000069:
	RJMP _0x20000D0
_0x2000068:
	CP   R21,R19
	BRLO _0x200006D
	SBRS R16,0
	RJMP _0x200006E
_0x200006D:
	RJMP _0x200006C
_0x200006E:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x200006F
	LDI  R18,LOW(48)
_0x20000D0:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x2000070
	ANDI R16,LOW(251)
	ST   -Y,R20
	CALL SUBOPT_0xB
	CPI  R21,0
	BREQ _0x2000071
	SUBI R21,LOW(1)
_0x2000071:
_0x2000070:
_0x200006F:
_0x2000067:
	CALL SUBOPT_0x9
	CPI  R21,0
	BREQ _0x2000072
	SUBI R21,LOW(1)
_0x2000072:
_0x200006C:
	SUBI R19,LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,2
	BRLO _0x200005F
	RJMP _0x200005E
_0x200005F:
_0x200005C:
	SBRS R16,0
	RJMP _0x2000073
_0x2000074:
	CPI  R21,0
	BREQ _0x2000076
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	ST   -Y,R30
	CALL SUBOPT_0xB
	RJMP _0x2000074
_0x2000076:
_0x2000073:
_0x2000077:
_0x2000036:
_0x20000CF:
	LDI  R17,LOW(0)
_0x2000021:
	RJMP _0x200001C
_0x200001E:
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	CALL __GETW1P
	CALL __LOADLOCR6
	ADIW R28,20
	RET
_vsprintf:
	ST   -Y,R27
	ST   -Y,R26
	SBIW R28,6
	ST   -Y,R17
	ST   -Y,R16
	LDD  R30,Y+12
	LDD  R31,Y+12+1
	SBIW R30,0
	BRNE _0x200007B
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0001
_0x200007B:
	LDD  R30,Y+12
	LDD  R31,Y+12+1
	STD  Y+4,R30
	STD  Y+4+1,R31
	LDI  R30,LOW(0)
	STD  Y+6,R30
	STD  Y+6+1,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_put_buff_G100)
	LDI  R31,HIGH(_put_buff_G100)
	ST   -Y,R31
	ST   -Y,R30
	MOVW R26,R28
	ADIW R26,8
	RCALL __print_G100
	MOVW R16,R30
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LDI  R30,LOW(0)
	ST   X,R30
	MOVW R30,R16
_0x20A0001:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET

	.CSEG
_strlen:
	ST   -Y,R27
	ST   -Y,R26
    ld   r26,y+
    ld   r27,y+
    clr  r30
    clr  r31
strlen0:
    ld   r22,x+
    tst  r22
    breq strlen1
    adiw r30,1
    rjmp strlen0
strlen1:
    ret
_strlenf:
	ST   -Y,R27
	ST   -Y,R26
    clr  r26
    clr  r27
    ld   r30,y+
    ld   r31,y+
strlenf0:
	lpm  r0,z+
    tst  r0
    breq strlenf1
    adiw r26,1
    rjmp strlenf0
strlenf1:
    movw r30,r26
    ret

	.CSEG

	.CSEG

	.DSEG

	.CSEG

	.CSEG

	.DSEG
__seed_G103:
	.BYTE 0x4

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x0:
	ST   -Y,R27
	ST   -Y,R26
	LDD  R30,Y+2
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x1:
	LD   R26,Y
	LDD  R27,Y+1
	CPI  R26,LOW(0x2580)
	LDI  R30,HIGH(0x2580)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x2:
	LDI  R30,LOW(0)
	STS  192,R30
	LDI  R30,LOW(152)
	STS  193,R30
	LDI  R30,LOW(6)
	STS  194,R30
	LDI  R30,LOW(0)
	STS  197,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x3:
	LD   R26,Y
	LDD  R27,Y+1
	CPI  R26,LOW(0x3840)
	LDI  R30,HIGH(0x3840)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x4:
	LD   R26,Y
	LDD  R27,Y+1
	CPI  R26,LOW(0x9600)
	LDI  R30,HIGH(0x9600)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x5:
	LDI  R30,LOW(0)
	STS  200,R30
	LDI  R30,LOW(152)
	STS  201,R30
	LDI  R30,LOW(6)
	STS  202,R30
	LDI  R30,LOW(0)
	STS  205,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x6:
	LDI  R30,LOW(0)
	STS  208,R30
	LDI  R30,LOW(152)
	STS  209,R30
	LDI  R30,LOW(6)
	STS  210,R30
	LDI  R30,LOW(0)
	STS  213,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x7:
	LDI  R30,LOW(0)
	STS  304,R30
	LDI  R30,LOW(152)
	STS  305,R30
	LDI  R30,LOW(6)
	STS  306,R30
	LDI  R30,LOW(0)
	STS  309,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0x8:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x9:
	ST   -Y,R18
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xA:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB:
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xC:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	CALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xD:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	CALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
	RET


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x7D0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETW1PF:
	LPM  R0,Z+
	LPM  R31,Z
	MOV  R30,R0
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
