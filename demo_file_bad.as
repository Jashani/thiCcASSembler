; file demo_file.as
.entry LIST
.entry mov
.extern W
.intern WHATAMI
MAIN: add r3, LIST
LOOP: prn #48, #49
macro m1
	inc r6
	mov r3, W
endm
	lea STR, r6
	m1
; macro lea
;	inc r15
; endm
	sub r1, r4
	bne END
	cmp val1, #-6
	bne END[r15]
	dec K
	heh W
.entry MAIN
	sub LOOP[r10] ,r14
IDONTEXIST: inc r1
END: stop
STR: .string "abcd"
LIST: .data 6, -9
	.data -100, hello
.entry K
K: .data 31
.extern val1