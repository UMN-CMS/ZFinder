# Select Events

Select events is a filter that selects events by event number.

## Usage

Add the following to your python configuration:

```python
# Event Number Filter
from ZFinder.SelectEvents.event_filter_cfi import EventFilter
process.EventFilter = EventFilter.clone(
    runs_to_save = cms.untracked.vuint32(
        2291311,
        2291312,
        2291313,
        2291314,
        2291315,
    )
)
```

And then add the process to the path:

```python
process.p = cms.Path(
    process.EventFilter
    ...
)
```
