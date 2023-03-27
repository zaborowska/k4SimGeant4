#include "SimG4FastSimMeshHistograms.h"

// FCCSW
#include "GaudiKernel/ITHistSvc.h"
#include "SimG4Interface/IGeoSvc.h"

// datamodel
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "TH1F.h"
#include "DD4hep/Detector.h"
#include "DD4hep/Segmentations.h"

DECLARE_COMPONENT(SimG4FastSimMeshHistograms)

SimG4FastSimMeshHistograms::SimG4FastSimMeshHistograms(const std::string& aName, ISvcLocator* aSvcLoc)
: GaudiAlgorithm(aName, aSvcLoc), m_geoSvc("GeoSvc", aName) {
  declareProperty("MCparticles", m_MCparticles, "Handle for the EDM MC particles to be read");
  declareProperty("Hits", m_hits, "Handle for the EDM calo hits to be read");
}
SimG4FastSimMeshHistograms::~SimG4FastSimMeshHistograms() {}

StatusCode SimG4FastSimMeshHistograms::initialize() {
  if (GaudiAlgorithm::initialize().isFailure()) return StatusCode::FAILURE;

  if (!m_geoSvc) {
    error() << "Unable to locate Geometry Service. "
            << "Make sure you have GeoSvc and SimSvc in the right order in the configuration." << endmsg;
    return StatusCode::FAILURE;
  }
  auto lcdd = m_geoSvc->lcdd();
  auto allReadouts = lcdd->readouts();
  if (allReadouts.find(m_readoutName) == allReadouts.end()) {
    error() << "Readout " << m_readoutName << " not found! Please check tool configuration." << endmsg;
    return StatusCode::FAILURE;
  } else {
    debug() << "Hits will be saved to EDM from the collection " << m_readoutName << endmsg;
  }

  m_histSvc = service("THistSvc");
  if (!m_histSvc) {
    error() << "Unable to locate Histogram Service" << endmsg;
    return StatusCode::FAILURE;
  }

  m_mc_energy = new TH1F("MC_energy", "Energy distribution of MC particles", 1024, 0, 128);
  if (m_histSvc->regHist("/sim/mc_energy", m_mc_energy).isFailure()) {
    error() << "Couldn't register MC_energy histogram" << endmsg;
  }
  m_mc_pdg = new TH1F("MC_pdg", "PDG distribution of MC particles", 1024, 0, 128);
  if (m_histSvc->regHist("/sim/mc_pdg", m_mc_pdg).isFailure()) {
    error() << "Couldn't register MC_pdg histogram" << endmsg;
  }
  m_dep_energy = new TH1F("dep_energy", "Deposited energy distribution", 1024, 0, 128);
  if (m_histSvc->regHist("/sim/dep_energy", m_dep_energy).isFailure()) {
    error() << "Couldn't register dep_energy histogram" << endmsg;
  }
  m_ratio_energy = new TH1F("ratio_energy", "Ratio of deposited energy to MC particle energy", 1024, 0, 1);
  if (m_histSvc->regHist("/sim/ratio_energy", m_ratio_energy).isFailure()) {
    error() << "Couldn't register Ratio_energy histogram" << endmsg;
  }
  m_long_profile = new TH1F("long_profile", "Longitudinal profile", m_numZ,
                            -0.5 * m_sizeZ, (m_numZ - 0.5) * m_sizeZ);
  if (m_histSvc->regHist("/sim/long_profile", m_long_profile).isFailure()) {
    error() << "Couldn't register long_profile histogram" << endmsg;
  }
  m_long_first_moment = new TH1F("long_first_moment", "Longitudinal first moment",  1024, -0.5 * m_sizeZ,
                                 m_numZ * m_sizeZ);
  if (m_histSvc->regHist("/sim/long_first_moment", m_long_first_moment).isFailure()) {
    error() << "Couldn't register long_first_moment histogram" << endmsg;
  }
  m_long_second_moment = new TH1F("long_second_moment", "Longitudinal second moment",1024, 0, std::pow(m_numZ * m_sizeZ, 2) / 25);  // arbitrary scaling of max value on axis
  if (m_histSvc->regHist("/sim/long_second_moment", m_long_second_moment).isFailure()) {
    error() << "Couldn't register long_second_moment histogram" << endmsg;
  }
  m_trans_profile = new TH1F("trans_profile", "Transverse profile", m_numRho,
                             -0.5 * m_sizeRho, (m_numRho - 0.5) * m_sizeRho);
  if (m_histSvc->regHist("/sim/trans_profile", m_trans_profile).isFailure()) {
    error() << "Couldn't register trans_profile histogram" << endmsg;
  }
  m_trans_first_moment = new TH1F("trans_first_moment", "Transitudinal first moment",  1024, -0.5 * m_sizeRho, m_numRho * m_sizeRho);
  if (m_histSvc->regHist("/sim/trans_first_moment", m_trans_first_moment).isFailure()) {
    error() << "Couldn't register trans_first_moment histogram" << endmsg;
  }
  m_trans_second_moment = new TH1F("trans_second_moment", "Transitudinal second moment",1024, 0, std::pow(m_numRho * m_sizeRho, 2) / 5);  // arbitrary scaling of max value on axis
  if (m_histSvc->regHist("/sim/trans_second_moment", m_trans_second_moment).isFailure()) {
    error() << "Couldn't register trans_second_moment histogram" << endmsg;
  }
  return StatusCode::SUCCESS;
}

StatusCode SimG4FastSimMeshHistograms::execute() {
  const auto mc_particles = m_MCparticles.get();
  const auto hits = m_hits.get();
  double mc_energy = 0.;
  for (const auto& particle : *mc_particles) {
    mc_energy = particle.getEnergy();
    m_mc_energy->Fill(mc_energy);
    m_mc_pdg->Fill(particle.getPDG());
  }
  double hitEn                 = 0;
  double totalEnergy           = 0;
  int hitZ                     = -1;
  int hitRho                   = -1;
  int hitPhi                   = -1;
  int hitType                  = -1;
  std::uint64_t hitID = 0;
  int numNonZeroThresholdCells = 0;
  double tDistance = 0., rDistance = 0.;
  double tFirstMoment = 0., tSecondMoment = 0.;
  double rFirstMoment = 0., rSecondMoment = 0.;
  double phiMean = 0.;
  debug() << "Collection size: " << hits->size() << endmsg;

  for (const auto& hit : *hits) {
    hitEn = hit.getEnergy();
    if (hitEn > 0) {
      hitID = hit.getCellID();
      hitZ = int(hitID / m_numPhi / m_numRho);
      hitRho = int(hitID % int(m_numPhi * m_numRho) / m_numPhi);
      hitPhi = int(hitID % int(m_numPhi * m_numRho) % int(m_numPhi));
      totalEnergy += hitEn;
      tDistance = hitZ * m_sizeZ;
      rDistance = hitRho * m_sizeRho;
      tFirstMoment += hitEn * tDistance;
      rFirstMoment += hitEn * rDistance;
      m_long_profile->Fill(tDistance, hitEn);
      m_trans_profile->Fill(rDistance, hitEn);
    }
  }

  tFirstMoment /= totalEnergy;
  tSecondMoment /= totalEnergy;
  m_long_first_moment->Fill(tFirstMoment);
  m_trans_first_moment->Fill(rFirstMoment);
  m_dep_energy->Fill(totalEnergy);
  m_ratio_energy->Fill(totalEnergy / mc_energy);
  for (const auto& hit : *hits) {
    hitEn = hit.getEnergy();
    hitID = hit.getCellID();
    hitZ = int(hitID / m_numPhi / m_numRho);
    hitRho = int(hitID % int(m_numPhi * m_numRho) / m_numPhi);
    hitPhi = int(hitID % int(m_numPhi * m_numRho) % int(m_numPhi));
    if (hitEn > 0) {
      tDistance = hitZ * m_sizeZ;
      rDistance = hitRho * m_sizeRho;
      tSecondMoment += hitEn * std::pow(tDistance - tFirstMoment, 2);
      rSecondMoment += hitEn * std::pow(rDistance - rFirstMoment, 2);
    }
  }
  tSecondMoment /= totalEnergy;
  rSecondMoment /= totalEnergy;
  m_long_second_moment->Fill(tSecondMoment);
  m_trans_second_moment->Fill(rSecondMoment);
  return StatusCode::SUCCESS;
}

StatusCode SimG4FastSimMeshHistograms::finalize() { return GaudiAlgorithm::finalize(); }
