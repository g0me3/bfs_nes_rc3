;
;	probe software nintendo famicom music-driver
;
;	developed & programmed by JEROEN TEL
;
;	copyright 1991 by probe software limited
;	 
;	all rights reserved. for use under license only. 
;	
;************************************************************************


	header	'p120'
    	ext	'pgm'

	org	$c000,7


pcr0    	=	$2000	;ppu 	control register 1
pcr1    	=	$2001	;ppu 	control register 2
pstr    	=	$2002	;	status register
dmacnt  	=	$2003	;	dma page count
psor    	=	$2005	;	scroll offset register
pmar    	=	$2006	;	memory address register
pmdr    	=	$2007	;	memory data register
joy1port	=	$4016	;joystick port 1
joy2port	=	$4017	;joystick port 2

joy1last	=	$04
joy1data	=	$05
joy1delay	=	$06
joy1edge	=	$07

aa	=	$08
runflag	=	$09

sfxcount	=	$0a
sfxdelay	=	$0b

joybuta	=	%10000000
joybutb	=	%01000000
joyselect	=	%00100000
joystart	=	%00010000
joyup	=	%00001000
joydown	=	%00000100
joyleft	=	%00000010
joyright	=	%00000001

resetvector	sei
	lda	#0
	sta	pcr0	;init the control registers
	sta	pcr1

ajohnny	lda	pstr	;wait for hardware to stabilise.
	bpl	ajohnny
bjohnny	lda	pstr
	bpl	bjohnny
cjohnny	lda	pstr
	bmi	cjohnny

	
	ldx	#$ff
	txs

	ldx	#$00
	txa
clearmem
	sta	$0000,x
	sta	$0100,x
	sta	$0200,x
	sta	$0300,x
	sta	$0400,x
	sta	$0500,x
	sta	$0600,x
	sta	$0700,x
	inx
	bne	clearmem

	jsr	audiocl
      
	lda	#initval0
	ldx	#$00
	jsr 	audioin
       	lda	#initval1
       	ldx	#$01
       	jsr	audioin
	lda	#initval2
	ldx	#$02
	jsr	audioin
	lda	#initval3
	ldx	#$03
	jsr	audioin 

	lda	#audtstart
	sta	sfxcount

	lda	#$80	;enable nmi's
	sta	pcr0




	;wait for flag to be set by nmi

processor	lda	runflag  
	beq	processor
	lda	#0       
	sta	runflag

	ldx	#$10;6
	ldy	#$00
dly	dey
	bne	dly
	dex
	bne	dly


	lda	#$00	;set tint
       	sta	pcr1

	ldx	#$00
       	jsr	audiopl	;play audio-routine
        ldx	#$01
        jsr	audiopl
       	ldx	#$02
       	jsr	audiopl
       	ldx	#$03
       	jsr	audiopl

        jsr	readjoy
	jsr	docoms

	lda	#$80
	sta	pcr1
        jmp	processor

nmi	sta	aa

     	lda	#1
     	sta	runflag

	lda	aa
irq	rti



readjoy	ldy	#1
	sty	joy1port
	dey
	sty	joy1port

	ldy	#8	;number of bits to read.
readj1	pha
	lda	joy1port
	sta	joy1last
	lsr	
 	ora	joy1last
	lsr	
	pla
	rol	
	dey
	bne	readj1

	ldy	joy1data
	sty	joy1last
	sta	joy1data
	eor	joy1last
	and	joy1data
	sta	joy1edge
	ldy	#$10
	lda	joy1data
	cmp	joy1last
	bne	readj2
	dec	joy1delay
	bne	readj3
	sta	joy1edge
	ldy	#12
readj2	sty	joy1delay
readj3	rts

docoms	lda	joy1data
   	and	#joybuta
   	beq	skip
	
	ldx	#$00
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	ldx	#$01
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	ldx	#$02
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	ldx	#$03
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl
	jsr	audiopl

skip	lda	joy1data
   	and	#joybutb
	beq	hond
	dec	sfxdelay
	bpl	hond
	lda	#$07
	sta	sfxdelay
	inc	sfxcount
hemdaar	ldy	sfxcount
	lda	sfxtab0,y
	ldx	#$00
	jsr	audioin
	lda	sfxtab1,y
	ldx	#$01
	jsr	audioin
	lda	sfxtab2,y
	ldx	#$02
	jsr	audioin
	lda	sfxtab3,y
	ldx	#$03
	jsr	audioin
	rts

hond	lda	joy1data
	and	#joystart
	beq	hond1
	jmp	hemdaar
hond1	rts

;-----------------------------------------------------------------------------
sfxtab0	db	$01,$04,$07,$0a,$0d,$10,$13,$16,$19,$1d,$20,$23,$27,$2b,$2f
	db	$32,$36,$39,$3c,$3f,$42,$46,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

sfxtab1	db	$00,$00,$00,$00,$00,$00,$00,$00,$1a,$00,$00,$24,$28,$2c,$00
	db	$33,$00,$00,$00,$00,$43,$47,$4a,$4b,$4c,$4d,$4e,$4f,$50,$51
	db	$52,$53,$54,$55,$56,$57,$58,$59,$5a,$5b,$5c,$5d,$5e,$5f,$60
	db	$61,$62,$63,$64,$65,$66,$67,$68,$69,$6a,$6b,$6c,$6d,$6e,$6f
	db	$70,$71,$72,$73,$74,$75,$76,$77,$78,$79,$7a,$7b,$7c,$7d,$7e
	db	$7f,$80,$81,$82,$83,$84,$85,$86,$87,$88,$89,$8a,$8b,$8c,$8d
	db	$8e,$8f,$90,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

sfxtab2	db	$02,$05,$08,$0b,$0e,$11,$14,$17,$1b,$1e,$21,$25,$29,$2d,$30
	db	$34,$37,$3a,$3d,$40,$44,$48,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

sfxtab3	db	$03,$06,$09,$0c,$0f,$12,$15,$18,$1c,$1f,$22,$26,$2a,$2e,$31
	db	$35,$38,$3b,$3e,$41,$45,$49,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

initval0	=	$46
initval1	=	$47
initval2	=	$48
initval3	=	$49

audtstart	=	$00



;-----------------------------------------------------------------------------
;everything up here can be removed before being inserted into game-source !!!
;-----------------------------------------------------------------------------
;
;
;
;
;
; SPECIFICATIONS FOR PROGRAMMER:-
;
;To initialise the audio-driver: "jsr audiocl" in game-initialisations.
;
;
;
;
;To initialise a specific tune or soundeffect look it up in "seqtablo"
;You'll see the initialisation-value right from the names. 
;
;An example:
;
; You want to play the "beautyintro"
;
; lda #$2b	   ; As you can see in the "seqtablo", this tune uses
; ldx #$00	   ; four channels.
; jsr audioin	   ;
; lda #$2c	   ; The ingame-level-tunes use channel (x) #$00,#$02,#$03
; ldx #$01	   ;
; jsr audioin	   ; The soundeffects have to be played on channel (x) #$01
; lda #$2d	   ;
; ldx #$02	   ;
; jsr audioin	   ;
; lda #$2e	   ;
; ldx #$03	   ;
; jsr audioin	   ;
;
; To clear any channel (stop a soundeffect or tune from playing):
;
; lda #$00 (empty sequence)
; ldx #$0x (any channel from $00-$03)
; jsr audioin
;
;
;
; To play the audio: jsr audiopl (pal :50 times/second)
;				 (ntsc:60 times/second)
;
;
; Please, use it properly...
;
;
; Best regards,
;
;
; Jeroen Tel / Maniacs of Noise
;

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;!!!!!!!!!!!!!!!!!!!!!!!!	                       !!!!!!!!!!!!!!!!!!!!!!!
;!!!!!!!!!!!!!!!!!!!!!!!!	P  A  L   VERSION      !!!!!!!!!!!!!!!!!!!!!!!
;!!!!!!!!!!!!!!!!!!!!!!!!	                       !!!!!!!!!!!!!!!!!!!!!!!
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

;-----------------------------------------------------------------------------
;NES Audio-driver - Copyright 1993 Jeroen Tel & (c) 1993 Probe Software Ltd
;-----------------------------------------------------------------------------

;game		:Beauty & the Beast 
;type		:ingame music & soundeffects
;format		:Nintendo Entertainment System (8bit)
;composer/sfx	:Jeroen Tel
;game		:Beauty & the Beast 
;Development	:Probe Software Ltd
;Publisher	:Someone
;date		:07/11/93

ssslo	=	$fe	
ssshi	=	$ff	

enabword	=	$0400
voictest 	=	enabword+$01
stpindex 	=	voictest+$04
seqindex 	=	stpindex+$04
seqposit 	=	seqindex+$04
stepnumb 	=	seqposit+$04
spdcount 	=	stepnumb+$04
spdindex 	=	spdcount+$04
realplay	=	spdindex+$04
noteinit	=	realplay+$04
realsoun 	=	noteinit+$04
lengcopy 	=	realsoun+$04
noteleng 	=	lengcopy+$04
realnote 	=	noteleng+$04
lofrqwrd	=	realnote+$04
hifrqwrd	=	lofrqwrd+$04
tranbyte 	=	hifrqwrd+$04
sfxtrans	=	tranbyte+$04
arpindex	=	sfxtrans+$04
arpnumbo	=	arpindex+$04
arponoto	=	arpnumbo+$04
vibglbyt	=	arponoto+$04
gldspeed	=	vibglbyt+$04
glddirec	=	gldspeed+$04
glddelay	=	glddirec+$04
destnote	=	glddelay+$04
vibdelay	=	destnote+$04
vibindex	=	vibdelay+$04
continos	=	vibindex+$04
amplitud 	=	continos+$04
ampindex	=	amplitud+$04
fx01		=	ampindex+$04
fx02		=	fx01+$04	
fx03		=	fx02+$04	
pulindex	=	fx03+$04	
pulsbyt1 	=	pulindex+$02 

;!!! just values !!!

stepbank2	=	$3f
sndbank2	=	$4a

;!!! just values !!!


	jmp	audiocl
	jmp	audioin
	jmp	audiopl

	db	"   beauty and the beast music and soundeffects (c) 1993"
	db	" jeroen tel / maniacs of noise   "

audiocl	lda	#$00
	sta	enabword
	rts

audioin	sta	seqposit,x	

	lda	enabword
	ora	enabchan,x
	sta	enabword
	sta	$4015
	lda	#$00	 
	sta	seqindex,x
 	sta	spdindex,x
	sta	tranbyte,x
	sta	stepnumb,x
	sta	ampindex,x
	sta	amplitud,x
	sta	tranbyte,x
	sta	sfxtrans,x
	sta	continos,x
speedcn2     	sta	spdcount,x
       	lda	#$01
	sta	voictest,x
;	sta	repstcnt,x
	sta	lengcopy,x
	sta	stpindex,x
	rts

enabchan	db	$01,$02,$04,$08

audiopl	lda	voictest,x
	beq	audioex
       	lda	enabword
       	sta	$4015
	cpx	#$00
	beq	voice01
       	cpx	#$01
       	beq	voice02
       	cpx	#$02
       	beq	voice03
       	cpx	#$03
       	beq	voice04
audioex	rts


voice04	txa
	asl
	asl
	tay
	lda	#$10		;set amplitude <bit 04>
	ora	#$20  		;continuous lenght enabled <bit 05>
	ora	amplitud,x	;amplitude/envelope value <bit 03-00>
	sta	$4000,y
	lda	lofrqwrd,x
	sta	$4002,y
	lda	#$f8
	sta	$4003,y
	jmp	jmpvo04	

voice03	txa
	asl
	asl
	tay
	lda	#$80
	jmp	jmpvo03
voice01
voice02 
	
setvoi1	txa
	asl
	asl
	tay
	lda	pulsbyt1,x    	;setpulse <bit 07-06>
	ora	#$10    		;set amplitude <bit 04>
	ora	#$20		;continuous lenght enabled	<bit 05>
jmpvo03	ora	amplitud,x	;amplitude/envelope value 	<bit 03-00>
	sta	$4000,y

	lda	#$08		;hardware-glide down		<bit 03>
	sta	$4001,y		;

	lda	lofrqwrd,x
	sta	$4002,y

	lda	hifrqwrd,x	;check if hi-frq thesame as note before
	cmp	continos,x
	beq	nohino1
	sta	$4003,y		;hf set
	sta	continos,x
nohino1

jmpvo04

spdhdl1	lda	noteleng,x
	cmp	#$01
	beq	speedrout
	lda	lengcopy,x
	cmp	#$01
	bne	speedrout
	lda	spdcount,x
	bne	speedrout
	lda	#$00
	sta	amplitud,x
speedrout
	
	
	lda	#$00		;speedroutine (two speed bytes)
	sta	realplay,x
	dec	spdcount,x
	bpl	spdhdx1

	ldy	seqposit,x
	lda	spdtablo,y
	sta	ssslo
	lda	spdtabhi,y
	sta	ssshi

	ldy	spdindex,x
	lda	(ssslo),y
	cmp	#$ff
	bne	setspeed
	ldy	#$00
	lda	(ssslo),y
setspeed	sta	spdcount,x
	iny
	tya
	sta	spdindex,x
	lda	#$ff
	sta	realplay,x
spdhdx1	
       	lda	realplay,x	;check if new leng subtraction necessary
	bpl	nostph1		;

audipl1	dec	lengcopy,x	;decrease speed, if zero then stephandle
	beq	stphdl1	

nostph1	lda	realnote,x	;check realnote on pause, no fx
	cmp	#$ff		;
	beq	nofxpl1		;
	jsr	playfix		;

nofxpl1	rts			;

stphdl1	ldy	stepnumb,x	;what step to take

	lda	seqposit,x
	cmp	#stepbank2
	bcs	nextbnk2

	lda	stptablo,y	;
	sta	ssslo		;
	lda	stptabhi,y	;
	jmp	sethstp
nextbnk2
	lda	stptabl2,y	;
	sta	ssslo		;
	lda	stptabh2,y	;
sethstp	sta	ssshi		;

	lda	#$00		;reset glide (if not reached dest. note)
	sta	gldspeed,x

	ldy	stpindex,x	;read step-index
	lda	(ssslo),y		;
	cmp	#$ff		;step check on end 
	beq	seqhdl1		;yes, goto sequence-handle
	jmp	stphd12		;no, goto stephandle 02

seqhdl1
;	dec	repstcnt,x	;check if step repeats
;	beq	realsq1		;no, goto real sequence-handle
;	lda	#$00		;yes, reset index & do last step again
;	sta	stpindex,x	;
;	tay			;
;	lda	(ssslo),y		;
;	jmp	stphd12		;

realsq1	ldy	seqposit,x	;what sequence to take
	lda	seqtablo,y	;
	sta	ssslo		;
	lda	seqtabhi,y	;
	sta	ssshi		;

	ldy	seqindex,x	;read sequence-index
	lda	(ssslo),y	;
	cmp	#$ff		;check sequence on end-repeat
	bne	seqhd12		;
	iny			;
	lda	(ssslo),y	;where to jump back in sequence
	sta	seqindex,x	;
	tay			;
	lda	(ssslo),y	;
	jmp	seqhd15		;goto next sequence

seqhd12	cmp	#$fe		;check sequence on end-real
	bne	seqhd13		;
	lda	#$00		;yes, disable voice
	sta	voictest,x	;
	sta	amplitud,x
	rts

seqhd13	cmp	#$fc		;!!!! do the fade! goddamned !!!!
	bne	seqhd15		;!!!! still to be programmed !!!!

seqhd15	cmp	#$80		;note transpose	check
	bcc	seqhd16  	      
       	and	#$7f
notrnsi	sta	tranbyte,x
	iny
	lda	(ssslo),y
	bmi	nonwstp

seqhd16	cmp	#$70		;sfix transpose	check
	bcc	seqhd17
	and	#$0f
	sta	sfxtrans,x
	iny
	lda	(ssslo),y

seqhd17	
;	cmp	#$60		;repeat step check
;	bcc	seqhd19
;	and	#$0f
;	sta	repstcnt,x
;	iny
;	lda	(ssslo),y

seqhd19	sta	stepnumb,x	;set step & clear stepindex
	iny
nonwstp	tya
	sta	seqindex,x
;	inc	repstcnt,x	;make sure step plays once
	lda	#$00
	sta	stpindex,x
	jmp	stphdl1		;back to step-handle

stphd12	cmp	#$fd		;glide check in step
	bne	stphd14
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
	jmp	nonewn1

stphd14	cmp	#$e0		;pause check
	bcc	stphd15
	and	#$1f
	sta	noteleng,x
	lda	#$ff
	sta	realnote,x
	lda	#$00
	sta	amplitud,x
	iny
	tya
	sta	stpindex,x
	jmp	pausex1

stphd15	cmp	#$c0		;sound check
	bcc	stphd16
	clc
	adc	sfxtrans,x
	asl
	asl
	sta	realsoun,x
	iny
	lda	(ssslo),y
	cmp	#$fd	
	beq	stphd12
	cmp	#$c0
	bcs	nonewn1
	
stphd16	cmp	#$80		;length check
	bcc	stphd17
	and	#$7f
	sta	noteleng,x
	iny
	lda	(ssslo),y
	cmp	#$fd
	beq	stphd12
	cmp	#$80
	bcs	nonewn1

stphd17	cmp	#$60		;arp check
	bcc	stphd18
	and	#$1f	
	sta	arpnumbo,x
	iny
	lda	(ssslo),y
	cmp	#$60
	bcs	nonewn1

stphd18	clc			;set note + note transpose
	adc	tranbyte,x	;
	sta	realnote,x	;
	iny			;
nonewn1	tya
	sta	stpindex,x	;restore step-index
	
	ldy	realsoun,x	;
	cpx	#$00		;check sound-init voice 01
	beq	initv12		;
	cpx	#$01		;check sound-init voice 02
	beq	initv12		;
	cpx	#$02		;check sound-init voice 03
	beq	initv03		;
	cpx	#$03		;check sound-init voice 04
	beq	initv04		;
	rts

initv12	
	lda	seqposit,x
	cmp	#sndbank2
	bcs	readfx2a

	lda	soundfix+$00,y	;check if new pulse to be set
	and	#$08		;
	bne	nonewpl		;
	lda	soundfix+$00,y	;yes, set new pulse
	and	#$c0		;
	sta	pulsbyt1,x     	;
	lda	#$00
	sta	pulindex,x
	jmp	nonewpl
readfx2a
	lda	soundfx2+$00,y	;check if new pulse to be set
	and	#$08		;
	bne	nonewpl		;			      
	lda	soundfx2+$00,y	;yes, set new pulse
	and	#$c0		;
	sta	pulsbyt1,x     	;
	lda	#$00					      
	sta	pulindex,x
initv03
initv04
nonewpl
	lda	seqposit,x
	cmp	#sndbank2
	bcs	readfx2b 
	
	lda	soundfix+$01,y	;set vibrato-byte into fx01 & set effects (arp, etc)
	sta	fx01,x		;
	lda	soundfix+$02,y	;set vibdelay pultab-number into fx02
	sta	fx02,x		;
	
	lda	soundfix+$03,y	;set effect-values into fx03	
	jmp	setfx03

readfx2b
	lda	soundfx2+$01,y	;set vibrato-byte into fx01 & set effects (arp, etc)
	sta	fx01,x		;
	lda	soundfx2+$02,y	;set vibdelay pultab-number into fx02
	sta	fx02,x		;
	
	lda	soundfx2+$03,y	;set effect-values into fx03	

setfx03	sta	fx03,x		;
	cpx	#$03
	beq	nforv04

	ldy	realnote,x	;read realnote as index
	lda	frqtablo,y 	;low-frequency read <bit 07-00>
	sta	lofrqwrd,x	;

	lda	#$f8		;hardware length endless <bit 07-03>
	ora	frqtabhi,y 	;hi-frequency read <bit 02-00>
	sta	hifrqwrd,x	;hf set
	jmp	pausex1

nforv04	lda	realnote,x
	sta	lofrqwrd,x

pausex1	lda	noteleng,x
       	sta	lengcopy,x

	lda	#$00
      	sta	ampindex,x
	sta	vibindex,x
       	sta	arpindex,x
;-----------------------------------------------------------------------------
intpla1	lda	gldspeed,x
	beq	fxpla12
	tay	
	lsr
	lsr
	lsr
	lsr
	sta	glddelay,x
	tya
	and	#$0f
	sta	gldspeed,x

	lda	noteleng,x
	sec
	sbc	glddelay,x
	sta	glddelay,x
	lda	realnote,x
	sec
	sbc	destnote,x
	sta	glddirec,x

fxpla12	lda	fx02,x
	lsr
	lsr
	lsr
	lsr
	sta	vibdelay,x
	lda	noteleng,x
	sec
	sbc	vibdelay,x
	bcs	refix01
	lda	fx01,x
	and	#$c0
	sta	fx01,x
	jmp	exitin1
refix01	sta	vibdelay,x
exitin1 rts

playfix	lda	fx03,x			;amplitude routine		
       	and	#$7f			
       	tay
       	lda	amptabl,y
       	sta	ssslo
       	lda	amptabh,y
       	sta	ssshi
       
       	ldy	ampindex,x
       	lda	(ssslo),y
	cmp	#$ff
	bne	ampy1
	iny
	lda	(ssslo),y
	sta	ampindex,x
	tay
	lda	(ssslo),y
ampy1 	cmp	#$fe
       	beq	noampl1
       	sta	amplitud,x
       	inc	ampindex,x
noampl1	

glide1	ldy	gldspeed,x		;glide routine
	beq	nogld12

	lda	glddelay,x
	cmp	lengcopy,x
	bcc	nogld12

	ldy	glddirec,x
	bmi	downgl1
	lda	lofrqwrd,x
	clc
	adc	gldspeed,x
	sta	lofrqwrd,x
	bcc	checkv1
       	inc	hifrqwrd,x

checkv1	ldy	destnote,x
	sec
	sbc	frqtablo,y
	bcc	nogld01
	bcs	froter1

downgl1	lda	lofrqwrd,x
	sec
	sbc	gldspeed,x
	sta	lofrqwrd,x
	bcs	checkv2
	dec	hifrqwrd,x
	
checkv2	ldy	destnote,x
	sec
	sbc	frqtablo,y
	bcs	nogld01
froter1	tya
	sta	realnote,x
       	lda	frqtablo,y
       	sta	lofrqwrd,x
       	lda	frqtabhi,y
       	sta	hifrqwrd,x
	
	lda	#$00
	sta	gldspeed,x
nogld01	jmp	novib01
nogld12
	lda	gldspeed,x
	bne	novib01

vibra01	
	lda	fx01,x	 		;vibrato routine
	and	#$0f
	beq	novib01
	lda	vibdelay,x
	cmp	lengcopy,x
	bcc	novib01

	lda	fx01,x
	and	#$0f
	tay
	lda	vibtabl-1,y
	sta	ssslo
	lda	vibtabh-1,y
	sta	ssshi

	ldy	vibindex,x
	lda	(ssslo),y
	cmp	#$80
	bne	vib01
	ldy	#$00
vib01	lda	(ssslo),y
	bpl	upvibos
	eor	#$ff
	clc
	adc	#$01
	sta	ssslo
	lda	lofrqwrd,x
	sec
	sbc	ssslo
	sta	lofrqwrd,x
	bcs	nohigh1
	dec	hifrqwrd,x
nohigh1	jmp	vibno01
       
upvibos	lda	lofrqwrd,x
	clc
	adc	(ssslo),y
	sta	lofrqwrd,x
	bcc	nohigh2
       	inc	hifrqwrd,x
nohigh2		
vibno01	iny
	tya
	sta	vibindex,x
novib01

pulse1 	cpx	#$02
       	bcs	nopuls1
       	lda	fx02,x			;pulse routine
       	and	#$0f
       	beq	nopuls1
       	tay
       	lda	pultabl-1,y
       	sta	ssslo
       	lda	pultabh-1,y
       	sta	ssshi

       	ldy	pulindex,x
       	lda	(ssslo),y
	cmp	#$fe
	beq	nopuls1
       	cmp	#$ff
       	bne	puls01
       	iny
       	lda	(ssslo),y
	tay
       	lda	(ssslo),y

puls01 	sta	pulsbyt1,x
       	iny
       	tya
	sta	pulindex,x	
nopuls1	

arpeg01	lda	fx01,x		;arp routine
	and	#$40		
	beq	noarp01
	ldy	arpnumbo,x
	lda	seqposit,x
	cmp	#sndbank2
	bcs	arpbank2
	lda	arptablo,y
	sta	ssslo
	lda	arptabhi,y
	jmp	stahiarp
arpbank2
	lda	arptabl2,y
	sta	ssslo
	lda	arptabh2,y
stahiarp
	sta	ssshi

	ldy	arpindex,x
     	lda	(ssslo),y
	cmp	#$fe
	beq	noarp01
	cmp	#$ff
	bne	arp03
	iny
	lda	(ssslo),y
	tay
	lda	(ssslo),y
arp03	bmi	norma01
	clc
	adc	realnote,x
norma01	and	#$7f
	sta	arponoto,x
	iny
	tya
	sta	arpindex,x
arpno01	lda	arponoto,x
	cpx	#$03
	bne	setan01
	sta	lofrqwrd,x
	jmp	noarp01
setan01
	tay
	lda	frqtablo,y
	sta	lofrqwrd,x
	lda	frqtabhi,y
	sta	hifrqwrd,x
noarp01	rts
;-----------------------------------------------------------------------------
frqtablo	db	$ad,$4a,$ef,$9b,$4b,$fe,$b5,$73,$30,$f5,$bd,$87 ;1
        	db	$56,$25,$f8,$cd,$a5,$7f,$5b,$3a,$19,$fb,$de,$c3 ;2
        	db	$aa,$92,$7b,$66,$52,$3f,$2d,$1c,$0c,$fd,$ef,$e1 ;3
        	db	$d4,$c9,$bd,$b3,$a9,$9f,$96,$8e,$86,$7e,$77,$70 ;4
        	db	$6a,$64,$5e,$59,$54,$4f,$4a,$47,$42,$3e,$3b,$38 ;5
        	db	$35,$31,$2e,$2c,$29,$27,$25,$23,$21,$1f,$1d,$1b ;6
        	db	$1a,$19,$18,$16,$15,$14,$13,$12,$11,$10,$0f,$0e ;7
        	db	$07	  	

frqtabhi	db	$06,$06,$05,$05,$05,$04,$04,$04,$04,$03,$03,$03 ;1
        	db	$03,$03,$02,$02,$02,$02,$02,$02,$02,$01,$01,$01 ;2
        	db	$01,$01,$01,$01,$01,$01,$01,$01,$01,$00,$00,$00 ;3
        	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;4
        	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;5
        	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;6
        	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00 ;7
        	db	$00
;-----------------------------------------------------------------------------

stptablo
		db	<s00,<s01,<s02,<s03,<s04,<s05,<s06,<s07,<s08,<s09,<s0a,<s0b,<s0c,<s0d,<s0e,<s0f
        	db	<s10,<s11,<s12,<s13,<s14,<s15,<s16,<s17,<s18,<s19,<s1a,<s1b,<s1c,<s1d,<s1e,<s1f
        	db	<s20,<s21,<s22,<s23,<s24,<s25,<s26,<s27,<s28,<s29,<s2a,<s2b,<s2c,<s2d,<s2e,<s2f
        	db	<s30,<s31,<s32,<s33,<s34,<s35,<s36,<s37,<s38,<s39,<s3a,<s3b,<s3c,<s3d,<s3e,<s3f
        	db	<s40,<s41,<s42,<s43,<s44,<s45,<s46,<s47,<s48,<s49,<s4a,<s4b,<s4c,<s4d,<s4e,<s4f
        	db	<s50,<s51,<s52,<s53,<s54,<s55,<s56,<s57,<s58,<s59,<s5a,<s5b,<s5c,<s5d,<s5e,<s5f
        	db	<s60,<s61,<s62,<s63,<s64,<s65,<s66,<s67,<s68,<s69,<s6a,<s6b,<s6c,<s6d,<s6e,<s6f
stptabhi
		db	>s00,>s01,>s02,>s03,>s04,>s05,>s06,>s07,>s08,>s09,>s0a,>s0b,>s0c,>s0d,>s0e,>s0f
        	db	>s10,>s11,>s12,>s13,>s14,>s15,>s16,>s17,>s18,>s19,>s1a,>s1b,>s1c,>s1d,>s1e,>s1f
        	db	>s20,>s21,>s22,>s23,>s24,>s25,>s26,>s27,>s28,>s29,>s2a,>s2b,>s2c,>s2d,>s2e,>s2f
        	db	>s30,>s31,>s32,>s33,>s34,>s35,>s36,>s37,>s38,>s39,>s3a,>s3b,>s3c,>s3d,>s3e,>s3f
        	db	>s40,>s41,>s42,>s43,>s44,>s45,>s46,>s47,>s48,>s49,>s4a,>s4b,>s4c,>s4d,>s4e,>s4f
        	db	>s50,>s51,>s52,>s53,>s54,>s55,>s56,>s57,>s58,>s59,>s5a,>s5b,>s5c,>s5d,>s5e,>s5f
	       	db	>s60,>s61,>s62,>s63,>s64,>s65,>s66,>s67,>s68,>s69,>s6a,>s6b,>s6c,>s6d,>s6e,>s6f
stptabl2
		db	<x00,<x01,<x02,<x03,<x04,<x05,<x06,<x07,<x08,<x09,<x0a,<x0b,<x0c,<x0d,<x0e,<x0f
        	db	<x10,<x11,<x12,<x13,<x14,<x15,<x16,<x17,<x18,<x19,<x1a,<x1b,<x1c,<x1d,<x1e,<x1f
        	db	<x20,<x21,<x22,<x23,<x24,<x25,<x26,<x27,<x28,<x29,<x2a,<x2b,<x2c,<x2d,<x2e,<x2f
        	db	<x30,<x31,<x32,<x33,<x34,<x35,<x36,<x37,<x38,<x39,<x3a,<x3b,<x3c,<x3d,<x3e,<x3f
        	db	<x40,<x41,<x42,<x43,<x44,<x45,<x46,<x47,<x48
stptabh2
		db	>x00,>x01,>x02,>x03,>x04,>x05,>x06,>x07,>x08,>x09,>x0a,>x0b,>x0c,>x0d,>x0e,>x0f
        	db	>x10,>x11,>x12,>x13,>x14,>x15,>x16,>x17,>x18,>x19,>x1a,>x1b,>x1c,>x1d,>x1e,>x1f
        	db	>x20,>x21,>x22,>x23,>x24,>x25,>x26,>x27,>x28,>x29,>x2a,>x2b,>x2c,>x2d,>x2e,>x2f
        	db	>x30,>x31,>x32,>x33,>x34,>x35,>x36,>x37,>x38,>x39,>x3a,>x3b,>x3c,>x3d,>x3e,>x3f
        	db	>x40,>x41,>x42,>x43,>x44,>x45,>x46,>x47,>x48

amptabl 	db	<am00,<am01,<am02,<am03,<am04,<am05,<am06,<am07,<am08,<am09,<am0a,<am0b,<am0c,<am0d,<am0e,<am0f
        	db	<am10,<am11,<am12,<am13,<am14,<am15,<am16,<am17,<am18,<am19,<am1a,<am1b,<am1c,<am1d,<am1e,<am1f
        	db	<am20,<am21,<am22,<am23,<am24
amptabh 	db	>am00,>am01,>am02,>am03,>am04,>am05,>am06,>am07,>am08,>am09,>am0a,>am0b,>am0c,>am0d,>am0e,>am0f
        	db	>am10,>am11,>am12,>am13,>am14,>am15,>am16,>am17,>am18,>am19,>am1a,>am1b,>am1c,>am1d,>am1e,>am1f
        	db	>am20,>am21,>am22,>am23,>am24

vibtabl 	db	<v01,<v02,<v03,<v04,<v05,<v06,<v07,<v08
vibtabh 	db	>v01,>v02,>v03,>v04,>v05,>v06,>v07,>v08

pultabl 	db	<p01,<p02,<p03,<p04,<p05,<p06,<p07,<p08
pultabh 	db	>p01,>p02,>p03,>p04,>p05,>p06,>p07,>p08

arptablo
		db 	<a00,<a01,<a02,<a03,<a04,<a05,<a06,<a07,<a08,<a09,<a0a,<a0b,<a0c,<a0d,<a0e,<a0f
        	db 	<a10,<a11,<a12,<a13,<a14,<a15,<a16,<a17,<a18,<a19,<a1a,<a1b,<a1c,<a1d,<a1e,<a1f
arptabhi
		db 	>a00,>a01,>a02,>a03,>a04,>a05,>a06,>a07,>a08,>a09,>a0a,>a0b,>a0c,>a0d,>a0e,>a0f
        	db 	>a10,>a11,>a12,>a13,>a14,>a15,>a16,>a17,>a18,>a19,>a1a,>a1b,>a1c,>a1d,>a1e,>a1f
arptabl2
		db 	<a20,<a21,<a22,<a23,<a24,<a25,<a26,<a27,<a28,<a29,<a2a,<a2b,<a2c,<a2d,<a2e,<a2f
        	db 	<a30,<a31,<a32,<a33,<a34,<a35,<a36,<a37,<a38,<a39,<a3a,<a3b,<a3c,<a3d,<a3e,<a3f
arptabh2
		db 	>a20,>a21,>a22,>a23,>a24,>a25,>a26,>a27,>a28,>a29,>a2a,>a2b,>a2c,>a2d,>a2e,>a2f
        	db 	>a30,>a31,>a32,>a33,>a34,>a35,>a36,>a37,>a38,>a39,>a3a,>a3b,>a3c,>a3d,>a3e,>a3f
;-----------------------------------------------------------------------------

seqtablo	db	<clearsfx		; $00 $xx
        	db	<dungeon1		; $01 $00 
        	db	<dungeon3		; $02 $02 
        	db	<dungeon4		; $03 $03
		db	<stairway1		; $04 $00 (+landings)
		db	<stairway3		; $05 $01 
		db	<stairway4		; $06 $03
		db	<corridor1		; $07 $00 (+attic)
		db	<corridor3		; $08 $01
		db	<corridor4		; $09 $03
		db	<canyon1		; $0a $00 (+cliff climb)
		db	<canyon3		; $0b $01
		db	<canyon4		; $0c $03
		db	<icylake1		; $0d $00 (+woods)
		db	<icylake3		; $0e $01
		db	<icylake4		; $0f $03
		db	<wolfboss1		; $10 $00
		db	<wolfboss3		; $11 $01
		db	<wolfboss4		; $12 $03
		db	<courtyard1		; $13 $00
		db	<courtyard3		; $14 $01
		db	<courtyard4		; $15 $03
		db	<library1		; $16 $00
		db	<library3		; $17 $01
		db	<library4		; $18 $03
		db	<lovejing1		; $19 $00
		db	<lovejing2		; $1a $01
		db	<lovejing3		; $1b $02
		db	<lovejing4		; $1c $03
		db	<hugegarg1		; $1d $00
		db	<hugegarg3		; $1e $02
		db	<hugegarg4		; $1f $03
		db	<lefou1			; $20 $00
		db	<lefou3			; $21 $02
		db	<lefou4			; $22 $03
		db	<stainglas1		; $23 $00
		db	<stainglas2		; $24 $01
		db	<stainglas3		; $25 $02
		db	<stainglas4		; $26 $03
		db	<waltz1			; $27 $00
		db	<waltz2			; $28 $01
		db	<waltz3			; $29 $02
		db	<waltz4			; $2a $03
		db	<beautyintro1		; $2b $00
		db	<beautyintro2		; $2c $01
		db	<beautyintro3		; $2d $02
		db	<beautyintro4		; $2e $03
		db	<gaston1		; $2f $00
		db	<gaston3		; $30 $02
		db	<gaston4		; $31 $03
		db	<transformation1	; $32 $00
		db	<transformation2	; $33 $01
		db	<transformation3	; $34 $02
		db	<transformation4	; $35 $03
		db	<panic1			; $36 $00
		db	<panic3			; $37 $02
		db	<panic4			; $38 $03
		db	<beastdies1		; $39 $00
		db	<beastdies3		; $3a $02
		db	<beastdies4		; $3b $03
		db	<prologue1		; $3c $00
		db	<prologue3		; $3d $02
		db	<prologue4		; $3e $03
		db	<balconies1		; $3f $00
		db	<balconies3		; $40 $02
		db	<balconies4		; $41 $03
		db	<endgame01		; $42 $00
		db	<endgame02		; $43 $01
		db	<endgame03		; $44 $02
		db	<endgame04		; $45 $03
		db	<endgame11		; $46 $00
		db	<endgame12		; $47 $01
		db	<endgame13		; $48 $02
		db	<endgame14		; $49 $03

		db	<beast_raws2		; $4a $01 ;Soundeffects
		db	<hit_enemy2		; $4b $01
		db	<beast_lands2		; $4c $01
		db	<land_on_platform2	; $4d $01
		db	<breath12		; $4e $01
		db	<breath22		; $4f $01
		db	<breath32		; $50 $01
		db	<swipewhoosh2		; $51 $01
		db	<pickup2		; $52 $01
		db	<bat_eyes2 		; $53 $01
		db	<candle_flame2		; $54 $01
		db	<batsqueek12		; $55 $01
		db	<batsqueek22		; $56 $01
		db	<ratsqueek12		; $57 $01
		db	<wingbuz2		; $59 $01
		db	<rocking2		; $5a $01
		db	<statue_hit_ground2	; $5b $01
		db	<chand_smash2		; $5c $01
		db	<chand_creak2		; $5d $01
		db	<head_rock2		; $5e $01
		db	<head_hit2		; $5f $01
		db	<metal_walk2		; $60 $01
		db	<swoop2			; $61 $01
		db	<sweep2			; $62 $01
		db	<blade_swipe2		; $63 $01
		db	<toad_splat2		; $64 $01
		db	<toad_plop2		; $65 $01
		db	<ice_splash2		; $66 $01
		db	<ice_splosh2		; $67 $01
		db	<ice_creak2		; $68 $01
		db	<ice_crack2		; $69 $01
		db	<ice_break2		; $6a $01
		db	<wolf_howl2		; $6b $01
		db	<wolf_grow2		; $6c $01
		db	<wolf_hurt2		; $6d $01
		db	<wolf_snar2		; $6e $01
		db	<wolf_hurt22		; $6f $01
		db	<wolf_death22		; $70 $01
		db	<snow_flurry2		; $71 $01
		db	<bird_tweet12		; $72 $01
		db	<bird_tweet22		; $73 $01
		db	<bird_flutter2		; $74 $01
		db	<snow_gather2		; $75 $01
		db	<ball_throw2		; $76 $01
		db	<book_push_out2		; $77 $01
		db	<book_jump_out2		; $78 $01
		db	<book_footstep2		; $79 $01
		db	<glass_break2		; $7a $01
		db	<gargoy_fly2		; $7b $01
		db	<gargoy_fire_ball2	; $7c $01
		db	<gargoy_hurt2		; $7d $01
		db	<gargoy_screech2	; $7e $01
		db	<ball_clang2		; $7f $01
		db	<ball_roll2		; $80 $01
		db	<wind_gust2		; $81 $01
		db	<thunder2		; $82 $01
		db	<lightning2		; $83 $01
		db	<electricity12		; $84 $01
		db	<electricity22		; $85 $01
		db	<lefou_gun2		; $86 $01
		db	<lefou_jump2		; $87 $01
		db	<lefou_hit2		; $88 $01
		db	<lefou_falls2		; $89 $01
		db	<gaston_falls2		; $8a $01
		db	<gaston_hit2		; $8b $01
		db	<arrow_fire2		; $8c $01
		db	<arrow_beast2		; $8d $01
		db	<gaston_tumble2		; $8e $01
		db	<gaston_punch2		; $8f $01
		db	<belle_gigle2		; $90 $01

seqtabhi	db	>clearsfx		; $00 $xx
        	db	>dungeon1		; $01 $00 
        	db	>dungeon3		; $02 $02 
        	db	>dungeon4		; $03 $03
		db	>stairway1		; $04 $00 (+landings)
		db	>stairway3		; $05 $01 
		db	>stairway4		; $06 $03
		db	>corridor1		; $07 $00 (+attic)
		db	>corridor3		; $08 $01
		db	>corridor4		; $09 $03
		db	>canyon1		; $0a $00 (+cliff climb)
		db	>canyon3		; $0b $01
		db	>canyon4		; $0c $03
		db	>icylake1		; $0d $00 (+woods)
		db	>icylake3		; $0e $01
		db	>icylake4		; $0f $03
		db	>wolfboss1		; $10 $00
		db	>wolfboss3		; $11 $01
		db	>wolfboss4		; $12 $03
		db	>courtyard1		; $13 $00
		db	>courtyard3		; $14 $01
		db	>courtyard4		; $15 $03
		db	>library1		; $16 $00
		db	>library3		; $17 $01
		db	>library4		; $18 $03
		db	>lovejing1		; $19 $00
		db	>lovejing2		; $1a $01
		db	>lovejing3		; $1b $02
		db	>lovejing4		; $1c $03
		db	>hugegarg1		; $1d $00
		db	>hugegarg3		; $1e $02
		db	>hugegarg4		; $1f $03
		db	>lefou1			; $20 $00
		db	>lefou3			; $21 $02
		db	>lefou4			; $22 $03
		db	>stainglas1		; $23 $00
		db	>stainglas2		; $24 $01
		db	>stainglas3		; $25 $02
		db	>stainglas4		; $26 $03
		db	>waltz1			; $27 $00
		db	>waltz2			; $28 $01
		db	>waltz3			; $29 $02
		db	>waltz4			; $2a $03
		db	>beautyintro1		; $2b $00
		db	>beautyintro2		; $2c $01
		db	>beautyintro3		; $2d $02
		db	>beautyintro4		; $2e $03
		db	>gaston1		; $2f $00
		db	>gaston3		; $30 $02
		db	>gaston4		; $31 $03
		db	>transformation1	; $32 $00
		db	>transformation2	; $33 $01
		db	>transformation3	; $34 $02
		db	>transformation4	; $35 $03
		db	>panic1			; $36 $00
		db	>panic3			; $37 $02
		db	>panic4			; $38 $03
		db	>beastdies1		; $39 $00
		db	>beastdies3		; $3a $02
		db	>beastdies4		; $3b $03
		db	>prologue1		; $3c $00
		db	>prologue3		; $3d $02
		db	>prologue4		; $3e $03
		db	>balconies1		; $3f $00
		db	>balconies3		; $40 $02
		db	>balconies4		; $41 $03
		db	>endgame01		; $42 $00
		db	>endgame02		; $43 $01
		db	>endgame03		; $44 $02
		db	>endgame04		; $45 $03
		db	>endgame11		; $46 $00
		db	>endgame12		; $47 $01
		db	>endgame13		; $48 $02
		db	>endgame14		; $49 $03

		db	>beast_raws2		; $4a $01 ;Soundeffects
		db	>hit_enemy2		; $4b $01
		db	>beast_lands2		; $4c $01
		db	>land_on_platform2	; $4d $01
		db	>breath12		; $4e $01
		db	>breath22		; $4f $01
		db	>breath32		; $50 $01
		db	>swipewhoosh2		; $51 $01
		db	>pickup2		; $52 $01
		db	>bat_eyes2 		; $53 $01
		db	>candle_flame2		; $54 $01
		db	>batsqueek12		; $55 $01
		db	>batsqueek22		; $56 $01
		db	>ratsqueek12		; $57 $01
		db	>wingbuz2		; $59 $01
		db	>rocking2		; $5a $01
		db	>statue_hit_ground2	; $5b $01
		db	>chand_smash2		; $5c $01
		db	>chand_creak2		; $5d $01
		db	>head_rock2		; $5e $01
		db	>head_hit2		; $5f $01
		db	>metal_walk2		; $60 $01
		db	>swoop2			; $61 $01
		db	>sweep2			; $62 $01
		db	>blade_swipe2		; $63 $01
		db	>toad_splat2		; $64 $01
		db	>toad_plop2		; $65 $01
		db	>ice_splash2		; $66 $01
		db	>ice_splosh2		; $67 $01
		db	>ice_creak2		; $68 $01
		db	>ice_crack2		; $69 $01
		db	>ice_break2		; $6a $01
		db	>wolf_howl2		; $6b $01
		db	>wolf_grow2		; $6c $01
		db	>wolf_hurt2		; $6d $01
		db	>wolf_snar2		; $6e $01
		db	>wolf_hurt22		; $6f $01
		db	>wolf_death22		; $70 $01
		db	>snow_flurry2		; $71 $01
		db	>bird_tweet12		; $72 $01
		db	>bird_tweet22		; $73 $01
		db	>bird_flutter2		; $74 $01
		db	>snow_gather2		; $75 $01
		db	>ball_throw2		; $76 $01
		db	>book_push_out2		; $77 $01
		db	>book_jump_out2		; $78 $01
		db	>book_footstep2		; $79 $01
		db	>glass_break2		; $7a $01
		db	>gargoy_fly2		; $7b $01
		db	>gargoy_fire_ball2	; $7c $01
		db	>gargoy_hurt2		; $7d $01
		db	>gargoy_screech2	; $7e $01
		db	>ball_clang2		; $7f $01
		db	>ball_roll2		; $80 $01
		db	>wind_gust2		; $81 $01
		db	>thunder2		; $82 $01
		db	>lightning2		; $83 $01
		db	>electricity12		; $84 $01
		db	>electricity22		; $85 $01
		db	>lefou_gun2		; $86 $01
		db	>lefou_jump2		; $87 $01
		db	>lefou_hit2		; $88 $01
		db	>lefou_falls2		; $89 $01
		db	>gaston_falls2		; $8a $01
		db	>gaston_hit2		; $8b $01
		db	>arrow_fire2		; $8c $01
		db	>arrow_beast2		; $8d $01
		db	>gaston_tumble2		; $8e $01
		db	>gaston_punch2		; $8f $01
		db	>belle_gigle2		; $90 $01

spdtablo	db	<sp00			; $00 $xx
		db	<sp01			; $01 $00
		db	<sp01			; $02 $02
		db	<sp01			; $03 $03
		db	<sp02			; $04 $00
		db	<sp02			; $05 $02
		db	<sp02			; $06 $03
		db	<sp01			; $07 $00
		db	<sp01			; $08 $02
		db	<sp01			; $09 $03
		db	<sp02			; $0a $00
		db	<sp02			; $0b $02
		db	<sp02			; $0c $03
		db	<sp02			; $0d $00
		db	<sp02			; $0e $02
		db	<sp02			; $0f $03
		db	<sp02			; $10 $00
		db	<sp02			; $11 $02
		db	<sp02			; $12 $03
		db	<sp01			; $13 $00
		db	<sp01			; $14 $02
		db	<sp01			; $15 $03
		db	<sp03			; $16 $00
		db	<sp03			; $17 $02
		db	<sp03			; $18 $03
		db	<sp04			; $19 $00
		db	<sp04			; $1a $01
		db	<sp04			; $1b $02
		db	<sp04			; $1c $03
		db	<sp01			; $1d $00
		db	<sp01			; $1e $02
		db	<sp01			; $1f $03
		db	<sp01			; $20 $00
		db	<sp01			; $21 $02
		db	<sp01			; $22 $03
		db	<sp01			; $23 $00
		db	<sp01			; $24 $02
		db	<sp01			; $25 $02
		db	<sp01			; $26 $03
		db	<sp05			; $27 $00
		db	<sp05			; $28 $01
		db	<sp05			; $29 $02
		db	<sp05			; $2a $03
		db	<sp03			; $2b $00
		db	<sp03			; $2c $01
		db	<sp03			; $2d $02
		db	<sp03			; $2e $03
		db	<sp01			; $2f $00
		db	<sp01			; $30 $02
		db	<sp01			; $31 $03
		db	<sp01			; $32 $00
		db	<sp01			; $33 $01
		db	<sp01			; $34 $02
		db	<sp01			; $35 $03
		db	<sp06			; $36 $00
		db	<sp06			; $37 $02
		db	<sp06			; $38 $03
		db	<sp03			; $39 $00
		db	<sp03			; $3a $02
		db	<sp03			; $3b $03
		db	<sp01			; $3c $00
		db	<sp01			; $3d $02
		db	<sp01			; $3e $03
		db	<sp02			; $3f $00
		db	<sp02			; $40 $02
		db	<sp02			; $41 $03
		db	<sp01			; $42 $00
		db	<sp01			; $43 $01
		db	<sp01			; $44 $02
		db	<sp01			; $45 $03
		db	<sp03			; $46 $00
		db	<sp03			; $47 $01
		db	<sp03			; $48 $02
		db	<sp03			; $49 $03

		db	<sp07			; $4a $01
		db	<sp07			; $4b $01
		db	<sp07			; $4c $01
		db	<sp07			; $4d $01
		db	<sp07			; $4e $01
		db	<sp07			; $4f $01
		db	<sp07			; $50 $01
		db	<sp07			; $51 $01
		db	<sp07			; $52 $01
		db	<sp07			; $53 $01
		db	<sp07			; $54 $01
		db	<sp07			; $55 $01
		db	<sp07			; $56 $01
		db	<sp07			; $57 $01
		db	<sp07			; $59 $01
		db	<sp07			; $5a $01
		db	<sp07			; $5b $01
		db	<sp07			; $5c $01
		db	<sp07			; $5d $01
		db	<sp07			; $5e $01
		db	<sp07			; $5f $01
		db	<sp07			; $60 $01
		db	<sp07			; $61 $01
		db	<sp07			; $62 $01
		db	<sp07			; $63 $01
		db	<sp07			; $64 $01
		db	<sp07			; $65 $01
		db	<sp07			; $66 $01
		db	<sp07			; $67 $01
		db	<sp07			; $68 $01
		db	<sp07			; $69 $01
		db	<sp07			; $6a $01
		db	<sp07			; $6b $01
		db	<sp07			; $6c $01
		db	<sp07			; $6d $01
		db	<sp07			; $6e $01
		db	<sp07			; $6f $01
		db	<sp07			; $70 $01
		db	<sp07			; $71 $01
		db	<sp05			; $72 $01
		db	<sp05			; $73 $01
		db	<sp07			; $74 $01
		db	<sp07			; $75 $01
		db	<sp07			; $76 $01
		db	<sp07			; $77 $01
		db	<sp07			; $78 $01
		db	<sp07			; $79 $01
		db	<sp07			; $7a $01
		db	<sp08			; $7b $01
		db	<sp07			; $7c $01
		db	<sp07			; $7d $01
		db	<sp07			; $7e $01
		db	<sp07			; $7f $01
		db	<sp08			; $80 $01
		db	<sp07			; $81 $01
		db	<sp07			; $82 $01
		db	<sp07			; $83 $01
		db	<sp08			; $84 $01
		db	<sp07			; $85 $01
		db	<sp07			; $86 $01
		db	<sp07			; $87 $01
		db	<sp07			; $88 $01
		db	<sp07			; $89 $01
		db	<sp07			; $8a $01
		db	<sp07			; $8b $01
		db	<sp07			; $8c $01
		db	<sp07			; $8d $01
		db	<sp07			; $8e $01
		db	<sp07			; $8f $01
		db	<sp07			; $90 $01

spdtabhi	db	>sp00			; $00 $xx
		db	>sp01			; $01 $00
		db	>sp01			; $02 $02
		db	>sp01			; $03 $03
		db	>sp02			; $04 $00
		db	>sp02			; $05 $02
		db	>sp02			; $06 $03
		db	>sp01			; $07 $00
		db	>sp01			; $08 $02
		db	>sp01			; $09 $03
		db	>sp02			; $0a $00
		db	>sp02			; $0b $02
		db	>sp02			; $0c $03
		db	>sp02			; $0d $00
		db	>sp02			; $0e $02
		db	>sp02			; $0f $03
		db	>sp02			; $10 $00
		db	>sp02			; $11 $02
		db	>sp02			; $12 $03
		db	>sp01			; $13 $00
		db	>sp01			; $14 $02
		db	>sp01			; $15 $03
		db	>sp03			; $16 $00
		db	>sp03			; $17 $02
		db	>sp03			; $18 $03
		db	>sp04			; $19 $00
		db	>sp04			; $1a $01
		db	>sp04			; $1b $02
		db	>sp04			; $1c $03
		db	>sp01			; $1d $00
		db	>sp01			; $1e $02
		db	>sp01			; $1f $03
		db	>sp01			; $20 $00
		db	>sp01			; $21 $02
		db	>sp01			; $22 $03
		db	>sp01			; $23 $00
		db	>sp01			; $24 $02
		db	>sp01			; $25 $02
		db	>sp01			; $26 $03
		db	>sp05			; $27 $00
		db	>sp05			; $28 $01
		db	>sp05			; $29 $02
		db	>sp05			; $2a $03
		db	>sp03			; $2b $00
		db	>sp03			; $2c $01
		db	>sp03			; $2d $02
		db	>sp03			; $2e $03
		db	>sp01			; $2f $00
		db	>sp01			; $30 $02
		db	>sp01			; $31 $03
		db	>sp01			; $32 $00
		db	>sp01			; $33 $01
		db	>sp01			; $34 $02
		db	>sp01			; $35 $03
		db	>sp06			; $36 $00
		db	>sp06			; $37 $02
		db	>sp06			; $38 $03
		db	>sp03			; $39 $00
		db	>sp03			; $3a $02
		db	>sp03			; $3b $03
		db	>sp01			; $3c $00
		db	>sp01			; $3d $02
		db	>sp01			; $3e $03
		db	>sp02			; $3f $00
		db	>sp02			; $40 $02
		db	>sp02			; $41 $03
		db	>sp01			; $42 $00
		db	>sp01			; $43 $01
		db	>sp01			; $44 $02
		db	>sp01			; $45 $03
		db	>sp03			; $46 $00
		db	>sp03			; $47 $01
		db	>sp03			; $48 $02
		db	>sp03			; $49 $03

		db	>sp07			; $4a $01
		db	>sp07			; $4b $01
		db	>sp07			; $4c $01
		db	>sp07			; $4d $01
		db	>sp07			; $4e $01
		db	>sp07			; $4f $01
		db	>sp07			; $50 $01
		db	>sp07			; $51 $01
		db	>sp07			; $52 $01
		db	>sp07			; $53 $01
		db	>sp07			; $54 $01
		db	>sp07			; $55 $01
		db	>sp07			; $56 $01
		db	>sp07			; $57 $01
		db	>sp07			; $59 $01
		db	>sp07			; $5a $01
		db	>sp07			; $5b $01
		db	>sp07			; $5c $01
		db	>sp07			; $5d $01
		db	>sp07			; $5e $01
		db	>sp07			; $5f $01
		db	>sp07			; $60 $01
		db	>sp07			; $61 $01
		db	>sp07			; $62 $01
		db	>sp07			; $63 $01
		db	>sp07			; $64 $01
		db	>sp07			; $65 $01
		db	>sp07			; $66 $01
		db	>sp07			; $67 $01
		db	>sp07			; $68 $01
		db	>sp07			; $69 $01
		db	>sp07			; $6a $01
		db	>sp07			; $6b $01
		db	>sp07			; $6c $01
		db	>sp07			; $6d $01
		db	>sp07			; $6e $01
		db	>sp07			; $6f $01
		db	>sp07			; $70 $01
		db	>sp07			; $71 $01
		db	>sp05			; $72 $01
		db	>sp05			; $73 $01
		db	>sp07			; $74 $01
		db	>sp07			; $75 $01
		db	>sp07			; $76 $01
		db	>sp07			; $77 $01
		db	>sp07			; $78 $01
		db	>sp07			; $79 $01
		db	>sp07			; $7a $01
		db	>sp08			; $7b $01
		db	>sp07			; $7c $01
		db	>sp07			; $7d $01
		db	>sp07			; $7e $01
		db	>sp07			; $7f $01
		db	>sp08			; $80 $01
		db	>sp07			; $81 $01
		db	>sp07			; $82 $01
		db	>sp07			; $83 $01
		db	>sp08			; $84 $01
		db	>sp07			; $85 $01
		db	>sp07			; $86 $01
		db	>sp07			; $87 $01
		db	>sp07			; $88 $01
		db	>sp07			; $89 $01
		db	>sp07			; $8a $01
		db	>sp07			; $8b $01
		db	>sp07			; $8c $01
		db	>sp07			; $8d $01
		db	>sp07			; $8e $01
		db	>sp07			; $8f $01
		db	>sp07			; $90 $01

sp00		db	$00,$ff
sp01		db	$02,$02,$ff  		;7
sp02		db	$02,$01,$ff  		;6
sp03		db	$02,$03,$02,$02,$ff  	;8
sp04		db	$03,$03,$03,$02,$ff  	;9
sp05		db	$01,$02,$01,$01,$ff  	;5
sp06		db	$00,$00,$ff  		;1
sp07		db	$01,$01,$01,$00,$ff  	;4
sp08		db	$7f,$7f,$ff  		;128
;-----------------------------------------------------------------------------

am00	db	15,14,13,12,9,8,7,$fe
am01	db	10,8,6,4,3,3,2,2,1,$fe
am02	db	15,12,9,8,7,6,6,5,5,4,4,4,3,3,3,2,2,2,1,$fe
am03	db	6,5,4,$fe
am04	db	15,13,11,9,8,7,7,6,6,5,5,4,4,3,3,2,2,2,1,$fe
am05	db	6,4,2,1,$fe	
am06	db	10,9,8,7,$fe
am07	db	15,14,13,12,11,9,8,7,6,$fe
am08 	db	15,13,11,9,8,7,$fe
am09	db	11,9,8,7,6,$fe
am0a	db	4,4,3,$fe
am0b	db	12,11,10,9,8,8,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,5,$fe
am0c	db	9,8,7,7,7,6,$fe
am0d	db	7,6,6,6,5,5,5,5,4,4,4,4,3,$fe
am0e	db	7,7,6,6,6,6,5,5,5,5,5,5,5,4,4,4,4,4,4,$fe
am0f	db	1,$fe

am10
	db	6,12,15,15,15,15,15,15,15,15,15,15,15,15,15,15,14,14,13,13
	db	12,12,11,11,10,10,9,9,8,8,7,7,6,6,6,5,5,5,4,4,4,3,3,3,3
	db	2,2,2,2,1,$fe
am11
	db	8,15,14,13,13,13,12,12,12,11,11,11,10,10,10,9,9,9,8,8,8
	db	7,7,7,6,6,6,5,5,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,2,1,$fe
am12
	db	15,14,13,12,11,10,10,9,9,8,8,7,7,6,6,6,5,5,5,4,4,4,3,3,3
	db	2,2,2,2,1,$fe
am13
	db	12,11,10,9,8,7,6,5,4,4,3,3,2,2,1,$fe
am14
	db	10,5,$ff,$00
am15	
	db	6,8,10,11,12,12,11,9,7,5,4,3,2,1,$fe
am16	
	db	15,15,15,14,14,14,13,13,13,12,12,12,12,11,11,11,11
	db	10,10,10,10,9,9,9,9,8,8,8,8,8,7,7,7,7,7,6,6,6,6,6
	db	5,5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2,1,$fe
am17	
	db	12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,4,3,3,3,2,2,2,1,$fe
am18	
	db	8,$fe
am19	
	db	2,4,6,8,10,12,14,15,14,12,10,8,7,6,5,4,4,3,3,2,2,1,$fe
am1a	
	db	15,0,0,0,15,0,0,15,0,0,15,0,15,0,0,15,0,15,0,15,0,0,0,15,0,15
	db	0,0,15,0,15,0,15,0,0,15,0,0,15,0,15,0,0,15,0,15,0
	db	$fe
am1b	
	db	4,6,8,10,12,11,12,11,10,8,9,6,7,8,5,6,7,6,5,3,4,2,1,3,2,1,2,0
	db	1,0,1,1,0,1,0,$fe
am1c	
	db	1,3,5,7,9,10,11,12,12,11,11,10,10,10,9,9,9,9,8,9,8,8,8,8,8,7,8
	db	7,8,7,7,7,7,7,7,6,7,6,7,6,7,6,6,6,6,6,6,5,6,5,6,5,6,5,5,5,5
	db	5,5,4,5,4,5,4,5,4,4,4,4,4,4,3,4,3,4,3,4,3,3,3,3,3,3,2,3,2,3,2
	db	3,2,2,2,2,2,2,1,2,1,2,1,2,1,$fe
am1d
	db	1,4,8,12,11,10,9,8,7,6,5,4,3,2,1,$fe
am1e	
	db	3,$fe
am1f	
	db	15,0,10,0,5,0,10,$ff,0
am20
	db	3,1,10,2,15,3,13,2,4,1,7,3,13,2,10,1,2,0,6,2,15,4,14,2,12,1
	db	11,2,5,3,2,1,5,2,7,3,13,4,15,5,12,4,15,5,4,1,15,2,14,3,8,4
	db	4,5,15,3,8,2,13,1,15,2,13,3,12,2,14,1,4,2,6,1,10,2,15,3,13,4
	db	11,3,6,2,3,1,11,4,3,1,7,2,15,3,10,2,12,1,$ff,$00
am21
	db	15,6,14,5,13,4,12,5,11,6,10,5,9,4,8,5,7,3,7,2,6,1,6,2,5,1
	db	5,2,4,3,4,2,3,1,3,0,2,1,2,0,1,0,1,0,1,0,$fe
am22
	db	2,4,6,8,10,12,11,10,9,8,8,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4
	db	3,4,3,3,3,3,2,3,2,2,2,2,1,2,1,$fe
am23
	db	1,2,3,4,5,6,7,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,$fe
am24
	db	2,4,6,4,2,$fe
;-----------------------------------------------------------------------------
v01		
	db	$02,$01,$01,$fe,$ff,$ff
	db	$fe,$ff,$ff,$02,$01,$01
	db	$80
v02
	db	$04,$04,$04,$fc,$fc,$fc
	db	$fc,$fc,$fc,$04,$04,$04
	db	$80	
v03	     
	db	$01,$02,$ff,$fe,$ff,$fe,$01,$02
	db	$80
v04	     
	db	$01,$ff
	db	$80
v05
	db	$ff,$ff,$ff,$ff,$ff,$01,$01,$01,$01,$01
	db	$80
v06
v07
v08
;-----------------------------------------------------------------------------
p01		
	db	$80,$80,$40,$40,$00,$00,$40,$40
	db	$80,$80,$80,$80,$80,$80,$80,$80,$80
	db	$40,$40,$40,$40,$40,$40,$40,$40,$40
	db	$ff,$08
p02	
	db	$00,$00,$00,$40,$40,$40
	db	$80,$80,$80,$40,$40,$40
	db	$ff,$00
p03		
	db	$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40
	db	$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80
	db	$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0
	db	$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80
	db	$ff,$00
p04	
	db	$80,$80,$80,$40,$40,$40,$80,$80,$80,$80,$80,$80
	db	$40,$40,$40,$40,$40,$40,$00,$00,$00,$00,$00,$00
	db	$40,$40,$40,$ff,$03	
p05
	db	$00,$00,$80,$80,$40,$40,$c0,$c0,$ff,$00
p06          
	db	$80,$40,$ff,$00
p07
	db	$00,$80,$40,$c0,$ff,$00
p08
	db	$00,$00,$c0,$00,$c0,$c0,$00,$c0
	db	$ff,$00
;-----------------------------------------------------------------------------
a00	db	$07,$07,$03,$03,$00,$00,$ff,$00
a01	db	$07,$07,$04,$04,$00,$00,$ff,$00
a02	db	$08,$08,$05,$05,$00,$00,$ff,$00
a03	db	$09,$09,$05,$05,$00,$00,$ff,$00
a04	db	$08,$08,$03,$03,$00,$00,$ff,$00
a05	db	$09,$09,$04,$04,$00,$00,$ff,$00
a06	db	$07,$07,$05,$05,$00,$00,$ff,$00
a07	db	$0a,$0a,$05,$05,$00,$00,$ff,$00

a08	db	$0c,$00,$0c,$0c,$0c,$00,$fe
a09	db	$24,$24,$18,$18,$0c,$0c,$00,$00,$0c,$fe
a0a	db	$0c,$18,$00,$fe

a0b	db	$08,$08,$04,$04,$00,$00,$ff,$00
a0c	db	$0b,$0b,$04,$04,$00,$00,$ff,$00
a0d	db	$06,$06,$03,$03,$00,$00,$ff,$00
a0e	db	$0b,$0b,$05,$05,$00,$00,$ff,$00
a0f	db	$07,$07,$02,$02,$00,$00,$ff,$00

a10	db	$9e,$99,$96,$94,$00,$fe
a11	db	$a5,$a2,$9f,$9d,$00,$fe
a12	db	$9e,$9b,$98,$95,$00,$fe
a13	db	$a5,$a2,$9f,$00,$fe
a14	db	$80,$00,$fe

a15	db	$08,$08,$02,$02,$00,$00,$ff,$00

a16	db	$9e,$9b,$98,$95,$0c,$0c,$00,$fe
a17	db	$0c,$0c,$0c,$0c,$0c,$00,$fe
a18	db	$0c,$0c,$0c,$0c,$0c,$0c,$0c,$0c,$0c,$0c
	db	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$00
a19	db 	$05,$05,$05,$00,$00,$00,$ff,$00
a1a	db	$9e,$99,$96,$94,$0c,$0c,$0c,$0c,$00,$fe

a1b	db	$0a,$0a,$03,$03,$00,$00,$ff,$00

a1c	db	$07,$07,$07,$02,$02,$02,$00,$00,$00,$ff,$00
a1d	db	$18,$18,$16,$13,$13,$11,$0f,$0c,$0c,10,7,7,5,3,0,$fe
a1e	db	12,12,0,0,12,12,0,0,0,0,0,0,12,12,12,$ff,4

a1f	db	$06,$06,$04,$04,$00,$00,$ff,$00
;-----------------------------------------------------------------------------
a20
	db	$06,$02,$04,$01,$05,$02,$04,$ff,$00
a21
	db	20,17,24,21,28,25,26,23,24,21,22,19,20,17,18,15
	db	16,13,14,11,12,10,11,8,10,7,9,6,8,5,7,4
	db	6,3,5,2,3,1,2,0,1,0,$48,$3c,$45,$40,$42,$3e
	db	$47,$40,$43,$3f,$44,$3c,$41,$3b
	db	$44,$3e,$43,$3f,$41,$38,$42,$ff,$2b
a22
	db	20,16,12,8,10,6,9,5,8,4,7,3,6,2,5,2,4,1,3,1,2,0,1,0,$ff,$0e
a23
	db	12,8,5,7,4,6,3,5,2,4,2,3,1,2,1,0,$ff,$0a
a24
	db	2,0,1,2,0,1,0,2,0,1,2,1,0,2,0,2,1,0,2,1,$ff,$00
a25
	db	0,3,6,9,12,9,10,7,8,5,6,3,4,1,3,0,2,0,1,0,$fe
a26
	db	$18,$18,$13,$18,$13,$13,$10,$13,$10,$10,$0c,$10
	db	$0c,$0c,$0f,$0c,$0f,$0f,$13,$0f,$13,$13,$16,$13
	db	$16,$16,$1b,$16,$1b,$1b,$1f,$1b,$16,$13,$0f,$0a
	db	$07,$03,$00,$ff,$1e
a27
	db	1,0,$fe
a28
	db	3,0,4,1,5,2,6,3,7,4,8,5,7,4,6,3,5,2,4,1,3,0,$fe
a29
	db	31,26,28,23,25,20,22,17,19,14,16,11,14,9,12,7,11,6,10,5,9,4,8,3,7,3,6,2,5,2,4,1,3,1
	db	2,0,1,0,$ff,$0e
a2a
	db	6,5,4,3,2,2,1,1,0,0,1,1,2,2,3,4,5,6,$fe
a2b
	db	0,5,10,15,20,25,18,21,14,17,11,13,7,8,4,7,3,6,2,5,2
	db	4,1,3,1,2,1,0,1,0
	db	$fe
a2c
	db	20,12,4,0,8,1,9,$ff,$03
a2d
	db	60,30,55,35,50,40,23,58,25,5,56,45,24,34,46,56,34
	db	23,43,51,24,66,34,52,25,12,64,51,23,59,32,46,51,26,68,13
	db	62,34,61,23,34,17,62,16,21,46,34,24,44,62,13,47,62,34,18
	db	23,57,52,34,66,12,36,18,46,12,43,67,23,47,$ff,$00
a2e
	db	5,0,2,4,3,1,5,4,0,2,4,0,3,2,0,3,4,5,0,1,3,2,4,0,3,2,1,0,3
	db	4,1,3,5,4,1,2,4,0,3,1,0,2,4,3,$ff,$00
a2f
	db	0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,$fe
a30
	db	16,3,14,2,18,6,19,1,20,3,16,5,18,2,15,8,11,3,17,6,13,2,15
	db	7,16,3,12,5,17,$ff,$00
a31
	db	0,1,2,3,5,8,11,14,18,20,21,21,21,21,21,20,21,20,21,20,20,20,20
	db	19,20,19,20,19,19,19,19,18,19,18,19,18,18,18,18,17,18,17,18
	db	17,17,17,17,16,18,16,18,16,16,16,16,15,16,15,16,15,$fe
a32
	db	0,2,5,9,14,19,18,17,16,15,14,15,13,14,12,14,11,13,10,13,9
	db	12,8,12,7,12,6,11,6,10,6,9,5,9,4,9,3,9,2,8,2,7,2,6,2,5,2,4,2
	db	3,2,3,1,3,0,2,0,1,0,$ff,$20
a33
	db	8,5,10,7,12,9,11,8,9,6,7,4,5,2,3,0,2,0,1,0,$fe
a34
	db	8,4,7,3,6,2,5,1,4,0,5,1,6,2,7,3,8,4,9,5,10,6,11,7,12,8,13,9
	db	12,8,11,7,10,6,9,5,$ff,$00
a35
	db	4,5,6,7,6,5,4,3,2,1,2,3,$ff,$00
a36
	db	18,0,$ff,$00
a37
	db	18,14,16,12,14,10,12,8,11,7,10,6,9,5,8,4,3,5,3,6,4,1,3,2,4
	db	2,3,1,2,3,1,2,0,1,3,2,1,0,2,5,7,5,3,7,8,6,7,5,6,3,4,5,3
	db	4,2,3,1,$ff,$20
a38
	db	40,35,30,25,20,15,10,5,2,0,$fe
a39
	db	4,2,3,1,2,0,6,4,8,6,3,1,5,3,$ff,$00
a3a
	db	24,20,16,12,8,6,5,4,3,2,2,1,2,1,1,0,1,0,0,1,0,1,1,1,2,1,2
	db	2,3,2,3,3,4,4,5,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19
	db	20,21,22,23,24,25,26,27,28,29,30,$ff,$28
a3b
	db	20,16,12,8,5,3,2,1,0,1,2,3,5,7,10,13,16,17,18,19,20,21,22,$fe
a3c
	db	0,1,2,3,4,5,6,7,8,9,10,11,12,11,10,9,8,7,6,5,6,7,8,9,10,9,8,7
	db	6,5,4,3,4,5,6,5,4,3,2,1,2,3,2,1,0,$fe
a3d
	db	0,1,2,3,4,5,8,24,20,16,13,11,9,8,7,6,5,4,3,2,1,0,$fe
a3e
a3f
;-----------------------------------------------------------------------------
soundfix

		;	bgp,a/v,vd/p,am

;leader  (1)
		db	$80,$41,$41,$00	;c0   
;BD+Bass (3)
		db	$80,$42,$40,$00	;c1
;Sis_v1	 (4)
		db	$00,$00,$00,$01	;c2
;Sis_v2  (4)
		db	$00,$00,$00,$02	;c3
;leade-1 (1)
		db	$80,$41,$41,$03	;c4   
;Xylo    (1)
		db	$80,$43,$20,$04	;c5   
;Sis_v0	 (4)
		db	$00,$00,$00,$05	;c6
;leader2 (1)
		db	$08,$01,$42,$06 ;c7
;leader3 (1)
		db	$88,$41,$41,$07	;c8   
;leade-1 (1)
		db	$88,$41,$41,$03	;c9   
;Xylo-1  (1)
		db	$80,$43,$20,$03	;ca   
;leaderF (1)
		db	$88,$41,$41,$08	;cb   
;leadSMT (1)
		db	$88,$41,$43,$09	;cc   
;leadSM-1(1)
		db	$88,$41,$43,$0a	;cd   
;leadSHP (1)
		db	$00,$41,$42,$09	;ce   
;leadSH-1(1)
		db	$00,$41,$42,$0a	;cf   
;leadAR2 (1)
		db	$88,$41,$43,$06 ;d0
;LEADNRMFADE
		db	$88,$41,$43,$0b ;d1
;LEADFLUTE
		db	$80,$41,$24,$0c ;d2
;LEADTINGEL
		db	$80,$41,$20,$0d ;d3
;ARPMIDVOL
		db	$88,$41,$43,$0e ;d4
;ARPMIDVOL-DTN-v
		db	$88,$00,$04,$0a ;d5
;LEADTINGEL-3
		db	$80,$41,$20,$0f ;d6
;-----------------------------------------------------------------------------
soundfx2

;RAW	   (2)
		db	$00,$41,$05,$10	;c0   
;*GENERAL1 (2)
		db	$80,$41,$05,$11	;c1   
;*FADEL_8  (2)
		db	$80,$41,$00,$12	;c2   
;*FADEL_4  (2)
		db	$80,$41,$00,$13	;c3   
;*VOLU_10  (2)
		db	$80,$41,$06,$14	;c4   
;*FADUD_10 (2)
		db	$80,$41,$00,$15	;c5   
;JINGLEARP (2)
		db	$80,$40,$01,$16	;c6   
;*FADED_12 (2)
		db	$80,$44,$10,$17	;c7   
;*VOLU_8   (2)
		db	$80,$00,$00,$18	;c8   
;*PULSE_00 (2)
		db	$00,$40,$00,$18	;c9   
;*RAWFADUD (2)
		db	$00,$40,$01,$19	;ca   
;*LOUDRAWFD(2)
		db	$00,$40,$05,$16	;cb   
;*SHARPFAD8(2)
		db	$00,$41,$00,$12	;cc   
;*PULS0_RAW(2)
		db	$00,$40,$07,$18	;cd   
;*VOL8PUL8A(2)
		db	$80,$40,$00,$18	;ce   
;*ICECRACK (2)
		db	$00,$05,$00,$1a	;cf   
;*UPDOWNLD (2)
		db	$80,$40,$00,$10	;d0   
;*SLUSHPUP (2)
		db	$00,$40,$07,$1b	;d1   
;*SLOWDOWN (2)
		db	$80,$40,$00,$1c	;d2   
;*><QUIKSH (2)
		db	$00,$40,$00,$1d	;d3   
;AMPLITUD3 (2)
		db	$80,$40,$00,$1e	;d4   
;VOLUPDORAW(2)
		db	$00,$40,$05,$1f	;d5   
;PUL8VOLDAR(2)
		db	$80,$40,$00,$16	;d6   
;ELECTRIC1 (2)
		db	$00,$40,$08,$20	;d7   
;PUL8FSTFAD(2)
		db	$80,$40,$00,$12	;d8   
;SHARPFADE (2)
		db	$00,$00,$00,$1c	;d9   
;ARROWING  (2)
		db	$80,$00,$00,$21	;da   
;TUMBLE    (2)
		db	$80,$40,$00,$22	;db   
;GASTONPNCH(2)
		db	$80,$40,$00,$23	;dc   
;GIGLE     (2)
		db	$40,$00,$00,$24	;dd   
;-----------------------------------------------------------------------------
beast_raws2
	db	$83,$01,$00
	db	$fe
x01
	db	$c0,$81,$60,0,1,3,5,7,10,11,10,12,11,13,12,10,9,8,7
	db	6,5,4,3,4,2,3,1,2,0,1,0
	db	$ff

hit_enemy2
	db	$83,$02,$00
	db	$fe
x02
	db	$c1,$a4,$61,0
	db	$ff
		
beast_lands2
	db	$83,$03,$00
	db	$fe
x03
	db	$c2,$90,$62,0
	db	$ff
		
land_on_platform2
	db	$83,$04,$00
	db	$fe
x04
	db	$c3,$88,$63,0
	db	$ff

breath12
	db	$83,$05,$00
	db	$fe 
breath22		
	db	$88,$05,$00
	db	$fe 
breath32	  
	db	$8d,$05,$00
	db	$fe 
x05
	db	$c4,$81,$64,0,1,3,4,5,7,8,9,9,10,11,11,11,12,12,13
	db	$ff

swipewhoosh2
	db	$88,$06,$00
	db	$fe		
x06
	db	$c5,$89,$65,$00
	db	$ff

pickup2
	db	$83,$07,$00
	db	$fe			
x07
	db	$c6,$a0,$66,$15
	db	$ff

bat_eyes2
	db	$cb,$08,$00
	db	$fe
x08
	db	$c7,$8d,$67,$00
	db	$ff
candle_flame2
	db	$86,$09,$00
	db	$fe
x09
	db	$c0,$81,$60,$00,$08,$10,$18,$14,$10,$0c
	db	$ff

batsqueek12
	db	$83,$0a,$00
	db	$fe
batsqueek22	
	db	$86,$0a,$00
	db	$fe
ratsqueek12	
	db	$88,$0a,$00
	db	$fe
x0a
	db	$c8,$81,$48,$4a,$4b,$4a,$49,$48
	db	$ff
wingbuz2
	db	$84,$0b
	db	$ff,$00	
x0b
	db	$c9,$81,$64,12,13,14,13,12,11,10,11,10,11,12,10,9,8
	db	7,6,5,6,7,8,7,8,9,8,7,8,7,8,9,10,11,10,9,8,9,8,7,6
	db	5,4,5,6,7,6,5,6,7,8,9,8,9,10,11,10,12,13,14,15,14,13
	db	12,13,14,13,12,11,10,9,8,9,8,7,6,7,8,7,6,5,4,5,6,7,6
	db	7,6,5,6,7,8,9,10,11,12,13,14,13,12
	db	$ff
rocking2
	db	$83,$0c,$00
	db	$fe
x0c
	db	$ca,$8b,$68,$00
	db	$ff	
statue_hit_ground2
	db	$83,$0d,$00
	db	$fe	
x0d
	db	$cb,$96,$69,$00
	db	$ff
chand_smash2
	db	$8b,$0d,$00
	db	$fe		
chand_creak2
	db	$81,$0e,$00
	db	$fe
x0e
	db	$ca,$8a,$6a,$4e
	db	$ff		
head_rock2
	db	$89,$0c,$00
	db	$fe		
head_hit2
	db	$83,$0f,$00
	db	$fe
x0f
	db	$cc,$8f,$6b,$08
	db	$ff		
metal_walk2
	db	$86,$10,$00
	db	$fe		
x10
	db	$c3,$86,$6c,$09
	db	$ff
swoop2	
	db	$90,$06,$00
	db	$fe		
sweep2		   
	db	$97,$06,$00
	db	$fe	
blade_swipe2
	db	$83,$11,$00
	db	$fe
x11
	db	$c0,$81,$60,$00,$02,$06,$09,$0b,$0c
	db	$ff
toad_splat2
	db	$95,$12,$00
	db	$fe
x12
	db	$c7,$90,$6e,$00
	db	$ff
toad_plop2
	db	$83,$13,$00
	db	$fe
x13		
	db	$ce,$83,$6f,$24
	db	$ff
ice_splash2    
	db	$b7,$12,$00
	db	$fe	
ice_splosh2
	db	$88,$14,$00
	db	$fe  
x14
	db	$cd,$81,0,1,3,6,8,9,10
	db	$ff
ice_creak2
	db	$cf,$15,$00
	db	$fe
x15
	db	$cf,$98,$01
	db	$ff
ice_crack2     
	db	$89,$15,$00
	db	$fe
ice_break2	
	db	$83,$16,$00
	db	$fe	
x16
	db	$cc,$90,$70,$04
	db	$ff		

wolf_howl2
	db	$a0,$17,$00
	db	$fe
x17
	db	$d0,$9d,$71,$00
	db	$ff		
wolf_grow2	   
	db	$83,$18,$00
	db	$fe
x18
	db	$cb,$81,$67,$00,$01,$00,$02,$04,$02,$04,$06,$04,$06,$08,$06
	db	$04,$05,$04,$03,$04,$03,$02,$03,$02,$01,$02,$01,$00,$01,$00
	db	$ff
wolf_hurt2	 
	db	$92,$19,$00
	db	$fe
x19
	db	$cb,$81,$67,$00,$02,$05,$09,$0d,$10,$0f,$0e,$0d,$0b,$09,$07
	db	$05,$04,$03,$02,$01,$00
	db	$ff	
wolf_snar2
	db	$89,$1a,$00
	db	$fe
x1a
	db	$cb,$81,$67,$04,$00,$08,$04,$0c,$08,$0a,$06,$09,$05,$08,$04
	db	$07,$03,$06,$02,$05,$01,$04,$00
	db	$ff		
wolf_hurt22    
	db	$8c,$19,$00
	db	$fe		
wolf_death22
	db	$8f,$1b,$00
	db	$fe
x1b
	db	$cb,$81,$67,0,2,4,6,10,13,15,16,17,18,19,20,20,19,18,17,18,17
	db	16,15,14,15,16,15,14,13,14,15,14,13,12,11,10,11,10,9,8,7
	db	8,9,8,7,6,5,6,5,4,5,6,5,4,3,2,3,2,1,0
	db	$ff
snow_flurry2
	db	$83,$1c,$00
	db	$fe
x1c
	db	$d1,$90,$6e,$00
	db	$ff		
bird_tweet12
	db	$87,$1d,$00
	db	$fe
x1d
	db	$c8,$84,$fd,$01,$46,$49
	db	$82,$fd,$01,$41,$44
	db	$82,$fd,$01,$41,$44
	db	$82,$fd,$01,$41,$44
	db	$ff		
bird_tweet22		
	db	$85,$1e,$00
	db	$fe
x1e
	db	$c8,$82,$fd,$04,$3e,$41
	db	$82,$fd,$04,$3e,$41
	db	$82,$fd,$04,$3f,$3c
	db	$83,$fd,$01,$46,$49
	db	$ff		
bird_flutter2
	db	$87,$1f
	db	$ff,$00
x1f
	db	$d1
	db	$82,$6e,$0c,$e2,$0b,$e2,$0c,$e2,$0d,$e2,$0e,$e2
	db	$0d,$e2,$0c,$e2,$0d,$e2,$0c,$e2,$0b,$e2,$0a,$e2
	db	$0b,$e2,$0c,$e2,$0b,$e2,$0c,$e2,$0d,$e2,$0b,$e2
	db	$ff
snow_gather2	  
	db	$90,$20,$00
	db	$fe
x20
	db	$d1,$87,$6e,$00
	db	$ff
ball_throw2
	db	$92,$21,$00
	db	$fe
x21
	db	$d2,$b6,$72,$00
	db	$ff
book_push_out2
	db	$9b,$22,$00
	db	$fe
x22
	db	$d3,$88,$73,$00
	db	$ff		
book_jump_out2	 
	db	$a3,$23,$00
	db	$fe
x23
	db	$d3,$88,$68,$00
	db	$ff	
book_footstep2
	db	$8b,$24,$00
	db	$fe    
x24
	db	$c7,$84,$63,0
	db	$ff 
glass_break2		
	db	$8f,$25,$00
	db	$fe
x25
	db	$cc,$98,$6d,$00
	db	$ff		
gargoy_fly2	    
	db	$c3,$26
	db	$ff,$00
x26
	db	$d4,$bf,$74,$00
	db	$ff	
gargoy_fire_ball2
	db	$83,$21,$00
	db	$fe	
gargoy_hurt2	   
	db	$8a,$02,$00
	db	$fe	
gargoy_screech2
	db	$a7,$27,$00
	db	$fe
x27
	db	$d5,$97,$75,$00
	db	$ff		
ball_clang2
	db	$b2,$28,$00
	db	$fe
x28
	db	$d6,$98,$76,$00
	db	$ff
ball_roll2
	db	$83,$29
	db	$ff,$00
x29
	db	$ce,$bf,$74,$00
	db	$ff		
wind_gust2
	db	$8f,$2a
	db	$ff,$00
x2a
	db	$d4,$81,$67,8,9,10,11,12,11,10,9,8,7,6,5,5,4,4,3,3,2,2,3,3,4
	db	4,5,6,7,6,5,6,7,8,9,10,11,12,12,13,13,12,12,11,10,9,8,7,7,6,6
	db	5,5,4,4,3,3,4,4,5,5,6,7,8,9,8,7,6,7,8,7,6,5,4,3,3,3,2,2,1,1
	db	2,2,3,3,4,4,5,5,6,6,7,7,8
	db	$ff		
thunder2 
	db	$83,$2b,$00
	db	$fe	   
x2b
	db	$cb,$a0,$77,$00
	db	$ff	
lightning2 
	db	$83,$2c,$00
	db	$fe
x2c
	db	$cb,$85,$78,$03
	db	$ff		
electricity12
	db	$83,$2d
	db	$ff,$00
x2d
	db	$d7,$bf,$79,$00
	db	$ff		
electricity22	 
	db	$90,$2d
	db	$ff,$00
lefou_gun2
	db	$83,$2e,$00
	db	$fe
x2e
	db	$d8,$9f,$7a,$00
	db	$ff		
lefou_jump2	
	db	$83,$2f,$00
	db	$fe
x2f
	db	$c7,$8d,$7b,$00
	db	$ff	
lefou_hit2	   
	db	$8e,$34,$00
	db	$fe	
lefou_falls2	
	db	$90,$03,$00
	db	$fe	
gaston_falls2
	db	$97,$30,$00
	db	$fe
x30
	db	$d9,$ac,$fd,$01,$20,$10
	db	$ff
gaston_hit2	   
	db	$97,$34,$00
	db	$fe	
x34
	db	$c1,$95,$61,0
	db	$ff
arrow_fire2  
	db	$9e,$31,$00
	db	$fe
x31		
	db	$da,$97,$fd,$01,$00,$04
	db	$ff
arrow_beast2   
	db	$9b,$24,$00
	db	$fe		
gaston_tumble2
	db	$8f,$32,$00
	db	$fe		
x32
	db	$db,$95,$7c,$00
	db	$ff
gaston_punch2
	db	$87,$33,$00
	db	$fe
x33
	db	$dc,$8b,$7d,$00
	db	$ff    
belle_gigle2
	db	$97,$35,$00
	db	$fe		
x35
	db	$dd,$83,$fd,$04,$17,$1a,$e1
	db	$82,$fd,$04,$17,$19,$e1
	db	$82,$fd,$04,$16,$18,$e1
	db	$82,$fd,$04,$15,$17,$e1
	db	$82,$fd,$04,$14,$16,$e1
	db	$ff



endgame11
	db	$84,$42,$46,$46,$46,$46
	db	$ff,$00
endgame12
	db	$84,$43,$47,$47,$47,$47
	db	$ff,$00
endgame13
	db	$84,$44,$48,$48,$48,$48
	db	$ff,$00
endgame14
	db	$80,$45
	db	$ff,$00

x42
	db	$d3,$82,$68,$1f,$18,$18,$1f,$1d,$18,$18,$1d,$1b,$18,$18
	db	$1b,$1a,$18,$18,$1a
	db	$1f,$18,$18,$1f,$1d,$18,$18,$1d,$1b,$18,$18
	db	$1b,$1a,$18,$18,$1a
	db	$1f,$18,$18,$1f,$1d,$18,$18,$1d,$1b,$18,$18
	db	$1b,$1a,$18,$18,$1a
	db	$17,$13,$17,$13,$18,$13,$18,$13,$1a,$13,$1a,$13
	db	$1f,$13,$1f,$13
	db	$19,$15,$19,$15,$1a,$15,$1a,$15,$1c,$15,$1c,$15
	db	$21,$15,$21,$15
	db	$ff
x46
	db	$84,$1b,$1f,$22,$26,$22,$1f,$1b,$1f
	db	$84,$1b,$1f,$22,$26,$22,$1f,$1b,$1f
	db	$84,$1d,$20,$24,$27,$24,$20,$1d,$20
	db	$84,$1d,$20,$24,$27,$24,$20,$1d,$20
	db	$ff
x43
	db	$d4,$a0,$60,$24
	db	$a0,$6f,$24
	db	$a0,$60,$24
	db	$a0,$64,$23
	db	$a0,$64,$25
	db	$ff
x47
	db	$a0,$63,$16
	db	$90,$61,$16
	db	$90,$63,$16
	db	$a0,$64,$18
	db	$90,$63,$16
	db	$90,$64,$18
	db	$ff
x44
	db	$d4,$88,$72,$0c,$0c,$0c,$0e,$07,$07,$07,$0b
	db	$d4,$88,$72,$0c,$0c,$0c,$0e,$07,$07,$07,$0e
	db	$d4,$88,$72,$09,$09,$09,$09
	db	$ff
x48
	db	$d4,$88,$72,$0f,$0f,$0f,$0f,$0a,$0a,$0f,$0f
	db	$d4,$88,$72,$11,$11,$11,$11,$0c,$0c,$11,$11
	db	$ff
x45
	db	$c3,$84,$01,$c2,$01
	db	$ff

x00
	db	$e1,$ff

endgame01
	db	$86,$3d
	db	$ff,$00
endgame02
	db	$86,$3c
	db	$86,$40,$40
	db	$ff,$00
endgame03
	db	$86,$3e,$3e,$3e,$3e
	db	$86,$41,$41,$41,$41
	db	$ff,$00
endgame04
	db	$80,$3f
	db	$ff,$00

x3c
	db	$d3
	db	$a0,$64,$1c
	db	$6f,$1d
	db	$64,$1c
	db	$6f,$1d
	db	$64,$1c
	db	$6f,$1d
	db	$64,$1c
	db	$6f,$1d
	db	$ff

balconies1
	db	$85,$36,$36,$39,$39,$3a
	db	$83,$36,$36,$39,$39,$3a
	db	$ff,$00
balconies3
	db	$85,$37,$37,$37,$37,$37,$37,$37,$37,$37,$37,$37,$37
	db	$3b
	db	$83,$37,$37,$37,$37,$37,$37,$37,$37,$37,$37,$37,$37
	db	$3b
	db	$ff,$00
balconies4
	db	$80,$38
	db	$ff,$00

x36
	db	$c0,$84,$6a
	db	$18,$1b,$24,$1b,$18,$1b,$23,$1b
	db	$18,$1b,$22,$1b,$18,$1b,$23,$1b
	db	$ff
x37	
	db	$c1,$84,$72,$0c
	db	$0c
	db	$0f
	db	$0c
	db	$12
	db	$13
	db	$11
	db	$0f
	db	$ff
x38
	db	$c2,$84,$01,$c3,$01
	db	$ff
x39
	db	$d0,$90,$68,$24
	db	$82,$23,$24,$25,$26,$27,$28,$29,$2a
	db	$94,$2b
	db	$82,$27,$e2
	db	$82,$26,$81,$27,$26
	db	$82,$24,$e2

	db	$90,$1f
	db	$82,$1e,$1f,$20,$21,$22,$23,$24,$25
	db	$94,$26
	db	$82,$1f,$e2
	db	$82,$1b,$81,$1d,$1e
	db	$82,$1f,$e2
	db	$ff
x3a
	db	$d0,$84,$68,$24,$23,$26,$24,$27,$26,$29,$27
	db	$2b,$29,$2c,$2b,$30,$2f,$32,$33
	db	$ff
x3b
	db	$d0,$84,$72,$0c,$0b,$0e,$0c,$0f,$0e,$11,$0f
	db	$13,$11,$14,$13,$18,$17,$1a,$1b
	db	$ff
	

prologue1
	db	$8b,$6d
	db	$ff,$00
prologue3
	db	$8b,$6e
	db	$ff,$00
prologue4
	db	$80,$6f
	db	$ff,$00

s6d
	db	$d2,$84
	db	$60,$18
	db	$65,$0f
	db	$67,$13
	db	$65,$0f
	db	$62,$13
	db	$65,$0f
	db	$66,$13
	db	$60,$0c

	db	$61,$14
	db	$66,$0c
	db	$7f,$14
	db	$66,$0c
	db	$63,$0f
	db	$66,$0c
	db	$66,$0f
	db	$66,$0c
	db	$88,$66,$13
	db	$88,$62,$13
	db	$88,$66,$13
	db	$88,$65,$0f
	db	$a0,$66,$13

	db	$d2,$84
	db	$60,$18
	db	$65,$0f
	db	$67,$13
	db	$65,$0f
	db	$62,$13
	db	$65,$0f
	db	$66,$13
	db	$60,$0c

	db	$61,$14
	db	$66,$0c
	db	$7f,$14
	db	$66,$0c
	db	$63,$0f
	db	$66,$0c
	db	$66,$0f
	db	$66,$0c
	db	$88,$66,$13
	db	$88,$62,$13
	db	$88,$66,$13
	db	$88,$65,$0f
	db	$a0,$66,$13

	db	$d0,$90,$65,$08

	db	$88,$60,$11
	db	$88,$61,$14
	db	$84,$64,$18
	db	$84,$60,$18
	db	$84,$67,$13
	db	$8c,$62,$13
	db	$88,$65,$0f
	db	$8c,$60,$11
	db	$84,$65,$14
	db	$88,$61,$14
	db	$88,$60,$11
	db	$90,$66,$13
	db	$90,$61,$13

	db	$a8,$67,$18
	db	$88,$62,$18
	db	$88,$60,$18
	db	$88,$62,$18
	db	$8c,$61,$14
	db	$a4,$65,$14
	db	$88,$66,$18
	db	$88,$62,$18
	db	$98,$60,$18
	db	$88,$67,$13
	db	$88,$62,$13
	db	$88,$66,$13
	db	$88,$62,$13
	db	$88,$65,$0f
	db	$a0,$66,$13
	db	$a0,$61,$13
	db	$ff   
s6e
	db	$c1
	db	$98,$72,$13
	db	$88,$13
	db	$98,$11
	db	$88,$0a
	db	$90,$0c
	db	$90,$07
	db	$94,$0c
	db	$84,$0e
	db	$84,$0f
	db	$84,$13

	db	$98,$72,$14
	db	$88,$13
	db	$98,$11
	db	$88,$0a
	db	$90,$0c
	db	$90,$07
	db	$94,$00
	db	$84,$0c
	db	$84,$0e
	db	$84,$0f

	db	$8c,$11
	db	$84,$11
	db	$88,$11
	db	$88,$13

	db	$8c,$0c
	db	$84,$0c
	db	$88,$0c
	db	$88,$0f

	db	$8c,$11
	db	$84,$11
	db	$88,$11
	db	$88,$0e

	db	$8c,$13
	db	$84,$13
	db	$84,$14
	db	$84,$13
	db	$84,$11
	db	$84,$0f

	db	$8c,$0c
	db	$84,$0c
	db	$88,$0c
	db	$88,$07

	db	$88,$0c
	db	$84,$0f
	db	$8c,$13
	db	$88,$0c

	db	$8c,$0d
	db	$84,$0d
	db	$88,$0d
	db	$88,$08

	db	$88,$0d
	db	$84,$11
	db	$8c,$14
	db	$88,$0d

	db	$8c,$0c
	db	$84,$0c
	db	$88,$0c
	db	$88,$07

	db	$88,$0c
	db	$84,$0f
	db	$8c,$13
	db	$88,$08

	db	$8c,$07
	db	$84,$07
	db	$88,$0c
	db	$88,$07

	db	$88,$0e
	db	$84,$11
	db	$88,$13
	db	$84,$07
	db	$84,$0f
	db	$84,$11
	db	$ff
s6f
	db	$c3,$84,$02,$c2,$02
	db	$c3,$02,$c2,$02
	db	$c3,$02,$c2,$02
	db	$c3,$88,$02
	db	$ff

beastdies1
	db	$8a,$6a,$00
	db	$fe
beastdies3
	db	$8a,$6b,$00
	db	$fe
beastdies4
	db	$80,$6c,$00
	db	$fe

s6a
	db	$c0,$90,$63,$11
	db	$60,$11
	db	$65,$0f
	db	$d2,$82,$6a,$0c,$0f,$0e,$0c,$12,$13,$19,$16,$c0,$84,$65,$1b
	db	$ff
s6b
	db	$c1,$90,$72,$16
	db	$90,$11
	db	$90,$0c
	db	$82,$00,$03,$02,$00,$06,$07,$0d,$0a,$83,$73,$0c
	db	$ff
s6c
	db	$c2,$88,$02,$02,$02,$02,$02,$02,$02,$02,$84,$04
	db	$ff

panic1
	db	$83,$67,$00
	db	$fe
panic3
	db	$83,$68,$00
	db	$fe
panic4
	db	$80,$69,$00
	db	$fe

s67
	db	$c0
	db	$96,$7c,$24,$24,$23,$23,$24,$24,$25,$25
	db	$95,$24,$24,$23,$23,$24,$24,$25,$25
	db	$94,$24,$24,$23,$23,$24,$24,$25,$25
	db	$93,$24,$24,$23,$23,$24,$24,$25,$25
	db	$92,$24,$24,$23,$23,$24,$24,$25,$25
	db	$91,$24,$24,$23,$23,$24,$24,$25,$25
	db	$90,$24,$24,$23,$23,$24,$24,$25,$25
	db	$8f,$24,$24,$23,$23,$24,$24,$25,$25
	db	$8e,$24,$24,$23,$23,$24,$24,$25,$25
	db	$8d,$24,$24,$23,$23,$24,$24,$25,$25
	db	$8c,$24,$24,$23,$23,$24,$24,$25,$25
	db	$8b,$24,$24,$23,$23,$24,$24,$25,$25
	db	$8a,$24,$24,$23,$23,$24,$24,$25,$25
	db	$89,$24,$24,$23,$23,$24,$24,$25,$25
	db	$88,$24,$24,$23,$23,$24,$24,$25,$25
	db	$87,$24,$24,$23,$23,$24,$24,$25,$25
	db	$86,$24,$24,$23,$23,$24,$24,$25,$25
	db	$85,$24,$24,$23,$23,$24,$24,$25,$25
	db	$ff
s68
	db	$c1
	db	$96,$72,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$95,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$94,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$93,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$92,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$91,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$90,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$8f,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$8e,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$8d,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$8c,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$8b,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$8a,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$89,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$88,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$87,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$86,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$85,$0c,$0c,$12,$12,$13,$13,$14,$14
	db	$ff
s69
	db	$c3
	db	$ac,4,4,4,4
	db	$aa,4,4,4,4
	db	$a8,4,4,4,4
	db	$a6,4,4,4,4
	db	$a4,4,4,4,4
	db	$a2,4,4,4,4
	db	$a0,4,4,4,4
	db	$9e,4,4,4,4
	db	$9c,4,4,4,4
	db	$9a,4,4,4,4
	db	$98,4,4,4,4
	db	$96,4,4,4,4
	db	$94,4,4,4,4
	db	$92,4,4,4,4
	db	$90,4,4,4,4
	db	$8e,4,4,4,4
	db	$8c,4,4,4,4
	db	$8a,4,4,4,4
	db	$ff

transformation1
	db	$83,$60,$5c,$5c,$63
	db	$00
	db	$fe
transformation2
	db	$9b,$61,$61,$83,$5d,$5d,$64
	db	$00
	db	$fe
transformation3
	db	$83,$62,$5e,$5e,$65
	db	$00
	db	$fe
transformation4
	db	$80,$5f,$5f
	db	$80,$5f,$5f,$5f,$5f
	db	$80,$5f,$5f,$5f,$5f
	db	$80,$66
	db	$00
	db	$fe
s5c
	db	$d4,$a0
	db	$63,$1f,$61,$1f
	db	$62,$1c,$60,$1c
	db	$63,$18,$61,$18
	db	$63,$18,$63,$1a
	db	$ff
s5d
	db	$d4,$82,$6a
	db	$18,$1c,$1f,$24,$28,$24,$1f,$1c
	db	$18,$1c,$1f,$24,$28,$26,$24,$26
	db	$17,$1a,$1f,$23,$26,$23,$1f,$1a
	db	$17,$1a,$1f,$23,$26,$24,$23,$24
	db	$15,$18,$1c,$21,$24,$21,$1c,$18
	db	$15,$18,$1c,$21,$24,$23,$21,$23
	db	$13,$17,$1c,$1f,$23,$1f,$1c,$17
	db	$13,$17,$1c,$1f,$23,$24,$23,$21

	db	$11,$15,$18,$1d,$21,$1d,$18,$15
	db	$11,$15,$18,$1d,$21,$23,$24,$1f
	db	$13,$18,$1c,$1f,$24,$1f,$1c,$18
	db	$13,$18,$1c,$1f,$1f,$21,$23,$24
	db	$15,$18,$1d,$21,$24,$21,$1d,$18
	db	$15,$18,$1d,$21,$21,$23,$24,$26
	db	$17,$1a,$1f,$23,$26,$23,$1f,$1a
	db	$17,$1a,$1f,$23,$23,$24,$26,$28
	db	$ff
s5e
	db	$c1,$88,$72
	db	$0c,$0c,$0c,$0e,$07,$07,$07,$04
	db	$09,$09,$09,$0b,$04,$04,$04,$00
	db	$05,$05,$05,$07,$00,$00,$04,$00
	db	$05,$05,$05,$09,$07,$07,$09,$0b
	db	$ff
s5f
	db	$c3,$84,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$c3,$01,$c2,$02
	db	$ff
s60
	db	$d4
	db	$8c,$66,$18
	db	$8c,$6f,$18
	db	$8c,$66,$18
	db	$8c,$6f,$18
	db	$88,$66,$18
	db	$88,$6f,$18
	db	$8c,$61,$18
	db	$8c,$66,$18
	db	$8c,$6f,$18
	db	$8c,$66,$18
	db	$90,$61,$18
	db	$ff
s61
	db 	$d5,$82,$6a
	db	$0a,$0c,$13,$0a,$0c,$13,$0a,$0c
	db	$11,$0a,$0c,$11,$0a,$0c,$10,$11
	db	$0a,$0c,$13,$0a,$0c,$13,$0a,$0c
	db	$11,$0a,$0c,$10,$0a,$0c,$11,$13
	db	$ff
s62
	db	$c1,$90,$72,$0c,$0c,$0c,$0c,$0c,$0c,$0c,$0c
	db	$ff
s63
	db	$d4,$8a,$63,$13
	db	$d4,$92,$64,$10
	db	$d4,$a0,$61,$0c
	db	$cb,$84,$61,$24
	db	$ff
s64
	db	$d4,$82,$6a
	db	$29,$28,$83,$24,$1f,$84,$1d,$1f,$85,$1d,$1b,$a0,$1c
	db	$cb,$85,$64,$28
	db	$ff
s65
	db	$c1,$84,$72,$18
	db	$86,$13
	db	$88,$10
	db	$8a,$0e
	db	$a0,$0c
	db	$84,$0c
	db	$ff
s66
	db	$c3,$84,$01,$c2,$86,$02
	db	$c3,$88,$01,$c2,$8a,$02
	db	$c3,$90,$01,$c2,$90,$02
	db	$c3,$85,$03
	db	$ff

gaston1
	db	$88,$55,$55,$8c,$55,$55
	db	$8c,$58
	db	$88,$5a,$5a,$8c,$5a,$5a
	db	$ff,$00
gaston3
	db	$88,$56,$56,$8c,$56,$56
	db	$8c,$59
	db	$88,$5b,$5b,$8c,$5b,$5b
	db	$ff,$00
gaston4
	db	$80,$57
	db	$ff,$00
s55
	db	$c0,$92,$60,$0c
	db	$84,$65,$0f
	db	$82,$66,$13
	db	$84,$62,$13
	db	$82,$66,$13
	db	$82,$65,$0f
	db	$c0,$92,$64,$0c
	db	$84,$63,$0f
	db	$82,$7f,$14
	db	$82,$61,$14
	db	$82,$7f,$14
	db	$84,$63,$0f
	db	$ff
s56
	db	$c1,$84,$72
	db	$0c,$13,$0f,$13,$14,$13,$0f,$0c
	db	$0f,$14,$13,$14,$16,$18,$11,$13
	db	$ff
s57
	db	$c3,$84,$01,$c2,$02
	db	$ff
s58
	db	$c0,$88,$7c
	db	$18,$17,$18,$17
	db	$1d,$1c,$1d,$1c
	db	$20,$1f,$20,$1f
	db	$84,$25,$24,$25,$24
	db	$82,$25,$24,$25,$24,$84,$25,$24
	db	$ff
s59
	db	$c1,$88,$73
	db	$0c,$0b,$0c,$0b,$11,$10,$11,$10
	db	$14,$13,$14,$13
	db	$84,$18,$17,$18,$17
	db	$82,$18,$17,$18,$17,$84,$18,$17
	db	$ff
s5a
	db	$c0,$82,$60,$18
	db	$c0,$82,$60,$18,$e2
	db	$c0,$82,$60,$18,$e2
	db	$c0,$82,$60,$18,$e2
	db	$c0,$86,$62,$18
	db	$c0,$82,$60,$18
	db	$d5,$82,$18,$1b,$21,$24,$27

	db	$c0,$82,$60,$18
	db	$c0,$82,$60,$18,$e2
	db	$c0,$82,$60,$18,$e2
	db	$c0,$82,$60,$18,$e2
	db	$c0,$86,$62,$1a
	db	$c0,$82,$60,$18
	db	$d5,$82,$27,$24,$21,$1b,$18
	db	$ff
s5b
	db	$c1
	db	$82,$72,$0c
	db	$82,$72,$0c
	db	$82,$74,$00
	db	$82,$72,$0c
	db	$82,$74,$00
	db	$82,$72,$0c
	db	$82,$74,$00
	db	$82,$72,$0d
	db	$82,$74,$01
	db	$82,$74,$0d
	db	$82,$72,$0c

	db	$82,$74,$18,$1b,$21,$24,$27

	db	$c1
	db	$82,$72,$0c
	db	$82,$72,$0c
	db	$82,$74,$00
	db	$82,$72,$0c
	db	$82,$74,$00
	db	$82,$72,$0c
	db	$82,$74,$00
	db	$82,$72,$0f
	db	$82,$74,$03
	db	$82,$74,$0f
	db	$82,$72,$0c

	db	$82,$74,$27,$24,$21,$1b,$18
	db	$ff


beautyintro1
	db	$88,$51
	db	$ff,$00
beautyintro2
	db	$88,$52
	db	$ff,$00
beautyintro3
	db	$88,$53
	db	$ff,$00
beautyintro4
	db	$80,$54
	db	$ff,$00

s51
	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$1c,$1d,$1a,$1c 	
	db	$90,$18,$13 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$1c,$1d,$1a,$1c
	db	$90,$18,$1f 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$1c,$1d,$1a,$1c 	
	db	$90,$18,$13 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2 	
	db	$84,$1c,$1d,$1a,$1c 	
	db	$90,$18,$1f 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$15,$17,$17,$18 	
	db	$a0,$11 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$15,$17,$17,$18 	
	db	$a0,$1a 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$18,$1a,$1c,$1d 	
	db	$90,$1f 	
	db	$84,$1f,$1d,$1c,$1a 	
	db	$90,$18 	
	db	$84,$1d,$1c,$1a,$18 	
	db	$a0,$13 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$15,$17,$17,$18 	
	db	$a0,$11 	

	db	$d5,$82,$6a,$0c,$10,$13,$18,$1c,$18,$13,$10

	db	$d2
	db	$84,$15,$17,$17,$18 	
	db	$a0,$1a

	db	$d5,$82,$6a,$11,$15,$18,$1d,$21,$1d,$18,$15

	db	$d2
	db	$84,$1c,$1d,$1d,$1f
	db	$90,$18

	db	$84,$1c
	db	$84,$1d
	db	$84,$1a
	db	$84,$1c
	db	$a0,$18

	db	$d5,$82,$6a,$0c,$11,$13,$18,$1d,$18,$13,$11
	db	$d5,$82,$6a,$0c,$11,$13,$18,$1d,$18,$13,$11
	db	$ff
s52
	db	$d4,$88
	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$a0
	db	$64,$10
	db	$60,$10
	db	$61,$11
	db	$90,$66,$13,$61,$13

	db	$88
	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11

	db	$64,$10
	db	$64,$10
	db	$64,$10
	db	$64,$10
	db	$90,$60,$13,$63,$13

	db	$a0,$61,$11,$60,$0e

	db	$88
	db	$64,$10,$10,$10,$10
	db	$6f,$11,$11,$11,$11
	db	$ff
s53
	db	$c1
	db	$8c,$72,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07
	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07

	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07
	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07

	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07
	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07

	db	$8c,$00
	db	$84,$00
	db	$88,$07
	db	$88,$00
	db	$8c,$04
	db	$84,$04
	db	$88,$0b
	db	$88,$04
	db	$8c,$05
	db	$84,$05
	db	$88,$00
	db	$88,$00
	db	$8c,$07
	db	$84,$07
	db	$88,$07
	db	$88,$07

	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07
	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$90,$13
	db	$90,$0c

	db	$90,$11
	db	$84,$11
	db	$84,$15
	db	$84,$10
	db	$84,$11
	db	$90,$0e
	db	$84,$10
	db	$84,$11
	db	$84,$0e
	db	$84,$10

	db	$8c,$0c
	db	$90,$0c
	db	$84,$0c
	db	$8c,$07
	db	$90,$07
	db	$84,$07
	db	$ff
s54
	db	$c2,$88,$00
	db	$ff

waltz1
	db	$86,$4a,$4a
	db	$86,$4e
	db	$ff,$00
waltz2
	db	$86,$4b,$92,$4b
	db	$86,$4f
	db	$ff,$00
waltz3
	db	$86,$4c,$4c
	db	$86,$50
	db	$ff,$00
waltz4
	db	$80,$4d
	db	$ff,$00
s4a
	db	$d1,$88,$68,$1f
	db	$d1,$88,$68,$1d
	db	$d1,$86,$68,$1c,$81,$1d,$1c
	db	$d1,$84,$68,$1a,$c9
	db	$d1,$84,$68,$1a
	db	$d1,$84,$68,$1c
	db	$d1,$84,$68,$1a,$c9

	db	$d1,$88,$68,$1f
	db	$d1,$88,$68,$1d
	db	$d1,$86,$68,$1c,$81,$1d,$1c
	db	$d1,$84,$68,$1a,$c9
	db	$d1,$84,$68,$1a
	db	$d1,$84,$68,$1c
	db	$d1,$84,$68,$1a,$c9

	db	$d1,$88,$68,$17
	db	$d1,$88,$68,$17
	db	$d1,$86,$68,$18,$81,$1a,$18
	db	$d1,$84,$68,$17,$c9
	db	$d1,$84,$68,$13,$c9
	db	$d1,$84,$68,$10,$c9

	db	$d1,$88,$68,$17
	db	$d1,$88,$68,$17
	db	$d1,$86,$68,$18,$81,$1a,$18
	db	$d1,$84,$68,$17,$c9
	db	$d1,$84,$68,$13,$c9
	db	$d1,$84,$68,$10,$c9
	db	$ff
s4b
	db	$cd,$84,$7e
	db	$0e,$11,$15,$11,$15,$11
	db	$0e,$13,$17,$13,$17,$13
	db	$0e,$11,$15,$11,$15,$11
	db	$0e,$13,$17,$13,$17,$13
	db	$0c,$13,$17,$13,$17,$13
	db	$0c,$13,$15,$13,$15,$13
	db	$0c,$13,$17,$13,$17,$13
	db	$0c,$13,$15,$13,$15,$13
	db	$ff
s4c
	db	$c1
	db	$88,$72,$0e
	db	$84,$73,$0e,$74,$02
	db	$88,$73,$0e
	db	$88,$72,$13
	db	$84,$73,$13,$74,$07
	db	$88,$73,$13
	db	$88,$72,$0e
	db	$84,$73,$0e,$74,$02
	db	$88,$73,$0e
	db	$88,$72,$13
	db	$84,$73,$13,$74,$07
	db	$88,$73,$13
	db	$88,$72,$0c
	db	$84,$73,$0c,$74,$00
	db	$88,$73,$0c
	db	$88,$72,$07
	db	$84,$73,$07,$74,$07
	db	$88,$73,$07
	db	$88,$72,$0c
	db	$84,$73,$0c,$74,$00
	db	$88,$73,$0c
	db	$88,$72,$07
	db	$84,$73,$07,$74,$07
	db	$88,$73,$07
	db	$ff
s4d
	db	$c2,$88,$02
	db	$c2,$88,$02
	db	$c3,$88,$02
	db	$ff
s4e
	db	$d1,$88,$68,$11
	db	$d1,$84,$68,$11
	db	$d1,$84,$68,$13
	db	$d1,$84,$68,$15,$c9
	db	$d1,$88,$68,$17
	db	$d1,$88,$68,$18
	db	$d1,$84,$68,$1a,$c9
	db	$d1,$88,$68,$1c
	db	$d1,$88,$68,$18
	db	$d1,$88,$68,$13
	db	$d1,$8c,$68,$17,$c9,$84,$15
	db	$d1,$84,$68,$15,$c9

	db	$d1,$88,$68,$1a
	db	$d1,$88,$68,$18
	db	$d1,$84,$68,$15,$c9
	db	$d1,$88,$68,$17
	db	$d1,$88,$68,$15
	db	$d1,$84,$68,$11,$c9
	db	$d1,$88,$68,$10
	db	$d1,$88,$68,$11
	db	$d1,$88,$68,$12
	db	$d1,$90,$68,$13,$c9,$88

	db	$d1,$88,$68,$11
	db	$d1,$84,$68,$11
	db	$d1,$84,$68,$15
	db	$d1,$84,$68,$18,$c9
	db	$d1,$88,$68,$17
	db	$d1,$88,$68,$18
	db	$d1,$84,$68,$1a,$c9
	db	$d1,$88,$68,$1c
	db	$d1,$88,$68,$1d
	db	$d1,$84,$68,$1f,$c9
	db	$d1,$8c,$68,$21,$c9,$84,$1c
	db	$d1,$84,$68,$1c,$c9

	db	$d1,$88,$68,$1d
	db	$d1,$84,$68,$1d
	db	$d1,$84,$68,$1f
	db	$d1,$84,$68,$1d,$c9
	db	$d1,$88,$68,$1c
	db	$d1,$88,$68,$1a
	db	$d1,$84,$68,$13,$c9
	db	$d1,$88,$68,$18
	db	$d1,$88,$68,$1c
	db	$d1,$88,$68,$1f
	db	$d1,$90,$68,$24,$c9,$88
	db	$ff
s4f
	db	$cd,$84,$7e
	db	$0e,$15,$18,$0e,$18,$15
	db	$11,$17,$1a,$17,$1a,$17
	db	$17,$18,$1c,$18,$1c,$18
	db	$13,$18,$1c,$18,$1c,$18

	db	$0e,$11,$15,$11,$15,$11
	db	$0c,$13,$17,$13,$17,$13
	db	$10,$0c,$11,$0c,$12,$0c
	db	$13,$10,$18,$13,$1c,$18

	db	$0e,$15,$18,$0e,$18,$15
	db	$11,$17,$1a,$17,$1a,$17
	db	$17,$18,$1c,$18,$1c,$18
	db	$13,$18,$1c,$18,$1c,$18

	db	$0e,$11,$15,$11,$15,$11
	db	$0c,$13,$17,$13,$17,$13
	db	$88,$1c,$1f,$24
	db	$98,$28
	db	$ff
s50
	db	$c1
	db	$88,$72,$11
	db	$84,$73,$11,$74,$05
	db	$88,$73,$11
	db	$88,$72,$13
	db	$84,$73,$13,$74,$07
	db	$88,$73,$13
	db	$88,$72,$0c
	db	$84,$73,$0c,$74,$00
	db	$88,$73,$0c
	db	$88,$72,$15
	db	$84,$73,$15,$74,$09
	db	$88,$73,$15
	db	$88,$72,$0e
	db	$84,$73,$0e,$74,$02
	db	$88,$73,$0e
	db	$88,$72,$13
	db	$84,$73,$13,$74,$07
	db	$88,$73,$13
	db	$88,$72,$10
	db	$84,$73,$11,$74,$05
	db	$88,$73,$12
	db	$88,$72,$13
	db	$84,$73,$07,$74,$07
	db	$88,$73,$10
	db	$88,$72,$11
	db	$84,$73,$11,$74,$05
	db	$88,$73,$11
	db	$88,$72,$13
	db	$84,$73,$13,$74,$07
	db	$88,$73,$13
	db	$88,$72,$0c
	db	$84,$73,$0c,$74,$00
	db	$88,$73,$0c
	db	$88,$72,$15
	db	$84,$73,$15,$74,$09
	db	$88,$73,$15
	db	$88,$72,$0e
	db	$84,$73,$0e,$74,$02
	db	$88,$73,$0e
	db	$88,$72,$13
	db	$84,$73,$13,$74,$07
	db	$88,$73,$13
	db	$88,$72,$18
	db	$84,$73,$13,$74,$07
	db	$88,$73,$10
	db	$88,$72,$0c
	db	$84,$73,$0c,$74,$00
	db	$88,$73,$0c
	db	$ff

stainglas1
	db	$86,$45,$45,$45,$45
	db	$86,$48,$48
	db	$ff,$00
stainglas2
	db	$99,$73,$45,$45,$45,$45
	db	$86,$71,$48,$48
	db	$ff,$00
stainglas3
	db	$86,$46,$46,$46,$46
	db	$86,$49,$49,$49,$49
	db	$ff,$00
stainglas4
	db	$80,$47
	db	$ff,$00

x3d
s45
	db	$d3
	db	$84,$6a,$1d
	db	$84,$1c
	db	$84,$1a
	db	$88,$18
	db	$84,$1c
	db	$84,$1f
	db	$84,$24
	db	$84,$21
	db	$84,$1f
	db	$84,$1d
	db	$88,$1f
	db	$84,$13
	db	$84,$1a
	db	$84,$1f
	db	$ff
x3e
s46
	db	$c1
	db	$8c,$72,$0c
	db	$90,$0c
	db	$84,$0e
	db	$8c,$07
	db	$90,$07
	db	$84,$07
	db	$ff
x3f
s47
	db	$c2,$88,$00,$02,$00,$02,$00,$02,$84,$00,$00,$88,$02
	db	$ff
x40
s48
	db	$d4
	db	$8c,$61,$18
	db	$8c,$63,$13
	db	$88,$61,$18
	db	$8c,$60,$1c
	db	$8c,$62,$17
	db	$88,$61,$1f
	db	$8c,$61,$1d
	db	$8c,$63,$18
	db	$88,$6e,$18
	db	$84,$67,$1a
	db	$88,$63,$1a
	db	$88,$64,$17
	db	$8c,$61,$13

	db	$8c,$63,$13
	db	$8c,$61,$18
	db	$88,$64,$1c
	db	$8c,$60,$1c
	db	$8c,$62,$17
	db	$88,$61,$1f
	db	$8c,$61,$1d
	db	$8c,$63,$18
	db	$88,$6e,$18
	db	$84,$67,$1a
	db	$88,$63,$1a
	db	$88,$66,$1a
	db	$8c,$64,$17
	db	$ff
x41
s49
	db	$c1
	db	$8c,$72,$00
	db	$8c,$07
	db	$88,$00
	db	$8c,$04
	db	$8c,$0b
	db	$88,$04
	db	$8c,$05
	db	$8c,$0c
	db	$88,$05
	db	$84,$0c
	db	$88,$0b
	db	$88,$07
	db	$8c,$02
	db	$ff

lefou1
	db	$90,$3e,$8c,$3e
	db	$8f,$41,$41
	db	$8a,$43
	db	$8f,$41,$41
	db	$ff,$00
lefou3
	db	$84,$3f,$83,$3f
	db	$8f,$42,$90,$42,$8f,$42,$90,$42
	db	$8a,$44
	db	$8f,$42,$90,$42,$8f,$42,$90,$42
	db	$ff,$00
lefou4
	db	$80,$40
	db	$ff,$00

s3e
	db	$ce
	db	$84,$7e,$18
	db	$84,$17
	db	$82,$16
	db	$82,$17
	db	$82,$16
	db	$82,$15
	db	$84,$14
	db	$84,$13
	db	$84,$12
	db	$84,$13
	db	$84,$14
	db	$84,$13
	db	$82,$12
	db	$82,$13
	db	$82,$12
	db	$82,$11
	db	$84,$10
	db	$84,$0f
	db	$84,$0e
	db	$84,$0f
	db	$ff
s3f
	db	$c1
	db	$84,$72,$18
	db	$84,$73,$17
	db	$82,$72,$16
	db	$82,$74,$17
	db	$82,$73,$16
	db	$82,$74,$15
	db	$84,$72,$14
	db	$84,$73,$13
	db	$84,$72,$12
	db	$84,$73,$13
	db	$84,$72,$14
	db	$84,$73,$13
	db	$82,$72,$12
	db	$82,$74,$13
	db	$82,$73,$12
	db	$82,$74,$11
	db	$84,$72,$10
	db	$84,$73,$0f
	db	$84,$72,$0e
	db	$84,$73,$0f
	db	$ff
s40
	db	$c3,$84,$01,$c2,$02
	db	$ff
s41
	db	$d2
	db	$84,$6a,$18,$18,$17,$17,$16,$16,$82,$18,$19,$1a,$1b
	db	$84,$6a,$18,$18,$17,$17,$16,$16,$82,$17,$18,$19,$1a
	db	$82,$6a
	db	$1b,$14,$17,$1b,$14,$17,$1b,$14
	db	$17,$1b,$14,$17,$1b,$14,$17,$19
	db	$1b,$14,$17,$1b,$14,$17,$1b,$14
	db	$17,$1b,$14,$17,$1b,$14,$0f,$08
	db	$ff
s42
	db	$c1
	db	$84,$72,$0c
	db	$73,$13
	db	$72,$07
	db	$73,$13
	db	$72,$0c
	db	$73,$13
	db	$72,$07
	db	$73,$13
	db	$72,$0c
	db	$73,$13
	db	$72,$07
	db	$73,$13
	db	$72,$0c
	db	$73,$13
	db	$72,$07
	db	$73,$13
	db	$ff
s43
	db	$c7
	db	$88,$0c
	db	$88,$07
	db	$84,$08
	db	$c5,$84,$6a,$1f,$ca,$c5
	db	$c7
	db	$88,$0c
	db	$88,$07
	db	$84,$08
	db	$c5,$84,$6a,$24,$ca,$c5
	db	$c7
	db	$88,$0c
	db	$88,$07
	db	$84,$08
	db	$c5,$84,$6a,$1f,$ca,$c5
	db	$c7
	db	$88,$0c
	db	$88,$07
	db	$c5,$84,$6a,$14
	db	$82,$17,$18,$19,$1a,$1b,$1c
	db	$ff
s44
	db	$c1
	db	$84
	db	$72,$0c
	db	$73,$0c
	db	$72,$07
	db	$73,$07
	db	$72,$08
	db	$73,$18
	db	$72,$08
	db	$73,$18
	db	$72,$0c
	db	$73,$0c
	db	$72,$07
	db	$73,$07
	db	$72,$08
	db	$73,$1e
	db	$72,$12
	db	$73,$1e
	db	$72,$0c
	db	$73,$0c
	db	$72,$07
	db	$73,$07
	db	$72,$08
	db	$73,$18
	db	$72,$07
	db	$73,$18
	db	$72,$0c
	db	$73,$0c
	db	$72,$07
	db	$73,$07
	db	$72,$20
	db	$82
	db	$73,$17
	db	$74,$18
	db	$72,$19
	db	$74,$1a
	db	$73,$1b
	db	$74,$1c
	db	$ff

hugegarg1
	db	$8c,$38,$38,$3b
	db	$85,$3c
	db	$91,$3b
	db	$ff,$00
hugegarg3
	db	$8c,$39,$39,$39,$39,$39,$39
	db	$91,$3d,$3d,$3d,$8c,$3d,$3d,$3d,$3d,$91,$3d
	db	$91,$3d,$3d,$3d,$8a,$3d,$89,$3d,$3d,$3d,$91,$3d
	db	$91,$39,$39,$39,$39
	db	$ff,$00
hugegarg4
	db	$80,$3a
	db	$ff,$00

s38
	db	$c7
	db	$8c,$00
	db	$88,$06
	db	$84,$06
	db	$82,$07
	db	$82,$0a
	db	$84,$01
	db	$8c,$00
	db	$88,$06
	db	$84,$06
	db	$82,$07
	db	$82,$0a
	db	$84,$0c
	db	$ff
s39
	db	$c1	
	db	$8c,$73,$00
	db	$88,$06
	db	$84,$06
	db	$82,$07
	db	$82,$0a
	db	$84,$01
	db	$8c,$00
	db	$88,$06
	db	$84,$06
	db	$82,$07
	db	$82,$0a
	db	$84,$0c
	db	$ff
s3a	
	db	$c2,$84,$04,$c6,$02
	db	$ff
s3b
	db	$d0,$84,$7e,$0c,$0e,$0f,$13,$18,$17,$14,$13
	db	$c5,$84,$6a,$0c,$0e,$0f,$13,$18,$17,$14,$13
	db	$d0,$84,$7e,$0c,$0e,$0f,$13,$1b,$1a,$17,$18
	db	$c5,$84,$6a,$0c,$0e,$0f,$13,$1b,$1a,$17,$18
	db	$d0,$84,$7e,$0c,$0e,$0f,$13,$18,$82,$17,$81,$18,$17,$84,$14,$13
	db	$c5,$84,$6a,$0c,$0e,$0f,$13,$18,$82,$17,$81,$18,$17,$84,$14,$13
	db	$d0,$84,$7e,$0e,$0f,$13,$18,$1b,$1e,$21,$24
	db	$c5,$84,$6a,$0e,$0f,$13,$18,$1b,$1e,$21,$24
	db	$ff
s3c
	db	$d1
	db	$8c,$60,$18
	db	$88,$60
	db	$84,$60
	db	$84,$6d
	db	$84,$60
	db	$84,$64
	db	$88,$60
	db	$88,$60
	db	$84,$60
	db	$84,$6d
	db	$84,$60
	db	$88,$64
	db	$88,$60
	db	$88,$67,$13
	db	$88,$62
	db	$90,$61

	db	$c5,$84,$6a,$1f,$82,$1a,$81,$1b,$1a,$84,$17,$13

	db	$d1
	db	$8c,$6d,$17
	db	$88,$6d
	db	$84,$6d
	db	$84,$64
	db	$84,$6d
	db	$84,$6b,$13
	db	$84,$61
	db	$84,$7f
	db	$88,$61
	db	$84,$61
	db	$84,$7f
	db	$84,$61
	db	$84,$6b
	db	$84,$61
	db	$84,$7f
	db	$84,$61
	db	$84,$6b
	db	$84,$61
	db	$84,$6b
	db	$84,$6d,$17
	db	$90,$60,$18

	db	$c5,$82,$6a,$1b,$1a,$18,$13,$0f,$0e,$0c,$00

	db	$d1
	db	$8c,$60,$18
	db	$88,$60,$18
	db	$84,$60,$18
	db	$84,$6d,$18
	db	$84,$60,$18
	db	$84,$64,$18
	db	$88,$60,$18
	db	$88,$60,$18
	db	$84,$60,$18
	db	$84,$6d,$18
	db	$84,$60,$18
	db	$88,$65,$1b
	db	$88,$61,$1b
	db	$88,$64,$18
	db	$88,$60,$18
	db	$90,$62,$18

	db	$c5,$84,$6a,$1d,$82,$16,$81,$18,$16,$84,$14,$11

	db	$d1
	db	$8c,$64,$17
	db	$88,$64,$17
	db	$84,$64,$17
	db	$84,$6d,$17
	db	$84,$6b,$13
	db	$84,$6b,$13
	db	$84,$61,$13
	db	$84,$7f,$13
	db	$88,$61,$13
	db	$84,$61,$13
	db	$84,$7f,$13
	db	$84,$61,$13
	db	$84,$6b,$13
	db	$84,$61,$13
	db	$84,$64,$17
	db	$84,$61,$13
	db	$84,$6b,$13
	db	$84,$61,$13
	db	$84,$63,$0e
	db	$84,$64,$0b
	db	$90,$6f,$11

	db	$c5,$82,$6a,$1f,$1b,$1a,$1d,$1b,$18,$13,$0c
	db	$ff
s3d
	db	$8c,$73,$00
	db	$88,$06
	db	$84,$06
	db	$82,$07
	db	$82,$0a
	db	$84,$0c
	db	$ff


lovejing1
	db	$91,$34,$00
	db	$fe
lovejing2
	db	$85,$35,$00
	db	$fe
lovejing3
	db	$85,$36,$00
	db	$fe
lovejing4
	db	$80,$37,$00
	db	$fe

s34
	db	$ca,$82,$6a
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c,$13,$16,$11,$13,$0a,$0c
	db	$18,$0c
	db	$ff
s35
	db	$cd,$90,$66,$18,$6f,$1d,$67,$1f,$66,$24
	db	$cc,$86,$6c,$20,$cd,$82
	db	$cc,$86,$65,$20,$cd,$82
	db	$cc,$86,$6c,$20,$cd,$82
	db	$cc,$86,$62,$24,$cd,$82
	db	$cc,$86,$61,$22,$cd,$82
	db	$cc,$86,$67,$1d,$cd,$82
	db	$cc,$86,$63,$1d,$cd,$82
	db	$cc,$86,$66,$1d,$cd,$82
	db	$cc,$9c,$66,$1f,$cd,$84
	db	$cc,$84,$7d,$18
	db	$ff
s36
	db	$c1,$88,$70
	db	$0c,$0c,$0c,$0c
	db	$0c,$0c,$0c,$0c
	db	$11,$11,$11,$11
	db	$16,$16,$16,$16
	db	$88,$0c,$0a,$05,$07,$83,$00
	db	$ff
s37
	db	$c3,$82,$00,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$c2,$c6,$c6
	db	$c3,$85,$03
	db	$ff

library1
	db	$84,$2d,$2d,$30,$30,$32
	db	$ff,$00
library3
	db	$84,$2e,$2e,$2e,$2e,$2e,$2e,$2e,$2e,$91,$31
	db	$ff,$00
library4
	db	$80,$2f,$2f,$2f,$2f,$2f,$2f,$2f,$2f,$33
	db	$ff,$00

s2d
	db	$cc,$90,$7c
	db	$18
	db	$17
	db	$18
	db	$84,$17
	db	$ca,$82,$6a,$17,$18,$17,$1e,$1f,$24
	db	$cc,$90,$7c,$18
	db	$17
	db	$14
	db	$84,$13
	db	$ca,$82,$6a,$24,$1f,$1e,$18,$17,$13
	db	$ff
s2e
	db	$c1
	db	$88,$7a
	db	$0c
	db	$13
	db	$12
	db	$0d
	db	$0c
	db	$13
	db	$12
	db	$17
	db	$ff
s2f
	db	$c3,$8c,$0d
	db	$c2,$84,$0d
	db	$c3,$8c,$0d
	db	$c2,$84,$0d
	db	$c3,$8c,$0d
	db	$c2,$84,$0d
	db	$c3,$88,$0d
	db	$c3,$88,$06
	db	$ff
s30
	db	$c5
	db	$84,$6a,$1f,$13
	db	$84,$18,$0c
	db	$84,$19,$0d
	db	$84,$1e,$12
	db	$84,$1f,$13
	db	$84,$18,$0c
	db	$84,$19
	db	$ca,$82,$17,$18,$23,$24,$2f,$30
	db	$c5
	db	$84,$1f,$13
	db	$84,$18,$0c
	db	$84,$19,$0d
	db	$84,$1e,$12
	db	$84,$1f,$13
	db	$84,$24,$18
	db	$84,$25
	db	$ca,$82,$30,$2f,$24,$23,$18,$17
	db	$ff
s31
	db	$c1,$82
	db	$68,$31,$30,$2b,$2a,$83,$25,$24,$1f,$1e
	db	$84,$19,$18,$0d,$0c,$85,$07,$86,$06,$87,$01,$88,$00
	db	$ff
s32
	db	$e5
	db	$cf,$82
	db	$31,$30,$2b,$2a,$83,$25,$24,$1f,$1e
	db	$84,$19,$18,$0d,$0c,$85,$07,$86,$06,$87,$01,$83,$00
	db	$ff
s33
	db	$c3
	db	$88,$06
	db	$8c,$08
	db	$90,$0b
	db	$9a,$0c
	db	$ff

courtyard1
	db	$83,$29,$29,$2c
	db	$ff,$00
courtyard3
	db	$83,$2a,$2a,$2a,$2a
	db	$ff,$00
courtyard4
	db	$80,$2b
	db	$ff,$00

s29
	db	$ca,$84,$6a
	db	$10,$13,$18,$13,$10,$13,$18,$13
	db	$11,$13,$18,$13,$11,$15,$1a,$1c
	db	$10,$13,$18,$13,$10,$13,$18,$13
	db	$11,$13,$18,$1a,$1d,$82,$1c,$81,$1d,$1c,$84,$1a,$18
	db	$ff
s2a
	db	$c1,$88
	db	$7a,$0c,$0c,$0c,$04,$05,$05,$05,$07
	db	$ff
s2b		 
	db	$c3,$84,$01,$c2
	db	$c3,$c2
	db	$c3,$c2
	db	$c3,$c2
	db	$c3,$c2
	db	$c3,$c2
	db	$c3,$c2
	db	$c3,$c2
	db	$ff
s2c
	db	$cc
	db	$88,$61,$18
	db	$84,$61,$18
	db	$8c,$61,$18
	db	$84,$64,$1c,$cd
	db	$cc,$88,$63,$18
	db	$84,$64,$15
	db	$8c,$61,$11
	db	$84,$61,$13,$cd

	db	$cc,$88,$63,$13
	db	$84,$61,$18
	db	$8c,$67,$13
	db	$84,$63,$13,$cd
	db	$cc,$84,$65,$11
	db	$84,$61,$11,$cd
	db	$cc,$8c,$61,$11
	db	$84,$61,$13,$cd

	db	$cc,$88,$63,$13
	db	$84,$61,$18
	db	$8c,$64,$1c
	db	$84,$63,$1f,$cd
	db	$cc,$88,$64,$21
	db	$84,$60,$21
	db	$cc,$8c,$65,$1d
	db	$84,$64,$21

	db	$9c,$63,$1f
	db	$84,$66,$1f
	db	$84,$64,$1c
	db	$88,$63,$18
	db	$84,$61,$1d,$cd
	db	$cc,$88,$63,$18
	db	$88,$64,$15
	
	db	$98,$63,$13
	db	$88,$64,$10
	db	$90,$61,$11
	db	$88,$64,$15
	db	$84,$61,$13,$cd
	db	$cc,$98,$63,$13
	db	$88,$64,$10
	db	$90,$61,$11
	db	$88,$64,$15
	db	$84,$64,$17,$cd
	db	$cc,$98,$63,$13
	db	$88,$64,$10
	db	$90,$61,$11
	db	$88,$64,$15
	db	$88,$64,$17
	db	$98,$64,$1c
	db	$84,$61,$18,$cd
	db	$cc,$88,$61,$1d
	db	$84,$65,$1d,$cd
	db	$cc,$88,$64,$21
	db	$88,$64,$23
	db	$ff

wolfboss1
	db	$8a,$25,$8f,$28
	db	$91,$25,$96,$28 
	db	$ff,$00
wolfboss3
	db	$8a,$26,$26,$8f,$26,$26
	db	$91,$26,$26,$8a,$26,$26 
	db	$ff,$00
wolfboss4
	db	$80,$27
	db	$ff,$00

s25
	db	$cb
	db	$84,$60,$18
	db	$62,$13
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$66,$13
	db	$62,$13
	db	$67,$13
	db	$60,$18
	db	$62,$13
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$62,$13
	db	$60,$18
	db	$65,$1b

	db	$60,$18
	db	$62,$13
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$66,$13
	db	$62,$13
	db	$67,$13
	db	$60,$18
	db	$65,$1b
	db	$62,$1f
	db	$63,$1d
	db	$65,$1b
	db	$60,$18
	db	$64,$18
	db	$7b,$18
	db	$ff
s26
	db	$c1
	db	$88,$7a,$00
	db	$88,$03
	db	$88,$02
	db	$84,$03
	db	$88,$00
	db	$88,$03
	db	$84,$00
	db	$88,$02
	db	$88,$03
	db	$ff
s27
	db	$c3,$88,$0d
	db	$88,$0c
	db	$88,$06
	db	$84,$0c
	db	$88,$0d
	db	$88,$0c
	db	$84,$0d
	db	$88,$06
	db	$88,$0c
	db	$ff
s28
	db	$cb
	db	$84,$60,$18
	db	$62,$13
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$66,$13
	db	$62,$13
	db	$67,$13
	db	$60,$18
	db	$62,$13
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$62,$13
	db	$60,$18
	db	$65,$1b

	db	$62,$1f
	db	$66,$1f
	db	$65,$1b
	db	$61,$1b
	db	$60,$18
	db	$67,$13
	db	$62,$13
	db	$66,$13
	db	$67,$13
	db	$62,$13
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$60,$0c
	db	$62,$07
	db	$65,$03
	db	$ff


icylake1
	db	$90,$1d,$8f,$1d,$90,$1d,$8f,$1d
	db	$90,$1d,$8f,$1d,$90,$1d,$8f,$1d
	db	$8f,$20
	db	$92,$21
	db	$90,$23,$23
	db	$ff,$00
icylake3
	db	$84,$1e,$83,$1e,$84,$1e,$83,$1e
	db	$84,$1e,$83,$1e,$84,$1e,$83,$1e
	db	$84,$1e,$83,$1e,$84,$1e,$83,$1e
	db	$84,$1e,$83,$1e,$84,$1e,$83,$1e
	db	$86,$22
	db	$84,$24,$83,$24,$84,$24,$83,$24
	db	$84,$24,$83,$24,$84,$24,$83,$24
	db	$ff,$00
icylake4
	db	$81,$1f
	db	$ff,$00

s1d
	db	$c8,$84
	db	$60,$00
	db	$65,$03
	db	$6c,$03
	db	$62,$07
	db	$60,$0c
	db	$62,$07
	db	$66,$07
	db	$62,$07
	db	$ff
s1e
	db	$c1
	db	$84,$73,$0c
	db	$82,$73,$0c
	db	$82,$73,$0c
	db	$84,$73,$13
	db	$84,$73,$13
	db	$84,$73,$18
	db	$84,$73,$18
	db	$84,$73,$07
	db	$84,$73,$07
	db	$ff
s1f
	db	$c3,$84,$02
	db	$c3,$81,$02,$e1
	db	$c3,$81,$02,$e1
	db	$c2,$84,$02
	db	$c2,$82,$02,$e2
	db	$c2,$84,$02
	db	$c2,$82,$02,$e2
	db	$c2,$84,$02
	db	$c2,$82,$02,$e2
	db	$ff
s20
	db	$c5,$88,$78,$0d
	db	$c5,$88,$78,$14
	db	$c0,$88,$60,$0d
	db	$c5,$88,$78,$14
	db	$c5,$88,$78,$13
	db	$c5,$88,$78,$0f
	db	$c5,$88,$78,$0e
	db	$c5,$88,$78,$0c

	db	$c5,$88,$78,$0d
	db	$c5,$88,$78,$14
	db	$c0,$88,$65,$10
	db	$c5,$88,$78,$14
	db	$c5,$88,$78,$13
	db	$c5,$88,$78,$19
	db	$c5,$88,$78,$18
	db	$c5,$88,$78,$13

	db	$c5,$88,$78,$14
	db	$c5,$88,$78,$0d
	db	$c0,$88,$62,$14
	db	$c5,$88,$78,$14
	db	$c5,$88,$78,$13
	db	$c5,$88,$78,$0f
	db	$c5,$88,$78,$0e
	db	$c5,$88,$78,$0c

	db	$c5,$88,$78,$0d
	db	$c5,$88,$78,$14
	db	$c0,$88,$60,$19
	db	$c5,$88,$78,$19
	db	$c5,$88,$78,$18
	db	$c5,$88,$78,$20
	db	$c5,$88,$78,$1f
	db	$c5,$84,$78,$24,$ca,$81,$18,$13,$0c,$07
	db	$ff
s21
	db	$c8,$84,$79
	db	$13,$12,$11,$10,$0f,$0e,$0d,$0c
	db	$0b,$0a,$09,$08,$07,$06,$05,$04
	db	$ff
s22
	db	$c1
	db	$84,$73,$18
	db	$82,$17
	db	$82,$17
	db	$84,$16
	db	$84,$15
	db	$84,$14
	db	$84,$13
	db	$84,$12
	db	$84,$11
	db	$84,$10
	db	$82,$0f
	db	$82,$0f
	db	$84,$0e
	db	$84,$0d
	db	$84,$0c
	db	$84,$0b
	db	$84,$0a
	db	$84,$09
	db	$ff
s23
	db	$c0,$84,$60,$0c
	db	$c8,$84,$6a,$13,$0f,$0e,$0c,$0e,$0f,$13
	db	$c0,$84,$60,$0b
	db	$c8,$84,$6a,$12,$0e,$0d,$0b,$0d,$0e,$12
	db	$c0,$84,$60,$0c
	db	$c8,$84,$6a,$13,$0f,$0e,$0c,$0e,$0f,$13
	db	$c0,$84,$60,$0b
	db	$c8,$84,$6a,$12,$0e,$0d,$0b,$12,$17,$1e
	db	$ff
s24
	db	$c1
	db	$84,$73,$0c
	db	$82,$13
	db	$82,$13
	db	$84,$18
	db	$84,$13
	db	$84,$0c
	db	$84,$13
	db	$84,$18
	db	$84,$13
	db	$ff

canyon1
	db	$90,$17,$17
	db	$90,$1a,$1a,$91,$1a,$1a
	db	$86,$1b,$1b,$87,$1b,$1b
	db	$94,$1a,$1a,$95,$1a,$1a
	db	$8a,$1b,$1b,$8b,$1b,$1b
	db	$ff,$00
canyon3
	db	$84,$18,$18
	db	$84,$18,$85,$18
	db	$92,$1c,$1c,$93,$1c,$1c
	db	$88,$18,$89,$18
	db	$96,$1c,$1c,$97,$1c,$1c
	db	$ff,$00
canyon4
	db	$81,$19,$19
	db	$81,$19,$19
	db	$81,$19,$19
	db	$81,$19,$19
	db	$ff,$00

s17
	db	$c7,$84
	db	$13,$0f,$18,$13,$14,$0c,$0f,$14
	db	$13,$0c,$18,$13,$14,$16,$11,$14
	db	$13,$0f,$18,$13,$14,$0c,$0f,$14
	db	$13,$0c,$18,$13,$1b,$82,$1a,$81,$1b,$1a,$84,$18,$16
	db	$ff	
s18
	db	$c1
	db	$88,$76,$0c
	db	$88,$77,$07
	db	$90,$76,$08
	db	$88,$76,$0c
	db	$88,$77,$07
	db	$90,$76,$08
	db	$88,$76,$0c
	db	$88,$77,$07
	db	$88,$76,$08
	db	$88,$77,$0a
	db	$88,$76,$0c
	db	$88,$77,$07
	db	$90,$76,$08
	db	$ff
s19
	db	$c2,$84
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$02,$01,$02,$01
	db	$ff
s1a
	db	$c8,$84,$66,$07
	db	$c8,$84,$62,$07
	db	$c8,$88,$65,$0f
	db	$c8,$84,$61,$08
	db	$c8,$84,$65,$08
	db	$c8,$88,$6e,$0f
	db	$c8,$84,$67,$07
	db	$c8,$84,$60,$0c
	db	$c8,$84,$62,$13

	db	$c5,$84,$6a,$07,$08
	db	$c7,$82,$1f,$1d,$1f,$1d,$1a,$1b
	db	$ff
s1b
	db	$c0,$84,$65,$0f
	db	$66,$13
	db	$62,$13
	db	$65,$0f
	db	$63,$11
	db	$67,$11
	db	$61,$16
	db	$63,$11
	db	$62,$13
	db	$60,$18
	db	$65,$1b
	db	$63,$1a
	db	$65,$1b
	db	$60,$18
	db	$66,$13
	db	$62,$13
	db	$ff
s1c
	db	$c1,$84,$77,$00
	db	$02
	db	$03
	db	$00
	db	$02
	db	$03
	db	$05
	db	$02
	db	$03
	db	$07
	db	$0c
	db	$0b
	db	$0c
	db	$07
	db	$02
	db	$03
	db	$ff


corridor1
	db	$a8,$11,$11
	db	$90,$14,$14
	db	$90,$16
	db	$ff,$00
corridor3
	db	$84,$12
	db	$84,$12,$12
	db	$84,$15
	db	$ff,$00
corridor4
	db	$80,$13,$13
	db	$80,$13,$13,$13,$13
	db	$80,$13,$13,$13,$13
	db	$ff,$00

s11
	db	$c5,$84,$6a,$0c,$0f,$88,$13,$0f
	db	$c5,$84,$6a,$0c,$0f,$88,$14,$0f
	db	$c5,$84,$6a,$0c,$0e,$88,$12,$0e
	db	$c5,$84,$6a,$07,$0e,$13,$11,$0e,$0f
	db	$ff
s12
	db	$c1
	db	$88,$70,$0c
	db	$88,$73,$0c
	db	$84,$71,$0e
	db	$84,$74,$0f
	db	$88,$70,$14
	db	$88,$73,$14
	db	$84,$71,$13
	db	$84,$74,$11
	db	$88,$70,$0e
	db	$88,$73,$0e
	db	$84,$71,$12
	db	$84,$74,$0e
	db	$88,$70,$13
	db	$88,$73,$07
	db	$84,$71,$0b
	db	$84,$74,$07

	db	$88,$70,$0c
	db	$88,$73,$0c
	db	$84,$71,$0e
	db	$84,$74,$0f
	db	$88,$70,$14
	db	$88,$73,$14
	db	$88,$71,$13
	db	$88,$70,$12
	db	$88,$73,$12
	db	$84,$71,$13
	db	$84,$74,$15
	db	$88,$70,$17
	db	$88,$73,$17
	db	$84,$71,$1a
	db	$84,$74,$18
	db	$ff
s13
	db	$c2,$84,$06
	db	$c6,$84,$05
	db	$c3,$84,$03
	db	$c6,$84,$05
	db	$c3,$88,$04
	db	$c2,$84,$06
	db	$c6,$84,$05
	db	$c3,$84,$03
	db	$c6,$84,$05
	db	$c3,$88,$04
	db	$c2,$84,$06
	db	$c6,$84,$05
	db	$c3,$84,$03
	db	$c6,$84,$05
	db	$c3,$88,$04
	db	$c2,$84,$06
	db	$c6,$84,$05
	db	$c3,$84,$03
	db	$c6,$84,$05
	db	$c3,$88,$04
	db	$ff
s14
	db	$c0
	db	$88,$60,$0c
	db	$84,$65,$0f
	db	$84,$60,$0c
	db	$84,$6c,$0f
	db	$84,$60,$0c
	db	$88,$61,$14
	db	$88,$6e,$0f
	db	$88,$63,$0f
	db	$88,$63,$09
	db	$88,$64,$12
	db	$88,$63,$09
	db	$92,$64,$0b,$c4,$81,$0a,$09,$08,$07,$06,$05

	db	$c0
	db	$88,$60,$0c
	db	$84,$65,$0f
	db	$84,$60,$0c
	db	$84,$6c,$0f
	db	$84,$60,$0c
	db	$88,$61,$14
	db	$88,$6e,$0f
	db	$88,$63,$0f
	db	$88,$64,$06
	db	$88,$64,$12
	db	$88,$6d,$12
	db	$90,$67,$0e
	db	$88,$63,$0e
	db	$ff
s15
	db	$c1
	db	$88,$70,$11
	db	$88,$73,$11
	db	$84,$71,$11
	db	$84,$74,$05
	db	$88,$70,$14
	db	$88,$73,$14
	db	$84,$71,$14
	db	$84,$74,$08
	db	$88,$70,$11
	db	$88,$73,$11
	db	$84,$71,$11
	db	$84,$74,$05
	db	$88,$70,$19
	db	$88,$73,$19
	db	$84,$71,$19
	db	$84,$74,$0d

	db	$88,$70,$11
	db	$88,$73,$11
	db	$84,$71,$11
	db	$84,$74,$05
	db	$88,$70,$14
	db	$88,$73,$14
	db	$84,$71,$14
	db	$84,$74,$08
	db	$88,$70,$11
	db	$88,$73,$11
	db	$84,$71,$11
	db	$84,$74,$05
	db	$88,$70,$19
	db	$88,$73,$19
	db	$84,$71,$19
	db	$84,$74,$0d

	db	$88,$70,$0e
	db	$88,$73,$0e
	db	$84,$71,$0e
	db	$84,$74,$02
	db	$88,$70,$11
	db	$88,$73,$11
	db	$84,$71,$14
	db	$84,$74,$11
	db	$88,$70,$13
	db	$88,$73,$13
	db	$84,$71,$11
	db	$84,$74,$0e
	db	$88,$70,$07
	db	$88,$73,$07
	db	$84,$71,$13
	db	$84,$74,$07
	db	$ff
s16
	db	$c0
	db	$88,$65,$08
	db	$84,$62,$0c
	db	$84,$60,$11
	db	$84,$61,$14
	db	$84,$65,$14
	db	$8c,$6d,$11

	db	$c5,$84,$6a,$20,$1d,$18

	db	$c0
	db	$84,$65,$08
	db	$84,$6c,$08
	db	$84,$62,$0c
	db	$84,$60,$11
	db	$84,$61,$14
	db	$84,$65,$14
	db	$8c,$6d,$11

	db	$c5,$84,$6a,$20,$1d,$18

	db	$c0
	db	$88,$65,$08
	db	$84,$62,$0c
	db	$84,$60,$11
	db	$84,$61,$14
	db	$84,$65,$14
	db	$8c,$6d,$11

	db	$c5,$84,$6a,$20,$1d,$18

	db	$c0
	db	$84,$65,$08
	db	$84,$6c,$08
	db	$84,$62,$0c
	db	$84,$60,$11
	db	$84,$61,$14
	db	$84,$65,$14
	db	$8c,$6d,$11

	db	$c5,$84,$6a,$20,$1d,$18

	db	$c0
	db	$88,$6d,$0e
	db	$84,$6d,$11
	db	$84,$6d,$14
	db	$84,$6d,$17
	db	$84,$6d,$1a
	db	$88,$6d,$1d
	db	$88,$6d,$1a
	db	$88,$6d,$17

	db	$88,$6f,$18
	db	$84,$6f,$18
	db	$84,$75,$18
	db	$84,$6f,$18
	db	$84,$75,$18
	db	$8a,$64,$17,$c4,$81,$16,$15,$14,$13,$12,$11

	db	$c5,$88,$6a,$2b
	db	$ff


stairway1
	db	$a8,$09,$09,$09,$09
	db	$90,$0c,$0c
	db	$90,$0d,$0d,$0d,$0d
	db	$90,$0c,$0c
	db	$84,$0f,$0f,$0f,$0f
	db	$ff,$00	
stairway3
	db	$84,$0a,$0a,$0a,$0a
	db	$84,$0a,$0a
	db	$84,$0e
	db	$84,$0a,$0a
	db	$84,$10,$10,$10,$10
	db	$ff,$00	
stairway4
	db	$80,$0b,$0b,$0b,$0b
	db	$80,$0b,$0b
	db	$80,$0b,$0b
	db	$80,$0b,$0b
	db	$ff,$00	

s09
	db	$c5,$88,$6a,$0c,$03,$0e,$05,$0f,$07,$0e,$05
	db	$ff
s0a
	db	$c1,$90,$72,$0c
	db	$90,$72,$13
	db	$88,$72,$18
	db	$84,$74,$17
	db	$84,$74,$14
	db	$84,$72,$13
	db	$84,$74,$11
	db	$84,$74,$0e
	db	$84,$74,$0f
	db	$ff
s0b
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$c3,$82,$02,$c6,$02,$02,$02
	db	$ff
s0c
	db	$c0,$82,$65,$0f
	db	$82,$66,$13
	db	$82,$62,$13
	db	$82,$66,$13
	db	$88,$65,$0f

	db	$82,$67,$0e
	db	$82,$61,$13
	db	$82,$6b,$13
	db	$82,$61,$13
	db	$84,$67,$0e
	db	$84,$63,$0e

	db	$82,$65,$0f
	db	$82,$66,$13
	db	$82,$62,$13
	db	$82,$66,$13
	db	$84,$65,$0f
	db	$84,$66,$13

	db	$84,$62,$13
	db	$86,$65,$1b
	db	$86,$63,$1a
	db	$ff
s0d
	db	$c0,$84,$65,$16
	db	$65,$0a
	db	$61,$16
	db	$65,$0a
	db	$64,$13
	db	$65,$0a
	db	$60,$13
	db	$65,$0a
	db	$ff
s0e
	db	$c1,$88,$72,$13
	db	$73,$11
	db	$72,$0f
	db	$73,$0e
	db	$72,$18
	db	$73,$16
	db	$72,$14
	db	$73,$13
	db	$72,$1f
	db	$73,$1d
	db	$72,$1b
	db	$73,$1a
	db	$72,$18
	db	$73,$16
	db	$72,$14
	db	$73,$13
	db	$ff
s0f
	db	$c0,$84,$68,$0f,$13,$0f,$13,$0e,$12,$0e,$12,$0f,$13,$0f,$13
	db	$c0,$84,$12,$17,$12,$17
	db	$ff
s10
	db	$c1,$88,$72,$0c
	db	$88,$72,$0f
	db	$88,$72,$12
	db	$88,$72,$17
	db	$88,$72,$18
	db	$88,$72,$13
	db	$88,$72,$12
	db	$84,$72,$0e
	db	$84,$74,$0f
	db	$ff



dungeon1		
	db	$8a,$01,$01
	db	$8d,$04,$04
	db	$8c,$05
	db	$8c,$06
	db	$ff,$00
dungeon3		
	db	$8a,$02,$02
	db	$8b,$02
	db	$8a,$02
	db	$8c,$07
	db	$ff,$00
dungeon4		
	db	$80,$03,$03,$03,$03
	db	$80,$03,$03
	db	$80,$03,$03
	db	$80,$08
	db	$ff,$00

s01
	db	$c0,$84,$68
	db	$02,$0e,$0c,$0e,$02,$0e,$0c,$10
	db	$02,$0e,$0c,$0e,$02,$10,$0e,$11
	db	$02,$0e,$0c,$0e,$02,$0e,$0c,$10
	db	$02,$0e,$0c,$0e,$02,$69,$08,$09,$0e
	db	$ff	   
s02
	db	$c1
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$84,$72,$02
	db	$84,$73,$04
	db	$84,$72,$05
	db	$84,$73,$09
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$88,$70,$02
	db	$84,$72,$02
	db	$84,$73,$08
	db	$84,$72,$09
	db	$84,$73,$0e
	db	$ff
s03
	db	$c2,$84,$05
	db	$c2,$84,$06
	db	$c2,$84,$05
	db	$c2,$84,$06
	db	$c3,$88,$0a
	db	$c2,$84,$05
	db	$c2,$84,$06
	db	$c2,$82,$05
	db	$c2,$82,$05
	db	$c2,$84,$06
	db	$c2,$84,$05
	db	$c2,$84,$06
	db	$c3,$84,$0a
	db	$c2,$84,$05
	db	$c3,$84,$0b
	db	$c2,$84,$06
	db	$ff		
s04
	db	$c0,$88,$65,$0f
	db	$88,$60,$18
	db	$88,$62,$13
	db	$88,$60,$18
	db	$88,$63,$11
	db	$88,$65,$16
	db	$84,$64,$13
	db	$82,$63,$11,$81,$64,$13,$63,$11
	db	$84,$65,$0f
	db	$84,$64,$0e
	db	$ff
s05
	db	$c0
	db	$84,$65,$0f
	db	$84,$65,$1b
	db	$84,$64,$1a
	db	$84,$64,$18
	db	$84,$63,$16
	db	$84,$64,$18
	db	$84,$63,$16
	db	$84,$65,$14
	db	$84,$64,$13
	db	$84,$65,$14
	db	$84,$63,$16
	db	$84,$65,$14
	db	$84,$64,$13
	db	$82,$63,$11
	db	$81,$64,$13
	db	$81,$63,$11
	db	$84,$65,$0f
	db	$84,$64,$0e

	db	$84,$65,$0f
	db	$84,$65,$1b
	db	$84,$64,$1a
	db	$84,$64,$18
	db	$84,$63,$16
	db	$84,$64,$18
	db	$84,$63,$16
	db	$84,$65,$14
	db	$84,$64,$13
	db	$84,$65,$14
	db	$84,$63,$16
	db	$84,$65,$14
	db	$84,$64,$13
	db	$82,$63,$11
	db	$81,$65,$0f
	db	$81,$63,$11
	db	$84,$64,$13
	db	$84,$65,$14
	db	$ff
s06
	db	$c0
	db	$9c,$64,$17
	db	$82,$64,$16,$17
	db	$9c,$64,$18
	db	$82,$64,$17,$16
	db	$88,$64,$17
	db	$88,$61,$13
	db	$88,$63,$1a
	db	$88,$64,$17
	db	$88,$61,$1f
	db	$88,$63,$1a
	db	$8a,$64,$23
	db	$c4,$81,$64,$22,$21,$20,$1f,$1e,$1d
	db	$ff
s07
	db	$c1
	db	$84,$73,$07
	db	$73,$07
	db	$72,$0b
	db	$72,$0b
	db	$72,$0e
	db	$72,$0e
	db	$72,$07
	db	$72,$06
	db	$73,$05
	db	$73,$05
	db	$72,$08
	db	$72,$08
	db	$72,$0c
	db	$72,$0c
	db	$72,$05
	db	$72,$06
	db	$73,$07
	db	$73,$07
	db	$72,$0b
	db	$72,$0b
	db	$72,$0e
	db	$72,$0e
	db	$72,$0f
	db	$72,$0e
	db	$73,$13
	db	$73,$14
	db	$72,$13
	db	$72,$11
	db	$73,$13
	db	$72,$11
	db	$73,$0e
	db	$72,$0f
	db	$ff
s08
	db	$c3,$84,$0f
	db	$c3,$84,$0e
	db	$c2,$88,$05
	db	$c2,$88,$05
	db	$c2,$88,$05
	db	$c3,$84,$0d
	db	$c3,$84,$0c
	db	$c2,$88,$05
	db	$c2,$88,$05
	db	$c2,$88,$05
	db	$c3,$84,$0b
	db	$c3,$84,$0a
	db	$c2,$88,$05
	db	$c2,$88,$05
	db	$c2,$88,$05
	db	$c3,$84,$09
	db	$c3,$84,$08
	db	$c2,$88,$05
	db	$c3,$88,$07
	db	$c3,$88,$06
	db	$ff


clearsfx
	db	$00,$fe
s00
	db	$e1,$ff

;-----------------------------------------------------------------------------
	org	$fffa
	dw	nmi
	dw	resetvector
	dw	irq


