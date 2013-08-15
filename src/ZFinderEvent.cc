#include "ZShape/ZFinder/interface/ZFidnerEvent.h"

ZFinderElectron* ZFinderEvent::allocateElectron() {
    return new ZFinderElectron(); // vanilla
}

void ZFinderEvent::setE0(const ZFinderElectron* electron){
    /* Set the class's pointer for e0 */
    e0 = electron;
}

void ZFinderEvent::setE1(const ZFinderElectron* electron){
    /* Set the class's pointer for e1 */
    e1 = electron;
}
