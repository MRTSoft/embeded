@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\common\bin\cspybat" "C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\avr\bin\avrproc.dll" "C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\avr\bin\avrJtagIce.dll"  %1 --plugin "C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\avr\bin\avrlibsupportbat.dll" --backend -B "--cpu=m16" "--enhanced_core" "-p" "C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.0\avr\Config\iom16.ddf" "--eeprom_size" "512" "-d" "jtagIce" "--drv_communication=COM11" "--jtagice_clock=100000" "--jtagice_leave_timers_running" "--drv_set_putchar_breakpoint" "--drv_set_getchar_breakpoint" 


