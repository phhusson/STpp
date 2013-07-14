_start:
	ldr r0, =_stack
	mov sp, r0
	bl init
	bl main
end:
	b end

.global _start
