#!/usr/bin/python

import sys
import os
import glob
sys.argv.append( '-b' )

from ROOT import *

'''
Here the functions for file handling are defined
'''

def walkDumpCanvs(plotfile,outdir='./',ext='.png'):

    plotdirlist = plotfile.GetListOfKeys()

    # set to 0 for normal, 1 for test
    switch = 0

    for refKey in plotdirlist:
        #if a folder
        if refKey.IsFolder() == 1:
            # create same folder
            canvDir = refKey.ReadObj()

            path=outdir+canvDir.GetName()
            os.mkdir(path)

            #loop over canvs inside
            for canvKey in canvDir.GetListOfKeys():
                canv = canvKey.ReadObj()
                # check whether canv
                if 'TCanvas' in str(type(canv)):
                    plotpath=path+'/'+canv.GetName()+ext
                    canv.SaveAs(plotpath)
                    print 'plotting', plotpath

            if switch:
                break
        # if not a folder
        else:
            canv = refKey.ReadObj()

            path=outdir

            # check whether canv
            if 'TCanvas' in str(type(canv)):
                canv.SaveAs(path+canv.GetName()+ext)

        print 80*'#'

    return 1

#copy(file1, outdir)

if __name__ == "__main__":

    # first argument is '-b' == for batch mode
    #    print sys.argv

    if len(sys.argv) > 2:
        plotfilename = sys.argv[1]

    if len(sys.argv) > 3:
        plotdir = sys.argv[2]
    else:
        plotdir='./'

    print 'Going to dumb plots in', plotdir

    plotfile = TFile.Open(plotfilename)

    if plotfile:
        walkDumpCanvs(plotfile,plotdir)
        plotfile.Close()

        print 80*'#'
        print 'Finished writing and closed files'

    else:
        print "Couldn't open file."
