// -*- C++ -*-
//
// Package:    CalibratedHighEtaPhotons
// Class:      CalibratedHighEtaPhotons
// 
/**\class CalibratedHighEtaPhotons CalibratedHighEtaPhotons.cc calibratedHighEtaPhotons/CalibratedHighEtaPhotons/src/CalibratedHighEtaPhotons.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alexey Finkel
//         Created:  Mon Mar 10 18:46:14 CDT 2014
// $Id$
//
//


// system include files
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"

#include "TVector3.h"
#include "TLorentzVector.h"



// class declaration
//

class CalibratedHighEtaPhotons : public edm::EDProducer {
   public:
      explicit CalibratedHighEtaPhotons(const edm::ParameterSet&);
      ~CalibratedHighEtaPhotons();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
      edm::InputTag photonTag_;
      edm::InputTag recHitTag_;
      
      typedef std::vector<std::pair<DetId,float> > HaFcollection; //hists and fractions collection
      
      int ix, iy;
      float constsN[40][40], constsP[40][40];//this may be a temporary approach...
      double rawHitSumEnergy, calHitSumEnergy; 
      TLorentzVector p4l;
      reco::Particle::LorentzVector newP4;
      double eta, phi, newE, newPt;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedHighEtaPhotons::CalibratedHighEtaPhotons(const edm::ParameterSet& iConfig)
{
   //now do what ever other initialization is needed
  photonTag_  = iConfig.getParameter<edm::InputTag>( "photons" );
  recHitTag_ =  iConfig.getParameter<edm::InputTag>( "ecalRecHits" );
  
  produces<reco::PhotonCollection>();
}


CalibratedHighEtaPhotons::~CalibratedHighEtaPhotons()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CalibratedHighEtaPhotons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

	edm::Handle<EcalRecHitCollection> ecalRecHits;
	iEvent.getByLabel(recHitTag_,ecalRecHits);
	const EcalRecHitCollection* theEErecHits = ecalRecHits.product() ;

	edm::Handle<reco::PhotonCollection> uncalibratedPhotons;
	iEvent.getByLabel(photonTag_,uncalibratedPhotons);
	
	std::auto_ptr<reco::PhotonCollection> calibratedHighEtaPhotons( new reco::PhotonCollection );
	
	//loop through photons, for high-eta ones get hits
	//then compute a correction to the energy, and create a new photon with corrected energy
	//put it into the new collection and put the collection into the event
	for(reco::PhotonCollection::const_iterator photIt = uncalibratedPhotons->begin(); photIt != uncalibratedPhotons->end(); photIt++)
	{
		if( fabs(photIt->p4().Eta())<2.5 )continue;
		rawHitSumEnergy = 0;
		calHitSumEnergy = 0;
		
		HaFcollection hafs = photIt->superCluster()->hitsAndFractions();
		
		for( HaFcollection::const_iterator hafIt = hafs.begin(); hafIt != hafs.end(); hafIt++ )
		{
			EEDetId eeId(hafIt->first);						
			EERecHitCollection::const_iterator itrechit = theEErecHits->find(eeId);
	        if (itrechit==theEErecHits->end()) continue;
	        ix = eeId.ix();
	        iy = eeId.iy();
	        rawHitSumEnergy += itrechit->energy();
	        if( (((ix-50.5)*(ix-50.5)+(iy-50.5)*(iy-50.5))  < 132.25) || (((ix-50.5)*(ix-50.5)+(iy-50.5)*(iy-50.5)) > 342.25) )//i.e. if hit is not in NT
	        {
	        	calHitSumEnergy += itrechit->energy();
	        }
	        else
	        {
	        	if(photIt->p4().eta()<0) calHitSumEnergy += itrechit->energy()*constsN[ix-30][iy-30]; 
		        else calHitSumEnergy += itrechit->energy()*constsP[ix-30][iy-30];
	        }
		}
		
		eta = photIt->p4().eta();
		phi = photIt->p4().phi();
		newE = photIt->p4().E()*calHitSumEnergy/rawHitSumEnergy;
		newPt = newE/cosh(eta);
		p4l.SetPtEtaPhiE(newPt, eta, phi, newE);
		
		//diagnostic cout:
		std::cout<<"Raw Hit Sum = "<<rawHitSumEnergy<<", Cal Hit Sum = "<<calHitSumEnergy<<std::endl;
		
		newP4 = reco::Particle::LorentzVector(p4l.X(),p4l.Y(),p4l.Z(),p4l.T());
		//now make a new photon from the old one
		reco::Photon newPhot = *photIt;
		newPhot.setP4(newP4);
		calibratedHighEtaPhotons->push_back(newPhot);
	}
	iEvent.put(calibratedHighEtaPhotons);
}

// ------------ method called once each job just before starting event loop  ------------
void 
CalibratedHighEtaPhotons::beginJob()
{
    std::ifstream ConstsFile;
       
    char line[1000];
    int cix, ciy, iz;
    float calConst, sigma;
    
    ConstsFile.open("/home/grad/finkel/work/ZFinder/CMSSW_5_3_13/src/photonBucket/CalibratedHighEtaPhotons/data/CalConsts_Data.txt");
	if(!ConstsFile.is_open())
	{
		std::cout<<"Failed to open Data constants file. Existing"<<std::endl;
		exit(1);
	}
    do
    {
        ConstsFile.getline(line, 1000)>>cix>>ciy>>iz>>calConst>>sigma;
        if( (((cix-50.5)*(cix-50.5)+(ciy-50.5)*(ciy-50.5))  < 132.25) || (((cix-50.5)*(cix-50.5)+(ciy-50.5)*(ciy-50.5)) > 342.25) )
        {
        	constsN[cix-30][ciy-30] = 1;
        	constsP[cix-30][ciy-30] = 1;
        	continue;
        }
        if(iz==1) 
        {
            constsP[cix-30][ciy-30] = calConst;
        }
        else if(iz==-1) 
        {
            constsN[cix-30][ciy-30] = calConst;
        }
        else {std::cout<<"Error reading in a cal const. Exiting."<<std::endl;exit(1);}
        
    } while(!ConstsFile.eof());
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalibratedHighEtaPhotons::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
CalibratedHighEtaPhotons::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
CalibratedHighEtaPhotons::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
CalibratedHighEtaPhotons::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
CalibratedHighEtaPhotons::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CalibratedHighEtaPhotons::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CalibratedHighEtaPhotons);
