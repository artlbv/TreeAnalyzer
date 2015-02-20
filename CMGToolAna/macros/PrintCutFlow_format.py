#!/usr/bin/python

import sys
import os
import glob
import itertools as IT

sys.argv.append( '-b' )

from ROOT import *

# samples order:
#sampList = ['cuts','TTZ','TTW','WJets','QCD','TTbar','DYJets','SingleTop','T1tttt_1500_100','T1tttt_1200_800']
sampList = ['cuts','WJets','TTbar','QCD','bkgs','T1tttt_1500_100','T1tttt_1200_800']


def simplePrint(cfdict):

    print(''.join(['{0:20}'.format(key) for key in sampList]))

    # number of cuts
    ncuts = len(cfdict['cuts'])

    for bin in range(0,ncuts):
        print(''.join(['{0:20}'.format(str(cfdict[samp][bin])) for samp in sampList]))

def texPrint(cfdict):

#    print(''.join(['{0:20}& '.format(key) for key in sampList]))
    sampnames = (''.join(['{0:20}& '.format(key) for key in sampList]))
    print sampnames[:-2]+' \\\ \hline'

    # number of cuts
    ncuts = len(cfdict['cuts'])

    for bin in range(0,ncuts):
        events = (''.join(['{0:20}& '.format(str(cfdict[samp][bin])) for samp in sampList]))
        print events[:-2]+' \\\ \hline'


def printTable(cfdict):

    matrix = zip(*[value if isinstance(value, list) else IT.repeat(value) for key,value in cfdict.items()])

    print(''.join(['{0:20}'.format(key) for key in cfdict.keys()]))
    for row in matrix:
        print(''.join(['{0:20}'.format(str(item)) for item in row]))

'''
Here the functions for file handling are defined
'''
def findHisto(file,name):
    for cutKey in file.GetListOfKeys():
        if cutKey.IsFolder() == 1:
            histDir = cutKey.ReadObj()
            for histKey in histDir.GetListOfKeys():
                if(histKey.GetName() == name):
                    return histKey.ReadObj()

        if(cutKey.GetName() == name):
            return cutKey.ReadObj()

    else:
        return 0

def dumpCF(plotfile,outname):

    canv = findHisto(plotfile,'CutFlow')

    if canv == 0:
        print "Error: couldn't find CutFlow plot in file"
        exit(0)

    cfEvents = {}
    cfCount = {}

    storedCuts = False

    for obj in canv.GetListOfPrimitives():
        if 'TLegend' in str(type(obj)):
            for entry in obj.GetListOfPrimitives():
                # histogram name
                sample =  entry.GetLabel()
                print 'Sample is', sample,
                hist = entry.GetObject()
#                wEvts = hist.Integral()
#                print 'Weighted entries', wEvts

                nbins = stack.GetNbinsX()+1

                evlist = []

                for bin in range(1,nbins):
                    evts = hist.GetBinContent(bin)
#                    print 'Events in bin', bin, evts
                    evlist.append(evts)

                if not storedCuts:
                    cutlist = []
                    for bin in range(1,nbins):
                        cutlist.append(hist.GetXaxis().GetBinLabel(bin))

                        cfEvents['cuts'] = cutlist
                        storedCuts = True


                print 'Events in', sample, evlist
                # store event number list in dict
                cfEvents[sample] = evlist

        if 'THStack' in str(type(obj)):
            stack = obj.GetStack().Last()
            nbins = stack.GetNbinsX()+1
            evlist = []

            for bin in range(1,nbins):
                evts = stack.GetBinContent(bin)
                evlist.append(evts)

            cfEvents['bkgs'] = evlist


    print 80*'#'
    # write to file
    sys.stdout = open(outname, "w")
#    printTable(cfEvents)
#    simplePrint(cfEvents)
    texPrint(cfEvents)
    sys.stdout = sys.__stdout__

#    outfile.close()
    return 1

if __name__ == "__main__":

    # first argument is '-b' == for batch mode
    #    print sys.argv

    if len(sys.argv) > 2:
        plotfilename = sys.argv[1]

    if len(sys.argv) > 3:
        cf_file = sys.argv[2]
    else:
        cf_file='cutflow'

    cf_file+='.txt'

    print 'Going to write out cutflow numbers to', cf_file

    plotfile = TFile.Open(plotfilename)

    if plotfile:
        dumpCF(plotfile,cf_file)
        plotfile.Close()

        print 80*'#'
        print 'Finished writing and closed files'

    else:
        print "Couldn't open file."
