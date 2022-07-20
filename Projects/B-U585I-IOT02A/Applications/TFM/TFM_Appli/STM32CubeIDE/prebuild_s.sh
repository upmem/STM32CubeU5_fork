#!/bin/bash - 

builddir=`pwd`
# Full path is required for tool used
templatedir=$builddir/Templates
mkdir -p Templates

# Define Templates sources in TFM
Third_Party=../../../../../../../../Middlewares/Third_Party
TFM_TEST_PATH=$Third_Party/trustedfirmware/test
TFM_PATH=$Third_Party/trustedfirmware
PARTITION=$Third_Party/trustedfirmware/secure_fw/partitions
TOOLS=$Third_Party/trustedfirmware/tools

# Define template source from adaptation 
CONFIG=../../../Secure/Src

# Update the templates directory in case one original file changed
# config from application
cp -u $CONFIG/tfm_manifest_list.yaml Templates 
cp -u $CONFIG/tfm_generated_file_list.yaml Templates 

# Config platform customizeable
cp -u $CONFIG/tfm_app_rot.yaml Templates
cp -u $CONFIG/tfm_platform.yaml Templates

# Config from middleware standard services
cp -u $PARTITION/crypto/tfm_crypto.yaml Templates
cp -u $PARTITION/protected_storage/tfm_protected_storage.yaml Templates
cp -u $PARTITION/internal_trusted_storage/tfm_internal_trusted_storage.yaml Templates
cp -u $PARTITION/initial_attestation/tfm_initial_attestation.yaml Templates
cp -u $PARTITION/firmware_update/tfm_firmware_update.yaml Templates
cp -u $PARTITION/tfm_ffm11_partition/tfm_ffm11_partition.yaml Templates
cp -u $PARTITION/manifestfilename.template Templates
cp -u $PARTITION/partition_intermedia.template Templates
cp -u $PARTITION/tfm_service_list.inc.template  Templates
cp -u $TFM_PATH/interface/include/tfm_veneers.h.template Templates
cp -u $TFM_PATH/secure_fw/spm/cmsis_psa/tfm_secure_irq_handlers_ipc.inc.template Templates
cp -u $TFM_PATH/secure_fw/spm/cmsis_psa/tfm_spm_db_ipc.inc.template Templates
cp -u $TFM_PATH/interface/include/psa_manifest/sid.h.template Templates
cp -u $TFM_PATH/interface/include/psa_manifest/pid.h.template Templates

# Config for test
cp -u $TFM_TEST_PATH/test_services/tfm_core_test/tfm_ss_core_test.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_core_test_2/tfm_ss_core_test_2.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_secure_client_service/tfm_secure_client_service.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_ipc_service/tfm_ipc_service_test.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_ipc_client/tfm_ipc_client_test.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_ps_test_service/tfm_ps_test_service.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_irq_test_service_1/tfm_irq_test_service_1.yaml Templates
cp -u $TFM_TEST_PATH/test_services/tfm_secure_client_2/tfm_secure_client_2.yaml Templates

# Config for linker
cp -u ../tfm_isolation_l2.ld.template Templates

# Generate files from manifest
export projectdir=$templatedir

$TOOLS/dist/tfm_parse_manifest_list/tfm_parse_manifest_list.exe -o $templatedir/../AutoGen -m $templatedir/tfm_manifest_list.yaml -f $templatedir/tfm_generated_file_list.yaml
#python $TOOLS/dist/pyscript/tfm_parse_manifest_list.py -o $templatedir/../AutoGen -m $templatedir/tfm_manifest_list.yaml -f $templatedir/tfm_generated_file_list.yaml

# Preprocess linker file
arm-none-eabi-gcc -E -P -xc -DBL2 -D$2 -DTFM_PSA_API  -I../../../../Linker -o./output.ld  ./tfm_common_s.ld
