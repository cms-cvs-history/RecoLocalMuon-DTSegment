# The following comments couldn't be translated into the new config version:

# Parameters for the updator
# this is the RecHit1D algo!!

# this are the RecSegment2D algo parameters!

import FWCore.ParameterSet.Config as cms

# 
# This is the include file with the parameters
# for the DTCombinatorialPatternReco algorithm,
# which is the concrete algo for the DTRecSegment4D production.
# The constant v drift algos is used.
#
# this is the RecSegment2D algo include!
from RecoLocalMuon.DTSegment.DTCombinatorialPatternReco2DAlgo_LinearDriftFromDB_cfi import *
# this is the RecHit1D algo include!
from RecoLocalMuon.DTRecHit.DTLinearDriftFromDBAlgo_cfi import *
DTCombinatorialPatternReco4DAlgo_LinearDriftFromDB = cms.PSet(
    Reco4DAlgoName = cms.string('DTCombinatorialPatternReco4D'),
    Reco4DAlgoConfig = cms.PSet(
        DTCombinatorialPatternReco2DAlgo_LinearDriftFromDB,
        DTLinearDriftFromDBAlgo,
        segmCleanerMode = cms.int32(1),
        nSharedHitsMax = cms.int32(2),
        debug = cms.untracked.bool(False),
        # Parameters for the cleaner
        nUnSharedHitsMin = cms.int32(2),
        # the input type. 
        # If true the instructions in setDTRecSegment2DContainer will be schipped and the 
        # theta segment will be recomputed from the 1D rechits
        # If false the theta segment will be taken from the Event. Caveat: in this case the
        # event must contain the 2D segments!
        AllDTRecHits = cms.bool(True)
    )
)
