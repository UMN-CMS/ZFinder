#ifndef ZFINDER_SETTERBASE_H_
#define ZFINDER_SETTERBASE_H_

// ZFinder Code
#include "ZFinderElectron.h"  // ZFinderElectron
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class SetterBase{
        public:
            // Constructor
            SetterBase() { }

            // Call this method on your ZFinderEvent and it will set cut for
            // all electrons using the SetCut_ method
            virtual void SetCuts(ZFinderEvent* zf_event);

        protected:
            virtual void SetCut_(ZFinderElectron* zf_elec) = 0;
    };
}  // namespace zf
#endif  // ZFINDER_SETTERBASE_H_
