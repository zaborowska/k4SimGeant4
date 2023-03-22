#include "SimG4Fast/DefineMeshModel.h"
#include <G4FastTrack.hh>              // for G4FastTrack
#include <G4Track.hh>                  // for G4Track
#include <G4VFastSimulationModel.hh>   // for G4VFastSimulationModel
#include <G4VUserEventInformation.hh>  // for G4VUserEventInformation
#include "G4Event.hh"                  // for G4Event
#include "G4EventManager.hh"           // for G4EventManager
#include "SimG4Fast/MeshEventInformation.h"    // for EventInformation
class G4FastStep;
class G4ParticleDefinition;
class G4Region;

namespace sim {
DefineMeshModel::DefineMeshModel(G4String aModelName, G4Region* aEnvelope)
  : G4VFastSimulationModel(aModelName, aEnvelope)
{}

DefineMeshModel::DefineMeshModel(G4String aModelName)
  : G4VFastSimulationModel(aModelName)
{}

DefineMeshModel::~DefineMeshModel() {}

G4bool DefineMeshModel::IsApplicable(const G4ParticleDefinition&)
{
  return true;
}

G4bool DefineMeshModel::ModelTrigger(const G4FastTrack&)
{
  MeshEventInformation* info = dynamic_cast<MeshEventInformation*>(
    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation());
  // check if particle direction and position were already set for this event
  if(info != nullptr)
    return !info->GetFlag();
  else
    return true;
}

void DefineMeshModel::DoIt(const G4FastTrack& aFastTrack, G4FastStep&)
{
  MeshEventInformation* info = dynamic_cast<MeshEventInformation*>(
    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation());
  if(info == nullptr)
    {
      info = new MeshEventInformation();
      G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetUserInformation(info);
    }
  info->SetPosition(aFastTrack.GetPrimaryTrack()->GetPosition());
  info->SetDirection(aFastTrack.GetPrimaryTrack()->GetMomentumDirection());
  info->SetFlag(true);
}
}
