call ../env.bat
:: Get config updated by OEMuROT_Boot
call ./img_config.bat

set rot_provisioning_path=%rot_provisioning_path:"=%
:: Enable delayed expansion
setlocal EnableDelayedExpansion

set bootpath=OEMiRoT_OEMuRoT

:: Data updated with the postbuild of OEMuROT-Boot
set sec1_start=0x0
set sec1_end=0x34
set sec2_start=0x7F
set sec2_end=0x0
set wrp1a_start=0x4
set wrp1a_end=0x15
set wrp1b_start=0x1A
set wrp1b_end=0x1B
set wrp2a_start=0x7A
set wrp2a_end=0x7F
set wrp2b_start=
set wrp2b_end=
set hdp1_start=
set hdp1_end=0x15
set hdp2_start=0x7f
set hdp2_end=0x15
set boot_lck=
set secbootadd0=0x1800C0
set flashsectnbr=0x7F

set slot0=0xC02C000
set slot1=0xC035000
set slot2=0xC067000
set slot3=0xC070000
set slot4=0x0
set slot5=0x0
set slot6=0x0
set slot7=0x0

set keyaddress=0xC01A000
set bootaddress=0xC006000
set oemirot_oemurot_slot0=0xC01C000

set s_code_image=%oemurot_appli_secure%
set ns_code_image=%oemurot_appli_non_secure%
set one_code_image=%oemurot_appli_assembly_sign%
set s_data_image=s_data_init_sign.bin
set ns_data_image=ns_data_init_sign.bin

:: STM32CubeProgammer connection
set connect_no_reset=-c port=SWD mode=Hotplug
set connect=-c port=SWD mode=UR

:: Get config updated by OEMiROT_Boot
set appli_dir=..\..\%oemirot_appli_path_project%
set boot_dir="..\..\%oemirot_boot_path_project%"

:: =============================================== Remove protections and initialize Option Bytes ==========================================
set remove_protect_init=SRAM2_RST=0
set remove_bank1_protect=-ob SECWM1_PSTRT=%flashsectnbr% SECWM1_PEND=0 WRP1A_PSTRT=%flashsectnbr% WRP1A_PEND=0 WRP1B_PSTRT=%flashsectnbr% WRP1B_PEND=0 %remove_protect_init%
set remove_bank2_protect=-ob SECWM2_PSTRT=%flashsectnbr% SECWM2_PEND=0 WRP2A_PSTRT=%flashsectnbr% WRP2A_PEND=0 WRP2B_PSTRT=%flashsectnbr% WRP2B_PEND=0
set erase_all=-e all
set remove_hdp_protection=-ob HDP1_PEND=0 HDP1EN=0xB4 HDP2_PEND=0 HDP2EN=0xB4
set remove_boot_lock=-ob BOOT_LOCK=0

:: =============================================================== Hardening ===============================================================
set hide_protect_1=HDP1_PEND=%hdp1_end% HDP1EN=0x1
set hide_protect_2=HDP2_PEND=%hdp2_end% HDP2EN=0x1
set boot_write_protect=WRP1A_PSTRT=%wrp1a_start% WRP1A_PEND=%wrp1a_end%
set sec_water_mark=SECWM1_PSTRT=%sec1_start% SECWM1_PEND=%sec1_end% SECWM2_PSTRT=%sec2_start% SECWM2_PEND=%sec2_end%
set boot_address=-ob SECBOOTADD0=%secbootadd0%
set boot_lock=BOOT_LOCK=1

:: =============================================== Configure Option Bytes ==================================================================
set "action=Set TZEN = 1"
echo %action%
:: Trust zone enabled is mandatory in order to execute OEM-iRoT
%stm32programmercli% %connect_no_reset% -ob TZEN=1
IF !errorlevel! NEQ 0 goto :error

set "action=Remove hdp protection"
echo %action%
%stm32programmercli% %connect_no_reset% %remove_hdp_protection%
IF !errorlevel! NEQ 0 goto :error

set "action=Remove Protection and erase All"
echo %action%
%stm32programmercli% %connect_no_reset% %remove_bank1_protect%
IF !errorlevel! NEQ 0 goto :error
%stm32programmercli% %connect_no_reset% %remove_bank2_protect% %erase_all%
IF !errorlevel! NEQ 0 goto :error

set "action=Remove Boot lock"
echo %action%
%stm32programmercli% %connect_no_reset% %remove_boot_lock%
IF !errorlevel! NEQ 0 goto :error

set "action=Set boot address @%secbootadd0%"
echo %action%
%stm32programmercli% %connect_no_reset% %boot_address%
IF !errorlevel! NEQ 0 goto :error

:: =============================================== Download images =========================================================================
echo "Application images programming in download slots"

IF  "%app_image_number%" == "2" (
set "action=Write Appli Secure"
echo %action%
%stm32programmercli% %connect_no_reset% -d %appli_dir%\Binary\%s_code_image% %slot0% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "TZ Appli Secure Written"
set "action=Write Appli NonSecure"
echo %action%
%stm32programmercli% %connect_no_reset% -d %appli_dir%\Binary\%ns_code_image% %slot1% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "TZ Appli NonSecure Written"
)

IF  "%app_image_number%" == "1" (
IF  "%app_full_secure%" == "1" (
set "action=Write Appli Full Secure"
echo %action%
%stm32programmercli% %connect_no_reset% -d %appli_dir%\Binary\%s_code_image% %slot0% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "Appli Full Secure Written"
) else (
set "action=Write One image Appli"
echo %action%
%stm32programmercli% %connect_no_reset% -d %appli_dir%\Binary\%one_code_image% %slot0% -v --skipErase
IF !errorlevel! NEQ 0 goto :error

echo "TZ Appli Written"
)
)

IF  "%s_data_image_number%" == "1" (
set "action=Write Secure Data"
echo %action%
IF not exist %rot_provisioning_path%\%bootpath%\Binary\%s_data_image% (
@echo [31mError: %s_data_image% does not exist! use TPC to generate it[0m
goto :error
)

%stm32programmercli% %connect_no_reset% -d %rot_provisioning_path%\%bootpath%\Binary\%s_data_image% %slot4% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
)

IF  "%ns_data_image_number%" == "1" (
set "action=Write non Secure Data"
echo %action%
IF not exist %rot_provisioning_path%\%bootpath%\Binary\%ns_data_image% (
@echo [31mError: %ns_data_image% does not exist! use TPC to generate it[0m
goto :error
)

%stm32programmercli% %connect_no_reset% -d %rot_provisioning_path%\%bootpath%\Binary\%ns_data_image% %slot5% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
)

set "action=Write Binary Keys of first boot Stage"
echo %action%
%stm32programmercli% %connect_no_reset% -d %rot_provisioning_path%\%bootpath%\Binary\OEMiRoT_Keys.bin 0xC004000 -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "Binary Keys Written"

set "action=Write the first boot stage OEMiROT_Boot"
echo %action%
%stm32programmercli% %connect_no_reset% -d %rot_provisioning_path%\OEMiRoT_OEMuRoT\Binary\OEMiROT_Boot.bin 0xC006000 -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "OEMiROT_Boot Written"

set "action=Write Binary Keys of the second boot stage"
echo %action%
%stm32programmercli% %connect_no_reset% -d %rot_provisioning_path%\OEMiRoT_OEMuRoT\Binary\OEMuRoT_Keys.bin %keyaddress% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "Binary Keys Written"

set "action=Write Write the second boot stage OEMuROT_Boot"
echo %action%
%stm32programmercli% %connect_no_reset% -d %boot_dir%\Binary\OEMuROT_Boot_init_sign.bin %oemirot_oemurot_slot0% -v --skipErase
IF !errorlevel! NEQ 0 goto :error
echo "OEMuROT_Boot Written"

:: ======================================================= Extra board protections =========================================================
set "action=Configure Option Bytes"
echo %action%
echo "Configure Secure option Bytes: Write Protection, Hide Protection and boot lock"
%stm32programmercli% %connect_no_reset% -ob %sec_water_mark% %boot_write_protect% %hide_protect_1% %boot_lock%
IF !errorlevel! NEQ 0 goto :error

echo Programming success
IF [%1] NEQ [AUTO] cmd /k
exit 0

:error
echo      Error when trying to "%action%" >CON
echo      Programming aborted >CON
echo.
IF [%1] NEQ [AUTO] cmd /k
exit 1
