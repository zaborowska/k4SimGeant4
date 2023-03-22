#include "SimG4Fast/MeshEventInformation.h"
#include <CLHEP/Vector/ThreeVector.h>  // for operator<<
#include <G4VUserEventInformation.hh>  // for G4VUserEventInformation
#include <G4ios.hh>                    // for G4cout, G4endl

namespace sim {
MeshEventInformation::MeshEventInformation()
  : G4VUserEventInformation()
  , fDirection()
  , fPosition()
{}

MeshEventInformation::~MeshEventInformation() {}
}
