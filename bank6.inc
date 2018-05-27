
		ORG $8000, $00
; --------------------------------
; SOUND ENGINE
; --------------------------------

; =============== S U B	R O U T	I N E =======================================
_apu_load_chnl:
		ASL
		STA		byte_140F,X
		LDA		#1
		STA		byte_1405,X
		LDA		#0
		STA		byte_140A,X
		STA		byte_1423,X
		CPX		#2
		BNE		loc_50024
		LDA		#1
		STA		byte_141E,X
		LDA		#0
		BEQ		loc_50027
loc_50024:
		STA		byte_141E,X
loc_50027:
		STA		byte_142D,X
		STA		byte_1455,X
		STA		byte_1419,X
		STA		byte_14A5,X
		STA		byte_14A0,X
		STA		byte_145A,X
		STA		byte_145F,X
		STA		byte_149B,X
		LDA		#1
		STA		byte_1400,X
		STA		byte_1414,X
		STA		byte_1437,X
		LDA		#$F
		STA		_APU_STATUS
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_play_chnl:
		LDA		byte_1400,X
		BEQ		locret_50065
		CPX		#0
		BEQ		loc_5008B
		CPX		#1
		BEQ		loc_5008B
		CPX		#2
		BEQ		loc_50082
		CPX		#3
		BEQ		loc_50066
locret_50065:
		RTS
loc_50066:
		TXA
		ASL
		ASL
		TAY
		LDA		#$10
		ORA		#$20
		ORA		byte_14A0,X
		STA		_APU_PULSE1_ENV,Y
		LDA		byte_144B,X
		STA		_APU_PULSE1_TIME,Y
		LDA		#$F8
		STA		_APU_PULSE1_LEN,Y
		JMP		loc_500B5
loc_50082:
		TXA
		ASL
		ASL
		TAY
		LDA		#$80
		JMP		loc_50096
loc_5008B:
		TXA
		ASL
		ASL
		TAY
		LDA		byte_14C8,X
		ORA		#$10
		ORA		#$20
loc_50096:
		ORA		byte_14A0,X
		STA		_APU_PULSE1_ENV,Y
		LDA		#8
		STA		_APU_PULSE1_SWEEP,Y
		LDA		byte_144B,X
		STA		_APU_PULSE1_TIME,Y
		LDA		byte_1450,X
		CMP		byte_149B,X
		BEQ		loc_500B5
		STA		_APU_PULSE1_LEN,Y
		STA		byte_149B,X
loc_500B5:
		LDA		#0
		STA		byte_1428,X
		DEC		byte_141E,X
		BPL		loc_500E0
		LDA		byte_1423,X
		EOR		#$FF
		STA		byte_1423,X
		BPL		loc_500D2
		LDY		byte_140F,X
		LDA		word_50580,Y
		JMP		loc_500D8
loc_500D2:
		LDY		byte_140F,X
		LDA		word_50580+1,Y
loc_500D8:
		STA		byte_141E,X
		LDA		#$FF
		STA		byte_1428,X
loc_500E0:
		LDA		byte_1428,X
		BPL		loc_500EA
		DEC		byte_1437,X
		BEQ		loc_500FA
loc_500EA:
		LDA		byte_1446,X
		CMP		#$FF
		BEQ		loc_500F4
		JSR		_apu_fetch
loc_500F4:
		LDA		#0
		STA		byte_142D,X
		RTS
loc_500FA:
		LDY		byte_1419,X
		LDA		_mus_lib0_lo,Y
		STA		_apu_tmp0
		LDA		_mus_lib0_hi,Y
		STA		_apu_tmp0+1
		LDA		#0
		STA		byte_147D,X
		LDY		byte_1405,X
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BEQ		loc_50118
		JMP		loc_5019D
loc_50118:
		DEC		byte_1414,X
		BEQ		loc_50128
		LDA		#0
		STA		byte_1405,X
		TAY
		LDA		(_apu_tmp0),Y
		JMP		loc_5019D
loc_50128:
		LDY		byte_140F,X
		LDA		_mus_lib1,Y
		STA		_apu_tmp0
		LDA		_mus_lib1+1,Y
		STA		_apu_tmp0+1
		LDY		byte_140A,X
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BNE		loc_5014A
		INY
		LDA		(_apu_tmp0),Y
		STA		byte_140A,X
		TAY
		LDA		(_apu_tmp0),Y
		JMP		loc_50158
loc_5014A:
		CMP		#$FE
		BNE		loc_50154
		LDA		#0
		STA		byte_1400,X
		RTS
loc_50154:
		CMP		#$FC
		BNE		loc_50158
loc_50158:
		CMP		#$80
		BCC		loc_50166
		AND		#$7F
		STA		byte_1455,X
		INY
		LDA		(_apu_tmp0),Y
		BMI		loc_5018E
loc_50166:
		CMP		#$70
		BCC		loc_50172
		AND		#$F
		STA		byte_145A,X
		INY
		LDA		(_apu_tmp0),Y
loc_50172:
		CMP		#$60
		BCC		loc_5017E
		AND		#$F
		STA		byte_1414,X
		INY
		LDA		(_apu_tmp0),Y
loc_5017E:
		CMP		#$50
		BCC		loc_5018A
		AND		#$F
		STA		byte_145F,X
		INY
		LDA		(_apu_tmp0),Y
loc_5018A:
		STA		byte_1419,X
		INY
loc_5018E:
		TYA
		STA		byte_140A,X
		INC		byte_1414,X
		LDA		#0
		STA		byte_1405,X
		JMP		loc_500FA
loc_5019D:
		CMP		#$FD
		BNE		loc_501BF
		INY
		LDA		(_apu_tmp0),Y
		STA		byte_147D,X
		INY
		LDA		(_apu_tmp0),Y
		CLC
		ADC		byte_1455,X
		STA		byte_1446,X
		INY
		LDA		(_apu_tmp0),Y
		CLC
		ADC		byte_1455,X
		STA		byte_148C,X
		INY
		JMP		loc_50222
loc_501BF:
		CMP		#$E0
		BCC		loc_501DA
		AND		#$1F
		STA		byte_143C,X
		LDA		#$FF
		STA		byte_1446,X
		LDA		#0
		STA		byte_14A0,X
		INY
		TYA
		STA		byte_1405,X
		JMP		loc_5027C
loc_501DA:
		CMP		#$C0
		BCC		loc_501F2
		CLC
		ADC		byte_145A,X
		ASL
		ASL
		STA		byte_1432,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$FD
		BEQ		loc_5019D
		CMP		#$C0
		BCS		loc_50222
loc_501F2:
		CMP		#$80
		BCC		loc_50206
		AND		#$7F
		STA		byte_143C,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$FD
		BEQ		loc_5019D
		CMP		#$80
		BCS		loc_50222
loc_50206:
		CMP		#$60
		BCC		loc_5021A
		AND		#$1F
		CLC
		ADC		byte_145F,X
		STA		byte_1469,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$60
		BCS		loc_50222
loc_5021A:
		CLC
		ADC		byte_1455,X
		STA		byte_1446,X
		INY
loc_50222:
		TYA
		STA		byte_1405,X
		INC		byte_142D,X
		LDY		byte_1432,X
		CPX		#0
		BEQ		loc_5023D
		CPX		#1
		BEQ		loc_5023D
		CPX		#2
		BEQ		loc_5024C
		CPX		#3
		BEQ		loc_5024C
		RTS
loc_5023D:
		LDA		_apu_instr_lib,Y
		AND		#8
		BNE		loc_5024C
		LDA		_apu_instr_lib,Y
		AND		#$C0
		STA		byte_14C8,X
loc_5024C:
		LDA		_apu_instr_lib+1,Y
		STA		byte_14AA,X
		LDA		_apu_instr_lib+2,Y
		STA		byte_14AF,X
		LDA		_apu_instr_lib+3,Y
		STA		byte_14B4,X
		CPX		#3
		BEQ		loc_50276
		LDY		byte_1446,X
		LDA		byte_5045E,Y
		STA		byte_144B,X
		LDA		#$F8
		ORA		byte_504B3,Y
		STA		byte_1450,X
		JMP		loc_5027C
loc_50276:
		LDA		byte_1446,X
		STA		byte_144B,X
loc_5027C:
		LDA		byte_143C,X
		STA		byte_1437,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_fetch:
		LDA		byte_142D,X
		BNE		loc_5028B
		JMP		loc_502E0
loc_5028B:
		LDA		byte_147D,X
		BEQ		loc_502B2
		TAY
		LSR
		LSR
		LSR
		LSR
		STA		byte_1487,X
		TYA
		AND		#$F
		STA		byte_147D,X
		LDA		byte_143C,X
		SEC
		SBC		byte_1487,X
		STA		byte_1487,X
		LDA		byte_1446,X
		SEC
		SBC		byte_148C,X
		STA		byte_1482,X
loc_502B2:
		LDA		#0
		STA		byte_14A5,X
		STA		byte_14C3,X
		STA		byte_1496,X
		STA		byte_1464,X
		LDA		byte_14AF,X
		LSR
		LSR
		LSR
		LSR
		STA		byte_1491,X
		LDA		byte_143C,X
		SEC
		SBC		byte_1491,X
		BCS		loc_502DA
		LDA		#0
		STA		byte_14AA,X
		BEQ		loc_502DD
loc_502DA:
		STA		byte_1491,X
loc_502DD:
		JMP		loc_503B4
loc_502E0:
		LDA		byte_14B4,X
		AND		#$7F
		TAY
		LDA		off_50680,Y
		STA		_apu_tmp0
		LDA		off_50698,Y
		STA		_apu_tmp0+1
		LDY		byte_14A5,X
		LDA		(_apu_tmp0),Y
		CMP		#$FE
		BEQ		loc_502FF
		STA		byte_14A0,X
		INC		byte_14A5,X
loc_502FF:
		LDY		byte_147D,X
		BEQ		loc_5035B
		LDA		byte_1487,X
		CMP		byte_1437,X
		BCC		loc_5035B
		LDY		byte_1482,X
		BMI		loc_5032B
		LDA		byte_144B,X
		CLC
		ADC		byte_147D,X
		STA		byte_144B,X
		BCC		loc_50320
		INC		byte_1450,X
loc_50320:
		LDY		byte_148C,X
		SEC
		SBC		byte_5045E,Y
		BCC		loc_50358
		BCS		loc_50343
loc_5032B:
		LDA		byte_144B,X
		SEC
		SBC		byte_147D,X
		STA		byte_144B,X
		BCS		loc_5033A
		DEC		byte_1450,X
loc_5033A:
		LDY		byte_148C,X
		SEC
		SBC		byte_5045E,Y
		BCS		loc_50358
loc_50343:
		TYA
		STA		byte_1446,X
		LDA		byte_5045E,Y
		STA		byte_144B,X
		LDA		byte_504B3,Y
		STA		byte_1450,X
		LDA		#0
		STA		byte_147D,X
loc_50358:
		JMP		loc_503B4
loc_5035B:
		LDA		byte_14AA,X
		AND		#$F
		BEQ		loc_503B4
		LDA		byte_1491,X
		CMP		byte_1437,X
		BCC		loc_503B4
		LDA		byte_14AA,X
		AND		#$F
		TAY
		LDA		off_506B0-1,Y
		STA		_apu_tmp0
		LDA		off_506B4-1,Y
		STA		_apu_tmp0+1
		LDY		byte_1496,X
		LDA		(_apu_tmp0),Y
		CMP		#$80
		BNE		loc_50385
		LDY		#0
loc_50385:
		LDA		(_apu_tmp0),Y
		BPL		loc_503A1
		EOR		#$FF
		CLC
		ADC		#1
		STA		_apu_tmp0
		LDA		byte_144B,X
		SEC
		SBC		_apu_tmp0
		STA		byte_144B,X
		BCS		loc_5039E
		DEC		byte_1450,X
loc_5039E:
		JMP		loc_503AF
loc_503A1:
		LDA		byte_144B,X
		CLC
		ADC		(_apu_tmp0),Y
		STA		byte_144B,X
		BCC		loc_503AF
		INC		byte_1450,X
loc_503AF:
		INY
		TYA
		STA		byte_1496,X
loc_503B4:
		CPX		#2
		BCS		loc_503E5
		LDA		byte_14AF,X
		AND		#$F
		BEQ		loc_503E5
		TAY
		LDA		off_506B8-1,Y
		STA		_apu_tmp0
		LDA		off_506BE-1,Y
		STA		_apu_tmp0+1
		LDY		byte_14C3,X
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BNE		loc_503D9
		INY
		LDA		(_apu_tmp0),Y
		TAY
		LDA		(_apu_tmp0),Y
loc_503D9:
		CMP		#$FE
		BEQ		loc_503E5
		STA		byte_14C8,X
		INY
		TYA
		STA		byte_14C3,X
loc_503E5:
		LDA		byte_14AA,X
		AND		#$40
		BEQ		locret_5045D
		LDY		byte_1469,X
		LDA		off_506C4,Y
		STA		_apu_tmp0
		LDA		off_506E6,Y
		STA		_apu_tmp0+1
		LDY		byte_1464,X
		BNE		loc_50407
		LDA		(_apu_tmp0),Y
		STA		byte_146E,X
		INY
		JMP		loc_5041C
loc_50407:
		LDA		byte_146E,X
		SEC
		SBC		#$10
		STA		byte_146E,X
		BPL		loc_50443
		LDY		#0
		LDA		(_apu_tmp0),Y
		STA		byte_146E,X
		LDY		byte_1464,X
loc_5041C:
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BNE		loc_5042C
		LDY		#0
		LDA		(_apu_tmp0),Y
		AND		#$F
		TAY
		INY
		LDA		(_apu_tmp0),Y
loc_5042C:
		CMP		#$FE
		BEQ		locret_5045D
		BMI		loc_50439
		CLC
		ADC		byte_1446,X
		STA		byte_1473,X
loc_50439:
		AND		#$7F
		STA		byte_1473,X
		INY
		TYA
		STA		byte_1464,X
loc_50443:
		LDA		byte_1473,X
		CPX		#3
		BNE		loc_50450
		STA		byte_144B,X
		JMP		locret_5045D
loc_50450:
		TAY
		LDA		byte_5045E,Y
		STA		byte_144B,X
		LDA		byte_504B3,Y
		STA		byte_1450,X
locret_5045D:
		RTS
byte_5045E:
		.BYTE	$AD,$4A,$EF,$9B,$4B,$FE,$B5,$73,$30,$F5,$BD,$87,$56,$25,$F8,$CD
		.BYTE	$A5,$7F,$5B,$3A,$19,$FB,$DE,$C3,$AA,$92,$7B,$66,$52,$3F,$2D,$1C
		.BYTE	 $C,$FD,$EF,$E1,$D4,$C9,$BD,$B3,$A9,$9F,$96,$8E,$86,$7E,$77,$70
		.BYTE	$6A,$64,$5E,$59,$54,$4F,$4A,$47,$42,$3E,$3B,$38,$35,$31,$2E,$2C
		.BYTE	$29,$27,$25,$23,$21,$1F,$1D,$1B,$1A,$19,$18,$16,$15,$14,$13,$12
		.BYTE	$11,$10, $F, $E,$FF
byte_504B3:
		.BYTE	  6,  6,  5,  5,  5,  4,  4,  4,  4,  3,  3,  3,  3,  3,  2,  2
		.BYTE	  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
		.BYTE	  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  7
_mus_lib1:
		.WORD _mus00_lib1
		.WORD _mus01_lib1
		.WORD _mus02_lib1
		.WORD _mus03_lib1
		.WORD _mus04_lib1
		.WORD _mus05_lib1
		.WORD _mus06_lib1
		.WORD _mus07_lib1
		.WORD _mus08_lib1
		.WORD _mus09_lib1
		.WORD _mus0A_lib1
		.WORD _mus0B_lib1
		.WORD _mus0C_lib1
		.WORD _mus0D_lib1
		.WORD _mus0E_lib1
		.WORD _mus0F_lib1
		.WORD _mus10_lib1
		.WORD _mus11_lib1
		.WORD _mus12_lib1
		.WORD _mus13_lib1
		.WORD _mus14_lib1
		.WORD _mus15_lib1
		.WORD _mus16_lib1
		.WORD _mus17_lib1
		.WORD _mus18_lib1
		.WORD _mus19_lib1
		.WORD _mus1A_lib1
		.WORD _mus1B_lib1
		.WORD _mus1C_lib1
		.WORD _mus1D_lib1
		.WORD _mus1E_lib1
		.WORD _mus1F_lib1
		.WORD _mus20_lib1
		.WORD _mus21_lib1
		.WORD _mus22_lib1
		.WORD _mus23_lib1
		.WORD _mus24_lib1
		.WORD _mus25_lib1
		.WORD _mus26_lib1
		.WORD _mus27_lib1
		.WORD _mus28_lib1
		.WORD _mus29_lib1
		.WORD _mus2A_lib1
		.WORD _mus2B_lib1
		.WORD _mus2C_lib1
		.WORD _mus2D_lib1
		.WORD _mus2E_lib1
		.WORD _mus2F_lib1
		.WORD _mus30_lib1
		.WORD _mus31_lib1
		.WORD _mus32_lib1
		.WORD _mus33_lib1
		.WORD _mus34_lib1
		.WORD _mus35_lib1
		.WORD _mus36_lib1
		.WORD _mus37_lib1
		.WORD _mus38_lib1
		.WORD _mus39_lib1
		.WORD _mus3A_lib1
		.WORD _mus3B_lib1
word_50580:
		.WORD      0, $202, $202, $202, $202, $202, $202, $202
		.WORD   $202, $202, $202, $202, $202, $202, $202, $202
		.WORD   $202,$7F7F, $202, $303, $404, $203, $203, $203
		.WORD   $203, $303, $303, $303, $303, $303, $203, $203
		.WORD   $203, $203, $203, $203, $203, $203, $202, $202
		.WORD   $404, $202, $202, $202, $202, $202, $202, $202
		.WORD   $202, $202, $202, $202,    0,    0, $101, $303
		.WORD   $303, $303, $303, $202
_mus_lib0_lo:
		.BYTE	<[_mus00_lib0]
		.BYTE	<[_mus01_lib0]
		.BYTE	<[_mus02_lib0]
		.BYTE	<[_mus03_lib0]
		.BYTE	<[_mus04_lib0]
		.BYTE	<[_mus05_lib0]
		.BYTE	<[_mus06_lib0]
		.BYTE	<[_mus07_lib0]
		.BYTE	<[_mus08_lib0]
		.BYTE	<[_mus09_lib0]
		.BYTE	<[_mus0A_lib0]
		.BYTE	<[_mus0B_lib0]
		.BYTE	<[_mus0C_lib0]
		.BYTE	<[_mus0D_lib0]
		.BYTE	<[_mus0E_lib0]
		.BYTE	<[_mus0F_lib0]
		.BYTE	<[_mus10_lib0]
		.BYTE	<[_mus11_lib0]
		.BYTE	<[_mus12_lib0]
		.BYTE	<[_mus13_lib0]
		.BYTE	<[_mus14_lib0]
		.BYTE	<[_mus15_lib0]
		.BYTE	<[_mus16_lib0]
		.BYTE	<[_mus17_lib0]
		.BYTE	<[_mus18_lib0]
		.BYTE	<[_mus19_lib0]
		.BYTE	<[_mus1A_lib0]
		.BYTE	<[_mus1B_lib0]
		.BYTE	<[_mus1C_lib0]
		.BYTE	<[_mus1D_lib0]
		.BYTE	<[_mus1E_lib0]
		.BYTE	<[_mus1F_lib0]
		.BYTE	<[_mus20_lib0]
		.BYTE	<[_mus21_lib0]
		.BYTE	<[_mus22_lib0]
		.BYTE	<[_mus23_lib0]
		.BYTE	<[_mus24_lib0]
		.BYTE	<[_mus25_lib0]
		.BYTE	<[_mus26_lib0]
		.BYTE	<[_mus27_lib0]
		.BYTE	<[_mus28_lib0]
		.BYTE	<[_mus29_lib0]
		.BYTE	<[_mus2A_lib0]
		.BYTE	<[_mus2B_lib0]
		.BYTE	<[_mus2C_lib0]
		.BYTE	<[_mus2D_lib0]
		.BYTE	<[_mus2E_lib0]
		.BYTE	<[_mus2F_lib0]
		.BYTE	<[_mus30_lib0]
		.BYTE	<[_mus31_lib0]
		.BYTE	<[_mus32_lib0]
		.BYTE	<[_mus33_lib0]
		.BYTE	<[_mus34_lib0]
		.BYTE	<[_mus35_lib0]
		.BYTE	<[_mus36_lib0]
		.BYTE	<[_mus37_lib0]
		.BYTE	<[_mus38_lib0]
		.BYTE	<[_mus39_lib0]
		.BYTE	<[_mus3A_lib0]
		.BYTE	<[_mus3B_lib0]
		.BYTE	<[_mus3C_lib0]
		.BYTE	<[_mus3D_lib0]
		.BYTE	<[_mus3E_lib0]
		.BYTE	<[_mus3F_lib0]
		.BYTE	<[_mus40_lib0]
		.BYTE	<[_mus41_lib0]
		.BYTE	<[_mus42_lib0]
		.BYTE	<[_mus43_lib0]
_mus_lib0_hi:
		.BYTE	>[_mus00_lib0]
		.BYTE	>[_mus01_lib0]
		.BYTE	>[_mus02_lib0]
		.BYTE	>[_mus03_lib0]
		.BYTE	>[_mus04_lib0]
		.BYTE	>[_mus05_lib0]
		.BYTE	>[_mus06_lib0]
		.BYTE	>[_mus07_lib0]
		.BYTE	>[_mus08_lib0]
		.BYTE	>[_mus09_lib0]
		.BYTE	>[_mus0A_lib0]
		.BYTE	>[_mus0B_lib0]
		.BYTE	>[_mus0C_lib0]
		.BYTE	>[_mus0D_lib0]
		.BYTE	>[_mus0E_lib0]
		.BYTE	>[_mus0F_lib0]
		.BYTE	>[_mus10_lib0]
		.BYTE	>[_mus11_lib0]
		.BYTE	>[_mus12_lib0]
		.BYTE	>[_mus13_lib0]
		.BYTE	>[_mus14_lib0]
		.BYTE	>[_mus15_lib0]
		.BYTE	>[_mus16_lib0]
		.BYTE	>[_mus17_lib0]
		.BYTE	>[_mus18_lib0]
		.BYTE	>[_mus19_lib0]
		.BYTE	>[_mus1A_lib0]
		.BYTE	>[_mus1B_lib0]
		.BYTE	>[_mus1C_lib0]
		.BYTE	>[_mus1D_lib0]
		.BYTE	>[_mus1E_lib0]
		.BYTE	>[_mus1F_lib0]
		.BYTE	>[_mus20_lib0]
		.BYTE	>[_mus21_lib0]
		.BYTE	>[_mus22_lib0]
		.BYTE	>[_mus23_lib0]
		.BYTE	>[_mus24_lib0]
		.BYTE	>[_mus25_lib0]
		.BYTE	>[_mus26_lib0]
		.BYTE	>[_mus27_lib0]
		.BYTE	>[_mus28_lib0]
		.BYTE	>[_mus29_lib0]
		.BYTE	>[_mus2A_lib0]
		.BYTE	>[_mus2B_lib0]
		.BYTE	>[_mus2C_lib0]
		.BYTE	>[_mus2D_lib0]
		.BYTE	>[_mus2E_lib0]
		.BYTE	>[_mus2F_lib0]
		.BYTE	>[_mus30_lib0]
		.BYTE	>[_mus31_lib0]
		.BYTE	>[_mus32_lib0]
		.BYTE	>[_mus33_lib0]
		.BYTE	>[_mus34_lib0]
		.BYTE	>[_mus35_lib0]
		.BYTE	>[_mus36_lib0]
		.BYTE	>[_mus37_lib0]
		.BYTE	>[_mus38_lib0]
		.BYTE	>[_mus39_lib0]
		.BYTE	>[_mus3A_lib0]
		.BYTE	>[_mus3B_lib0]
		.BYTE	>[_mus3C_lib0]
		.BYTE	>[_mus3D_lib0]
		.BYTE	>[_mus3E_lib0]
		.BYTE	>[_mus3F_lib0]
		.BYTE	>[_mus40_lib0]
		.BYTE	>[_mus41_lib0]
		.BYTE	>[_mus42_lib0]
		.BYTE	>[_mus43_lib0]
off_50680:
		.BYTE	<[byte_50708]
		.BYTE	<[byte_5071E]
		.BYTE	<[byte_50740]
		.BYTE	<[byte_50774]
		.BYTE	<[byte_507B0]
		.BYTE	<[byte_507CD]
		.BYTE	<[byte_507F0]
		.BYTE	<[byte_50814]
		.BYTE	<[byte_50816]
		.BYTE	<[byte_50824]
		.BYTE	<[byte_50835]
		.BYTE	<[byte_5083B]
		.BYTE	<[byte_5083D]
		.BYTE	<[byte_50846]
		.BYTE	<[byte_50854]
		.BYTE	<[byte_5085B]
		.BYTE	<[byte_50861]
		.BYTE	<[byte_50863]
		.BYTE	<[byte_50865]
		.BYTE	<[byte_5086E]
		.BYTE	<[byte_50874]
		.BYTE	<[byte_50876]
		.BYTE	<[byte_50884]
		.BYTE	<[byte_50895]
off_50698:
		.BYTE	>[byte_50708]
		.BYTE	>[byte_5071E]
		.BYTE	>[byte_50740]
		.BYTE	>[byte_50774]
		.BYTE	>[byte_507B0]
		.BYTE	>[byte_507CD]
		.BYTE	>[byte_507F0]
		.BYTE	>[byte_50814]
		.BYTE	>[byte_50816]
		.BYTE	>[byte_50824]
		.BYTE	>[byte_50835]
		.BYTE	>[byte_5083B]
		.BYTE	>[byte_5083D]
		.BYTE	>[byte_50846]
		.BYTE	>[byte_50854]
		.BYTE	>[byte_5085B]
		.BYTE	>[byte_50861]
		.BYTE	>[byte_50863]
		.BYTE	>[byte_50865]
		.BYTE	>[byte_5086E]
		.BYTE	>[byte_50874]
		.BYTE	>[byte_50876]
		.BYTE	>[byte_50884]
		.BYTE	>[byte_50895]
off_506B0:
		.BYTE	<[byte_50897]
		.BYTE	<[byte_508A4]
		.BYTE	<[byte_508B1]
		.BYTE	<[byte_508BE]
off_506B4:
		.BYTE	>[byte_50897]
		.BYTE	>[byte_508A4]
		.BYTE	>[byte_508B1]
		.BYTE	>[byte_508BE]
off_506B8:
		.BYTE	<[byte_508C3]
		.BYTE	<[byte_508E9]
		.BYTE	<[byte_50909]
		.BYTE	<[byte_50935]
		.BYTE	<[byte_5094F]
		.BYTE	<[byte_50954]
off_506BE:
		.BYTE	>[byte_508C3]
		.BYTE	>[byte_508E9]
		.BYTE	>[byte_50909]
		.BYTE	>[byte_50935]
		.BYTE	>[byte_5094F]
		.BYTE	>[byte_50954]
off_506C4:
		.BYTE	<[byte_50962]
		.BYTE	<[byte_50977]
		.BYTE	<[byte_5097F]
		.BYTE	<[byte_5098D]
		.BYTE	<[byte_509B7]
		.BYTE	<[byte_509EC]
		.BYTE	<[byte_509FB]
		.BYTE	<[byte_50A09]
		.BYTE	<[byte_50A11]
		.BYTE	<[byte_50A19]
		.BYTE	<[byte_50A23]
		.BYTE	<[byte_50A33]
		.BYTE	<[byte_50A3A]
		.BYTE	<[byte_50A41]
		.BYTE	<[byte_50A44]
		.BYTE	<[byte_50A48]
		.BYTE	<[byte_50A53]
		.BYTE	<[byte_50A5E]
		.BYTE	<[byte_50A69]
		.BYTE	<[byte_50A74]
		.BYTE	<[byte_50A7F]
		.BYTE	<[byte_50A8A]
		.BYTE	<[byte_50A95]
		.BYTE	<[byte_50AA3]
		.BYTE	<[byte_50AA7]
		.BYTE	<[byte_50AAB]
		.BYTE	<[byte_50AAF]
		.BYTE	<[byte_50AB3]
		.BYTE	<[byte_50AB7]
		.BYTE	<[byte_50ABB]
		.BYTE	<[byte_50AC6]
		.BYTE	<[byte_50AD1]
		.BYTE	<[byte_50ADC]
		.BYTE	<[byte_50AE2]
off_506E6:
		.BYTE	>[byte_50962]
		.BYTE	>[byte_50977]
		.BYTE	>[byte_5097F]
		.BYTE	>[byte_5098D]
		.BYTE	>[byte_509B7]
		.BYTE	>[byte_509EC]
		.BYTE	>[byte_509FB]
		.BYTE	>[byte_50A09]
		.BYTE	>[byte_50A11]
		.BYTE	>[byte_50A19]
		.BYTE	>[byte_50A23]
		.BYTE	>[byte_50A33]
		.BYTE	>[byte_50A3A]
		.BYTE	>[byte_50A41]
		.BYTE	>[byte_50A44]
		.BYTE	>[byte_50A48]
		.BYTE	>[byte_50A53]
		.BYTE	>[byte_50A5E]
		.BYTE	>[byte_50A69]
		.BYTE	>[byte_50A74]
		.BYTE	>[byte_50A7F]
		.BYTE	>[byte_50A8A]
		.BYTE	>[byte_50A95]
		.BYTE	>[byte_50AA3]
		.BYTE	>[byte_50AA7]
		.BYTE	>[byte_50AAB]
		.BYTE	>[byte_50AAF]
		.BYTE	>[byte_50AB3]
		.BYTE	>[byte_50AB7]
		.BYTE	>[byte_50ABB]
		.BYTE	>[byte_50AC6]
		.BYTE	>[byte_50AD1]
		.BYTE	>[byte_50ADC]
		.BYTE	>[byte_50AE2]
byte_50708:
		.BYTE	 $A,  9,  8,  7,  7,  6,  6,  6,  5,  5,  5,  4,  4,  4,  3,  3
		.BYTE	  2,  2,  1,  1,  0,$FE
byte_5071E:
		.BYTE	  1,  2,  3,  4,  5,  6,  6,  7,  7,  8,  8,  9,  9,  9,  9,  9
		.BYTE	  9,  8,  8,  8,  7,  7,  7,  6,  6,  6,  5,  5,  5,  4,  4,  4
		.BYTE	  3,$FE
byte_50740:
		.BYTE	  1,  2,  3,  4,  5,  6,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  7,  7,  7,  7,  6,  6,  6,  5,  5,  4,  4,  3,  3
		.BYTE	  2,  2,  1,$FE
byte_50774:
		.BYTE	 $F, $E, $D, $D, $C, $C, $C, $B, $B, $B, $A, $A, $A, $A,  9,  9
		.BYTE	  9,  9,  9,  8,  8,  8,  8,  8,  7,  7,  7,  7,  7,  7,  7,  7
		.BYTE	  6,  6,  6,  6,  6,  6,  6,  6,  5,  5,  5,  5,  5,  5,  5,  5
		.BYTE	  4,  4,  4,  4,  3,  3,  3,  2,  2,  2,  1,$FE
byte_507B0:
		.BYTE	 $A,  9,  8,  8,  7,  7,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4
		.BYTE	  4,  4,  3,  3,  3,  3,  3,  2,  2,  2,  2,  1,$FE
byte_507CD:
		.BYTE	 $C, $B, $A, $A,  9,  9,  8,  8,  8,  7,  7,  7,  7,  6,  6,  6
		.BYTE	  6,  5,  5,  5,  5,  4,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2
		.BYTE	  2,  1,$FE
byte_507F0:
		.BYTE	  7,  7,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  4,  4,  4
		.BYTE	  4,  4,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2
		.BYTE	  2,  2,  1,$FE
byte_50814:
		.BYTE	  4,$FE
byte_50816:
		.BYTE	  8,  7,  6,  5,  4,  4,  3,  3,  3,  2,  2,  2,  1,$FE
byte_50824:
		.BYTE	 $F, $A,  9,  8,  7,  6,  5,  5,  4,  4,  4,  3,  3,  3,  3,  2
		.BYTE	$FE
byte_50835:
		.BYTE	  6,  3,  2,  2,  1,$FE
byte_5083B:
		.BYTE	  1,$FE
byte_5083D:
		.BYTE	 $A,  6,  5,  4,  4,  3,  3,  2,$FE
byte_50846:
		.BYTE	 $F, $C, $B, $A,  9,  8,  8,  7,  6,  5,  4,  3,  2,$FE
byte_50854:
		.BYTE	 $F, $C,  9,  7,  6,  5,$FE
byte_5085B:
		.BYTE	  8,  6,  5,  4,  3,$FE
byte_50861:
		.BYTE	  2,$FE
byte_50863:
		.BYTE	  1,$FE
byte_50865:
		.BYTE	 $A,  6,  4,  4,  3,  3,  3,  2,$FE
byte_5086E:
		.BYTE	  5,  4,  3,  3,  2,$FE
byte_50874:
		.BYTE	  4,$FE
byte_50876:
		.BYTE	  1,  2,  3,  4,  4,  5,  5,  5,  4,  3,  2,  2,  1,$FE
byte_50884:
		.BYTE	  1,  3,  5,  7,  9, $B, $D, $F, $D, $B,  9,  7,  5,  3,  1,  0
		.BYTE	$FE
byte_50895:
		.BYTE	  8,$FE
byte_50897:
		.BYTE	  1,  1,  0,$FF,$FF,  0,$FF,$FF,  0,  1,  1,  0,$80
byte_508A4:
		.BYTE	  4,  4,  4,$FC,$FC,$FC,$FC,$FC,$FC,  4,  4,  4,$80
byte_508B1:
		.BYTE	  2,  1,  1,$FE,$FF,$FF,$FE,$FF,$FF,  2,  1,  1,$80
byte_508BE:
		.BYTE	  1,  0,$FF,  0,$80
byte_508C3:
		.BYTE	  0,  0,  0,  0,  0,  0,$40,$40,$40,$40,$40,$40,$80,$80,$80,$80
		.BYTE	$80,$80,$C0,$C0,$C0,$C0,$C0,$C0,$80,$80,$80,$80,$80,$80,$40,$40
		.BYTE	$40,$40,$40,$40,$FF,  0
byte_508E9:
		.BYTE	  0,$40,$80,$80,$C0,$C0,$80,$80,$80,$80,$80,$80,$40,$40,$40,$40
		.BYTE	$40,$40,$80,$80,$80,$80,$80,$80,$C0,$C0,$C0,$C0,$C0,$C0,$FF,  6
byte_50909:
		.BYTE	  0,  0,$40,$40,$40,$40,$80,$80,$80,$80,$80,$80,$80,$80,$C0,$C0
		.BYTE	$C0,$C0,$C0,$C0,$C0,$80,$80,$80,$80,$80,$80,$80,$40,$40,$40,$40
		.BYTE	$40,$40,$40,$80,$80,$80,$80,$80,$80,$80,$FF, $E
byte_50935:
		.BYTE	  0,  0,$40,$40,$40,$40,$80,$80,$80,$80,$C0,$C0,$C0,$C0,$80,$80
		.BYTE	$80,$80,$40,$40,$40,$40,  0,  0,$FF,  0
byte_5094F:
		.BYTE	  0,  0,  0,$40,$FE
byte_50954:
		.BYTE	$80,$80,$80,$40,$40,$40,  0,  0,  0,$40,$40,$40,$80,$FE
byte_50962:
		.BYTE	 $C,$18,$16,$14,$12,$10, $E, $C, $B, $A,  9,  8,  7,  6,  5,  4
		.BYTE	  3,  2,  1,  0,$FF
byte_50977:
		.BYTE	$10,  4,  6,  7,  6,  5,  4,$FE
byte_5097F:
		.BYTE	$10, $F, $E, $D, $C, $B, $A,  9,  8,  7,  6,  6,  5,$FE
byte_5098D:
		.BYTE	$10, $F, $E, $D, $C, $B, $A,  9,  8,  7,  6,  7,  6,  7,  6,  7
		.BYTE	  6,  7,  6,  7,  6,  7,  8,  9,  9, $A, $A, $B, $B, $A, $A,  9
		.BYTE	  8,  7,  6,  5,  4,  3,  2,  1,  0,$FE
byte_509B7:
		.BYTE	  0,$12, $F, $E,$11, $E, $D,$10, $D, $C, $F, $C, $B, $E, $B, $A
		.BYTE	 $D, $A,  9, $C,  9,  8, $B,  8,  7, $A,  7,  6,  9,  6,  5,  8
		.BYTE	  5,  4,  7,  4,  3,  6,  3,  2,  5,  2,  1,  4,  1,  0,  3,  1
		.BYTE	  0,  2,  1,  0,$FE
byte_509EC:
		.BYTE	  0,  8,  9, $A, $B, $C, $D, $E, $F, $F, $E, $E, $E, $D,$FE
byte_509FB:
		.BYTE	  0,  2,  0,  1,  2,  0,  1,  0,  2,  1,  2,  0,  1,$FF
byte_50A09:
		.BYTE	$20,  0,  3,  7, $C,  7,  5,$FF
byte_50A11:
		.BYTE	$30,  0,  2,  5,  7, $A, $C,$FF
byte_50A19:
		.BYTE	  0,  8,  0,  6,  2,  9,  4,  7,  1,$FF
byte_50A23:
		.BYTE	  7,$18,$15,$12, $F, $C,  9,  7,  6,  5,  4,  3,  2,  1,  0,$FF
byte_50A33:
		.BYTE	  0,$9A,$98,$96,$94,  0,$FE
byte_50A3A:
		.BYTE	  0,$A2,$A0,$9E,$9C,  0,$FE
byte_50A41:
		.BYTE	  0,  0,$FE
byte_50A44:
		.BYTE	  0,  1,  0,$FF
byte_50A48:
		.BYTE	  3,  0,  2,  7,  0,  0,  2,  2,  7,  7,$FF
byte_50A53:
		.BYTE	  3,  0,  3,  7,  0,  0,  3,  3,  7,  7,$FF
byte_50A5E:
		.BYTE	  3,  0,  4,  7,  0,  0,  4,  4,  7,  7,$FF
byte_50A69:
		.BYTE	  3,  0,  5,  8,  0,  0,  5,  5,  8,  8,$FF
byte_50A74:
		.BYTE	  3,  0,  5,  9,  0,  0,  5,  5,  9,  9,$FF
byte_50A7F:
		.BYTE	  3,  0,  3,  8,  0,  0,  3,  3,  8,  8,$FF
byte_50A8A:
		.BYTE	  3,  0,  4,  9,  0,  0,  4,  4,  9,  9,$FF
byte_50A95:
		.BYTE	  4,  0,  2,  7, $C,  0,  0,  2,  2,  7,  7, $C, $C,$FF
byte_50AA3:
		.BYTE	$20,  0,  5,$FF
byte_50AA7:
		.BYTE	$20,  0,  7,$FF
byte_50AAB:
		.BYTE	$20,  0,  9,$FF
byte_50AAF:
		.BYTE	$20,  0,  8,$FF
byte_50AB3:
		.BYTE	$20,  0,  4,$FF
byte_50AB7:
		.BYTE	$20,  0,  3,$FF
byte_50ABB:
		.BYTE	  3,  0,  2,  8,  0,  0,  2,  2,  8,  8,$FF
byte_50AC6:
		.BYTE	  3,  0,  3,  6,  0,  0,  3,  3,  6,  6,$FF
byte_50AD1:
		.BYTE	  3,  0,  5,  7,  0,  0,  5,  5,  7,  7,$FF
byte_50ADC:
		.BYTE	$10,  0,  5,  3,  8,$FF
byte_50AE2:
		.BYTE	$10,  0,  7,  5,  9,$FF
_apu_instr_lib:
		.BYTE	$80,$40,  0,  0
		.BYTE	$80,$40,  0,  3
		.BYTE	$80,$40,  0,  2
		.BYTE	$80,$40,  0,  3
		.BYTE	$80,$40,  0,  4
		.BYTE	$80,$40,  0,  5
		.BYTE	  0,$40,  1,  6
		.BYTE	  0,  0,  0,  7
		.BYTE	  0,$40,  0,  8
		.BYTE	  0,$40,  1,  9
		.BYTE	  0,$40,  2, $A
		.BYTE	  0,$40,  0, $B
		.BYTE	  0,  0,  0, $C
		.BYTE	  0,$40,  0, $D
		.BYTE	  0,  1,$43, $E
		.BYTE	  0,  1,$43, $F
		.BYTE	  0,$40,  3, $E
		.BYTE	  0,$40,  3, $F
		.BYTE	  0,$40,  0,$10
		.BYTE	  0,$40,  0, $B
		.BYTE	$80,  1,$60,$10
		.BYTE	$80,  1,$50,$11
		.BYTE	$C0,  1,$60,$10
		.BYTE	$C0,  1,$50,$11
		.BYTE	  0,$42,  0, $B
		.BYTE	  0,$40,  4,$12
		.BYTE	  0,  0,  5,$10
		.BYTE	  0,  0,  5,$12
		.BYTE	  0,  3,$15,$12
		.BYTE	$80,  1,$60,$13
		.BYTE	$C0,  1,$60,$14
		.BYTE	$C0,  1,$50,$10
		.BYTE	  0,$42,$20, $B
		.BYTE	  0,  0,  0,$15
		.BYTE	$80,  4,  6,$16
		.BYTE	$80,  0,  0,$17
_mus01_lib1:
		.BYTE	  1,$FE
_mus02_lib1:
		.BYTE	  2,$FE
_mus26_lib1:
		.BYTE	$85,  1,$FE
_mus27_lib1:
		.BYTE	$85,  2,$FE
_mus01_lib0:
		.BYTE	$C0,$8C,$60,  0,$FF
_mus02_lib0:
		.BYTE	$C0,$8C,$61,  0,$FF
_mus03_lib1:
		.BYTE	  3,$FE
_mus03_lib0:
		.BYTE	$C1,$98,$62,  0,$E1,$FF
_mus04_lib1:
		.BYTE	  4,$FE
_mus04_lib0:
		.BYTE	$C2,$98,$63,  0,$E1,$FF
_mus05_lib1:
		.BYTE	  5,$FE
_mus06_lib1:
		.BYTE	  6,$FE
_mus05_lib0:
		.BYTE	$C3,$8E,$64,  8,$E1,$FF
_mus06_lib0:
		.BYTE	$C3,$97,$65,  0,$E1,$FF
_mus07_lib1:
		.BYTE	  7,$FE
_mus08_lib1:
		.BYTE	  8,$FE
_mus07_lib0:
		.BYTE	$C4,$86,$64,  4,$E1,$FF
_mus08_lib0:
		.BYTE	$C4,$8C,$66, $B,$E1,$FF
_mus09_lib1:
		.BYTE	  9,$FE
_mus0A_lib1:
		.BYTE	 $A,$FE
_mus09_lib0:
		.BYTE	$C5,$89,$64,  5,$E1,$FF
_mus0A_lib0:
		.BYTE	$C5,$90,$66, $C,$E1,$FF
_mus0B_lib1:
		.BYTE	 $B,$FE
_mus0C_lib1:
		.BYTE	 $C,$FE
_mus0B_lib0:
		.BYTE	$C3,$8C,$64,  6,$E1,$FF
_mus0C_lib0:
		.BYTE	$C3,$97,$66, $D,$E1,$FF
_mus0D_lib1:
		.BYTE	 $D,$FE
_mus0E_lib1:
		.BYTE	 $E,$FE
_mus0D_lib0:
		.BYTE	$C6,$90,$67,$18,$E1,$FF
_mus0E_lib0:
		.BYTE	$E1,$C6,$90,$67,$18,$E1,$FF
_mus0F_lib1:
		.BYTE	 $F,$FE
_mus10_lib1:
		.BYTE	$10,$FE
_mus0F_lib0:
		.BYTE	$C6,$90,$68,$1C,$E1,$FF
_mus10_lib0:
		.BYTE	$E1,$C6,$90,$68,$1C,$E1,$FF
_mus11_lib1:
		.BYTE	$11,$FE
_mus11_lib0:
		.BYTE	$C7,$BF,  9,$FF
_mus12_lib1:
		.BYTE	$12,$FE
_mus13_lib1:
		.BYTE	$13,$FE
_mus14_lib1:
		.BYTE	$14,$FE
_mus28_lib1:
		.BYTE	$F7,$13,$FE
_mus29_lib1:
		.BYTE	$81,  8,$FE
_mus12_lib0:
		.BYTE	$C8,$85,$69,$4C,$E1,$FF
_mus13_lib0:
		.BYTE	$C3,$83,$6A,  8,$E1,$FF
_mus14_lib0:
		.BYTE	$C8,$84,$66,  8,$E1,$FF
_mus15_lib1:
		.BYTE	$85,$22,$25,$85,$19,$83,$81,$83,$85,$83,$81,$83,$85,$83,$81,$83
		.BYTE	$85,$83,$81,$83,$85,$83,$81,$83,$85,$83,$81,$80,$1F,$85,$19,$83
		.BYTE	$81,$83,$85,$83,$81,$83,$85,$83,$81,$83,$85,$83,$81,$83,$85,$83
		.BYTE	$81,$80,$1F,$85,$19,$83,$81,$80,$1F,$85,$19,$83,$81,$83,$85,$83
		.BYTE	$81,$83,$FF,  3
_mus16_lib1:
		.BYTE	$85,$71,$23,$22,$91,$19,$8F,$8D,$8F,$91,$8F,$8D,$8F,$91,$8F,$8D
		.BYTE	$8F,$91,$8F,$8D,$8F,$91,$70,$1A,$1D,$91,$19,$8F,$8D,$8F,$91,$8F
		.BYTE	$8D,$8F,$91,$20,$21,$26,$26,$91,$67,$19,$FF,  4
_mus17_lib1:
		.BYTE	$24,$69,$18,$27,$27,$6F,$16,$16,$16,$1B,$1B,$1C,$1C,$1B,$1B,$16
		.BYTE	$16,$1B,$1B,$1C,$1C,$1E,$1E,$6F,$16,$16,$16,$1B,$1B,$1C,$1C,$1B
		.BYTE	$1B,$16,$16,$1B,$1B,$1C,$1C,$1B,$1B,$16,$16,$1B,$1B,$1C,$1C,$1E
		.BYTE	$1E,$16,$16,$1B,$1B,$1C,$1C,$1E,$1E,$67,$16,$67,$16,$FF,  3
_mus18_lib1:
		.BYTE	$24,$69,$18,$28,$28,$29,$6E,$17,$6F,$17,$29,$6E,$17,$6F,$17,$6F
		.BYTE	$17,$29,$66,$17,$29,$66,$17,$FF,  3
_mus16_lib0:
		.BYTE	$CB,$84,$6B,  5,$6D,$11,$82,$6C,  5,$84,$6B, $F,$82,  5,$84,$11
		.BYTE	$D8,$11,$CB,$82,$6C,  5,$84,$6B, $F,$82,$11,$FF
_mus29_lib0:
		.BYTE	$C5,$88,$6E,  4,$CD,$84,  5,$CC,  2,  4,  2,$CD,  5,$CC,  2,$FF
_mus17_lib0:
		.BYTE	$CC,$84,  4,  2,$CD,$84,$6E,  5,$CC,  2,  4,  2,$CD,  5,$CC,  2
		.BYTE	$FF
_mus18_lib0:
		.BYTE	$F0,$F0,$FF
_mus19_lib0:
		.BYTE	$C9,$82,$76,$18,$18,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84,$CA
		.BYTE	$C9,$C9,$82,$C9,$C9,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84,$CA
		.BYTE	$C9,$FF
_mus1A_lib0:
		.BYTE	$CE,$88,$FD,$4F,$1B,$18,$82,$18,$CF,$CE,$88,$FD,$1F,$16,$18,$CF
		.BYTE	$84,$CE,$82,$13,$CF,$18,$CE,$88,$16,$84,$18,$CF,$84,$16,$13, $C
		.BYTE	$CE,$82,$13,$CF,$CE,$84,$16,$82,$18,$CF,$16,$CE,$88,$FD,$4F,$1D
		.BYTE	$1B,$84,$CF,$1D,$CE,$88,$FD,$1F,$18,$1A,$82,$1B,$CF,$1A,$CE,$98
		.BYTE	$FD,$1F,$18,$1A,$82,$13,$CF,$1A,$CE,$84,$16,$82,$18,$CF,$16,$CE
		.BYTE	$88,$FD,$4F,$1B,$18,$82,$16,$CF,$18,$CE,$88,$FD,$1F,$16,$18,$82
		.BYTE	$13,$CF,$18,$CE,$84,$16,$82,$18,$CF,$16,$CE,$88,$FD,$1F,$1D,$1F
		.BYTE	$82,$22,$CF,$1F,$CE,$88,$FD,$1F,$1E,$1D,$82,$1B,$CF,$1D,$CE,$84
		.BYTE	$FD,$1F,$1B,$1D,$82,$1B,$CF,$1D,$CE,$84,$FD,$1F,$1D,$1F,$82,$18
		.BYTE	$CF,$1F,$CE,$88,$FD,$1F,$16,$18,$84,$FD,$1F,$16,$18,$82,$1B,$CF
		.BYTE	$18,$CE,$84,$FD,$1F,$1B,$1D,$82,$1E,$CF,$1D,$CE,$84,$FD,$1F,$1E
		.BYTE	$1F,$82,$18,$CF,$1F,$CE,$84,$FD,$1F,$16,$18,$CF,$16,$CE,$88,$FD
		.BYTE	$1F,$16,$18,$D0,$84,$77,$16,$D0,$82,$13,$D1,$16,$FF
_mus1D_lib0:
		.BYTE	$D0,$8C,$77,$16,$88,$13,$D1,$84,$D0,$84,$16,$82,$13,$D1,$16,$D0
		.BYTE	$8C,$16,$88,$13,$D1,$84,$D0,$84,$16,$82,$13,$D1,$16,$D0,$88,$79
		.BYTE	$16,$82,$78,$D1,$D0,$88,$79,$82,$78,$D1,$79,$D0,$88,$79,$82,$78
		.BYTE	$D1,$79,$D0,$84,$77,$78,$82,$77,$D1,$78,$D0,$84,$7A,$82,$78,$D1
		.BYTE	$7A,$D0,$84,$77,$82,$11,$D1,$16,$D0,$84,$7B,$14,$82,$D1,$D0,$D1
		.BYTE	$D0,$82,$86,$7C,$13,$82,$7B,$14,$D1,$7C,$13,$D0,$84,$7C,$18,$82
		.BYTE	$77,$D1,$7C,$D0,$84,$7C,$18,$82,$7B,$14,$D1,$7C,$18,$D0,$84,$7B
		.BYTE	$14,$82,$7C,$13,$84,$7B,$14,$D1,$82,$D0,$82,$16,$D1,$14,$D0,$84
		.BYTE	$7C,$18,$82,$1A,$D1,$18,$D0,$84,$6F,$82,$7D,$D1,$6F,$D0,$84,$82
		.BYTE	$7D,$D1,$6F,$D0,$84,$82,$7D,$D1,$6F,$D0,$84,$82,$7D,$D1,$6F,$D0
		.BYTE	$86,$74,$17,$D1,$82,$D0,$86,$7E,$D1,$82,$D0,$86,$72,$13,$D1,$82
		.BYTE	$D0,$86,$71,$D1,$82,$FF
_mus1B_lib0:
		.BYTE	$CB,$84,$6B,  3,$6D, $F,$82,$6C,  3,$84,$6B, $D,$82,  3,$84, $F
		.BYTE	$D8,$CB,$82,$6C,  3,$84,$6B, $D,$82, $F,$FF
_mus1C_lib0:
		.BYTE	$CB,$84,$6B,  1,$6D, $D,$82,$6C,  1,$84,$6B, $C,$82,  1,$84, $D
		.BYTE	$D8,$CB,$82,$6C,  1,$84,$6B, $C,$82, $D,$FF
_mus1E_lib0:
		.BYTE	$CB,$84,$6B,  0,$6D, $C,$82,$6C,  0,$84,$6B, $A,$82,  0,$84, $C
		.BYTE	$D8,$CB,$82,$6C,  0,$84,$6B, $A,$82, $C,$FF
_mus1F_lib0:
		.BYTE	$C9,$82,$7F,$18,$7F,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84,$CA
		.BYTE	$C9,$C9,$82,$71,$C9,$C9,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84
		.BYTE	$CA,$C9,$FF
_mus20_lib0:
		.BYTE	$CE,$8C,$FD,  4, $A, $C,$98,$84, $A, $C, $F,$FD,$1F, $F,$11,$82
		.BYTE	$CF,$CE,$84, $F, $C,$88,$FD,$1F,$11,$12,$84,$11, $F,$88,$FD,$1F
		.BYTE	 $F,$11,$82,$13,$CF,$11,$CE,$88,$FD,$4F, $F, $C,$84,$88,$FD,  8
		.BYTE	 $A, $C,$84,$FD,$1C, $A, $C,$13,$16,$13,$88,$FD,$18,$16,$18,$86
		.BYTE	$FD,$18,$16,$18,$FD,$2F,$12,$11,$84, $F, $C,$8C,$FD,$18,$16,$18
		.BYTE	$86,$FD,$18,$16,$18,$FD,$2F,$12,$11,$84, $F,$11,$12,$88,$FD,$18
		.BYTE	$12,$13,$82,$16,$CF,$13,$CE,$88,$FD,$18,$12,$11,$84, $F, $C, $A
		.BYTE	 $C,$13,$CF,$11, $F, $C,$CE,$16,$18,$1B,$FF
_mus21_lib0:
		.BYTE	$CE,$88,$FD,$18,$1B,$1D,$88,$FD,$18,$1E,$1D,$84,$1B,$16,$18,$8C
		.BYTE	$FD,$18,$1B,$1D,$88,$FD,$18,$1E,$1D,$84,$1B,$16,$18,$8C,$FD,$18
		.BYTE	$22,$24,$88,$22,$FD,$18,$1E,$1F,$84,$22,$88,$FD,$4F,$1E,$1D,$84
		.BYTE	$1D,$82,$1B,$CF,$1D,$CE,$88,$FD,$1F,$1B,$1D,$84,$18,$1B,$1D,$FD
		.BYTE	$1F,$1D,$1E,$82,$1D,$CF,$1E,$CE,$84,$1B,$84,$FD,$1F,$1D,$1E,$82
		.BYTE	$1D,$CF,$1E,$CE,$84,$1B,$84,$1D,$1F,$FD,$1F,$1B,$1D,$1B,$82,$18
		.BYTE	$CF,$1B,$CE,$88,$16,$82,$18,$CF,$16,$CE,$84,$1B,$82,$1D,$CF,$1B
		.BYTE	$CE,$88,$FD,  8,$1D,$1F,$86,$FD,  8,$21,$22,$CF,$82,$24,$CE,$84
		.BYTE	$FD,$1C,$1E,$1D,$84,$1B,$82,$18,$CF,$1B,$CE,$98,$FD,  6,$22,$24
		.BYTE	$CE,$84,$FD,$2F,$1D,$1F,$22,$24,$FF
_mus26_lib0:
		.BYTE	$CE,$98,$FD,$1F,$26,$27,$84,$24,$CF,$27,$24,$CE,$84,$18,$1B,$1D
		.BYTE	$FD,$1F,$1D,$1E,$82,$1D,$81,$1E,$1D,$84,$1B,$18,$98,$FD,$1F,$24
		.BYTE	$26,$84,$22,$CF,$26,$22,$CE,$84,$18,$84,$1B,$1D,$FD,$1F,$1D,$1E
		.BYTE	$82,$1D,$81,$1E,$1D,$84,$1B,$18,$88,$FD,$1F,$22,$24,$84,$22,$20
		.BYTE	$22,$20,$1F,$88,$FD,$18,$1B,$1D,$84,$1B,$82,$1E,$CF,$1B,$CE,$88
		.BYTE	$FD,$18,$1B,$1D,$84,$1F,$22,$24,$88,$FD,$1C,$24,$26,$84,$27,$CE
		.BYTE	$88,$FD,$1F,$24,$26,$84,$24,$CE,$FD,$1F,$22,$23,$20,$8C,$FD,$1F
		.BYTE	$1E,$1F,$84,$13,$17,$18,$1A,$1F,$FF
_mus22_lib0:
		.BYTE	$D4,$82,$3F,$3E,$3A,$37,$3E,$3A,$37,$35,$3A,$37,$35,$33,$37,$33
		.BYTE	$32,$2E,$33,$32,$2E,$2B,$32,$2E,$2B,$29,$2E,$2B,$29,$27,$2B,$29
		.BYTE	$27,$26,$27,$22,$1F,$27,$22,$1F,$29,$26,$27,$24,$1F,$27,$24,$1F
		.BYTE	$26,$27,$29,$22,$1D,$27,$22,$1D,$29,$22,$26,$22,$1D,$26,$27,$26
		.BYTE	$24,$22,$D4,$83,$24,$20,$1B,$26,$20,$1B,$27,$20,$1B,$27,$20,$1B
		.BYTE	$26,$20,$24,$20,$84,$23,$1F,$1A,$24,$1F,$1A,$26,$23,$1F,$27,$24
		.BYTE	$1F,$85,$29,$26,$2C,$2B,$A8,$FD,  1,$2E,$30,$D6,$81,$33,$30,$2E
		.BYTE	$2B,$2A,$29,$27,$24,$30,$2E,$2B,$2A,$29,$27,$24,$22,$2B,$2A,$29
		.BYTE	$27,$24,$22,$1F,$1E,$29,$27,$24,$22,$1F,$1E,$1D,$1B,$27,$24,$22
		.BYTE	$1F,$1E,$1D,$1B,$18,$24,$22,$1F,$1E,$1D,$1B,$18,$16,$22,$1F,$1E
		.BYTE	$1D,$1B,$18,$16,$13,$1F,$1E,$1D,$FF
_mus23_lib0:
		.BYTE	$E5,$FF
_mus24_lib0:
		.BYTE	$FC,$FF
_mus25_lib0:
		.BYTE	$1B,$18,$16,$13,$12,$FF
_mus27_lib0:
		.BYTE	$CB,$84,$6C,  5,$FC,$FA,$D8,$86,  3,$CB,$84,  5,$FC,$FA,$D8,$86
		.BYTE	  8,$CB,$84,  5,$FC,$FA,$D8,$86,  3,$CB,$84,  5,$FC,$F8,$D8,$88
		.BYTE	  8,$FF
_mus28_lib0:
		.BYTE	$C5,$90,$6E,  4,$F0,$FA,$86,  4,$90,$F0,$FA,$86,  4,$90,$F0,$FA
		.BYTE	$86,  4,$90,$F0,$F8,$88,  4,$FF
_mus19_lib1:
		.BYTE	$80,$63,$52,$15,$2A,$2A,$63,$15,$2A,$2A,$FF,  0
_mus15_lib0:
		.BYTE	$DB,$82,  3,$DB,  5,$D9,$82,$7E,$18,$18,$DB,$82,  5,$D9,$84,$18
		.BYTE	$DB,$82,  5,$D9,$84,$18,$DB,$82,  5,$D9,$84,$18,$DB,$82,  3,$D9
		.BYTE	$84,$7F,$16,$FF
_mus2A_lib0:
		.BYTE	$DB,$82,  1, $D,$D9,$7F,$14,$D9,$D9,$84,$82,$D9,$D9,$16,$D9,$D9
		.BYTE	$D9,$84,$DB,$82,  3,  5,  0,  1, $D,$D9,$14,$D9,$D9,$84,$82,$D9
		.BYTE	$D9,$84,$16,$82,$84,$DB,$82,  5,$11,  0,$DB,$82,  1, $D,$D9,$7F
		.BYTE	$14,$D9,$D9,$84,$82,$D9,$D9,$16,$D9,$D9,$D9,$84,$DB,$82,  3, $F
		.BYTE	  1,  0, $C,$D9,$11,$D9,$D9,$D9,$84,$D9,$13,$DB,$82,  0,  3,  0
		.BYTE	  8,  7,  3,  0,$FF
_mus1A_lib1:
		.BYTE	$8E,$2B,$2D,$2B,$87,$2E,$89,$2B,$2D,$2B,$82,$2E,$FF,  0
_mus1B_lib1:
		.BYTE	$93,$2B,$2D,$2B,$87,$2F,$8E,$2B,$2D,$2B,$82,$2F,$FF,  0
_mus1C_lib1:
		.BYTE	$87,$2C,$2C,$82,$2C,$2C,$FF,  0
_mus1D_lib1:
		.BYTE	$30,$FF,  0
_mus2B_lib0:
		.BYTE	$DB,$82, $C,$DA,$DB, $E,$84,  2,$82,  5,$DA,$DB,  2,  7,$DB,$DA
		.BYTE	$DB,  2,  8,$DA,$DB,  9,$DC,$86, $C,$DB,$82, $E,$84,  2,$82,  5
		.BYTE	$DA,$DB,  2,  7,$FF
_mus2D_lib0:
		.BYTE	$82,  7,$DA,$DB,  8,  7,  5,  2,  0,$FF
_mus2E_lib0:
		.BYTE	$DC,$84,$18,$DB,$82, $E,$DC,$84,$17,$DB,$15,$FF
_mus2F_lib0:
		.BYTE	$DC,$84,$11,$DB,$82,  9,$DC,$84,$13,$DB,$11,$FF
_mus2C_lib0:
		.BYTE	$CB,$84,$6B,  2,$D8,$82,$6D,$6B,$CB,$84,$6C,  5,$D8,$6B,$CB,  7
		.BYTE	$D8,$6D,$CB,$6C,  9,$D8,$6B,$CB,  2,$D8,$82,$6D,$6B,$CB,$84,$6C
		.BYTE	  5,$D8,$6B,$CB,  7,$D8,$82,$D8,$CB,$84,$6C,  9,$D8,$82,$D8,$FF
_mus30_lib0:
		.BYTE	$CC,$84,  2,$CC,$CD,$6E,  5,$CC,  2,$CC,$CC,$CD,  5,$CC,  2,$CC
		.BYTE	$CC,$CD,  5,$CC,  2,$CC,$CC,$CD,  5,$82,$CD,$FF
_mus1E_lib1:
		.BYTE	$86,$31,$89,$31,$FF,  0
_mus1F_lib1:
		.BYTE	$86,$32,$89,$32,$FF,  0
_mus20_lib1:
		.BYTE	$86,$71,$33,$89,$33,$FF,  0
_mus21_lib1:
		.BYTE	$34,$FF,  0
_mus31_lib0:
		.BYTE	$C9,$88,$71,$18,$84,$8C,$84,$74,$1C,$CA,$71,$18,$C9,$88,$74,$1C
		.BYTE	$84,$73,$18,$94,$88,$7F,$84,$88,$18,$84,$6F,$1D,$CA,$7F,$18,$C9
		.BYTE	$A4,$6F,$1D,$88,$71,$18,$84,$8C,$84,$74,$1C,$CA,$71,$18,$C9,$88
		.BYTE	$73,$1A,$84,$94,$90,$1B,$1D,$A0,$1F,$FF
_mus32_lib0:
		.BYTE	$CE,$88,$1F,$84,$8C,$FD,  4,$1E,$1F,$84,$24,$CF,$1F,$CE,$88,$FD
		.BYTE	  4,$23,$24,$84,$21,$88,$84,$1F,$1D,$21,$88,$FD,  4,$1D,$1F,$84
		.BYTE	$1F,$88,$84,$24,$CF,$1F,$CE,$8C,$FD,  4,$22,$24,$DD,$84,$2B,$29
		.BYTE	$FD,$1F,$29,$2B,$29,$24,$21,$CE,$88,$1F,$84,$8C,$FD,  4,$1E,$1F
		.BYTE	$84,$24,$CF,$1F,$CE,$88,$FD,  4,$22,$24,$84,$23,$8C,$84,$21,$23
		.BYTE	$86,$FD,  4,$22,$24,$22,$84,$24,$86,$FD,  4,$24,$26,$24,$84,$26
		.BYTE	$FD,$1F,$26,$28,$CF,$24,$1F,$1D,$1C,$DD,$82,$28,$2B,$88,$30,$FF
_mus33_lib0:
		.BYTE	$DF,$90,$6B,  0,$6C,  4,$6B,  5,$6C,  9,$6B,  2,$6C,  4,$6B,  5
		.BYTE	$6C,$6B,  0,$6C,  4,$6B,  7,$6C,$6B,  8,$6C, $A,$6B, $C,$6C,$FF
_mus34_lib0:
		.BYTE	$CC,$84,  0,  1,  2,  3,$CD,$6E,  5,$CC,  3,  2,  1,$FF
_mus22_lib1:
		.BYTE	$8E,$36,$23,$FE
_mus23_lib1:
		.BYTE	$8E,$71,$23,$36,$23,$FE
_mus24_lib1:
		.BYTE	$82,$71,$35,$FE
_mus25_lib1:
		.BYTE	$63,$34,$37,$FE
_mus35_lib0:
		.BYTE	$DF,$90,$6B, $C,$6C, $A,$6B,  8,$6C,  7,$6B,  5,$6C,  3,$6B,  2
		.BYTE	$A0,$6C,  0,$85, $C,$E1,$FF
_mus36_lib0:
		.BYTE	$DE,$86,$FD,  8,$1D,$1F,$1D,$84,$1B,$86,$FD,  8,$1B,$1D,$1B,$84
		.BYTE	$1A,$86,$FD,  8,$1A,$1B,$1A,$84,$18,$8C,$16,$82,$14,$16,$86,$18
		.BYTE	$1A,$84,$1B,$86,$FD, $C,$16,$18,$13,$84, $F,$86,$FD, $F, $D, $E
		.BYTE	 $C,$84, $B,$9E,$FD,  8, $B, $C,$81,$18,$24,$82,$30,$FF
_mus37_lib0:
		.BYTE	$CC,$84,  0,  1,  2,  3,$CD,$86,$6E,  5,$E1,$FF
_mus2A_lib1:
		.BYTE	$72,$38,$FE
_mus38_lib0:
		.BYTE	$DF,$86,$6D,  4,$E1,$FF
_mus2B_lib1:
		.BYTE	$8A,$73,$39,$FE
_mus2C_lib1:
		.BYTE	$8B,$73,$39,$FE
_mus39_lib0:
		.BYTE	$DF,$88,$22,$E1,$FF
_mus2D_lib1:
		.BYTE	$84,$74,$3A,$FE
_mus2E_lib1:
		.BYTE	$86,$74,$3A,$FE
_mus2F_lib1:
		.BYTE	$88,$74,$3A,$FE
_mus30_lib1:
		.BYTE	$8A,$74,$3A,$FE
_mus31_lib1:
		.BYTE	$98,$74,$3A,$FE
_mus32_lib1:
		.BYTE	$9A,$74,$3A,$FE
_mus33_lib1:
		.BYTE	$9C,$74,$3A,$FE
_mus34_lib1:
		.BYTE	$A3,$3A,$FE
_mus3A_lib0:
		.BYTE	$DF,$84,$26,$E1,$FF
_mus35_lib1:
		.BYTE	$AD,$3B,$FE
_mus36_lib1:
		.BYTE	$AA,$3B,$FE
_mus3B_lib0:
		.BYTE	$DF,$83,$26,$E1,$FF
_mus37_lib1:
		.BYTE	$3C,$FF,  0
_mus3C_lib0:
		.BYTE	$DB,$82,  2,  2,  0,  2,$D9,$88,$7F,$1A,$86,$18,$18,$84,$19,$DB
		.BYTE	$82,  2,  5,  0,  2,$D9,$88,$1A,$86,$1D,$1D,$84,$1B,$DB,$82,  2
		.BYTE	 $E,  0,  2,$D9,$88,$1A,$86,$18,$19,$84,$1A,$DB,$82,  2, $E, $C
		.BYTE	  2,$D9,$88,$1A,$86,$1F,$1D,$82,$1B,$1A,$DB,$84,  0,$82,  3,  0
		.BYTE	$D9,$86,$70,$18,$84,$DB,$82,  0,  3,  0,  5,  7, $A, $C,$84,  0
		.BYTE	$82,  3,  7,$D9,$86,$75,$1B,$84,$74,$1A,$82,$70,$18,$84,$71,$16
		.BYTE	$70,$18,$74,$DB,$84,  0,$82,  3,  0,$D9,$86,$70,$18,$84,$DB,$82
		.BYTE	  3,  5,  7, $A, $C, $F, $C,$84,  0,$82,  3,  0,$D9,$86,$72,$1F
		.BYTE	$84,$73,$1D,$82,$75,$1B,$84,$74,$1A,$75,$1B,$1C,$FF
_mus38_lib1:
		.BYTE	$3D,$3E,$FF,  0
_mus3D_lib0:
		.BYTE	$DB,$82,  2,  4,$D9,$70,$1C,$82,$84,$DB,$82,  2,  4,$D9,$71,$1A
		.BYTE	$84,$70,$1C,$D9,$D9,$82,$71,$1A,$84,$70,$1C,$DB,$82,  2,  4,$D9
		.BYTE	$70,$1C,$D9,$D9,$84,$DB,$82,  2,  4,$D9,$84,$71,$1F,$D9,$D9,$D9
		.BYTE	$82,$21,$1F,$DB,$82,  2,  4,$D9,$70,$1C,$D9,$D9,$84,$DB,$82,  2
		.BYTE	  4,$D9,$71,$1A,$84,$70,$1C,$D9,$D9,$82,$71,$1A,$84,$70,$1C,$DB
		.BYTE	$82,  1,  2,$D9,$84,$71,$1A,$DB,$82,  2,  3,$D9,$84,$71,$1B,$DB
		.BYTE	$82,  3,  4,$D9,$84,$75,$1F,$74,$1E,$82,$73,$1C,$1A,$FF
_mus3E_lib0:
		.BYTE	$DB,$82,  7,$D9,$84,$71,$1F,$DB,$82,  7,$D9,$84,$1F,$DB,$82,  9
		.BYTE	$D9,$86,$73,$1C,$84,$82,$1A,$84,$74,$19,$82,$17,$DB,$82,  7,$D9
		.BYTE	$84,$71,$1F,$DB,$82,  7,$D9,$84,$1F,$DB,$82,  9,$D9,$86,$71,$21
		.BYTE	$84,$73,$1C,$82,$1A,$84,$74,$19,$82,$17,$DB,$82,  7,$D9,$84,$71
		.BYTE	$1F,$DB,$82,  7,$D9,$84,$1F,$DB,$82,  9,$D9,$86,$73,$1C,$84,$1C
		.BYTE	$1A,$74,$19,$DB,$84,  0,$D9,$71,$18,$DB,  2,$D9,$1A,$DB,$82,  4
		.BYTE	$D9,$84,$73,$1A,$82,$84,$82,$71,$73,$FF
_mus39_lib1:
		.BYTE	$80,$3F,$40,$83,$40,$80,$40,$85,$40,$FF,  0
_mus3F_lib0:
		.BYTE	$DB,$82,  1,  3,$D9,$84,$70,$1B,$DB,$82,  3,$D9,$84,$71,$19,$DB
		.BYTE	$82,  3,$D9,$84,$70,$1B,$DB,$82,  3,$D9,$84,$71,$19,$DB,$82,  3
		.BYTE	$D9,$84,$70,$1B,$DB,$82,  1,  3,$D9,$84,$70,$1B,$DB,$82,  3,$D9
		.BYTE	$84,$71,$19,$DB,$82,  3,$D9,$84,$70,$1B,$DB,$82,  3,$D9,$84,$71
		.BYTE	$19,$DB,$82,  3,$D9,$84,$73,$1B,$DB,$82,  1,  3,$D9,$84,$73,$19
		.BYTE	$DB,$82,  3,$D9,$84,$71,$19,$DB,$82,  3,$D9,$84,$73,$19,$DB,$82
		.BYTE	  3,$D9,$84,$71,$19,$DB,$82,  3,$D9,$84,$73,$19,$DB,$82,  1,  3
		.BYTE	$D9,$84,$19,$DB,$82,  3,$D9,$84,$71,$19,$DB,$82,  3,$D9,$84,$74
		.BYTE	$16,$DB,$82,  3,$D9,$84,$73,$14,$DB,$82,  3,$D9,$74,$11,$75,$12
		.BYTE	$FF
_mus40_lib0:
		.BYTE	$DB,$82,  1,  3,$D9,$70,$1B,$84,$D9,$DB,$82,  3,$D9,$84,$73,$19
		.BYTE	$DB,$82,  3,$D9,$84,$19,$DB,$82,  3,$D9,$84,$19,$DB,$82,  1,$DB
		.BYTE	$82,  3,$D9,$82,$70,$1B,$84,$D9,$DB,$82,  3,$D9,$84,$73,$1B,$DB
		.BYTE	$82,  3,$D9,$84,$1B,$DB,$82,  3,$D9,$84,$1B,$FF
_mus3A_lib1:
		.BYTE	$80,$41,$41,$42,$85,$41,$41,$42,$82,$41,$41,$42,$87,$41,$41,$42
		.BYTE	$FF,  0
_mus41_lib0:
		.BYTE	$D9,$84,$78, $E,$82,$78,$82,$84,$84,$82, $C,$82,$84,$84,$82, $A
		.BYTE	 $C,$84, $E,$82,$82,$84,$84,$82, $F,$82,$84,$84,$82,$11, $F,$FF
_mus42_lib0:
		.BYTE	$D9,$84,$78, $E,$82,$13,$11,$84, $F, $C,$82, $E,$82,$82,$82,$82
		.BYTE	 $F, $E,$84, $C, $E,$82,$13,$11,$84, $F, $C,$90, $E,$84, $F,$82
		.BYTE	$82,$84,$84, $E,$88, $C,$84, $A,  7,  9,$82,$82,$84,$84,  7,$90
		.BYTE	  9,$FF
_mus3B_lib1:
		.BYTE	$72,$43,$FE
_mus43_lib0:
		.BYTE	$CD,$81,  0,  2,  3,  5,  2,  3,  2,  4,  6,  3,  0,  2,  2,  4
		.BYTE	  5,  2,  1,  3,  5,  0,  2,  4,  1,  3,  0,  4,  2,  5,  4,  0
		.BYTE	  1,  5,  3,  2,  0,  6,  1,  3,  1,  4,  3,  2,  3,$83,  2,$E1
		.BYTE	$FF
_mus00_lib1:
		.BYTE	  0,$FE
_mus00_lib0:
		.BYTE	$E1,$FF
		
; --------------------------------
; SPRITES DATA
; --------------------------------

_srp_lib_lo:
		.BYTE	<[_spr00]
		.BYTE	<[_spr01]
		.BYTE	<[_spr02]
		.BYTE	<[_spr03]
		.BYTE	<[_spr04]
		.BYTE	<[_spr05]
		.BYTE	<[_spr06]
		.BYTE	<[_spr07]
		.BYTE	<[_spr08]
		.BYTE	<[_spr09]
		.BYTE	<[_spr0A]
		.BYTE	<[_spr0B]
		.BYTE	<[_spr0C]
		.BYTE	<[_spr0D]
		.BYTE	<[_spr0E]
		.BYTE	<[_spr0F]
		.BYTE	<[_spr10]
		.BYTE	<[_spr11]
		.BYTE	<[_spr12]
		.BYTE	<[_spr13]
		.BYTE	<[byte_52210]
		.BYTE	<[byte_52230]
		.BYTE	<[byte_52250]
		.BYTE	<[byte_52270]
		.BYTE	<[byte_52290]
		.BYTE	<[byte_522B0]
		.BYTE	<[byte_522D0]
		.BYTE	<[byte_522F0]
		.BYTE	<[byte_52310]
		.BYTE	<[byte_52330]
		.BYTE	<[byte_52350]
		.BYTE	<[byte_52370]
		.BYTE	<[_spr20]
		.BYTE	<[byte_523B0]
		.BYTE	<[byte_523D0]
		.BYTE	<[byte_523F0]
		.BYTE	<[byte_52410]
		.BYTE	<[byte_52430]
		.BYTE	<[byte_5244A]
		.BYTE	<[byte_5246A]
		.BYTE	<[byte_5248A]
		.BYTE	<[byte_524AA]
		.BYTE	<[byte_524CA]
		.BYTE	<[byte_524EA]
		.BYTE	<[byte_5250A]
		.BYTE	<[byte_5252A]
		.BYTE	<[byte_5254A]
		.BYTE	<[byte_5256A]
		.BYTE	<[_spr30]
		.BYTE	<[byte_52598]
		.BYTE	<[byte_525A6]
		.BYTE	<[byte_525B4]
		.BYTE	<[byte_525C2]
		.BYTE	<[byte_525D0]
		.BYTE	<[byte_525DE]
		.BYTE	<[byte_525EC]
		.BYTE	<[byte_525FA]
		.BYTE	<[byte_5261A]
		.BYTE	<[byte_5263A]
		.BYTE	<[byte_5265A]
		.BYTE	<[byte_5267A]
		.BYTE	<[byte_5269A]
		.BYTE	<[byte_526BA]
		.BYTE	<[byte_526DA]
		.BYTE	<[_spr40]
		.BYTE	<[byte_5271A]
		.BYTE	<[byte_5273A]
		.BYTE	<[byte_5275A]
		.BYTE	<[byte_5277A]
		.BYTE	<[byte_52788]
		.BYTE	<[byte_52796]
		.BYTE	<[byte_527A4]
		.BYTE	<[byte_527B2]
		.BYTE	<[byte_527C0]
		.BYTE	<[byte_527CE]
		.BYTE	<[byte_527DC]
		.BYTE	<[byte_527EA]
		.BYTE	<[byte_527F8]
		.BYTE	<[byte_52806]
		.BYTE	<[byte_52814]
		.BYTE	<[_spr50]
		.BYTE	<[byte_52830]
		.BYTE	<[byte_5283E]
		.BYTE	<[byte_5284C]
		.BYTE	<[byte_5285A]
		.BYTE	<[byte_5287A]
		.BYTE	<[byte_5289A]
		.BYTE	<[byte_528BA]
		.BYTE	<[byte_528DA]
		.BYTE	<[byte_528FA]
		.BYTE	<[byte_52900]
		.BYTE	<[byte_52920]
		.BYTE	<[byte_52940]
		.BYTE	<[byte_52960]
		.BYTE	<[byte_52980]
		.BYTE	<[byte_529A0]
		.BYTE	<[_spr60]
		.BYTE	<[byte_529E0]
		.BYTE	<[byte_529EE]
		.BYTE	<[byte_529FC]
		.BYTE	<[byte_52A0A]
		.BYTE	<[byte_52A18]
		.BYTE	<[byte_52A38]
		.BYTE	<[byte_52A58]
		.BYTE	<[byte_52A78]
		.BYTE	<[byte_52A98]
		.BYTE	<[byte_52AA6]
		.BYTE	<[byte_52AB4]
		.BYTE	<[byte_52AC2]
		.BYTE	<[_spr6D_unused]
		.BYTE	<[_spr6E_unused]
		.BYTE	<[_spr6F_unused]
		.BYTE	<[_spr70_unused]
		.BYTE	<[byte_52B08]
		.BYTE	<[byte_52B28]
		.BYTE	<[byte_52B48]
		.BYTE	<[byte_52B68]
		.BYTE	<[byte_52B88]
		.BYTE	<[byte_52BA8]
		.BYTE	<[byte_52BC8]
		.BYTE	<[byte_52BE8]
		.BYTE	<[byte_52C08]
		.BYTE	<[byte_52C28]
		.BYTE	<[byte_52C48]
		.BYTE	<[byte_52C68]
		.BYTE	<[byte_52C88]
		.BYTE	<[byte_52CA8]
		.BYTE	<[byte_52CC8]
		.BYTE	<[byte_52CE8]
		.BYTE	<[byte_52D08]
		.BYTE	<[byte_52D28]
		.BYTE	<[byte_52D48]
		.BYTE	<[byte_52D68]
		.BYTE	<[byte_52D88]
		.BYTE	<[byte_52DA8]
		.BYTE	<[byte_52DC8]
		.BYTE	<[byte_52DE8]
		.BYTE	<[byte_52E22]
		.BYTE	<[byte_52E5C]
		.BYTE	<[byte_52E96]
		.BYTE	<[byte_52ED0]
		.BYTE	<[byte_52ED8]
		.BYTE	<[byte_52EE0]
		.BYTE	<[byte_52EE8]
		.BYTE	<[byte_52EF0]
		.BYTE	<[byte_52EF8]
		.BYTE	<[byte_52EFC]
		.BYTE	<[byte_52F00]
		.BYTE	<[byte_52F05]
		.BYTE	<[byte_52F25]
		.BYTE	<[byte_52F45]
		.BYTE	<[byte_52F65]
		.BYTE	<[byte_52F85]
		.BYTE	<[byte_52FA5]
		.BYTE	<[byte_52FC5]
		.BYTE	<[byte_52FE5]
		.BYTE	<[byte_53005]
		.BYTE	<[byte_53025]
		.BYTE	<[byte_53045]
		.BYTE	<[byte_5304B]
		.BYTE	<[byte_53051]
		.BYTE	<[byte_53057]
		.BYTE	<[byte_5305D]
		.BYTE	<[byte_5307D]
		.BYTE	<[byte_5309D]
		.BYTE	<[byte_530BD]
		.BYTE	<[byte_530DD]
		.BYTE	<[byte_530FD]
		.BYTE	<[byte_5311D]
		.BYTE	<[byte_5313D]
		.BYTE	<[byte_5315D]
		.BYTE	<[byte_5317D]
		.BYTE	<[byte_53182]
		.BYTE	<[byte_5318A]
		.BYTE	<[byte_53198]
		.BYTE	<[byte_531A6]
		.BYTE	<[byte_531B4]
		.BYTE	<[byte_531C2]
		.BYTE	<[byte_531E2]
		.BYTE	<[byte_53202]
		.BYTE	<[byte_53222]
		.BYTE	<[byte_53242]
		.BYTE	<[byte_53262]
		.BYTE	<[byte_53282]
		.BYTE	<[byte_532A2]
		.BYTE	<[byte_532C2]
		.BYTE	<[byte_532E2]
		.BYTE	<[byte_53302]
		.BYTE	<[byte_53322]
_srp_lib_hi:
		.BYTE	>[_spr00]
		.BYTE	>[_spr01]
		.BYTE	>[_spr02]
		.BYTE	>[_spr03]
		.BYTE	>[_spr04]
		.BYTE	>[_spr05]
		.BYTE	>[_spr06]
		.BYTE	>[_spr07]
		.BYTE	>[_spr08]
		.BYTE	>[_spr09]
		.BYTE	>[_spr0A]
		.BYTE	>[_spr0B]
		.BYTE	>[_spr0C]
		.BYTE	>[_spr0D]
		.BYTE	>[_spr0E]
		.BYTE	>[_spr0F]
		.BYTE	>[_spr10]
		.BYTE	>[_spr11]
		.BYTE	>[_spr12]
		.BYTE	>[_spr13]
		.BYTE	>[byte_52210]
		.BYTE	>[byte_52230]
		.BYTE	>[byte_52250]
		.BYTE	>[byte_52270]
		.BYTE	>[byte_52290]
		.BYTE	>[byte_522B0]
		.BYTE	>[byte_522D0]
		.BYTE	>[byte_522F0]
		.BYTE	>[byte_52310]
		.BYTE	>[byte_52330]
		.BYTE	>[byte_52350]
		.BYTE	>[byte_52370]
		.BYTE	>[_spr20]
		.BYTE	>[byte_523B0]
		.BYTE	>[byte_523D0]
		.BYTE	>[byte_523F0]
		.BYTE	>[byte_52410]
		.BYTE	>[byte_52430]
		.BYTE	>[byte_5244A]
		.BYTE	>[byte_5246A]
		.BYTE	>[byte_5248A]
		.BYTE	>[byte_524AA]
		.BYTE	>[byte_524CA]
		.BYTE	>[byte_524EA]
		.BYTE	>[byte_5250A]
		.BYTE	>[byte_5252A]
		.BYTE	>[byte_5254A]
		.BYTE	>[byte_5256A]
		.BYTE	>[_spr30]
		.BYTE	>[byte_52598]
		.BYTE	>[byte_525A6]
		.BYTE	>[byte_525B4]
		.BYTE	>[byte_525C2]
		.BYTE	>[byte_525D0]
		.BYTE	>[byte_525DE]
		.BYTE	>[byte_525EC]
		.BYTE	>[byte_525FA]
		.BYTE	>[byte_5261A]
		.BYTE	>[byte_5263A]
		.BYTE	>[byte_5265A]
		.BYTE	>[byte_5267A]
		.BYTE	>[byte_5269A]
		.BYTE	>[byte_526BA]
		.BYTE	>[byte_526DA]
		.BYTE	>[_spr40]
		.BYTE	>[byte_5271A]
		.BYTE	>[byte_5273A]
		.BYTE	>[byte_5275A]
		.BYTE	>[byte_5277A]
		.BYTE	>[byte_52788]
		.BYTE	>[byte_52796]
		.BYTE	>[byte_527A4]
		.BYTE	>[byte_527B2]
		.BYTE	>[byte_527C0]
		.BYTE	>[byte_527CE]
		.BYTE	>[byte_527DC]
		.BYTE	>[byte_527EA]
		.BYTE	>[byte_527F8]
		.BYTE	>[byte_52806]
		.BYTE	>[byte_52814]
		.BYTE	>[_spr50]
		.BYTE	>[byte_52830]
		.BYTE	>[byte_5283E]
		.BYTE	>[byte_5284C]
		.BYTE	>[byte_5285A]
		.BYTE	>[byte_5287A]
		.BYTE	>[byte_5289A]
		.BYTE	>[byte_528BA]
		.BYTE	>[byte_528DA]
		.BYTE	>[byte_528FA]
		.BYTE	>[byte_52900]
		.BYTE	>[byte_52920]
		.BYTE	>[byte_52940]
		.BYTE	>[byte_52960]
		.BYTE	>[byte_52980]
		.BYTE	>[byte_529A0]
		.BYTE	>[_spr60]
		.BYTE	>[byte_529E0]
		.BYTE	>[byte_529EE]
		.BYTE	>[byte_529FC]
		.BYTE	>[byte_52A0A]
		.BYTE	>[byte_52A18]
		.BYTE	>[byte_52A38]
		.BYTE	>[byte_52A58]
		.BYTE	>[byte_52A78]
		.BYTE	>[byte_52A98]
		.BYTE	>[byte_52AA6]
		.BYTE	>[byte_52AB4]
		.BYTE	>[byte_52AC2]
		.BYTE	>[_spr6D_unused]
		.BYTE	>[_spr6E_unused]
		.BYTE	>[_spr6F_unused]
		.BYTE	>[_spr70_unused]
		.BYTE	>[byte_52B08]
		.BYTE	>[byte_52B28]
		.BYTE	>[byte_52B48]
		.BYTE	>[byte_52B68]
		.BYTE	>[byte_52B88]
		.BYTE	>[byte_52BA8]
		.BYTE	>[byte_52BC8]
		.BYTE	>[byte_52BE8]
		.BYTE	>[byte_52C08]
		.BYTE	>[byte_52C28]
		.BYTE	>[byte_52C48]
		.BYTE	>[byte_52C68]
		.BYTE	>[byte_52C88]
		.BYTE	>[byte_52CA8]
		.BYTE	>[byte_52CC8]
		.BYTE	>[byte_52CE8]
		.BYTE	>[byte_52D08]
		.BYTE	>[byte_52D28]
		.BYTE	>[byte_52D48]
		.BYTE	>[byte_52D68]
		.BYTE	>[byte_52D88]
		.BYTE	>[byte_52DA8]
		.BYTE	>[byte_52DC8]
		.BYTE	>[byte_52DE8]
		.BYTE	>[byte_52E22]
		.BYTE	>[byte_52E5C]
		.BYTE	>[byte_52E96]
		.BYTE	>[byte_52ED0]
		.BYTE	>[byte_52ED8]
		.BYTE	>[byte_52EE0]
		.BYTE	>[byte_52EE8]
		.BYTE	>[byte_52EF0]
		.BYTE	>[byte_52EF8]
		.BYTE	>[byte_52EFC]
		.BYTE	>[byte_52F00]
		.BYTE	>[byte_52F05]
		.BYTE	>[byte_52F25]
		.BYTE	>[byte_52F45]
		.BYTE	>[byte_52F65]
		.BYTE	>[byte_52F85]
		.BYTE	>[byte_52FA5]
		.BYTE	>[byte_52FC5]
		.BYTE	>[byte_52FE5]
		.BYTE	>[byte_53005]
		.BYTE	>[byte_53025]
		.BYTE	>[byte_53045]
		.BYTE	>[byte_5304B]
		.BYTE	>[byte_53051]
		.BYTE	>[byte_53057]
		.BYTE	>[byte_5305D]
		.BYTE	>[byte_5307D]
		.BYTE	>[byte_5309D]
		.BYTE	>[byte_530BD]
		.BYTE	>[byte_530DD]
		.BYTE	>[byte_530FD]
		.BYTE	>[byte_5311D]
		.BYTE	>[byte_5313D]
		.BYTE	>[byte_5315D]
		.BYTE	>[byte_5317D]
		.BYTE	>[byte_53182]
		.BYTE	>[byte_5318A]
		.BYTE	>[byte_53198]
		.BYTE	>[byte_531A6]
		.BYTE	>[byte_531B4]
		.BYTE	>[byte_531C2]
		.BYTE	>[byte_531E2]
		.BYTE	>[byte_53202]
		.BYTE	>[byte_53222]
		.BYTE	>[byte_53242]
		.BYTE	>[byte_53262]
		.BYTE	>[byte_53282]
		.BYTE	>[byte_532A2]
		.BYTE	>[byte_532C2]
		.BYTE	>[byte_532E2]
		.BYTE	>[byte_53302]
		.BYTE	>[byte_53322]
_spr00:
		.BYTE	  1,  1
		.BYTE	  0,  1
_spr01:
		.BYTE	  2,  1
		.BYTE	  0,  2,  3
_spr02:
		.BYTE	  1,  2
		.BYTE	  0,  4
		.BYTE	  0,  5
_spr03:
		.BYTE	  1,  1,  0,  6
_spr04:
		.BYTE	  1,  1,  0,  7
_spr05:
		.BYTE	  1,  1,  0,  8
_spr06:
		.BYTE	  1,  1,  0,  8
_spr07:
		.BYTE	  1,  1,  0,  9
_spr08:
		.BYTE	  1,  1,  0, $A
_spr09:
		.BYTE	  1,  1,  0, $B
_spr0A:
		.BYTE	  1,  1,  0, $C
_spr0B:
		.BYTE	  1,  1,  0, $D
_spr0C:
		.BYTE	  1,  1,  0, $E
_spr0D:
		.BYTE	  1,  1,  0, $F
_spr0E:
		.BYTE	  1,  1,  0,$10
_spr0F:
		.BYTE	  1,  1,  0,$11
_spr10:
		.BYTE	  1,  1,  0,$12
_spr11:
		.BYTE	  1,  1,  0,$13
_spr12:
		.BYTE	  1,  1,  0,$14
_spr13:
		.BYTE	  1,  1,  0,$15
byte_52210:
		.BYTE	  4,  6
		.BYTE	$FF,  0,$16,$17,  0
		.BYTE	  1,  0,$18,$19,  0
		.BYTE	  1,$1A,$1B,$1C,  0
		.BYTE	  0,  0,$1D,$1E,  0
		.BYTE	  0,  0,$1F,$20,  0
		.BYTE	  0,  0,$21,$22,  0
byte_52230:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,  1,  0,$25,$26,  0,  1,$27,$28,$29
		.BYTE	  0,  0,  0,$2A,$2B,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
byte_52250:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,  1,  0,$18,$19,  0,  1,$1A,$1B,$1C
		.BYTE	  0,  0,  0,$32,$33,  0,  0,  0,$34,$35,  0,  0,  0,$21,$36,  0
byte_52270:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,  1,  0,$37,$38,  0,  1,$39,$3A,$3B
		.BYTE	  0,  0,  0,$3C,$3D,  0,  0,  0,$3E,$3F,$2E,  0,  0,$40,$41,$42
byte_52290:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,  1,  0,$18,$19,  0,  1,$1A,$1B,$1C
		.BYTE	  0,  0,  0,$32,$33,  0,  1,  0,$43,$35,  0,  1,  0,$44,$45,  0
byte_522B0:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$46,$47,$48,  0,$FF,$49,$4A,$4B
		.BYTE	  0,  0,  0,$4C,$1E,  0,  0,  0,$1F,$20,  0,  0,  0,$21,$22,  0
byte_522D0:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,$FF,$4D,$4E,$4F,  0,$FF,$50,$51,$52
		.BYTE	  0,  0,  0,$53,$54,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
byte_522F0:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$46,$47,$48,  0,$FF,$49,$4A,$4B
		.BYTE	  0,  0,  0,$55,$33,  0,  0,  0,$34,$35,  0,  0,  0,$21,$36,  0
byte_52310:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,$FF,$4D,$4E,$4F,  0,$FF,$50,$51,$52
		.BYTE	  0,  0,  0,$56,$3D,  0,  0,  0,$3E,$3F,$2E,  0,  0,$40,$41,$42
byte_52330:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$46,$47,$48,  0,$FF,$49,$4A,$4B
		.BYTE	  0,  0,  0,$55,$33,  0,  1,  0,$43,$35,  0,  1,  0,$44,$45,  0
byte_52350:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$57,$58,$59,  0,$FF,$5A,$5B,$5C
		.BYTE	  0,  0,  0,$4C,$1E,  0,  0,  0,$1F,$20,  0,  0,  0,$21,$22,  0
byte_52370:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,$FF,$5D,$5E,$5F,  0,$FF,$60,$61,$62
		.BYTE	  0,  0,  0,$53,$54,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
_spr20:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$57,$58,$59,  0,$FF,$5A,$5B,$5C
		.BYTE	  0,  0,  0,$55,$33,  0,  0,  0,$34,$35,  0,  0,  0,$21,$36,  0
byte_523B0:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,$FF,$5D,$5E,$5F,  0,$FF,$60,$61,$62
		.BYTE	  0,  0,  0,$56,$3D,  0,  0,  0,$3E,$3F,$2E,  0,  0,$40,$41,$42
byte_523D0:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$57,$58,$59,  0,$FF,$5A,$5B,$5C
		.BYTE	  0,  0,  0,$55,$33,  0,  1,  0,$43,$35,  0,  1,  0,$44,$45,  0
byte_523F0:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$46,$47,$48,  0,$FF,$49,$4A,$4B
		.BYTE	  0,$FF,  0,$63,$3D,  0,$FF,  0,$64,$65,$66,$FF,  0,  0,$67,$68
byte_52410:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$57,$58,$59,  0,$FF,$5A,$5B,$5C
		.BYTE	  0,$FF,  0,$63,$3D,  0,$FF,  0,$64,$65,$66,$FF,  0,  0,$67,$68
byte_52430:
		.BYTE	  3,  6,  0,$69,$6A,  0,  0,$6B,$6C,$6D,  0,$6E,$6F,$70,  0,$71
		.BYTE	$72,$73,  0,$74,$75,$76,  0,$77,  0,$78
byte_5244A:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,$FF,$4D,$4E,$4F,  0,$FF,$50,$51,$52
		.BYTE	  0,  0,  0,$56,$3D,  0,  0,  0,$3E,$3F,$2E,  0,  0,$40,$41,$42
byte_5246A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$23,$24,  0,  0,$5D,$5E,$5F
		.BYTE	  0,  0,$60,$61,$62,  0,  0,  0,$79,$7A,  0,  0,  0,$7B,$7C,$7D
byte_5248A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$16,$17
		.BYTE	  0,  0,$46,$47,$48,  0,  0,$49,$7E,$7F,  0,  0,  0,$80,$81,$82
byte_524AA:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$16,$17
		.BYTE	  0,  0,$57,$58,$59,  0,  0,$5A,$83,$84,  0,  0,  0,$80,$81,$82
byte_524CA:
		.BYTE	  4,  6,$FF,$85,$23,$24,  0,$FF,$86,$87,$88,  0,$FF,  0,$89,$8A
		.BYTE	  0,  0,  0,$53,$54,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
byte_524EA:
		.BYTE	  4,  6,$FF,$8B,$23,$24,  0,$FF,$8C,$8D,$8E,  0,$FF,  0,$8F,$90
		.BYTE	  0,  0,  0,$53,$54,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
byte_5250A:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,  1,  0,$25,$91,  0,  1,$27,$92,$93
		.BYTE	  0,  0,$94,$95,$96,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
byte_5252A:
		.BYTE	  4,  6,$FF,  0,$23,$24,  0,  1,  0,$25,$26,  0,  1,$27,$28,$29
		.BYTE	  0,  0,  0,$2A,$2B,  0,  0,  0,$2C,$2D,$2E,  0,  0,$2F,$30,$31
byte_5254A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$98,$99,$9A,  0,  0,$9B,$9C
		.BYTE	  0,  0,  0,$9D,$9E,  0,  0,  0,$9F,$A0,  0,  0,  0,$A1,$A2,  0
byte_5256A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$A3,$A4,  0,  0,  0,$9B,$A5
		.BYTE	  0,  0,  0,$9D,$9E,  0,  0,  0,$9F,$A0,  0,  0,  0,$A1,$A2,  0
_spr30:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A6,$A7,$A8
byte_52598:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,$A9,$AA,$AB,  0,$AC,$AD,$AE
byte_525A6:
		.BYTE	  3,  3,  0,$AF,$B0,$B1,  0,$B2,$B3,$B4,  0,$B5,$B6,$B7
byte_525B4:
		.BYTE	  3,  3,  0,$AF,$B0,$B1,  0,$B8,$B3,$B4,  0,$B9,$BA,$BB
byte_525C2:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A6,$A7,$A8
byte_525D0:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,$A9,$AA,$AB,  0,$AC,$AD,$AE
byte_525DE:
		.BYTE	  3,  3,  0,$BC,$BD,  0,  0,$BE,$BF,$C0,  0,$C1,$B6,$B7
byte_525EC:
		.BYTE	  3,  3,  0,$BC,$BD,  0,  0,$C2,$BF,$C0,  0,$C3,$BA,$C4
byte_525FA:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$C5,$C6,  0,  0,  0,$C7,$C8
		.BYTE	  0,  0,  0,$C9,$CA,  0,  0,  0,$CB,$CC,  0,  0,  0,$CD,$CE,  0
byte_5261A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CF,$D0,  0,  0,  0,$D1,$D2
		.BYTE	  0,  0,  0,$D3,$D4,  0,  0,  0,$D5,$D6,  0,  0,  0,$D7,$D8,$D9
byte_5263A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$C5,$C6,  0,  0,  0,$C7,$C8
		.BYTE	  0,  0,  0,$C9,$CA,  0,  0,  0,$DA,$DB,  0,  0,  0,$CD,$DC,  0
byte_5265A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CF,$DD,  0,  0,  0,$DE,$DF
		.BYTE	  0,  0,  0,$E0,$E1,  0,  0,  0,$E2,$E3,  0,  0,  0,$E4,$E5,$D9
byte_5267A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CF,$D0,  0,  0,$E6,$E7,$E8
		.BYTE	  0,  0,  0,$E9,$EA,  0,  0,  0,$D5,$D6,  0,  0,  0,$D7,$D8,$D9
byte_5269A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CF,$D0,  0,  0,$E6,$E7,$E8
		.BYTE	  0,  0,$EB,$EC,$ED,  0,  0,$EE,$EF,$F0,  0,  0,  0,  0,$CE,  0
byte_526BA:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F1,$F2,  0,  0,  0,$C7,$F3
		.BYTE	  0,  0,  0,$C9,$CA,  0,  0,  0,$CB,$CC,  0,  0,  0,$CD,$CE,  0
byte_526DA:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F4,$F5,  0,  0,  0,$D1,$F6
		.BYTE	  0,  0,  0,$D3,$D4,  0,  0,  0,$D5,$D6,  0,  0,  0,$D7,$D8,$D9
_spr40:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F1,$F2,  0,  0,  0,$C7,$F3
		.BYTE	  0,  0,  0,$C9,$CA,  0,  0,  0,$DA,$DB,  0,  0,  0,$CD,$DC,  0
byte_5271A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F4,$F5,  0,  0,  0,$F7,$F8
		.BYTE	  0,  0,  0,$E0,$E1,  0,  0,  0,$E2,$E3,  0,  0,  0,$E4,$E5,$D9
byte_5273A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F4,$F5,  0,  0,$E6,$F9,$FA
		.BYTE	  0,  0,  0,$E9,$EA,  0,  0,  0,$D5,$D6,  0,  0,  0,$D7,$D8,$D9
byte_5275A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F4,$F5,  0,  0,$E6,$F9,$FA
		.BYTE	  0,  0,$EB,$EC,$ED,  0,  0,$EE,$EF,$F0,  0,  0,  0,  0,$CE,  0
byte_5277A:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A6
byte_52788:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,$A7,$A8,  0,  0,$A9,$AA
byte_52796:
		.BYTE	  3,  3,  0,  0,$AB,$AC,  0,  0,$AD,$AE,  0,$AF,$B0,$B1
byte_527A4:
		.BYTE	  3,  3,  0,  0,$AB,$AC,  0,  0,$B2,$B3,  0,$B4,$B5,$B6
byte_527B2:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A6
byte_527C0:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,$A7,$A8,  0,  0,$A9,$AA
byte_527CE:
		.BYTE	  3,  3,  0,$AB,$AC,$B7,  0,$B8,$B9,$BA,  0,$BB,$BC,$BD
byte_527DC:
		.BYTE	  3,  3,  0,$BE,$BF,$C0,  0,  0,$C1,$C2,  0,  0,$C3,$C4
byte_527EA:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$C5
byte_527F8:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,$C6,$C7,  0,  0,$A9,$AA
byte_52806:
		.BYTE	  3,  3,  0,  0,$C8,$C9,  0,  0,$AD,$CA,  0,$AF,$B0,$B1
byte_52814:
		.BYTE	  3,  3,  0,  0,$C8,$C9,  0,  0,$B2,$CB,  0,$B4,$B5,$B6
_spr50:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$C5
byte_52830:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,$C6,$C7,  0,  0,$A9,$AA
byte_5283E:
		.BYTE	  3,  3,  0,$C8,$C9,$B7,  0,$B8,$CC,$BA,  0,$BB,$BC,$BD
byte_5284C:
		.BYTE	  3,  3,  0,$BE,$CD,$CE,  0,  0,$C1,$C2,  0,  0,$C3,$C4
byte_5285A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CF,$D0,  0,  0,  0,$D1,$D2
		.BYTE	  0,  0,  0,$D3,$D4,  0,  0,  0,$D5,$D6,  0,  0,  0,$D7,$D8,  0
byte_5287A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$D9,$DA,  0,  0,  0,$DB,$DC
		.BYTE	  0,  0,  0,$DD,$DE,  0,  0,  0,$DF,$E0,  0,  0,  0,$E1,$E2,$E3
byte_5289A:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CF,$D0,  0,  0,  0,$D1,$D2
		.BYTE	  0,  0,  0,$E4,$E5,  0,  0,  0,$E6,$E7,  0,  0,  0,$D7,$E8,  0
byte_528BA:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$D9,$E9,  0,  0,  0,$EA,$EB
		.BYTE	  0,  0,  0,$EC,$ED,  0,  0,  0,$EE,$EF,  0,  0,  0,$F0,$F1,$E3
byte_528DA:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$F2,$F3
		.BYTE	  0,  0,$F4,$F5,$F6,  0,  0,$F7,$F8,$F9,  0,  0,  0,$FA,$FB,$FC
byte_528FA:
		.BYTE	  3,  1,  0,$15,$15,$15
byte_52900:
		.BYTE	  4,  6,  0,$97,  0,  0,  0,  0,$98,$99,$9A,  0,  0,$9B,$9C,$9D
		.BYTE	  0,  0,  0,$9E,$9F,  0,  0,  0,$A0,$A1,  0,  0,  0,$A2,$A3,  0
byte_52920:
		.BYTE	  4,  6,  0,$A4,$A5,  0,  0,  0,$A6,$A7,$A8,  0,  0,$A9,$AA,$AB
		.BYTE	  0,  0,  0,$AC,$AD,  0,  0,  0,$AE,$AF,  0,  0,  0,$B0,$B1,$B2
byte_52940:
		.BYTE	  4,  6,  0,$97,  0,  0,  0,  0,$98,$99,$9A,  0,  0,$9B,$9C,$9D
		.BYTE	  0,  0,  0,$9E,$9F,  0,  0,  0,$B3,$B4,  0,  0,  0,$A2,$B5,  0
byte_52960:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,$B6,$B7,$A8,  0,  0,$B8,$B9,$BA
		.BYTE	  0,  0,  0,$BB,$BC,  0,  0,  0,$BD,$BE,  0,  0,  0,$BF,$C0,$C1
byte_52980:
		.BYTE	  4,  6,  0,  0,  0,  0,$C2,  0,  0,$C3,$C4,$C5,  0,  0,$C6,$C7
		.BYTE	$C8,  0,  0,$C9,$CA,  0,  0,  0,$CB,$CC,$CD,  0,$CE,$CF,$D0,$D1
byte_529A0:
		.BYTE	  4,  6,  0,  0,  0,  0,$D2,  0,  0,$D3,$D4,$D5,  0,  0,$D6,$D7
		.BYTE	  0,  0,$D8,$D9,$DA,  0,  0,$DB,$DC,$DD,  0,  0,$DE,$DF,$E0,$E1
_spr60:
		.BYTE	  4,  6
		.BYTE	  0,  0,  0,  0,  0
		.BYTE	  0,$E2,$E3,  0,  0
		.BYTE	  0,$E4,$E5,$E6,  0
		.BYTE	  0,$E7,$E8,$E9,$EA
		.BYTE	  0,  0,$EB,$EC,  0
		.BYTE	  0,  0,$ED,$EE,$EF
byte_529E0:
		.BYTE	  3,  3
		.BYTE	  0,$F0,$F1,$F2
		.BYTE	  0,$F3,$F4,$F5
		.BYTE	  0,  0,$F6,$F7
byte_529EE:
		.BYTE	  3,  3
		.BYTE	  0,$F8,$F9,$FA
		.BYTE	  0,$FB,$FC,$FD
		.BYTE	  0,$FE,$FF,  0
byte_529FC:
		.BYTE	  3,  3,  0,  0,$F6,$F7,  0,$F3,$F4,$F5,  0,$F0,$F1,$F2
byte_52A0A:
		.BYTE	  3,  3,  0,$FE,$FF,  0,  0,$FB,$FC,$FD,  0,$F8,$F9,$FA
byte_52A18:
		.BYTE	  4,  6,  0,  0,$98,  0,  0,  0,  0,$99,$9A,  0,  0,  0,$9B,$9C
		.BYTE	  0,  0,$9D,$9E,$9F,  0,  0,  0,$A0,$A1,  0,  0,  0,$A2,$A3,  0
byte_52A38:
		.BYTE	  4,  6,  0,  0,$98,  0,  0,  0,  0,$99,$9A,  0,  0,  0,$A4,$A5
		.BYTE	  0,  0,$A6,$A7,$9F,  0,  0,  0,$A0,$A1,  0,  0,  0,$A2,$A3,  0
byte_52A58:
		.BYTE	  4,  6,  0,  0,$98,  0,  0,  0,  0,$A8,$A9,  0,  0,  0,$AA,$AB
		.BYTE	  0,  0,$9D,$9E,$9F,  0,  0,  0,$A0,$A1,  0,  0,  0,$A2,$A3,  0
byte_52A78:
		.BYTE	  4,  6,  0,  0,$98,  0,  0,  0,  0,$A8,$A9,  0,  0,  0,$AC,$AD
		.BYTE	  0,  0,$A6,$A7,$9F,  0,  0,  0,$A0,$A1,  0,  0,  0,$A2,$A3,  0
byte_52A98:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$AE,$AF
byte_52AA6:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,$B0,$B1,  0,  0,$B2,$B3,$B4
byte_52AB4:
		.BYTE	  3,  3,  0,  0,$B5,  0,  0,  0,$B6,$B7,  0,$B8,$B9,$BA
byte_52AC2:
		.BYTE	  3,  3,  0,  0,$B5,  0,  0,  0,$B6,$B7,  0,$BB,$BC,$BD
_spr6D_unused:
		.BYTE	  3,  3
		.BYTE	  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0
		.BYTE	  0,  0,$AE,$AF
_spr6E_unused:
		.BYTE	  3,  3
		.BYTE	  0,  0,  0,  0
		.BYTE	  0,$BE,$BF,  0
		.BYTE	  0,$B2,$C0,$B4
_spr6F_unused:
		.BYTE	  3,  3
		.BYTE	  0,  0,$B5,  0
		.BYTE	  0,  0,$C1,$C2
		.BYTE	  0,$B8,$B9,$BA
_spr70_unused:
		.BYTE	  3,  3
		.BYTE	  0,  0,$B5,  0
		.BYTE	  0,  0,$C1,$C2
		.BYTE	  0,$BB,$BC,$BD
byte_52B08:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$C3,$C4,  0,  0,  0,$C5,$C6
		.BYTE	  0,  0,  0,$C7,$C8,  0,  0,  0,$C9,$CA,  0,  0,  0,$CB,$CC,  0
byte_52B28:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CD,$CE,  0,  0,  0,$CF,$D0
		.BYTE	  0,  0,  0,$D1,$D2,  0,  0,  0,$D3,$D4,  0,  0,  0,$D5,$D6,$D7
byte_52B48:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$C3,$C4,  0,  0,  0,$D8,$D9
		.BYTE	  0,  0,  0,$DA,$DB,  0,  0,  0,$DC,$DD,  0,  0,  0,$CB,$DE,  0
byte_52B68:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CD,$CE,  0,  0,  0,$DF,$E0
		.BYTE	  0,  0,  0,$E1,$E2,  0,  0,  0,$E3,$E4,  0,  0,  0,$E5,$E6,$D7
byte_52B88:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CD,$CE,  0,  0,$E7,$E8,$E9
		.BYTE	  0,  0,  0,$EA,$EB,  0,  0,  0,$EC,$ED,  0,  0,  0,$D5,$D6,$D7
byte_52BA8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$CD,$EE,  0,  0,$E7,$E8,$EF
		.BYTE	  0,  0,$F0,$F1,$F2,  0,  0,$F3,$F4,$F5,  0,  0,  0,  0,$F6,  0
byte_52BC8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F7,$F8,  0,  0,  0,$F9,$C6
		.BYTE	  0,  0,  0,$C7,$C8,  0,  0,  0,$C9,$CA,  0,  0,  0,$CB,$CC,  0
byte_52BE8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$FA,$FB,  0,  0,  0,$FC,$D0
		.BYTE	  0,  0,  0,$D1,$D2,  0,  0,  0,$D3,$D4,  0,  0,  0,$D5,$D6,$D7
byte_52C08:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$F7,$F8,  0,  0,  0,$FD,$D9
		.BYTE	  0,  0,  0,$DA,$DB,  0,  0,  0,$DC,$DD,  0,  0,  0,$CB,$DE,  0
byte_52C28:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$FA,$FB,  0,  0,  0,$FE,$E0
		.BYTE	  0,  0,  0,$E1,$E2,  0,  0,  0,$E3,$E4,  0,  0,  0,$E5,$E6,$D7
byte_52C48:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$FA,$FB,  0,  0,$E7,$FF,$E9
		.BYTE	  0,  0,  0,$EA,$EB,  0,  0,  0,$EC,$ED,  0,  0,  0,$D5,$D6,$D7
byte_52C68:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$FA,$FB,  0,  0,$E7,$FF,$EF
		.BYTE	  0,  0,$F0,$F1,$F2,  0,  0,$F3,$F4,$F5,  0,  0,  0,  0,$F6,  0
byte_52C88:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$AE,$AF,  0,  0,  0,$B0,$B1
		.BYTE	  0,  0,  0,$B2,$B3,  0,  0,  0,$B4,$B5,  0,  0,  0,$B6,$B7,  0
byte_52CA8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$B8,$B9,  0,  0,  0,$BA,$BB
		.BYTE	  0,  0,  0,$BC,$BD,  0,  0,  0,$BE,$BF,  0,  0,  0,$C0,$C1,$C2
byte_52CC8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$AE,$AF,  0,  0,  0,$C3,$C4
		.BYTE	  0,  0,  0,$C5,$C6,  0,  0,  0,$C7,$C8,  0,  0,  0,$B6,$C9,  0
byte_52CE8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$B8,$B9,  0,  0,  0,$CA,$CB
		.BYTE	  0,  0,  0,$CC,$CD,  0,  0,  0,$CE,$CF,  0,  0,  0,$D0,$D1,$C2
byte_52D08:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$D2,$D3,$D4,  0,$D5,$D6,$D7
		.BYTE	$D8,  0,$D9,$DA,$DB,$DC,  0,  0,$BE,$DD,  0,  0,  0,$C0,$C1,$C2
byte_52D28:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,$DE,$DF,$E0,  0,  0,$E1,$E2,$E3
		.BYTE	  0,  0,$E4,$E5,$E6,  0,  0,  0,$BE,$DD,  0,  0,  0,$C0,$C1,$C2
byte_52D48:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$E7,$E8,  0,  0,  0,$E9,$EA
		.BYTE	  0,  0,  0,$EB,$EC,  0,  0,  0,$B4,$B5,  0,  0,  0,$B6,$B7,  0
byte_52D68:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$ED,$EE,  0,  0,  0,$EF,$F0
		.BYTE	  0,  0,  0,$F1,$F2,  0,  0,  0,$BE,$BF,  0,  0,  0,$C0,$C1,$C2
byte_52D88:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$E7,$E8,  0,  0,  0,$E9,$EA
		.BYTE	  0,  0,  0,$F3,$F4,  0,  0,  0,$C7,$C8,  0,  0,  0,$B6,$C9,  0
byte_52DA8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$ED,$EE,  0,  0,  0,$EF,$F0
		.BYTE	  0,  0,  0,$F5,$F6,  0,  0,  0,$CE,$CF,  0,  0,  0,$D0,$D1,$C2
byte_52DC8:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$A8,$A9
		.BYTE	  0,  0,$F7,$F8,$F9,  0,  0,  0,$FA,$FB,  0,  0,  0,$FC,$FD,$FE
byte_52DE8:
		.BYTE	  7,  7,  0,  0,$97,$98,$99,$9A,$9B,  0,  0,  0,$9C,$9D,$9E,$9F
		.BYTE	$A0,$A1,  0,  0,$A2,$A3,$A4,$A5,  0,  0,  0,  0,$A8,$A9,$AA,$AB
		.BYTE	  0,  0,  0,$AE,$AF,$B0,$B1,$B2,  0,  0,  0,  0,$B3,$B4,$B5,$B6
		.BYTE	  0,  0,  0,  0,$B7,$B8,$B9,$BA,$BB,  0
byte_52E22:
		.BYTE	  7,  7
		.BYTE	  0,  0,$97,$98,$99,$9A,$9B,  0
		.BYTE	  0,  0,$9C,$9D,$9E,$9F,$A0,$A1
		.BYTE	  0,  0,$A2,$A3,$A4,$A5,  0,  0
		.BYTE	  0,  0,$BC,$BD,$AA,$AB,  0,  0
		.BYTE	  0,  0,$BE,$BF,$C0,$B2,  0,  0
		.BYTE	  0,  0,$C1,$C2,$C3,$C4,  0,  0
		.BYTE	  0,  0,  0,$C5,$C6,$C7,  0,  0
byte_52E5C:
		.BYTE	  7,  7
		.BYTE	  0,  0,$97,$98,$99,$9A,$9B,  0
		.BYTE	  0,  0,$9C,$9D,$9E,$9F,$A0,$A1
		.BYTE	  0,  0,$A2,$A3,$A4,$A5,  0,  0
		.BYTE	  0,$C8,$C9,$CA,$AA,$AB,  0,  0
		.BYTE	  0,$CB,$CC,$CD,$CE,$B2,  0,  0
		.BYTE	  0,  0,$CF,$D0,$D1,$D2,  0,  0
		.BYTE	  0,  0,$D3,$D4,$D5,$D6,$D7,  0
byte_52E96:
		.BYTE	  7,  7
		.BYTE	  0,  0,$97,$98,$99,$9A,$9B,  0
		.BYTE	  0,  0,$9C,$9D,$9E,$9F,$A0,$A1
		.BYTE	  0,  0,$A2,$A3,$A4,$A5,  0,  0
		.BYTE	  0,  0,$D8,$D9,$AA,$AB,  0,  0
		.BYTE	  0,  0,$DA,$DB,$DC,$B2,  0,  0
		.BYTE	  0,  0,$DD,$DE,$DF,$E0,  0,  0
		.BYTE	  0,  0,  0,$E1,$E2,$E3,  0,  0
byte_52ED0:
		.BYTE	  2,  2,  0,$A6,$A7,  0,$AC,$AD
byte_52ED8:
		.BYTE	  2,  2,  0,$A6,$E4,  0,$AC,$E5
byte_52EE0:
		.BYTE	  2,  2,  0,$A6,$E6,  0,$AC,$E7
byte_52EE8:
		.BYTE	  2,  2,  0,$A6,$A7,  0,$E8,$E9
byte_52EF0:
		.BYTE	  2,  2,  0,$A6,$A7,  0,$EA,$EB
byte_52EF8:
		.BYTE	  1,  1,  0,$EC
byte_52EFC:
		.BYTE	  1,  1,  0,$ED
byte_52F00:
		.BYTE	  2,  1,  0,$EE,$EF
byte_52F05:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$18,$19,$1A,$1B,$FF,$1C,$1D,$1E
		.BYTE	$1F,  0,  0,$20,$21,$22,  0,  0,$23,$24,  0,  0,  0,$25,$26,$27
byte_52F25:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$28,$29,$2A,$1B,$FF,$2B,$2C,$2D
		.BYTE	$1F,  0,  0,$20,$21,$22,  0,  0,$23,$24,  0,  0,  0,$25,$26,$27
byte_52F45:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$18,$19,$1A,$1B,$FF,$1C,$1D,$1E
		.BYTE	$1F,  0,  0,$2E,$2F,$22,  0,  0,$30,$31,  0,  0,  0,$32,$33,  0
byte_52F65:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,$36,$37,$38,$39,$FF,$3A,$3B,$3C
		.BYTE	$3D,  0,  0,$3E,$3F,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_52F85:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$18,$19,$1A,$1B,$FF,$1C,$1D,$1E
		.BYTE	$1F,  0,  0,$47,$48,$22,  0,  0,$49,$4A,  0,  0,  0,$32,$4B,  0
byte_52FA5:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,$36,$37,$38,$39,$FF,$3A,$3B,$3C
		.BYTE	$3D,  0,  0,$4C,$4D,$40,  0,  0,$4E,$4F,$43,  0,  0,$50,$51,$52
byte_52FC5:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$28,$29,$2A,$1B,$FF,$2B,$53,$2D
		.BYTE	$1F,  0,  0,$2E,$2F,$22,  0,  0,$30,$31,  0,  0,  0,$32,$33,  0
byte_52FE5:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,$54,$55,$56,$39,$FF,$57,$58,$59
		.BYTE	$3D,  0,  0,$5A,$5B,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_53005:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,$FF,$28,$29,$2A,$1B,$FF,$2B,$2C,$2D
		.BYTE	$1F,  0,  0,$47,$48,$22,  0,  0,$49,$4A,  0,  0,  0,$32,$4B,  0
byte_53025:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,$54,$55,$56,$39,$FF,$57,$58,$59
		.BYTE	$3D,  0,  0,$5C,$5D,$40,  0,  0,$4E,$4F,$43,  0,  0,$50,$51,$52
byte_53045:
		.BYTE	  1,  2,  0,$5E,  0,$5F
byte_5304B:
		.BYTE	  1,  2,  0,$60,  0,$61
byte_53051:
		.BYTE	  1,  2,  0,$62,  0,$63
byte_53057:
		.BYTE	  1,  2,  0,$64,  0,$65
byte_5305D:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,$36,$37,$38,$39,$FF,$3A,$3B,$3C
		.BYTE	$3D,  0,  0,$3E,$3F,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_5307D:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$34,$35,  0,  0,$36,$37,$38
		.BYTE	$39,  0,$3A,$3B,$3C,$3D,  0,  0,$66,$67,$40,  0,  0,$68,$69,$6A
byte_5309D:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$34,$35
		.BYTE	  0,  0,$36,$37,$38,$39,  0,$3A,$3B,$3C,$3D,  0,  0,$6B,$6C,$6D
byte_530BD:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$34,$35
		.BYTE	  0,  0,$54,$55,$56,$39,  0,$57,$58,$59,$3D,  0,  0,$6B,$6C,$6D
byte_530DD:
		.BYTE	  4,  6,$FF,$6E,$34,$35,  0,$FF,$6F,$70,$38,$39,$FF,  0,$71,$72
		.BYTE	$3D,  0,  0,$73,$3F,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_530FD:
		.BYTE	  4,  6,$FF,$74,$34,$35,  0,$FF,$75,$76,$38,$39,$FF,  0,$77,$78
		.BYTE	$3D,  0,  0,$73,$3F,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_5311D:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,  0,$79,$7A,$39,$FF,$7B,$7C,$7D
		.BYTE	$7E,  0,$7F,$80,$81,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_5313D:
		.BYTE	  4,  6,$FF,  0,$34,$35,  0,$FF,  0,$79,$82,$83,$FF,$7B,$84,$85
		.BYTE	$86,  0,  0,$87,$88,$40,  0,  0,$41,$42,$43,  0,  0,$44,$45,$46
byte_5315D:
		.BYTE	  4,  6,$FF,  0,$16,$17,  0,  1,  0,$18,$19,  0,  1,$1A,$1B,$1C
		.BYTE	  0,  0,  0,$32,$33,  0,  1,  0,$43,$35,  0,  1,  0,  0,  0,  0
byte_5317D:
		.BYTE	  2,  1,  0,$89,$8A
byte_53182:
		.BYTE	  2,  2,  0,$8B,$8C,  0,$8D,$8E
byte_5318A:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,$98,$99,$9A
byte_53198:
		.BYTE	  3,  3,  0,  0,  0,  0,  0,$9B,$9C,$9D,  0,$9E,$9F,$A0
byte_531A6:
		.BYTE	  3,  3,  0,$A1,$A2,$A3,  0,$A4,$A5,$A6,  0,$A7,$A8,$A9
byte_531B4:
		.BYTE	  3,  3,  0,$A1,$A2,$A3,  0,$AA,$A5,$A6,  0,$AB,$AC,$AD
byte_531C2:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$AE,$AF,  0,  0,  0,$B0,$B1
		.BYTE	  0,  0,  0,$B2,$B3,  0,  0,  0,$B4,$B5,  0,  0,  0,$B6,$B7,  0
byte_531E2:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$B8,$B9,  0,  0,  0,$BA,$BB
		.BYTE	  0,  0,  0,$BC,$BD,  0,  0,  0,$BE,$BF,$C0,  0,  0,$C1,$C2,$C3
byte_53202:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$AE,$AF,  0,  0,  0,$C4,$C5
		.BYTE	  0,  0,  0,$C6,$C7,  0,  0,  0,$C8,$B5,  0,  0,  0,$C9,$B7,  0
byte_53222:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$B8,$B9,  0,  0,  0,$BA,$BB
		.BYTE	  0,  0,  0,$CA,$CB,  0,  0,  0,$BE,$BF,$C0,  0,  0,$C1,$C2,$C3
byte_53242:
		.BYTE	  4,  6
		.BYTE	  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0
		.BYTE	  0,  0,$CC,$CD,  0
		.BYTE	  0,$CE,$CF,$D0,  0
		.BYTE	  0,$D1,$D2,$D3,  0
		.BYTE	  0,$D4,$D5,$D6,$D7
byte_53262:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,$CC,$CD
		.BYTE	  0,  0,$D8,$D9,$DA,  0,  0,$D1,$DB,$D3,  0,  0,$D4,$D5,$D6,$D7
byte_53282:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$DC,$DD,  0,  0,  0,$B0,$B1
		.BYTE	  0,  0,  0,$B2,$B3,  0,  0,  0,$B4,$B5,  0,  0,  0,$B6,$B7,  0
byte_532A2:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$DE,$DF,  0,  0,  0,$E0,$BB
		.BYTE	  0,  0,  0,$BC,$BD,  0,  0,  0,$BE,$BF,$C0,  0,  0,$C1,$C2,$C3
byte_532C2:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$DC,$DD,  0,  0,  0,$C4,$C5
		.BYTE	  0,  0,  0,$C6,$C7,  0,  0,  0,$C8,$B5,  0,  0,  0,$C9,$B7,  0
byte_532E2:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$DE,$DF,  0,  0,  0,$E0,$BB
		.BYTE	  0,  0,  0,$CA,$CB,  0,  0,  0,$BE,$BF,$C0,  0,  0,$C1,$C2,$C3
byte_53302:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,  0,$E1,$E2,$E3,  0,  0,$E4,$E5
		.BYTE	$E6,  0,$E7,$E8,$E9,$EA,  0,  0,$BE,$BF,  0,  0,  0,$C1,$C2,$C3
byte_53322:
		.BYTE	  4,  6,  0,  0,  0,  0,  0,  0,$EB,$EC,$ED,  0,  0,$EE,$EF,$F0
		.BYTE	  0,  0,  0,$F1,$F2,  0,  0,  0,$F3,$F4,  0,  0,  0,$C1,$F5,$F6
_spr_bbox_x_list:
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,$63,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_spr_bbox_y_list:
		.BYTE	  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  8,$18,$18,  0,  0,  0,  0,  8,  8
		.BYTE	  0,  8,  0,  0,  0,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  8,  8,  8,  8,$10,  0,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  0,  0,  0,  0,  8,  8,  8,  8,  0,$10,  8,  8,  0,$10,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,$10,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  8,$18,$18,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0
		.BYTE	  0,  8,  8,  8,  8,$10,$10,  8,  8,  8,  8,  8,  8,$63,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_spr_bbox_width_list:
		.BYTE	  0,  8,$10,  4,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30
		.BYTE	$30,$30,$30,$30,$30,$30,$30,$28,$18,$18,$30,$30,$30,$30,$28,$28
		.BYTE	  0,$10,$18,$18,  0,$10,$18,$18,$28,$28,$28,$28,$28,$28,$28,$28
		.BYTE	$28,$28,$28,$28,  0,$18,$18,$18,  0,$18,$18,$18,  0,$18,$18,$18
		.BYTE	  0,$18,$18,$18,$28,$28,$28,$28,$20,  8,$28,$28,$28,$28,$28,$28
		.BYTE	$28,$18,$18,$18,$18,$28,$28,$28,$28,  0,$10,$18,$18,  0,$10,$18
		.BYTE	$18,$28,$28,$28,$28,$28,$28,$28,$28,$28,$28,$28,$28,$28,$28,$28
		.BYTE	$28,$28,$28,$28,$28,$28,$28,$20,$20,$20,$20,$20,  0,  0,  0,  0
		.BYTE	  0,  4,  4,  8,$30,$30,$30,$30,$30,$30,$30,$30,$30,$30,  0,  0
		.BYTE	  0,  0,$30,$28,$18,$18,$30,$30,$30,$30,$30,  8,$10,  0,$10,$18
		.BYTE	$28,$28,$28,$28,$28,$20,$20,$28,$28,$28,$28,$28,$28,$28,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
_spr_bbox_height_list:
		.BYTE	  0,$10,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10
		.BYTE	$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$20,$20
		.BYTE	  0,$18,$18,$18,  0,$18,$18,$18,$20,$20,$20,$20,$20,$20,$20,$20
		.BYTE	$20,$20,$20,$20,  0,$18,$18,$18,  0,$18,$18,$18,  0,$18,$18,$18
		.BYTE	  0,$18,$18,$18,$20,$20,$20,$20,$20,$18,$10,$10,$10,$10,$10,$10
		.BYTE	$10,$10,$10,$10,$10,$10,$10,$10,$10,  0,$10,$10,$10,  0,$10,$10
		.BYTE	$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10
		.BYTE	$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,  0,  0,  0,  0
		.BYTE	  0,  8,  8,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10
		.BYTE	$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,  0,$10,$10
		.BYTE	$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0

		ECHO "bank 6 free space", [SAFE_STUB_ORG-.]

		SAFE_STUB