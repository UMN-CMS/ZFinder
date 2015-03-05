#include "ZFinder/Event/interface/ZFinderElectron.h"

// Standard Library
#include <iostream>  // std::cout, std::endl;
#include <cmath>  // asin

// ROOT
#include "Math/VectorUtil.h"  // Phi_mpi_pi

// ZFinder
#include "ZFinder/Event/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)


namespace zf {
    ZFinderElectron::ZFinderElectron(reco::GsfElectron input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_GSFELECTRON;
        AddCutResult("type_gsf", true, 1.);
        gsf_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt_ = input_electron.pt();
        eta_ = input_electron.eta();
        phi_ = input_electron.phi();
        r9_ = input_electron.r9();
        sigma_ieta_ieta_ = input_electron.sigmaIetaIeta();
        h_over_e_ = input_electron.hadronicOverEm();
        deta_in_ = input_electron.deltaEtaSuperClusterTrackAtVtx();
        dphi_in_ = input_electron.deltaPhiSuperClusterTrackAtVtx();
        track_iso_ = input_electron.dr03TkSumPt();
        ecal_iso_ = input_electron.dr03EcalRecHitSumEt();
        hcal_iso_ = input_electron.dr03HcalTowerSumEt();
        one_over_e_mins_one_over_p_ = (1.0/input_electron.ecalEnergy() - input_electron.eSuperClusterOverP()/input_electron.ecalEnergy());
        charge_ = input_electron.charge();
        sc_eta_ = input_electron.superCluster()->eta();
        // Get SC phi and correct for the magnetic field
        using ROOT::Math::VectorUtil::Phi_mpi_pi;
        const double SC_PHI = input_electron.superCluster()->phi();
        const double EFFECTIVE_ECAL_RADIUS = EffectiveRadius(eta_);
        const double SC_PHI_CORRECTION = GetSCDeltaPhi(pt_, charge_, EFFECTIVE_ECAL_RADIUS);
        sc_phi_ = Phi_mpi_pi(SC_PHI - SC_PHI_CORRECTION);
    }

    ZFinderElectron::ZFinderElectron(reco::GenParticle input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_GENPARTICLE;
        AddCutResult("type_gen", true, 1.);
        gen_elec_ = input_electron;
        // reco::GenParticle is a child of reco::candidate
        candidate_ = dynamic_cast<reco::Candidate*>(&gen_elec_);
        /* Extract the useful quantities from a gen electron */
        pt_ = input_electron.pt();
        phi_ = input_electron.phi();
        eta_ = input_electron.eta();
        r9_ = -1;
        sc_eta_ = -10;
        sc_phi_ = -10;
        sigma_ieta_ieta_ = -1;
        h_over_e_ = -1;
        deta_in_ = -1;
        dphi_in_ = -1;
        track_iso_ = -1;
        ecal_iso_ = -1;
        hcal_iso_ = -1;
        one_over_e_mins_one_over_p_ = -1;
        // Using the input_electron Data Group ID Number, determine if the input_electron is an
        // electron or positron
        if (input_electron.pdgId() == PDGID::MUON) {
            charge_ = -1;
        }
        else if (input_electron.pdgId() == PDGID::ANTIMUON) {
            charge_ = 1;
        }
    }

    ZFinderElectron::ZFinderElectron(reco::GenParticle born_electron,
                                     reco::GenParticle dressed_electron,
                                     reco::GenParticle naked_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_GENPARTICLE;
        AddCutResult("type_gen", true, 1.);
        gen_elec_ = dressed_electron;
        // reco::GenParticle is a child of reco::candidate
        candidate_ = dynamic_cast<reco::Candidate*>(&gen_elec_);
        //here using only the dressed one, becaues it should share
        //all the properties with the other two, except kinematics
        /* Extract the useful quantities from a gen electron */
        pt_ = dressed_electron.pt();
        phi_ = dressed_electron.phi();
        eta_ = dressed_electron.eta();
        r9_ = -1;
        sigma_ieta_ieta_ = -1;
        h_over_e_ = -1;
        deta_in_ = -1;
        dphi_in_ = -1;
        track_iso_ = -1;
        ecal_iso_ = -1;
        hcal_iso_ = -1;
        one_over_e_mins_one_over_p_ = -1;
        sc_eta_ = -10;
        sc_phi_ = -10;
        //born:
        bornPt_ = born_electron.pt();
        bornPhi_ = born_electron.phi();
        bornEta_ = born_electron.eta();
        //naked:
        nakedPt_ = naked_electron.pt();
        nakedPhi_ = naked_electron.phi();
        nakedEta_ = naked_electron.eta();
        // Using the input_electron Data Group ID Number, determine if the input_electron is an
        // electron or positron
        if (dressed_electron.pdgId() == PDGID::MUON) {
            charge_ = -1;
        }
        else if (dressed_electron.pdgId() == PDGID::ANTIMUON) {
            charge_ = 1;
        }
    }

    ZFinderElectron::ZFinderElectron(reco::RecoEcalCandidate input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_RECOECALCANDIDATE;
        AddCutResult("type_ecalcandidate", true, 1.);
        recan_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt_ = input_electron.pt();
        eta_ = input_electron.eta();
        phi_ = input_electron.phi();
        r9_ = -1;
        sigma_ieta_ieta_ = -1;
        h_over_e_ = -1;
        deta_in_ = -1;
        dphi_in_ = -1;
        track_iso_ = -1;
        ecal_iso_ = -1;
        hcal_iso_ = -1;
        one_over_e_mins_one_over_p_ = -1;
        sc_eta_ = -10;
        sc_phi_ = -10;
        charge_ = input_electron.charge();
        // Add cut result to store parent information
    }

    ZFinderElectron::ZFinderElectron(reco::Photon input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_PHOTON;
        AddCutResult("type_photon", true, 1.);
        photon_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt_ = input_electron.pt();
        eta_ = input_electron.eta();
        phi_ = input_electron.phi();
        r9_ = input_electron.r9();
        sigma_ieta_ieta_ = -1;
        h_over_e_ = -1;
        deta_in_ = -1;
        dphi_in_ = -1;
        track_iso_ = -1;
        ecal_iso_ = -1;
        hcal_iso_ = -1;
        one_over_e_mins_one_over_p_ = -1;
        sc_eta_ = -10;
        sc_phi_ = -10;
        charge_ = 0;  // No charge because no tracker
    }

    ZFinderElectron::ZFinderElectron(trigger::TriggerObject input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_TRIGGER;
        AddCutResult("type_hlt", true, 1.);
        trig_elec_ = input_electron;
        candidate_ = nullptr;
        /* Extract the useful quantities from a GsfElectron */
        pt_ = input_electron.pt();
        eta_ = input_electron.eta();
        phi_ = input_electron.phi();
        r9_ = -1;
        sigma_ieta_ieta_ = -1;
        h_over_e_ = -1;
        deta_in_ = -1;
        dphi_in_ = -1;
        track_iso_ = -1;
        ecal_iso_ = -1;
        hcal_iso_ = -1;
        one_over_e_mins_one_over_p_ = -1;
        sc_eta_ = -10;
        sc_phi_ = -10;
        charge_ = 0;  // No charge
    }

    const CutResult* ZFinderElectron::GetCutResult(const std::string& cut_name) const {
        /* Return a CutResult based on the name */
        // Find the cut
        std::map<std::string, CutResult>::const_iterator i = cutresults_.find(cut_name);
        // Return a CutResult if it exists, otherwise return nullptr
        if (i != cutresults_.end()) {
            return &(i->second);
        }
        else {
            return nullptr;
        }
    }

    int ZFinderElectron::CutPassed(const std::string& cut_name) const {
        /* Return the passed status of a cut based on the name */
        const CutResult* cr = GetCutResult(cut_name);
        // If the CutResult exists, return the passed value, otherwise return -1
        if (cr != nullptr) {
            return cr->passed;
        }
        else {
            return -1;
        }
    }

    double ZFinderElectron::CutWeight(const std::string& cut_name) const {
        /* Return the weight of a cut based on the name */
        const CutResult* cr = GetCutResult(cut_name);
        // If the CutResult exists, return the weight, otherwise return -1.
        // This way calling code can decide what to do when no result exists.
        // Using either 0 or 1 might be appropriate depending on the case.
        if (cr != nullptr) {
            return cr->weight;
        }
        else {
            return -1.;
        }
    }

    void ZFinderElectron::AddCutResult(const std::string& cut_name, const bool passed, const double weight) {
        /* Given a name and passed status, will add it to the cutmap. It may also
         * be given a weight, but if not it is assumed to be 1.
         *
         * This function will overwrite previous entries with the same name!
         */
        CutResult cr;
        cr.passed = passed;
        cr.name = cut_name;
        cr.weight = weight;
        cutresults_[cut_name] = cr;  // This will overwrite
    }

    void ZFinderElectron::SetCutWeight(const std::string& cut_name, const double WEIGHT) {
        /* Given the name of a cut and a weight, sets the weight of the cut to
         * the new value.
         */
        const CutResult* cr = GetCutResult(cut_name);
        if (cr != nullptr) {
            AddCutResult(cr->name, cr->passed, WEIGHT);
        }
    }

    std::vector<const CutResult*>* ZFinderElectron::GetAllCuts() {
        /* Return all cuts */
        std::vector<const CutResult*>* tmp_vec = new std::vector<const CutResult*>();
        for (auto& i_cut : cutresults_) {
            tmp_vec->push_back(&(i_cut.second));
        }
        return tmp_vec;
    }

    std::vector<const CutResult*>* ZFinderElectron::GetCutsBool(const bool PASSED) {
        /* Finds all cuts that have passed matching PASSED */
        std::vector<const CutResult*>* tmp_vec = new std::vector<const CutResult*>();
        for (auto& i_cut : cutresults_) {
            if (i_cut.second.passed == PASSED) {
                tmp_vec->push_back(&(i_cut.second));
            }
        }
        return tmp_vec;
    }

    double ZFinderElectron::GetSCDeltaPhi(
            const double PT, // GeV
            const int CHARGE, // 1 or -1
            const double DETECTOR_RADIUS, // meters
            const int B_FIELD // Tesla
            )
    {
        /*
         * From Green's "The Physics of Particle Detectors" Equations 7.20 and
         * 7.21. The Factor of 3.33 comes from 1/q when p is given in GeV and
         * the magnetic field is given in Tesla as given by Green on page 133.
         *
         * 7.20 gives:
         *
         *      a_T = p_T / (q B)
         *
         * 7.21 gives:
         *
         *      sin(phi_at_radius - phi_0) = - r / (2 a_T)
         *
         * Where q is the charge, B is the field, r is the detector radius,
         * phi_0 is the angle the particle left the interaction point at, and
         * phi_at_radius is the angle of the location of the detector where the
         * particle impacted.
         *
         * We _assume_ that all particles have charge of 1, 0, or -1.
         *
         * Then we can solve and find:
         *
         *      phi_at_radius - phi_0 = asin( - r q B / (2 p_T))
         *
         *  In units where B is in Tesla and p_T is in GeV, 1/q = 3.33.
         */
        const double ONE_OVER_Q = 3.33;
        const double AT = PT * ONE_OVER_Q / B_FIELD;
        // Get the sign of the charge.
        const double SIGN = int(0 < CHARGE) - int(CHARGE < 0);
        const double INTERNAL = -SIGN * DETECTOR_RADIUS / (2 * AT);
        const double DELTA_PHI = std::asin(INTERNAL);

        return DELTA_PHI;
    }

    double ZFinderElectron::EffectiveRadius(const double ETA) {
        /*
         * Get the effective radius of ECAL, assuming it is a cylinder.
         */
        // Eta values of the end of EB and the start of EE
        const double MAX_EB = 1.4442;
        //const double MIN_EE = 1.56;
        // The radius of the barrel and the distance of the endcap from the
        // interaction point, in meters.
        const double EB_RADIUS = 1.29;
        const double EE_DISTANCE = 3.14;
        // If we're in the barrel, then the radius is just the radius of the
        // barrel.
        const double FETA = fabs(ETA);
        if (FETA < MAX_EB) {
            return EB_RADIUS;
        }
        /*
         * Otherwise we're in the endcap. This means that we have to calculate:
         *
         *  EE_DISTANCE * tan(theta)
         *
         * Where theta is the angle relative to the beam. In terms of eta:
         *
         *  exp(-eta) = tan(theta/2)
         *
         * We then use the double angle formula to get tan(theta) without
         * needing an atan.
         */
        const double TTOT = std::exp(-FETA);  // Tan(theta/2)
        const double TAN_THETA = (2 * TTOT) / (1 - (TTOT * TTOT));
        return EE_DISTANCE * TAN_THETA;
    }
}
