#include "DD4hep/Detector.h"
#include "DDG4/Factories.h"

#include "SimG4Fast/FastSimCalorimeterSD.h"

namespace dd4hep {
namespace sim {

// All G4VSensitiveDetector are deleted by ~G4SDManager called by ~G4RunManagerKernel
// Factory method to create an instance of FastSimCalorimeterSD
static G4VSensitiveDetector* create_fastsim_calorimeter_sd(const std::string& aDetectorName,
                                                          dd4hep::Detector& aLcdd) {
  std::string readoutName = aLcdd.sensitiveDetector(aDetectorName).readout().name();
  return new det::FastSimCalorimeterSD(
      aDetectorName, readoutName, aLcdd.sensitiveDetector(aDetectorName).readout().segmentation());
}
}
}

DECLARE_EXTERNAL_GEANT4SENSITIVEDETECTOR(FastSimCalorimeterSD, dd4hep::sim::create_fastsim_calorimeter_sd)

