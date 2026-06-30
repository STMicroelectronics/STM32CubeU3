#!/bin/bash -

#==============================================================================
#                                    General
#==============================================================================
# Configure tools installation path
WIN_CUBE_PROGRAMMER_PATH="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"

POSIX_CUBE_PROGRAMMER_PATH=$(echo ${WIN_CUBE_PROGRAMMER_PATH} | tr '\\' '/')
POSIX_CUBE_PROGRAMMER_PATH=$(echo "$POSIX_CUBE_PROGRAMMER_PATH" | sed 's/ /\\ /g')


stm32programmercli_path=${POSIX_CUBE_PROGRAMMER_PATH}

stm32programmercli="$stm32programmercli_path/STM32_Programmer_CLI.exe"


# ==============================================================================
# Export the DUA key
# ==============================================================================

# Extract DUA_USER_LU
#eval "${stm32programmercli}" -c port=SWD -rssekw ./STM32CubeExpansion_RSSe_V2.0.0/RSSe/STM32U3/RSSe_KW_U375_U385_v1.0.0.bin ./Wrapped_Keys/DUA_USLU_DHUK_Sign_NS_Priv.bin KeyType=eccchiplu ExportPublicKey=No WrappingKeySelect=DHUK KeyUsage=ECDSA_USAGE_SIGN SecAttr=NON_SECURE PrivAttr=PRIVILEGE

# Extract DUA_USER_FU
eval "${stm32programmercli}" -c port=SWD -rssekw ./STM32CubeExpansion_RSSe_V2.0.0/RSSe/STM32U3/RSSe_KW_U375_U385_v1.0.0.bin ./Wrapped_Keys/DUA_USFU_DHUK_Sign_NS_Priv.bin KeyType=eccchipfu ExportPublicKey=No WrappingKeySelect=DHUK KeyUsage=ECDSA_USAGE_SIGN SecAttr=NON_SECURE PrivAttr=PRIVILEGE

# ==============================================================================
# Reset the board options bytes using the STM32CubeProgrammer.
# ==============================================================================
# Set RDP level to 1 to be able to remove TZ
eval "${stm32programmercli}" -c port=SWD mode=HotPlug -ob RDP=0xBB

# Unset TZ and set RDP level to 0
eval "${stm32programmercli}" -c port=SWD mode=HotPlug -ob RDP=0xAA TZEN=0

# Set nSWBOOT0 & nBOOT0 to be sure to get SysTick Interrupt
eval "${stm32programmercli}" -c port=SWD mode=HotPlug -ob nSWBOOT0=1 nBOOT0=1

echo "        Press any key to continue..."
echo
read -p "" -n1 -s
