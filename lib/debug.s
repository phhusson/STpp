.global putchar
putchar:
	ldr       r1, =putchar_temp
	strb      r0, [r1]
	mov       r0, #3
	bkpt 0xab
	bx lr

.global putchar_temp
putchar_temp:
	.word 0x00

.global putstring
putstring:
	mov r1, r0
	mov r0, #4
	bkpt 0xab
	bx lr

.global getchar
getchar:
	mov r1, #0
	mov r0, #7
	bkpt 0xab
	bx lr
