_start:
	ldr sp, =_stack
	bl init
	bl main
end:
	b end

.global _start
