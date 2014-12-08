##!/usr/bin/python
from operator import mul

def scale(fac,list):
    return map(mul,len(list)*[fac],list)

class Component():
    def __init__(self, scene, name, location, xsec, HTbins = ['/'] ):

        self.scene = scene
        if 'MC' in scene:
            self.isMC = True
        else:
            self.isMC = False

        self.name = name
        self.xsec = xsec
        self.location = location
        self.HTbins = HTbins

        self.intLumi = 1000.
        self.weight = []
        self.treename = 'treeProducerSusySingleLepton/'

        # set weights for all HT bins
        if type(self.xsec) is list:
            for ixsec in self.xsec:
                self.weight.append( self.intLumi * ixsec)
        else:
            self.weight = self.intLumi * self.xsec

#    def getWeight( self, intLumi = None):
#        return intLumi * self.xsec

    def setWeight( self, intLumi = None):
        if intLumi == None:
            intLumi = self.intLumi

        if type(self.xsec) is list:
            for bin,ixsec in enumerate(self.xsec):
                self.weight[bin] =  intLumi * ixsec
        else:
            self.weight = intLumi * self.xsec

### class Component ###

MC_TTbar = Component(
    scene = 'MC',
    name = 'TTbar',
    location = '/nfs/dust/cms/group/susy-desy/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/',
    xsec = 809.1)

print dir(MC_TTbar)

MC_WJets = Component(
    scene = 'MC',
    name = 'WJets',
    location = '/nfs/dust/cms/group/susy-desy/ISOTrck/WJets/',
    xsec = [2234.9,580.06,68.38,23.14],
    HTbins = ['100-200/','200-400/','400-600/','600-Inf/'])

Samples = [ MC_TTbar, MC_WJets ]

for samp in Samples:
    print samp.name, samp.intLumi, samp.weight
    samp.setWeight(10)
    print samp.name, samp.intLumi, samp.weight
