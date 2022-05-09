#include "SimG4FastGdmlDetector.h"

// FCCSW
#include "SimG4Fast/FastGdmlDetectorConstruction.h"

// Geant4
#include "G4VUserDetectorConstruction.hh"

DECLARE_COMPONENT(SimG4FastGdmlDetector)

SimG4FastGdmlDetector::SimG4FastGdmlDetector(const std::string& aType, const std::string& aName, const IInterface* aParent)
    : AlgTool(aType, aName, aParent) {
  declareInterface<ISimG4DetectorConstruction>(this);
}

SimG4FastGdmlDetector::~SimG4FastGdmlDetector() {}

StatusCode SimG4FastGdmlDetector::initialize() { return AlgTool::initialize(); }
StatusCode SimG4FastGdmlDetector::finalize() { return AlgTool::finalize(); }

G4VUserDetectorConstruction* SimG4FastGdmlDetector::detectorConstruction() {
  return new sim::FastGdmlDetectorConstruction(m_gdmlFile, m_validate);
}
