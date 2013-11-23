# ZFinder

## Using ZFinder

A basic analyzer will look like this:

```c++

// ZFinder
#include "ZFinder/ZFinder/interface/ZFinderEvent.h"  // ZFinderEvent
#include "ZFinder/ZFinder/interface/ZFinderPlotter.h"  // ZFinderPlotter

class ZFinder : public edm::EDAnalyzer {
    public:
        explicit ZFinder(const edm::ParameterSet&);
        ~ZFinder();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob() ;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

        // ----------member data ---------------------------
        const edm::ParameterSet& iConfig_;
        TFile* output_file_;
        TDirectory* tdir_;
        ZFinderPlotter* z_plotter;
};

ZFinder::ZFinder(const edm::ParameterSet& iConfig) : iConfig_(iConfig) {
    //now do what ever initialization is needed
    edm::Service<TFileService> ts;
    output_file_ = &(ts->file());
    tdir_ = output_file_->mkdir("outputdir", "outputdir");
    z_plotter = new ZFinderPlotter(tdir_);
}


void ZFinder::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    bool use_mc_truth = false;
    ZFinderEvent zfe(iEvent, iSetup, iConfig_, use_mc_truth);
    zfe.PrintElectrons();

    // Add plots
    z_plotter->Fill(zfe);
}
```
