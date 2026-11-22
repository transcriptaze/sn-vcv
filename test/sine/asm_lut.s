	.build_version macos, 11, 0	sdk_version 27, 2
	.file	1 "sn-vcv/test/sine" "benchmarks/asm_lut.cpp"
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z10sincos_lutjRfS_            ; -- Begin function _Z10sincos_lutjRfS_
	.p2align	2
__Z10sincos_lutjRfS_:                   ; @_Z10sincos_lutjRfS_
Lfunc_begin0:
	.cfi_startproc
; %bb.0:
	;DEBUG_VALUE: sincos_lut:phase <- $w0
	;DEBUG_VALUE: sincos_lut:s <- $x1
	;DEBUG_VALUE: sincos_lut:c <- $x2
	.loc	1 14 33 prologue_end            ; benchmarks/asm_lut.cpp:14:33
	add	w8, w0, #256, lsl #12           ; =1048576
	.loc	1 14 56 is_stmt 0               ; benchmarks/asm_lut.cpp:14:56
	lsr	w8, w8, #21
Ltmp0:
	;DEBUG_VALUE: sincos_lut:idx <- $w8
	.loc	1 15 57 is_stmt 1               ; benchmarks/asm_lut.cpp:15:57
	ubfiz	w9, w0, #2, #21
	.loc	1 15 63 is_stmt 0               ; benchmarks/asm_lut.cpp:15:63
	eor	w9, w9, #0x3fc00000
Ltmp1:
	;DEBUG_VALUE: sincos_lut:m <- $w9
	.loc	1 18 5 is_stmt 1                ; benchmarks/asm_lut.cpp:18:5
	fmov	s0, w9
Ltmp2:
	;DEBUG_VALUE: sincos_lut:m <- $s0
	;DEBUG_VALUE: sincos_lut:f <- $s0
	.loc	1 0 5 is_stmt 0                 ; benchmarks/asm_lut.cpp:0:5
	mov	w9, #4059                       ; =0xfdb
	movk	w9, #15177, lsl #16
	fmov	s1, w9
	mov	w9, #52196                      ; =0xcbe4
	movk	w9, #48022, lsl #16
	fmov	s2, w9
	.loc	1 19 32 is_stmt 1               ; benchmarks/asm_lut.cpp:19:32
	fmadd	s0, s0, s1, s2
Ltmp3:
	;DEBUG_VALUE: sincos_lut:d <- $s0
	.loc	1 21 18                         ; benchmarks/asm_lut.cpp:21:18
Lloh0:
	adrp	x9, _tab@GOTPAGE
Lloh1:
	ldr	x9, [x9, _tab@GOTPAGEOFF]
	add	x8, x9, w8, uxtw #3
Ltmp4:
	ldp	s1, s2, [x8]
Ltmp5:
	;DEBUG_VALUE: sincos_lut:e <- [DW_OP_LLVM_fragment 0 32] $s1
	;DEBUG_VALUE: sincos_lut:e <- [DW_OP_LLVM_fragment 32 32] $s2
	.loc	1 22 13                         ; benchmarks/asm_lut.cpp:22:13
	fmadd	s3, s2, s0, s1
	.loc	1 22 7 is_stmt 0                ; benchmarks/asm_lut.cpp:22:7
	str	s3, [x1]
	.loc	1 23 13 is_stmt 1               ; benchmarks/asm_lut.cpp:23:13
	fmsub	s0, s1, s0, s2
Ltmp6:
	.loc	1 23 7 is_stmt 0                ; benchmarks/asm_lut.cpp:23:7
	str	s0, [x2]
	.loc	1 24 1 is_stmt 1                ; benchmarks/asm_lut.cpp:24:1
	ret
Ltmp7:
	.loh AdrpLdrGot	Lloh0, Lloh1
Lfunc_end0:
	.cfi_endproc
                                        ; -- End function
