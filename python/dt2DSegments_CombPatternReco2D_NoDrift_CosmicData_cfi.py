# The following comments couldn't be translated into the new config version:

# The reconstruction algo and its parameter set

import FWCore.ParameterSet.Config as cms

# Module for 2D rechit building 
# The algo uses the Linear Drift Velocity (which is a 1D rec hit algo)
# The block of the reconstruction
from RecoLocalMuon.DTSegment.DTCombinatorialPatternReco2DAlgo_NoDrift_CosmicData_cfi import *
dt2DSegments = cms.EDProducer("DTRecSegment2DProducer",
    DTCombinatorialPatternReco2DAlgo_NoDrift_CosmicData,
    # debuggin opt
    debug = cms.untracked.bool(False),
    # name of the rechit 1D collection in the event
    recHits1DLabel = cms.InputTag("dt1DRecHits")
)

