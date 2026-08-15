

mcro two_adds
	add $1, $2, $3
	add $4, $5, $6
mcroend

mcro single
	hlt
mcroend

MAIN:	sub $7, $8, $9
	two_adds
	single	
END: hlt

.entry MAIN
