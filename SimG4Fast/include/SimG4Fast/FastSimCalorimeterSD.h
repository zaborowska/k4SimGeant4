#ifndef DETSENSITIVE_FASTSIMCALORIMETERSD_H
#define DETSENSITIVE_FASTSIMCALORIMETERSD_H

// DD4hep
#include "DDG4/Geant4Hits.h"
#include "SimG4Fast/Par04Hit.h"



// Geant
#include "G4THitsCollection.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"


/** FastSimCalorimeterSD DetectorDescription/DetSensitive/src/FastSimCalorimeterSD.h FastSimCalorimeterSD.h
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
class FastSimCalorimeterSD : public G4VSensitiveDetector {
public:
  /** Constructor.
   *  @param aDetectorName Name of the detector
   *  @param aReadoutName Name of the readout (used to name the collection)
   *  @param aSeg Segmentation of the detector (used to retrieve the cell ID)
   */
  FastSimCalorimeterSD(const std::string& aDetectorName,
                      const std::string& aReadoutName,
                      const dd4hep::Segmentation& aSeg);
  /// Destructor
  virtual ~FastSimCalorimeterSD();
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
  /** Process energy deposit - common part for full and fast simulation
   *   It is invoked from ProcessHits() methods, and sets basic hit properties
   *   (position, etc.), common for hit from fast and full simulation.
   */
  Par04Hit* RetrieveAndSetupHit(G4ThreeVector aPosition);

private:
  /// Collection of calorimeter hits
  G4THitsCollection<Par04Hit>* m_calorimeterCollection;
  /// Segmentation of the detector used to retrieve the cell Ids
  dd4hep::Segmentation m_seg;
  /// Number of mesh readout cells in cylindrical coordinates
  G4ThreeVector fMeshNbOfCells = { 18, 50, 48 };
  /// Size of mesh readout cells in cylindrical coordinates.
  G4ThreeVector fMeshSizeOfCells = { 2.325 * mm, 2 * CLHEP::pi / 50., 3.4 * mm };
  /// Retrieved once per event: position of entering particle
  G4ThreeVector fEntrancePosition = { 0, -1, -1 };
  /// Retrieved once per event: direction of entering particle
  G4ThreeVector fEntranceDirection = { -1, -1, -1 };

};
}

#endif /* DETSENSITIVE_FASTSIMCALORIMETERSD_H */
