
; == DEFINES ==

SYS_STUB_ORG	EQU	$FF00
SYS_VECTOR_ORG	EQU	$FFFA

SAFE_STUB_ORG	EQU	SYS_STUB_ORG & $BFFF
SAFE_VECTOR_ORG	EQU	SYS_VECTOR_ORG & $BFFF

; == DAMAGE TWEAKS ==

MIN_PERCENT_VALUE		EQU	$19
DEF_PERCENT_VALUE		EQU	$32
MAX_PERCENT_VALUE		EQU	$64
UPPER_DAMAGE_ALERT		EQU	$14
LOWER_DAMAGE_ALERT		EQU	$E
FAILURE_TRESHOLD		EQU	$18
DEF_PERCENT_POWERUP		EQU	6
DEF_PERCENT_REPAIR		EQU $A
ACID_DAMAGE_VALUE		EQU	4
BACK_DAMAGE_VALUE		EQU	2						; additional damage from regular gunmans if close up from back (no shoot)
NINJA_DAMAGE_VALUE		EQU $E
PROJ0_FAMAGE_VALUE		EQU 2
PROJ1_FAMAGE_VALUE		EQU 4
PROJ2_FAMAGE_VALUE		EQU 6
PROJ3_FAMAGE_VALUE		EQU $14

; == ENUMS ==

APU_MUS_ENABLE			EQU	1
APU_SND_ENABLE			EQU 2

; == BIOS FONT ==

_	EQU	$00
_A	EQU	$01
_B	EQU	$02
_C	EQU	$03
_D	EQU	$04
_E	EQU	$05
_F	EQU	$06
_G	EQU	$07
_H	EQU	$08
_I	EQU	$09
_J	EQU	$0A
_K	EQU	$0B
_L	EQU	$0C
_M	EQU	$0D
_N	EQU	$0E
_O	EQU	$0F
_P	EQU	$10
_Q	EQU	$11
_R	EQU	$12
_S	EQU	$13
_T	EQU	$14
_U	EQU	$15
_V	EQU	$16
_W	EQU	$17
_X	EQU	$18
_Y	EQU	$19
_Z	EQU	$1A
_1	EQU	$1B
_2	EQU	$1C
_3	EQU	$1D
_4	EQU	$1E
_5	EQU	$1F
_6	EQU	$20
_7	EQU	$21
_8	EQU	$22
_9	EQU	$23
_DT	EQU	$24
_DD	EQU	$25
_TM	EQU	$25
_CO	EQU	$26
_ASK	EQU	$F9
_END	EQU	$FA
_CLR	EQU	$FB
_DLY	EQU	$FC
_MUT	EQU	$FD
_SCL	EQU	$FE
_NXT	EQU	$FF

; == HUD FONTS ==

__	EQU	$0B
_a	EQU	$0C
_b	EQU	$0D
_c	EQU	$0E
_d	EQU	$0F
_e	EQU	$10
_f	EQU	$11
_g	EQU	$12
_h	EQU	$13
_i	EQU	$14
_j	EQU	$15
_k	EQU	$16
_l	EQU	$17
_m	EQU	$18
_n	EQU	$19
_o	EQU	$1A
_p	EQU	$1B
_q	EQU	$1C
_r	EQU	$1D
_s	EQU	$1E
_t	EQU	$1F
_u	EQU	$20
_v	EQU	$21
_w	EQU	$22
_x	EQU	$23
_y	EQU	$24
_z	EQU	$25
_excl	EQU	$26
_lq		EQU	$27
_rq		EQU	$28
_gt		EQU	$29
_dot	EQU $2A
_cur	EQU	$2B

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

	MAC	TPOW
{1}				EQU	.
{1}.head		EQU	.
		.BYTE 0
{1}.body		EQU .
		.BYTE 0
{1}.arms		EQU	.
		.BYTE 0
{1}.legs		EQU	.
		.BYTE 0
	ENDM

; == OPCODE ==

	MAC	INCY
		INY
		BNE		.1
		INC		{1}+1
.1:
	ENDM
	
	MAC	ADD16
		LDA		{1}
		CLC
		ADC		#[{2}]
		STA		{1}
		BCC		.1
		INC		{1}+1
.1:
	ENDM
	
	MAC	PRINTF
		LDA		#<{1}
		LDY		#>{1}
		JSR		_printf
	ENDM
	
	MAC	U8TOS
		STA		_str_ptr
		LDA		#0
		STA		_str_ptr+1
		LDX		#<[{1}]
		LDY		#>[{1}]
		JSR		_hex2str
	ENDM

	MAC	U16TOS
		LDA		[{1}]
		STA		_str_ptr
		LDA		[{1}+1]
		STA		_str_ptr+1
		LDX		#<[{2}]
		LDY		#>[{2}]
		JSR		_hex2str
	ENDM

; == DATA ==

	MAC	STRING
	.BYTE	[.1-.-1]
	.BYTE	{#}
.1	EQU	.
	ENDM

	MAC	SOUND
SND_{1}		EQU	[[.-_sound_lib]>>3]
	.BYTE	{2},{3},{4},{5},{6},{7},{8},{9}
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
		JMP		_RESET
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