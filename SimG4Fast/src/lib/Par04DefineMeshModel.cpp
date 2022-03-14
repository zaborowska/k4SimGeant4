#include "SimG4Fast/Par04DefineMeshModel.h"
#include <G4FastTrack.hh>              // for G4FastTrack
#include <G4Track.hh>                  // for G4Track
#include <G4VFastSimulationModel.hh>   // for G4VFastSimulationModel
#include <G4VUserEventInformation.hh>  // for G4VUserEventInformation
#include "G4Event.hh"                  // for G4Event
#include "G4EventManager.hh"           // for G4EventManager
#include "SimG4Fast/Par04EventInformation.h"    // for Par04EventInformation
class G4FastStep;
class G4ParticleDefinition;
class G4Region;

namespace sim {
Par04DefineMeshModel::Par04DefineMeshModel(G4String aModelName, G4Region* aEnvelope)
  : G4VFastSimulationModel(aModelName, aEnvelope)
{}

Par04DefineMeshModel::Par04DefineMeshModel(G4String aModelName)
  : G4VFastSimulationModel(aModelName)
{}

Par04DefineMeshModel::~Par04DefineMeshModel() {}

G4bool Par04DefineMeshModel::IsApplicable(const G4ParticleDefinition&)
{
  return true;
}

G4bool Par04DefineMeshModel::ModelTrigger(const G4FastTrack&)
{
  Par04EventInformation* info = dynamic_cast<Par04EventInformation*>(
    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation());
  // check if particle direction and position were already set for this event
  if(info != nullptr)
    return !info->GetFlag();
  else
    return true;
}

void Par04DefineMeshModel::DoIt(const G4FastTrack& aFastTrack, G4FastStep&)
{
  Par04EventInformation* info = dynamic_cast<Par04EventInformation*>(
    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation());
  if(info == nullptr)
  {
    info = new Par04EventInformation();
    G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetUserInformation(info);
  }
  info->SetPosition(aFastTrack.GetPrimaryTrack()->GetPosition());
  info->SetDirection(aFastTrack.GetPrimaryTrack()->GetMomentumDirection());
  info->SetFlag(true);
}
}
