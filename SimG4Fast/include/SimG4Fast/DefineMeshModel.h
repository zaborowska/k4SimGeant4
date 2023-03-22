#ifndef DEFINEMESHMODEL_H
#define DEFINEMESHMODEL_H

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
 *
 */

namespace sim {
  class DefineMeshModel : public G4VFastSimulationModel
  {
    public:
    DefineMeshModel(G4String, G4Region*);
    DefineMeshModel(G4String);
    ~DefineMeshModel();
    /// Model is applicable to all particles.
    virtual G4bool ModelTrigger(const G4FastTrack&) final;
    /// Model is applicable to all particles.
    virtual G4bool IsApplicable(const G4ParticleDefinition&) final;
    /// Check particle direction, entrance point, and store it in event information.
    /// Then go back to the full simulation.
    virtual void DoIt(const G4FastTrack&, G4FastStep&) final;
  };
}
#endif /* DEFINEMESHMODEL_H */
