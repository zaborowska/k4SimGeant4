#ifndef PAR04RUNACTION_HH
#define PAR04RUNACTION_HH

#include "G4UserRunAction.hh"
#include <CLHEP/Units/SystemOfUnits.h>       // for GeV
#include <G4String.hh>                       // for G4String
#include <G4ThreeVector.hh>                  // for G4ThreeVector
#include <G4Types.hh>                        // for G4int
#include <G4VUserPrimaryGeneratorAction.hh>  // for G4VUserPrimaryGeneratorA...
#include <string>                            // for basic_string
#include "G4Event.hh"                        // for G4Event
#include "G4ParticleGun.hh"                  // for G4ParticleGun
#include "G4ParticleTable.hh"                // for G4ParticleTable
#include "G4SystemOfUnits.hh"                // for GeV
#include "SimG4Fast/Par04EventInformation.h"          // for Par04EventInformation
#include "SimG4Fast/Par04EventAction.h"
class G4ParticleDefinition;
class G4Run;

/**
 * @brief Run action
 *
 * Create analysis file and define control histograms for showers in detectors.
 * Histograms are configured taking into account the dimensions of the readout mesh.
 * Ntuple with hits is also stored. It contains energy of the primary particle,
 * coordinates (cylindrical) of the hit and the deposited energy.
 *
 */
namespace sim {
class Par04RunAction : public G4UserRunAction
{
 public:
  /// Constructor. Defines the histograms.
  Par04RunAction(Par04EventAction* aEventAction);
  virtual ~Par04RunAction();

  /// Open the file for the analysis
  virtual void BeginOfRunAction(const G4Run*) final;
  /// Write and close the file
  virtual void EndOfRunAction(const G4Run*) final;

 private:
  /// Pointer to event action to save hits
  Par04EventAction* fEventAction;
};
}
#endif /* PAR04RUNACTION_HH */
