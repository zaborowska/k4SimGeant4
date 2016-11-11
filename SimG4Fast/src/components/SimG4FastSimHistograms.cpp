#include "SimG4FastSimHistograms.h"

// FCCSW
#include "GaudiKernel/ITHistSvc.h"

// datamodel
#include "datamodel/ParticleCollection.h"
#include "datamodel/ParticleMCParticleAssociationCollection.h"
#include "datamodel/MCParticleCollection.h"

#include "TH1F.h"
#include "CLHEP/Vector/ThreeVector.h"


DECLARE_ALGORITHM_FACTORY(SimG4FastSimHistograms)

SimG4FastSimHistograms::SimG4FastSimHistograms(const std::string& aName, ISvcLocator* aSvcLoc):
  GaudiAlgorithm(aName, aSvcLoc),
  m_particlesMCparticles("particlesMCparticles", Gaudi::DataHandle::Reader, this),
  m_p(nullptr),
  m_diffP(nullptr),
  m_pdg(nullptr) {
  declareProperty("particlesMCparticles", m_particlesMCparticles);
}
SimG4FastSimHistograms::~SimG4FastSimHistograms() {}

StatusCode SimG4FastSimHistograms::initialize() {
  if (GaudiAlgorithm::initialize().isFailure())
    return StatusCode::FAILURE;
  m_histSvc = service("THistSvc");
  if (! m_histSvc) {
    error() << "Unable to locate Histogram Service" << endmsg;
    return StatusCode::FAILURE;
  }
  m_p = new TH1F("SmP", "Smeared particles momentum", 100, 0, 100);
  if (m_histSvc->regHist("/rec/SmP", m_p).isFailure()) {
    error() << "Couldn't register SmP histogram" << endmsg;
  }
  m_diffP = new TH1F("DiffP", "Smeared-MC particles momentum", 100, -0.5, 0.5);
  if (m_histSvc->regHist("/rec/DiffP", m_diffP).isFailure()) {
    error() << "Couldn't register DifP histogram" << endmsg;
  }
  m_eta = new TH1F("SmEta", "Smeared particles pseudorapidity", 100, -10, 10);
  if (m_histSvc->regHist("/rec/SmEta", m_eta).isFailure()) {
    error() << "Couldn't register SmEta histogram" << endmsg;
  }
  m_pdg = new TH1F("SmPdg", "Smeared particles PDG code", 4500, -2250, 2249);
  if (m_histSvc->regHist("/rec/SmPdg", m_pdg).isFailure()) {
    error() << "Couldn't register SmPdg histogram" << endmsg;
  }
  return StatusCode::SUCCESS;
}

StatusCode SimG4FastSimHistograms::execute() {
  const auto associations = m_particlesMCparticles.get();
  for(const auto& assoc : *associations) {
    const fcc::BareParticle& core = assoc.rec().core();
    CLHEP::Hep3Vector mom(core.p4.px, core.p4.py, core.p4.pz);
    m_eta->Fill(mom.eta());
    m_p->Fill(mom.mag());
    m_pdg->Fill(core.pdgId);
    const fcc::BareParticle& coreMC = assoc.sim().core();
    CLHEP::Hep3Vector momMC(coreMC.p4.px, coreMC.p4.py, coreMC.p4.pz);
    m_diffP->Fill((momMC.mag()-mom.mag())/momMC.mag());
  }
  return StatusCode::SUCCESS;
}

StatusCode SimG4FastSimHistograms::finalize() {
  return GaudiAlgorithm::finalize();
}
