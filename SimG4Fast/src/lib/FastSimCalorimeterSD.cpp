#include "SimG4Fast/FastSimCalorimeterSD.h"

// FCCSW
#include "DetCommon/DetUtils.h"
#include "SimG4Fast/Par04Hit.h"
#include "SimG4Fast/Par04EventInformation.h"

// DD4hep
#include "DDG4/Defs.h"
#include "DDG4/Geant4Mapping.h"
#include "DDG4/Geant4VolumeManager.h"

// CLHEP
#include "CLHEP/Vector/ThreeVector.h"

// Geant4
#include "G4SDManager.hh"
#include "G4Event.hh"                  // for G4Event
#include "G4EventManager.hh"           // for G4EventManager

namespace det {
FastSimCalorimeterSD::FastSimCalorimeterSD(const std::string& aDetectorName,
                                         const std::string& aReadoutName,
                                         const dd4hep::Segmentation& aSeg)
    : G4VSensitiveDetector(aDetectorName), m_calorimeterCollection(nullptr), m_seg(aSeg) {
  // name of the collection of hits is determined byt the readout name (from XML)
  collectionName.insert(aReadoutName);
}

FastSimCalorimeterSD::~FastSimCalorimeterSD() {}

void FastSimCalorimeterSD::Initialize(G4HCofThisEvent* aHitsCollections) {
  // create a collection of hits and add it to G4HCofThisEvent
  // deleted in ~G4Event
  m_calorimeterCollection = new G4THitsCollection<Par04Hit>(SensitiveDetectorName, collectionName[0]);
  aHitsCollections->AddHitsCollection(G4SDManager::GetSDMpointer()->GetCollectionID(m_calorimeterCollection),
                                      m_calorimeterCollection);

  // fill calorimeter hits with zero energy deposition
  for(G4int iphi = 0; iphi < fMeshNbOfCells.y(); iphi++)
    for(G4int irho = 0; irho < fMeshNbOfCells.x(); irho++)
      for(G4int iz = 0; iz < fMeshNbOfCells.z(); iz++)
      {
        Par04Hit* hit = new Par04Hit();
        m_calorimeterCollection->insert(hit);
      }
  // reset entrance position
  fEntrancePosition.set(-1, -1, -1);
  fEntranceDirection.set(-1, -1, -1);

}

bool FastSimCalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  // check if energy was deposited
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return false;

  auto hit = RetrieveAndSetupHit(aStep->GetPostStepPoint()->GetPosition());
  if(hit == nullptr)
    return false;

  // Add energy deposit from G4Step
  hit->AddEdep(edep);

  // Fill time information from G4Step
  // If it's already filled, choose hit with earliest global time
  if(hit->GetTime() == -1 || hit->GetTime() > aStep->GetTrack()->GetGlobalTime())
    hit->SetTime(aStep->GetTrack()->GetGlobalTime());
}

Par04Hit* FastSimCalorimeterSD::RetrieveAndSetupHit(G4ThreeVector aGlobalPosition) {
  // Done once per event to set the readout mesh orientation and position
  if(fEntrancePosition.x() == -1)
  {
    sim::Par04EventInformation* info = dynamic_cast<sim::Par04EventInformation*>(
      G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation());
    if(info == nullptr)
      return nullptr;
    fEntrancePosition  = info->GetPosition();
    fEntranceDirection = info->GetDirection();
  }

  auto delta = aGlobalPosition - fEntrancePosition;

  // Calculate rotation matrix along the particle momentum direction
  // It will rotate the shower axes to match the incoming particle direction
  G4RotationMatrix rotMatrix = G4RotationMatrix();
  double particleTheta       = fEntranceDirection.theta();
  double particlePhi         = fEntranceDirection.phi();
  rotMatrix.rotateZ(-particlePhi);
  rotMatrix.rotateY(-particleTheta);
  G4RotationMatrix rotMatrixInv = CLHEP::inverseOf(rotMatrix);

  delta = rotMatrix * delta;

  G4int rhoNo = floor(delta.perp() / fMeshSizeOfCells.x());
  G4int phiNo = floor((CLHEP::pi + delta.phi()) / fMeshSizeOfCells.y());
  G4int zNo   = floor(delta.z() / fMeshSizeOfCells.z());

  std::size_t hitID =
    fMeshNbOfCells.x() * fMeshNbOfCells.z() * phiNo + fMeshNbOfCells.z() * rhoNo + zNo;

  if(hitID >= m_calorimeterCollection->entries() || zNo > fMeshNbOfCells.z() ||
     rhoNo > fMeshNbOfCells.x() || zNo < 0)
  {
    return nullptr;
  }

  Par04Hit* hit = (*m_calorimeterCollection)[hitID];

  if(hit->GetRhoId() < 0)
  {
    hit->SetRhoId(rhoNo);
    hit->SetPhiId(phiNo);
    hit->SetZid(zNo);
    hit->SetRot(rotMatrixInv);
    hit->SetPos(fEntrancePosition +
                rotMatrixInv * G4ThreeVector(0, 0, (zNo + 0.5) * fMeshSizeOfCells.z()));
  }
  return hit;
}
}
