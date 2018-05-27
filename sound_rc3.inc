
; -----------------------------------------------------------------------------
; NES Audio-player written for exclusive use by JEROEN TEL / JT & PROBE (C)1991
; -----------------------------------------------------------------------------

; THIS IS NOT THE FINAL VERSION
; RIGHT VERSION IS ON SOME LOST BACKUP_DISK 3-1/2"
; JEROEN TEL 3-2-93

_apu_load_chnl:
AUDIOIN:
		ASL
		STA		_apu_seqposit,X
		LDA		#sex-s00-1
		STA		_apu_stpindex,X
		LDA		#0
		STA		_apu_seqindex,X
		STA		_apu_spdcheck,X
		CPX		#2
		BNE		MORESP1
		LDA		#1
		STA		_apu_spdcount,X
		LDA		#0
		BEQ		MORESP2
MORESP1:
		STA		_apu_spdcount,X
MORESP2:
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
		LDA		#$F					;VOICE 01 - ?? enabled
		STA		_APU_STATUS
		RTS

_apu_play:
		LDX		#0
		JSR		AUDIOPL
		LDX		#1
		JSR		AUDIOPL
		LDX		#2
		JSR		AUDIOPL
		LDX		#3
		JSR		AUDIOPL
		RTS

AUDIOPL:
		LDA		_apu_voictest,X
		BEQ		AUDIOEX
		CPX		#0
		BEQ		VOICE01
		CPX		#1
		BEQ		VOICE02
		CPX		#2
		BEQ		VOICE03
		CPX		#3
		BEQ		VOICE04
;		CPX		#4
;		BEQ		VOICE05
AUDIOEX:
		RTS
VOICE04:
		TXA
		ASL
		ASL
		TAY
		LDA		#$10				;Set _apu_amplitude				<bit 04>
		ORA		#$20				;Continuous lenght enabled	<bit 05>
		ORA		_apu_amplitud,X		;_apu_amplitude/Envelope Value	<bit 03-00>
		STA		_APU_PULSE1_ENV,Y
		LDA		_apu_lofrqwrd,X
		STA		_APU_PULSE1_TIME,Y
		LDA		#$F8
		STA		_APU_PULSE1_LEN,Y
		JMP		JMPVO04
VOICE03:
		TXA
		ASL
		ASL
		TAY
		LDA		#$80
		JMP		JMPVO03
VOICE02:
VOICE01:
setvoi1:
		TXA
		ASL
		ASL
		TAY
		LDA		_apu_pulsbyt1,X		;setpulse						<bit 07-06>
		ORA		#$10				;Set _apu_amplitude				<bit 04>
		ORA		#$20				;continuous lenght enabled		<bit 05>
JMPVO03:
		ORA		_apu_amplitud,X		;_apu_amplitude/Envelope Value	<bit 03-00>
		STA		_APU_PULSE1_ENV,Y
		LDA		#8					;Hardware-glide down			<bit 03>
		STA		_APU_PULSE1_SWEEP,Y
		LDA		_apu_lofrqwrd,X
		STA		_APU_PULSE1_TIME,Y
		LDA		_apu_hifrqwrd,X		;Check if hi-frq thesame as note before
		CMP		_apu_continos,X
		BEQ		NOHINO1
		STA		_APU_PULSE1_LEN,Y	;HF set
		STA		_apu_continos,X
NOHINO1:
JMPVO04:
SPDHDL1:
		LDA		#0					;Speedroutine (two speed bytes)
		STA		_apu_realplay,X
		DEC		_apu_spdcount,X
		BPL		SPDHDX1
		LDA		_apu_spdcheck,X
		EOR		#$FF
		STA		_apu_spdcheck,X
		BPL		SECONS1
		LDY		_apu_seqposit,X
		LDA		_apu_seqtabes,Y
		JMP		SPDSET1
SECONS1:
		LDY		_apu_seqposit,X
		LDA		_apu_seqtabes+1,Y
SPDSET1:
		STA		_apu_spdcount,X
		LDA		#$FF
		STA		_apu_realplay,X
SPDHDX1
		LDA		_apu_realplay,X		;check if new leng subtraction necessary
		BPL		NOSTPH1
AUDIPL1:
		DEC		_apu_noteleng,X		;decrease speed, if zero then stephandle
		BEQ		STPHDL1
NOSTPH1:
		LDA		_apu_realnote,X		;check _apu_realnote on pause, no FX
		CMP		#$FF
		BEQ		NOFXPL1
		JSR		FXPLAY1
NOFXPL1:
		LDA		#0					;clear _apu_noteinit
		STA		_apu_noteinit,X
		RTS
STPHDL1:
		LDY		_apu_stepnumb,X		;What step to take
		LDA		_apu_stptablo,Y
		STA		_apu_tmp0
		LDA		_apu_stptabhi,Y
		STA		_apu_tmp0+1
		LDA		#0					;reset glide (if not reached dest. note)
		STA		_apu_gldspeed,X
		LDY		_apu_stpindex,X		;read step-index
		LDA		(_apu_tmp0),Y
		CMP		#$FF				;step check on end
		BEQ		SEQHDL1				;yes, goto sequence-handle
		JMP		STPHD12				;no, goto stephandle 02
SEQHDL1:
		DEC		_apu_repstcnt,X		;check if step repeats
		BEQ		REALSQ1				;no, goto real sequence-handle
		LDA		#0					;yes, reset index & do last step again
		STA		_apu_stpindex,X
		TAY
		LDA		(_apu_tmp0),Y
		JMP		STPHD12
REALSQ1:
		LDY		_apu_seqposit,X		;what sequence to take
		LDA		_apu_seqtabel,Y
		STA		_apu_tmp0
		LDA		_apu_seqtabel+1,Y
		STA		_apu_tmp0+1
		LDY		_apu_seqindex,X		;read sequence-index
		LDA		(_apu_tmp0),Y
		CMP		#$FF				;check sequence on end-repeat
		BNE		SEQHD12
		INY
		LDA		(_apu_tmp0),Y		;where to jump back in sequence
		STA		_apu_seqindex,X
		TAY
		LDA		(_apu_tmp0),Y
		JMP		SEQHD15				;goto next sequence
SEQHD12:
		CMP		#$FE				;check sequence on end-real
		BNE		SEQHD13
		LDA		#0					;yes, disable voice
		STA		_apu_voictest,X
		RTS
SEQHD13:
		CMP		#$FC				;!!!! DO THE FADE! GODDAMNED !!!!
		BNE		SEQHD15				;!!!! still to be programmed !!!!
SEQHD15:
		CMP		#$80				;note transpose	check
		BCC		SEQHD16
		AND		#$7F
;		CPX		#3
;		BEQ		NOTRNSI
;		CLC
;		ADC		#transio
NOTRNSI:
		STA		_apu_tranbyte,X
		INY
		LDA		(_apu_tmp0),Y
		BMI		NONWSTP
;		CMP		#$80
;		BCS		NONWSTP
SEQHD16:
		CMP		#$70				;sfix transpose	check
		BCC		SEQHD17
		AND		#$F
		STA		_apu_sfxtrans,X
		INY
		LDA		(_apu_tmp0),Y
SEQHD17:
		CMP		#$60				;repeat step check
		BCC		SEQHD18
		AND		#$F
		STA		_apu_repstcnt,X
		INY
		LDA		(_apu_tmp0),Y
SEQHD18:
		CMP		#$50				;arpg transpose check
		BCC		SEQHD19
		AND		#$F
		STA		_apu_arptrans,X
		INY
		LDA		(_apu_tmp0),Y
SEQHD19:
		STA		_apu_stepnumb,X		;set step & clear stepindex
		INY
NONWSTP:
		TYA
		STA		_apu_seqindex,X
		INC		_apu_repstcnt,X		;make sure step plays once
		LDA		#0
		STA		_apu_stpindex,X
		JMP		STPHDL1				;back to step-handle
STPHD12:
		CMP		#$FD				;glide check in step
		BNE		STPHD14
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
		JMP		NONEWN1
STPHD14:
		CMP		#$E0				;pause check
		BCC		STPHD15
		AND		#$1F
		STA		_apu_realleng,X
		LDA		#$FF
		STA		_apu_realnote,X
		LDA		#0
		STA		_apu_amplitud,X
		INY
		TYA
		STA		_apu_stpindex,X
		JMP		PAUSEX1
STPHD15:
		CMP		#$C0				;sound check
		BCC		STPHD16
		CLC
		ADC		_apu_sfxtrans,X
		ASL
		ASL
		STA		_apu_realsoun,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$FD
		BEQ		STPHD12
		CMP		#$C0
		BCS		NONEWN1
STPHD16:
		CMP		#$80				;length check
		BCC		STPHD17
		AND		#$7F
;		CLC
;		ADC		_apu_noteleng,X
;		STA		_apu_noteleng,X
		STA		_apu_realleng,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$FD
		BEQ		STPHD12
		CMP		#$80
		BCS		NONEWN1
;		CMP		#$80
;		BCS		STPHD16
STPHD17:
		CMP		#$60				;arp check
		BCC		STPHD18
		AND		#$1F
		CLC
		ADC		_apu_arptrans,X
		STA		_apu_arpnumbo,X
		INY
		LDA		(_apu_tmp0),Y
		CMP		#$60
		BCS		NONEWN1
STPHD18:
		CLC							;set note + note transpose
		ADC		_apu_tranbyte,X
		STA		_apu_realnote,X
		INY
NONEWN1:
		TYA
		STA		_apu_stpindex,X		;restore step-index
		INC		_apu_noteinit,X		;first init has been done
		LDY		_apu_realsoun,X
		CPX		#0					;check sound-init VOICE 01
		BEQ		INITV12
		CPX		#1					;check sound-init VOICE 02
		BEQ		INITV12
		CPX		#2					;check sound-init VOICE 03
		BEQ		INITV03
		CPX		#3					;check sound-init VOICE 04
		BEQ		INITV04
		RTS
INITV12:
		LDA		_apu_soundfix,Y		;check if new pulse to be set
		AND		#8
		BNE		NONEWPL
		LDA		_apu_soundfix,Y		;yes, set new pulse
		AND		#$C0
		STA		_apu_pulsbyt1,X
INITV03:
INITV04:
NONEWPL:
		LDA		_apu_soundfix+1,Y	;set vibrato-byte into _apu_fx01 & set effects (arp, etc)
		STA		_apu_fx01,X
		LDA		_apu_soundfix+2,Y	;set _apu_vibdelay pultab-number into _apu_fx02
		STA		_apu_fx02,X
;		LDA		_apu_soundfix+1,Y	;set gate-leng value
;		AND		#$F0
;		LSR
;		LSR
;		LSR
;		LSR
;		STA		_apu_gateleng,X
NONEWG1:
		LDA		_apu_soundfix+3,Y	;set effect-values into _apu_fx03
		STA		_apu_fx03,X
		CPX		#3
		BEQ		NFORV04
		LDY		_apu_realnote,X		;Read _apu_realnote as index
		LDA		_apu_frqtablo,Y		;Low-frequency read			<bit 07-00>
		STA		_apu_lofrqwrd,X
		LDA		#$F8				;hardware length endless	<bit 07-03>
		ORA		_apu_frqtabhi,Y		;hi-frequency read			<bit 02-00>
		STA		_apu_hifrqwrd,X		;HF set
		JMP		PAUSEX1
NFORV04:
		LDA		_apu_realnote,X
		STA		_apu_lofrqwrd,X
PAUSEX1:
		LDA		_apu_realleng,X
		STA		_apu_noteleng,X
		RTS

;INITV03:
;		RTS

FXPLAY1:
		LDA		_apu_noteinit,X
		BNE		INTPLA1
		JMP		PLAYFXR
INTPLA1:
		LDA		_apu_gldspeed,X
		BEQ		FXPLA12
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
FXPLA12:
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
		BCS		REFIX01
		LDA		#0
		STA		_apu_fx01,X
		BEQ		EXITIN1
REFIX01:
		STA		_apu_vibdelay,X
EXITIN1:
		JMP		NOVIB01
PLAYFXR:
		LDA		_apu_fx03,X			;_apu_amplitude routine
		AND		#$7F
		TAY
		LDA		_apu_amptabl,Y
		STA		_apu_tmp0
		LDA		_apu_amptabh,Y
		STA		_apu_tmp0+1
		LDY		_apu_ampindex,X
		LDA		(_apu_tmp0),Y
		CMP		#$FE
		BEQ		NOAMPL1
		STA		_apu_amplitud,X
		INC		_apu_ampindex,X
NOAMPL1:
GLIDE1:
		LDY		_apu_gldspeed,X		;glide routine
		BEQ		NOGLD12
		LDA		_apu_glddelay,X
		CMP		_apu_noteleng,X
		BCC		NOGLD12
		LDY		_apu_glddirec,X
		BMI		DOWNGL1
		LDA		_apu_lofrqwrd,X
		CLC
		ADC		_apu_gldspeed,X
		STA		_apu_lofrqwrd,X
		BCC		CHECKV1
		INC		_apu_hifrqwrd,X
CHECKV1:
		LDY		_apu_destnote,X
		SEC
		SBC		_apu_frqtablo,Y
		BCC		NOGLD01
		BCS		FROTER1
DOWNGL1:
		LDA		_apu_lofrqwrd,X
		SEC
		SBC		_apu_gldspeed,X
		STA		_apu_lofrqwrd,X
		BCS		CHECKV2
		DEC		_apu_hifrqwrd,X
CHECKV2:
		LDY		_apu_destnote,X
		SEC
		SBC		_apu_frqtablo,Y
		BCS		NOGLD01
FROTER1:
		TYA
		STA		_apu_realnote,X
		LDA		_apu_frqtablo,Y
		STA		_apu_lofrqwrd,X
		LDA		_apu_frqtabhi,Y
		STA		_apu_hifrqwrd,X
		LDA		#0
		STA		_apu_gldspeed,X
NOGLD01:
		JMP		NOVIB01
NOGLD12:
VIBRA01:
		LDA		_apu_fx01,X			;vibrato routine
		AND		#$F
		BEQ		NOVIB01
		LDA		_apu_vibdelay,X
		CMP		_apu_noteleng,X
		BCC		NOVIB01
		LDA		_apu_fx01,X
		AND		#$F
		TAY
		LDA		_apu_vibtabl-1,Y
		STA		_apu_tmp0
		LDA		_apu_vibtabh-1,Y
		STA		_apu_tmp0+1
		LDY		_apu_vibindex,X
		LDA		(_apu_tmp0),Y
		CMP		#$80
		BNE		VIB01
		LDY		#0
VIB01:
		LDA		(_apu_tmp0),Y
		BPL		UPVIBOS
		EOR		#$FF
		CLC
		ADC		#1
		STA		_apu_tmp0
		LDA		_apu_lofrqwrd,X
		SEC
		SBC		_apu_tmp0
		STA		_apu_lofrqwrd,X
		BCS		NOHIGH1
		DEC		_apu_hifrqwrd,X
NOHIGH1:
		JMP		VIBNO01
UPVIBOS:
		LDA		_apu_lofrqwrd,X
		CLC
		ADC		(_apu_tmp0),Y
		STA		_apu_lofrqwrd,X
		BCC		NOHIGH2
		INC		_apu_hifrqwrd,X
NOHIGH2:
VIBNO01:
		INY
		TYA
		STA		_apu_vibindex,X
NOVIB01:
PULSE1:
		CPX		#2
		BCS		NOPULS1
		LDA		_apu_fx02,X			;PULSE ROUTINE
		AND		#$F
		BEQ		NOPULS1
		TAY
		LDA		_apu_pultabl-1,Y
		STA		_apu_tmp0
		LDA		_apu_pultabh-1,Y
		STA		_apu_tmp0+1
		LDY		_apu_pulindex,X
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BNE		PULS01
		INY
		LDA		(_apu_tmp0),Y
		TAY
		LDA		(_apu_tmp0),Y
PULS01:
		CMP		#$FE
		BEQ		NOPULS1
		STA		_apu_pulsbyt1,X
		INY
		TYA
		STA		_apu_pulindex,X
NOPULS1:
ARPEG01:
		LDA		_apu_fx01,X			;ARP ROUTINE
		AND		#$40
		BEQ		NOARP01
		LDY		_apu_arpnumbo,X
		LDA		_apu_arptablo,Y
		STA		_apu_tmp0
		LDA		_apu_arptabhi,Y
		STA		_apu_tmp0+1
		LDY		_apu_arpindex,X
		BNE		ARP01
		LDA		(_apu_tmp0),Y
		STA		_apu_arpspeed,X
		INY
		JMP		ARP02
ARP01:
		LDA		_apu_arpspeed,X
		SEC
		SBC		#$10
		STA		_apu_arpspeed,X
		BPL		ARPNO01
		LDY		#0
		LDA		(_apu_tmp0),Y
		STA		_apu_arpspeed,X
		LDY		_apu_arpindex,X
ARP02:
		LDA		(_apu_tmp0),Y
		CMP		#$FF
		BNE		ARP03
		LDY		#0
		LDA		(_apu_tmp0),Y
		AND		#$F
		TAY
		INY
		LDA		(_apu_tmp0),Y
ARP03:
		CMP		#$FE
		BEQ		NOARP01
		BMI		NORMA01
		CLC
		ADC		_apu_realnote,X
		STA		_apu_arponoto,X
NORMA01:
		AND		#$7F
		STA		_apu_arponoto,X
		INY
		TYA
		STA		_apu_arpindex,X
ARPNO01:
		LDA		_apu_arponoto,X
		CPX		#3
		BNE		SETAN01
		STA		_apu_lofrqwrd,X
		JMP		NOARP01
SETAN01:
		TAY
		LDA		_apu_frqtablo,Y
		STA		_apu_lofrqwrd,X
		LDA		_apu_frqtabhi,Y
		STA		_apu_hifrqwrd,X
NOARP01:
		RTS

;---------------------------------------------------------------------------------------------------------------------------------
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
;---------------------------------------------------------------------------------------------------------------------------------

_apu_seqtabel:
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq01_sinshot1	; 1 0
		.WORD	_seq02_sinshot4	; 2 3
		.WORD	_seq03_launchs4	; 3 3
		.WORD	_seq04_launchb4	; 4 3
		.WORD	_seq05_scrnexp3	; 5 2
		.WORD	_seq06_scrnexp4	; 6 3
		.WORD	_seq07_exploss3	; 7 2
		.WORD	_seq08_exploss4	; 8 3
		.WORD	_seq09_explosm3	; 9 2
		.WORD	_seq0A_explosm4	;  $A 3
		.WORD	_seq0B_explosb3	;  $B 2
		.WORD	_seq0C_explosb4	;  $C 3
		.WORD	_seq0D_pickupa1	;  $D 0
		.WORD	_seq0E_pickupa2	;  $E 1
		.WORD	_seq0F_pickupb1	;  $F 0
		.WORD	_seq10_pickupb2	; $10 1
		.WORD	_seq11_jetpack4	; $11 3
		.WORD	_seq12_robofal2	; $12 1
		.WORD	_seq13_robofal3	; $13 2
		.WORD	_seq14_robofal4	; $14 3
		.WORD	_seq15_title_01	; $15 0
		.WORD	_seq16_title_02	; $16 1
		.WORD	_seq17_title_03	; $17 2
		.WORD	_seq18_title_04	; $18 3
		.WORD	_seq19_ingame_2	; $19 1
		.WORD	_seq1A_repair_1	; $1A 0
		.WORD	_seq1B_repair_2	; $1B 1
		.WORD	_seq1C_repair_3	; $1C 2
		.WORD	_seq1D_repair_4	; $1D 3
		.WORD	_seq1E_wintun_1	; $1E 0
		.WORD	_seq1F_wintun_2	; $1F 1
		.WORD	_seq20_wintun_3	; $20 2
		.WORD	_seq21_wintun_4	; $21 3
		.WORD	_seq22_lostun_1	; $22 0
		.WORD	_seq23_lostun_2	; $23 1
		.WORD	_seq24_lostun_3	; $24 2
		.WORD	_seq25_lostun_4	; $25 3
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX
		.WORD	_seq00_clearsfx	; 0 $XX

_apu_seqtabes:
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  2,  2		; 1 0
		.BYTE	  2,  2		; 2 3
		.BYTE	  2,  2		; 3 3
		.BYTE	  2,  2		; 4 3
		.BYTE	  2,  2		; 5 2
		.BYTE	  2,  2		; 6 3
		.BYTE	  2,  2		; 7 2
		.BYTE	  2,  2		; 8 3
		.BYTE	  2,  2		; 9 2
		.BYTE	  2,  2		;  $A 3
		.BYTE	  2,  2		;  $B 2
		.BYTE	  2,  2		;  $C 3
		.BYTE	  2,  2		;  $D 0
		.BYTE	  2,  2		;  $E 1
		.BYTE	  2,  2		;  $F 0
		.BYTE	  2,  2		; $10 1
		.BYTE	$7F,$7F		; $11 3
		.BYTE	  2,  2		; $12 1
		.BYTE	  3,  3		; $13 2
		.BYTE	  4,  4		; $14 3
		.BYTE	  3,  2		; $15 0
		.BYTE	  3,  2		; $16 1
		.BYTE	  3,  2		; $17 2
		.BYTE	  3,  2		; $18 3
		.BYTE	  3,  3		; $19 1
		.BYTE	  3,  3		; $1A 0
		.BYTE	  3,  3		; $1B 1
		.BYTE	  3,  3		; $1C 2
		.BYTE	  3,  3		; $1D 3
		.BYTE	  3,  2		; $1E 0
		.BYTE	  3,  2		; $1F 1
		.BYTE	  3,  2		; $20 2
		.BYTE	  3,  2		; $21 3
		.BYTE	  3,  2		; $22 0
		.BYTE	  3,  2		; $23 1
		.BYTE	  3,  2		; $24 2
		.BYTE	  3,  2		; $25 3
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX
		.BYTE	  0,  0		; 0 $XX

_apu_stptablo:
		.BYTE	<s00,<s01,<s02,<s03,<s04,<s05,<s06,<s07,<s08,<s09,<s0a,<s0b,<s0c,<s0d,<s0e,<s0f
		.BYTE	<s10,<s11,<s12,<s13,<s14,<s15,<s16,<s17,<s18,<s19,<s1a,<s1b,<s1c,<s1d,<s1e,<s1f
		.BYTE	<s20,<s21,<s22,<s23,<s24,<s25,<s26,<s27,<s28,<s29,<s2a,<s2b,<s2c,<s2d,<s2e,<s2f
		.BYTE	<s30,<s31,<s32,<s33,<s34,<s35,<s36,<s37

_apu_stptabhi:
		.BYTE	>s00,>s01,>s02,>s03,>s04,>s05,>s06,>s07,>s08,>s09,>s0a,>s0b,>s0c,>s0d,>s0e,>s0f
		.BYTE	>s10,>s11,>s12,>s13,>s14,>s15,>s16,>s17,>s18,>s19,>s1a,>s1b,>s1c,>s1d,>s1e,>s1f
		.BYTE	>s20,>s21,>s22,>s23,>s24,>s25,>s26,>s27,>s28,>s29,>s2a,>s2b,>s2c,>s2d,>s2e,>s2f
		.BYTE	>s30,>s31,>s32,>s33,>s34,>s35,>s36,>s37

_apu_amptabl:
		.BYTE	<am00,<am01,<am02,<am03,<am04,<am05,<am06,<am07,<am08,<am09,<am0a,<am0b,<am0c,<am0d,<am0e,<am0f
		.BYTE	<am10,<am11,<am12,<am13,<am14
_apu_amptabh:
		.BYTE	>am00,>am01,>am02,>am03,>am04,>am05,>am06,>am07,>am08,>am09,>am0a,>am0b,>am0c,>am0d,>am0e,>am0f
		.BYTE	>am10,>am11,>am12,>am13,>am14

_apu_vibtabl:
		.BYTE	<v00,<v01,<v02
_apu_vibtabh:
		.BYTE	>v00,>v01,>v02

_apu_pultabl:
		.BYTE	<p00,<p01,<p02,<p03,<p04
_apu_pultabh:
		.BYTE	>p00,>p01,>p02,>p03,>p04

_apu_arptablo:
		.BYTE	<a00,<a01,<a02,<a03,<a04,<a05,<a06,<a07,<a08,<a09,<a0a,<a0b,<a0c,<a0d,<a0e,<a0f
		.BYTE	<a10,<a11,<a12,<a13,<a14,<a15,<a16,<a17,<a18,<a19,<a1a,<a1b,<a1c,<a1d,<a1e,<a1f
		.BYTE	<a20,<a21
_apu_arptabhi:
		.BYTE	>a00,>a01,>a02,>a03,>a04,>a05,>a06,>a07,>a08,>a09,>a0a,>a0b,>a0c,>a0d,>a0e,>a0f
		.BYTE	>a10,>a11,>a12,>a13,>a14,>a15,>a16,>a17,>a18,>a19,>a1a,>a1b,>a1c,>a1d,>a1e,>a1f
		.BYTE	>a20,>a21

;---------------------------------------------------------------------------------------------------------------------------------
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
am0a:
		.BYTE	  6,  3,  2,  2,  1,$FE
am0b:
		.BYTE	  1,$FE
am0c:
		.BYTE	 $A,  6,  5,  4,  4,  3,  3,  2,$FE
am0d:
		.BYTE	 $F, $C, $B, $A,  9,  8,  8,  7,  6,  5,  4,  3,  2,$FE
am0e:
		.BYTE	 $F, $C,  9,  7,  6,  5,$FE
am0f:
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
;---------------------------------------------------------------------------------------------------------------------------------
v00:
		.BYTE	  1,  1,  0,$FF,$FF,  0,$FF,$FF,  0,  1,  1,  0,$80
v01:
		.BYTE	  4,  4,  4,$FC,$FC,$FC,$FC,$FC,$FC,  4,  4,  4,$80
v02:
		.BYTE	  2,  1,  1,$FE,$FF,$FF,$FE,$FF,$FF,  2,  1,  1,$80
;---------------------------------------------------------------------------------------------------------------------------------
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
;---------------------------------------------------------------------------------------------------------------------------------
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
a0a:
		.BYTE	  7,$18,$15,$12, $F, $C,  9,  7,  6,  5,  4,  3,  2,  1,  0,$FF
a0b:
		.BYTE	  0,$9A,$98,$96,$94,  0,$FE
a0c:
		.BYTE	  0,$A2,$A0,$9E,$9C,  0,$FE
a0d:
		.BYTE	  0,  0,$FE
a0e:
		.BYTE	  0,  1,  0,$FF
a0f:
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
a1a:
		.BYTE	$20,  0,  8,$FF
a1b:
		.BYTE	$20,  0,  4,$FF
a1c:
		.BYTE	$20,  0,  3,$FF
a1d:
		.BYTE	  3,  0,  2,  8,  0,  0,  2,  2,  8,  8,$FF
a1e:
		.BYTE	  3,  0,  3,  6,  0,  0,  3,  3,  6,  6,$FF
a1f:
		.BYTE	  3,  0,  5,  7,  0,  0,  5,  5,  7,  7,$FF
a20:
		.BYTE	$10,  0,  5,  3,  8,$FF
a21:
		.BYTE	$10,  0,  7,  5,  9,$FF
;---------------------------------------------------------------------------------------------------------------------------------

_apu_soundfix:
		.BYTE	$80,$40,  0,  0	;C0 SingShot (1)
		.BYTE	$80,$40,  0,  1	;C1 LaunchMs (4)
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

;---------------------------------------------------------------------------------------------------------------------------------
_seq01_sinshot1:
		.BYTE	  1				;SINGLE SHOT (REPEAT BY PRG)
		.BYTE	$FE

_seq02_sinshot4:
		.BYTE	  2
		.BYTE	$FE
s01:
		.BYTE	$C0,$8C,$60,  0
		.BYTE	$FF
s02:
		.BYTE	$C0,$8C,$61,  0
		.BYTE	$FF

_seq03_launchs4:
		.BYTE	  3				;LAUNCH MISSILE
		.BYTE	$FE
s03:
		.BYTE	$C1,$90,$62,  0
		.BYTE	$FF

_seq04_launchb4:
		.BYTE	  4				;LAUNCH BIG MISSILE
		.BYTE	$FE

s04:
		.BYTE	$C2,$98,$63,  0,$E1
		.BYTE	$FF

_seq05_scrnexp3:
		.BYTE	  5				;SCREEN EXPLODE
		.BYTE	$FE

_seq06_scrnexp4:
		.BYTE	  6
		.BYTE	$FE
s05:
		.BYTE	$C3,$8E,$64,  8,$E1
		.BYTE	$FF
s06:
		.BYTE	$C3,$97,$65,  0,$E1
		.BYTE	$FF

_seq07_exploss3:
		.BYTE	  7				;SMALL EXPLOSION
		.BYTE	$FE

_seq08_exploss4:
		.BYTE	  8
		.BYTE	$FE

s07:
		.BYTE	$C4,$86,$64,  4,$E1
		.BYTE	$FF
s08:
		.BYTE	$C4,$8C,$66, $B,$E1
		.BYTE	$FF

_seq09_explosm3:
		.BYTE	  9				;MEDIUM EXPLOSION
		.BYTE	$FE

_seq0A_explosm4:
		.BYTE	 $A
		.BYTE	$FE

s09:
		.BYTE	$C5,$89,$64,  5,$E1
		.BYTE	$FF
s0a:
		.BYTE	$C5,$90,$66, $C,$E1
		.BYTE	$FF

_seq0B_explosb3:
		.BYTE	 $B				;BIG EXPLOSION
		.BYTE	$FE

_seq0C_explosb4:
		.BYTE	 $C
		.BYTE	$FE

s0b:
		.BYTE	$C3,$8C,$64,  6,$E1
		.BYTE	$FF
s0c:
		.BYTE	$C3,$97,$66, $D,$E1
		.BYTE	$FF

_seq0D_pickupa1:
		.BYTE	 $D				;PICK UP WEAPON
		.BYTE	$FE

_seq0E_pickupa2:
		.BYTE	 $E
		.BYTE	$FE

s0d:
		.BYTE	$C6,$90,$67,$18,$E1
		.BYTE	$FF
s0e:
		.BYTE	$E1,$C6,$90,$67,$18,$E1
		.BYTE	$FF

_seq0F_pickupb1:
		.BYTE	 $F				;PICK UP ENERGY
		.BYTE	$FE

_seq10_pickupb2:
		.BYTE	$10
		.BYTE	$FE

s0f:
		.BYTE	$C6,$90,$68,$1C,$E1
		.BYTE	$FF
s10:
		.BYTE	$E1,$C6,$90,$68,$1C,$E1
		.BYTE	$FF

_seq11_jetpack4:
		.BYTE	$11				;JETPACK NOISE
		.BYTE	$FE

s11:
		.BYTE	$C7,$BF,  9
		.BYTE	$FF

_seq12_robofal2:
		.BYTE	$12				;ROBOCOP LANDING AFTER JUMP
		.BYTE	$FE

_seq13_robofal3:
		.BYTE	$13
		.BYTE	$FE

_seq14_robofal4:
		.BYTE	$14
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

		.ORG $8C31

_seq15_title_01:
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

_seq16_title_02:
		.BYTE	$85,$71,$23,$22			;A
		.BYTE	$91,$19,$8F			;A
		.BYTE	$8D,$8F			;A
		.BYTE	$91,$8F,$8D,$8F		;A
		.BYTE	$91,$8F,$8D,$8F			;A
		.BYTE	$91,$8F,$8D,$8F			;A
		.BYTE	$91,$70,$1A,$1D			;B
		.BYTE	$91,$19,$8F,$8D,$8F		;A
		.BYTE	$91,$8F,$8D,$8F			;A
		.BYTE	$91,$20,$21,$26,$26		;C
		.BYTE	$91,$67,$19			;A
		.BYTE	$FF,  4

_seq17_title_03:
		.BYTE	$24,$69,$18,$27,$27			;A
		.BYTE	$6F,$16					;A
		.BYTE	$16,$16,$1B,$1B,$1C,$1C,$1B,$1B		;B
		.BYTE	$16,$16,$1B,$1B,$1C,$1C,$1E,$1E		;B
		.BYTE	$6F,$16					;A
		.BYTE	$16,$16,$1B,$1B,$1C,$1C,$1B,$1B		;C
		.BYTE	$16,$16,$1B,$1B,$1C,$1C,$1B,$1B		;C
		.BYTE	$16,$16,$1B,$1B,$1C,$1C,$1E,$1E		;C
		.BYTE	$16,$16,$1B,$1B,$1C,$1C,$1E,$1E		;C
		.BYTE	$67,$16,$67,$16				;A
		.BYTE	$FF,  3

_seq18_title_04:
		.BYTE	$24,$69,$18,$28,$28				;A
		.BYTE	$29,$6E,$17					;A
		.BYTE	$6F,$17							;B
		.BYTE	$29,$6E,$17					;A
		.BYTE	$6F,$17,$6F,$17					;C
		.BYTE	$29,$66,$17,$29,$66,$17				;A
		.BYTE	$FF,  3

s16:
		.BYTE	$CB,$84,$6B,  5
		.BYTE	$6D,$11
		.BYTE	$82,$6C,  5
		.BYTE	$84,$6B, $F
		.BYTE	$82,  5
		.BYTE	$84,$11
		.BYTE	$D8,$11
		.BYTE	$CB,$82,$6C,  5
		.BYTE	$84,$6B, $F
		.BYTE	$82,$11
		.BYTE	$FF
s29:
		.BYTE	$C5,$88,$6E,  4
		.BYTE	$CD,$84,  5
		.BYTE	$CC,  2
		.BYTE	  4
		.BYTE	  2
		.BYTE	$CD,  5
		.BYTE	$CC,  2
		.BYTE	$FF
s17:
		.BYTE	$CC,$84,  4
		.BYTE	  2
		.BYTE	$CD,$84,$6E,  5
		.BYTE	$CC,  2
		.BYTE	  4
		.BYTE	  2
		.BYTE	$CD,  5
		.BYTE	$CC,  2
		.BYTE	$FF
s18:
		.BYTE	$F0,$F0
		.BYTE	$FF
s19:
		.BYTE	$C9,$82,$76,$18
		.BYTE	$18
		.BYTE	$84
		.BYTE	$C9,$CA,$82
		.BYTE	$C9,$84,$CA,$82
		.BYTE	$C9,$84,$CA
		.BYTE	$C9
		.BYTE	$C9,$82
		.BYTE	$C9
		.BYTE	$C9,$84
		.BYTE	$C9,$CA,$82
		.BYTE	$C9,$84,$CA,$82
		.BYTE	$C9,$84,$CA
		.BYTE	$C9
		.BYTE	$FF
s1a:
		.BYTE	$CE,$88,$FD,$4F,$1B,$18
		.BYTE	$82,$18,$CF
		.BYTE	$CE,$88,$FD,$1F,$16,$18,$CF,$84
		.BYTE	$CE,$82,$13,$CF,$18
		.BYTE	$CE,$88,$16
		.BYTE	$84,$18,$CF,$84,$16,$13, $C
		.BYTE	$CE,$82,$13,$CF
		.BYTE	$CE,$84,$16
		.BYTE	$82,$18,$CF,$16
		.BYTE	$CE,$88,$FD,$4F,$1D,$1B
		.BYTE	$84,$CF,$1D
		.BYTE	$CE,$88,$FD,$1F,$18,$1A
		.BYTE	$82,$1B,$CF,$1A
		.BYTE	$CE,$98,$FD,$1F,$18,$1A
		.BYTE	$82,$13,$CF,$1A
		.BYTE	$CE,$84,$16
		.BYTE	$82,$18,$CF,$16
		.BYTE	$CE,$88,$FD,$4F,$1B,$18
		.BYTE	$82,$16,$CF,$18
		.BYTE	$CE,$88,$FD,$1F,$16,$18
		.BYTE	$82,$13,$CF,$18
		.BYTE	$CE,$84,$16
		.BYTE	$82,$18,$CF,$16
		.BYTE	$CE,$88,$FD,$1F,$1D,$1F
		.BYTE	$82,$22,$CF,$1F
		.BYTE	$CE,$88,$FD,$1F,$1E,$1D
		.BYTE	$82,$1B,$CF,$1D
		.BYTE	$CE,$84,$FD,$1F,$1B,$1D
		.BYTE	$82,$1B,$CF,$1D
		.BYTE	$CE,$84,$FD,$1F,$1D,$1F
		.BYTE	$82,$18,$CF,$1F
		.BYTE	$CE,$88,$FD,$1F,$16,$18
		.BYTE	$84,$FD,$1F,$16,$18
		.BYTE	$82,$1B,$CF,$18
		.BYTE	$CE,$84,$FD,$1F,$1B,$1D
		.BYTE	$82,$1E,$CF,$1D
		.BYTE	$CE,$84,$FD,$1F,$1E,$1F
		.BYTE	$82,$18,$CF,$1F
		.BYTE	$CE,$84,$FD,$1F,$16,$18,$CF,$16
		.BYTE	$CE,$88,$FD,$1F,$16,$18

		.BYTE	$D0,$84,$77,$16
		.BYTE	$D0,$82,$13,$D1,$16
		.BYTE	$FF
s1d:
		.BYTE	$D0,$8C,$77,$16
		.BYTE	$88,$13,$D1,$84
		.BYTE	$D0,$84,$16
		.BYTE	$82,$13,$D1,$16
		.BYTE	$D0,$8C,$16
		.BYTE	$88,$13,$D1,$84
		.BYTE	$D0,$84,$16
		.BYTE	$82,$13,$D1,$16
		.BYTE	$D0,$88,$79,$16
		.BYTE	$82,$78,$D1
		.BYTE	$D0,$88,$79
		.BYTE	$82,$78,$D1,$79
		.BYTE	$D0,$88,$79
		.BYTE	$82,$78,$D1,$79
		.BYTE	$D0,$84,$77
		.BYTE	$78
		.BYTE	$82,$77,$D1,$78
		.BYTE	$D0,$84,$7A
		.BYTE	$82,$78,$D1,$7A
		.BYTE	$D0,$84,$77
		.BYTE	$82,$11,$D1,$16

		.BYTE	$D0,$84,$7B,$14
		.BYTE	$82,$D1
		.BYTE	$D0,$D1
		.BYTE	$D0,$82
		.BYTE	$86,$7C,$13
		.BYTE	$82,$7B,$14,$D1,$7C,$13
		.BYTE	$D0,$84,$7C,$18
		.BYTE	$82,$77,$D1,$7C
		.BYTE	$D0,$84,$7C,$18
		.BYTE	$82,$7B,$14,$D1,$7C,$18
		.BYTE	$D0,$84,$7B,$14
		.BYTE	$82,$7C,$13
		.BYTE	$84,$7B,$14,$D1,$82
		.BYTE	$D0,$82,$16,$D1,$14
		.BYTE	$D0,$84,$7C,$18
		.BYTE	$82,$1A,$D1,$18
		.BYTE	$D0,$84,$6F
		.BYTE	$82,$7D,$D1,$6F
		.BYTE	$D0,$84
		.BYTE	$82,$7D,$D1,$6F
		.BYTE	$D0,$84
		.BYTE	$82,$7D,$D1,$6F
		.BYTE	$D0,$84
		.BYTE	$82,$7D,$D1,$6F
		.BYTE	$D0,$86,$74,$17,$D1,$82
		.BYTE	$D0,$86,$7E,$D1,$82
		.BYTE	$D0,$86,$72,$13,$D1,$82
		.BYTE	$D0,$86,$71,$D1,$82
		.BYTE	$FF
s1b:
		.BYTE	$CB,$84,$6B,  3
		.BYTE	$6D, $F
		.BYTE	$82,$6C,  3
		.BYTE	$84,$6B, $D
		.BYTE	$82,  3
		.BYTE	$84, $F
		.BYTE	$D8
		.BYTE	$CB,$82,$6C,  3
		.BYTE	$84,$6B, $D
		.BYTE	$82, $F
		.BYTE	$FF
s1c:
		.BYTE	$CB,$84,$6B,  1
		.BYTE	$6D, $D
		.BYTE	$82,$6C,  1
		.BYTE	$84,$6B, $C
		.BYTE	$82,  1
		.BYTE	$84, $D
		.BYTE	$D8
		.BYTE	$CB,$82,$6C,  1
		.BYTE	$84,$6B, $C
		.BYTE	$82, $D
		.BYTE	$FF
s1e:
		.BYTE	$CB,$84,$6B,  0
		.BYTE	$6D, $C
		.BYTE	$82,$6C,  0
		.BYTE	$84,$6B, $A
		.BYTE	$82,  0
		.BYTE	$84, $C
		.BYTE	$D8
		.BYTE	$CB,$82,$6C,  0
		.BYTE	$84,$6B, $A
		.BYTE	$82, $C
		.BYTE	$FF
s1f:
		.BYTE	$C9,$82,$7F,$18
		.BYTE	$7F
		.BYTE	$84
		.BYTE	$C9,$CA,$82
		.BYTE	$C9,$84,$CA,$82
		.BYTE	$C9,$84,$CA
		.BYTE	$C9
		.BYTE	$C9,$82,$71
		.BYTE	$C9
		.BYTE	$C9,$84
		.BYTE	$C9,$CA,$82
		.BYTE	$C9,$84,$CA,$82
		.BYTE	$C9,$84,$CA
		.BYTE	$C9
		.BYTE	$FF
s20:
		.BYTE	$CE,$8C,$FD,  4, $A, $C
		.BYTE	$98
		.BYTE	$84, $A
		.BYTE	 $C
		.BYTE	 $F
		.BYTE	$FD,$1F, $F,$11
		.BYTE	$82,$CF
		.BYTE	$CE,$84, $F
		.BYTE	 $C
		.BYTE	$88,$FD,$1F,$11,$12
		.BYTE	$84,$11
		.BYTE	 $F
		.BYTE	$88,$FD,$1F, $F,$11
		.BYTE	$82,$13,$CF,$11
		.BYTE	$CE,$88,$FD,$4F, $F, $C
		.BYTE	$84
		.BYTE	$88,$FD,  8, $A, $C
		.BYTE	$84,$FD,$1C, $A, $C
		.BYTE	$13
		.BYTE	$16
		.BYTE	$13
		.BYTE	$88,$FD,$18,$16,$18
		.BYTE	$86,$FD,$18,$16,$18
		.BYTE	$FD,$2F,$12,$11
		.BYTE	$84, $F
		.BYTE	 $C
		.BYTE	$8C,$FD,$18,$16,$18
		.BYTE	$86,$FD,$18,$16,$18
		.BYTE	$FD,$2F,$12,$11
		.BYTE	$84, $F
		.BYTE	$11
		.BYTE	$12
		.BYTE	$88,$FD,$18,$12,$13
		.BYTE	$82,$16,$CF,$13
		.BYTE	$CE,$88,$FD,$18,$12,$11
		.BYTE	$84, $F
		.BYTE	 $C
		.BYTE	 $A
		.BYTE	 $C
		.BYTE	$13,$CF,$11, $F, $C
		.BYTE	$CE,$16
		.BYTE	$18
		.BYTE	$1B
		.BYTE	$FF
s21:
		.BYTE	$CE,$88,$FD,$18,$1B,$1D
		.BYTE	$88,$FD,$18,$1E,$1D
		.BYTE	$84,$1B
		.BYTE	$16
		.BYTE	$18
		.BYTE	$8C,$FD,$18,$1B,$1D
		.BYTE	$88,$FD,$18,$1E,$1D
		.BYTE	$84,$1B
		.BYTE	$16
		.BYTE	$18
		.BYTE	$8C,$FD,$18,$22,$24
		.BYTE	$88,$22
		.BYTE	$FD,$18,$1E,$1F
		.BYTE	$84,$22
		.BYTE	$88,$FD,$4F,$1E,$1D
		.BYTE	$84,$1D
		.BYTE	$82,$1B,$CF,$1D
		.BYTE	$CE,$88,$FD,$1F,$1B,$1D
		.BYTE	$84,$18
		.BYTE	$1B
		.BYTE	$1D
		.BYTE	$FD,$1F,$1D,$1E
		.BYTE	$82,$1D,$CF,$1E
		.BYTE	$CE,$84,$1B
		.BYTE	$84,$FD,$1F,$1D,$1E
		.BYTE	$82,$1D,$CF,$1E
		.BYTE	$CE,$84,$1B
		.BYTE	$84,$1D
		.BYTE	$1F
		.BYTE	$FD,$1F,$1B,$1D
		.BYTE	$1B
		.BYTE	$82,$18,$CF,$1B
		.BYTE	$CE,$88,$16
		.BYTE	$82,$18,$CF,$16
		.BYTE	$CE,$84,$1B
		.BYTE	$82,$1D,$CF,$1B
		.BYTE	$CE,$88,$FD,  8,$1D,$1F
		.BYTE	$86,$FD,  8,$21,$22,$CF,$82,$24
		.BYTE	$CE,$84,$FD,$1C,$1E,$1D
		.BYTE	$84,$1B
		.BYTE	$82,$18,$CF,$1B
		.BYTE	$CE,$98,$FD,  6,$22,$24

		.BYTE	$CE,$84,$FD,$2F,$1D,$1F
		.BYTE	$22
		.BYTE	$24
		.BYTE	$FF
s26:
		.BYTE	$CE,$98,$FD,$1F,$26,$27
		.BYTE	$84,$24,$CF,$27,$24
		.BYTE	$CE,$84,$18
		.BYTE	$1B
		.BYTE	$1D
		.BYTE	$FD,$1F,$1D,$1E
		.BYTE	$82,$1D,$81,$1E,$1D
		.BYTE	$84,$1B
		.BYTE	$18
		.BYTE	$98,$FD,$1F,$24,$26
		.BYTE	$84,$22,$CF,$26,$22
		.BYTE	$CE,$84,$18
		.BYTE	$84,$1B
		.BYTE	$1D
		.BYTE	$FD,$1F,$1D,$1E
		.BYTE	$82,$1D,$81,$1E,$1D
		.BYTE	$84,$1B
		.BYTE	$18
		.BYTE	$88,$FD,$1F,$22,$24
		.BYTE	$84,$22
		.BYTE	$20
		.BYTE	$22
		.BYTE	$20
		.BYTE	$1F
		.BYTE	$88,$FD,$18,$1B,$1D
		.BYTE	$84,$1B
		.BYTE	$82,$1E,$CF,$1B
		.BYTE	$CE,$88,$FD,$18,$1B,$1D
		.BYTE	$84,$1F
		.BYTE	$22
		.BYTE	$24

		.BYTE	$88,$FD,$1C,$24,$26
		.BYTE	$84,$27
		.BYTE	$CE,$88,$FD,$1F,$24,$26
		.BYTE	$84,$24
		.BYTE	$CE,$FD,$1F,$22,$23
		.BYTE	$20
		.BYTE	$8C,$FD,$1F,$1E,$1F
		.BYTE	$84,$13
		.BYTE	$17
		.BYTE	$18
		.BYTE	$1A
		.BYTE	$1F
		.BYTE	$FF
s22:
		.BYTE	$D4,$82,$3F,$3E,$3A,$37,$3E,$3A,$37,$35
		.BYTE	$3A,$37,$35,$33,$37,$33,$32,$2E
		.BYTE	$33,$32,$2E,$2B,$32,$2E,$2B,$29
		.BYTE	$2E,$2B,$29,$27,$2B,$29,$27,$26

		.BYTE	$27,$22,$1F,$27,$22,$1F,$29,$26
		.BYTE	$27,$24,$1F,$27,$24,$1F,$26,$27
		.BYTE	$29,$22,$1D,$27,$22,$1D,$29,$22
		.BYTE	$26,$22,$1D,$26,$27,$26,$24,$22

		.BYTE	$D4,$83,$24,$20,$1B,$26,$20,$1B,$27,$20
		.BYTE	$1B,$27,$20,$1B,$26,$20,$24,$20
		.BYTE	$84,$23,$1F,$1A,$24,$1F,$1A,$26,$23
		.BYTE	$1F,$27,$24,$1F,$85,$29,$26,$2C,$2B
		.BYTE	$A8,$FD,  1,$2E,$30
		.BYTE	$D6,$81
		.BYTE	$33,$30,$2E,$2B,$2A,$29,$27,$24
		.BYTE	$30,$2E,$2B,$2A,$29,$27,$24,$22
		.BYTE	$2B,$2A,$29,$27,$24,$22,$1F,$1E
		.BYTE	$29,$27,$24,$22,$1F,$1E,$1D,$1B
		.BYTE	$27,$24,$22,$1F,$1E,$1D,$1B,$18
		.BYTE	$24,$22,$1F,$1E,$1D,$1B,$18,$16
		.BYTE	$22,$1F,$1E,$1D,$1B,$18,$16,$13
		.BYTE	$1F,$1E,$1D
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
		.BYTE	$CB,$84,$6C,  5
		.BYTE	$FC,$FA
		.BYTE	$D8,$86,  3
		.BYTE	$CB,$84,  5
		.BYTE	$FC,$FA
		.BYTE	$D8,$86,  8
		.BYTE	$CB,$84,  5
		.BYTE	$FC,$FA
		.BYTE	$D8,$86,  3
		.BYTE	$CB,$84,  5
		.BYTE	$FC,$F8
		.BYTE	$D8,$88,  8
		.BYTE	$FF
s28:
		.BYTE	$C5,$90,$6E,  4
		.BYTE	$F0,$FA
		.BYTE	$86,  4
		.BYTE	$90
		.BYTE	$F0,$FA
		.BYTE	$86,  4
		.BYTE	$90
		.BYTE	$F0,$FA
		.BYTE	$86,  4
		.BYTE	$90
		.BYTE	$F0,$F8
		.BYTE	$88,  4
		.BYTE	$FF

_seq19_ingame_2
		.BYTE	$80,$63,$52,$15,$2A,$2A,$63,$15,$2A,$2A
		.BYTE	$FF,  0

s15:
		.BYTE	$DB,$82,  3
		.BYTE	$DB,  5
		.BYTE	$D9,$82,$7E,$18,$18
		.BYTE	$DB,$82,  5
		.BYTE	$D9,$84,$18
		.BYTE	$DB,$82,  5
		.BYTE	$D9,$84,$18
		.BYTE	$DB,$82,  5
		.BYTE	$D9,$84,$18
		.BYTE	$DB,$82,  3
		.BYTE	$D9,$84,$7F,$16
		.BYTE	$FF
s2a:
		.BYTE	$DB,$82,  1, $D
		.BYTE	$D9,$7F,$14
		.BYTE	$D9
		.BYTE	$D9,$84
		.BYTE	$82
		.BYTE	$D9
		.BYTE	$D9,$16
		.BYTE	$D9
		.BYTE	$D9
		.BYTE	$D9,$84
		.BYTE	$DB,$82,  3,  5,  0
		.BYTE	  1
		.BYTE	 $D
		.BYTE	$D9,$14
		.BYTE	$D9
		.BYTE	$D9,$84
		.BYTE	$82
		.BYTE	$D9
		.BYTE	$D9,$84,$16
		.BYTE	$82
		.BYTE	$84
		.BYTE	$DB,$82,  5,$11,  0

		.BYTE	$DB,$82,  1, $D
		.BYTE	$D9,$7F,$14
		.BYTE	$D9
		.BYTE	$D9,$84
		.BYTE	$82
		.BYTE	$D9
		.BYTE	$D9,$16
		.BYTE	$D9
		.BYTE	$D9
		.BYTE	$D9,$84
		.BYTE	$DB,$82,  3, $F,  1
		.BYTE	  0
		.BYTE	 $C
		.BYTE	$D9,$11
		.BYTE	$D9
		.BYTE	$D9
		.BYTE	$D9,$84
		.BYTE	$D9,$13
		.BYTE	$DB,$82,  0,  3,  0,  8,  7,  3,  0
		.BYTE	$FF

_seq1A_repair_1:
		.BYTE	$8E,$2B,$2D,$2B,$87,$2E
		.BYTE	$89,$2B,$2D,$2B,$82,$2E
		.BYTE	$FF,  0
_seq1B_repair_2:
		.BYTE	$93,$2B,$2D,$2B,$87,$2F
		.BYTE	$8E,$2B,$2D,$2B,$82,$2F
		.BYTE	$FF,  0
_seq1C_repair_3:
		.BYTE	$87,$2C,$2C,$82,$2C,$2C
		.BYTE	$FF,  0
_seq1D_repair_4:
		.BYTE	$30
		.BYTE	$FF,  0

s2b:
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
		.BYTE	$DB,$82,  5,$DA			; the only music sequences difference from the final version
		.BYTE	$DB,  2
		.BYTE	  7
		.BYTE	$FF
s2d:
		.BYTE	$82,  7,$DA
		.BYTE	$DB,  8,  7,  5,  2,  0
		.BYTE	$FF
s2e:
		.BYTE	$DC,$84,$18
		.BYTE	$DB,$82, $E
		.BYTE	$DC,$84,$17
		.BYTE	$DB,$15
		.BYTE	$FF

s2f:
		.BYTE	$DC,$84,$11
		.BYTE	$DB,$82,  9
		.BYTE	$DC,$84,$13
		.BYTE	$DB,$11
		.BYTE	$FF

s2c:
		.BYTE	$CB,$84,$6B,  2
		.BYTE	$D8,$82,$6D
		.BYTE	$6B
		.BYTE	$CB,$84,$6C,  5
		.BYTE	$D8,$6B
		.BYTE	$CB,  7
		.BYTE	$D8,$6D
		.BYTE	$CB,$6C,  9
		.BYTE	$D8,$6B
		.BYTE	$CB,  2
		.BYTE	$D8,$82,$6D
		.BYTE	$6B
		.BYTE	$CB,$84,$6C,  5
		.BYTE	$D8,$6B
		.BYTE	$CB,  7
		.BYTE	$D8,$82
		.BYTE	$D8
		.BYTE	$CB,$84,$6C,  9
		.BYTE	$D8,$82
		.BYTE	$D8
		.BYTE	$FF
s30:
		.BYTE	$CC,$84,  2
		.BYTE	$CC
		.BYTE	$CD,$6E,  5
		.BYTE	$CC,  2
		.BYTE	$CC
		.BYTE	$CC
		.BYTE	$CD,  5
		.BYTE	$CC,  2
		.BYTE	$CC
		.BYTE	$CC
		.BYTE	$CD,  5
		.BYTE	$CC,  2
		.BYTE	$CC
		.BYTE	$CC
		.BYTE	$CD,  5
		.BYTE	$82
		.BYTE	$CD
		.BYTE	$FF

_seq1E_wintun_1:
		.BYTE	$86,$31,$89,$31
		.BYTE	$FF,  0

_seq1F_wintun_2:
		.BYTE	$86,$32,$89,$32
		.BYTE	$FF,  0

_seq20_wintun_3:
		.BYTE	$86,$71,$33,$89,$33
		.BYTE	$FF,  0

_seq21_wintun_4:
		.BYTE	$34
		.BYTE	$FF,  0

s31:
		.BYTE	$C9,$88,$71,$18
		.BYTE	$84
		.BYTE	$8C
		.BYTE	$84,$74,$1C,$CA,$71,$18
		.BYTE	$C9,$88,$74,$1C
		.BYTE	$84,$73,$18
		.BYTE	$94
		.BYTE	$88,$7F
		.BYTE	$84
		.BYTE	$88,$18
		.BYTE	$84,$6F,$1D,$CA,$7F,$18
		.BYTE	$C9,$A4,$6F,$1D

		.BYTE	$88,$71,$18
		.BYTE	$84
		.BYTE	$8C
		.BYTE	$84,$74,$1C,$CA,$71,$18
		.BYTE	$C9,$88,$73,$1A
		.BYTE	$84
		.BYTE	$94
		.BYTE	$90,$1B
		.BYTE	$1D
		.BYTE	$A0,$1F
		.BYTE	$FF
s32:
		.BYTE	$CE,$88,$1F
		.BYTE	$84
		.BYTE	$8C,$FD,  4,$1E,$1F
		.BYTE	$84,$24,$CF,$1F
		.BYTE	$CE,$88,$FD,  4,$23,$24
		.BYTE	$84,$21
		.BYTE	$88
		.BYTE	$84,$1F
		.BYTE	$1D
		.BYTE	$21
		.BYTE	$88,$FD,  4,$1D,$1F
		.BYTE	$84,$1F
		.BYTE	$88
		.BYTE	$84,$24,$CF,$1F
		.BYTE	$CE,$8C,$FD,  4,$22,$24
		.BYTE	$DD,$84,$2B
		.BYTE	$29
		.BYTE	$FD,$1F,$29,$2B
		.BYTE	$29
		.BYTE	$24
		.BYTE	$21
		.BYTE	$CE,$88,$1F
		.BYTE	$84
		.BYTE	$8C,$FD,  4,$1E,$1F
		.BYTE	$84,$24,$CF,$1F
		.BYTE	$CE,$88,$FD,  4,$22,$24
		.BYTE	$84,$23
		.BYTE	$8C
		.BYTE	$84,$21
		.BYTE	$23
		.BYTE	$86,$FD,  4,$22,$24
		.BYTE	$22
		.BYTE	$84,$24
		.BYTE	$86,$FD,  4,$24,$26
		.BYTE	$24
		.BYTE	$84,$26
		.BYTE	$FD,$1F,$26,$28
		.BYTE	$CF,$24
		.BYTE	$1F
		.BYTE	$1D
		.BYTE	$1C
		.BYTE	$DD,$82,$28
		.BYTE	$2B
		.BYTE	$88,$30
		.BYTE	$FF
s33:
		.BYTE	$DF,$90,$6B,  0
		.BYTE	$6C,  4
		.BYTE	$6B,  5
		.BYTE	$6C,  9
		.BYTE	$6B,  2
		.BYTE	$6C,  4
		.BYTE	$6B,  5
		.BYTE	$6C
		.BYTE	$6B,  0
		.BYTE	$6C,  4
		.BYTE	$6B,  7
		.BYTE	$6C
		.BYTE	$6B,  8
		.BYTE	$6C, $A
		.BYTE	$6B, $C
		.BYTE	$6C
		.BYTE	$FF
s34:
		.BYTE	$CC,$84,  0
		.BYTE	  1
		.BYTE	  2
		.BYTE	  3
		.BYTE	$CD,$6E,  5
		.BYTE	$CC,  3
		.BYTE	  2
		.BYTE	  1
		.BYTE	$FF

_seq22_lostun_1:
		.BYTE	$8E,$36,$23
		.BYTE	$FE
_seq23_lostun_2:
		.BYTE	$8E,$71,$23,$36,$23
		.BYTE	$FE
_seq24_lostun_3:
		.BYTE	$82,$71,$35
		.BYTE	$FE
_seq25_lostun_4:
		.BYTE	$63,$34,$37
		.BYTE	$FE

s35:
		.BYTE	$DF,$90,$6B, $C
		.BYTE	$6C, $A
		.BYTE	$6B,  8
		.BYTE	$6C,  7
		.BYTE	$6B,  5
		.BYTE	$6C,  3
		.BYTE	$6B,  2
		.BYTE	$A0,$6C,  0
		.BYTE	$85, $C,$E1
		.BYTE	$FF
s36:
		.BYTE	$DE,$86,$FD,  8,$1D,$1F
		.BYTE	$1D
		.BYTE	$84,$1B
		.BYTE	$86,$FD,  8,$1B,$1D
		.BYTE	$1B
		.BYTE	$84,$1A
		.BYTE	$86,$FD,  8,$1A,$1B
		.BYTE	$1A
		.BYTE	$84,$18
		.BYTE	$8C,$16
		.BYTE	$82,$14,$16
		.BYTE	$86,$18
		.BYTE	$1A
		.BYTE	$84,$1B
		.BYTE	$86,$FD, $C,$16,$18
		.BYTE	$13
		.BYTE	$84, $F
		.BYTE	$86,$FD, $F, $D, $E
		.BYTE	 $C
		.BYTE	$84, $B
		.BYTE	$9E,$FD,  8, $B, $C
		.BYTE	$81,$18,$24
		.BYTE	$82,$30
		.BYTE	$FF
s37:
		.BYTE	$CC,$84,  0
		.BYTE	  1
		.BYTE	  2
		.BYTE	  3
		.BYTE	$CD,$86,$6E,  5,$E1
		.BYTE	$FF

_seq00_clearsfx:
		.BYTE	  0,$FE			;EMPTY SFX
s00:
		.BYTE	$E1,$FF
sex:
