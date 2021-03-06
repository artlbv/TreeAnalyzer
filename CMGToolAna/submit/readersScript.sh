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
## the cpu time for this job
##$ -l h_cpu=02:00:00
#$ -l h_cpu=05:00:00
#$ -l site=hh
## the maximum memory usage of this job
#$ -l h_vmem=1500M
##$ -l h_vmem=3000M
## stderr and stdout are merged together to stdout
#$ -j y
## define input dir,output dir,executable LD_LIBRARY_PATH
#$ -v INDIR=ZZZZ
#$ -v OUTDIR=ZZZZ
#$ -o ZZZZ
#$ -v LD_LIBRARY_PATH=ZZZZ:/opt/d-cache/dcap/lib64:/nfs/cms/usernaf.desy.de/products/root/amd64_rhel50/5.34.00/lib:./:
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
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/d-cache/dcap/lib64:/afs/desy.de/products/root/amd64_rhel50/5.34.00/lib::/opt/products/fftw/2.1.5/lib/
export ROOTSYS=/afs/desy.de/products/root/amd64_rhel50/5.34.00
########################################################
#
echo $PWD
cd $OUTDIR
echo start at `date`
module load root/5.34
ini ROOT534
echo $ROOTSYS
which root
./runAnalyzer.py XXXX YYYY
echo
#mv XXXX_*.txt XXXX_*.root ./XXXX
echo job done at `date`
