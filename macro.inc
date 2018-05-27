
; == DEFINES ==

SYS_STUB_ORG	EQU	$FF00
SYS_VECTOR_ORG	EQU	$FFFA

SAFE_STUB_ORG	EQU	SYS_STUB_ORG & $BFFF
SAFE_VECTOR_ORG	EQU	SYS_VECTOR_ORG & $BFFF

; == ENUMS ==

; == STRUCT ==

	MAC	TOBJ
{1}				EQU	.
{1}._player		EQU	.
		.BYTE 0
{1}._enemy		EQU .
		.BYTE 0,0,0
{1}._pbullet	EQU	.
		.BYTE 0,0,0
{1}._ebullet	EQU	.
		.BYTE 0,0,0
{1}._misc		EQU	.
		.BYTE 0
{1}._bonus		EQU	.
		.BYTE 0
{1}._spr0hit	EQU	.
		.BYTE 0
	ENDM

; == STUB ==

	MAC	SAFE_STUB0
		SEI
		CLD
		LDA		#$EF
		STA		$8000
		INC		$8000
		LDX		#0
		STX		_PPU_CTRL
		STX		_PPU_MASK
		STX		_PPU_STATUS
		STX		_PPU_OAM_ADDR
		STX		_PPU_OAM_DATA
		STX		_PPU_OAM_DMA
		DEX
		TXS
.1:
		BIT		_PPU_STATUS
		BPL		.1
.2:
		BIT		_PPU_STATUS
		BMI		.2
.3:
		BIT		_PPU_STATUS
		BPL		.3
.4:
		BIT		_PPU_STATUS
		BMI		.4
		LDA		#$80
		STA		$A000
		STA		$C000
		STA		$E000
		LDA		#$80
		STA		$8000
		LDA		#$1E
		STA		$8000
		LSR
		STA		$8000
		LSR
		STA		$8000
		LSR
		STA		$8000
		LSR
		STA		$8000
		LDA		#$80
		STA		$E000
		LDA		#7
		STA		$E000
		LSR
		STA		$E000
		LSR
		STA		$E000
		LSR
		STA		$E000
		LSR
		STA		$E000
		LDA		#$80
		STA		$8000
		LDA		#$1A
		STA		$8000
		LSR
		STA		$8000
		LSR
		STA		$8000
		LSR
		STA		$8000
		LSR
		STA		$8000
		LDA		#$80
		STA		$E000
		LDA		#2
		STA		$E000
		LSR
		STA		$E000
		LSR
		STA		$E000
		LSR
		STA		$E000
		LSR
		STA		$E000
		JMP		_j_RESET
	ENDM

	MAC	SAFE_STUB1
		PHA
		LDA		_PPU_STATUS
		PLA
	ENDM
	
	MAC	SAFE_STUB2
		RTI
	ENDM
	
	MAC	SAFE_STUB
		ORG	SAFE_STUB_ORG, $00
.1:		SAFE_STUB0
.2:		SAFE_STUB1
.3:		SAFE_STUB2
		ORG	SAFE_VECTOR_ORG, $00
		.WORD	.2
		.WORD	RESET
		.WORD	.3
	ENDM