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
