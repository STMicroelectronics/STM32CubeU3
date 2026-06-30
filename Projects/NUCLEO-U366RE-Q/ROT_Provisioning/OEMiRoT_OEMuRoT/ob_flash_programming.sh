#!/bin/bash
if [ $# -ge 1 ]; then script_mode=$1; else script_mode=MANUAL; fi
param2=$2
rdp_lev=$param2

source ../env.sh
# Get config updated by OEMuROT_Boot
source img_config.sh

# Error when external script is executed.
error()
{
  echo "        Error when trying to "$action #> "ob_flash_programming.log"
  echo "        Programming aborted" #>> "ob_flash_programming.log"
  if [ "$script_mode" != "AUTO" ]; then $SHELL; fi
  return 1
}

bootpath=OEMiRoT_OEMuRoT

# Data updated with the postbuild of OEMuROT-Boot
sec1_start=0x0
sec1_end=0x7F
sec2_start=0x0
sec2_end=0x21
wrp1a_start=0x4
wrp1a_end=0x15
wrp1b_start=0x2A
wrp1b_end=0x2B
wrp2a_start=0x7A
wrp2a_end=0x7F
wrp2b_start=
wrp2b_end=
hdp1_start=
hdp1_end=0x15
hdp2_start=0x7f
hdp2_end=0x15
boot_lck=
secbootadd0=0x1800C0
flashsectnbr=0x7F

slot0=0xC042000
slot1=0x0
slot2=0xC0A2000
slot3=0x0
slot4=0xC03E000
slot5=0x0
slot6=0xC102000
slot7=0x0

keyaddress=0xC02A000
bootaddress=0xC006000
oemirot_oemurot_slot0=0xC02C000

s_code_image=$oemurot_appli_secure
ns_code_image=$oemurot_appli_non_secure
s_data_image=s_data_init_sign.bin
ns_data_image=ns_data_init_sign.bin

# STM32CubeProgammer connection
connect_no_reset="-c port=SWD mode=Hotplug"
connect="-c port=SWD mode=UR"

# Get config updated by OEMiROT_Boot
appli_dir="../../$oemirot_appli_path_project"
boot_dir="../../$oemirot_boot_path_project"

# =============================================== Remove protections and initialize Option Bytes ==========================================
remove_protect_init="SRAM2_RST=0"
remove_bank1_protect="-ob SECWM1_PSTRT=$flashsectnbr SECWM1_PEND=0 WRP1A_PSTRT=$flashsectnbr WRP1A_PEND=0 WRP1B_PSTRT=$flashsectnbr WRP1B_PEND=0 $remove_protect_init"
remove_bank2_protect="-ob SECWM2_PSTRT=$flashsectnbr SECWM2_PEND=0 WRP2A_PSTRT=$flashsectnbr WRP2A_PEND=0 WRP2B_PSTRT=$flashsectnbr WRP2B_PEND=0"
erase_all="-e all"
remove_hdp_protection="-ob HDP1_PEND=0 HDP1EN=0xB4 HDP2_PEND=0 HDP2EN=0xB4"
remove_boot_lock="-ob BOOT_LOCK=0"
wrp_protect=
if [ "$rdp_lev" -ge 1 ]; then
	wrp_protect="UNLOCK_1A=0 UNLOCK_1B=0 UNLOCK_2A=0 UNLOCK_2B=0"
fi

# =============================================================== Hardening ===============================================================
hide_protect_1="HDP1_PEND=$hdp1_end HDP1EN=0x1"
hide_protect_2="HDP2_PEND=$hdp2_end HDP2EN=0x1"
boot_write_protect="WRP1A_PSTRT=$wrp1a_start WRP1A_PEND=$wrp1a_end"
sec_water_mark=SECWM1_PSTRT="$sec1_start SECWM1_PEND=$sec1_end SECWM2_PSTRT=$sec2_start SECWM2_PEND=$sec2_end"
boot_address="-ob SECBOOTADD0=$secbootadd0 NSBOOTADD0=$secbootadd0 NSBOOTADD1=$secbootadd0"
boot_lock="BOOT_LOCK=1"

# =============================================== Configure Option Bytes ==================================================================
action="Set TZEN = 1"
echo "$action"
# Trust zone enabled is mandatory in order to execute OEM-uRoT
"$stm32programmercli" $connect_no_reset -ob TZEN=1
if [ $? -ne 0 ]; then error; return 1; fi

action="Remove hdp protection"
echo "$action"
"$stm32programmercli" $connect_no_reset $remove_hdp_protection
if [ $? -ne 0 ]; then error; return 1; fi

action="Remove Protection and erase All"
echo "$action"
"$stm32programmercli" $connect_no_reset $remove_bank1_protect
if [ $? -ne 0 ]; then error; return 1; fi
"$stm32programmercli" $connect_no_reset $remove_bank2_protect $erase_all
if [ $? -ne 0 ]; then error; return 1; fi

action="Remove Boot lock"
echo "$action"
"$stm32programmercli" $connect_no_reset $remove_boot_lock
if [ $? -ne 0 ]; then error; return 1; fi

set "action=Set boot address @$secbootadd0"
echo "$action"
"$stm32programmercli" $connect_no_reset $boot_address
if [ $? -ne 0 ]; then error; return 1; fi

# =============================================== Download images =========================================================================
echo "Application images programming in download slots"

if [ "$app_image_number" == "2" ]; then
    action="Write Appli Secure"
    echo "$action $appli_dir/Binary/$s_code_image"
    "$stm32programmercli" $connect_no_reset -d "$appli_dir/Binary/$s_code_image" $slot0 -v --skipErase
    if [ $? -ne 0 ]; then error; return 1; fi
    echo "TZ Appli Secure Written"
    action="Write Appli NonSecure"
    echo "$action"
    "$stm32programmercli" $connect_no_reset -d "$appli_dir/Binary/$ns_code_image" $slot1 -v --skipErase
    if [ $? -ne 0 ]; then error; return 1; fi
    echo "TZ Appli NonSecure Written"
fi

if [ "$app_image_number" == "1" ]; then
    if [ "$app_full_secure" == "1" ]; then
        action="Write Appli Full Secure"
        echo "$action $appli_dir/Binary/$s_code_image"
        "$stm32programmercli" $connect_no_reset -d "$appli_dir/Binary/$s_code_image" $slot0 -v --skipErase
        if [ $? -ne 0 ]; then error; return 1; fi
        echo "Appli Full Secure Written"
    fi
fi

if [ "$s_data_image_number" == "1" ]; then
    action="Write Secure Data"
    echo "$action"
    if [ ! -f "$rot_provisioning_path/$bootpath/Binary/$s_data_image" ]; then
        echo "Error: $s_data_image does not exist! use TPC to generate it"
        error
    fi

    "$stm32programmercli" $connect_no_reset -d "$rot_provisioning_path/$bootpath/Binary/$s_data_image" $slot4 -v --skipErase
    if [ $? -ne 0 ]; then error; return 1; fi
fi

if [ "$ns_data_image_number" == "1" ]; then
    action="Write non Secure Data"
    echo "$action"
    if [ ! -f "$rot_provisioning_path/$bootpath/Binary/$ns_data_image" ]; then
        echo "Error: $ns_data_image does not exist! use TPC to generate it"
        error
    fi

    "$stm32programmercli" $connect_no_reset -d "$rot_provisioning_path/$bootpath/Binary/$ns_data_image" $slot5 -v --skipErase
    if [ $? -ne 0 ]; then error; return 1; fi
fi

action="Write Binary Keys of first boot Stage"
echo "$action"
"$stm32programmercli" "$connect_no_reset" -d "$rot_provisioning_path/OEMiRoT_OEMuRoT/Binary/OEMiRoT_Keys.bin" 0xC004000 -v --skipErase
if [ $? -ne 0 ]; then error; return 1; fi
echo "Binary Keys Written"

action="Write the first boot stage OEMiROT_Boot"
echo "$action"
"$stm32programmercli" "$connect_no_reset" -d "$rot_provisioning_path/OEMiRoT_OEMuRoT/Binary/OEMiROT_Boot.bin" 0xC006000 -v --skipErase
if [ $? -ne 0 ]; then error; return 1; fi
echo "OEMiROT_Boot Written"

action="Write Binary Keys of the second boot stage"
echo "$action"
"$stm32programmercli" "$connect_no_reset" -d "$rot_provisioning_path/OEMiRoT_OEMuRoT/Binary/OEMuRoT_Keys.bin" $keyaddress -v --skipErase
if [ $? -ne 0 ]; then error; return 1; fi
echo "Binary Keys Written"

action="Write Write the second boot stage OEMuROT_Boot"
echo "$action"
"$stm32programmercli" "$connect_no_reset" -d "$boot_dir/Binary/OEMuROT_Boot_init_sign.bin" $oemirot_oemurot_slot0 -v --skipErase
if [ $? -ne 0 ]; then error; return 1; fi
echo "OEMuROT_Boot Written"

# ======================================================= Extra board protections =========================================================
action="Configure Option Bytes"
echo "$action"
echo "Configure Secure option Bytes: Write Protection, Hide Protection and boot lock"
"$stm32programmercli" $connect_no_reset -ob $sec_water_mark $boot_write_protect $hide_protect_1 $boot_lock $wrp_protect
if [ $? -ne 0 ]; then error; return 1; fi

echo "Programming success"
if [ "$script_mode" != "AUTO" ]; then $SHELL;  fi

return
