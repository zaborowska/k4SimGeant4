#ifndef SIMG4FAST_SIMG4FASTSIMDEFINEMESHREGION_H
#define SIMG4FAST_SIMG4FASTSIMDEFINEMESHREGION_H

// Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/ToolHandle.h"

// FCCSW
#include "SimG4Interface/ISimG4GflashTool.h"
#include "SimG4Interface/ISimG4RegionTool.h"

// Geant
class G4VFastSimulationModel;
class G4Region;

/** @class SimG4FastSimDefineMeshRegion SimG4Fast/src/components/SimG4FastSimDefineMeshRegion.h
 * SimG4FastSimDefineMeshRegion.h
 *
 *  Tool for creating regions for fast simulation, attaching DefineMeshModel to them.
 *  Regions are created for volumes specified in the job options (\b'volumeNames').
 *
 *  @author Anna Zaborowska
 */

class SimG4FastSimDefineMeshRegion : public GaudiTool, virtual public ISimG4RegionTool {
  public:
  explicit SimG4FastSimDefineMeshRegion(const std::string& type, const std::string& name, const IInterface* parent);
  virtual ~SimG4FastSimDefineMeshRegion();
  /**  Initialize.
   *   @return status code
   */
  virtual StatusCode initialize() final;
  /**  Finalize.
   *   @return status code
   */
  virtual StatusCode finalize() final;
  /**  Create regions and fast simulation models
   *   @return status code
   */
  virtual StatusCode create() final;
  /**  Get the names of the volumes where fast simulation should be performed.
   *   @return vector of volume names
   */
  inline virtual const std::vector<std::string>& volumeNames() const final { return m_volumeNames; };

  private:
  /// Envelopes that are used in a parametric simulation
  /// deleted by the G4RegionStore
  std::vector<G4Region*> m_g4regions;
  /// Fast simulation (parametrisation) models
  std::vector<std::unique_ptr<G4VFastSimulationModel>> m_models;
  /// Names of the parametrised volumes (set by job options)
  Gaudi::Property<std::vector<std::string>> m_volumeNames{
    this, "volumeNames", {}, "Names of the parametrised volumes (set by job options)"};
  /// minimum energy of the electron (positron) that triggers the model
  Gaudi::Property<double> m_minTriggerEnergy{this, "minEnergy", 0.1 * Gaudi::Units::GeV,
      "minimum energy of the electron (positron) that triggers the model"};
  /// maximum energy of the electron (positron) that triggers the model
  Gaudi::Property<double> m_maxTriggerEnergy{this, "maxEnergy", 10 * Gaudi::Units::TeV,
      "maximum energy of the electron (positron) that triggers the model"};
  /// threshold below which the electrons (positrons) are killed
  Gaudi::Property<double> m_energyToKill{this, "killEnergy", 0.1 * Gaudi::Units::GeV,
      "threshold below which the electrons (positrons) are killed"};
};

#endif /* SIMG4FAST_SIMG4FASTSIMDEFINEMESHREGION_H */
