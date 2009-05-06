import FWCore.ParameterSet.Config as cms

process = cms.Process('USER')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/EventContent/EventContent_cff')

process.load('RecoMET/METProducers/genMetFromGenJets_cfi')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.99.2.3 $'),
    annotation = cms.untracked.string('-s nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:/usr/users/ratnikov/cms/dev/CMSSW_2_2_1/src/susyscan_221_slha21X.root'
#    'file:/usr/users/ratnikov/cms/SUSYSCAN/CMSSW_2_2_3/susyscan_LM9_sftsdkpyt.root'
#    'file:/usr/users/ratnikov/cms/SUSYSCAN/CMSSW_2_2_3/susyscan_14501750_513.root',
#    'file:/usr/users/ratnikov/cms/SUSYSCAN/CMSSW_2_2_3/susyscan_14501850_543.root'
    )
#    fileNames = cms.untracked.vstring('file:/scratch/ratnikov/cms/dev/CMSSW_2_2_3/src/crab_0_090425_224248/res/test_1.root')
)
# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTEventContent.outputCommands,
    fileName = cms.untracked.string('test.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    )
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("ScanSUSYData_14501750_513.root"))

process.genEventProcID = cms.EDProducer("GenEventProcIDProducer", src = cms.InputTag("source"))
process.scanSUSY = cms.EDAnalyzer ("ScanSUSYData", reweight = cms.untracked.bool(False) )

# Additional output definition


# Path and EndPath definitions
process.p = cms.Path(process.genEventProcID * process.genMetIC5GenJets * process.scanSUSY)
process.p2 = cms.Path(process.scanSUSY)
process.out_step = cms.EndPath(process.output)

# Schedule definition
#process.schedule = cms.Schedule(process.p, process.out_step)
#process.schedule = cms.Schedule(process.p)
process.schedule = cms.Schedule(process.p2)
