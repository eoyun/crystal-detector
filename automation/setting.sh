#! /bin/sh

set -e
declare -a options

function set_vars()
{
    INITIAL_DIR_ROW=`awk '/initialdir/{ print NR; exit }' condor.sub`
    sed -i "${INITIAL_DIR_ROW}s/=.*/= $SAVE_DIR/" condor.sub
    
    TRANSFER_INPUT_ROW=`awk '/transfer_input_files/{ print NR; exit}' condor.sub`
    sed -i "${TRANSFER_INPUT_ROW}s?=.*?= ${CEPC_BUILD_DIR}/../CMakeFiles, ${CEPC_BUILD_DIR}/../CMakeCache.txt, ${CEPC_BUILD_DIR}/HTCondor_env.sh, ${CEPC_BUILD_DIR}/condor_exe.sh, ${CEPC_BUILD_DIR}/init.mac, ${CEPC_BUILD_DIR}/run_muon.mac, ${CEPC_BUILD_DIR}/run_ele.mac, ${CEPC_BUILD_DIR}/run_gamma.mac, ${CEPC_BUILD_DIR}/CBDsim?" condor.sub

    #SOURCE_ROW=`awk '/source/{ print NR; exit }' condor_exe.sh`
    #sed -i "${SOURCE_ROW}s?.*?source ${CEPC_BUILD_DIR}/HTCondor_env.sh?" condor_exe.sh
            
    EXE_ROW=`awk '/CBDsim/{ print NR; exit }' condor_exe.sh`
    sed -i "${EXE_ROW}s?.*?\./CBDsim ${MACRO} \$1?" condor_exe.sh

    condor_submit condor.sub
}


function choose_particle()
{
    echo -e "\nChoose the particle you want to shoot"
    PS3="Shoot: "
    options=("muon" "ele" "Gamma" "Quit")
    select opt in "${options[@]}"
    do
        case $opt in
            "muon")
            MACRO="run_muon.mac"
            break;;

 	    "ele")
            MACRO="run_ele.mac"
            break;;
            
            "Gamma")
            MACRO="run_gamma.mac"
            break;;
            
            "Quit")
            echo "Exit the program"
            exit;;
            
            *) echo "$REPLY is an invalid option";;
        esac
    done
}
##MACRO="run_ele.mac"
CEPC_BUILD_DIR=`pwd`
##export BATCH_SYS=${option[$(($REPLY-1))]}

echo -e "#####################################################"
echo -e "#                                                   #"
echo -e "#        Setting the automation scripts(KNU)        #"
echo -e "#                                                   #"
echo -e "#####################################################"

echo -e
read -p "What is the dir where you want to store result data? : " SAVE_DIR
mkdir -p $SAVE_DIR/log

echo -e "\nThe output and log datas will be stored unger $CEPC_BUILD_DIR/$SAVE_DIR !\n"
choose_particle
set_vars
