;-----------------------------------------------------------------------------
;NES Audio-player written for exclusive use by JEROEN TEL / JT & PROBE (C)1991
;-----------------------------------------------------------------------------

;GAME		: OVERLORD
;MUSIC BY	: JEROEN TEL
;Format		: NINTENDO ENTERTAINMENT SYSTEM 8-BIT
;game by	: MARK KELLEY & STEVE CROW FOR PROBE SOFTWARE LTD
;published by	: VIRGIN GAMES U.S.A.
;date		: 23/08/91

ssslo	=	$fe	
ssshi	=	$ff

variable	=	$0700
voictest	=	variable+$00*$04
stpindex	=	variable+$01*$04
seqindex	=	variable+$02*$04
seqposit	=	variable+$03*$04
repstcnt	=	variable+$04*$04
stepnumb	=	variable+$05*$04
spdcount	=	variable+$06*$04
spdcheck	=	variable+$07*$04
realplay	=	variable+$08*$04
noteinit	=	variable+$09*$04
realsoun	=	variable+$0a*$04
noteleng	=	variable+$0b*$04
realleng	=	variable+$0c*$04
gateleng	=	variable+$0d*$04
realnote	=	variable+$0e*$04
lofrqwrd	=	variable+$0f*$04
hifrqwrd	=	variable+$10*$04
tranbyte	=	variable+$11*$04
sfxtrans	=	variable+$12*$04
arptrans	=	variable+$13*$04
arpindex	=	variable+$14*$04
arpnumbo	=	variable+$15*$04
arpspeed	=	variable+$16*$04
arponoto	=	variable+$17*$04
vibglbyt	=	variable+$18*$04
gldspeed	=	variable+$19*$04
glddirec	=	variable+$1a*$04
glddelay	=	variable+$1b*$04
destnote	=	variable+$1c*$04
vibdelay	=	variable+$1d*$04
vibindex	=	variable+$1e*$04
continos	=	variable+$1f*$04
amplitud	=	variable+$20*$04
ampindex	=	variable+$21*$04
fx01	=	variable+$22*$04
fx02	=	variable+$23*$04
fx03	=	variable+$24*$04
pulindex	=	variable+$27*$04
pulsbyt1	=	variable+$28*$04
spacespd	=	variable+$29*$04
spacecnt	=	variable+$2a*$04
;---------------------------------------------------------------------------------------------------------------------------------
4015wrd		=	$0f	
spacedl		=	$0b

	jmp	AUDIOIN
	jmp	AUDIOPL
	jmp	MUSICIN
	jmp	MUSICPL


MUSICIN	asl	a
	sta	seqposit,x	

	lda	#sex-s00-1
	sta	stpindex,x
	jmp	INITREALOS	 
	   
MUSICPL	
	lda	voictest,x
	beq	AUDIOEX
	cpx	#$00
	beq	VOICE01
	cpx	#$01
	beq	VOICE02
	cpx	#$02
	beq	VOICE03
	cpx	#$03
	beq	VOICE04
AUDIOEX	rts

VOICE04	txa
	asl	a
	asl	a
	tay
	lda	#$10		;Set Amplitude			<bit 04>
	ora	#$20		;Continuous lenght enabled	<bit 05>
	ora	amplitud,x	;Amplitude/Envelope Value	<bit 03-00>
	sta	$4000,y
	lda	lofrqwrd,x
	sta	$4002,y
	lda	#$f8
	sta	$4003,y
	jmp	JMPVO04	

VOICE03	txa
	asl	a
	asl	a
	tay
	lda	#$80
	jmp	JMPVO03
VOICE02
VOICE01 
	
setvoi1	txa
	asl	a
	asl	a
	tay
	lda	pulsbyt1,x	;setpulse			<bit 07-06>
	ora	#$10	;Set amplitude			<bit 04>
	ora	#$20		;continuous lenght enabled	<bit 05>
JMPVO03	ora	amplitud,x	;Amplitude/Envelope Value	<bit 03-00>
	sta	$4000,y

	lda	#$08		;Hardware-glide down		<bit 03>
	sta	$4001,y		;

	lda	lofrqwrd,x
	sta	$4002,y

	lda	hifrqwrd,x	;Check if hi-frq thesame as note before
	cmp	continos,x
	beq	NOHINO1
	sta	$4003,y		;HF set
	sta	continos,x
NOHINO1

JMPVO04

SPDHDL1	lda	#$00		;Speedroutine (two speed bytes)
	sta	realplay,x
	dec	spdcount,x
	bpl	SPDHDX1
	lda	spdcheck,x
	eor	#$ff
	sta	spdcheck,x
	bpl	SECONS1
	ldy	seqposit,x	
	lda	seqtabes+$00,y
	jmp	SPDSET1
SECONS1	ldy	seqposit,x
	lda	seqtabes+$01,y
SPDSET1	sta	spdcount,x
	lda	#$ff
	sta	realplay,x
SPDHDX1	
	lda	realplay,x	;check if new leng subtraction necessary
	bpl	NOSTPH1		;

AUDIPL1	dec	noteleng,x	;decrease speed, if zero then stephandle
	beq	STPHDL1	

NOSTPH1	lda	realnote,x	;check realnote on pause, no FX
	cmp	#$ff		;
	beq	NOFXPL1		;
	jsr	FXPLAY1		;

NOFXPL1	lda	#$00		;clear noteinit
	sta	noteinit,x	;
	rts			;

STPHDL1	ldy	stepnumb,x	;What step to take
	lda	stptablo,y	;
	sta	ssslo		;
	lda	stptabhi,y	;
	sta	ssshi		;

	lda	#$00		;reset glide (if not reached dest. note)
	sta	gldspeed,x

	ldy	stpindex,x	;read step-index
	lda	(ssslo),y	;
	cmp	#$ff		;step check on end 
	beq	SEQHDL1		;yes, goto sequence-handle
	jmp	STPHD12		;no, goto stephandle 02

SEQHDL1	dec	repstcnt,x	;check if step repeats
	beq	REALSQ1		;no, goto real sequence-handle
	lda	#$00		;yes, reset index & do last step again
	sta	stpindex,x	;
	tay			;
	lda	(ssslo),y	;
	jmp	STPHD12		;

REALSQ1	ldy	seqposit,x	;what sequence to take
	lda	seqtabel+$00,y	;
	sta	ssslo		;
	lda	seqtabel+$01,y	;
	sta	ssshi		;

	jsr	SEQZOOI

SEQHD19	sta	stepnumb,x	;set step & clear stepindex
	lda	#$00
	sta	stpindex,x
	inc	repstcnt,x	;make sure step plays once
	iny
	tya
	sta	seqindex,x
	jmp	STPHDL1		;back to step-handle

STPHD12 cmp	#$fd		;glide check in step
	bne	STPHD14
	iny
	lda	(ssslo),y
	sta	gldspeed,x
	iny
	lda	(ssslo),y
	clc
	adc	tranbyte,x
	sta	realnote,x
	iny
	lda	(ssslo),y
	clc
	adc	tranbyte,x
	sta	destnote,x
	iny
	jmp	NONEWN1

STPHD14	cmp	#$e0		;pause check
	bcc	STPHD15
	and	#$1f
	sta	realleng,x
	lda	#$ff
	sta	realnote,x
	lda	#$00
	sta	amplitud,x
	iny
	tya
	sta	stpindex,x
	jmp	PAUSEX1

STPHD15	cmp	#$c0		;sound check
	bcc	STPHD16
	clc
	adc	sfxtrans,x
	and	#$1f
	asl	a
	asl	a
	sta	realsoun,x
	iny
	lda	(ssslo),y
	cmp	#$fd	
	beq	STPHD12
	cmp	#$c0
	bcs	NONEWN1
	
STPHD16	cmp	#$80		;length check
	bcc	STPHD17
	and	#$7f
	clc
	adc	noteleng,x
	sta	noteleng,x
	sta	realleng,x
	iny
	lda	(ssslo),y
	cmp	#$fd
	beq	STPHD12
	cmp	#$c0
	bcs	NONEWN1
	cmp	#$80
	bcs	STPHD16

STPHD17	cmp	#$60		;arp check
	bcc	STPHD18
	and	#$1f	
	clc
	adc	arptrans,x
	sta	arpnumbo,x
	iny
	lda	(ssslo),y
	cmp	#$60
	bcs	NONEWN1

STPHD18	clc			;set note + note transpose
	adc	tranbyte,x	;
	sta	realnote,x	;
	iny			;
NONEWN1	tya			;
	sta	stpindex,x	;restore step-index
	
	inc	noteinit,x	;first init has been done
	ldy	realsoun,x	;
	cpx	#$00		;check sound-init VOICE 01
	beq	INITV12		;
	cpx	#$01		;check sound-init VOICE 02
	beq	INITV12		;
	cpx	#$02		;check sound-init VOICE 03
	beq	INITV03		;
	cpx	#$03		;check sound-init VOICE 04
	beq	INITV04		;
	rts

INITV12 lda	soundfix+$00,y	;check if new pulse to be set
	and	#$08		;
	bne	NONEWPL		;
	lda	soundfix+$00,y	;yes, set new pulse
	and	#$c0		;
	sta	pulsbyt1,x	;
INITV03
INITV04
NONEWPL	lda	soundfix+$01,y	;set vibrato-byte into fx01 & set effects (arp, etc)
	sta	fx01,x		;
	lda	soundfix+$02,y	;set vibdelay pultab-number into fx02
	sta	fx02,x		;
	
       ;lda	soundfix+$01,y	;set gate-leng value
       ;and	#$f0		;
       ;lsr	a		;
       ;lsr	a		;
       ;lsr	a		;
       ;lsr	a		;
       ;sta	gateleng,x	;

NONEWG1	lda	soundfix+$03,y	;set effect-values into fx03	
	sta	fx03,x		;
	cpx	#$03
	beq	NFORV04

	ldy	realnote,x	;Read realnote as index
	lda	frqtablo,y	;Low-frequency read	<bit 07-00>
	sta	lofrqwrd,x	;

        lda	#$f8		;hardware length endless	<bit 07-03>
        ora	frqtabhi,y	;hi-frequency read		<bit 02-00>
        sta	hifrqwrd,x	;HF set
	jmp	PAUSEX1

NFORV04	lda	realnote,x
	sta	lofrqwrd,x

PAUSEX1	lda	realleng,x
	sta	noteleng,x
	rts

;INITV03	rts

FXPLAY1 lda	noteinit,x
	bne	INTPLA1
	jmp	PLAYFXR

INTPLA1	
	jsr	INTPLAYOS

FXPLA12	jsr	CLEARZOOI

	lda	realleng,x
	sec
	sbc	vibdelay,x
	bcs	REFIX01
	lda	#$00
	sta	fx01,x
	beq	EXITIN1
REFIX01	sta	vibdelay,x
EXITIN1	jmp	NOVIB01	

PLAYFXR	lda	fx03,x		;amplitude routine		
	and	#$7f		;maximum of $7f amplitude tabels
	tay
	lda	amptabl,y
	sta	ssslo
	lda	amptabh,y
	sta	ssshi
       
	jsr	SHITAMP


GLIDE1	ldy	gldspeed,x	;glide routine
	beq	NOGLD12

	lda	glddelay,x
	cmp	noteleng,x
	bcc	NOGLD12

	ldy	glddirec,x
	bmi	DOWNGL1
	lda	lofrqwrd,x
	clc
	adc	gldspeed,x
	sta	lofrqwrd,x
	ldy	destnote,x
	sec
	sbc	frqtablo,y
	bcs	FROTER1
	jmp	NOGLD01

DOWNGL1	lda	lofrqwrd,x
	sec
	sbc	gldspeed,x
	sta	lofrqwrd,x
	ldy	destnote,x
	sec
	sbc	frqtablo,y
	bcs	NOGLD01
FROTER1	lda	frqtablo,y
	sta	lofrqwrd,x
	
	lda	#$00
	sta	gldspeed,x
NOGLD01	jmp	NOVIB01
NOGLD12

	
VIBRA01	lda	fx01,x		;vibrato routine
	and	#$0f
	beq	NOVIB01
	lda	vibdelay,x
	cmp	noteleng,x
	bcc	NOVIB01

	lda	fx01,x
	and	#$0f
	tay
	lda	vibtabl-1,y
	sta	ssslo
	lda	vibtabh-1,y
	sta	ssshi
	
	jsr	JSRVIB

HOGERVB
	lda	lofrqwrd,x
	clc
	adc	(ssslo),y
	bcc	VIBNO01
	clc
VIBNO01	sta	lofrqwrd,x
	iny
	tya
	sta	vibindex,x
NOVIB01

PULSE1	cpx	#$02
	bcs	NOPULS1
	lda	fx02,x		;PULSE ROUTINE
	and	#$0f
	beq	NOPULS1
	tay
	lda	pultabl-1,y
	sta	ssslo
	lda	pultabh-1,y
	sta	ssshi

	jsr	JSRPUL
NOPULS1	

SPACE01	lda	fx01,x		;SPACE ROUTINE
	and	#$20
	beq	NOSPC01
	lda	noteleng,x
	cmp	#spacedl
	bcs	NOSPC01
	lda	#$00
	sta	spacespd,x
	ldy	realnote,x
	lda	spacecnt,x
	and	#$01
	bne	NORNOT1
	lda	spacecnt,x
	lsr	a
	sta	spacespd,x
NORNOT1	lda	frqtablo,y
	clc
	adc	spacespd,x
	sta	lofrqwrd,x
	inc	spacecnt,x
	inc	spacecnt,x
NOSPC01	

ARPEG01	lda	fx01,x		;ARP ROUTINE
	and	#$40		
	bne	YESAR01
	rts	
YESAR01
	ldy	arpnumbo,x
	lda	arptablo,y
	sta	ssslo
	lda	arptabhi,y
	sta	ssshi
	jmp	ARPJSR
;---------------------------------------------------------------------------------------------------------------------------------
frqtablo	db	$ad,$4a,$ef,$9b,$4b,$fe,$b5,$73,$30,$f5,$bd,$87 ;1
		db	$56,$25,$f8,$cd,$a5,$7f,$5b,$3a,$19,$fb,$de,$c3 ;2
		db	$aa,$92,$7b,$66,$52,$3f,$2d,$1c,$0c,$fd,$ef,$e1 ;3
		db	$d4,$c9,$bd,$b3,$a9,$9f,$96,$8e,$86,$7e,$77,$70 ;4
		db	$6a,$64,$5e,$59,$54,$4f,$4a,$47,$42,$3e,$3b,$38 ;5
		db	$35,$31,$2e,$2c,$29,$27,$25,$23,$21,$1f,$1d,$1b ;6
		db	$1a,$19,$18,$16,$15,$14,$13,$12,$11,$10,$0f,$0e ;7
		db	$ff		

frqtabhi	db	$06,$06,$05,$05,$05,$04,$04,$04,$04,$03,$03,$03 ;1
		db	$03,$03,$02,$02,$02,$02,$02,$02,$02,$01,$01,$01 ;2
		db	$01,$01,$01,$01,$01,$01,$01,$01,$01,$00,$00,$00 ;3
		db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;4
		db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;5
		db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;6
		db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;7
		db	$07

						; lda ldx
seqtabel	db	<clearsfx,>clearsfx	; $00 $xx
		db	<medibad1,>medibad1	; $01 $00 
		db	<medibad2,>medibad2	; $02 $01 
		db	<medibad3,>medibad3	; $03 $02 
		db	<medibad4,>medibad4	; $04 $03 
		db	<notgood1,>notgood1	; $05 $00 
		db	<notgood2,>notgood2	; $06 $01 
		db	<notgood3,>notgood3	; $07 $02 
		db	<notgood4,>notgood4	; $08 $03 
		db	<betters1,>betters1	; $09 $00 
		db	<betters2,>betters2	; $0a $01 
		db	<betters3,>betters3	; $0b $02 
		db	<betters4,>betters4	; $0c $03 
		db	<bestbes1,>bestbes1	; $0d $00 
		db	<bestbes2,>bestbes2	; $0e $01 
		db	<bestbes3,>bestbes3	; $0f $02 
		db	<bestbes4,>bestbes4	; $10 $03 
		db	<victory1,>victory1	; $11 $00 
		db	<victory2,>victory2	; $12 $01 
		db	<victory3,>victory3	; $13 $02 
		db	<doomned1,>doomned1	; $14 $00 
		db	<doomned2,>doomned2	; $15 $01 
		db	<introdu1,>introdu1	; $16 $00 
		db	<introdu2,>introdu2	; $17 $01 
		db	<introdu3,>introdu3	; $18 $02 
		db	<introdu4,>introdu4	; $19 $03 
		db	<explosi1,>explosi1	; $1a $02 
		db	<explosi2,>explosi2	; $1b $03 
		db	<firewrxu,>firewrxu	; $1c $03 
		db	<firewrxe,>firewrxe	; $1d $03 


seqtabes	db	$00,$00			; $00 $xx
		db	$02,$03			; $01 $00
		db	$02,$03			; $02 $01
		db	$02,$03			; $03 $02
		db	$02,$03			; $04 $03
		db	$02,$03			; $05 $00
		db	$02,$03			; $06 $01
		db	$02,$03			; $07 $02
		db	$02,$03			; $08 $03
		db	$02,$03			; $09 $00
		db	$02,$03			; $0a $01
		db	$02,$03			; $0b $02
		db	$02,$03			; $0c $03
		db	$02,$03			; $0d $00
		db	$02,$03			; $0e $01
		db	$02,$03			; $0f $02
		db	$02,$03			; $10 $03
		db	$03,$03			; $11 $00
		db	$03,$03			; $12 $01
		db	$03,$03			; $13 $02
		db	$04,$04			; $14 $00
		db	$04,$04			; $15 $01
		db	$03,$03			; $16 $00
		db	$03,$03			; $17 $01
		db	$03,$03			; $18 $02
		db	$03,$03			; $19 $03
		db	$02,$02			; $1a $02
		db	$02,$02			; $1b $03
		db	$02,$02			; $1c $03
		db	$02,$02			; $1d $03
	
stptablo	db	<s00,<s01,<s02,<s03,<s04,<s05,<s06,<s07,<s08,<s09,<s0a,<s0b,<s0c,<s0d,<s0e,<s0f
		db	<s10,<s11,<s12,<s13,<s14,<s15,<s16,<s17,<s18,<s19,<s1a,<s1b,<s1c,<s1d,<s1e,<s1f
		db	<s20,<s21,<s22,<s23,<s24,<s25,<s26,<s27,<s28,<s29,<s2a,<s2b,<s2c,<s2d,<s2e,<s2f
		db	<s30,<s31,<s32,<s33,<s34,<s35

stptabhi	db	>s00,>s01,>s02,>s03,>s04,>s05,>s06,>s07,>s08,>s09,>s0a,>s0b,>s0c,>s0d,>s0e,>s0f
		db	>s10,>s11,>s12,>s13,>s14,>s15,>s16,>s17,>s18,>s19,>s1a,>s1b,>s1c,>s1d,>s1e,>s1f
		db	>s20,>s21,>s22,>s23,>s24,>s25,>s26,>s27,>s28,>s29,>s2a,>s2b,>s2c,>s2d,>s2e,>s2f
		db	>s30,>s31,>s32,>s33,>s34,>s35

amptabl		db	<am00,<am01,<am02,<am03,<am04,<am05,<am06,<am07,<am08,<am09,<am0a,<am0b,<am0c,<am0d
amptabh		db	>am00,>am01,>am02,>am03,>am04,>am05,>am06,>am07,>am08,>am09,>am0a,>am0b,>am0c,>am0d

vibtabl		db	<v01,<v02,<v03,<v04,<v05
vibtabh		db	>v01,>v02,>v03,>v04,>v05

pultabl		db	<p01,<p02,<p03,<p04,<p05,<p06,<p07
pultabh		db	>p01,>p02,>p03,>p05,>p05,>p06,>p07

arptablo	db	<a00,<a01,<a02,<a03,<a04,<a05,<a06,<a07,<a08,<a09,<a0a,<a0b,<a0c,<a0d,<a0e,<a0f
		db	<a10,<a11,<a12,<a13,<a14,<a15,<a16,<a17,<a18,<a19,<a1a,<a1b,<a1c
arptabhi	db	>a00,>a01,>a02,>a03,>a04,>a05,>a06,>a07,>a08,>a09,>a0a,>a0b,>a0c,>a0d,>a0e,>a0f
		db	>a10,>a11,>a12,>a13,>a14,>a15,>a16,>a17,>a18,>a19,>a1a,>a1b,>a1c
;---------------------------------------------------------------------------------------------------------------------------------
am00		
	db	9,8,7,6,5,5,4,4,3,3,2,$fe
am01
	db	1,$fe
am02
	db	6,5,4,4,3,3,3,2,$fe
am03	   
	db	7,6,5,5,4,4,4,4,3,3,3,3,3,2,$fe
am04	
	db	2,2,1,$fe       
am05		
	db	4,3,3,2,2,2,2,2,1,$fe
am06
	db	4,4,3,3,3,2,$fe
am07 
	db	3,3,3,3,3,3,3,2,2,2,2,2,2,1,$fe
am08		
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,$fe
am09
	db	1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
	db	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3
	db	3,3,3,3,3,3,3,3,2,$fe
am0a
	db	7,6,5,4,4,3,3,3,2,$fe
am0b
	db	6,5,5,5,4,4,4,4,3,3,3,3,3,3,2,$fe
am0c
	db	15,15,15,14,14,14,13,13,13,12,12,12,11,11,11,10,10,10,9,9,9
	db	8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,5,5,5,5
	db	5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,2,2,2,2,1,$fe
am0d		
	db	15,12,9,8,7,7,6,6,6,5,5,5,4,4,4,3,3,3,2,2,2,1,1,1
	db	2,2,2,3,3,3,2,2,2,1,1,1,1,1,1,0,$fe
;---------------------------------------------------------------------------------------------------------------------------------
v01		
	db	$02,$02,$02,$02,$fe,$fe,$fe,$fe
	db	$fe,$fe,$fe,$fe,$02,$02,$02,$02
	db	$80,0
v02
	db	$05,$05,$05,$05,$fb,$fb,$fb,$fb
	db	$fb,$fb,$fb,$fb,$05,$05,$05,$05
	db	$80,0
v03	     
	db	$01,$00,$01,$00,$ff,$00,$ff,$00
	db	$ff,$00,$ff,$00,$01,$00,$01,$00
	db	$80,0	
v04
	db	$07,$07,$07,$f9,$f9,$f9,$f9,$f9,$f9,$07,$07,$07
	db	$80,0
v05
	db	$01,$01,$01,$01,$ff,$ff,$ff,$ff
	db	$ff,$ff,$ff,$ff,$01,$01,$01,$01
	db	$80,0
;---------------------------------------------------------------------------------------------------------------------------------
p01		
	db	$00,$00,$00,$00,$00,$00,$40,$40,$40,$40,$40,$40
	db	$80,$80,$80,$80,$80,$80,$c0,$c0,$c0,$c0,$c0,$c0
	db	$80,$80,$80,$80,$80,$80,$40,$40,$40,$40,$40,$40
	db	$ff,$00
p02	
	db	$40,$40,$40,$40,$80,$80,$80,$80
	db	$c0,$c0,$c0,$c0,$80,$80,$80,$80
	db	$ff,$00
p03		
	db	$40,$40,$40,$40,$40,$40,$40,$40
	db	$80,$80,$80,$80,$80,$80,$80,$80
	db	$ff,$00
p04		
	db	$80,$80,$80,$40,$40,$40,$00,$00,$40,$40,$40,$80,$80,$80,$40,$40
	db	$80,$80,$80,$80,$80,$80,$80,$80,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0
	db	$80,$80,$80,$80,$80,$80,$80,$80,$40,$40,$40,$40,$40,$40,$40,$40
	db	$ff,$00
p05		
	db	$80,$80,$80,$80,$80,$80,$80,$80
	db	$40,$40,$40,$40,$40,$40,$40,$40
	db	$00,$00,$00,$00,$00,$00,$00,$00
	db	$40,$40,$40,$40,$40,$40,$40,$40
	db	$ff,$00
p06		
	db	$80,$80,$40,$40,$40,$40,$40,$40,$40,$40
	db	$80,$80,$80,$80,$80,$80,$c0,$c0,$c0,$c0,$c0,$c0
	db	$ff,$0a
p07		
	db	$80,$40,$00,$40,$00
	db	$40,$40,$40,$40,$40,$40,$40,$40
	db	$80,$80,$80,$80,$80,$80,$80,$80
	db	$ff,$05
;---------------------------------------------------------------------------------------------------------------------------------
a00	db	$20,$00,$03,$07,12,$ff
a01	db	$20,$00,$04,$07,12,$ff
a02	db	$20,$00,$05,$08,12,$ff
a03	db	$20,$00,$05,$09,12,$ff	
a04	db	$20,$00,$03,$08,12,$ff	
a05	db	$20,$00,$04,$09,12,$ff
a06	db	$00,$9c,$94,$90,$00,$fe
a07	db	$00,$00,$fe
a08	db	$50,8,7,6,5,4,3,2,1,0,$fe
a09	db	$20,$00,$05,$07,12,$ff
a0a	db	$00,$98,$8c,$80,$00,$fe
a0b	db	$00,4,0,$fe
a0c	db	$00,$9c,$94,$90,$8c,$88,0,$fe
a0d	db	$30,0,5,8,$ff
a0e	db	$30,0,4,9,$ff
a0f	db	$30,0,4,7,$ff
a10	db	$10,12,7,3,0,$ff	
a11	db	$10,12,7,4,0,$ff
a12	db	$10,12,8,5,0,$ff
a13	db	$10,12,9,5,0,$ff
a14	db	$10,12,8,3,0,$ff
a15	db	$10,12,9,4,0,$ff
a16	db	$10,12,7,5,0,$ff
a17	db	$10,12,10,5,0,$ff
a18	db	$10,10,9,5,0,$ff
a19	db	$00,$a1,$9f,$9e,$00,$fe
a1a	db	$70,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,$fe
a1b	db	$00
	db	24,21,19,18,21,18,16,15,18,15,13,12,15,12,10,9,12,9,7,6
	db	9,6,4,3,6,3,1,0,$fe
a1c	
	db	$00,12,13,14,15,14,13,12,11,10,9,9,8,8,9,9,10,10,11,11,12,12
	db	13,13,14,$fe
;---------------------------------------------------------------------------------------------------------------------------------
soundfix	

;LeadNrm (1)
		db	$00,$01,$41,$00	;C0   
;LeadArp (2)
		db	$00,$40,$01,$03	;C1   
;BassArp (3)
		db	$00,$42,$40,$00	;C2   
;Sissers (4)
		db	$00,$40,$00,$01	;C3   
;LeadNrm (1)
		db	$80,$03,$42,$02	;C4   
;FillNrm (1)
		db	$00,$01,$41,$02	;C5   
;LeadSPC (1)
		db	$80,$23,$42,$02	;C6   
;LeadSOF (1)
		db	$80,$03,$42,$04	;C7   
;LeadFLT (1)
		db	$40,$03,$43,$04	;C8   
;LEADFLT (1)
		db	$40,$03,$43,$05	;C9   
;REALEAD (1)
		db	$00,$03,$44,$06	;CA   
;FIDDLES (1)
		db	$80,$03,$40,$01	;CB   
;BassVib (3)
		db	$00,$44,$40,$00	;CC   
;ARPPULS (1)
		db	$00,$40,$02,$03	;CD   
;LEADARP (1)
		db	$80,$40,$02,$02	;CE   
;leadarp (1)
		db	$80,$40,$00,$01	;CF   
;DOOMNAR (1)
		db	$80,$40,$55,$08	;D0   
;doomned (1)
		db	$80,$40,$46,$01	;D1   
;DOOMERS (1)
		db	$80,$03,$46,$07	;D2   
;DOOMNAR (1)
		db	$80,$40,$55,$09	;D3   
;SISSERS (4)
		db	$00,$00,$00,$04	;D4   
;SISDrum (4)
		db	$00,$00,$00,$0a	;D5   
;LEADFLT (1)
		db	$80,$05,$47,$0b	;D6   
;LEADARP (1)
		db	$80,$45,$07,$0b	;D7   
;LEADSPC (1)
		db	$80,$25,$47,$0b	;D8   
;leadFLT (1)
		db	$80,$05,$47,$04	;D9   
;EXPLOSI (4)
		db	$00,$40,$00,$0c	;DA   
;FIREBAM (4)
		db	$00,$40,$00,$0d	;DB   
;---------------------------------------------------------------------------------------------------------------------------------
medibad1	db	$85,$0a
		db	$fe

medibad2	db	$85,$0b
		db	$fe

medibad3	db	$85,$0c
		db	$fe

medibad4	db	$80,$0d
		db	$fe

s0a		db	$c4,$90,$1f,$1b,$20,$1f
		db	$8c,$1b,$84,$1d,$86,$1a,$18,$84,$16
		db	$94,$18,$82,$1d,$1d,$84,$e1
		db	$ff
s0b
		db	$c1,$90,$60,$18
		db	$61,$14
		db	$c1,$19
		db	$60,$18
		db	$8c,$61,$14
		db	$84,$65
		db	$86,$60,$13
		db	$61,$11
		db	$84,$0f
		db	$94,$69,$11,$82,$69,$69,$84,$69,$11,$e1
		db	$ff
s0c
		db	$c2,$90,$66,$0c
		db	$90,$08
		db	$0d
		db	$0c
		db	$88,$08
		db	$08
		db	$86,$07
		db	$05
		db	$84,$03
		db	$05,$67,$00,$66,$03,$67,$00,$84,$66,$05,$82,$05,$05,$84,$05,$e1
		db	$ff
s0d
		db	$c3,$90,$68,$00
		db	$c3
		db	$c3
		db	$c3
		db	$c3,$88
		db	$c3
		db	$c3,$86
		db	$c3
		db	$c3,$84
		db	$c3,$88
		db	$c3
		db	$c3,$85,$00,$e1
		db	$ff

notgood1	db	$81,$0e
		db	$fe

notgood2	db	$81,$0f
		db	$fe

notgood3	db	$81,$10
		db	$fe

notgood4	db	$81,$11
		db	$fe

s0e
		db	$c4,$88,$24
		db	$84,$29
		db	$88,$25
		db	$84,$2c
		db	$88,$2a
		db	$84,$31
		db	$8c,$30
		db	$84,$2f
		db	$2b
		db	$26
		db	$2e
		db	$2a
		db	$27
		db	$8c,$29
		db	$84,$1d,$22,$24,$86,$29,$e1
		db	$ff		      
s0f
		db	$e5
		db	$c7,$88,$24
		db	$84,$29
		db	$88,$25
		db	$84,$2c
		db	$88,$2a
		db	$84,$31
		db	$8c,$30
		db	$84,$2f
		db	$2b
		db	$26
		db	$2e
		db	$2a
		db	$27
		db	$87,$29
		db	$c4,$84,$18,$1d,$1d,$85,$21,$e1
		db	$ff
s10
		db	$c2,$88,$66,$0c
		db	$84,$0c
		db	$0d,$67
		db	$66
		db	$88,$0f
		db	$84,$0f
		db	$8c,$11
		db	$88,$0b
		db	$84,$0e
		db	$0d,$67
		db	$66,$12
		db	$8c,$11
		db	$84,$05,$0a,$03,$84,$05,$e1
		db	$ff
s11
		db	$c3,$88,$68,$00
		db	$84,0
		db	$88,0
		db	$84,0
		db	$88,0
		db	$84,0
		db	$8c,0
		db	$88,0
		db	$84,0
		db	$88,0
		db	$84,0
		db	$8c,0
		db	$84,$68,$00,$00,$00,$87,$00,$e1
		db	$ff

betters1	db	$81,$12
		db	$fe

betters2	db	$81,$13
		db	$fe

betters3	db	$81,$14
		db	$fe

betters4	db	$80,$15
		db	$fe

s12		
		db	$c4,$84,$18
		db	$1a
		db	$1b
		db	$1f
		db	$c4,$88,$1d
		db	$84,$1a
		db	$16
		db	$18
		db	$1a
		db	$1b
		db	$1f
		db	$88,$22
		db	$24

		db	$84,$21
		db	$1d
		db	$18
		db	$1a
		db	$1b
		db	$82,$1d
		db	$1b
		db	$84,$1a
		db	$16
		db	$18
		db	$18
		db	$16
		db	$18,$e1
		db	$ff
s13		
		db	$cd,$90,$70,$18
		db	$71,$16
		db	$70,$18
		db	$88,$71,$16
		db	$70,$18
		db	$90,$73
		db	$88,$16
		db	$71
		db	$c1,$84,$76,$18
		db	$76
		db	$c1,$16
		db	$18,$e1
		db	$ff
s14
		db	$c2,$88,$66,$0c
		db	$0c
		db	$0a
		db	$0a
		db	$0c
		db	$0c
		db	$0a
		db	$0c
		db	$11
		db	$11
		db	$0f
		db	$0a
		db	$84,$0c
		db	$0c
		db	$0a
		db	$0c,$e1
		db	$ff
s15
		db	$c3,$90,$68,$00
		db	$88,$00
		db	$00
		db	$90,$00
		db	$88,$00
		db	$00
		db	$90,$00
		db	$88,$00
		db	$00
		db	$84,$00
		db	$00
		db	$00
		db	$00,$e1
		db	$ff

bestbes1	db	$8d,$16
		db	$fe
			  
bestbes2	db	$8d,$17
		db	$fe
			  
bestbes3	db	$84,$18
		db	$fe

bestbes4	db	$80,$19
		db	$fe

s16
		db	$c4,$84,$0c
		db	$8c,$13
		db	$84,$0c
		db	$8c,$13
		db	$84,$0c
		db	$88,$16
		db	$86,$15,$81,$16,$15
		db	$8c,$13
		db	$84,$0c
		db	$8c,$13
		db	$84,$0c
		db	$8c,$13
		db	$84,$0c
		db	$88,$11
		db	$86,$10,$81,$11,$10
		db	$8c,$0e

		db	$84,$0c
		db	$8c,$13
		db	$84,$0c
		db	$8c,$13
		db	$84,$0c
		db	$88,$16
		db	$18
		db	$84,$15,$82,$16,$15
		db	$84,$13
		db	$84,$11
		db	$88,$13
		db	$0c
		db	$11
		db	$0a
		db	$8e,$0c,$81,$10,$13
		db	$84,$18,$e1
		db	$ff
s17
		db	$e4
		db	$c1,$90,$71,$0c
		db	$90,$0c
		db	$88,$0f
		db	$11
		db	$90,$0c
		db	$0c
		db	$0c
		db	$88,$0a
		db	$0c
		db	$90,$07
		db	$0c
		db	$0c
		db	$88,$0f
		db	$0f
		db	$11
		db	$73,$0c
		db	$71,$0c
		db	$0c
		db	$74,$0e
		db	$73,$11
		db	$90,$13
		db	$84,$71,$0c,$e1
		db	$ff
s18
		db	$e4
		db	$c2,$8c,$66,$09
		db	$84,$09
		db	$8c,$09
		db	$84,$09
		db	$88,$0c
		db	$0e
		db	$8c,$09
		db	$84,$09
		db	$8c,$09
		db	$84,$09
		db	$8c,$09
		db	$84,$09
		db	$88,$07
		db	$09
		db	$8c,$04
		db	$84,$04
		db	$8c,$09
		db	$84,$09
		db	$8c,$09
		db	$84,$09
		db	$88,$0c
		db	$0c
		db	$0e
		db	$0e
		db	$09
		db	$09
		db	$04
		db	$04
		db	$90,$09
		db	$84,$09,$e1
		db	$ff
s19
		db	$e4
		db	$c3,$90,$68,$00
		db	$90,$00
		db	$88,$00
		db	$00
		db	$90,$00
		db	$00
		db	$00
		db	$88,$00
		db	$00
		db	$90,$00
		db	$00
		db	$00
		db	$88,$00
		db	$00
		db	$00
		db	$00
		db	$00
		db	$00
		db	$00
		db	$00
		db	$90,$00
		db	$84,$00,$e1
		db	$ff

victory1	db	$88,$1a,$1d
		db	$ff,$00

victory2	db	$88,$1b,$1e
		db	$ff,$00

victory3	db	$86,$1c,$1f
		db	$ff,$00

s1a
		db	$c4,$84,$18		;a
		db	$82,$18
		db	$18
		db	$18
		db	$18
		db	$86,$1f
		db	$82,$1f
		db	$1d
		db	$1f
		db	$21,$cb,$1f
		db	$c4,$1d
		db	$92,$1f
		db	$84,$18
		db	$82,$18
		db	$18
		db	$18
		db	$18
		db	$86,$1f
		db	$82,$1f
		db	$1d
		db	$1f
		db	$21,$cb,$1f
		db	$c4,$1d
		db	$8e,$1f
		db	$82,$1f
		db	$21

		db	$8a,$22
		db	$82,$24
		db	$86,$21
		db	$1d
		db	$94,$1f
		db	$82,$1f
		db	$21
		db	$8a,$22
		db	$82,$24
		db	$87,$21
		db	$88,$1d
		db	$a4,$1f

		db	$84,$18		;b
		db	$81,$18,$18
		db	$82,$18
		db	$18
		db	$18
		db	$86,$1f
		db	$82,$1f
		db	$1d
		db	$1f
		db	$21,$cb,$1f
		db	$c4,$1d
		db	$92,$1f
		db	$84,$18
		db	$81,$18,$18
		db	$82,$18
		db	$18
		db	$18
		db	$86,$1f
		db	$82,$1f
		db	$1d
		db	$1f
		db	$21,$cb,$1f
		db	$c4,$1d
		db	$8e,$1f
		db	$82,$1f
		db	$21

		db	$8a,$22
		db	$82,$24
		db	$86,$21
		db	$1d
		db	$94,$1f
		db	$82,$1f
		db	$21
		db	$8a,$22
		db	$82,$24
		db	$86,$21
		db	$1d
		db	$98,$1f
		db	$ff
s1b
		db	$c4,$84,$13		;a
		db	$82,$13
		db	$13
		db	$13
		db	$13
		db	$86,$18
		db	$82,$18
		db	$18
		db	$18
		db	$1d,$cb,$18
		db	$c4,$18
		db	$92,$18
		db	$84,$13
		db	$82,$13
		db	$13
		db	$13
		db	$13
		db	$86,$18
		db	$82,$18
		db	$18
		db	$18
		db	$1d,$cb,$18
		db	$c4,$18
		db	$8e,$18
		db	$82,$18
		db	$1a

		db	$c5,$8a,$1b		
		db	$82,$1d
		db	$86,$18
		db	$15
		db	$94,$18
		db	$82,$18
		db	$1a
		db	$8a,$1b
		db	$82,$1d
		db	$87,$18
		db	$88,$15
		db	$a4,$17

		db	$ce,$84,$71,$0c		;b
		db	$81,$0c,$0c
		db	$82,$0c
		db	$0c
		db	$0c
		db	$86,$73,$13
		db	$82,$13
		db	$78,$13
		db	$73,$13
		db	$74,$15,$cf,$73,$13
		db	$ce,$71,$11
		db	$92,$73,$13
		db	$84,$71,$0c
		db	$81,$0c,$0c
		db	$82,$0c
		db	$0c
		db	$0c
		db	$86,$73,$13
		db	$82,$13
		db	$78
		db	$73
		db	$74,$15,$cf,$73,$13
		db	$ce,$71,$11
		db	$8e,$73,$13
		db	$82,$13
		db	$74,$15

		db	$8a,$73,$16
		db	$82,$18
		db	$86,$74,$15
		db	$71,$11
		db	$94,$73,$13
		db	$82,$13
		db	$74,$15
		db	$8a,$73,$16
		db	$82,$18
		db	$86,$74,$15
		db	$71,$11
		db	$8c,$76,$13
		db	$71,$13
		db	$ff
s1c
		db	$cc,$8c,$6c,$02	;a
		db	$02
		db	$86,$07
		db	$02
		db	$8c,$02
		db	$02
		db	$02
		db	$86,$07
		db	$02
		db	$8c,$02

		db	$c3,$88,$05
		db	$cc,$82,$6b,$07
		db	$09
		db	$86,$6c,$07
		db	$00
		db	$8c,$02
		db	$86,$02
		db	$04
		db	$c3,$88,$05
		db	$cc,$82,$6b,$07
		db	$09
		db	$87,$6c,$07
		db	$88,$0b
		db	$89,$09
		db	$04
		db	$09
		db	$04

		db	$8c,$02	;b
		db	$02
		db	$86,$07
		db	$02
		db	$8c,$02
		db	$02
		db	$02
		db	$86,$07
		db	$02
		db	$8c,$02

		db	$c3,$88,$05
		db	$cc,$82,$6b,$07
		db	$09
		db	$86,$6c,$07
		db	$00
		db	$8c,$02
		db	$86,$02
		db	$04
		db	$c3,$88,$05
		db	$cc,$82,$6b,$07
		db	$09
		db	$86,$6c,$07
		db	$0b
		db	$09
		db	$04
		db	$07
		db	$09
		db	$ff
s1d
		db	$c4,$84,$20	;c
		db	$84,$1f
		db	$1b
		db	$18
		db	$1a
		db	$1b
		db	$81,$1a,$85,$1b
		db	$84,$1a,$81,$1b,$1a
		db	$86,$18
		db	$16
		db	$84,$20
		db	$1f
		db	$1b
		db	$18
		db	$1a
		db	$1b
		db	$81,$1b,$97,$1d
		db	$84,$20
		db	$1f
		db	$1b
		db	$18
		db	$1a
		db	$1b
		db	$81,$1b,$85,$1d
		db	$84,$1a,$81,$1b,$1a
		db	$86,$18
		db	$16
		db	$84,$20
		db	$1f
		db	$1b
		db	$22
		db	$24
		db	$26
		db	$81,$23,$97,$24
		db	$ff
s1e
		db	$cd,$8c,$74,$0c		;c
		db	$0c
		db	$86,$0e
		db	$0e
		db	$73,$11
		db	$74,$0e
		db	$8c,$0c
		db	$0c
		db	$86,$0e
		db	$77,$11
		db	$73
		db	$74,$0e
		db	$8c,$0c		;c
		db	$0c
		db	$86,$0e
		db	$0e
		db	$73,$11
		db	$74,$0e
		db	$8c,$0c
		db	$0e
		db	$98,$10
		db	$ff
s1f
		db	$cc,$86,$6c,$0a
		db	$05
		db	$0a
		db	$05
		db	$0c
		db	$07
		db	$0c
		db	$07
		db	$0a
		db	$05
		db	$0a
		db	$05
		db	$0c
		db	$07
		db	$84,$11
		db	$10
		db	$0c
		db	$86,$0a
		db	$05
		db	$0a
		db	$05
		db	$0c
		db	$07
		db	$0c
		db	$07
		db	$0a
		db	$05
		db	$0c
		db	$07
		db	$02
		db	$09
		db	$8c,$0e
		db	$ff

doomned1	db	$81,$22
		db	$ff,$00
doomned2	db	$81,$71,$23,$70,$24
		db	$ff,$00

s22
		db	$d0,$a0,$6d,$13
		db	$6e,$14
		db	$6f,$13
		db	$6d
		db	$ff
s23
		db	$e3
		db	$d0,$a0,$6d,$13
		db	$6e,$14
		db	$6f,$13
		db	$6d
		db	$ff
s24
		db	$d1,$85,$6d,$13
		db	$d2,$84,$18,$1a
		db	$88,$fd,$4f,$1b,$18
		db	$84,$18
		db	$88,$fd,$0c,$1d,$1f
		db	$84,$1d
		db	$1b
		db	$88,$fd,$0c,$1b,$1d
		db	$84,$1f
		db	$1b
		db	$1d
		db	$88,$fd,$0f,$18,$1a
		db	$1a
		db	$fd,$0f,$18,$1a
		db	$84,$1b,$1d
		db	$86,$fd,$0f,$1b,$1d
		db	$1b
		db	$84,$1a
		db	$8c,$18
		db	$84,$13

		db	$86,$fd,$0f,$16,$18
		db	$18
		db	$84,$1a
		db	$1b
		db	$18
		db	$1f
		db	$22
		db	$fd,$0f,$20,$22
		db	$20
		db	$1f
		db	$88,$1d
		db	$84,$1d
		db	$1f
		db	$20
		db	$88,$fd,$0f,$1d,$1f
		db	$1a
		db	$86,$13
		db	$0f
		db	$84,$0e
		db	$a0,$fd,$0f,$0b,$0c
		db	$ff



introdu1	db	$86,$70,$26,$26,$26,$26,$71,$2c		;intro
		db	$70,$62,$01				;actio
		db	$04,$71,$2c,$70,$62,$01			;overl
		db	$08					;OVERL
		db	$ff,$04

introdu2	db	$86,$28,$27,$27,$27,$27,$2d,$2c		;intro
		db	$03,$02,$30				;actio
		db	$05,$2c,$03,$02,$31			;overl
		db	$09,$28					;OVERL
		db	$ff,$04

introdu3	db	$6e,$29,$2a,$2b,$2b			;intro
		db	$62,$20					;actio
		db	$06,$63,$20				;overl
		db	$2e					;OVERL
		db	$ff,$04

introdu4        db	$6f,$29,$6f,$29,$67,$25			;intro
		db	$6b,$21				;actio
		db	$63,$07,$6f,$21				;overl
		db	$2f,$67,$25				;OVERL
		db	$ff,$05

s26
		db	$c8
		db	$88,$0d,$13,$0d,$13
		db	$0d,$13,$0d,$13
		db	$0c,$13,$0c,$13
		db	$0c,$13,$0c,$13
		db	$ff
s27
		db	$c8
		db	$88,$11,$19,$11,$19
		db	$11,$19,$11,$19
		db	$11,$18,$11,$18
		db	$11,$18,$11,$18
		db	$ff
s28
		db	$e4
		db	$ff
s29
		db	$f0
		db	$ff
s2a
		db	$e4
		db	$c2,$84,$67,$02
		db	$6a,$04
		db	$67,$06
		db	$ff
s2b
		db	$c2,$b0,$6a,$07
		db	$84,$09,$67,$09
		db	$88,$6a,$07
		db	$b0,$06
		db	$88,$04
		db	$06
		db	$b8,$07
		db	$88,$0c
		db	$b0,$0b
		db	$88,$04
		db	$06
		db	$ff
s2c
		db	$c8,$84,$0d,$11,$13,$19
		db	$11,$13,$19,$1d
		db	$19,$1d,$20,$1d
		db	$25,$24,$22,$20
		db	$1f,$20,$1f,$1d
		db	$1c,$19,$18,$16
		db	$82,$18,$19,$81,$18,$16,$14,$16
		db	$82,$18,$19,$81,$18,$1d,$1f,$24
		db	$84,$18,$14,$16,$18

		db	$19,$14,$13,$19
		db	$13,$11,$19,$14
		db	$13,$19,$13,$11
		db	$13,$11,$10,$0d
		db	$0c,$11,$13,$14
		db	$0c,$13,$14,$18
		db	$13,$14,$18,$1d
		db	$81,$1b,$18,$16,$14,$18,$16,$14,$11
		db	$16,$14,$11,$0f,$14,$11,$0f,$0c
		db	$ff
s2d
		db	$e1
		db	$ff
s01
		db	$d3,$a0,$a0,$63,$14
		db	$13
		db	$14
		db	$60,$11
		db	$ff
s25
		db	$d4,$84,$00,$00,$01,$01,$02,$01,$01,$00
		db	$ff
s20
		db	$c2,$8c,$6a,$07
		db	$84,$07
		db	$88,$79
		db	$6a
		db	$90,$07
		db	$84,$79,$09,$67
		db	$88,$6a,$07
		db	$8c,$06
		db	$84,$06
		db	$88,$79
		db	$6a
		db	$90,$06
		db	$88,$79,$04
		db	$6a,$06
		db	$8c,$07
		db	$84,$07
		db	$88,$79
		db	$6a
		db	$90,$07
		db	$88,$79
		db	$6a,$0c
		db	$8c,$0b
		db	$84,$0b
		db	$88,$79
		db	$6a
		db	$90,$0b
		db	$88,$79,$04
		db	$6a,$06
		db	$ff
s21
		db	$d5,$84,$01
		db	$00
		db	$00
		db	$01
		db	$04
		db	$00
		db	$01
		db	$00
		db	$01
		db	$00
		db	$00
		db	$00
		db	$04
		db	$00
		db	$01
		db	$00
		db	$ff
s02
		db	$d6,$84,$19,$18,$16,$14,$16,$13,$88,$fd,$08,$13,$14
		db	$84,$13,$88,$11,$84,$14,$16,$18,$19
		db	$88,$fd,$08,$16,$18
		db	$84,$16,$8c,$14
		db	$84,$16,$8c,$fd,$08,$11,$13
		db	$84,$11,$88,$10
		db	$84,$0c,$11,$13
		db	$84,$fd,$0c,$13,$14,$13,$11,$88,$fd,$08,$1b,$1d
		db	$84,$1b,$19,$14,$86,$fd,$08,$1b,$1d,$1f,$84,$20
		db	$86,$fd,$08,$20,$22,$24,$84,$25
		db	$88,$fd,$08,$22,$24,$84,$25
		db	$88,$fd,$08,$22,$24,$84,$22,$20,$1f
		db	$84,$fd,$06,$1f,$20,$1f,$1d
		db	$84,$18,$16,$14,$13,$14
		db	$d8,$ac,$11
		db	$d6,$84,$13,$14,$16,$13,$0f
		db	$84,$11,$10,$0d,$8c,$0c
		db	$84,$0a,$88,$fd,$0f,$0a,$0c
		db	$84,$11,$13,$14,$16,$14,$13,$14
		db	$d8,$a4,$11
		db	$d6,$84,$13,$14,$16,$18,$1b,$18
		db	$8c,$fd,$0c,$1d,$1f
		db	$84,$1b,$8c,$1b
		db	$84,$18
		db	$88,$fd,$08,$1b,$1d
		db	$ff
s30
		db	$84,$18,$16,$18,$11,$13,$14,$11
		db	$ff
s31
		db	$84,$18,$16,$18,$1b,$1d,$1f,$1b
		db	$ff
s03
		db	$d3,$a0,$a0,$63,$14
		db	$13
		db	$14
		db	$bf,$60,$11
		db	$ff
s04
		db	$d3,$a0,$a0,$61,$16
		db	$18
		db	$14
		db	$a0,$69,$13
		db	$61
		db	$ff
s05
		db	$d6,$88,$fd,$48,$18,$16
		db	$84,$16,$8c,$16,$84,$14,$88,$16,$84,$18
		db	$d9,$16,$18,$16
		db	$d6,$82,$0f,$11,$84,$16,$18
		db	$88,$fd,$48,$1f,$1d
		db	$84,$1c,$88,$18
		db	$82,$11,$13,$84,$16,$18,$13,$11,$10,$84,$0c,$d9,$10
		db	$d6,$11,$13,$16
		db	$88,$fd,$08,$16,$18
		db	$fd,$08,$16,$18
		db	$fd,$08,$16,$18
		db	$84,$fd,$08,$16,$18
		db	$1a,$1b,$1a,$18
		db	$88,$fd,$08,$1f,$20,$82,$1a,$1b,$84,$1d,$1b
		db	$d7,$a0,$a0,$7a,$1f,$e5
		db	$ff
s06
		db	$c2,$8c,$6a,$04
		db	$84,$6a
		db	$c2,$88,$79
		db	$6a
		db	$8c,$04
		db	$84,$04
		db	$88,$79
		db	$c2
		db	$c2,$8c,$6a,$06
		db	$84,$06
		db	$88,$79
		db	$6a
		db	$8c,$06
		db	$84,$06
		db	$88,$79
		db	$c2
		db	$c2,$8c,$6a,$02
		db	$84,$02
		db	$88,$79
		db	$6a
		db	$8c,$02
		db	$84,$02
		db	$88,$79
		db	$c2
		db	$c2,$8c,$6a,$01
		db	$84,$01
		db	$88,$79
		db	$6a
		db	$8c,$01
		db	$84,$01
		db	$c2,$88,$79,$79
		db	$ff
s07
		db	$d5,$84,$01
		db	$00
		db	$00
		db	$01
		db	$04
		db	$00
		db	$01
		db	$00
		db	$01
		db	$00
		db	$00
		db	$01
		db	$04
		db	$00
		db	$04
		db	$00
		db	$ff
s08
		db	$d3,$a0,$61,$14
		db	$64,$13
		db	$b0,$63,$11
		db	$cd,$84,$0f,$8c,$11
		db	$d3,$a0,$61,$14
		db	$64,$13
		db	$b0,$61,$16
		db	$cd,$84,$14,$8c,$16
		db	$d3,$a0,$14
		db	$64,$13
		db	$b0,$63,$11
		db	$cd,$84,$0f,$8c,$11
		db	$d3,$a0,$61,$14
		db	$16
		db	$cd,$88,$69,$18,$18,$18,$18,$61,$61,$61,$61
		db	$ff
s09
		db	$d6,$88,$20,$20,$20,$22,$fd,$08,$1d,$1f,$84,$20,$88,$1f,$84,$1d
		db	$84,$1b,$16,$88,$fd,$08,$1d,$1f,$1d,$1d,$84,$1b,$88,$1d
		db	$84,$1f,$d9,$1d,$1f

		db	$d6,$84,$16,$1a,$1d,$1f

		db	$88,$20,$20,$20,$22,$fd,$08,$1d,$1f,$84,$20,$88,$1f,$84,$1d
		db	$84,$1b,$16,$88,$fd,$04,$26,$27,$84,$29,$88,$fd,$08,$24,$26,$84,$27,$26,$88,$22
		db	$84,$16,$1a,$16,$1b,$1d,$1f,$1b

		db	$d6,$88,$20,$20,$20,$22,$88,$fd,$08,$1d,$1f,$84,$20,$88,$1f,$84,$1d
		db	$84,$1b,$16,$88,$fd,$08,$1d,$1f,$1d,$1d,$84,$1b,$88,$1d
		db	$84,$1f,$d9,$1d,$1f,$1d
		db	$d6,$1b,$1d,$1f
		db	$88,$20,$24,$27,$2b,$fd,$06,$27,$29,$84,$27,$88,$26
		db	$84,$24,$22,$1f,$a0,$a0,$fd,$08,$22,$24
		db	$ff
s2e
		db	$c2
		db	$8c,$6a,$02
		db	$84,$6a
		db	$88,$79
		db	$88,$6a
		db	$84,$09
		db	$6a,$67
		db	$6a
		db	$79,$67,$08
		db	$6a,$09
		db	$67,$0b
		db	$8c,$6a,$04
		db	$84,$6a
		db	$88,$79
		db	$84,$6a,$06
		db	$67,$08
		db	$6a,$04
		db	$88,$04
		db	$84,$04
		db	$84,$79,$06
		db	$8c,$6a,$01
		db	$02
		db	$84,$6a
		db	$88,$79
		db	$6a
		db	$84,$09
		db	$6a,$67,$6a
		db	$84,$79,$67,$08
		db	$6a,$09
		db	$67,$0b
		db	$8c,$6a,$04
		db	$84,$6a
		db	$88,$79
		db	$84,$6a,$06
		db	$67,$08
		db	$6a,$04
		db	$88,$04
		db	$84,$04
		db	$84,$79,$06
		db	$8c,$6a,$01
		db	$02
		db	$84,$6a
		db	$88,$79
		db	$6a
		db	$84,$09,$09,$67,$6a
		db	$84,$79,$67,$08
		db	$6a,$09
		db	$67,$0b
		db	$8c,$6a,$04
		db	$84,$6a
		db	$88,$79
		db	$84,$6a,$06
		db	$67,$08
		db	$6a,$04
		db	$88,$04
		db	$84,$04
		db	$84,$79
		db	$88,$6a,$01
		db	$84,$67
		db	$8c,$6a,$02
		db	$84,$6a
		db	$88,$79
		db	$6a
		db	$84,$04
		db	$88,$04
		db	$84,$04
		db	$88,$79
		db	$84,$6a,$01,$67
		db	$8c,$6a,$06
		db	$84,$6a
		db	$88,$79
		db	$84,$6a,$04
		db	$67,$01
		db	$6a,$06
		db	$88,$06
		db	$84,$06
		db	$88,$79
		db	$84,$79,$79
		db	$ff
s2f
		db	$d5,$84
		db	1,0,0,1,4,0,1,0,1,1,0,1,4,0,1,0,1,0,0,1,4,0,1,0
		db	1,1,0,1,4,1,0,2
		db	1,0,0,1,4,0,1,0,1,1,0,1,4,0,1,0,1,0,0,1,4,0,1,0
		db	1,1,0,1,4,1,0,2
		db	1,0,0,1,4,0,1,0,1,1,0,1,4,0,1,0,1,0,0,1,4,0,1,0
		db	1,1,0,1,4,1,0,2
		db	1,0,0,1,4,0,1,0,1,1,0,1,4,0,1,0,1,0,0,1,4,0,1,0
		db	1,1,0,1,4,1,4,4
		db	$ff

explosi1	db	$32,$fe
explosi2	db	$33,$fe

s32
		db	$cf,$8e,$7b,$00,$e1
		db	$ff
s33
		db	$da,$9a,$7c,$00,$e1
		db	$ff

firewrxu	db	$34,$ff,$00

s34		db	$cf,$81,$67,5,5,5,5,4,5,4,4,4,4,3,4,3,3,3,3,3,4,3
		db	4,4,4,4,5,4,$ff

firewrxe	db	$35,$fe

s35		db	$db,$9c,$67,$0f,$e1
		db	$ff

clearsfx	db	$00,$fe					;EMPTY SFX
s00		db	$e1,$ff
sex
;---------------------------------------------------------------------------------------------------------------------------------
xspacedl		=	$0b
xdetunes		=	$08


AUDIOIN	jmp	MUSICIN
	asl	a
	sta	seqposit,x	

	lda	#xsex-xs00-1
	sta	stpindex,x

	jmp	INITREALOS

AUDIOPL	jmp	MUSICPL
	lda	voictest,x
	beq	xAUDIOEX
	cpx	#$00
	beq	xVOICE01
	cpx	#$01
	beq	xVOICE02
	cpx	#$02
	beq	xVOICE03
	cpx	#$03
	beq	xVOICE04
xAUDIOEX	rts

xVOICE04	txa
	asl	a
	asl	a
	tay
	lda	#$10		;Set Amplitude			<bit 04>
	ora	#$20		;Continuous lenght enabled	<bit 05>
	ora	amplitud,x	;Amplitude/Envelope Value	<bit 03-00>
	sta	$4000,y
	lda	lofrqwrd,x
	sta	$4002,y
	lda	#$f8
	sta	$4003,y
	jmp	xJMPVO04	

xVOICE03	txa
	asl	a
	asl	a
	tay
	lda	#$80
	jmp	xJMPVO03
xVOICE02
xVOICE01 
	
xsetvoi1	txa
	asl	a
	asl	a
	tay
	lda	pulsbyt1,x	;setpulse			<bit 07-06>
	ora	#$10	;Set amplitude			<bit 04>
	ora	#$20		;continuous lenght enabled	<bit 05>
xJMPVO03	ora	amplitud,x	;Amplitude/Envelope Value	<bit 03-00>
	sta	$4000,y

	lda	#$08		;Hardware-glide down		<bit 03>
	sta	$4001,y		;

	lda	lofrqwrd,x
	sta	$4002,y

	lda	hifrqwrd,x	;Check if hi-frq thesame as note before
	cmp	continos,x
	beq	xNOHINO1
	sta	$4003,y		;HF set
	sta	continos,x
xNOHINO1

xJMPVO04

xSPDHDL1	lda	#$00		;Speedroutine (two speed bytes)
	sta	realplay,x
	dec	spdcount,x
	bpl	xSPDHDX1
	lda	spdcheck,x
	eor	#$ff
	sta	spdcheck,x
	bpl	xSECONS1
	ldy	seqposit,x	
	lda	xseqtabes+$00,y
	jmp	xSPDSET1
xSECONS1	ldy	seqposit,x
	lda	xseqtabes+$01,y
xSPDSET1	sta	spdcount,x
	lda	#$ff
	sta	realplay,x
xSPDHDX1	
	lda	realplay,x	;check if new leng subtraction necessary
	bpl	xNOSTPH1		;

xAUDIPL1	dec	noteleng,x	;decrease speed, if zero then stephandle
	beq	xSTPHDL1	

xNOSTPH1	lda	realnote,x	;check realnote on pause, no FX
	cmp	#$ff		;
	beq	xNOFXPL1		;
	jsr	xFXPLAY1		;

xNOFXPL1	lda	#$00		;clear noteinit
	sta	noteinit,x	;
	rts			;

xSTPHDL1	ldy	stepnumb,x	;What step to take
	lda	xstptablo,y	;
	sta	ssslo		;
	lda	xstptabhi,y	;
	sta	ssshi		;

	lda	#$00		;reset glide (if not reached dest. note)
	sta	gldspeed,x

			ldy	stpindex,x	;read step-index
	lda	(ssslo),y	;
	cmp	#$ff		;step check on end 
	beq	xSEQHDL1		;yes, goto sequence-handle
	jmp	xSTPHD12		;no, goto stephandle 02

xSEQHDL1	dec	repstcnt,x	;check if step repeats
	beq	xREALSQ1		;no, goto real sequence-handle
	lda	#$00		;yes, reset index & do last step again
	sta	stpindex,x	;
	tay			;
	lda	(ssslo),y	;
	jmp	xSTPHD12		;

xREALSQ1	ldy	seqposit,x	;what sequence to take
	lda	xseqtabel+$00,y	;
	sta	ssslo		;
	lda	xseqtabel+$01,y	;
	sta	ssshi		;

	jsr	SEQZOOI

xSEQHD19	sta	stepnumb,x	;set step & clear stepindex
	lda	#$00
	sta	stpindex,x
	inc	repstcnt,x	;make sure step plays once
	iny
	tya
	sta	seqindex,x
	jmp	xSTPHDL1		;back to step-handle

xSTPHD12 cmp	#$fd		;glide check in step
	bne	xSTPHD14
	iny
	lda	(ssslo),y
	sta	gldspeed,x
	iny
	lda	(ssslo),y
	clc
	adc	tranbyte,x
	sta	realnote,x
	iny
	lda	(ssslo),y
	clc
	adc	tranbyte,x
	sta	destnote,x
	iny
	jmp	xNONEWN1

xSTPHD14	cmp	#$e0		;pause check
	bcc	xSTPHD15
	and	#$1f
	sta	realleng,x
	lda	#$ff
	sta	realnote,x
	lda	#$00
	sta	amplitud,x
	iny
	tya
	sta	stpindex,x
	jmp	xPAUSEX1

xSTPHD15	cmp	#$c0		;sound check
	bcc	xSTPHD16
	clc
	adc	sfxtrans,x
	asl	a
	asl	a
	sta	realsoun,x
	iny
	lda	(ssslo),y
	cmp	#$fd	
	beq	xSTPHD12
	cmp	#$c0
	bcs	xNONEWN1
	
xSTPHD16	cmp	#$80		;length check
	bcc	xSTPHD17
	and	#$7f
	clc
	adc	noteleng,x
	sta	noteleng,x
	sta	realleng,x
	iny
	lda	(ssslo),y
	cmp	#$fd
	beq	xSTPHD12
	cmp	#$80
	bcs	xSTPHD16

xSTPHD17	cmp	#$60		;arp check
	bcc	xSTPHD18
	and	#$1f	
	clc
	adc	arptrans,x
	sta	arpnumbo,x
	iny
	lda	(ssslo),y
	cmp	#$60
	bcs	xNONEWN1

xSTPHD18	clc			;set note + note transpose
	adc	tranbyte,x	;
	sta	realnote,x	;
	iny			;
xNONEWN1	tya			;
	sta	stpindex,x	;restore step-index

	inc	noteinit,x	;first init has been done
	ldy	realsoun,x	;
	cpx	#$00		;check sound-init VOICE 01
	beq	xINITV12		;
	cpx	#$01		;check sound-init VOICE 02
	beq	xINITV12		;
	cpx	#$02		;check sound-init VOICE 03
	beq	xINITV03		;
	cpx	#$03		;check sound-init VOICE 04
	beq	xINITV04		;
	rts

xINITV12 lda	xsoundfix+$00,y	;check if new pulse to be set
	and	#$08		;
	bne	xNONEWPL		;
	lda	xsoundfix+$00,y	;yes, set new pulse
	and	#$c0		;
	sta	pulsbyt1,x	;
xINITV03
xINITV04
xNONEWPL	lda	soundfix+$01,y	;set vibrato-byte into fx01 & set effects (arp, etc)
	sta	fx01,x		;
	lda	xsoundfix+$02,y	;set vibdelay pultab-number into fx02
	sta	fx02,x		;
	
xNONEWG1	lda	soundfix+$03,y	;set effect-values into fx03	
	sta	fx03,x		;
	cpx	#$03
	beq	xNFORV04

	ldy	realnote,x	;Read realnote as index
	lda	frqtablo,y ;Low-frequency read	<bit 07-00>
	sta	lofrqwrd,x	;

	lda	#$f8	;hardware length endless	<bit 07-03>
	ora	frqtabhi,y ;hi-frequency read		<bit 02-00>
	sta	hifrqwrd,x	;HF set
	jmp	xPAUSEX1

xNFORV04	lda	realnote,x
	sta	lofrqwrd,x

xPAUSEX1	lda	realleng,x
	sta	noteleng,x
	rts

;INITV03	rts

xFXPLAY1 lda	noteinit,x
	bne	xINTPLA1
	jmp	xPLAYFXR
		
xINTPLA1 jsr	INTPLAYOS

xFXPLA12 jsr	CLEARZOOI     

	lda	realleng,x
	sec
	sbc	vibdelay,x
	bcs	xREFIX01
	lda	#$00
	sta	fx01,x
	beq	xEXITIN1
xREFIX01	sta	vibdelay,x
xEXITIN1	jmp	xNOVIB01	

xPLAYFXR	lda	fx03,x		;amplitude routine		
	and	#$7f		;maximum of $7f amplitude tabels
	tay
	lda	xamptabl,y
	sta	ssslo
	lda	xamptabh,y
	sta	ssshi
       
	jsr	SHITAMP

xGLIDE1	ldy	gldspeed,x		;glide routine
	beq	xNOGLD12

	lda	glddelay,x
	cmp	noteleng,x
	bcc	xNOGLD12

	ldy	glddirec,x
	bmi	xDOWNGL1
	lda	lofrqwrd,x
	clc
	adc	gldspeed,x
	sta	lofrqwrd,x
	ldy	destnote,x
	sec
	sbc	frqtablo,y
	bcs	xFROTER1
	jmp	xNOGLD01

xDOWNGL1	lda	lofrqwrd,x
	sec
	sbc	gldspeed,x
	sta	lofrqwrd,x
	ldy	destnote,x
	sec
	sbc	frqtablo,y
	bcs	xNOGLD01
xFROTER1	lda	frqtablo,y
	sta	lofrqwrd,x
	
	lda	#$00
	sta	gldspeed,x
xNOGLD01	jmp	xNOVIB01
xNOGLD12
	
xVIBRA01	lda	fx01,x		;vibrato routine
	and	#$0f
	beq	xNOVIB01
	lda	vibdelay,x
	cmp	noteleng,x
	bcc	xNOVIB01

	lda	fx01,x
	and	#$0f
	tay
	lda	xvibtabl-1,y
	sta	ssslo
	lda	xvibtabh-1,y
	sta	ssshi

	jsr	JSRVIB

	lda	lofrqwrd,x
	clc
	adc	(ssslo),y
	bcc	xVIBNO01
	clc
xVIBNO01	sta	lofrqwrd,x
	iny
	tya
	sta	vibindex,x
xNOVIB01

xPULSE1	cpx	#$02
	bcs	xNOPULS1
	lda	fx02,x		;PULSE ROUTINE
	and	#$0f
	beq	xNOPULS1
	tay
	lda	xpultabl-1,y
	sta	ssslo
	lda	xpultabh-1,y
	sta	ssshi

	jsr	JSRPUL

xNOPULS1	

xSPACE01	lda	fx01,x		;SPACE ROUTINE
	and	#$20
	beq	xNOSPC01
	lda	noteleng,x
	cmp	#xspacedl
	bcs	xNOSPC01
	lda	#$00
	sta	spacespd,x
	ldy	realnote,x
	lda	spacecnt,x
	and	#$01
	bne	xNORNOT1
	lda	spacecnt,x
	lsr	a
	sta	spacespd,x
xNORNOT1	lda	frqtablo,y
	clc
	adc	spacespd,x
	sta	lofrqwrd,x
	inc	spacecnt,x
xNOSPC01	

xFINEDTN	lda	fx01,x		;FINE DETUNE
	and	#$10
	beq	xNOFIDTN
	ldy	realnote,x
	lda	frqtablo,y
	clc
	adc	#xdetunes
	sta	lofrqwrd,x	
xNOFIDTN		


xARPEG01	lda	fx01,x		;ARP ROUTINE
	and	#$40		
	beq	xNOARP01
	ldy	arpnumbo,x
	lda	xarptablo,y
	sta	ssslo
	lda	xarptabhi,y
	sta	ssshi
ARPJSR	ldy	arpindex,x
	bne	xARP01
	lda	(ssslo),y
	sta	arpspeed,x
	iny
	jmp	xARP02
xARP01	lda	arpspeed,x
	sec
	sbc	#$10
	sta	arpspeed,x
	bpl	xARPNO01
	ldy	#$00
	lda	(ssslo),y
	sta	arpspeed,x
	ldy	arpindex,x
xARP02	lda	(ssslo),y
	cmp	#$ff
	bne	xARP03
	ldy	#$00
	lda	(ssslo),y
	and	#$0f
	tay
	iny
	lda	(ssslo),y
xARP03	cmp	#$fe
	beq	xNOARP01
	bmi	xNORMA01
	clc
	adc	realnote,x
	sta	arponoto,x
xNORMA01	and	#$7f
	sta	arponoto,x
	iny
	tya
	sta	arpindex,x
xARPNO01	lda	arponoto,x
	cpx	#$03
	bne	xSETAN01
	sta	lofrqwrd,x
	jmp	xNOARP01
xSETAN01
	tay
	lda	frqtablo,y
	sta	lofrqwrd,x
	lda	frqtabhi,y
	sta	hifrqwrd,x
xNOARP01
	rts




;---------------------------------------------------------------------------------------------------------------------------------
xseqtabel	db	<xclearsfx,>xclearsfx	; $00 $xx 
		db	<xrainshi4,>xrainshi4	; $01 $03
		db	<xsmokepuf,>xsmokepuf	; $02 $03
		db	<xsmokepuh,>xsmokepuh	; $03 $03
		db	<xwarning1,>xwarning1	; $04 $00
		db	<xwarning2,>xwarning2	; $05,$01
		db	<xsparksh1,>xsparksh1	; $06,$03
		db	<xsparksh1,>xsparksh1	; $07,$03
		db	<xsparksh1,>xsparksh1	; $08,$03
		db	<xsparksh1,>xsparksh1	; $09,$03
		db	<xsparksh1,>xsparksh1	; $0a,$03
		db	<xcoindrop,>xcoindrop	; $0b,$00
		db	<xlasers01,>xlasers01	; $0c,$02
		db	<xlasers02,>xlasers02	; $0d,$03
		db	<xlasers11,>xlasers11	; $0e,$02
		db	<xlasers12,>xlasers12	; $0f,$03
		db	<xlasers21,>xlasers21	; $10,$00 or $01
		db	<xvolcano1,>xvolcano1	; $11,$00
		db	<xvolcano2,>xvolcano2	; $12,$03
		db	<xsplashin,>xsplashin	; $13,$03
		db	<xbirdfl00,>xbirdfl00	; $14,$00
		db	<xbirdfl01,>xbirdfl01	; $15,$01
		db	<xbirdfl10,>xbirdfl10	; $16,$00
		db	<xbirdfl11,>xbirdfl11	; $17,$01
		db	<xbirdfl20,>xbirdfl20	; $18,$00
		db	<xbirdfl21,>xbirdfl21	; $19,$01
		db	<xkeyclick,>xkeyclick	; $1a,$00
		db	<xmeteorc1,>xmeteorc1	; $1b,$00
		db	<xmeteorc2,>xmeteorc2	; $1c,$03
		db	<xiconse01,>xiconse01	; $1d,$01
		db	<xiconse11,>xiconse11	; $1e,$01
		db	<xiconse21,>xiconse21	; $1f,$01
		db	<xiconse31,>xiconse31	; $20,$01
		db	<xiconse41,>xiconse41	; $21,$01
		db	<xiconse51,>xiconse51	; $22,$01
		db	<xfeedbac1,>xfeedbac1	; $23,$00
		db	<xfeedbac2,>xfeedbac2	; $24,$01
		db	<xexplos01,>xexplos01	; $25,$00
		db	<xexplos02,>xexplos02	; $26,$03
		db	<xexplos11,>xexplos11	; $27,$00
		db	<xexplos12,>xexplos12	; $28,$03
		db	<xstarspin,>xstarspin	; $29,$03
		db	<xenginer1,>xenginer1	; $2a,$00
		db	<xenginer2,>xenginer2	; $2b,$03
		db	<xcannon01,>xcannon01	; $2c,$02
		db	<xcannon02,>xcannon02	; $2d,$03
		db	<xcoindro1,>xcoindro1	; $2e,$01
		db	<xwarthin1,>xwarthin1	; $2f,$00
		db	<xwarthin2,>xwarthin2	; $30,$01
		db	<xlooser01,>xlooser01	; $31,$00
		db	<xlooser02,>xlooser02	; $32,$01
		db	<xlooser03,>xlooser03	; $33,$02
		db	<xlooser04,>xlooser04	; $34,$03
		db	<xwinner01,>xwinner01	; $35,$00
		db	<xwinner02,>xwinner02	; $36,$01
		db	<xwinner03,>xwinner03	; $37,$02
		db	<xwinner04,>xwinner04	; $38,$03
		db	<xgotplan1,>xgotplan1	; $39,$00
		db	<xgotplan2,>xgotplan2	; $3a,$01
		db	<xlosplan1,>xlosplan1	; $3b,$00
		db	<xlosplan2,>xlosplan2	; $3c,$01
		db	<xexplolit,>xexplolit	; $3d,$02
		db	<xpirats01,>xpirats01	; $3e,$00
		db	<xpirats02,>xpirats02	; $3f,$01
		db	<xpirats03,>xpirats03	; $40,$02
		db	<xpirats04,>xpirats04	; $41,$03
		db	<xprepare1,>xprepare1	; $42,$00
		db	<xprepare2,>xprepare2	; $43,$01
		db	<xscraper1,>xscraper1	; $44,$00
		db	<xscraper2,>xscraper2	; $45,$01
		db	<xselecdo1,>xselecdo1	; $46,$00
		db	<xselecdo2,>xselecdo2	; $47,$01
		db	<xselecup1,>xselecup1	; $48,$00
		db	<xselecup2,>xselecup2	; $49,$01
		db	<xselbeat1,>xselbeat1	; $4a,$02
		db	<xselbeat2,>xselbeat2	; $4b,$03
		db	<xnintend1,>xnintend1	; $4c,$00
		db	<xnintend2,>xnintend2	; $4d,$01
		db	<xnintend3,>xnintend3	; $4e,$02
		db	<xnintend4,>xnintend4	; $4f,$03
		db	<xhartbeat,>xhartbeat	; $50,$00
		db	<xiconrock,>xiconrock	; $51,$03
		db	<xiconatmo,>xiconatmo	; $52,$00
		db	<xiconcra1,>xiconcra1	; $53,$00
		db	<xiconcra2,>xiconcra2	; $54,$01
		db	<xiconpage,>xiconpage	; $55,$00
		db	<xiconfis1,>xiconfis1	; $56,$00
		db	<xiconfis2,>xiconfis2	; $57,$03
		db	<xiconhead,>xiconhead	; $58,$03


xseqtabes	db	$00,$00 ; $00 $xx 
		db	$7f,$7f	; $01 $03
		db	$02,$02	; $02 $03
		db	$02,$02	; $03 $03
		db	$02,$02	; $04 $00
		db	$02,$02	; $05,$01
		db	$00,$00	; $06,$03
		db	$01,$01	; $07,$03
		db	$04,$04	; $08,$03
		db	$0b,$0b	; $09,$03
		db	$21,$21	; $0a,$03
		db	$01,$01	; $0b,$00
		db	$02,$02	; $0c,$02
		db	$02,$02	; $0d,$03
		db	$02,$02	; $0e,$02
		db	$02,$02	; $0f,$03
		db	$02,$02	; $10,$00 or $01
		db	$02,$02	; $11,$00
		db	$02,$02	; $12,$03
		db	$02,$02	; $13,$03
		db	$02,$02	; $14,$00
		db	$02,$02	; $15,$01
		db	$02,$02	; $16,$00
		db	$02,$02	; $17,$01
		db	$02,$02	; $18,$00
		db	$02,$02	; $19,$01
		db	$02,$02	; $1a,$00
		db	$02,$02	; $1b,$00
		db	$02,$02	; $1c,$03
		db	$02,$02	; $1d,$01
		db	$02,$02	; $1e,$01
		db	$02,$02	; $1f,$01
		db	$02,$02	; $20,$01
		db	$02,$02	; $21,$01
		db	$02,$02	; $22,$01
		db	$02,$02	; $23,$00
		db	$02,$02	; $24,$01
		db	$02,$02	; $25,$00
		db	$02,$02	; $26,$03
		db	$02,$02	; $27,$00
		db	$02,$02	; $28,$03
		db	$02,$02	; $29,$03
		db	$02,$02	; $2a,$00
		db	$02,$02	; $2b,$03
		db	$02,$02	; $2c,$02
		db	$02,$02	; $2d,$03
		db	$01,$01	; $2e,$01
		db	$02,$02	; $2f,$00
		db	$02,$02	; $30,$01
		db	$02,$02	; $31,$00
		db	$02,$02	; $32,$01
		db	$02,$02	; $33,$02
		db	$02,$02	; $34,$03
		db	$02,$02	; $35,$00
		db	$02,$02	; $36,$01
		db	$02,$02	; $37,$02
		db	$02,$02	; $38,$03
		db	$03,$03	; $39,$00
		db	$03,$03	; $3a,$01
		db	$04,$03	; $3b,$00
		db	$04,$03	; $3c,$01
		db	$00,$00	; $3d,$02
		db	$02,$03	; $3e,$00
		db	$02,$03	; $3f,$01
		db	$02,$03	; $40,$02
		db	$02,$03	; $41,$03
		db	$02,$02	; $42,$00
		db	$02,$02	; $43,$01
		db	$02,$02	; $44,$00
		db	$02,$02	; $45,$01
		db	$03,$03	; $46,$00
		db	$03,$03	; $47,$01
		db	$03,$03	; $48,$00
		db	$03,$03	; $49,$01
		db	$04,$04	; $4a,$02
		db	$04,$04	; $4b,$03
		db	$02,$02	; $4c,$00
		db	$02,$02	; $4d,$01
		db	$02,$02	; $4e,$02
		db	$02,$02	; $4f,$03
		db	$02,$02	; $50,$00
		db	$04,$04	; $51,$03
		db	$04,$04	; $52,$00
		db	$02,$02	; $53,$00
		db	$02,$02	; $54,$01
		db	$03,$03	; $55,$00
		db	$03,$03	; $56,$00
		db	$03,$03	; $57,$03
		db	$02,$02	; $58,$03
	
;---------------------------------------------------------------------------------------------------------------------------------
xam00		
	db	7,6,6,5,5,5,4,4,4,4,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,1,$fe
xam01
	db	1,$fe
xam02
	db	12,11,10,9,8,8,7,7,7,6,6,6,5,5,5,4,4,3,3,2,2,1,1,1,0,$fe
xam03	   
	db	15,15,14,14,13,13,12,12,11,11,10,10,10,9,9,9,9,8,8,8,8,7,7,7,7,6,6,6,5,5,5,4,4,3,3,2,2,1,1,1,1,0,$fe
xam04	       
	db	1,2,3,4,5,5,6,6,7,7,7,8,$fe
xam05		
	db	7,7,6,6,5,5,4,4,4,3,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0,$fe
xam06
	db	2,1,2,0,2,1,4,2,3,1,2,0,1,0,6,2,5,1,3,0,9,5,8,4,3,0,10,6,7,2,9,2,11,7,12,8
	db	6,2,9,5,12,8,10,6,2,4,0,9,5,3,0,7,3,8,4,0,2,0,6,2,4,0,8,4,7,3,3,0,5,1,6,2
	db	$ff,$00
xam07 
	db	8,8,8,8,8,8,7,7,7,6,6,6,5,5,5,4,4,4,3,3,3,3,2,2,1,1,0,$fe	       
xam08		
	db	2,4,8,8,8,8,8,7,7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,2,2,1,1,0,$fe
xam09
	db	4,3,3,2,2,2,1,1,1,1,1,1,1,0,$fe
xam0a
	db	3,0,$fe
xam0b
	db	8,8,8,8,8,7,7,7,7,7,6,6,6,6,6,5,5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2
	db	1,1,1,1,1,1,1,1,1,1,1,0,$fe
xam0c
	db	2,1,0,$fe
xam0d		
	db	1,2,3,4,4,5,5,6,6,6,7,7,7,7,6,6,6,5,5,4,4,3,2,1,0,$fe
xam0e
	db	2,$fe
xam0f	
	db	10,8,6,4,2,1,0,$fe
xam10
	db	8,7,7,6,6,5,5,5,4,4,4,3,$fe	
xam11	
	db	2,2,1,$fe
xam12	
	db	9,8,7,6,5,4,3,3,2,2
	db	7,6,5,4,3,3,2,2,1,1
	db	5,4,3,3,2,2,2,1,1,1
	db	4,3,2,2,2,1,1,1,0,0
	db	3,2,1,1,1,0,0,0,0,0
	db	2,2,1,1,0,0,0,0,0,0
	db	1,1,0,$fe
xam13	
	db	8,7,6,5,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,$fe
xam14	
	db	2,1,0,0,0,0,0,0,1,0,$fe
xam15	
	db	7,5,4,3,3,2,2,1,1,0,$fe
xam16	
xam17	
xam18	
xam19	
xam1a	
xam1b	
xam1c	
xam1d	
xam1e	
xam1f	
;---------------------------------------------------------------------------------------------------------------------------------
xv01		
	db	$05,$05,$05,$05,$05,$05,$05,$05,$fb,$fb,$fb,$fb,$fb,$fb,$fb,$fb
	db	$fb,$fb,$fb,$fb,$fb,$fb,$fb,$fb,$05,$05,$05,$05,$05,$05,$05,$05
	db	$80,$00
xv02
	db	$f5,$00
	db	$80,$01
xv03		
	db	$01,$ff
	db	$80,$00
xv04
	db	$02,$02,$02,$02,$fe,$fe,$fe,$fe,$fe,$fe,$fe,$fe,$02,$02,$02,$02
	db	$80,$00
xv05
	db	$01,$01,$00,$00,$ff,$ff,$00,$00,$ff,$ff,$00,$00,$01,$01,$00,$00
	db	$80,$00
xv06
	db	$01,$01,$00,$ff,$ff,$00,$ff,$ff,$00,$01,$01,$00
	db	$80,$00
xv07
	db	$06,$06,$06,$fa,$fa,$fa,$fa,$fa,$fa,$06,$06,$06
	db	$07,$07,$07,$f9,$f9,$f9,$f9,$f9,$f9,$07,$07,$07
	db	$08,$08,$08,$f8,$f8,$f8,$f8,$f8,$f8,$08,$08,$08
	db	$80,$18
xv08
	db	$01,$00,$00,$ff,$00,$00,$ff,$00,$00,$01,$00,$00
	db	$80,$00
xv09
xv0a
xv0b
xv0c
xv0d
xv0e
xv0f
;---------------------------------------------------------------------------------------------------------------------------------
xp01		
	db	$00,$00,$00,$00,$00,$00,$40,$40,$40,$40,$40,$40,$80,$80,$80,$80,$80,$80,$40,$40,$40,$40,$40,$40
	db	$ff,$00
xp02	   
	db	$c0,$80,$40,$00
	db	$ff,$00	
xp03	      
	db	$40,$40,$40,$40,$40,$40,$40,$40,$40,$40
	db	$80,$80,$80,$80,$80,$80,$80,$80,$80,$80
	db	$ff,$00	
xp04		
	db	$00,$00,$40,$80,$c0,$80,$40
	db	$ff,$00
xp05		
xp06		
xp07		
xp08		
xp09		
xp0a		
xp0b		
xp0c		
xp0d	       
xp0e		
xp0f		
;---------------------------------------------------------------------------------------------------------------------------------
xa00	db	$00,28,30,28,26,24,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,$fe
xa01	db	$00,0,$fe		
xa02	db	$00,27,30,33,32,30,28,26,24,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,1,0,$fe
xa03	db	$00,4,5,6,6,6,5,5,5,4,4,3,3,2,2,1,1,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,$fe
xa04	db	$00,4,3,2,1,0,$fe	
xa05	db	$00,9,11,9,7,6,5,4,3,2,1,0,$fe
xa06	db	$00,$fd,$fe			          ;use again
xa07	db	$00,4,2,1,0,7,5,4,3,5,3,2,1,4,2,1,0,8,6,5,4,7,5,4,3,$ff
xa08	db	$00,0,1,$ff
xa09	db	$00,$a4,$a1,$9e,$9b,$98,$95,$fe
xa0a	db	$00,0,1,2,3,4,5,6,7,8,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10
	db	10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,$ff
xa0b	db	$00,$a0,$9c,$98,$00,$fe
xa0c	db	$00,$00,$fe	
xa0d	db	$50,6,1,0,$ff	
xa0e	db	$50,7,6,0,$ff	
xa0f	db	$50,11,5,0,$ff
xa10	db	$50,1,0,$ff	
xa11	db	$10,0,5,7,$ff
xa12	db	$10,0,2,7,$ff		
xa13	db	$10,0,5,10,$ff	
xa14	db	$10,0,3,8,12,$ff
xa15	db	$10,0,3,8,$ff	
xa16	db	$00,$19,$16,$14,$12,$16,$13,$11,$10,$13,$10,$0e,$0d
	db	$10,$0d,$0b,$0a,$0d,$0a,$08,$07,$0a,$07,$05,$04
	db	$07,$04,$02,$01,$04,$02,$01,$00,$fe
xa17	db	$20,0,7,$ff
xa18	db	$00,0,1,7,12,$ff
xa19	db	$20,0,5,$ff
xa1a	db	$10,19,15,12,7,1,0,$ff
xa1b	db	$00,$9a,$94,$90,$00,$fe
xa1c	db	$00,8,8,8,8,7,8,7,8,7,7,7,7,6,7,6,7,6,6,6,6,5,6,5,6,5,5,5,5,4,5,4,5,4,4,4,4,3,4,3,4,3,3,3,3,2,3,2,3,2,2,2,2,1,2,1,2,1,1,1,1,0,$fe
xa1d	db	$10,12,7,4,0,$ff 
xa1e	db	$10,12,7,3,0,$ff	  
xa1f	db	1,0,$fe
;--------------------------------------------------------------------------------
xsoundfix	

;rainship  (4)
		db	$00,$00,$00,$01	;C0   
;smokepuf  (4)
		db	$00,$00,$00,$02	;C1   
;SMOKEPUH  (4)
		db	$00,$00,$00,$03	;C2   
;warninON  (1)
		db	$80,$01,$00,$04	;C3   
;warninOF  (1)
		db	$80,$01,$00,$05	;C4   
;sparking  (4)
		db	$00,$02,$00,$06	;C5   
;CoinDrop  (1)
		db	$00,$03,$00,$09	;C6   
;LASER-00  (13)
		db	$80,$40,$00,$07	;C7   
;LASER-10  (13)
		db	$80,$40,$00,$08	;C8   
;LASER-20  (1)
		db	$00,$00,$00,$05	;C9   
;VOLCANOS  (4)
		db	$80,$40,$00,$01	;CA   
;BIRDFLUT  (1)
		db	$80,$00,$00,$01	;CB   
;KEYCLICK  (1)
		db	$00,$00,$00,$0a	;CC   
;METEORCR  (4)
		db	$80,$40,$00,$0b	;CD   
;ICONSELEC (1)
		db	$80,$00,$02,$0c	;CE   
;FEEDBACK  (1)
		db	$00,$04,$01,$0d	;CF   
;EXPLOSIO  (14)
		db	$80,$40,$00,$03	;D0   
;ENGINRUM  (14)
		db	$80,$40,$00,$0e	;D1   
;CANNONFI  (34)
		db	$80,$40,$00,$0f	;D2   
;WARTHING  (1)
		db	$80,$40,$01,$04	;D3   
;WARTHOFF  (1)
		db	$80,$40,$01,$07	;D4   
;JINGCONT  (1)
		db	$00,$05,$41,$10	;D5   
;JINGARP!  (1)
		db	$00,$40,$01,$00	;D6   
;JINGCARP  (1)
		db	$00,$40,$01,$10	;D7   
;JINGBASS  (3)
		db	$00,$44,$41,$10	;D8   
;FIDJINGI  (1)
		db	$40,$06,$21,$11	;D9   
;FIDJINGI  (1)
		db	$40,$06,$21,$01	;DA   
;DRUMBEAT  (4)
		db	$00,$00,$00,$12	;DB   
;DRUMMELO  (3)
		db	$00,$47,$20,$01	;DC   
;NINTARPS  (1)			      
		db	$40,$40,$01,$13	;DD   
;NINTNORM  (1)			      
		db	$c0,$05,$61,$13	;DE   
;HARTBEAT  (1)			      
		db	$00,$00,$00,$14	;DF   
;ICONATMO  (1)
		db	$40,$00,$03,$01	;E0   
;ICONDETN  (1)
		db	$80,$10,$00,$01	;E1   
;ICONDRUM  (4)					      
		db	$00,$00,$00,$15	;E2   
;---------------------------------------------------------------------------------------------------------------------------------

xclearsfx	db	$00,$fe					;EMPTY SFX to clear voice(s)

xs00		db	$e1,$ff
xsex
			      
xrainshi4	db	$01,$ff,$00				;RAIN SFX for atmosphere processor

xs01		db	$c0,$bf,$05,$ff

xsmokepuf	db	$02,$fe					;SMOKEPUF FOR MECHANIC THING	

xs02		db	$c1,$90,$02,$ff

xsmokepuh	db	$03,$fe					;SMOKEPUF FOR STEAM THING

xs03		db	$c2,$90,$04,$ff

xwarning1	db	$04,$fe					;WARNING SIRENE	for danger or so
xwarning2	db	$05,$fe

xs04		db	$c3,$a0,$28,$c4,$8c,$28,$ff

xs05		db	$e2,$c3,$a0,$28,$c4,$8c,$28,$ff	

xsparksh1	db	$06,$fe					;SPARK WITH FLEXIBLE LENGHT

xs06		db	$c5,$84,$03,$e1,$ff

xcoindrop	db	$07,$fe					;COIN DROP (poly voice 1 & 2)

xs07		db	$f8	
		db	$c6,$83,$53
		db	$c6,$81,$53
		db	$c6,$83,$53;4
		db	$c6,$82,$53
		db	$c6,$85,$53
		db	$c6,$83,$53
		db	$c6,$83,$53
		db	$c6,$88,$53
		db	$c6,$88,$53
		db	$e1
		db	$ff

xs24		db	$f9	
		db	$c6,$83,$53
		db	$c6,$83,$53
		db	$c6,$81,$53;2
		db	$c6,$83,$53
		db	$c6,$84,$53
		db	$c6,$86,$53
		db	$c6,$8c,$53
		db	$e1
		db	$ff

xcoindro1	db	$24,$fe					;COIN DROP (poly voice 1 & 2)

xlasers01	db	$08,$fe					;LASER 00

xs08		db	$c7,$8b,$60,$00,$e1,$ff

xlasers02	db	$09,$fe						

xs09		db	$c7,$8b,$61,$03,$ff

xlasers11	db	$0a,$fe					;LASER 10 (ion)

xs0a		db	$c8,$89,$62,$00,$e1,$ff

xlasers12	db	$0b,$fe						

xs0b		db	$c8,$8c,$63,$00,$ff

xlasers21	db	$0c,$fe					;LASER 20 (poly voice 1 & 2)

xs0c		db	$c9,$8c,$fd,$04,$33,$24,$ff

xvolcano1	db	$0d,$ff,$00				;VOLCANO RUMBLE

xs0d		db	$ca,$82,$64,0,3,2,5,3,1,2,4,3,5,2,3,4,2,5,4,2,3,1,5,3,4,2,0,3,2,4,3,5,3,2,4,1,0,4,2,5,3,$ff

xvolcano2	db	$0e,$ff,$00						

xs0e		db	$ca,$81,$61,$0f,$ff

xsplashin	db	$0f,$fe					;DOLPHIN DIVES INTO WATER	

xs0f		db	$ca,$81,$61,6,6,7,7,7,6,6,6,5,5,5,4,4,3,3,2,2,1,1,0,0,$e1,$ff

xbirdfl00	db	$10,$fe					;BIRDS 01	
xbirdfl01	db	$14,$10,$fe					

xs10		db	$cb
		db	$83,$fd,$01,$48,$4f
		db	$83,$fd,$01,$48,$4f
		db	$83,$fd,$01,$48,$4f
		db	$83,$fd,$01,$48,$4f
		db	$83,$fd,$01,$48,$4f
		db	$e1,$ff
	
xs11		db	$e3
		db	$ff

xbirdfl10	db	$12,$fe					;BIRDS 02	
xbirdfl11	db	$11,$12,$fe					

xs12		db	$cb
		db	$84,$fd,$01,$47,$42
		db	$84,$fd,$01,$47,$42
		db	$84,$fd,$01,$47,$42
		db	$84,$fd,$01,$47,$42
		db	$e1,$ff

xbirdfl20	db	$13,$fe					;BIRDS 03	
xbirdfl21	db	$14,$13,$fe					

xs13		db	$cb
		db	$83,$fd,$01,$3f,$42
		db	$83,$fd,$01,$3b,$3e
		db	$83,$fd,$01,$42,$4d
		db	$e1,$ff
xs14
		db	$e1,$ff

xkeyclick	db	$15,$fe					;KEYCLICK

xs15		db	$cc,$82,$48,$ff

xmeteorc1	db	$16,$fe					;METEOR HIT PLANET AFTER ZOOM
xmeteorc2	db	$1e,$fe

xs16		db	$ec
		db	$cd,$91,$67,$00,$e1,$ff

xs1e		db	$c0,$81,0,1,2,3,3,4,4,5,5,6,7,8
		db	$cd,$92,$61,$0f,$ff

xiconse01	db	$98,$17,$fe				;ICON SELECT OKAY! 01
xiconse11	db	$8c,$17,$fe				;ICON SELECT WRONG 01
xiconse21	db	$9b,$17,$fe				;ICON SELECT OKAY! 02
xiconse31	db	$8f,$17,$fe				;ICON SELECT WRONG 02
xiconse41	db	$9e,$17,$fe				;ICON SELECT OKAY! 03
xiconse51	db	$92,$17,$fe				;ICON SELECT WRONG 03

xs17		db	$ce,$82,$28,$e1,$ff

xfeedbac1	db	$18,$fe					;ELECTRIC FEEDBACK (CLASSIFIED)
xfeedbac2	db	$19,$fe

xs18		db	$cf,$88,$01,$89,$01,$ff
xs19		db	$e2,$cf,$88,$01,$89,$01,$ff

xexplos01	db	$1a,$fe					;EXPLOSION 01
xexplos02	db	$1b,$fe

xs1a		db	$d0,$90,$67,$01,$ff

xs1b		db	$d0,$90,$61,$0f,$ff

xexplos11	db	$1c,$fe					;EXPLOSION 02
xexplos12	db	$1d,$fe

xs1c
		db	$d0,$90,$67,$05,$ff
xs1d
		db	$d0,$90,$68,$0e,$ff

xstarspin	db	$1f,$fe					;SPINNING STAR

xs1f		db	$c0,$81,0,1,2,3,3,2,1,0,$e1,$ff

xenginer1	db	$20,$ff,$00				;ENGINE RUMBLE (DEEP)
xenginer2	db	$21,$ff,$00

xs20		db	$d1,$bf,$67,$02,$ff
xs21		db	$d1,$bf,$61,$0d,$ff	

xcannon01	db	$22,$fe					;CANNON FIRING
xcannon02	db	$23,$fe

xs22		db	$d2,$85,$69,$00,$ff

xs23		db	$d2,$85,$61,$09,$ff

xwarthin1	db	$25,$fe					;WAR-ALARM
xwarthin2	db	$26,$fe

xs25		db	$d3,$b0,$6a,$23
		db	$d4,$8c,$6a,$23
		db	$e1,$ff

xs26		db	$e3
		db	$d3,$b0,$6a,$23
		db	$d4,$8c,$6a,$23
		db	$e1,$ff

xlooser01	db	$81,$27,$fe				;WAR LOST
xlooser02	db	$81,$28,$fe
xlooser03	db	$81,$29,$fe
xlooser04	db	$2a,$fe

xs27		db	$e2
		db	$d1
		db	$88,$6d,$23,$6e,$1d,$6f,$18,$6d,$17,$6e,$11,$6f,$0c,$6d,$0b,$86,$6e,$05
		db	$d5,$84,$11,$11,$0f,$11,$e1		     
		db	$ff

xs28		db	$d6
		db	$88,$6d,$23,$6e,$1d,$6f,$18,$6d,$17,$6e,$11,$6f,$0c,$6d,$0b,$6e,$05
		db	$d5,$84,$14,$16,$0a,$0c,$e1		     
		db	$ff

xs29		db	$d8
		db	$88,$6b,$05
		db	$88,$6c,$05
		db	$88,$6b,$0b
		db	$88,$6c,$0b
		db	$88,$6b,$0c
		db	$88,$6c,$0c
		db	$88,$6b,$11
		db	$88,$6c,$11
		db	$84,$6b,$05,$0c,$03,$05,$e1
		db	$ff

xs2a		db	$c7,$90,$70,$00
		db	$c7,$90,$00
		db	$c7,$90,$00
		db	$c7,$90,$00
		db	$c7,$84,$00,$00,$00,$00,$e1
		db	$ff

xwinner01	db	$83,$2b,$fe				;WAR WON!
xwinner02	db	$83,$2c,$fe
xwinner03	db	$83,$2d,$fe
xwinner04	db	$2e,$fe

xs2b		db	$d7,$8c,$71,$0c,$84,$0c
		db	$d7,$8c,$72,$11,$84,$11
		db	$d7,$8c,$73,$13,$84,$13
		db	$d7,$86,$71,$18,$73,$13,$84,$71,$18
		db	$d7,$ac,$74,$1c
		db	$d7,$82,$75,$10,$10,$10
		db	$e1
		db	$ff
xs2c
		db	$d5,$8c,$13,$84,$13
		db	$d5,$8c,$18,$84,$18
		db	$d5,$8c,$1d,$84,$1d
		db	$d5,$86,$1f,$1d,$84,$1f
		db	$d5,$ac,$fd,$05,$22,$24
		db	$d5,$82,$18,$18,$83,$18
		db	$e1
		db	$ff
xs2d
		db	$d8
		db	$8c,$6b,$07,$84,$07
		db	$8c,$0c,$84,$0c
		db	$8c,$11,$84,$11
		db	$86,$13,$11,$84,$13
		db	$88,$0c,$07,$0c,$07
		db	$8c,$0c
		db	$ca,$82,$0c,$0c,$83,$0c
		db	$e1
		db	$ff
xs2e
		db	$c7,$8c,$70,$00,$84,$00
		db	$c7,$8c,$70,$00,$84,$00
		db	$c7,$8c,$70,$00,$84,$00
		db	$c7,$86,$70,$00,$00,$84,$00
		db	$c7,$88,$70,$00,$00,$00,$00
		db	$c7,$8c,$70,$00
		db	$c7,$82,$70,$00,$00,$84,$00
		db	$e1
		db	$ff

xgotplan1	db	$2f,$fe					;PLANET YOURS!
xgotplan2	db	$30,$fe

xs2f
		db	$d9,$82,$24,$29,$2b,$30,$e1
		db	$ff
xs30
		db	$e1,$cb,$82,$24,$29,$2b,$30,$e1
		db	$ff

xlosplan1	db	$31,$fe					;PLANET HIS
xlosplan2	db	$32,$fe

xs31
		db	$d9,$82,$1e,$1b,$18,$15,$e1
		db	$ff
xs32
		db	$e1,$cb,$82,$1e,$1b,$18,$15,$e1
		db	$ff

xexplolit	db	$33,$fe					;SINGLE EXPLO

xs33		db	$d1,$a0,$76,$00,$e1
		db	$ff

xpirats01	db	$8b,$34,$fe				;PIRATS!
xpirats02	db	$8b,$35,$fe
xpirats03	db	$8b,$36,$fe
xpirats04	db	$37,$fe

xs34		db	$d7
		db	$84,$78,$18
		db	$84,$78,$18
		db	$d5,$84,$0c,$0c,$0d,$0c,$12,$13
		db	$d7
		db	$84,$78,$16
		db	$84,$78,$18,$e4
		db	$84,$78,$19,$e4
		db	$84,$78,$1b
		db	$84,$78,$18,$e1
		db	$ff
xs35
		db	$d7
		db	$84,$77,$0c
		db	$84,$77,$0c
		db	$d5,$84,$79,$07,$07,$08,$07,$0b,$0c
		db	$d7
		db	$84,$77,$0a
		db	$84,$77,$0c,$e4
		db	$84,$77,$0d,$e4
		db	$84,$77,$0f
		db	$84,$77,$0c,$e1
		db	$ff
xs36
		db	$ca,$84,$6b,$00,$00
		db	$ca,$84,$6b,$00
		db	$ca,$84,$6c,$00
		db	$ca,$84,$6b,$01
		db	$ca,$84,$6c,$00
		db	$ca,$84,$6b,$06
		db	$ca,$84,$6c,$07
		db	$ca,$84,$6b,$0a,$0c,$e4,$84,$0d,$e4,$84,$0f,$0c,$e1
		db	$ff
xs37
		db	$c7,$84,$70,$00,$00
		db	$c7,$88,$00,$00,$00,$84,$00,$85,$00,$e3,$85,$00,$e3,$84,$00,$00,$e1
		db	$ff

xprepare1	db	$38,$fe					;SHIP PREPARED
xprepare2	db	$39,$fe

xs38		db	$d9,$82,$2c,$27,$22,$27,$2c,$e1
		db	$ff
xs39
		db	$e1
		db	$da,$82,$2c,$27,$22,$27,$2c,$e2
		db	$ff

xscraper1	db	$3a,$fe					;SHIP SCRAPPED
xscraper2	db	$3b,$fe

xs3a		db	$d9,$82,$2a,$29,$25,$24,$1d,$e1
		db	$ff
xs3b
		db	$e1
		db	$da,$82,$2a,$29,$25,$24,$1d,$e1
		db	$ff

xselecdo1	db	$3c,$fe					;SELECTSCRN DOWN
xselecdo2	db	$3d,$fe

xs3c		db	$d9,$81,$29,$24,$22,$1d,$18,$16,$11,$e1
		db	$ff
xs3d
		db	$e2
		db	$da,$81,$29,$24,$22,$1d,$18,$16,$11,$e1
		db	$ff

xselecup1	db	$3e,$fe					;SELECTSCRN UP
xselecup2	db	$3f,$fe

xs3e
		db	$d9,$81,$14,$19,$1b,$20,$25,$27,$2c,$e1
		db	$ff
xs3f
		db	$e2
		db	$da,$81,$14,$19,$1b,$20,$25,$27,$2c,$e1
		db	$ff

xselbeat1	db	$81,$40,$ff,$00
xselbeat2	db	$41,$ff,$00

xs40 
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$05
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$06
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$05
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$03
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$05
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$0a
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$05
		db	$dc,$8c,$7b,$05
		db	$dc,$84,$7b,$03
		db	$ff

xs41		db	$db,$a0,0
		db	$ca,$93,$7c,0,$ed
		db	$ff
		
		db	$db,$88,0,1,2,3,4,3,2,1,4,3,2,1,0,0,$d1,$90,$7c,0
		db	$ff	      
		db	$db,$8c,0,$84,0
		db	$db,$8c,0,$84,0
		db	$db,$8c,0,$84,0
		db	$db,$88,0,0
		db	$db,$8c,0,$84,0
		db	$db,$88,0,$84,0,0
		db	$db,$88,0,0
		db	$d1,$90,$7c,0
		db	$ff

xnintend1		db	$85,$42,$fe
xnintend2		db	$91,$43,$fe
xnintend3		db	$85,$44,$fe
xnintend4		db	$45,$fe

xs42		db	$de,$8c,$16,$82,$16,$16,$86,$16,$13,$84,$16,$84,$18,$18,$16,$18,$e1
		db	$ff
xs43
		db	$dd,$8c,$7d,$0a,$82,$0a,$0a,$86,$0a,$7e,$07,$84,$7d,$0a,$0c,$0c,$0a,$0c,$e1
		db	$ff
xs44
		db	$ca,$8c,$6b,$0a,$82,$0a,$0a,$86,$0a,$07,$84,$0a,$0c,$0c,$0a,$0c,$e1
		db	$ff
xs45
		db	$c7,$8c,$70,$00,$82,$00,$00,$86,$00,$00,$84,$00,$00,$00,$00,$00,$e1
		db	$ff

xhartbeat		db	$46,$fe

xs46		db	$f0,$f3
		db	$df,$90,$46,$46
		db	$ff

xiconrock		db	$47,$fe

xs47		db	$c0,$be,$0f,$e1
		db	$ff

xiconatmo		db	$71,$48,$fe	

xs48		db	$df,$a4,$01,$e1
		db	$ff

xiconcra1		db	$49,$fe
xiconcra2		db	$72,$4a,$fe

xs49
		db	$e2,$c0,$9f,$04,$fa,$a3,$04,$e1
		db	$ff
xs4a
		db	$e2,$df,$9f,$04,$fa,$a2,$04,$e1
		db	$ff

xiconpage		db	$4b,$fe

xs4b		db	$e5,$dc,$86,$7f,$00,$82,$02,$03,$e2,$82,$00,$e2,$90,$06,$e1
		db	$ff

xiconfis1		db	$89,$4c,$fe		
xiconfis2		db	$73,$4d,$fe		

xs4c		db	$e2
		db	$ca,$83,$65,0,$e1,$82,0,0,$83,0,$e1,$82,0,0,0,0
		db	$83,0,$e1,$83,0,$e1
		db	$ff
xs4d	     
		db	$e2
		db	$df,$83,0,$e1,$82,0,0,$83,0,$e1,$82,0,0,0,0
		db	$83,0,$e1,$83,0,$e1
		db	$ff

xiconhead		db	$4e,$fe

xs4e		db	$fc,$c0,$81
		db	0,0,1,1,2,1,2,2,3,2,3,3,4,3,4,4,5,4,5,5,6,5,6,6,7
		db	6,5,4,4,3,3,2,2,1,0,$e1,$ff		
xs4f






xstptablo	db	<xs00,<xs01,<xs02,<xs03,<xs04,<xs05,<xs06,<xs07,<xs08,<xs09,<xs0a,<xs0b,<xs0c,<xs0d,<xs0e,<xs0f
		db	<xs10,<xs11,<xs12,<xs13,<xs14,<xs15,<xs16,<xs17,<xs18,<xs19,<xs1a,<xs1b,<xs1c,<xs1d,<xs1e,<xs1f
		db	<xs20,<xs21,<xs22,<xs23,<xs24,<xs25,<xs26,<xs27,<xs28,<xs29,<xs2a,<xs2b,<xs2c,<xs2d,<xs2e,<xs2f
		db	<xs30,<xs31,<xs32,<xs33,<xs34,<xs35,<xs36,<xs37,<xs38,<xs39,<xs3a,<xs3b,<xs3c,<xs3d,<xs3e,<xs3f
		db	<xs40,<xs41,<xs42,<xs43,<xs44,<xs45,<xs46,<xs47,<xs48,<xs49,<xs4a,<xs4b,<xs4c,<xs4d,<xs4e,<xs4f

xstptabhi	db	>xs00,>xs01,>xs02,>xs03,>xs04,>xs05,>xs06,>xs07,>xs08,>xs09,>xs0a,>xs0b,>xs0c,>xs0d,>xs0e,>xs0f
		db	>xs10,>xs11,>xs12,>xs13,>xs14,>xs15,>xs16,>xs17,>xs18,>xs19,>xs1a,>xs1b,>xs1c,>xs1d,>xs1e,>xs1f
		db	>xs20,>xs21,>xs22,>xs23,>xs24,>xs25,>xs26,>xs27,>xs28,>xs29,>xs2a,>xs2b,>xs2c,>xs2d,>xs2e,>xs2f
		db	>xs30,>xs31,>xs32,>xs33,>xs34,>xs35,>xs36,>xs37,>xs38,>xs39,>xs3a,>xs3b,>xs3c,>xs3d,>xs3e,>xs3f
		db	>xs40,>xs41,>xs42,>xs43,>xs44,>xs45,>xs46,>xs47,>xs48,>xs49,>xs4a,>xs4b,>xs4c,>xs4d,>xs4e,>xs4f

xamptabl	db	<xam00,<xam01,<xam02,<xam03,<xam04,<xam05,<xam06,<xam07,<xam08,<xam09,<xam0a,<xam0b,<xam0c,<xam0d,<xam0e,<xam0f
		db	<xam10,<xam11,<xam12,<xam13,<xam14,<xam15,<xam16,<xam17,<xam18,<xam19,<xam1a,<xam1b,<xam1c,<xam1d,<xam1e,<xam1f
xamptabh	db	>xam00,>xam01,>xam02,>xam03,>xam04,>xam05,>xam06,>xam07,>xam08,>xam09,>xam0a,>xam0b,>xam0c,>xam0d,>xam0e,>xam0f
		db	>xam10,>xam11,>xam12,>xam13,>xam14,>xam15,>xam16,>xam17,>xam18,>xam19,>xam1a,>xam1b,>xam1c,>xam1d,>xam1e,>xam1f

xvibtabl	db	<xv01,<xv02,<xv03,<xv04,<xv05,<xv06,<xv07,<xv08,<xv09,<xv0a,<xv0b,<xv0c,<xv0d,<xv0e,<xv0f
xvibtabh	db	>xv01,>xv02,>xv03,>xv04,>xv05,>xv06,>xv07,>xv08,>xv09,>xv0a,>xv0b,>xv0c,>xv0d,>xv0e,>xv0f

xpultabl	db	<xp01,<xp02,<xp03,<xp04,<xp05,<xp06,<xp07,<xp08,<xp09,<xp0a,<xp0b,<xp0c,<xp0d,<xp0e,<xp0f
xpultabh	db	>xp01,>xp02,>xp03,>xp05,>xp05,>xp06,>xp07,>xp08,>xp09,>xp0a,>xp0b,>xp0c,>xp0d,>xp0e,>xp0f

xarptablo	db	<xa00,<xa01,<xa02,<xa03,<xa04,<xa05,<xa06,<xa07,<xa08,<xa09,<xa0a,<xa0b,<xa0c,<xa0d,<xa0e,<xa0f
		db	<xa10,<xa11,<xa12,<xa13,<xa14,<xa15,<xa16,<xa17,<xa18,<xa19,<xa1a,<xa1b,<xa1c,<xa1d,<xa1e,<xa1f
xarptabhi	db	>xa00,>xa01,>xa02,>xa03,>xa04,>xa05,>xa06,>xa07,>xa08,>xa09,>xa0a,>xa0b,>xa0c,>xa0d,>xa0e,>xa0f
		db	>xa10,>xa11,>xa12,>xa13,>xa14,>xa15,>xa16,>xa17,>xa18,>xa19,>xa1a,>xa1b,>xa1c,>xa1d,>xa1e,>xa1f

INTPLAYOS
	lda	gldspeed,x
	beq	INTPLAYOSOUT
	tay	
	lsr	a
	lsr	a
	lsr	a
	lsr	a
	sta	glddelay,x
	tya
	and	#$0f
	sta	gldspeed,x

	lda	realleng,x
	sec
	sbc	glddelay,x
	sta	glddelay,x
	lda	realnote,x
	sec
	sbc	destnote,x
	sta	glddirec,x
INTPLAYOSOUT

CLEARZOOI	lda	#$00
	sta	ampindex,x
	sta	pulindex,x
	sta	vibindex,x
	sta	spacecnt,x
	sta	arpindex,x

	lda	fx02,x
	lsr	a
	lsr	a
	lsr	a
	lsr	a
	sta	vibdelay,x
	rts

SHITAMP
	ldy	ampindex,x
	lda	(ssslo),y
	cmp	#$ff
	bne	xAMPL00
	iny
	lda	(ssslo),y
	sta	ampindex,x
	tay
	lda	(ssslo),y
       
xAMPL00	cmp	#$fe
	beq	xNOAMPL1
	sta	amplitud,x
	iny
	tya
	sta	ampindex,x
xNOAMPL1
	rts	

INITREALOS
	lda	#$00	 
	sta	seqindex,x
	sta	spdcheck,x
	cpx	#$02
	bne	xMORESP1
	lda	#$01
	sta	spdcount,x
	lda	#$00
	beq	xMORESP2
xMORESP1 sta	spdcount,x
xMORESP2	sta	noteinit,x
	sta	tranbyte,x
	sta	stepnumb,x
	sta	ampindex,x
	sta	amplitud,x
	sta	sfxtrans,x
	sta	arptrans,x
	sta	continos,x
	lda	#$01
	sta	voictest,x
	sta	repstcnt,x
	sta	noteleng,x
	lda	#4015wrd	;VOICE 01 - ?? enabled
	sta	$4015
	rts

SEQZOOI
	ldy	seqindex,x	;read sequence-index
	lda	(ssslo),y	;
	cmp	#$ff		;check sequence on end-repeat
	bne	SEQHD12		;
	iny			;
	lda	(ssslo),y	;where to jump back in sequence
	sta	seqindex,x	;
	tay			;
	lda	(ssslo),y	;
	jmp	SEQHD15		;goto next sequence

SEQHD12	cmp	#$fe		;check sequence on end-real
	bne	SEQHD13		;
	lda	#$00		;yes, disable voice
	sta	voictest,x	;
	rts
SEQHD13	
SEQHD15	cmp	#$80		;note transpose	check
	bcc     SEQHD16  
	and	#$7f
	sta	tranbyte,x
	iny
	lda	(ssslo),y

SEQHD16	cmp	#$70		;sfix transpose	check
	bcc	SEQHD17
	and	#$0f
	sta	sfxtrans,x
	iny
	lda	(ssslo),y

SEQHD17	cmp	#$60		;repeat step check
	bcc	SEQHD18
	and	#$0f
	sta	repstcnt,x
	iny
	lda	(ssslo),y

SEQHD18	cmp	#$50		;arpg transpose check
	bcc	SEQZOOIOUT
	and	#$0f
	sta	arptrans,x
	iny
	lda	(ssslo),y
SEQZOOIOUT
	rts

JSRVIB	ldy	vibindex,x
	lda	(ssslo),y
	cmp	#$80
	bne	xVIB01
	iny
	lda	(ssslo),y
	sta	vibindex,x
	tay
	lda	(ssslo),y
xVIB01	rts

JSRPUL	ldy	pulindex,x
	lda	(ssslo),y
	cmp	#$ff
	bne	xPULS01
	iny
	lda	(ssslo),y
	sta	pulindex,x
	tay
	lda	(ssslo),y

xPULS01	cmp	#$fe
	beq	JSRPULOUT
	sta	pulsbyt1,x
	iny
	tya
	sta	pulindex,x	
JSRPULOUT rts
;---------------------------------------------------------------------------------------------------------------------------------




	org	$fffa
	dw	nmi
	dw	resetvector
	dw	irq


	end




