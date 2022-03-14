#ifndef PAR04DEFINEMESHMODEL_HH
#define PAR04DEFINEMESHMODEL_HH

#include <G4String.hh>                // for G4String
#include <G4Types.hh>                 // for G4bool
#include "G4VFastSimulationModel.hh"  // for G4VFastSimulationModel
class G4FastStep;
class G4FastTrack;
class G4ParticleDefinition;
class G4Region;

/**
 * @brief Defince mesh for energy scoring
 *
 * Fast simulation model is used in order to define where particle enters the detector
 * and its direction. That data is used to define the mesh used for energy scoring.
 * TODO To make the simulation faster, this model could be attached to a very thin layer at the
 * entrance to the calorimeter instead of attaching it to the volume of the detector.
 *
 */

namespace sim {
class Par04DefineMeshModel : public G4VFastSimulationModel
{
 public:
  Par04DefineMeshModel(G4String, G4Region*);
  Par04DefineMeshModel(G4String);
  ~Par04DefineMeshModel();

  /// Check if particle is entering the volume. Check particle energy. It must be
  /// no smaller than 99% of the primary particle energy. This is to ensure that in case of
  /// prior interactions, particle energy does not differ (much) from the assumed
  ///  energy.
  virtual G4bool ModelTrigger(const G4FastTrack&) final;
  /// Model is applicable to all particles.
  virtual G4bool IsApplicable(const G4ParticleDefinition&) final;
  /// Check particle direction, entrance point, and store it in event information.
  /// Then go back to the full simulation.
  virtual void DoIt(const G4FastTrack&, G4FastStep&) final;
};
}
#endif /* PAR04DEFINEMESHMODEL_HH */
