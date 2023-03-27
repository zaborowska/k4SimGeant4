#ifndef SIMG4FAST_G4FASTSIMMESHHISTOGRAMS_H
#define SIMG4FAST_G4FASTSIMMESHHISTOGRAMS_H

// GAUDI
#include "GaudiAlg/GaudiAlgorithm.h"

// FCCSW
#include "k4FWCore/DataHandle.h"
class ITHistSvc;
class IGeoSvc;

// datamodel
namespace edm4hep {
class MCParticleCollection;
class SimCalorimeterHitCollection;
}

class TH1F;

/** @class SimG4FastSimMeshHistograms SimG4Components/src/SimG4FastSimMeshHistograms.h SimG4FastSimMeshHistograms.h
 *
 *  Fast shower simulation histograms algorithm.
 *  Fills validation histograms.
 *
 *  @author Anna Zaborowska
 */

class SimG4FastSimMeshHistograms : public GaudiAlgorithm {
public:
  explicit SimG4FastSimMeshHistograms(const std::string&, ISvcLocator*);
  virtual ~SimG4FastSimMeshHistograms();
  /**  Initialize.
   *   @return status code
   */
  virtual StatusCode initialize() final;
  /**  Fills the histograms.
   *   @return status code
   */
  virtual StatusCode execute() final;
  /**  Finalize.
   *   @return status code
   */
  virtual StatusCode finalize() final;

private:
  /// Pointer to the geometry service
  ServiceHandle<IGeoSvc> m_geoSvc;
  /// Name of the readouts (hits collections)
  Gaudi::Property<std::string> m_readoutName{this, "readoutName", {}, "Name of the readouts (hits collections)"};
  /// Size of the mesh voxel in rho
  Gaudi::Property<double> m_sizeRho{this, "meshSizeRho", 4.5, "Size of single mesh voxel dimension in rho"};
  /// Size of the mesh voxel in z
  Gaudi::Property<double> m_sizeZ{this, "meshSizeZ", 5.05, "Size of single mesh voxel dimension in z"};
  /// Number of the mesh voxels in z
  Gaudi::Property<int> m_numRho{this, "meshNumRho", 18, "Number of mesh voxels in rho"};
  /// Number of the mesh voxels in z
  Gaudi::Property<int> m_numPhi{this, "meshNumPhi", 50, "Number of mesh voxels in phi"};
  /// Number of the mesh voxels in z
  Gaudi::Property<int> m_numZ{this, "meshNumZ", 45, "Number of mesh voxels in z"};
  /// Handle for the EDM MC particles to be read
  DataHandle<edm4hep::MCParticleCollection> m_MCparticles{"MCparticles", Gaudi::DataHandle::Reader, this};
  /// Handle for the EDM MC particles to be read
  DataHandle<edm4hep::SimCalorimeterHitCollection> m_hits{"Hits", Gaudi::DataHandle::Reader, this};
  /// Pointer to the interface of histogram service
  SmartIF<ITHistSvc> m_histSvc;
  // Histogram of the MC particle energy
  TH1F* m_mc_energy{nullptr};
  // Histogram of the MC particle PDG
  TH1F* m_mc_pdg{nullptr};
  // Histogram of the deposited energy
  TH1F* m_dep_energy{nullptr};
  // Histogram of the ratio between deposited and MC particle energy
  TH1F* m_ratio_energy{nullptr};
  // Histogram of the longitudinal profile
  TH1F* m_long_profile{nullptr};
  // Histogram of the longitudinal first moment
  TH1F* m_long_first_moment{nullptr};
  // Histogram of the longitudinal second moment
  TH1F* m_long_second_moment{nullptr};
  // Histogram of the transverse profile
  TH1F* m_trans_profile{nullptr};
  // Histogram of the transverse first moment
  TH1F* m_trans_first_moment{nullptr};
  // Histogram of the transverse second moment
  TH1F* m_trans_second_moment{nullptr};
};
#endif /* SIMG4FAST_G4FASTSIMMESHHISTOGRAMS_H */
