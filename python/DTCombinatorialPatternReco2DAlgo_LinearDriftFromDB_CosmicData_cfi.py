# The following comments couldn't be translated into the new config version:

# Parameters for the updator
# this is the RecHit1D algo!!

import FWCore.ParameterSet.Config as cms

# 
# This is the include file with the parameters
# for the DTCombinatorialPatternReco algorithm,
# which is the concrete algo for the DTRecSegment2D production.
# The linear Drift algos is used.
#
# The reconstruction algo and its parameter set
from RecoLocalMuon.DTRecHit.DTLinearDriftFromDBAlgo_CosmicData_cfi import *
DTCombinatorialPatternReco2DAlgo_LinearDriftFromDB_CosmicData = cms.PSet(
    Reco2DAlgoConfig = cms.PSet(
        DTLinearDriftFromDBAlgo_CosmicData,
        segmCleanerMode = cms.int32(1),
        AlphaMaxPhi = cms.double(100.0),
        MaxAllowedHits = cms.uint32(50),
        # Parameters for the cleaner
        nSharedHitsMax = cms.int32(2),
        AlphaMaxTheta = cms.double(100.0),
        debug = cms.untracked.bool(False)
    ),
    Reco2DAlgoName = cms.string('DTCombinatorialPatternReco')
)

