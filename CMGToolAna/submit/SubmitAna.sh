#!/bin/bash

# SAMPLES TO RUN ON
#samples="MC_DYJets MC_WJets MC_TTbar "
samples="MC_DYJets MC_QCD MC_WJets MC_TTbar MC_TTV MC_SingleTop "
#samples="MC_QCD_Iso MC_WJets_Iso MC_TTbar_Iso MC_TTV"
#samples+="MC_SingleTop MC_TTH MC_TTW MC_TTZ "
samples+=" MC_T1tttt_1500_100 MC_T1tttt_1200_800 "
#samples="MC_T1tttt_1500_100_Iso MC_T1tttt_1200_800_Iso"
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

# find the runAnalyzer
EXE=$(find .. -maxdepth 2 -name "runAnalyzer.py")
EXE=$(readlink -f $EXE)

echo "Analyzer is:"
echo $EXE

for samp in $samples; do

    # create temp dir
    JobDir=$OutDir/.$samp"_"$Pref

    if [ ! -d $JobDir ]; then
	mkdir -p $JobDir
    fi

    cd $JobDir

    echo "Submitting" $samp "from" $JobDir

    reader=reader_${AnaName}_${samp}.job
    cat $SubDir/jobTemplate.sh >| $reader

    sed -i "s|JBNAME|An_$samp|" $reader
    sed -i "s|EXE|$EXE|" $reader
    sed -i "s|ANANAME|$AnaName|" $reader
    sed -i "s|SAMPLE|$samp|" $reader

    qsub -o . $reader $OutDir

    cd $SubDir
done
