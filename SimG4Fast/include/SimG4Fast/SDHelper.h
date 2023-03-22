#ifndef SIMG4FAST_SDHELPER_H
#define SIMG4FAST_SDHELPER_H

// DD4hep
#include "SimG4Common/Geant4CaloHit.h"

// Geant
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

/**
 * @brief Sensitive detector helper.
 *
 * The common code between SD classes defined in RetrieveAdnSetupHit() method.
 *
 */

namespace det
{
/// Process energy deposit - common part for full and fast simulation
/// It is invoked from ProcessHits() methods, and sets basic hit properties
/// (position, etc.), common for hit from fast and full simulation.
  k4::Geant4CaloHit* RetrieveAndSetupHit(G4THitsCollection<k4::Geant4CaloHit>* aHitsCollection, G4ThreeVector aPosition,
                              G4ThreeVector& aEntrancePosition, G4ThreeVector& aEntranceDirection,
                              G4ThreeVector aMeshSizeOfCells, G4ThreeVector aMeshNbOfCells);
};

#endif /* SIMG4FAST_SDHELPER_H */
