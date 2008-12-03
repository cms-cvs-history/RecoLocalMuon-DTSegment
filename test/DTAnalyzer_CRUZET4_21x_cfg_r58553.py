# The following comments couldn't be translated into the new config version:

# Test the production of the complete chain:
# from digi to RecSegment 4D, through RecHit1D and RecSegment2D
# starting from real cosmic data from commissioning
# Up to now only the Linear Drift alogo is available for the
# commisioning data.
#

#    
# MessageLogger
#    
# avoid printout at each event...

#cms_conditions_data/CMS_COND_21X_ALIGNMENT"
import FWCore.ParameterSet.Config as cms

process = cms.Process("testDTAnalyzer")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    ),
    destinations = cms.untracked.vstring('cout')
)

#    
#
#                 R E C O N S T R U C T I O N 
#
#    
process.load("Geometry.CMSCommonData.cmsSimIdealGeometryXML_cfi")

#include "Geometry/MuonCommonData/data/muonIdealGeometryXML.cfi"
process.load("Geometry.DTGeometry.dtGeometry_cfi")

#include "Geometry/CSCGeometry/data/cscGeometry.cfi"
#
#      include "Geometry/RPCGeometry/data/rpcGeometry.cfi"
#
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

# The module for 1D DT RecHit building
# process.load("RecoLocalMuon.DTRecHit.dt1DRecHits_LinearDrift_CosmicData_cfi")
# process.DTLinearDriftAlgo_CosmicData.recAlgoConfig.hitResolution = 0.05
# process.DTLinearDriftAlgo_CosmicData.recAlgoConfig.tTrigModeConfig.kFactor = -1.00

#process.load("CalibMuon.DTCalibration.DTFakeVDriftESProducer_cfi")
process.load("RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff")
process.dt1DRecHits.dtDigiLabel = 'dtunpacker'
process.dt4DSegments.Reco4DAlgoConfig.AllDTRecHits = cms.bool(False)
process.dt4DSegments.recHits2DLabel= cms.InputTag("dt2DExtendedSegments")
# process.dt4DSegments.debug= cms.untracked.bool(True)
# process.dt4DSegments.Reco4DAlgoConfig.Reco2DAlgoConfig.debug= cms.untracked.bool(True)
process.DTCombinatorialPatternReco4DAlgo_LinearDriftFromDB_CosmicData.Reco4DAlgoConfig.segmCleanerMode = 1
# process.load("RecoLocalMuon.DTRecHit.dt1DRecHits_LinearDriftFromDB_CosmicData_cfi")
# #process.DTLinearDriftAlgo_CosmicData.recAlgoConfig.hitResolution = 0.05
# process.DTLinearDriftFromDBAlgo_CosmicData.recAlgoConfig.tTrigModeConfig.kFactor = -1.00

process.dt1DClusters = cms.EDFilter("DTClusterer",
    debug = cms.untracked.bool(False),
    minLayers = cms.uint32(3),
    minHits = cms.uint32(3),
    recHits1DLabel = cms.InputTag("dt1DRecHits")
)

process.dt2DExtendedSegments = cms.EDProducer("DTRecSegment2DExtendedProducer",
    process.DTCombinatorialPatternReco2DAlgo_LinearDriftFromDB_CosmicData,
    debug = cms.untracked.bool(False),
    recClusLabel = cms.InputTag("dt1DClusters"),
    recHits1DLabel = cms.InputTag("dt1DRecHits")
)
#process.dt2DExtendedSegments.Reco2DAlgoConfig.debug = cms.untracked.bool(True)
process.dt2DExtendedSegments.Reco2DAlgoConfig.segmCleanerMode = cms.int32(1)


# # _4DAlgo ##
# # process.load("RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_LinearDrift_CosmicData_cfi")
# process.load("RecoLocalMuon.DTSegment.dt4DSegments_CombPatternReco4D_LinearDriftFromDB_CosmicData_cfi")
# process.DTCombinatorialPatternReco4DAlgo_LinearDriftFromDB_CosmicData.Reco4DAlgoConfig.segmCleanerMode = 2

# Geometry
process.load("Geometry.CommonDetUnit.globalTrackingGeometry_cfi")

#process.load("MagneticField.Engine.uniformMagneticField_cfi") ##
process.load("Configuration.StandardSequences.MagneticField_0T_cff")



#  temporary....(and wrong!, just for 'formal' tests...)
#include "MagneticField/Engine/data/volumeBasedMagneticField.cfi"  ## For cmssw <21x
process.load("RecoMuon.DetLayers.muonDetLayerGeometry_cfi")

process.load("RecoMuon.MuonSeedGenerator.CosmicMuonSeedProducer_cfi")
process.CosmicMuonSeed.EnableCSCMeasurement = False

process.load("RecoMuon.TrackingTools.MuonServiceProxy_cff") ##those includes were inside cosmicMuons.cfi


process.load("RecoMuon.TrackingTools.MuonTrackLoader_cff") ## on versions previous to CMSSW200 (and after??).   


process.load("RecoMuon.CosmicMuonProducer.cosmicMuons_cfi")
process.cosmicMuons.TrajectoryBuilderParameters.EnableDTMeasurement = True
process.cosmicMuons.TrajectoryBuilderParameters.EnableCSCMeasurement = False
process.cosmicMuons.TrajectoryBuilderParameters.EnableRPCMeasurement = False

#replace cosmicMuons.TrajectoryBuilderParameters.BackwardMuonTrajectoryUpdatorParameters.Granularity= 0
#process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagator_cfi")

process.source = cms.Source("PoolSource",
###process.source = cms.Source("NewEventStreamFileReader",
    skipEvents = cms.untracked.uint32(0),
    firstRun = cms.untracked.uint32(0),
    fileMatchMode = cms.untracked.string('permissive'),
    #      untracked uint32 skipEvents = 1070
   fileNames = cms.untracked.vstring(
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/206314FC-FD70-DD11-9CEF-001617E30F4C.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/346D8541-FC70-DD11-827E-000423D98804.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/34AA9C4D-FB70-DD11-A92A-000423D98B28.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/3C89BAFC-FB70-DD11-8310-001617C3B5E4.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/543BD01D-FB70-DD11-A64F-001617C3B64C.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/6429F89A-FD70-DD11-8087-001617DF785A.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/94A0319C-FD70-DD11-BEE0-000423D9870C.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/98AD01E9-FC70-DD11-927F-000423D99BF2.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/9A2D54C0-FB70-DD11-B56C-001617DF785A.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/B0604D5E-FD70-DD11-9D95-000423D98EC4.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/B21EE62E-FC70-DD11-9EB6-000423D6006E.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/BE9B37D0-FD70-DD11-BC90-000423D987E0.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/C21D90F8-FB70-DD11-85E2-001617E30D12.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/E4EBFA8D-FC70-DD11-B681-001617E30D0A.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/EC9C4061-FD70-DD11-8DD5-000423D94A20.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/F8F80BD6-FA70-DD11-8713-000423D6B5C4.root',
       '/store/data/Commissioning08/BarrelMuon/RAW/CRUZET4_v1/000/058/553/FEF82AE5-FD70-DD11-B28E-001617E30D12.root'
   )                                        
)                                                
                                                 
                                                 
process.dtunpacker = cms.EDFilter("DTUnpackingModule",
    dataType = cms.string('DDU'),                
    useStandardFEDid = cms.untracked.bool(True), 
    fedbyType = cms.untracked.bool(True),        
    readOutParameters = cms.PSet(                
        debug = cms.untracked.bool(False),       
        rosParameters = cms.PSet(                
            writeSC = cms.untracked.bool(True),  
            # untracked bool localDAQ = true              #            # For local runs
            readingDDU = cms.untracked.bool(True),
            performDataIntegrityMonitor = cms.untracked.bool(False),
            readDDUIDfromDDU = cms.untracked.bool(True), ##
                                                 
            debug = cms.untracked.bool(False),   
            localDAQ = cms.untracked.bool(False) 
        ),                                       
        localDAQ = cms.untracked.bool(False),    
        performDataIntegrityMonitor = cms.untracked.bool(False)
    )                                            
)                                                
                                                 
process.GlobalPositionSource = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,                         
    toGet = cms.VPSet(cms.PSet(                  
        record = cms.string('GlobalPositionRcd'),
        tag = cms.string('IdealGeometry')        
    ),                                           
        cms.PSet(                                
            record = cms.string('DTAlignmentRcd'),
            tag = cms.string('DTIdealGeometry200_mc')
        ),                                       
        cms.PSet(                                
            record = cms.string('DTAlignmentErrorRcd'),
            tag = cms.string('DTIdealGeometryErrors200_mc')
        ),                                       
        cms.PSet(                                
            record = cms.string('CSCAlignmentRcd'),
            tag = cms.string('CSCIdealGeometry200_mc')
        ),                                       
        cms.PSet(                                
            record = cms.string('CSCAlignmentErrorRcd'),
            tag = cms.string('CSCIdealGeometryErrors200_mc')
        ),                                       
        cms.PSet(                                
            record = cms.string('TrackerAlignmentRcd'),
            tag = cms.string('TrackerIdealGeometry210_mc')
        ),                                       
        cms.PSet(                                
            record = cms.string('TrackerAlignmentErrorRcd'),
            tag = cms.string('TrackerIdealGeometryErrors210_mc')
        )),                                      
    connect = cms.string('frontier://cms_conditions_data/CMS_COND_21X_ALIGNMENT')
)                                                
                                                 
process.roMapping = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,                         
    timetype = cms.string('runnumber'),          
    toGet = cms.VPSet(cms.PSet(                  
        record = cms.string('DTReadOutMappingRcd'),
        tag = cms.string('map_CRUZET')           
    ),                                           
        cms.PSet(                                
            record = cms.string('DTT0Rcd'),      
            tag = cms.string('t0_CRUZET_hlt')    
        ),                                       
        cms.PSet(                                
            record = cms.string('DTTtrigRcd'),   
            ###tag = cms.string('tTrig_CRUZET_080708_2019')
            tag = cms.string('tTrig_CRUZET_080820_1535')
        ),                                       
        cms.PSet(                                
            record = cms.string('DTStatusFlagRcd'),
            tag = cms.string('noise_CRUZET_hlt') 
        )),                                      
    #connect = cms.string('frontier://Frontier/CMS_COND_21X_DT'),
    connect = cms.string('frontier://Frontier/CMS_COND_20X_DT'),
    authenticationMethod = cms.untracked.uint32(0)
)                                                
                                                 
process.vdrift = cms.ESSource("PoolDBESSource",
    process.CondDBSetup,                         
    timetype = cms.string('runnumber'),          
    toGet = cms.VPSet(
      cms.PSet(                                
        record = cms.string('DTMtimeRcd'),   
        tag = cms.string('vDrift_LHCStartUp_555_CMSSW219')
        ),                                       
      ),
    connect = cms.string('frontier://Frontier/CMS_COND_21X_DT'),
    #connect = cms.string('frontier://Frontier/CMS_COND_20X_DT'),
    authenticationMethod = cms.untracked.uint32(0)
)                                                
                                                 
# ===================================================
#  7) configuration of your analyzer
# ===================================================
process.load("RecoLocalMuon.DTSegment.test.DTFilter_cfi")

# Hits and Segs ##
process.load("RecoLocalMuon.DTSegment.test.DTAnalyzerDetailed_cfi")

# Segs ##
process.load("RecoLocalMuon.DTSegment.test.DTSegAnalyzer_cfi")
process.DTSegAnalyzer.recHits2DLabel = cms.string('dt2DExtendedSegments')

# Clusters ##     
process.load("RecoLocalMuon.DTSegment.test.DTClusAnalyzer_cfi")
process.DTClusAnalyzer.recHits2DLabel = cms.string('dt2DExtendedSegments')

# StandAlone ##
process.load("RecoLocalMuon.DTSegment.test.STAnalyzer_cfi")
process.STAnalyzer.SALabel = cms.string('cosmicMuons')
process.STAnalyzer.recHits2DLabel = cms.string('dt2DExtendedSegments')
# process.STAnalyzer.debug = cms.untracked.bool(True)

# Segs Eff ##
process.load("RecoLocalMuon.DTSegment.test.DTEffAnalyzer_cfi")

# Validation RecHits
process.load("Validation.DTRecHits.test.DTRecHitQuality_cfi")

process.options = cms.untracked.PSet(
    #FailPath = cms.untracked.vstring('ProductNotFound'),
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

process.muonLocalReco = cms.Sequence(
        process.dtunpacker *
        # process.dtlocalreco_with_2DSegments *
        process.dt1DRecHits *
        process.dt1DClusters *
        process.dt2DExtendedSegments *
        process.dt4DSegments *
        # process.dt1DRecHits *
        # process.dt2DSegments *
        # process.dt4DSegments *
        process.offlineBeamSpot *
        process.CosmicMuonSeed *
        process.cosmicMuons
        )

process.ana = cms.Sequence(
        process.DTClusAnalyzer +
        process.DTSegAnalyzer +
        process.STAnalyzer +
        process.DTEffAnalyzer
        # process.dtLocalRecoValidation
        )

process.p = cms.Path(
    process.muonLocalReco *
    process.ana
    )

# process.out = cms.OutputModule ("PoolOutputModule",
#   fileName =cms.untracked.string('output.root')
# )

# process.endpath = cms.Path( process.out )

temp = process.dumpPython()
outputfile = file("merdaputtanatroia.py",'w')
outputfile.write(temp)
outputfile.close()

