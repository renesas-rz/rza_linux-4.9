#!/bin/bash


# first make sure we are running from the base of the kernel directory
if [ ! -e .get_maintainer.ignore ] ; then
  echo "ERROR: You must run this script from the base of the linux-4.9 directory"
  echo "press enter to continue"
  read dummy
  exit
fi

#defaults
boardname=toaster
companyname=mycompany
devicetype=RZ_A1H

extal=13.33MHz
extalspeed=13330000
p1clockspeed=66666666
hasusbxtal=no

hasrtcxtal=no

memory="Internal RAM only"
memoryaddr=20000000
memorysize=00A00000
memorysizename=10Mbyte

haslcd=no

scif=2

hassdhi=no

hasmmc=no
haseth=no

hasi2c=no

hassdram=no

usb0=no
usb1=no

while [ "1" == "1" ]
do
	echo 'whiptail --title "Add new BSP Board"  --noitem --menu "Make changes the items below as needed, then press create.\nYou may use ESC+ESC to cancel." 0 0 0 \' > /tmp/whipcmd.txt
	echo '"         Board Name: $boardname" "" \' >> /tmp/whipcmd.txt
	echo '"       Company Name: $companyname" "" \' >> /tmp/whipcmd.txt
	echo '"        Device Type: $devicetype" "" \' >> /tmp/whipcmd.txt
	echo '"              EXTAL: $extal" "" \' >> /tmp/whipcmd.txt
	echo '"     48MHz USB XTAL: $hasusbxtal" "" \' >> /tmp/whipcmd.txt
	echo '"     32kHz RTC XTAL: $hasrtcxtal" "" \' >> /tmp/whipcmd.txt
	echo '"     Serial Console: SCIF${scif}" "" \' >> /tmp/whipcmd.txt
	echo '"      System Memory: $memory" "" \' >> /tmp/whipcmd.txt
	echo '"        Memory Size: $memorysizename" "" \' >> /tmp/whipcmd.txt
	echo '"            USB ch0: $usb0" "" \' >> /tmp/whipcmd.txt
	echo '"            USB ch1: $usb1" "" \' >> /tmp/whipcmd.txt
	echo '"         eMMC Flash: $hasmmc" "" \' >> /tmp/whipcmd.txt
	echo '"     SD Card (SDIO): $hassdhi" "" \' >> /tmp/whipcmd.txt
	echo '"                LCD: $haslcd" "" \' >> /tmp/whipcmd.txt
	echo '"           Ethernet: $haseth" "" \' >> /tmp/whipcmd.txt
#	echo '"                I2C: $hasi2c" "" \' >> /tmp/whipcmd.txt
	echo '"       [Create BSP]" "" \' >> /tmp/whipcmd.txt
	echo '2> /tmp/answer.txt' >> /tmp/whipcmd.txt

source /tmp/whipcmd.txt

  #ans=$(head -c 3 /tmp/answer.txt)
  ans=$(cat /tmp/answer.txt)

  # Cancel
  if [ "$ans" == "" ]; then
    exit
    break;
  fi

  ####################################
  # boardname
  ####################################
  echo "$ans" | grep -q "Board Name:"
  if [ "$?" == "0" ] ; then
    whiptail --title "Board Name" --inputbox \
"Enter a board name.\n"\
"Please use all lower case, no spaces.\n"\
"Numbers are OK as long as the first character\n"\
"is not a number.\n"\
"This string will be used for file names, directory names and in the source code.\n"\
"This name has to be unique.\n"\
"Example: rskrza1" 0 20 \
      2> /tmp/answer.txt

    boardname=$(cat /tmp/answer.txt)

    CHECK=`grep board-${boardname} arch/arm/mach-shmobile/Makefile`
    if [ "$CHECK" != "" ] ; then
      whiptail --msgbox "ERROR:\n      Board Name=$boardname\n    Company Name=$companyname\n\n This combination has already been used. Please choose a different board name." 0 0
    fi

    #check for spaces
    echo "$boardname" | grep -q " "
    if [ "$?" == "0" ] ; then
      whiptail --msgbox "ERROR:\nBoard Name contains spaces ($boardname).\nPlesae try again." 0 0
      boardname=toaster
    fi

    #check for any character other than lowercase letters or numbers
    CHECK=`echo "$boardname" | grep "^[a-z0-9]\+$"`
    if [ "$?" != "0" ] ; then
      whiptail --msgbox "ERROR:\nBoard Name contains characters other than lowercase letters and numbers $CHECK ($boardname).\nPlesae try again." 0 0
      boardname=toaster
    fi

    continue
  fi

  ####################################
  # companyname
  ####################################
  echo "$ans" | grep -q "Company Name:"
  if [ "$?" == "0" ] ; then
    whiptail --title "Board Name" --inputbox \
"Enter a company name.\n"\
"Please use all lower case, no spaces.\n"\
"Numbers are OK as long as the first character\n"\
"is not a number.\n"\
"This string will be used as the directory name for the custom boards files.\n"\
"You can use the same company name for multiple boards.\n"\
"\n"\
"Example: acme (will create directory u-boot-2017.05/board/acme/ " 0 20 \
      2> /tmp/answer.txt

    companyname=$(cat /tmp/answer.txt)

    CHECK=`grep board-${boardname} arch/arm/mach-shmobile/Makefile`
    if [ "$CHECK" != "" ] ; then
      whiptail --msgbox "ERROR:\n      Board Name=$boardname\n    Company Name=$companyname\n\n This combination has already been used. Please choose a different board name." 0 0
    fi

    #check for spaces
    echo "$companyname" | grep -q " "
    if [ "$?" == "0" ] ; then
      whiptail --msgbox "ERROR:\nCompany Name contains spaces ($companyname).\nPlesae try again." 0 0
      companyname=mycompany
    fi

    #check for any character other than lowercase letters or numbers
    CHECK=`echo "$companyname" | grep "^[a-z0-9]\+$"`
    if [ "$?" != "0" ] ; then
      whiptail --msgbox "ERROR:\nCompany Name contains characters other than lowercase letters and numbers $CHECK ($companyname).\nPlesae try again." 0 0
      companyname=mycompany
    fi

    continue
  fi

  ####################################
  # devicetype
  ####################################
  echo "$ans" | grep -q "Device Type:"
  if [ "$?" == "0" ] ; then
    whiptail --title "Device Type" --nocancel --menu "What RZ/A1 device are you using?" 0 0 0 \
	"RZ_A1H" "(10MB intneral RAM)" \
	"RZ_A1M" "(5MB intneral RAM)" \
	"RZ_A1L" "(3MB intneral RAM)" \
	2> /tmp/answer.txt

    devicetype=$(cat /tmp/answer.txt)

    if [ "$memory" == "Internal RAM only" ] ; then
      memoryaddr=20000000
      if [ "$devicetype" == "RZ_A1H" ] ; then
        memorysize=00A00000
        memorysizename=10Mbyte
      elif [ "$devicetype" == "RZ_A1M" ] ; then
        memorysize=00500000
        memorysizename=5Mbyte
      elif [ "$devicetype" == "RZ_A1L" ] ; then
        memorysize=00300000
        memorysizename=3Mbyte
      fi
    fi
    continue
  fi

  ####################################
  # scif
  ####################################
  echo "$ans" | grep -q "Serial Console:"
  if [ "$?" == "0" ] ; then
    whiptail --title "Serial Console" --nocancel --menu "What SCIF channel is your serial console on?" 0 0 0 \
	"SCIF0" "TxD0/RxD0" \
	"SCIF1" "TxD1/RxD1" \
	"SCIF2" "TxD2/RxD2" \
	"SCIF3" "TxD3/RxD3" \
	"SCIF4" "TxD4/RxD4" \
	"SCIF5" "TxD5/RxD5" \
	"SCIF6" "TxD6/RxD6" \
	"SCIF7" "TxD7/RxD7" \
	2> /tmp/answer.txt

    ans=$(cat /tmp/answer.txt)
    scif=${ans:4:1}	# just save the number
    continue
  fi

  ####################################
  # memory
  ####################################
  echo "$ans" | grep -q " System Memory:"
  if [ "$?" == "0" ] ; then
    whiptail --title " System Memory" --nocancel --menu "What System Memory will Linux use?" 0 0 0 \
	"Internal RAM only" "(address 0x2000000)" \
	"External SDRAM on CS2" "(address 0x0800000)" \
	"External SDRAM on CS3" "(address 0x0C00000)" \
	"External SDRAM on CS2+CS3" "(address 0x0800000) " \
	2> /tmp/answer.txt

    memory=$(cat /tmp/answer.txt)

    if [ "$memory" == "Internal RAM only" ] ; then
      memoryaddr=20000000
      if [ "$devicetype" == "RZ_A1H" ] ; then
        memorysize=00A00000
        memorysizename=10Mbyte
      elif [ "$devicetype" == "RZ_A1M" ] ; then
        memorysize=00500000
        memorysizename=5Mbyte
      elif [ "$devicetype" == "RZ_A1L" ] ; then
        memorysize=00300000
        memorysizename=3Mbyte
      fi
    fi
    if [ "$memory" == "External SDRAM on CS2" ] ; then
      memoryaddr=08000000
      memorysize=00800000
      memorysizename=8Mbyte
    fi
    if [ "$memory" == "External SDRAM on CS3" ] ; then
      memoryaddr=0C000000
      memorysize=00800000
      memorysizename=8Mbyte
    fi
    if [ "$memory" == "External SDRAM on CS2+CS3" ] ; then
      memoryaddr=08000000
      memorysize=8000000
      memorysizename=128Mbyte
    fi

    continue
  fi

  ####################################
  # memorysize
  ####################################
  echo "$ans" | grep -q " Memory Size:"
  if [ "$?" == "0" ] ; then

    if [ "$memory" == "Internal RAM only" ] ; then

      whiptail --msgbox "A $devicetype has $memorysizename of internal RAM.\n" 0 0
      continue
    fi

    whiptail --title " Memory Size" --nocancel --menu "What is the size your memory ($memory) ?" 0 0 0 \
	"8Mbyte" "" \
	"16Mbyte" "" \
	"32Mbyte" "" \
	"64Mbyte" "" \
	"128Mbyte" "" \
	2> /tmp/answer.txt

    memorysizename=$(cat /tmp/answer.txt)
    if [ "$memorysizename" == "8Mbyte" ] ; then
      memorysize=00800000
    elif [ "$memorysizename" == "16Mbyte" ] ; then
      memorysize=01000000
    elif [ "$memorysizename" == "32Mbyte" ] ; then
      memorysize=02000000
    elif [ "$memorysizename" == "64Mbyte" ] ; then
      memorysize=04000000
    elif [ "$memorysizename" == "128Mbyte" ] ; then
      memorysize=08000000
    fi
    continue
  fi


  ####################################
  # extal
  ####################################
  echo "$ans" | grep -q "EXTAL:"
  if [ "$?" == "0" ] ; then
    whiptail --title "EXTAL"  --nocancel --menu "What speed is the EXTAL clock?" 0 0 0 \
	"10MHz" "" \
	"12MHz" "" \
	"13.33MHz" "" \
	"none" "(assumes you only have a 48 MHz USB clock)" \
	2> /tmp/answer.txt

    extal=$(cat /tmp/answer.txt)
    
  if [ "$extal" == "10MHz" ] ; then
    p1clockspeed=50000000
    extalspeed=10000000
  fi
  if [ "$extal" == "12MHz" ] ; then
    p1clockspeed=64000000
    extalspeed=12000000
  fi
  if [ "$extal" == "none" ] ; then
    p1clockspeed=64000000
    extalspeed=00000000
  fi
  if [ "$extal" == "13.33MHz" ] ; then
    p1clockspeed=66666666
    extalspeed=13330000
  fi

    continue
  fi


  ####################################
  # hasusbxtal
  ####################################
  echo "$ans" | grep -q "48MHz USB XTAL:"
  if [ "$?" == "0" ] ; then
    whiptail --title "48MHz USB XTAL" --nocancel --menu "Does this board have a 48MHz USB XTAL?" 0 0 0 \
	"yes" "" \
	"no" "" \
	2> /tmp/answer.txt

    hasusbxtal=$(cat /tmp/answer.txt)
    continue
  fi

  ####################################
  # hasrtcxtal
  ####################################
  echo "$ans" | grep -q "32kHz RTC XTAL:"
  if [ "$?" == "0" ] ; then
    whiptail --title "32kHz RTC XTAL" --nocancel --menu "Does this board have a 32kHz RTC XTAL?" 0 0 0 \
	"yes" "" \
	"no" "" \
	2> /tmp/answer.txt

    hasrtcxtal=$(cat /tmp/answer.txt)
    continue
  fi

  ####################################
  # hasmmc
  ####################################
  echo "$ans" | grep -q "eMMC Flash:"
  if [ "$?" == "0" ] ; then
    whiptail --title "eMMC Flash" --nocancel --menu "Does this board have eMMC Flash?" 0 0 0 \
	"yes" "" \
	"no" "" \
	2> /tmp/answer.txt

    hasmmc=$(cat /tmp/answer.txt)

    continue
  fi

  ####################################
  # hassdhi
  ####################################
  echo "$ans" | grep -q "SD Card (SDIO):"
  if [ "$?" == "0" ] ; then

    whiptail --title "SD Card (SDIO)" --nocancel --menu "Does this board have SD Card (SDIO)?" 0 0 0 \
	"ch0" "SDHI-0" \
	"ch1" "SDHI-1" \
	"no" "" \
	2> /tmp/answer.txt

    hassdhi=$(cat /tmp/answer.txt)
    continue
  fi

  ####################################
  # usb0
  ####################################
  echo "$ans" | grep -q "USB ch0:"
  if [ "$?" == "0" ] ; then
    whiptail --title "USB ch0" --nocancel --menu "Does this board have USB ch0?" 0 0 0 \
	"host" "(configured to be a host)" \
	"no" "" \
	2> /tmp/answer.txt

    usb0=$(cat /tmp/answer.txt)
    continue
  fi

  ####################################
  # usb1
  ####################################
  echo "$ans" | grep -q "USB ch1:"
  if [ "$?" == "0" ] ; then
    whiptail --title "USB ch1" --nocancel --menu "Does this board have USB ch1?" 0 0 0 \
	"host" "(configured to be a host)" \
	"no" "" \
	2> /tmp/answer.txt

    usb1=$(cat /tmp/answer.txt)
    continue
  fi


  ####################################
  # haslcd
  ####################################
  echo "$ans" | grep -q "LCD:"
  if [ "$?" == "0" ] ; then

    if [ "$devicetype" == "RZ_A1L" ] ; then
      whiptail --title "LCD" --nocancel --menu "Does this board have a LCD?" 0 0 0 \
	"ch0-RGB24" "VDC5 ch0 with 24-bit parallel RGB output" \
	"ch0-LVDS" "VDC5 ch0 with LVDS output" \
	"no" "" \
	2> /tmp/answer.txt
    else
      whiptail --title "LCD" --nocancel --menu "Does this board have a LCD?" 0 0 0 \
	"ch0-RGB24" "VDC5 ch0 with 24-bit parallel RGB output" \
	"ch0-LVDS" "VDC5 ch0 with LVDS output" \
	"ch1-RGB24" "VDC5 ch1 with 24-bit parallel RGB output" \
	"ch1-LVDS" "VDC5 ch1 with LVDS output" \
	"no" "" \
	2> /tmp/answer.txt
    fi

    haslcd=$(cat /tmp/answer.txt)

    continue
  fi

  ####################################
  # haseth
  ####################################
  echo "$ans" | grep -q "Ethernet:"
  if [ "$?" == "0" ] ; then
    whiptail --title "Ethernet" --nocancel --menu "Does this board have Ethernet?" 0 0 0 \
	"yes" "" \
	"no" "" \
	2> /tmp/answer.txt

    haseth=$(cat /tmp/answer.txt)
    continue
  fi

  ####################################
  # hasi2c
  ####################################
  echo "$ans" | grep -q "I2C:"
  if [ "$?" == "0" ] ; then
    whiptail --title "I2C" --nocancel --menu "Does this board use I2C (that you plan to access while in u-boot)?" 0 0 0 \
	"yes" "" \
	"no" "" \
	2> /tmp/answer.txt

    hasi2c=$(cat /tmp/answer.txt)
    continue
  fi

  ####################################
  # hassdram
  ####################################
  echo "$ans" | grep -q "External SDRAM:"
  if [ "$?" == "0" ] ; then
    whiptail --title "External SDRAM" --nocancel --menu "Does this board have External SDRAM?" 0 0 0 \
	"yes" "" \
	"no" "" \
	2> /tmp/answer.txt

    hassdram=$(cat /tmp/answer.txt)
    continue
  fi


  # Create BSP
  echo "$ans" | grep -q "Create BSP"
  if [ "$?" == "0" ] ; then

    # Do you want to continue?
    whiptail --title "Create BSP" --yesno \
"We will now create the following files:\n"\
"arch/arm/configs/${boardname}_defconfig\n"\
"arch/arm/configs/${boardname}_xip_defconfig\n"\
"arch/arm/boot/dts/r7s72100-${boardname}.dts\n"\
"\n"\
"Do you want to continue?\n" 0 0 2> /tmp/answer.txt

    if [ "$?" == "0" ] ; then
      # yes
      break
    fi
  fi

  # while loop
  continue

done


# copy over the files
cp -a arch/arm/configs/rza1template_defconfig arch/arm/configs/${boardname}_defconfig
cp -a arch/arm/configs/rza1template_xip_defconfig arch/arm/configs/${boardname}_xip_defconfig
cp -a arch/arm/boot/dts/r7s72100-rza1template.dts arch/arm/boot/dts/r7s72100-${boardname}.dts
cp -a arch/arm/mach-shmobile/board-rza1template.c arch/arm/mach-shmobile/board-${boardname}.c


# Convert board name to upper case
boardnameupper=`echo ${boardname} | tr '[:lower:]' '[:upper:]'`


# use sed to change all instances of the board name to the new name
# rza1template >> ${boardname}
# RZA1TEMPLATE >> ${boardnameupper}
# companyname >> ${companyname}

# arch/arm/configs/xxxx_defconfig
sed -i "s/RZA1TEMPLATE/${boardnameupper}/g"  arch/arm/configs/${boardname}_defconfig

# arch/arm/configs/xxxx_xip_defconfig
sed -i "s/RZA1TEMPLATE/${boardnameupper}/g"  arch/arm/configs/${boardname}_xip_defconfig

# arch/arm/boot/dts/r7s72100-xxxx.dts
sed -i "s/rza1template/${boardname}/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
sed -i "s/RZA1TEMPLATE/${boardnameupper}/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
sed -i "s/mycompany/${companyname}/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts

# arch/arm/mach-shmobile/board-xxxx.c
sed -i "s/rza1template/${boardname}/g"  arch/arm/mach-shmobile/board-${boardname}.c
sed -i "s/RZA1TEMPLATE/${boardnameupper}/g"  arch/arm/mach-shmobile/board-${boardname}.c
sed -i "s/mycompany/${companyname}/g"  arch/arm/mach-shmobile/board-${boardname}.c



# NOTE: When finding the line to insert our new board after, notice the extra
#       '-' which makes the text search only return that line
LINE=---------------------------------------------


# arch/arm/boot/dts/Makefile
#sed -i "s/---------------------------------------------/---------------------------------------------\ndtb-y += r7s72100-${boardnameboard}.dtb/g" arch/arm/boot/dts/Makefile
sed -i "s/${LINE}/${LINE}\ndtb-y += r7s72100-${boardname}.dtb/g" arch/arm/boot/dts/Makefile


# arch/arm/mach-shmobile/Makefile
#sed -i "s/---------------------------------------------/---------------------------------------------\nobj-$(CONFIG_MACH_${boardnameupper})	+= board-${boardname}.o/g"  arch/arm/mach-shmobile/Makefile
sed -i "s/${LINE}/${LINE}\nobj-\$(CONFIG_MACH_${boardnameupper})	+= board-${boardname}.o/g"  arch/arm/mach-shmobile/Makefile

# arch/arm/mach-shmobile/Kconfig
sed -i "s/${LINE}/${LINE}\nconfig MACH_${boardnameupper}\n\tbool \"${boardnameupper} board\"\n/g"  arch/arm/mach-shmobile/Kconfig



####################################
# Modify the device tree and board file
####################################

#devicetype
#if [ "$devicetype" == "RZ_A1H" ] ; then
# echo ""  # keep default setting 
#elif [ "$devicetype" == "RZ_A1M" ] ; then
#
#elif [ "$devicetype" == "RZ_A1L" ] ; then
#fi


#extal
sed -i "s/13.33MHz/$extal/" arch/arm/boot/dts/r7s72100-${boardname}.dts 
sed -i "s/13330000/$extalspeed/" arch/arm/boot/dts/r7s72100-${boardname}.dts

if [ "$extal" == "none" ] ; then
  # Remove next 4 lines starting with "/* EXTAL"
  sed -i -e '/\/\* EXTAL/,+4d' arch/arm/boot/dts/r7s72100-${boardname}.dts

fi

#hasusbxtal
if [ "$hasusbxtal" == "no" ] ; then

  # Remove next 4 lines starting with "/* USB_X1"
  sed -i -e '/\/\* USB_X1/,+4d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove everything between "/* USB_X1" and "];"
  #sed -i -e '/\/\* USB_X1/,/};/ d' arch/arm/boot/dts/r7s72100-${boardname}.dts
fi

#hasrtcxtal
if [ "$hasrtcxtal" == "no" ] ; then

  # Remove next 4 lines starting with "/* RTC_X1"
  sed -i -e '/\/\* RTC_X1/,+4d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 3 lines starting with "/* Real Time Clock */"
  sed -i -e '/\/\* Real Time Clock \*\//,+3d' arch/arm/boot/dts/r7s72100-${boardname}.dts

fi


#scif
sed -i "s/scif2/scif${scif}/" arch/arm/boot/dts/r7s72100-${boardname}.dts
sed -i "s:serial2:serial${scif}:" arch/arm/boot/dts/r7s72100-${boardname}.dts
sed -i "s/\* TxD2 \*/\* TxD${scif} \*/" arch/arm/boot/dts/r7s72100-${boardname}.dts
sed -i "s/\* RxD2 \*/\* TxD${scif} \*/" arch/arm/boot/dts/r7s72100-${boardname}.dts

# CONFIG_DEBUG_LL=y
#+CONFIG_DEBUG_UART_PHYS=0xe8008000
# CONFIG_EARLY_PRINTK=y

if [ "$scif" == "0" ] ; then
  sed -i "s/0xE8008000/0xE8007000/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE8007000/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE8007000/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "1" ] ; then
  sed -i "s/0xE8008000/0xE8007800/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE8007800/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE8007800/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "2" ] ; then
  sed -i "s/0xE8008000/0xE8008000/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE8008000/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE8008000/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "3" ] ; then
  sed -i "s/0xE8008000/0xE8008800/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE8008800/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE8008800/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "4" ] ; then
  sed -i "s/0xE8008000/0xE8009000/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE8009000/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE8009000/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "5" ] ; then
  sed -i "s/0xE8008000/0xE8009800/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE8009800/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE8009800/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "6" ] ; then
  sed -i "s/0xE8008000/0xE800A000/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE800A000/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE800A000/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
elif [ "$scif" == "7" ] ; then
  sed -i "s/0xE8008000/0xE800A800/g"  arch/arm/configs/${boardname}_defconfig
  sed -i "s/0xE8008000/0xE800A800/g"  arch/arm/configs/${boardname}_xip_defconfig
  sed -i "s/0xE8008000/0xE800A800/g"  arch/arm/boot/dts/r7s72100-${boardname}.dts
fi

#memory
sed -i "s:memory@8000000:memory@${memoryaddr}:" arch/arm/boot/dts/r7s72100-${boardname}.dts
sed -i "s:reg = <0x20000000 0x00A00000>;:reg = <0x${memoryaddr} 0x${memorysize}>;\t \/\* ${memorysizename} of ${memory} \*\/:" arch/arm/boot/dts/r7s72100-${boardname}.dts


#hasmmc=no
if [ "$hasmmc" == "no" ] ; then
  # Remove next 24 lines starting with the line "/* MMC */"
  sed -i -e '/\/\* MMC \*\//,+24d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 14 lines starting with the line "/* MMC Pins */"
  sed -i -e '/\/\* MMC Pins \*\//,+14d' arch/arm/boot/dts/r7s72100-${boardname}.dts

fi

#hassdhi=
if [ "$hassdhi" != "ch0" ] ; then
  # Remove next 7 lines starting with the line "/* SDHI ch0 */"
  sed -i -e '/\/\* SDHI ch0 \*\//,+7d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 11 lines starting with the line "/* SDHI ch0 Pins */"
  sed -i -e '/\/\* SDHI ch0 Pins \*\//,+11d' arch/arm/boot/dts/r7s72100-${boardname}.dts
fi
if [ "$hassdhi" != "ch1" ] ; then
  # Remove next 7 lines starting with the line "/* SDHI ch1 */"
  sed -i -e '/\/\* SDHI ch1 \*\//,+7d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 11 lines starting with the line "/* SDHI ch1 Pins */"
  sed -i -e '/\/\* SDHI ch1 Pins \*\//,+11d' arch/arm/boot/dts/r7s72100-${boardname}.dts
fi


#hasusbxtal
if [ "$hasusbxtal" == "no" ] ; then
  sed -i "s/xtal-48mhz/xtal-12mhz/" arch/arm/boot/dts/r7s72100-${boardname}.dts
fi

#usb1
if [ "$usb1" != "host" ] ; then
  # Remove next 7 lines starting with the line "/* USB Host ch1 */"
  sed -i -e '/\/\* USB Host ch1 \*\//,+7d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  if [ "$usb0" != "host" ] ; then
    # Remove next 5 lines starting with the line "/* USB Host ch0 */"
    sed -i -e '/\/\* USB Host ch0 \*\//,+5d' arch/arm/boot/dts/r7s72100-${boardname}.dts
  fi
fi


#haseth=no
if [ "$haseth" == "no" ] ; then

  # Remove next 11 lines starting with the line "/* Ethernet */"
  sed -i -e '/\/\* Ethernet \*\//,+11d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 21 lines starting with the line "/* Ethernet Pins */"
  sed -i -e '/\/\* Ethernet Pins \*\//,+21d' arch/arm/boot/dts/r7s72100-${boardname}.dts

fi

#haslcd
if [ "$haslcd" == "no" ] ; then

  # Remove next 42 lines starting with the line "/* VDC5 LCD ch 0 Pins */"
  sed -i -e '/\/\* VDC5 LCD ch 0 Pins \*\//,+42d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 11 lines starting with the line "/* VDC5 LCD ch 0 LVDS Pins */"
  sed -i -e '/\/\* VDC5 LCD ch 0 LVDS Pins \*\//,+11d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove lines between /* LCD */ and /* LCD end */
  sed -i -e '/\/\* LCD \*\//,/\/\* LCD end \*\// d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  # Remove next 23 lines starting with the line "/* RIIC0 (I2C Touchscreen example) */"
  sed -i -e '/\/\* RIIC0 (I2C Touchscreen example) \*\//,+23d' arch/arm/boot/dts/r7s72100-${boardname}.dts

fi

# RGB24
if [ "${haslcd:4}" == "RGB24" ] ; then
  # Remove next 11 lines starting with the line "/* VDC5 LCD ch 0 LVDS Pins */"
  sed -i -e '/\/\* VDC5 LCD ch 0 LVDS Pins \*\//,+11d' arch/arm/boot/dts/r7s72100-${boardname}.dts
fi

# LVDS
if [ "${haslcd:4}" == "LVDS" ] ; then
  # Remove next 42 lines starting with the line "/* VDC5 LCD ch 0 Pins */"
  sed -i -e '/\/\* VDC5 LCD ch 0 Pins \*\//,+42d' arch/arm/boot/dts/r7s72100-${boardname}.dts

  #use_lvds = <1>
  sed -i "s/use_lvds = <0>/use_lvds = <1>/g" arch/arm/boot/dts/r7s72100-${boardname}.dts

  # panel_icksel = <0>;	/* (don't care when lvds=1) */
  sed -i -e "s/panel_icksel =.*/panel_icksel = <0>;	\/\* (don't care when lvds=1) \*\//" arch/arm/boot/dts/r7s72100-${boardname}.dts

  # panel_ocksel = <2>;	/* 2=OCKSEL_PLL_DIV7 (Peripheral clock 1) */
  sed -i -e "s/panel_ocksel =.*/panel_ocksel = <2>;	\/\* 2=OCKSEL_PLL_DIV7 (Peripheral clock 1) \*\//" arch/arm/boot/dts/r7s72100-${boardname}.dts
fi

if [ "${haslcd:0:3}" == "ch1" ] ; then

  #vdc50 -> vdc51
  sed -i "s/vdc50/vdc51/g" arch/arm/boot/dts/r7s72100-${boardname}.dts

  #VDC5 LCD ch 0 -> VDC5 LCD ch 1
  sed -i "s/VDC5 LCD ch 0/VDC5 LCD ch 1/g" arch/arm/boot/dts/r7s72100-${boardname}.dts

  #LCD0_ -> LCD1_
  sed -i "s/LCD0_/LCD1_/g" arch/arm/boot/dts/r7s72100-${boardname}.dts

fi

if [ "$memory" != "Internal RAM only" ] ; then

  # remove '//' from //#define VDC5_FB_ADDR (0x60000000)
  sed -i "s/\/\/#define VDC5_FB_ADDR /#define VDC5_FB_ADDR /g" arch/arm/boot/dts/r7s72100-${boardname}.dts

  #define VDC5_FB_ADDR 0	/* 0 = allocate memory at probe (don't use when using SDRAM) */
  # ad '//' in front of "#define VDC5_FB_ADDR 0"
  sed -i "s/#define VDC5_FB_ADDR 0/\/\/#define VDC5_FB_ADDR 0/g" arch/arm/boot/dts/r7s72100-${boardname}.dts

fi


#hasi2c
#if [ "$hasi2c" == "no" ] ; then
#fi

###############################
# done
###############################

whiptail --msgbox \
"Complete!"\
"\nPlease note that none of the pin mux settings have been configured yet.\n"\
"You MUST manually edit the device tree file: \n"\
"      arch/arm/boot/dts/r7s72100-${boardname}.dts\n\n"\
"Also, please review your configuration file and make any necessary changes:\n"\
"      include/configs/${boardname}.h\n\n"\
"To build for your board, please use the following commands:\n"\
"   make ${boardname}_defconfig\n"\
"     or\n"\
"   make ${boardname}_xip_defconfig\n"\
"   make\n\n"\
"Or, if you are using the bsp build environment:\n"\
"   ./build.sh kernel ${boardname}_defconfig\n"\
"   ./build.sh kernel uImage\n"\
"     or\n"\
"   ./build.sh kernel xipImage\n" 0 0

gedit arch/arm/boot/dts/r7s72100-${boardname}.dts &


