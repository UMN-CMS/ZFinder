#!/usr/bin/env python

from sys import argv

india = argv[1]
minnesota = argv[2]

india_events = []
minnesota_events = []

print "Loading india text file"
with open(india) as f:
    for line in f:
        if not line.startswith('#'):
            event_number = int(line.split(',')[0])
            india_events.append(event_number)

print "Loading minnesota text file"
with open(minnesota) as f:
    for line in f:
        if not line.startswith('#'):
            event_number = int(line.split(',')[0])
            minnesota_events.append(event_number)

# Make sets
print "Making India set"
india_set = frozenset(india_events)
print "Making Minnesota set"
minnesota_set = frozenset(minnesota_events)
del india_events
del minnesota_events

print "Selecting unique Minnesota events"
minnesota_unique = minnesota_set.difference(india_set)

print "Loading minnesota text file"
with open(minnesota) as f:
    for line in f:
        if not line.startswith('#'):
            event_number = int(line.split(',')[0])
            if event_number in minnesota_unique:
                print line
