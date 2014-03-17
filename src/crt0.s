Reset_Handler:
_start:
	ldr r0, =_stack
	mov sp, r0
	bl init
end:
	b end

.global _start
.global Reset_Handler
