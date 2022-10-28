#!/bin/bash - 

echo "dataimg.sh started"
# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
SCRIPTPATH=`dirname $SCRIPT`

# args are optional
# arg1 is the build directory
# arg2 is the version i.e "2.1.0" (default is "1.0.0")
# arg3 is the binary type (nonsecure, secure) (default is both)
# arg4 : dependency with the other image
# "1.0.0" : version dependency with appli image
# "nodep": no dependency
# arg5 is version and dependency in output filename
# "version" : filename contains the version and dependency
# arg6 is to force python execution
# "python" : execute with python script

if [ $# == 0 ]; then
generate_both=1
projectdir=.
else
projectdir=$1
fi

if [ "_$2" == "_" ]; then
  version=1.0.0
else
  version=$2
fi

signing=$3
userAppBinary="$projectdir/../../TFM_Appli/Binary"

if [ "_$4" == "_" ]; then
dep=nodep
else
dep=$4
fi

if [ $dep != nodep ]; then
  depname=_$dep
  if [ "_$signing" == "_secure" ]; then
    option="-d (0,$dep)"
  else
    option="-d (1,$dep)"
  fi
fi

if [ "$5" == "version" ]; then
  ver="_$version$depname"
fi

tfm_s_init="$userAppBinary/tfm_s_data_init$ver.bin"
tfm_ns_init="$userAppBinary/tfm_ns_data_init$ver.bin"
tfm_s_sign="$userAppBinary/tfm_s_data_sign$ver.bin"
tfm_ns_sign="$userAppBinary/tfm_ns_data_sign$ver.bin"
tfm_s_enc_sign="$userAppBinary/tfm_s_data_enc_sign$ver.bin"
tfm_ns_enc_sign="$userAppBinary/tfm_ns_data_enc_sign$ver.bin"

#field updated by TFM_SBSFU_Boot postbuild

image_ns_size=0x2000
image_s_size=0x2000
primary_only=0
crypto_scheme=2
encrypted=1
over_write=1
#end of updated field
LocalDir=`pwd`
cd "$projectdir/../../TFM_SBSFU_Boot/Src"
tfm_key_dir=`pwd`
cd "$LocalDir"
cd "$projectdir/../../../../../../Middlewares/Third_Party/mcuboot"
mcuboot_dir=`pwd`
cd "$LocalDir"
#Make sure we have a Binary sub-folder in UserApp folder
if [ ! -e "$userAppBinary" ]; then
  mkdir "$userAppBinary"
fi

#if [ "_$6" == "_" ]; then
#  #line for window executable
#  echo dataimg with windows executable
#  imgtool=$mcuboot_dir/scripts/dist/imgtool/imgtool.exe
#  python=
#else
  # line for python
  echo dataimg with python script
  imgtool=$mcuboot_dir/scripts/imgtool/main.py
  python="python3 "
#fi

#sign mode
if [ "$crypto_scheme" == "0" ]; then
  mode=rsa2048
fi
if [ "$crypto_scheme" == "1" ]; then
  mode=rsa3072
fi
if [ "$crypto_scheme" == "2" ]; then
  mode=ec256
fi

#keys selection
if [ "$mode" == "rsa2048" ]; then
  key_s="$tfm_key_dir/root-rsa-2048.pem"
  key_ns="$tfm_key_dir/root-rsa-2048_1.pem"
  key_enc_pub="$tfm_key_dir/enc-rsa2048-pub.pem"
fi
if [ "$mode" == "rsa3072" ]; then
  key_s="$tfm_key_dir/root-rsa-3072.pem"
  key_ns="$tfm_key_dir/root-rsa-3072_1.pem"
  key_enc_pub="$tfm_key_dir/enc-rsa2048-pub.pem"
fi
if [ "$mode" == "ec256" ]; then
  key_s="$tfm_key_dir/root-ec-p256.pem"
  key_ns="$tfm_key_dir/root-ec-p256_1.pem"
  key_enc_pub="$tfm_key_dir/enc-ec256-pub.pem"
fi

tfm_s="$projectdir/../../TFM_SBSFU_Boot/Src/s_data.bin"
tfm_ns="$projectdir/../../TFM_SBSFU_Boot/Src/ns_data.bin"
# encrypt option is for init image in swap mode
if [ "$encrypted" == "1" ]; then
  encrypt="-E $key_enc_pub -c"
fi

if [ "$primary_only" == "1" ]; then
  option="$option --primary-only --overwrite-only"
  encrypt=
else
    if [ "$over_write" == "1" ]; then
      option="$option --overwrite-only"
      encrypt=
    fi
fi

#signing
echo $signing $mode $option > $projectdir/output.txt 2>&1

#alignment value (minimum write size value in bytes)
val=16

#nb sectors in image areas
image_ns_sectors=$((image_ns_size / 8192))
image_s_sectors=$((image_s_size / 8192))

if [ $generate_both -eq 1 ]; then signing=nonsecure; fi

if [ "$signing" == "nonsecure" ]; then

  echo "$signing init ns image signing" >> $projectdir/output.txt 2>&1
  command_init_ns="$python$imgtool sign -k $key_ns $encrypt -e little -S $image_ns_size -M $image_ns_sectors -H 0x20 --pad-header $option -v $version -s auto --align $val $tfm_ns $tfm_ns_init"
  $command_init_ns  >> $projectdir/output.txt 2>&1
  ret=$?
  if [ $ret != 0 ]; then
    echo "$command_init_ns : failed" >> $projectdir/output.txt 2>&1
    exit 1
  fi

  echo "$signing clear ns image signing" >> $projectdir/output.txt 2>&1
  command_clear_ns="$python$imgtool sign -k $key_ns -e little -S $image_ns_size -M $image_ns_sectors -H 0x20 --pad-header $option -v $version -s auto --align $val $tfm_ns $tfm_ns_sign" 
  $command_clear_ns >> $projectdir/output.txt 2>&1
  ret=$?
  if [ $ret != 0 ]; then
    echo "$command_clear_ns : failed" >> $projectdir/output.txt 2>&1
    exit 1
  fi
 
  if [ "$encrypted" != "0" ]; then
    echo "$signing enc ns image encrypting and signing" >> $projectdir/output.txt 2>&1
    command_enc_ns="$python$imgtool sign -k $key_ns -E $key_enc_pub -e little -S $image_ns_size -M $image_ns_sectors -H 0x20 --pad-header $option -v $version  -s auto --align $val $tfm_ns $tfm_ns_enc_sign"
    $command_enc_ns >> $projectdir/output.txt 2>&1
    ret=$?
    if [ $ret != 0 ]; then
      echo "$command_enc_ns : failed" >> $projectdir/output.txt 2>&1
      exit 1
    fi
  fi
fi

if [ $generate_both -eq 1 ]; then signing=secure; fi
if [ "$signing" == "secure" ]; then

  echo "$signing init s image signing" >> $projectdir/output.txt 2>&1
  command_init_s="$python$imgtool sign -k $key_s $encrypt -e little -S $image_s_size -M $image_s_sectors -H 0x20 --pad-header $option -v $version -s auto --align $val $tfm_s $tfm_s_init"
  $command_init_s  >> $projectdir/output.txt 2>&1
  ret=$?
  if [ $ret != 0 ]; then
    echo "$command_init_s : failed" >> $projectdir/output.txt 2>&1
    exit 1
  fi

  echo "$signing clear s image signing" >> $projectdir/output.txt 2>&1
  command_clear_s="$python$imgtool sign -k $key_s -e little -S $image_s_size -M $image_s_sectors -H 0x20 --pad-header $option -v $version -s auto --align $val $tfm_s $tfm_s_sign"
  $command_clear_s >> $projectdir/output.txt 2>&1
  ret=$?
  if [ $ret != 0 ]; then
    echo "$command_clear_s : failed" >> $projectdir/output.txt 2>&1
    exit 1
  fi  

  if [ "$encrypted" != "0" ]; then
    echo "$signing enc s image encrypting and signing" >> $projectdir/output.txt 2>&1
    command_enc_s="$python$imgtool sign -k $key_s -E $key_enc_pub -e little -S $image_s_size -M $image_s_sectors -H 0x20 --pad-header $option -v $version  -s auto --align $val $tfm_s $tfm_s_enc_sign"
    $command_enc_s  >> $projectdir/output.txt 2>&1
    ret=$?
    if [ $ret != 0 ]; then
      echo "$command_enc_s : failed" >> $projectdir/output.txt 2>&1
      exit 1
    fi
  fi
fi
  
exit 0