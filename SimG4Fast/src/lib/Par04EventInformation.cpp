#include "SimG4Fast/Par04EventInformation.h"
#include <CLHEP/Vector/ThreeVector.h>  // for operator<<
#include <G4VUserEventInformation.hh>  // for G4VUserEventInformation
#include <G4ios.hh>                    // for G4cout, G4endl
#include <ostream>                     // for operator<<, basic_ostream, ost...

namespace sim {
Par04EventInformation::Par04EventInformation()
  : G4VUserEventInformation()
  , fDirection()
  , fPosition()
{}

Par04EventInformation::~Par04EventInformation() {}

void Par04EventInformation::Print() const
{
  G4cout << "Event information\nPrimary particle direction = " << fDirection
         << "\nPrimary particle position = " << fPosition << G4endl;
}
}
