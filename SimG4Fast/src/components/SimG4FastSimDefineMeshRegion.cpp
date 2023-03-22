#include "SimG4FastSimDefineMeshRegion.h"

// Geant4
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4RegionStore.hh"
#include "G4TransportationManager.hh"
#include "G4VFastSimulationModel.hh"
#include "SimG4Fast/DefineMeshModel.h"

#include "G4NistManager.hh"

DECLARE_COMPONENT(SimG4FastSimDefineMeshRegion)

SimG4FastSimDefineMeshRegion::SimG4FastSimDefineMeshRegion(const std::string& type, const std::string& name,
                                                           const IInterface* parent)
: GaudiTool(type, name, parent) {
  declareInterface<ISimG4RegionTool>(this);
}

SimG4FastSimDefineMeshRegion::~SimG4FastSimDefineMeshRegion() {}

StatusCode SimG4FastSimDefineMeshRegion::initialize() {
  if (GaudiTool::initialize().isFailure()) {
    return StatusCode::FAILURE;
  }
  if (m_volumeNames.size() == 0) {
    error() << "No detector name is specified for the parametrisation" << endmsg;
    return StatusCode::FAILURE;
  }
  if (m_minTriggerEnergy > m_maxTriggerEnergy) {
    error() << "Energy range is not defined properly" << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}

StatusCode SimG4FastSimDefineMeshRegion::finalize() { return GaudiTool::finalize(); }

StatusCode SimG4FastSimDefineMeshRegion::create() {
  G4LogicalVolume* world =
    (*G4TransportationManager::GetTransportationManager()->GetWorldsIterator())->GetLogicalVolume();
  for (const auto& calorimeterName : m_volumeNames) {
    for (int iter_region = 0; iter_region < world->GetNoDaughters(); ++iter_region) {
      if (world->GetDaughter(iter_region)->GetName().find(calorimeterName) != std::string::npos) {
        /// all G4Region objects are deleted by the G4RegionStore
        m_g4regions.emplace_back(
          new G4Region(world->GetDaughter(iter_region)->GetLogicalVolume()->GetName() + "_fastsim"));
        m_g4regions.back()->AddRootLogicalVolume(world->GetDaughter(iter_region)->GetLogicalVolume());
        m_models.emplace_back(new sim::DefineMeshModel(m_g4regions.back()->GetName(), m_g4regions.back()));
        info() << "Attaching a define mesh model to the region "
               << m_g4regions.back()->GetName() << endmsg;
      }
    }
  }
  return StatusCode::SUCCESS;
}
