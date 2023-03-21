#include "SimG4Fast/DynamicMeshCalorimeterSD.h"

// k4SimGeant4
#include "SimG4Common/Geant4CaloHit.h"
#include "SimG4Fast/SDHelper.h"

// CLHEP
#include "CLHEP/Vector/ThreeVector.h"

// Geant4
#include "G4SDManager.hh"

namespace det {
DynamicMeshCalorimeterSD::DynamicMeshCalorimeterSD(const std::string& aDetectorName,
                                         const std::string& aReadoutName,
                                         const dd4hep::Segmentation&)
    : G4VSensitiveDetector(aDetectorName), m_calorimeterCollection(nullptr) {
  // name of the collection of hits is determined byt the readout name (from XML)
  collectionName.insert(aReadoutName);
}

DynamicMeshCalorimeterSD::~DynamicMeshCalorimeterSD() {}

void DynamicMeshCalorimeterSD::Initialize(G4HCofThisEvent* aHitsCollections) {
  // create a collection of hits and add it to G4HCofThisEvent
  // deleted in ~G4Event
  m_calorimeterCollection = new G4THitsCollection<k4::Geant4CaloHit>(SensitiveDetectorName, collectionName[0]);
  aHitsCollections->AddHitsCollection(G4SDManager::GetSDMpointer()->GetCollectionID(m_calorimeterCollection),
                                      m_calorimeterCollection);
  // fill calorimeter hits with zero energy deposition
  for (G4int iHit = 0; iHit < fMeshNbOfCells.x() * fMeshNbOfCells.y() * fMeshNbOfCells.z(); iHit++) {
    k4::Geant4CaloHit* hit = new k4::Geant4CaloHit(0, 0, 0, 0);
    hit->cellID = iHit;
    m_calorimeterCollection->insert(hit);
  }
  // reset entrance position
  fEntrancePosition.set(-1, -1, -1);
  fEntranceDirection.set(-1, -1, -1);
}

bool DynamicMeshCalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  // check if energy was deposited
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return false;

  auto hit = RetrieveAndSetupHit(m_calorimeterCollection, aStep->GetPostStepPoint()->GetPosition(),
                                 fEntrancePosition, fEntranceDirection,
                                 fMeshSizeOfCells, fMeshNbOfCells);
  if(hit == nullptr)
    return false;
  hit->energyDeposit += edep;

  return true;
}
}
