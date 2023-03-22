#ifndef SIMG4FAST_DYNAMICMESHCALORIMETERSD_H
#define SIMG4FAST_DYNAMICMESHCALORIMETERSD_H

// k4SimGeant4
#include "SimG4Common/Geant4CaloHit.h"

// DD4hep
#include "DDG4/Defs.h"

// Geant
#include "G4THitsCollection.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"

namespace k4 {
class Geant4CaloHit;

}

/** DynamicMeshCalorimeterSD DetectorDescription/DetSensitive/src/DynamicMeshCalorimeterSD.h DynamicMeshCalorimeterSD.h
 *
 *  Simple sensitive detector for calorimeter.
 *  It is based on dd4hep::sim::Geant4GenericSD<Calorimeter> (but it is not identical).
 *  In particular, the position of the hit is set to G4Step::GetPreStepPoint() position.
 *  New hit is created for each energy deposit.
 *  No timing information is saved.
 *
 *  @author    Anna Zaborowska
 */

namespace det {
class DynamicMeshCalorimeterSD : public G4VSensitiveDetector {
public:
  /** Constructor.
   *  @param aDetectorName Name of the detector
   *  @param aReadoutName Name of the readout (used to name the collection)
   *  @param aSeg Segmentation of the detector (not used in this SD).
   */
  DynamicMeshCalorimeterSD(const std::string& aDetectorName,
                      const std::string& aReadoutName,
                      const dd4hep::Segmentation& aSeg);
  /// Destructor
  virtual ~DynamicMeshCalorimeterSD();
  /** Initialization.
   *  Creates the hit collection with the name passed in the constructor.
   *  The hit collection is registered in Geant.
   *  @param aHitsCollections Geant hits collection.
   */
  virtual void Initialize(G4HCofThisEvent* aHitsCollections) final;
  /** Process hit once the particle hit the sensitive volume.
   *  Checks if the energy deposit is larger than 0, calculates the position and cellID,
   *  saves that into the hit collection.
   *  New hit is created for each energy deposit.
   *  @param aStep Step in which particle deposited the energy.
   */
  virtual bool ProcessHits(G4Step* aStep, G4TouchableHistory*) final;

private:
  /// Collection of calorimeter hits
  G4THitsCollection<k4::Geant4CaloHit>* m_calorimeterCollection;
  /// Number of mesh readout cells in cylindrical coordinates
  G4ThreeVector fMeshNbOfCells = { 18, 50, 48 };
  /// Size of mesh readout cells in cylindrical coordinates.
  G4ThreeVector fMeshSizeOfCells = { 4.5 * mm , 2 * CLHEP::pi / 50., 5.05*mm };
  /// Retrieved once per event: position of entering particle
  G4ThreeVector fEntrancePosition = { 0, -1, -1 };
  /// Retrieved once per event: direction of entering particle
  G4ThreeVector fEntranceDirection = { -1, -1, -1 };
};
}

#endif /* SIMG4FAST_DYNAMICMESHCALORIMETERSD_H */
