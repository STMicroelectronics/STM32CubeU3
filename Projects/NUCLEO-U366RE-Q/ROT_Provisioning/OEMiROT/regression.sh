#!/bin/bash -
echo "regression script started"
source ../env.sh

secbootadd0=0x1800C0
flashsectnbr=0x7F

connect="-c port=SWD mode=UR"
connect_no_reset="-c port=SWD mode=HotPlug"

rdp_0="-ob RDP=0xAA TZEN=1 UNLOCK_1A=1 UNLOCK_1B=1 UNLOCK_2A=1 UNLOCK_2B=1"
remove_bank1_protect="-ob SECWM1_PSTRT=$flashsectnbr SECWM1_PEND=0 WRP1A_PSTRT=$flashsectnbr WRP1A_PEND=0 WRP1B_PSTRT=$flashsectnbr WRP1B_PEND=0"
remove_bank2_protect="-ob SECWM2_PSTRT=$flashsectnbr SECWM2_PEND=0 WRP2A_PSTRT=$flashsectnbr WRP2A_PEND=0 WRP2B_PSTRT=$flashsectnbr WRP2B_PEND=0"
erase_all="-e all"
remove_hdp_protection="-ob HDP1_PEND=0 HDP1EN=0xB4 HDP2_PEND=0 HDP2EN=0xB4"
remove_boot_lock="-ob BOOT_LOCK=0"
default_ob1="-ob SRAM2_RST=0 NSBOOTADD0=0x100000 NSBOOTADD1=0x17F1E0 BOOT_LOCK=0 SECBOOTADD0=$secbootadd0 DBANK=1 SWAP_BANK=0 nSWBOOT0=1 SECWM1_PSTRT=0 SECWM1_PEND=$flashsectnbr"
default_ob2="-ob SECWM2_PSTRT=0 SECWM2_PEND=$flashsectnbr"
# Get OEM2 key
oem2_password="./Keys/oem2_password.txt"
# Check if oem2_password.txt exist
if [ ! -e "$oem2_password" ]; then
echo "The file $oem2_password does not exist."
fi
# Get OEM2 key
read -r oem2_key < "$oem2_password"

echo "Regression to RDP 0, enable tz"
# Unlock RDP 2 to switch in RDP 1
"$stm32programmercli" $connect -hardRst -unlockRDP2 $oem2_key
echo "Please unplug USB cable and plug it again to recover SWD Connection."
echo "Press any key to continue..."
echo
if [ "$1" != "AUTO" ]; then read -p "" -n1 -s; fi

# Switch RDP 1 to RDP 0
"$stm32programmercli" $connect_no_reset $rdp_0
ret=$?
if [ $ret != 0 ]; then
  "$stm32programmercli" $connect $rdp_0
  ret=$?
  if [ $ret != 0 ]; then
    if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
    exit 1
  fi
fi

echo "Remove bank1 protection"
"$stm32programmercli" $connect $remove_bank1_protect
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Remove bank2 protection and erase all"
"$stm32programmercli" $connect_no_reset $remove_bank2_protect $erase_all
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Remove hdp protection"
"$stm32programmercli" $connect_no_reset $remove_hdp_protection
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Set default OB 1 (dual bank, swap bank, sram2 reset, secure entry point, bank 1 full secure)"
"$stm32programmercli" $connect_no_reset $remove_boot_lock 
"$stm32programmercli" $connect_no_reset $default_ob1
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Set default OB 2 (bank 2 full secure)"
"$stm32programmercli" $connect_no_reset $default_ob2
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "regression script done, press key"
if [ "$1" != "AUTO" ]; then read -p "" -n1 -s; fi
exit 0

