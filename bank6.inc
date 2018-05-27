
		ORG $8000, $00

		INCLUDE	"sound.inc"
;		INCLUDE	"sound_rc3.inc"

		ECHO "sound data size", [.-$8000]
		
sprites_start	EQU	.

		INCLUDE "sprites.inc"

		ECHO "sprites data size", [.-sprites_start]

		ECHO "bank 6 free space", [SAFE_STUB_ORG-.]

		SAFE_STUB