#!/bin/bash

# SAMPLES TO RUN ON
#samples="MC_DYJets MC_QCD MC_WJets MC_TTbar "
samples="MC_QCD MC_WJets MC_TTbar "
#samples+="MC_SingleTop MC_TTH MC_TTW MC_TTZ "
#samples+="MC_T1tttt_1500_100 MC_T1tttt_1200_800"
#samples+="MC_T5tttt_1000_280 MC_T5tttt_1300_280 MC_T5tttt_1000_285 MC_T5tttt_1300_285 MC_T1ttbbWW_1300_290 MC_T1ttbbWW_1300_295 MC_T1ttbbWW_1000_715 MC_T1ttbbWW_1000_720 "
#samples+="SqGl_1300_100"


if [  $# = 0 ]; then
    echo "Usage:"
    echo "./SubmitAna.sh AnalyzerName [OutputDir]"
    exit 0
else
    AnaName=$1
fi

SubDir=$PWD

if [ $# -eq 2 ]; then
    OutDir=$(readlink -f Output/$2)
    Pref=$2
else
    OutDir=$(readlink -f Output)
fi

if [ ! -d $OutDir ]; then
    mkdir -p $OutDir
fi


echo "Going to submit following samples"
echo $samples
echo "With output to" $OutDir

for samp in $samples; do

    # create temp dir
    JobDir=$PWD/../.$samp$Pref

    if [ ! -d $JobDir ]; then
	mkdir -p $JobDir
    fi

    cd $JobDir

    echo "Submitting" $samp "from" ../.$samp$Pref

    reader=reader_${AnaName}_${samp}.job
    cat $SubDir/jobTemplate.sh >| $reader

    sed -i "s|JBNAME|An_$samp|" $reader
    sed -i "s|XXXX|$AnaName|" $reader
    sed -i "s|YYYY|$samp|" $reader

    qsub -o . $reader $OutDir

    cd $SubDir
done
