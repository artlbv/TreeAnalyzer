#!/bin/zsh
## make sure the right shell will be used
#$ -S /bin/zsh
#
#######################################################
##### new commands for running on bird #####
#$ -l h=!bird025.desy.de
#$ -l h=!bird020.desy.de
#$ -l h=!bird028.desy.de
#$ -l arch=amd64
#$ -l distro=sld5
#######################################################
#
## Job Name
#$ -N JBNAME
## the cpu time for this job
#$ -l h_cpu=02:00:00
## the maximum memory usage of this job
#$ -l h_vmem=1900M
#$ -l site=hh
#$ -l distro=sld6
## Use the submitting hosts env variables
#$ -V
## Use same dir as submission script
#$ -cwd
## stderr and stdout are merged together to stdout
#$ -j y
## define input dir,output dir,executable LD_LIBRARY_PATH
#
#######################################################
### bird ###
date
echo
echo running script $JOB_NAME in queue $QUEUE
echo with JobID $JOB_ID on $HOSTNAME as $LOGNAME $USER
echo with group $GROUP and home $SGE_O_HOME
echo output goes to  $SGE_CWD_PATH $CWD
echo command dir is $SGE_O_WORKDIR
echo sent from host $SGE_O_HOST
echo
echo temp dir: $TMP
########################################################
#
echo "Job running on"
echo `uname -a`

BASEDIR=$(dirname $PWD)
echo "Running in" $PWD

EXE=$(find .. -name "runAnalyzer.py")
EXE=$(readlink -f $EXE)

if [ $# -eq 1 ]; then
    OutDir=$1
else
    OutDir="."
fi

echo "Outdir is" $OutDir

echo start at `date`

#../python/./runAnalyzer.py XXXX YYYY
$EXE XXXX YYYY

mv CMG* $OutDir

echo
echo job done at `date`
