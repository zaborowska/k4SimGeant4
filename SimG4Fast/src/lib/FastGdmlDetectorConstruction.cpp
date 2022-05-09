#include "SimG4Fast/FastGdmlDetectorConstruction.h"
#include "G4SDManager.hh"
#include "SimG4Fast/FastSimCalorimeterSD.h"

namespace sim {
FastGdmlDetectorConstruction::FastGdmlDetectorConstruction(const std::string& aFileName, bool validate)
    : m_msgSvc("MessageSvc", "FastGdmlDetectorConstruction"),
      m_log(&(*m_msgSvc), "FastGdmlDetectorConstruction"),
      m_fileName(aFileName) {
  m_parser.Read(m_fileName, validate);
}

FastGdmlDetectorConstruction::~FastGdmlDetectorConstruction() {}

G4VPhysicalVolume* FastGdmlDetectorConstruction::Construct() { return m_parser.GetWorldVolume(); }

void FastGdmlDetectorConstruction::ConstructSDandField() {
  // Example from Geant4 examples/extended/persistency/gdml/G04
  // First create and register sensitive detectors:
  // HOW TO USE: uncomment, use a proper implementation of the SensitiveDetector,
  // preferably using the Tracker Hits from DD4hep to simplify saving to the output
  det::FastSimCalorimeterSD* fastsimSD = new det::FastSimCalorimeterSD("ECalBarrelCollection","ECalBarrelCollection");
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector( fastsimSD );
  const G4GDMLAuxMapType* auxMap = m_parser.GetAuxMap();
  for (auto& entry : *auxMap) {
    for (auto& info : entry.second) {
      if (info.type == "SensDet") {
        // HOW TO USE: uncomment to attach SD registered above
        m_log<<MSG::INFO<<"Attaching a sensitive detector: "<<info.value<<" to volume: "<<entry.first->GetName()<<endmsg;
        if(fastsimSD) {
          entry.first->SetSensitiveDetector(fastsimSD);
        }
        else {
          m_log<<MSG::WARNING<<"GDML contains sensitive detector of type: "<<info.value<<" but it is not found"<<endmsg;
        }
      }
    }
  }
}
}
