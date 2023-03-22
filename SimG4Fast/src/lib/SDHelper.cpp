#include "SimG4Fast/SDHelper.h"
#include "SimG4Fast/MeshEventInformation.h"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

namespace det {
k4::Geant4CaloHit* RetrieveAndSetupHit(G4THitsCollection<k4::Geant4CaloHit>* aHitsCollection, G4ThreeVector aPosition,
                              G4ThreeVector& aEntrancePosition, G4ThreeVector& aEntranceDirection,
                              G4ThreeVector aMeshSizeOfCells, G4ThreeVector aMeshNbOfCells)
{
  if(aEntrancePosition.x() == -1)
  {
    sim::MeshEventInformation* info = dynamic_cast<sim::MeshEventInformation*>(
      G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation());
    if(info == nullptr)
      return nullptr;
    aEntrancePosition  = info->GetPosition();
    aEntranceDirection = info->GetDirection();
  }

  auto delta = aPosition - aEntrancePosition;

  // Calculate rotation matrix along the particle momentum direction
  // It will rotate the shower axes to match the incoming particle direction
  G4RotationMatrix rotMatrix = G4RotationMatrix();
  double particleTheta       = aEntranceDirection.theta();
  double particlePhi         = aEntranceDirection.phi();
  rotMatrix.rotateZ(-particlePhi);
  rotMatrix.rotateY(-particleTheta);
  G4RotationMatrix rotMatrixInv = CLHEP::inverseOf(rotMatrix);

  delta = rotMatrix * delta;

  G4int rhoNo = std::floor(delta.perp() / aMeshSizeOfCells.x());
  G4int phiNo = std::floor((CLHEP::pi + delta.phi()) / aMeshSizeOfCells.y());
  G4int zNo   = std::floor(delta.z() / aMeshSizeOfCells.z());

  std::size_t hitID =
    aMeshNbOfCells.y() * aMeshNbOfCells.x() * zNo + aMeshNbOfCells.y() * rhoNo + phiNo;

  if(hitID >= aHitsCollection->entries() || zNo >= aMeshNbOfCells.z() ||
     rhoNo >= aMeshNbOfCells.x() || zNo < 0)
  {
    return nullptr;
  }

  int hitZ = floor(hitID / aMeshNbOfCells.y() / aMeshNbOfCells.x());
  int hitRho = floor((hitID % int(aMeshNbOfCells.x() * aMeshNbOfCells.y())) / aMeshNbOfCells.y());
  int hitPhi = int(hitID % int(aMeshNbOfCells.x() * aMeshNbOfCells.y()) % int(aMeshNbOfCells.y()));

  k4::Geant4CaloHit* hit = dynamic_cast<k4::Geant4CaloHit*>(aHitsCollection->GetHit(hitID));

return hit;
}
}
