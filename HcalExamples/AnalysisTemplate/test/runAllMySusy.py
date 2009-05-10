events = -1

import sys, os, os.path, string

if len(sys.argv) < 3:
    print "Specify point name, please!"
    sys.exit(-1)
point = sys.argv[2]
inputDir = "/usr/users/ratnikov/data/susy/v2/%s" % point

import FWCore.ParameterSet.Config as cms

def getFiles (dir, patterns):
    result = []
    if os.path.isdir (dir):
        files = os.listdir (dir)
        for file in files:
            matchedFile = True
            for pattern in patterns:
                if string.find (file, pattern) < 0:
                    matchedFile = False
                    break
            if matchedFile:
                result.append (os.path.join (dir, file))
    else:
        print "ERROR: ", dir, "is not a directory"
    return result

def inputFilesList (files):
    allFiles = []
    for file in files:
        allFiles.append ("file:%s" % file)
    return allFiles

inputFiles = inputFilesList (getFiles (inputDir, [".root", "layer1_SC5Jets_"]))
histFile = "%s/mySusyAnalysis_%s.root" % (inputDir, point)

print "Input Files:", inputFiles
print "histFile", histFile

import FWCore.ParameterSet.Config as cms

process = cms.Process('USER')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/EventContent/EventContent_cff')

process.load('RecoMET/METProducers/genMetFromGenJets_cfi')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('-s nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(events)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring (inputFiles))

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTEventContent.outputCommands,
    fileName = cms.untracked.string('test.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    )
)

process.TFileService = cms.Service("TFileService", fileName = cms.string(histFile))

process.mySusyAnalysis = cms.EDAnalyzer ("MySusyAnalysis")

# Additional output definition


# Path and EndPath definitions
process.p = cms.Path(process.mySusyAnalysis)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.p)
