#ifndef SIMG4FAST_FASTSIMACTIONS_H
#define SIMG4FAST_FASTSIMACTIONS_H

#include <string>

// Geant4
#include "G4VUserActionInitialization.hh"

/** @class FastSimActions SimG4Fast/SimG4Fast/FastSimActions.h FastSimActions.h
 *  User action initialization for fast simulation.
 *  @author    Anna Zaborowska
 */

namespace sim {
class FastSimActions : public G4VUserActionInitialization {
public:
  explicit FastSimActions();
  explicit FastSimActions(std::string aOutputName);
  virtual ~FastSimActions();
  /// Create all user actions.
  virtual void Build() const final;
private:
  // Name of the output file, priduced by Geant4 actions
  std::string m_outputName = "defualtName.root";
};
}

#endif /* SIMG4FAST_FASTSIMACTIONS_H */
