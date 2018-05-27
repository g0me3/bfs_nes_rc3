
; --------------------------------
; SOUND ENGINE
; --------------------------------

; =============== S U B	R O U T	I N E =======================================
_apu_load_chnl:
		ASL
		STA		_apu_seqposit,X
		LDA		#1
		STA		_apu_stpindex,X
		LDA		#0
		STA		_apu_seqindex,X
		STA		_apu_spdcheck,X
		CPX		#2
		BNE		loc_50024
		LDA		#1
		STA		_apu_spdcount,X
		LDA		#0
		BEQ		loc_50027
loc_50024:
		STA		_apu_spdcount,X
loc_50027:
		STA		_apu_noteinit,X
		STA		_apu_tranbyte,X
		STA		_apu_stepnumb,X
		STA		_apu_ampindex,X
		STA		_apu_amplitud,X
		STA		_apu_sfxtrans,X
		STA		_apu_arptrans,X
		STA		_apu_continos,X
		LDA		#1
		STA		_apu_voictest,X
		STA		_apu_repstcnt,X
		STA		_apu_noteleng,X
		LDA		#$F
		STA		_APU_STATUS
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_play:
		LDX		#0
		JSR		_apu_play_chnl
		LDX		#1
		JSR		_apu_play_chnl
		LDX		#2
		JSR		_apu_play_chnl
		LDX		#3
		JSR		_apu_play_chnl
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_play_chnl:
		LDA		_apu_voictest,X
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
		ORA		_apu_amplitud,X
		STA		_APU_PULSE1_ENV,Y
		LDA		_apu_lofrqwrd,X
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
		LDA		_apu_pulsbyt1,X
		ORA		#$10
		ORA		#$20
loc_50096:
		ORA		_apu_amplitud,X
		STA		_APU_PULSE1_ENV,Y
		LDA		#8
		STA		_APU_PULSE1_SWEEP,Y
		LDA		_apu_lofrqwrd,X
		STA		_APU_PULSE1_TIME,Y
		LDA		_apu_hifrqwrd,X
		CMP		_apu_continos,X
		BEQ		loc_500B5
		STA		_APU_PULSE1_LEN,Y
		STA		_apu_continos,X
loc_500B5:
		LDA		#0
		STA		_apu_realplay,X
		DEC		_apu_spdcount,X
		BPL		loc_500E0
		LDA		_apu_spdcheck,X
		EOR		#$FF
		STA		_apu_spdcheck,X
		BPL		loc_500D2
		LDY		_apu_seqposit,X
		LDA		_ptr_seqtabes,Y
		JMP		loc_500D8
loc_500D2:
		LDY		_apu_seqposit,X
		LDA		_ptr_seqtabes+1,Y
loc_500D8:
		STA		_apu_spdcount,X
		LDA		#$FF
		STA		_apu_realplay,X
loc_500E0:
		LDA		_apu_realplay,X
		BPL		loc_500EA
		DEC		_apu_noteleng,X
		BEQ		loc_500FA
loc_500EA:
		LDA		_apu_realnote,X
		CMP		#$FF
		BEQ		loc_500F4
		JSR		_apu_fetch
loc_500F4:
		LDA		#0
		STA		_apu_noteinit,X
		RTS
loc_500FA:
		LDY		_apu_stepnumb,X
		LDA		_ptr_stptablo,Y
		STA		_apu_tmp0
		LDA		_ptr_stptabhi,Y
		STA		_apu_tmp0+1
		LDA		#0
		STA		_apu_gldspeed,X
		LDY		_apu_stpindex,X
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BEQ		loc_50118
		JMP		loc_5019D
loc_50118:
		DEC		_apu_repstcnt,X
		BEQ		loc_50128
		LDA		#0
		STA		_apu_stpindex,X
		TAY
		LDA		(_apu_tmp0),Y
		JMP		loc_5019D
loc_50128:
		LDY		_apu_seqposit,X
		LDA		_ptr_seqtabel,Y
		STA		_apu_tmp0
		LDA		_ptr_seqtabel+1,Y
		STA		_apu_tmp0+1
		LDY		_apu_seqindex,X
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BNE		loc_5014A
		INY
		LDA		(_apu_tmp0),Y
		STA		_apu_seqindex,X
		TAY
		LDA		(_apu_tmp0),Y
		JMP		loc_50158
loc_5014A:
		CMP		#$FE
		BNE		loc_50154
		LDA		#0
		STA		_apu_voictest,X
		RTS
loc_50154:
		CMP		#$FC
		BNE		loc_50158
loc_50158:
		CMP		#$80
		BCC		loc_50166
		AND		#$7F
		STA		_apu_tranbyte,X
		INY
		LDA		(_apu_tmp0),Y
		BMI		loc_5018E
loc_50166:
		CMP		#$70
		BCC		loc_50172
		AND		#$F
		STA		_apu_sfxtrans,X
		INY
		LDA		(_apu_tmp0),Y
loc_50172:
		CMP		#$60
		BCC		loc_5017E
		AND		#$F
		STA		_apu_repstcnt,X
		INY
		LDA		(_apu_tmp0),Y
loc_5017E:
		CMP		#$50
		BCC		loc_5018A
		AND		#$F
		STA		_apu_arptrans,X
		INY
		LDA		(_apu_tmp0),Y
loc_5018A:
		STA		_apu_stepnumb,X
		INY
loc_5018E:
		TYA
		STA		_apu_seqindex,X
		INC		_apu_repstcnt,X
		LDA		#0
		STA		_apu_stpindex,X
		JMP		loc_500FA
loc_5019D:
		CMP		#$FD
		BNE		loc_501BF
		INY
		LDA		(_apu_tmp0),Y
		STA		_apu_gldspeed,X
		INY
		LDA		(_apu_tmp0),Y
		CLC
		ADC		_apu_tranbyte,X
		STA		_apu_realnote,X
		INY
		LDA		(_apu_tmp0),Y
		CLC
		ADC		_apu_tranbyte,X
		STA		_apu_destnote,X
		INY
		JMP		loc_50222
loc_501BF:
		CMP		#$E0
		BCC		loc_501DA
		AND		#$1F
		STA		_apu_realleng,X
		LDA		#$FF
		STA		_apu_realnote,X
		LDA		#0
		STA		_apu_amplitud,X
		INY
		TYA
		STA		_apu_stpindex,X
		JMP		loc_5027C
loc_501DA:
		CMP		#$C0
		BCC		loc_501F2
		CLC
		ADC		_apu_sfxtrans,X
		ASL
		ASL
		STA		_apu_realsoun,X
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
		STA		_apu_realleng,X
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
		ADC		_apu_arptrans,X
		STA		_apu_arpnumbo,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$60
		BCS		loc_50222
loc_5021A:
		CLC
		ADC		_apu_tranbyte,X
		STA		_apu_realnote,X
		INY
loc_50222:
		TYA
		STA		_apu_stpindex,X
		INC		_apu_noteinit,X
		LDY		_apu_realsoun,X
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
		LDA		_apu_soundfix,Y
		AND		#8
		BNE		loc_5024C
		LDA		_apu_soundfix,Y
		AND		#$C0
		STA		_apu_pulsbyt1,X
loc_5024C:
		LDA		_apu_soundfix+1,Y
		STA		_apu_fx01,X
		LDA		_apu_soundfix+2,Y
		STA		_apu_fx02,X
		LDA		_apu_soundfix+3,Y
		STA		_apu_fx03,X
		CPX		#3
		BEQ		loc_50276
		LDY		_apu_realnote,X
		LDA		_apu_frqtablo,Y
		STA		_apu_lofrqwrd,X
		LDA		#$F8
		ORA		_apu_frqtabhi,Y
		STA		_apu_hifrqwrd,X
		JMP		loc_5027C
loc_50276:
		LDA		_apu_realnote,X
		STA		_apu_lofrqwrd,X
loc_5027C:
		LDA		_apu_realleng,X
		STA		_apu_noteleng,X
		RTS

; =============== S U B	R O U T	I N E =======================================
_apu_fetch:
		LDA		_apu_noteinit,X
		BNE		loc_5028B
		JMP		loc_502E0
loc_5028B:
		LDA		_apu_gldspeed,X
		BEQ		loc_502B2
		TAY
		LSR
		LSR
		LSR
		LSR
		STA		_apu_glddelay,X
		TYA
		AND		#$F
		STA		_apu_gldspeed,X
		LDA		_apu_realleng,X
		SEC
		SBC		_apu_glddelay,X
		STA		_apu_glddelay,X
		LDA		_apu_realnote,X
		SEC
		SBC		_apu_destnote,X
		STA		_apu_glddirec,X
loc_502B2:
		LDA		#0
		STA		_apu_ampindex,X
		STA		_apu_pulindex,X
		STA		_apu_vibindex,X
		STA		_apu_arpindex,X
		LDA		_apu_fx02,X
		LSR
		LSR
		LSR
		LSR
		STA		_apu_vibdelay,X
		LDA		_apu_realleng,X
		SEC
		SBC		_apu_vibdelay,X
		BCS		loc_502DA
		LDA		#0
		STA		_apu_fx01,X
		BEQ		loc_502DD
loc_502DA:
		STA		_apu_vibdelay,X
loc_502DD:
		JMP		loc_503B4
loc_502E0:
		LDA		_apu_fx03,X
		AND		#$7F
		TAY
		LDA		_ptr_amptabl,Y
		STA		_apu_tmp0
		LDA		_ptr_amptabh,Y
		STA		_apu_tmp0+1
		LDY		_apu_ampindex,X
		LDA		(_apu_tmp0),Y
		CMP		#$FE
		BEQ		loc_502FF
		STA		_apu_amplitud,X
		INC		_apu_ampindex,X
loc_502FF:
		LDY		_apu_gldspeed,X
		BEQ		loc_5035B
		LDA		_apu_glddelay,X
		CMP		_apu_noteleng,X
		BCC		loc_5035B
		LDY		_apu_glddirec,X
		BMI		loc_5032B
		LDA		_apu_lofrqwrd,X
		CLC
		ADC		_apu_gldspeed,X
		STA		_apu_lofrqwrd,X
		BCC		loc_50320
		INC		_apu_hifrqwrd,X
loc_50320:
		LDY		_apu_destnote,X
		SEC
		SBC		_apu_frqtablo,Y
		BCC		loc_50358
		BCS		loc_50343
loc_5032B:
		LDA		_apu_lofrqwrd,X
		SEC
		SBC		_apu_gldspeed,X
		STA		_apu_lofrqwrd,X
		BCS		loc_5033A
		DEC		_apu_hifrqwrd,X
loc_5033A:
		LDY		_apu_destnote,X
		SEC
		SBC		_apu_frqtablo,Y
		BCS		loc_50358
loc_50343:
		TYA
		STA		_apu_realnote,X
		LDA		_apu_frqtablo,Y
		STA		_apu_lofrqwrd,X
		LDA		_apu_frqtabhi,Y
		STA		_apu_hifrqwrd,X
		LDA		#0
		STA		_apu_gldspeed,X
loc_50358:
		JMP		loc_503B4
loc_5035B:
		LDA		_apu_fx01,X
		AND		#$F
		BEQ		loc_503B4
		LDA		_apu_vibdelay,X
		CMP		_apu_noteleng,X
		BCC		loc_503B4
		LDA		_apu_fx01,X
		AND		#$F
		TAY
		LDA		_ptr_vibtabl-1,Y
		STA		_apu_tmp0
		LDA		_ptr_vibtabh-1,Y
		STA		_apu_tmp0+1
		LDY		_apu_vibindex,X
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
		LDA		_apu_lofrqwrd,X
		SEC
		SBC		_apu_tmp0
		STA		_apu_lofrqwrd,X
		BCS		loc_5039E
		DEC		_apu_hifrqwrd,X
loc_5039E:
		JMP		loc_503AF
loc_503A1:
		LDA		_apu_lofrqwrd,X
		CLC
		ADC		(_apu_tmp0),Y
		STA		_apu_lofrqwrd,X
		BCC		loc_503AF
		INC		_apu_hifrqwrd,X
loc_503AF:
		INY
		TYA
		STA		_apu_vibindex,X
loc_503B4:
		CPX		#2
		BCS		loc_503E5
		LDA		_apu_fx02,X
		AND		#$F
		BEQ		loc_503E5
		TAY
		LDA		_ptr_pultabl-1,Y
		STA		_apu_tmp0
		LDA		_ptr_pultabh-1,Y
		STA		_apu_tmp0+1
		LDY		_apu_pulindex,X
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
		STA		_apu_pulsbyt1,X
		INY
		TYA
		STA		_apu_pulindex,X
loc_503E5:
		LDA		_apu_fx01,X
		AND		#$40
		BEQ		locret_5045D
		LDY		_apu_arpnumbo,X
		LDA		_ptr_arptablo,Y
		STA		_apu_tmp0
		LDA		_ptr_arptabhi,Y
		STA		_apu_tmp0+1
		LDY		_apu_arpindex,X
		BNE		loc_50407
		LDA		(_apu_tmp0),Y
		STA		_apu_arpspeed,X
		INY
		JMP		loc_5041C
loc_50407:
		LDA		_apu_arpspeed,X
		SEC
		SBC		#$10
		STA		_apu_arpspeed,X
		BPL		loc_50443
		LDY		#0
		LDA		(_apu_tmp0),Y
		STA		_apu_arpspeed,X
		LDY		_apu_arpindex,X
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
		ADC		_apu_realnote,X
		STA		_apu_arponoto,X
loc_50439:
		AND		#$7F
		STA		_apu_arponoto,X
		INY
		TYA
		STA		_apu_arpindex,X
loc_50443:
		LDA		_apu_arponoto,X
		CPX		#3
		BNE		loc_50450
		STA		_apu_lofrqwrd,X
		JMP		locret_5045D
loc_50450:
		TAY
		LDA		_apu_frqtablo,Y
		STA		_apu_lofrqwrd,X
		LDA		_apu_frqtabhi,Y
		STA		_apu_hifrqwrd,X
locret_5045D:
		RTS

_apu_frqtablo:
		.BYTE	$AD,$4A,$EF,$9B,$4B,$FE,$B5,$73,$30,$F5,$BD,$87 ;1
		.BYTE	$56,$25,$F8,$CD,$A5,$7F,$5B,$3A,$19,$FB,$DE,$C3 ;2
		.BYTE	$AA,$92,$7B,$66,$52,$3F,$2D,$1C, $C,$FD,$EF,$E1 ;3
		.BYTE	$D4,$C9,$BD,$B3,$A9,$9F,$96,$8E,$86,$7E,$77,$70 ;4
		.BYTE	$6A,$64,$5E,$59,$54,$4F,$4A,$47,$42,$3E,$3B,$38 ;5
		.BYTE	$35,$31,$2E,$2C,$29,$27,$25,$23,$21,$1F,$1D,$1B ;6
		.BYTE	$1A,$19,$18,$16,$15,$14,$13,$12,$11,$10, $F, $E ;7
		.BYTE	$FF

_apu_frqtabhi:
		.BYTE	  6,  6,  5,  5,  5,  4,  4,  4,  4,  3,  3,  3 ;1
		.BYTE	  3,  3,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1 ;2
		.BYTE	  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 ;3
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 ;4
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 ;5
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 ;6
		.BYTE	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 ;7
		.BYTE	  7

_ptr_seqtabel:
		.WORD	_seq00
		.WORD	_seq01
		.WORD	_seq02
		.WORD	_seq03
		.WORD	_seq04
		.WORD	_seq05
		.WORD	_seq06
		.WORD	_seq07
		.WORD	_seq08
		.WORD	_seq09
		.WORD	_seq0A
		.WORD	_seq0B
		.WORD	_seq0C
		.WORD	_seq0D
		.WORD	_seq0E
		.WORD	_seq0F
		.WORD	_seq10
		.WORD	_seq11
		.WORD	_seq12
		.WORD	_seq13
		.WORD	_seq14
		.WORD	_seq15
		.WORD	_seq16
		.WORD	_seq17
		.WORD	_seq18
		.WORD	_seq19
		.WORD	_seq1A
		.WORD	_seq1B
		.WORD	_seq1C
		.WORD	_seq1D
		.WORD	_seq1E
		.WORD	_seq1F
		.WORD	_seq20
		.WORD	_seq21
		.WORD	_seq22
		.WORD	_seq23
		.WORD	_seq24
		.WORD	_seq25
		.WORD	_seq26
		.WORD	_seq27
		.WORD	_seq28
		.WORD	_seq29
		.WORD	_seq2A
		.WORD	_seq2B
		.WORD	_seq2C
		.WORD	_seq2D
		.WORD	_seq2E
		.WORD	_seq2F
		.WORD	_seq30
		.WORD	_seq31
		.WORD	_seq32
		.WORD	_seq33
		.WORD	_seq34
		.WORD	_seq35
		.WORD	_seq36
		.WORD	_seq37
		.WORD	_seq38
		.WORD	_seq39
		.WORD	_seq3A
		.WORD	_seq3B

_ptr_seqtabes:
		.BYTE	  0,  0
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	$7F,$7F
		.BYTE	  2,  2
		.BYTE	  3,  3
		.BYTE	  4,  4
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  3,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  4,  4
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  2,  2
		.BYTE	  0,  0
		.BYTE	  0,  0
		.BYTE	  1,  1
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  3,  3
		.BYTE	  2,  2

_ptr_stptablo:
		.BYTE	<s00,<s01,<s02,<s03,<s04,<s05,<s06,<s07,<s08,<s09,<s0A,<s0B,<s0C,<s0D,<s0E,<s0F
		.BYTE	<s10,<s11,<s12,<s13,<s14,<s15,<s16,<s17,<s18,<s19,<s1A,<s1B,<s1C,<s1D,<s1E,<s1F
		.BYTE	<s20,<s21,<s22,<s23,<s24,<s25,<s26,<s27,<s28,<s29,<s2A,<s2B,<s2C,<s2D,<s2E,<s2F
		.BYTE	<s30,<s31,<s32,<s33,<s34,<s35,<s36,<s37,<s38,<s39,<s3A,<s3B,<s3C,<s3D,<s3E,<s3F
		.BYTE	<s40,<s41,<s42,<s43
_ptr_stptabhi:
		.BYTE	>s00,>s01,>s02,>s03,>s04,>s05,>s06,>s07,>s08,>s09,>s0A,>s0B,>s0C,>s0D,>s0E,>s0F
		.BYTE	>s10,>s11,>s12,>s13,>s14,>s15,>s16,>s17,>s18,>s19,>s1A,>s1B,>s1C,>s1D,>s1E,>s1F
		.BYTE	>s20,>s21,>s22,>s23,>s24,>s25,>s26,>s27,>s28,>s29,>s2A,>s2B,>s2C,>s2D,>s2E,>s2F
		.BYTE	>s30,>s31,>s32,>s33,>s34,>s35,>s36,>s37,>s38,>s39,>s3A,>s3B,>s3C,>s3D,>s3E,>s3F
		.BYTE	>s40,>s41,>s42,>s43

_ptr_amptabl:
		.BYTE	<am00,<am01,<am02,<am03,<am04,<am05,<am06,<am07,<am08,<am09,<am0A,<am0B,<am0C,<am0D,<am0E,<am0F
		.BYTE	<am10,<am11,<am12,<am13,<am14,<am15,<am16,<am17
_ptr_amptabh:
		.BYTE	>am00,>am01,>am02,>am03,>am04,>am05,>am06,>am07,>am08,>am09,>am0A,>am0B,>am0C,>am0D,>am0E,>am0F
		.BYTE	>am10,>am11,>am12,>am13,>am14,>am15,>am16,>am17

_ptr_vibtabl:
		.BYTE	<v00,<v01,<v02,<v03
_ptr_vibtabh:
		.BYTE	>v00,>v01,>v02,>v03

_ptr_pultabl:
		.BYTE	<p00,<p01,<p02,<p03,<p04,<p05
_ptr_pultabh:
		.BYTE	>p00,>p01,>p02,>p03,>p04,>p05

_ptr_arptablo:
		.BYTE	<a00,<a01,<a02,<a03,<a04,<a05,<a06,<a07,<a08,<a09,<a0A,<a0B,<a0C,<a0D,<a0E,<a0F
		.BYTE	<a10,<a11,<a12,<a13,<a14,<a15,<a16,<a17,<a18,<a19,<a1A,<a1B,<a1C,<a1D,<a1E,<a1F
		.BYTE	<a20,<a21
_ptr_arptabhi:
		.BYTE	>a00,>a01,>a02,>a03,>a04,>a05,>a06,>a07,>a08,>a09,>a0A,>a0B,>a0C,>a0D,>a0E,>a0F
		.BYTE	>a10,>a11,>a12,>a13,>a14,>a15,>a16,>a17,>a18,>a19,>a1A,>a1B,>a1C,>a1D,>a1E,>a1F
		.BYTE	>a20,>a21

am00:
		.BYTE	 $A,  9,  8,  7,  7,  6,  6,  6,  5,  5,  5,  4,  4,  4,  3,  3
		.BYTE	  2,  2,  1,  1,  0,$FE
am01:
		.BYTE	  1,  2,  3,  4,  5,  6,  6,  7,  7,  8,  8,  9,  9,  9,  9,  9
		.BYTE	  9,  8,  8,  8,  7,  7,  7,  6,  6,  6,  5,  5,  5,  4,  4,  4
		.BYTE	  3,$FE
am02:
		.BYTE	  1,  2,  3,  4,  5,  6,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8
		.BYTE	  8,  8,  8,  7,  7,  7,  7,  6,  6,  6,  5,  5,  4,  4,  3,  3
		.BYTE	  2,  2,  1,$FE
am03:
		.BYTE	 $F, $E, $D, $D, $C, $C, $C, $B, $B, $B, $A, $A, $A, $A,  9,  9
		.BYTE	  9,  9,  9,  8,  8,  8,  8,  8,  7,  7,  7,  7,  7,  7,  7,  7
		.BYTE	  6,  6,  6,  6,  6,  6,  6,  6,  5,  5,  5,  5,  5,  5,  5,  5
		.BYTE	  4,  4,  4,  4,  3,  3,  3,  2,  2,  2,  1,$FE
am04:
		.BYTE	 $A,  9,  8,  8,  7,  7,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4
		.BYTE	  4,  4,  3,  3,  3,  3,  3,  2,  2,  2,  2,  1,$FE
am05:
		.BYTE	 $C, $B, $A, $A,  9,  9,  8,  8,  8,  7,  7,  7,  7,  6,  6,  6
		.BYTE	  6,  5,  5,  5,  5,  4,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2
		.BYTE	  2,  1,$FE
am06:
		.BYTE	  7,  7,  6,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  4,  4,  4
		.BYTE	  4,  4,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2
		.BYTE	  2,  2,  1,$FE
am07:
		.BYTE	  4,$FE
am08:
		.BYTE	  8,  7,  6,  5,  4,  4,  3,  3,  3,  2,  2,  2,  1,$FE
am09:
		.BYTE	 $F, $A,  9,  8,  7,  6,  5,  5,  4,  4,  4,  3,  3,  3,  3,  2
		.BYTE	$FE
am0A:
		.BYTE	  6,  3,  2,  2,  1,$FE
am0B:
		.BYTE	  1,$FE
am0C:
		.BYTE	 $A,  6,  5,  4,  4,  3,  3,  2,$FE
am0D:
		.BYTE	 $F, $C, $B, $A,  9,  8,  8,  7,  6,  5,  4,  3,  2,$FE
am0E:
		.BYTE	 $F, $C,  9,  7,  6,  5,$FE
am0F:
		.BYTE	  8,  6,  5,  4,  3,$FE
am10:
		.BYTE	  2,$FE
am11:
		.BYTE	  1,$FE
am12:
		.BYTE	 $A,  6,  4,  4,  3,  3,  3,  2,$FE
am13:
		.BYTE	  5,  4,  3,  3,  2,$FE
am14:
		.BYTE	  4,$FE
am15:
		.BYTE	  1,  2,  3,  4,  4,  5,  5,  5,  4,  3,  2,  2,  1,$FE
am16:
		.BYTE	  1,  3,  5,  7,  9, $B, $D, $F, $D, $B,  9,  7,  5,  3,  1,  0
		.BYTE	$FE
am17:
		.BYTE	  8,$FE
v00:
		.BYTE	  1,  1,  0,$FF,$FF,  0,$FF,$FF,  0,  1,  1,  0,$80
v01:
		.BYTE	  4,  4,  4,$FC,$FC,$FC,$FC,$FC,$FC,  4,  4,  4,$80
v02:
		.BYTE	  2,  1,  1,$FE,$FF,$FF,$FE,$FF,$FF,  2,  1,  1,$80
v03:
		.BYTE	  1,  0,$FF,  0,$80
p00:
		.BYTE	  0,  0,  0,  0,  0,  0,$40,$40,$40,$40,$40,$40,$80,$80,$80,$80
		.BYTE	$80,$80,$C0,$C0,$C0,$C0,$C0,$C0,$80,$80,$80,$80,$80,$80,$40,$40
		.BYTE	$40,$40,$40,$40,$FF,  0
p01:
		.BYTE	  0,$40,$80,$80,$C0,$C0,$80,$80,$80,$80,$80,$80,$40,$40,$40,$40
		.BYTE	$40,$40,$80,$80,$80,$80,$80,$80,$C0,$C0,$C0,$C0,$C0,$C0,$FF,  6
p02:
		.BYTE	  0,  0,$40,$40,$40,$40,$80,$80,$80,$80,$80,$80,$80,$80,$C0,$C0
		.BYTE	$C0,$C0,$C0,$C0,$C0,$80,$80,$80,$80,$80,$80,$80,$40,$40,$40,$40
		.BYTE	$40,$40,$40,$80,$80,$80,$80,$80,$80,$80,$FF, $E
p03:
		.BYTE	  0,  0,$40,$40,$40,$40,$80,$80,$80,$80,$C0,$C0,$C0,$C0,$80,$80
		.BYTE	$80,$80,$40,$40,$40,$40,  0,  0,$FF,  0
p04:
		.BYTE	  0,  0,  0,$40,$FE
p05:
		.BYTE	$80,$80,$80,$40,$40,$40,  0,  0,  0,$40,$40,$40,$80,$FE
a00:
		.BYTE	 $C,$18,$16,$14,$12,$10, $E, $C, $B, $A,  9,  8,  7,  6,  5,  4
		.BYTE	  3,  2,  1,  0,$FF
a01:
		.BYTE	$10,  4,  6,  7,  6,  5,  4,$FE
a02:
		.BYTE	$10, $F, $E, $D, $C, $B, $A,  9,  8,  7,  6,  6,  5,$FE
a03:
		.BYTE	$10, $F, $E, $D, $C, $B, $A,  9,  8,  7,  6,  7,  6,  7,  6,  7
		.BYTE	  6,  7,  6,  7,  6,  7,  8,  9,  9, $A, $A, $B, $B, $A, $A,  9
		.BYTE	  8,  7,  6,  5,  4,  3,  2,  1,  0,$FE
a04:
		.BYTE	  0,$12, $F, $E,$11, $E, $D,$10, $D, $C, $F, $C, $B, $E, $B, $A
		.BYTE	 $D, $A,  9, $C,  9,  8, $B,  8,  7, $A,  7,  6,  9,  6,  5,  8
		.BYTE	  5,  4,  7,  4,  3,  6,  3,  2,  5,  2,  1,  4,  1,  0,  3,  1
		.BYTE	  0,  2,  1,  0,$FE
a05:
		.BYTE	  0,  8,  9, $A, $B, $C, $D, $E, $F, $F, $E, $E, $E, $D,$FE
a06:
		.BYTE	  0,  2,  0,  1,  2,  0,  1,  0,  2,  1,  2,  0,  1,$FF
a07:
		.BYTE	$20,  0,  3,  7, $C,  7,  5,$FF
a08:
		.BYTE	$30,  0,  2,  5,  7, $A, $C,$FF
a09:
		.BYTE	  0,  8,  0,  6,  2,  9,  4,  7,  1,$FF
a0A:
		.BYTE	  7,$18,$15,$12, $F, $C,  9,  7,  6,  5,  4,  3,  2,  1,  0,$FF
a0B:
		.BYTE	  0,$9A,$98,$96,$94,  0,$FE
a0C:
		.BYTE	  0,$A2,$A0,$9E,$9C,  0,$FE
a0D:
		.BYTE	  0,  0,$FE
a0E:
		.BYTE	  0,  1,  0,$FF
a0F:
		.BYTE	  3,  0,  2,  7,  0,  0,  2,  2,  7,  7,$FF
a10:
		.BYTE	  3,  0,  3,  7,  0,  0,  3,  3,  7,  7,$FF
a11:
		.BYTE	  3,  0,  4,  7,  0,  0,  4,  4,  7,  7,$FF
a12:
		.BYTE	  3,  0,  5,  8,  0,  0,  5,  5,  8,  8,$FF
a13:
		.BYTE	  3,  0,  5,  9,  0,  0,  5,  5,  9,  9,$FF
a14:
		.BYTE	  3,  0,  3,  8,  0,  0,  3,  3,  8,  8,$FF
a15:
		.BYTE	  3,  0,  4,  9,  0,  0,  4,  4,  9,  9,$FF
a16:
		.BYTE	  4,  0,  2,  7, $C,  0,  0,  2,  2,  7,  7, $C, $C,$FF
a17:
		.BYTE	$20,  0,  5,$FF
a18:
		.BYTE	$20,  0,  7,$FF
a19:
		.BYTE	$20,  0,  9,$FF
a1A:
		.BYTE	$20,  0,  8,$FF
a1B:
		.BYTE	$20,  0,  4,$FF
a1C:
		.BYTE	$20,  0,  3,$FF
a1D:
		.BYTE	  3,  0,  2,  8,  0,  0,  2,  2,  8,  8,$FF
a1E:
		.BYTE	  3,  0,  3,  6,  0,  0,  3,  3,  6,  6,$FF
a1F:
		.BYTE	  3,  0,  5,  7,  0,  0,  5,  5,  7,  7,$FF
a20:
		.BYTE	$10,  0,  5,  3,  8,$FF
a21:
		.BYTE	$10,  0,  7,  5,  9,$FF

_apu_soundfix:
		.BYTE	$80,$40,  0,  0	;C0 SingShot (1)
		.BYTE	$80,$40,  0,  3	;C1 LaunchMs (4)
		.BYTE	$80,$40,  0,  2	;C2 LaunchBg (4)
		.BYTE	$80,$40,  0,  3	;C3 ScrmExpl (4)
		.BYTE	$80,$40,  0,  4	;C4 Explosis (4)
		.BYTE	$80,$40,  0,  5	;C5 Explosim (4)
		.BYTE	  0,$40,  1,  6	;C6 Pickupos (1)
		.BYTE	  0,  0,  0,  7	;C7 Jetpacks (4)
		.BYTE	  0,$40,  0,  8	;C8 RoboFall (1)
		.BYTE	  0,$40,  1,  9	;C9 Titlearp (1)
		.BYTE	  0,$40,  2, $A	;CA Titlea-1 (1)
		.BYTE	  0,$40,  0, $B	;CB Titlebas (3)
		.BYTE	  0,  0,  0, $C	;CC Percussi (4)
		.BYTE	  0,$40,  0, $D	;CD PERCUSSI (4)
		.BYTE	  0,  1,$43, $E	;CE LEADERS! (1)
		.BYTE	  0,  1,$43, $F	;CF LEADER-1 (1)
		.BYTE	  0,$40,  3, $E	;D0 LEADARP! (1)
		.BYTE	  0,$40,  3, $F	;D1 LEADAR-1 (1)
		.BYTE	  0,$40,  0,$10	;D2 softsoft (1)
		.BYTE	  0,$40,  0, $B	;D3 softso-1 (1)
		.BYTE	$80,  1,$60,$10	;D4 HARDintr (1)
		.BYTE	$80,  1,$50,$11	;D5 softintr (1)
		.BYTE	$C0,  1,$60,$10	;D6 HARDpuls (1)
		.BYTE	$C0,  1,$50,$11	;D7 softpuls (1)
		.BYTE	  0,$42,  0, $B	;D8 BasNorml (3)
		.BYTE	  0,$40,  4,$12	;D9 ING-ARPS (2)
		.BYTE	  0,  0,  5,$10	;DA ING-BA-1 (2)
		.BYTE	  0,  0,  5,$12	;DB ING-BASS (2)
		.BYTE	  0,  3,$15,$12	;DC ING-BAVI (2)
		.BYTE	$80,  1,$60,$13	;DD HARDintr (1)
		.BYTE	$C0,  1,$60,$14	;DE OPDIEHRD (1)
		.BYTE	$C0,  1,$50,$10	;DF OPDIEHRD (1)
		.BYTE	  0,$42,$20, $B	;E0 BasVIBOS (3)
		.BYTE	  0,  0,  0,$15
		.BYTE	$80,  4,  6,$16
		.BYTE	$80,  0,  0,$17

_seq01:
		.BYTE	  1
		.BYTE	$FE

_seq02:
		.BYTE	  2
		.BYTE	$FE

_seq26:
		.BYTE	$85,  1
		.BYTE	$FE

_seq27:
		.BYTE	$85,  2
		.BYTE	$FE

s01:
		.BYTE	$C0,$8C,$60,  0
		.BYTE	$FF
s02:
		.BYTE	$C0,$8C,$61,  0
		.BYTE	$FF

_seq03:
		.BYTE	  3
		.BYTE	$FE
s03:
		.BYTE	$C1,$98,$62,  0,$E1
		.BYTE	$FF

_seq04:
		.BYTE	  4
		.BYTE	$FE
s04:
		.BYTE	$C2,$98,$63,  0,$E1
		.BYTE	$FF

_seq05:
		.BYTE	  5
		.BYTE	$FE

_seq06:
		.BYTE	  6
		.BYTE	$FE
s05:
		.BYTE	$C3,$8E,$64,  8,$E1
		.BYTE	$FF
s06:
		.BYTE	$C3,$97,$65,  0,$E1
		.BYTE	$FF

_seq07:
		.BYTE	  7
		.BYTE	$FE

_seq08:
		.BYTE	  8
		.BYTE	$FE

s07:
		.BYTE	$C4,$86,$64,  4,$E1
		.BYTE	$FF
s08:
		.BYTE	$C4,$8C,$66, $B,$E1
		.BYTE	$FF

_seq09:
		.BYTE	  9
		.BYTE	$FE

_seq0A:
		.BYTE	 $A
		.BYTE	$FE

s09:
		.BYTE	$C5,$89,$64,  5,$E1
		.BYTE	$FF
s0A:
		.BYTE	$C5,$90,$66, $C,$E1
		.BYTE	$FF

_seq0B:
		.BYTE	 $B
		.BYTE	$FE

_seq0C:
		.BYTE	 $C
		.BYTE	$FE

s0B:
		.BYTE	$C3,$8C,$64,  6,$E1
		.BYTE	$FF
s0C:
		.BYTE	$C3,$97,$66, $D,$E1
		.BYTE	$FF

_seq0D:
		.BYTE	 $D
		.BYTE	$FE

_seq0E:
		.BYTE	 $E
		.BYTE	$FE

s0D:
		.BYTE	$C6,$90,$67,$18,$E1
		.BYTE	$FF
s0E:
		.BYTE	$E1,$C6,$90,$67,$18,$E1
		.BYTE	$FF

_seq0F:
		.BYTE	 $F
		.BYTE	$FE

_seq10:
		.BYTE	$10
		.BYTE	$FE

s0F:
		.BYTE	$C6,$90,$68,$1C,$E1
		.BYTE	$FF
s10:
		.BYTE	$E1,$C6,$90,$68,$1C,$E1
		.BYTE	$FF

_seq11:
		.BYTE	$11
		.BYTE	$FE

s11:
		.BYTE	$C7,$BF,  9
		.BYTE	$FF

_seq12:
		.BYTE	$12
		.BYTE	$FE

_seq13:
		.BYTE	$13
		.BYTE	$FE

_seq14:
		.BYTE	$14
		.BYTE	$FE

_seq28:
		.BYTE	$F7,$13
		.BYTE	$FE

_seq29:
		.BYTE	$81,  8
		.BYTE	$FE

s12:
		.BYTE	$C8,$85,$69,$4C,$E1
		.BYTE	$FF
s13:
		.BYTE	$C3,$83,$6A,  8,$E1
		.BYTE	$FF
s14:
		.BYTE	$C8,$84,$66,  8,$E1
		.BYTE	$FF

		echo "music data start", .

_seq15:
		.BYTE	$85,$22,$25			;A
		.BYTE	$85,$19,$83,$81,$83		;A
		.BYTE	$85,$83,$81,$83			;A
		.BYTE	$85,$83,$81,$83			;A
		.BYTE	$85,$83,$81,$83			;A
		.BYTE	$85,$83,$81,$83		;B
		.BYTE	$85,$83,$81,$80,$1F		;B
		.BYTE	$85,$19,$83,$81,$83		;A
		.BYTE	$85,$83,$81,$83			;A
		.BYTE	$85,$83,$81,$83		;C
		.BYTE	$85,$83,$81,$83		;C
		.BYTE	$85,$83,$81,$80,$1F		;C
		.BYTE	$85,$19,$83,$81,$80,$1F		;C
		.BYTE	$85,$19,$83,$81,$83		;A
		.BYTE	$85,$83,$81,$83			;A
		.BYTE	$FF,  3

_seq16:
		.BYTE	$85,$71,$23,$22,$91,$19,$8F,$8D,$8F,$91,$8F,$8D,$8F,$91,$8F,$8D
		.BYTE	$8F,$91,$8F,$8D,$8F,$91,$70,$1A,$1D,$91,$19,$8F,$8D,$8F,$91,$8F
		.BYTE	$8D,$8F,$91,$20,$21,$26,$26,$91,$67,$19,$FF,  4

_seq17:
		.BYTE	$24,$69,$18,$27,$27,$6F,$16,$16,$16,$1B,$1B,$1C,$1C,$1B,$1B,$16
		.BYTE	$16,$1B,$1B,$1C,$1C,$1E,$1E,$6F,$16,$16,$16,$1B,$1B,$1C,$1C,$1B
		.BYTE	$1B,$16,$16,$1B,$1B,$1C,$1C,$1B,$1B,$16,$16,$1B,$1B,$1C,$1C,$1E
		.BYTE	$1E,$16,$16,$1B,$1B,$1C,$1C,$1E,$1E,$67,$16,$67,$16,$FF,  3

_seq18:
		.BYTE	$24,$69,$18,$28,$28,$29,$6E,$17,$6F,$17,$29,$6E,$17,$6F,$17,$6F
		.BYTE	$17,$29,$66,$17,$29,$66,$17,$FF,  3

s16:
		.BYTE	$CB,$84,$6B,  5,$6D,$11,$82,$6C,  5,$84,$6B, $F,$82,  5,$84,$11
		.BYTE	$D8,$11,$CB,$82,$6C,  5,$84,$6B, $F,$82,$11
		.BYTE	$FF
s29:
		.BYTE	$C5,$88,$6E,  4,$CD,$84,  5,$CC,  2,  4,  2,$CD,  5,$CC,  2
		.BYTE	$FF
s17:
		.BYTE	$CC,$84,  4,  2,$CD,$84,$6E,  5,$CC,  2,  4,  2,$CD,  5,$CC,  2
		.BYTE	$FF
s18:
		.BYTE	$F0,$F0
		.BYTE	$FF
s19:
		.BYTE	$C9,$82,$76,$18,$18,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84,$CA
		.BYTE	$C9,$C9,$82,$C9,$C9,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84,$CA
		.BYTE	$C9
		.BYTE	$FF
s1A:
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
		.BYTE	$1F,$16,$18,$D0,$84,$77,$16,$D0,$82,$13,$D1,$16
		.BYTE	$FF
s1D:
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
		.BYTE	$D0,$86,$71,$D1,$82
		.BYTE	$FF
s1B:
		.BYTE	$CB,$84,$6B,  3,$6D, $F,$82,$6C,  3,$84,$6B, $D,$82,  3,$84, $F
		.BYTE	$D8,$CB,$82,$6C,  3,$84,$6B, $D,$82, $F
		.BYTE	$FF
s1C:
		.BYTE	$CB,$84,$6B,  1,$6D, $D,$82,$6C,  1,$84,$6B, $C,$82,  1,$84, $D
		.BYTE	$D8,$CB,$82,$6C,  1,$84,$6B, $C,$82, $D
		.BYTE	$FF
s1E:
		.BYTE	$CB,$84,$6B,  0,$6D, $C,$82,$6C,  0,$84,$6B, $A,$82,  0,$84, $C
		.BYTE	$D8,$CB,$82,$6C,  0,$84,$6B, $A,$82, $C
		.BYTE	$FF
s1F:
		.BYTE	$C9,$82,$7F,$18,$7F,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84,$CA
		.BYTE	$C9,$C9,$82,$71,$C9,$C9,$84,$C9,$CA,$82,$C9,$84,$CA,$82,$C9,$84
		.BYTE	$CA,$C9
		.BYTE	$FF
s20:
		.BYTE	$CE,$8C,$FD,  4, $A, $C,$98,$84, $A, $C, $F,$FD,$1F, $F,$11,$82
		.BYTE	$CF,$CE,$84, $F, $C,$88,$FD,$1F,$11,$12,$84,$11, $F,$88,$FD,$1F
		.BYTE	 $F,$11,$82,$13,$CF,$11,$CE,$88,$FD,$4F, $F, $C,$84,$88,$FD,  8
		.BYTE	 $A, $C,$84,$FD,$1C, $A, $C,$13,$16,$13,$88,$FD,$18,$16,$18,$86
		.BYTE	$FD,$18,$16,$18,$FD,$2F,$12,$11,$84, $F, $C,$8C,$FD,$18,$16,$18
		.BYTE	$86,$FD,$18,$16,$18,$FD,$2F,$12,$11,$84, $F,$11,$12,$88,$FD,$18
		.BYTE	$12,$13,$82,$16,$CF,$13,$CE,$88,$FD,$18,$12,$11,$84, $F, $C, $A
		.BYTE	 $C,$13,$CF,$11, $F, $C,$CE,$16,$18,$1B
		.BYTE	$FF
s21:
		.BYTE	$CE,$88,$FD,$18,$1B,$1D,$88,$FD,$18,$1E,$1D,$84,$1B,$16,$18,$8C
		.BYTE	$FD,$18,$1B,$1D,$88,$FD,$18,$1E,$1D,$84,$1B,$16,$18,$8C,$FD,$18
		.BYTE	$22,$24,$88,$22,$FD,$18,$1E,$1F,$84,$22,$88,$FD,$4F,$1E,$1D,$84
		.BYTE	$1D,$82,$1B,$CF,$1D,$CE,$88,$FD,$1F,$1B,$1D,$84,$18,$1B,$1D,$FD
		.BYTE	$1F,$1D,$1E,$82,$1D,$CF,$1E,$CE,$84,$1B,$84,$FD,$1F,$1D,$1E,$82
		.BYTE	$1D,$CF,$1E,$CE,$84,$1B,$84,$1D,$1F,$FD,$1F,$1B,$1D,$1B,$82,$18
		.BYTE	$CF,$1B,$CE,$88,$16,$82,$18,$CF,$16,$CE,$84,$1B,$82,$1D,$CF,$1B
		.BYTE	$CE,$88,$FD,  8,$1D,$1F,$86,$FD,  8,$21,$22,$CF,$82,$24,$CE,$84
		.BYTE	$FD,$1C,$1E,$1D,$84,$1B,$82,$18,$CF,$1B,$CE,$98,$FD,  6,$22,$24
		.BYTE	$CE,$84,$FD,$2F,$1D,$1F,$22,$24
		.BYTE	$FF
s26:
		.BYTE	$CE,$98,$FD,$1F,$26,$27,$84,$24,$CF,$27,$24,$CE,$84,$18,$1B,$1D
		.BYTE	$FD,$1F,$1D,$1E,$82,$1D,$81,$1E,$1D,$84,$1B,$18,$98,$FD,$1F,$24
		.BYTE	$26,$84,$22,$CF,$26,$22,$CE,$84,$18,$84,$1B,$1D,$FD,$1F,$1D,$1E
		.BYTE	$82,$1D,$81,$1E,$1D,$84,$1B,$18,$88,$FD,$1F,$22,$24,$84,$22,$20
		.BYTE	$22,$20,$1F,$88,$FD,$18,$1B,$1D,$84,$1B,$82,$1E,$CF,$1B,$CE,$88
		.BYTE	$FD,$18,$1B,$1D,$84,$1F,$22,$24,$88,$FD,$1C,$24,$26,$84,$27,$CE
		.BYTE	$88,$FD,$1F,$24,$26,$84,$24,$CE,$FD,$1F,$22,$23,$20,$8C,$FD,$1F
		.BYTE	$1E,$1F,$84,$13,$17,$18,$1A,$1F
		.BYTE	$FF
s22:
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
		.BYTE	$1D,$1B,$18,$16,$13,$1F,$1E,$1D
		.BYTE	$FF
s23:
		.BYTE	$E5
		.BYTE	$FF
s24:
		.BYTE	$FC
		.BYTE	$FF
s25:
		.BYTE	$1B,$18,$16,$13,$12
		.BYTE	$FF
s27:
		.BYTE	$CB,$84,$6C,  5,$FC,$FA,$D8,$86,  3,$CB,$84,  5,$FC,$FA,$D8,$86
		.BYTE	  8,$CB,$84,  5,$FC,$FA,$D8,$86,  3,$CB,$84,  5,$FC,$F8,$D8,$88
		.BYTE	  8
		.BYTE	$FF
s28:
		.BYTE	$C5,$90,$6E,  4,$F0,$FA,$86,  4,$90,$F0,$FA,$86,  4,$90,$F0,$FA
		.BYTE	$86,  4,$90,$F0,$F8,$88,  4
		.BYTE	$FF
_seq19:
		.BYTE	$80,$63,$52,$15,$2A,$2A,$63,$15,$2A,$2A,$FF,  0
s15:
		.BYTE	$DB,$82,  3,$DB,  5,$D9,$82,$7E,$18,$18,$DB,$82,  5,$D9,$84,$18
		.BYTE	$DB,$82,  5,$D9,$84,$18,$DB,$82,  5,$D9,$84,$18,$DB,$82,  3,$D9
		.BYTE	$84,$7F,$16
		.BYTE	$FF
s2A:
		.BYTE	$DB,$82,  1, $D,$D9,$7F,$14,$D9,$D9,$84,$82,$D9,$D9,$16,$D9,$D9
		.BYTE	$D9,$84,$DB,$82,  3,  5,  0,  1, $D,$D9,$14,$D9,$D9,$84,$82,$D9
		.BYTE	$D9,$84,$16,$82,$84,$DB,$82,  5,$11,  0,$DB,$82,  1, $D,$D9,$7F
		.BYTE	$14,$D9,$D9,$84,$82,$D9,$D9,$16,$D9,$D9,$D9,$84,$DB,$82,  3, $F
		.BYTE	  1,  0, $C,$D9,$11,$D9,$D9,$D9,$84,$D9,$13,$DB,$82,  0,  3,  0
		.BYTE	  8,  7,  3,  0
		.BYTE	$FF
_seq1A:
		.BYTE	$8E,$2B,$2D,$2B,$87,$2E
		.BYTE	$89,$2B,$2D,$2B,$82,$2E
		.BYTE	$FF,  0
_seq1B:
		.BYTE	$93,$2B,$2D,$2B,$87,$2F
		.BYTE	$8E,$2B,$2D,$2B,$82,$2F
		.BYTE	$FF,  0
_seq1C:
		.BYTE	$87,$2C,$2C,$82,$2C,$2C
		.BYTE	$FF,  0
_seq1D:
		.BYTE	$30
		.BYTE	$FF,  0

s2B:
		.BYTE	$DB,$82, $C,$DA
		.BYTE	$DB, $E
		.BYTE	$84,  2
		.BYTE	$82,  5,$DA
		.BYTE	$DB,  2
		.BYTE	  7
		.BYTE	$DB,$DA
		.BYTE	$DB,  2
		.BYTE	  8,$DA
		.BYTE	$DB,  9
		.BYTE	$DC,$86, $C
		.BYTE	$DB,$82, $E
		.BYTE	$84,  2
		.BYTE	$82,  5,$DA
		.BYTE	$DB,  2
		.BYTE	  7
		.BYTE	$FF
s2D:
		.BYTE	$82,  7,$DA,$DB,  8,  7,  5,  2,  0
		.BYTE	$FF
s2E:
		.BYTE	$DC,$84,$18,$DB,$82, $E,$DC,$84,$17,$DB,$15
		.BYTE	$FF
s2F:
		.BYTE	$DC,$84,$11,$DB,$82,  9,$DC,$84,$13,$DB,$11
		.BYTE	$FF
s2C:
		.BYTE	$CB,$84,$6B,  2,$D8,$82,$6D,$6B,$CB,$84,$6C,  5,$D8,$6B,$CB,  7
		.BYTE	$D8,$6D,$CB,$6C,  9,$D8,$6B,$CB,  2,$D8,$82,$6D,$6B,$CB,$84,$6C
		.BYTE	  5,$D8,$6B,$CB,  7,$D8,$82,$D8,$CB,$84,$6C,  9,$D8,$82,$D8
		.BYTE	$FF
s30:
		.BYTE	$CC,$84,  2,$CC,$CD,$6E,  5,$CC,  2,$CC,$CC,$CD,  5,$CC,  2,$CC
		.BYTE	$CC,$CD,  5,$CC,  2,$CC,$CC,$CD,  5,$82,$CD
		.BYTE	$FF
_seq1E:
		.BYTE	$86,$31,$89,$31,$FF,  0
_seq1F:
		.BYTE	$86,$32,$89,$32,$FF,  0
_seq20:
		.BYTE	$86,$71,$33,$89,$33,$FF,  0
_seq21:
		.BYTE	$34,$FF,  0
s31:
		.BYTE	$C9,$88,$71,$18,$84,$8C,$84,$74,$1C,$CA,$71,$18,$C9,$88,$74,$1C
		.BYTE	$84,$73,$18,$94,$88,$7F,$84,$88,$18,$84,$6F,$1D,$CA,$7F,$18,$C9
		.BYTE	$A4,$6F,$1D,$88,$71,$18,$84,$8C,$84,$74,$1C,$CA,$71,$18,$C9,$88
		.BYTE	$73,$1A,$84,$94,$90,$1B,$1D,$A0,$1F
		.BYTE	$FF
s32:
		.BYTE	$CE,$88,$1F,$84,$8C,$FD,  4,$1E,$1F,$84,$24,$CF,$1F,$CE,$88,$FD
		.BYTE	  4,$23,$24,$84,$21,$88,$84,$1F,$1D,$21,$88,$FD,  4,$1D,$1F,$84
		.BYTE	$1F,$88,$84,$24,$CF,$1F,$CE,$8C,$FD,  4,$22,$24,$DD,$84,$2B,$29
		.BYTE	$FD,$1F,$29,$2B,$29,$24,$21,$CE,$88,$1F,$84,$8C,$FD,  4,$1E,$1F
		.BYTE	$84,$24,$CF,$1F,$CE,$88,$FD,  4,$22,$24,$84,$23,$8C,$84,$21,$23
		.BYTE	$86,$FD,  4,$22,$24,$22,$84,$24,$86,$FD,  4,$24,$26,$24,$84,$26
		.BYTE	$FD,$1F,$26,$28,$CF,$24,$1F,$1D,$1C,$DD,$82,$28,$2B,$88,$30
		.BYTE	$FF
s33:
		.BYTE	$DF,$90,$6B,  0,$6C,  4,$6B,  5,$6C,  9,$6B,  2,$6C,  4,$6B,  5
		.BYTE	$6C,$6B,  0,$6C,  4,$6B,  7,$6C,$6B,  8,$6C, $A,$6B, $C,$6C
		.BYTE	$FF
s34:
		.BYTE	$CC,$84,  0,  1,  2,  3,$CD,$6E,  5,$CC,  3,  2,  1
		.BYTE	$FF
_seq22:
		.BYTE	$8E,$36,$23
		.BYTE	$FE
_seq23:
		.BYTE	$8E,$71,$23,$36,$23
		.BYTE	$FE
_seq24:
		.BYTE	$82,$71,$35
		.BYTE	$FE
_seq25:
		.BYTE	$63,$34,$37
		.BYTE	$FE
s35:
		.BYTE	$DF,$90,$6B, $C,$6C, $A,$6B,  8,$6C,  7,$6B,  5,$6C,  3,$6B,  2
		.BYTE	$A0,$6C,  0,$85, $C,$E1
		.BYTE	$FF
s36:
		.BYTE	$DE,$86,$FD,  8,$1D,$1F,$1D,$84,$1B,$86,$FD,  8,$1B,$1D,$1B,$84
		.BYTE	$1A,$86,$FD,  8,$1A,$1B,$1A,$84,$18,$8C,$16,$82,$14,$16,$86,$18
		.BYTE	$1A,$84,$1B,$86,$FD, $C,$16,$18,$13,$84, $F,$86,$FD, $F, $D, $E
		.BYTE	 $C,$84, $B,$9E,$FD,  8, $B, $C,$81,$18,$24,$82,$30
		.BYTE	$FF
s37:
		.BYTE	$CC,$84,  0,  1,  2,  3,$CD,$86,$6E,  5,$E1
		.BYTE	$FF
_seq2A:
		.BYTE	$72,$38
		.BYTE	$FE
s38:
		.BYTE	$DF,$86,$6D,  4,$E1
		.BYTE	$FF
_seq2B:
		.BYTE	$8A,$73,$39
		.BYTE	$FE
_seq2C:
		.BYTE	$8B,$73,$39
		.BYTE	$FE
s39:
		.BYTE	$DF,$88,$22,$E1
		.BYTE	$FF
_seq2D:
		.BYTE	$84,$74,$3A
		.BYTE	$FE
_seq2E:
		.BYTE	$86,$74,$3A
		.BYTE	$FE
_seq2F:
		.BYTE	$88,$74,$3A
		.BYTE	$FE
_seq30:
		.BYTE	$8A,$74,$3A
		.BYTE	$FE
_seq31:
		.BYTE	$98,$74,$3A
		.BYTE	$FE
_seq32:
		.BYTE	$9A,$74,$3A
		.BYTE	$FE
_seq33:
		.BYTE	$9C,$74,$3A
		.BYTE	$FE
_seq34:
		.BYTE	$A3,$3A
		.BYTE	$FE
s3A:
		.BYTE	$DF,$84,$26,$E1
		.BYTE	$FF
_seq35:
		.BYTE	$AD,$3B
		.BYTE	$FE
_seq36:
		.BYTE	$AA,$3B
		.BYTE	$FE
s3B:
		.BYTE	$DF,$83,$26,$E1
		.BYTE	$FF
_seq37:
		.BYTE	$3C,$FF,  0
s3C:
		.BYTE	$DB,$82,  2,  2,  0,  2,$D9,$88,$7F,$1A,$86,$18,$18,$84,$19,$DB
		.BYTE	$82,  2,  5,  0,  2,$D9,$88,$1A,$86,$1D,$1D,$84,$1B,$DB,$82,  2
		.BYTE	 $E,  0,  2,$D9,$88,$1A,$86,$18,$19,$84,$1A,$DB,$82,  2, $E, $C
		.BYTE	  2,$D9,$88,$1A,$86,$1F,$1D,$82,$1B,$1A,$DB,$84,  0,$82,  3,  0
		.BYTE	$D9,$86,$70,$18,$84,$DB,$82,  0,  3,  0,  5,  7, $A, $C,$84,  0
		.BYTE	$82,  3,  7,$D9,$86,$75,$1B,$84,$74,$1A,$82,$70,$18,$84,$71,$16
		.BYTE	$70,$18,$74,$DB,$84,  0,$82,  3,  0,$D9,$86,$70,$18,$84,$DB,$82
		.BYTE	  3,  5,  7, $A, $C, $F, $C,$84,  0,$82,  3,  0,$D9,$86,$72,$1F
		.BYTE	$84,$73,$1D,$82,$75,$1B,$84,$74,$1A,$75,$1B,$1C
		.BYTE	$FF
_seq38:
		.BYTE	$3D,$3E,$FF,  0
s3D:
		.BYTE	$DB,$82,  2,  4,$D9,$70,$1C,$82,$84,$DB,$82,  2,  4,$D9,$71,$1A
		.BYTE	$84,$70,$1C,$D9,$D9,$82,$71,$1A,$84,$70,$1C,$DB,$82,  2,  4,$D9
		.BYTE	$70,$1C,$D9,$D9,$84,$DB,$82,  2,  4,$D9,$84,$71,$1F,$D9,$D9,$D9
		.BYTE	$82,$21,$1F,$DB,$82,  2,  4,$D9,$70,$1C,$D9,$D9,$84,$DB,$82,  2
		.BYTE	  4,$D9,$71,$1A,$84,$70,$1C,$D9,$D9,$82,$71,$1A,$84,$70,$1C,$DB
		.BYTE	$82,  1,  2,$D9,$84,$71,$1A,$DB,$82,  2,  3,$D9,$84,$71,$1B,$DB
		.BYTE	$82,  3,  4,$D9,$84,$75,$1F,$74,$1E,$82,$73,$1C,$1A
		.BYTE	$FF
s3E:
		.BYTE	$DB,$82,  7,$D9,$84,$71,$1F,$DB,$82,  7,$D9,$84,$1F,$DB,$82,  9
		.BYTE	$D9,$86,$73,$1C,$84,$82,$1A,$84,$74,$19,$82,$17,$DB,$82,  7,$D9
		.BYTE	$84,$71,$1F,$DB,$82,  7,$D9,$84,$1F,$DB,$82,  9,$D9,$86,$71,$21
		.BYTE	$84,$73,$1C,$82,$1A,$84,$74,$19,$82,$17,$DB,$82,  7,$D9,$84,$71
		.BYTE	$1F,$DB,$82,  7,$D9,$84,$1F,$DB,$82,  9,$D9,$86,$73,$1C,$84,$1C
		.BYTE	$1A,$74,$19,$DB,$84,  0,$D9,$71,$18,$DB,  2,$D9,$1A,$DB,$82,  4
		.BYTE	$D9,$84,$73,$1A,$82,$84,$82,$71,$73
		.BYTE	$FF
_seq39:
		.BYTE	$80,$3F,$40,$83,$40,$80,$40,$85,$40,$FF,  0
s3F:
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
s40:
		.BYTE	$DB,$82,  1,  3,$D9,$70,$1B,$84,$D9,$DB,$82,  3,$D9,$84,$73,$19
		.BYTE	$DB,$82,  3,$D9,$84,$19,$DB,$82,  3,$D9,$84,$19,$DB,$82,  1,$DB
		.BYTE	$82,  3,$D9,$82,$70,$1B,$84,$D9,$DB,$82,  3,$D9,$84,$73,$1B,$DB
		.BYTE	$82,  3,$D9,$84,$1B,$DB,$82,  3,$D9,$84,$1B
		.BYTE	$FF
_seq3A:
		.BYTE	$80,$41,$41,$42,$85,$41,$41,$42,$82,$41,$41,$42,$87,$41,$41,$42
		.BYTE	$FF,  0
s41:
		.BYTE	$D9,$84,$78, $E,$82,$78,$82,$84,$84,$82, $C,$82,$84,$84,$82, $A
		.BYTE	 $C,$84, $E,$82,$82,$84,$84,$82, $F,$82,$84,$84,$82,$11, $F
		.BYTE	$FF
s42:
		.BYTE	$D9,$84,$78, $E,$82,$13,$11,$84, $F, $C,$82, $E,$82,$82,$82,$82
		.BYTE	 $F, $E,$84, $C, $E,$82,$13,$11,$84, $F, $C,$90, $E,$84, $F,$82
		.BYTE	$82,$84,$84, $E,$88, $C,$84, $A,  7,  9,$82,$82,$84,$84,  7,$90
		.BYTE	  9
		.BYTE	$FF
_seq3B:
		.BYTE	$72,$43
		.BYTE	$FE
s43:
		.BYTE	$CD,$81,  0,  2,  3,  5,  2,  3,  2,  4,  6,  3,  0,  2,  2,  4
		.BYTE	  5,  2,  1,  3,  5,  0,  2,  4,  1,  3,  0,  4,  2,  5,  4,  0
		.BYTE	  1,  5,  3,  2,  0,  6,  1,  3,  1,  4,  3,  2,  3,$83,  2,$E1
		.BYTE	$FF
_seq00:
		.BYTE	  0,$FE
s00:
		.BYTE	$E1,$FF
sex:
