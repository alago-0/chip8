Core structure - registers, ram, etc

ram - 4096KB - from 0x000 (0) to 0xFFF (4095) - DONE
v - 16 general purpose 8-bit registers - Vx, where x is hex (0 through F) - DONE
i - 16-bit index register - points at locations in memory - DONE
delay_timer - 8-bit register - DONE
sound_timer - 8-bit register - DONE
pc - program counter - 16-bit register - stores the currently executing address - DONE
sp - stack pointer - 8-bit register - points to the topmost level of the stack - DONE
stack - array of 16 16-bit values - stores the address that the interpreter should return to when finished with a subroutine - DONE
