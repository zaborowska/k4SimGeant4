#include "SimG4Fast/FastSimActions.h"
#include "SimG4Fast/Par04EventAction.h"             // for Par04EventAction
#include "SimG4Fast/Par04RunAction.h"               // for Par04RunAction

namespace sim {
FastSimActions::FastSimActions() : G4VUserActionInitialization() {}

FastSimActions::~FastSimActions() {}

void FastSimActions::Build() const {
   auto eventAction = new Par04EventAction();
   SetUserAction(eventAction);
   SetUserAction(new Par04RunAction(eventAction));
}
}
