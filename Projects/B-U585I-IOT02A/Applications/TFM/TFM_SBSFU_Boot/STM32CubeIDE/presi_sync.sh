LOCAL_PATH=~/workspace/STM32Cube_FW_U5/Projects/B-U585I-IOT02A/Applications/TFM
REMOTE_PATH=/home/upmem/work/mbartoli
scp $LOCAL_PATH/TFM_Appli/Binary/tfm_ns_app_init.bin upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_Appli/Binary/tfm_ns_data_init.bin upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_Appli/Binary/tfm_s_app_init.bin upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_Appli/Binary/tfm_s_data_init.bin upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_Loader/Binary/loader.bin  upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_SBSFU_Boot/STM32CubeIDE/Release/TFM_SBSFU_Boot.bin   upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_Appli/Binary/tfm_ns_app_init.bin upmem@172.16.3.75:$REMOTE_PATH/binaries
scp $LOCAL_PATH/TFM_SBSFU_Boot/STM32CubeIDE/TFM_UPDATE_upmem.sh   upmem@172.16.3.75:$REMOTE_PATH
scp $LOCAL_PATH/TFM_SBSFU_Boot/STM32CubeIDE/regression.sh   upmem@172.16.3.75:$REMOTE_PATH

