#!/afs/desy.de/products/python/.amd64_rhel50/2.6/bin/python

from math import sqrt
import sys,os 
#############################################
##### This code needs the Higgs combine tool to merge the different HT bins
##### follow instructions here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit
print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv[0])
signalsample = 'T1tttt_1300_100'
if len(sys.argv) == 2:
	signalsample = str(sys.argv[1])

# sample names according to: base/base_tag_sample.txt
#samples = ['TTbar','WJets','T1tttt_1300_100']#,'T1tttt_1500_100','T1tttt_800_450','T1tttt_1200_800']
samples = [signalsample,'TTbar','WJets']#,'T1tttt_1500_100','T1tttt_800_450','T1tttt_1200_800']
process = [0,1,2]
processName = ['signal','ttbar','wjets']
# tag is NoPU,50PU,140PU

def GetLinesList(path,base,tag,n,moreBin,pick=[]):
	first=base+'_'+moreBin+'_'+tag+'_'
	sample = 0
	
	for s in samples:
		file = open(path+'/'+first+s+'.txt')
		linesList.append(file.readlines())
		file.close
		sample =  sample + 1

def GetData(linesList,n,pick=[]):
	bin = 1
	N=len(linesList[0])-n
	#linesList has number of backgrounds + 1 signal entry
	for i in range(2,N):
		if len(pick)>0 and not i in pick: continue
		what=linesList[0][i].split('\t')[0][6:]
		SRstringb = 'Nb'
		SRstringdf = 'Df>'
		SRstringST = 'ST'
		nbkg = 0
		if SRstringb in what and SRstringdf  in what  and SRstringST in what:
			for k in range(len(linesList)):
				bins.append(bin)
			for k in range(len(linesList)):
				val = float(linesList[k][i].split('\t')[2])
				ival = float(linesList[k][i].split('\t')[4])
				d.append(val)
				s.append(ival)
				if val > 0.001:
					uncert = ival/val + 1
				else :
					uncert = 2.0
				statuncertainties.append(uncert)
				if val > 0.001:
					values.append(val)
				else :
					values.append(0.001)
				if k > 0:
					nbkg = nbkg + val
			if nbkg > 0.001:
				nbkgs.append(nbkg)
			else :
				nbkgs.append(0.001)
			bin = bin + 1
			

def PrintDataCard(name):
	f = open('datacard'+name+'.txt', 'w')
	f.write( 'imax '+str(len(nbkgs))+' number of channels \n')
	f.write( 'jmax  2  number of backgrounds \n')
	f.write( 'kmax '+str(len(statuncertainties))+'  number of nuisance parameters (sources of systematic uncertainties) \n')
	f.write( 'bin ')
	for i in range(len(nbkgs)):
		f.write( "bin"+str(i+1)+ " ")
	f.write("\n")
	f.write( 'observation ')
	for i in range(len(nbkgs)):
		f.write( str(nbkgs[i]) + " ")

	f.write("\n")  
	f.write( 'bin ')
	for i in range(len(bins)):
		f.write( "bin"+str(bins[i]) + " ")
	f.write("\n")
	f.write( 'process ')
	for i in range(len(nbkgs)):
		f.write( ' signal ttbar wjets ')
	f.write("\n")
	f.write( 'process ')
	for i in range(len(nbkgs)):
		f.write( " 0 1 2 ")
	f.write("\n")
	
	f.write( 'rate ')
	for i in range(len(values)):
		f.write( str(values[i])+ " ")
	f.write('\n')
	
	for k in range(len(statuncertainties)):
		f.write(processName[k % 3]+"_bin"+str(bins[k]) + " lnN ")
		for i in range(0,k):
			f.write( " - ")
		f.write(str(statuncertainties[k]))
		for i in range(k+1,len(statuncertainties)):
			f.write(" - ")
		f.write("\n")
		       
			       
	f.close()
############################################################################################################
#print '-----------------------------------------------------------------------------'



#Cutflow('Tables/Df0.75', 'CMG_cutflow_Softb_HT500','MC',0)
#Cutflow('Tables/Df0.75', 'CMG_cutflow_Softb_HT750','MC',0)
	

MoreBins = ['HT500','HT750','HT1000']
for moreBin in MoreBins:
	linesList=[]
	d=[] # expected events
	s=[] # statistical uncertainty from MC counts
	bins = []
	values = []
	nbkgs = []
	statuncertainties = []
	GetLinesList('Tables/Df0.75', 'CMG_cutflow_Softb','MC',0,moreBin)
	GetData(linesList,0)
	PrintDataCard(samples[0]+'_'+moreBin)

cmd = 'combineCards.py '+MoreBins[0]+'=datacard'+samples[0]+'_'+MoreBins[0]+'.txt '+MoreBins[1]+'=datacard'+samples[0]+'_'+MoreBins[1]+'.txt '+MoreBins[2]+'=datacard'+samples[0]+'_'+MoreBins[2]+'.txt > datacard'+samples[0]+'_AllBins.txt'
print cmd
os.popen(cmd)
