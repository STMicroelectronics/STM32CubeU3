echo regression script started
call ../env.bat

set secbootadd0=0x1800C0
set flashsectnbr=0x7F

set connect=-c port=SWD mode=UR
set connect_no_reset=-c port=SWD mode=HotPlug

set rdp_0=-ob RDP=0xAA TZEN=1 UNLOCK_1A=1 UNLOCK_1B=1 UNLOCK_2A=1 UNLOCK_2B=1
set remove_bank1_protect=-ob SECWM1_PSTRT=%flashsectnbr% SECWM1_PEND=0 WRP1A_PSTRT=%flashsectnbr% WRP1A_PEND=0 WRP1B_PSTRT=%flashsectnbr% WRP1B_PEND=0
set remove_bank2_protect=-ob SECWM2_PSTRT=%flashsectnbr% SECWM2_PEND=0 WRP2A_PSTRT=%flashsectnbr% WRP2A_PEND=0 WRP2B_PSTRT=%flashsectnbr% WRP2B_PEND=0
set erase_all=-e all
set remove_hdp_protection=-ob HDP1_PEND=0 HDP1EN=0xB4 HDP2_PEND=0 HDP2EN=0xB4
set remove_boot_lock=-ob BOOT_LOCK=0
set default_ob1=-ob SRAM2_RST=0 NSBOOTADD0=0x100000 NSBOOTADD1=0x17F1E0 BOOT_LOCK=1 SECBOOTADD0=%secbootadd0% DBANK=1 SWAP_BANK=0 nSWBOOT0=1 SECWM1_PSTRT=0 SECWM1_PEND=%flashsectnbr%
set default_ob2=-ob SECWM2_PSTRT=0 SECWM2_PEND=%flashsectnbr%
:: Get OEM2 key
set "oem2_password=./Keys/oem2_password.txt"
:: Check if oem2_password.txt exist
If not exist %oem2_password% (
echo The file oem2_password.txt does not exist.
goto :error
)
:: Get OEM2 key
set /p oem2_key=<"%oem2_password%"

echo Regression to RDP 0, enable tz
:: Unlock RDP 2 to switch in RDP 1
%stm32programmercli% %connect% -hardRst -unlockRDP2 %oem2_key%
echo Please unplug USB cable and plug it again to recover SWD Connection.
echo Press any key to continue...
echo.
IF [%1] neq [AUTO] pause >nul

:: Switch RDP 1 to RDP 0
%stm32programmercli% %connect_no_reset% %rdp_0%
IF %errorlevel% NEQ 0 %stm32programmercli% %connect% %rdp_0%
IF %errorlevel% NEQ 0 goto :error

echo Remove bank1 protection
%stm32programmercli% %connect% %remove_bank1_protect%
IF %errorlevel% NEQ 0 goto :error

echo Remove bank2 protection and erase all
%stm32programmercli% %connect_no_reset% %remove_bank2_protect% %erase_all%
IF %errorlevel% NEQ 0 goto :error

echo Remove hdp protection
%stm32programmercli% %connect_no_reset% %remove_hdp_protection%
IF %errorlevel% NEQ 0 goto :error

echo Set default OB 1 (dual bank, swap bank, sram2 reset, secure entry point, bank 1 full secure)
%stm32programmercli% %connect_no_reset% %remove_boot_lock%
%stm32programmercli% %connect_no_reset% %default_ob1%
IF %errorlevel% NEQ 0 goto :error

echo Set default OB 2 (bank 2 full secure)
%stm32programmercli% %connect_no_reset% %default_ob2%
IF %errorlevel% NEQ 0 goto :error

echo regression script done, press key
IF [%1] NEQ [AUTO] pause >nul
exit 0

:error
echo regression script failed, press key
IF [%1] NEQ [AUTO] pause >nul
exit 1
