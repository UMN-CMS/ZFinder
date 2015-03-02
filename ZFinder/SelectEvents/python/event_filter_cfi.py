import FWCore.ParameterSet.Config as cms

print "NOT SAVING ALL EVENTS"

# Runs that Nick at Northwestern has be we do not
nick_events = cms.untracked.vuint32(
    1,
    )

EventFilter = cms.EDFilter('SelectEvents',
    events_to_save = nick_events,
    )
