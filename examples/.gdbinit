target extended-remote :3333
set remote hardware-breakpoint-limit 6
set remote hardware-watchpoint-limit 4
mon stm32f4x.cpu arm semihosting enable

define reload-code
	mon reset init
	load
	set $pc=_start
	continue
end

define reset_context
	set $r0=(*(uint32_t*)($sp))
	set $r1=(*(uint32_t*)($sp+4))
	set $r2=(*(uint32_t*)($sp+8))
	set $r3=(*(uint32_t*)($sp+12))
	set $r12=(*(uint32_t*)($sp+16))
	set $lr=(*(uint32_t*)($sp+20))
	set $pc=(*(uint32_t*)($sp+24))

	set $sp = $sp+28
end
