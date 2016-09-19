# # J-LINK GDB SERVER initialization 
# # This connects to a GDB Server listening 
# for commands on localhost at tcp port 2331 

set print pretty
set debug remote 0
set debug serial 0

## Connect to the remote target via a TCP socket on host:port.

define armtarget
	target remote localhost:2331
	monitor speed auto
	monitor endian little 
	monitor flash device STM32F405RG
	monitor flash download = 1
	monitor flash breakpoints = 1
	monitor SWO GetSpeedInfo
	monitor SWO EnableTarget 168000000 6000000 0x03 0
	monitor reset
end

define reset
       monitor reset
end

define res
       monitor reset
end

armtarget

## Set a break point at the beginning of main().
echo (gdb) break main\n
break main

