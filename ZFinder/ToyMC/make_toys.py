#!/usr/bin/env python

import ROOT
import array
from random import gauss, uniform, gammavariate
from math import pi as PI
from math import log, cosh, tan, sqrt

RUNS = int(1e6)

E_MASS = 5.11e-4

PT_SMEAR = 0.1
ETA_SMEAR = 0.01
PHI_SMEAR = 0.01

# Random distributions to draw from
def get_z_mass(MASS = 91.1876, WIDTH = 2.4952):
   """We model the Z mass as a Gaussian. """
   return gauss(MASS, WIDTH)

def get_phi():
    """ Assume a spherical decay. """
    return uniform(-PI, PI)

def get_theta():
    """ Assume a spherical decay. """
    return uniform(0, PI)

def get_z_eta():
    return uniform(-2, 2)

def get_pt(ALPHA=1.5, BETA=10):
    """ We assume a gamma distribution. """
    return gammavariate(ALPHA, BETA)

# Convert Coordinates
def get_eta_from_theta(theta):
    return -log(tan(theta/2.))

def convert_pt(p, eta):
    return sqrt(p*p) / cosh(eta)

# Smearing function
def get_smear(value, resolution=0.05):
    return value * gauss(1, resolution)

# Phistar
def get_phistar(eta0, phi0, eta1, phi1):
    dphi = phi0 - phi1
    # Account for 2pi = 0
    if (dphi < 0):
        if (dphi > -PI):
            dphi = abs(dphi)
        if (dphi < -PI):
            dphi += 2*PI

    if (dphi > PI):
        dphi = 2*PI - dphi

    deta = abs(eta0 - eta1)

    return ( ( 1. / cosh( deta / 2. ) ) * (1. / tan( dphi / 2. ) ) )

# Set up the histograms

DOUBLE = 'd'
PHISTAR_BIN_EDGES = array.array(
        DOUBLE,
        sorted([ # The very small first value is to prevent a root drawing error
            0.001, 0.004, 0.008, 0.012, 0.016, 0.020, 0.024, 0.029, 0.034,
            0.039, 0.045, 0.052, 0.057, 0.064, 0.072, 0.081, 0.091, 0.102,
            0.114, 0.128, 0.145, 0.165, 0.189, 0.219, 0.258, 0.312, 0.391,
            0.524, 0.695, 0.918, 1.153, 1.496, 1.947, 2.522, 3.277
            ])
        )

outfile = ROOT.TFile("toy_mc.root", "RECREATE")
outfile.cd()
unsmear_phi_hist = ROOT.TH1D("unsmeared_phistar", "unsmeared_phistar", len(PHISTAR_BIN_EDGES)-1, PHISTAR_BIN_EDGES)
smear_phi_hist = ROOT.TH1D("smeared_phistar", "smeared_phistar", len(PHISTAR_BIN_EDGES)-1, PHISTAR_BIN_EDGES)
unsmear_pt_hist = ROOT.TH1D("unsmeared_pt", "unsmeared_pt", 100, 0, 100)
smear_pt_hist = ROOT.TH1D("smeared_pt", "unsmeared_pt", 100, 0, 100)

count = 0
while count <= RUNS:
    count += 1

    # The Z at rest
    Z_MASS = get_z_mass()
    z_rest = ROOT.TLorentzVector(0, 0, 0, Z_MASS)

    # The Z boosted (so we can get a boost vector to boost the electrons)
    Z_PT = get_pt()
    #Z_ETA = get_eta_from_theta(get_theta())
    Z_ETA = get_z_eta()
    Z_PHI = get_phi()
    z_boost = ROOT.TLorentzVector()
    z_boost.SetPtEtaPhiM(Z_PT, Z_ETA, Z_PHI, Z_MASS)

    boost_vect = z_boost.BoostVector()

    # The First Electron
    E_ETA = get_eta_from_theta(get_theta())
    E_PHI = get_phi()
    E_MOMENTUM = Z_MASS / 2.
    E_PT = convert_pt(E_MOMENTUM, E_ETA)
    e0_vect = ROOT.TLorentzVector()
    e0_vect.SetPtEtaPhiM(E_PT, E_ETA, E_PHI, E_MASS)

    # The Second Electron
    e1_vect = z_rest - e0_vect

    # Boost the electrons
    e0_vect.Boost(boost_vect)
    e1_vect.Boost(boost_vect)

    # Smear
    pt0 = e0_vect.Perp()
    pt1 = e1_vect.Perp()
    smear_pt0 = get_smear(pt0, PT_SMEAR)
    smear_pt1 = get_smear(pt1, PT_SMEAR)

    eta0 = e0_vect.Eta()
    eta1 = e1_vect.Eta()
    smear_eta0 = get_smear(eta0, ETA_SMEAR)
    smear_eta1 = get_smear(eta1, ETA_SMEAR)

    phi0 = e0_vect.Phi()
    phi1 = e1_vect.Phi()
    smear_phi0 = get_smear(phi0, PHI_SMEAR)
    smear_phi1 = get_smear(phi1, PHI_SMEAR)

    phistar = get_phistar(eta0, phi0, eta1, phi1)
    smear_phistar = get_phistar(smear_eta0, smear_phi0, smear_eta1, smear_phi1)

    smear_e0_vect = ROOT.TLorentzVector()
    smear_e0_vect.SetPtEtaPhiM(smear_pt0, smear_eta0, smear_phi0, E_MASS)
    smear_e1_vect = ROOT.TLorentzVector()
    smear_e1_vect.SetPtEtaPhiM(smear_pt1, smear_eta1, smear_phi1, E_MASS)

    smear_z_pt = (smear_e0_vect + smear_e1_vect).Perp()

    # Fill Histograms
    unsmear_phi_hist.Fill(phistar)
    smear_phi_hist.Fill(smear_phistar)
    unsmear_pt_hist.Fill(Z_PT)
    smear_pt_hist.Fill(smear_z_pt)


#unsmear_phi_hist.Write()
#smear_phi_hist.Write()
#unsmear_pt_hist.Write()
#smear_pt_hist.Write()

outfile.Write()
outfile.Close()
