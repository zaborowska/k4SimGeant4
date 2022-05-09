#ifndef PAR04EVENTACTION_HH
#define PAR04EVENTACTION_HH

#include <G4Types.hh>            // for G4int, G4double
#include <vector>                // for vector
#include "G4Timer.hh"            // for G4Timer
#include "G4UserEventAction.hh"  // for G4UserEventAction
#include "G4SystemOfUnits.hh"
class G4Event;

/**
 * @brief Event action class for hits' analysis.
 *
 * Analysis of single-particle events and developed showers in the detector.
 * At the end of the event basic variables are calculated and saved in the
 * histograms.
 * Additionally ntuple with cell energies and IDs (in cylindrical coordinates) is stored.
 *
 */
namespace sim {
class Par04EventAction : public G4UserEventAction
{
 public:
  Par04EventAction();
  virtual ~Par04EventAction();

  /// Timer is started
  virtual void BeginOfEventAction(const G4Event* aEvent) final;
  /// Hits collection is retrieved, analysed, and histograms are filled.
  virtual void EndOfEventAction(const G4Event* aEvent) final;
  inline std::vector<G4double>& GetCalEdep() { return fCalEdep; }
  inline std::vector<G4int>& GetCalRho() { return fCalRho; }
  inline std::vector<G4int>& GetCalPhi() { return fCalPhi; }
  inline std::vector<G4int>& GetCalZ() { return fCalZ; }
  inline G4int GetMeshCellNbZ() const {return fCellNbZ;}
  inline G4int GetMeshCellNbRho() const {return fCellNbRho;}
  inline G4int GetMeshCellNbPhi() const {return fCellNbPhi;}
  inline G4double GetMeshCellSizeZ() const {return fCellSizeZ;}
  inline G4double GetMeshCellSizeRho() const {return fCellSizeRho;}
  inline G4double GetMeshCellSizePhi() const {return fCellSizePhi;}

 private:
  /// ID of a hit collection to analyse
  G4int fHitCollectionID;
  /// Timer measurement
  G4Timer fTimer;
  /// Size of cell along Z axis
  G4double fCellSizeZ = 5.05;
  /// Size of cell along radius of cylinder
  G4double fCellSizeRho = 4.8975;
  /// Size of cell in azimuthal angle
  G4double fCellSizePhi = 2*CLHEP::pi/50.;
  /// Number of readout cells along radius
  G4int fCellNbRho = 18;
  /// Number of readout cells in azimuthal angle
  G4int fCellNbPhi = 50;
  /// Number of readout cells along z axis
  G4int fCellNbZ = 45;
  /// Cell energy deposits to be stored in ntuple
  std::vector<G4double> fCalEdep;
  /// Cell ID of radius to be stored in ntuple
  std::vector<G4int> fCalRho;
  /// Cell ID of azimuthal angle to be stored in ntuple
  std::vector<G4int> fCalPhi;
  /// Cell ID of z axis to be stored in ntuple
  std::vector<G4int> fCalZ;
};
}
#endif /* PAR04EVENTACTION_HH */
