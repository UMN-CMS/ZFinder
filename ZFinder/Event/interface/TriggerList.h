#ifndef ZFINDER_TRIGGERLIST_H_
#define ZFINDER_TRIGGERLIST_H_

/* These vectors contain all of the trigger filters that make up the triggers
 * we use. */

namespace zf {
    static const std::vector<std::string> ET_ET_TIGHT = {
        "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter"  // ET-ET Tight Leg
    };
    static const std::vector<std::string> ET_ET_DZ = {
        "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDZ"  // ET-ET Vertex distance
    };
    static const std::vector<std::string> ET_ET_LOOSE = {
        "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter"  // ET-ET Loose Leg
    };
    static const std::vector<std::string> ET_NT_ET_TIGHT = {
        "hltEle27TightIdLooseIsoTrackIsoFilter"  // ET-NT Tight ECAL Leg AND ET-HF TightLoose ECAL Leg
    };

    static const std::vector<std::string> ET_HF_ET_TIGHT = ET_NT_ET_TIGHT;

    static const std::vector<std::string> ET_HF_ET_LOOSE = {
        "hltEle23TightIdLooseIsoTrackIsoFilter"  // ET-HF LooseTight ECAL Leg
    };

    static const std::vector<std::string> ET_HF_HF_TIGHT = {
        "hltHFEMPt30TightFilter" // ET-HF LooseTight HF Leg (30pt)
    };

    static const std::vector<std::string> ET_HF_HF_LOOSE = {
        "hltHFEMTightFilter"  // ET-HF TightLoose HF Leg (15pt)
    };

    // The giant combined list
    static const std::vector<std::string> ALL_TRIGGERS = {
        "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDZ",
        "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter",
        "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter",
        "hltEle23TightIdLooseIsoTrackIsoFilter",
        "hltEle27TightIdLooseIsoTrackIsoFilter",
        "hltHFEMPt30TightFilter",
        "hltHFEMTightFilter"
    };

}  // namespace zf
#endif  // ZFINDER_TRIGGERLIST_H_
