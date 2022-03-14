#include "SimG4Fast/Par04RunAction.h"
#include <G4GenericAnalysisManager.hh>   // for G4GenericAnalysisManager
#include <G4ThreeVector.hh>              // for G4ThreeVector
#include <G4Types.hh>                    // for G4int, G4double
#include <G4UserRunAction.hh>            // for G4UserRunAction
#include "G4RootAnalysisManager.hh"          // for G4AnalysisManager
#include "SimG4Fast/Par04EventAction.h"           // for Par04EventAction

namespace sim {
Par04RunAction::Par04RunAction(Par04EventAction* aEventAction)
  : G4UserRunAction()
  , fEventAction(aEventAction)
{
  // Create analysis manager
  G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
//  analysisManager->SetDefaultFileType("root");

  // Default filename, can be overriden with /analysis/setFileName
  analysisManager->SetFileName("Par04Output");
}

Par04RunAction::~Par04RunAction() {}

void Par04RunAction::BeginOfRunAction(const G4Run*)
{
  // Get analysis manager
  G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();

  // Create directories
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(0);

  // Get detector dimensions TODO: CHANGE< NOT HARDOCODED
  G4int cellNumZ       = 45;//fDetector->GetMeshNbOfCells().z();
  G4int cellNumRho     = 18;//fDetector->GetMeshNbOfCells().x();
  G4int cellNumPhi     = 50;//fDetector->GetMeshNbOfCells().y();
  G4double cellSizeZ   = 3.4;//fDetector->GetMeshSizeOfCells().z();
  G4double cellSizeRho = 2.325;//fDetector->GetMeshSizeOfCells().x();
  G4double cellSizePhi = 2*CLHEP::pi/cellNumPhi;//fDetector->GetMeshSizeOfCells().y();
  // Default max value of energy stored in histogram (in GeV)
  G4double maxEnergy = 1000;

  // Creating control histograms
  analysisManager->CreateH1("energyParticle", "Primary energy;E_{MC} (GeV);Entries", 1024, 0,
                            1.1 * maxEnergy);
  analysisManager->CreateH1("energyDeposited", "Deposited energy;E_{MC} (GeV);Entries", 1024, 0,
                            1.1 * maxEnergy);
  analysisManager->CreateH1(
    "energyRatio", "Ratio of energy deposited to primary;E_{dep} /  E_{MC};Entries", 1024, 0, 1);
  analysisManager->CreateH1("time", "Simulation time; time (s);Entries", 2048, 0, 100);
  analysisManager->CreateH1("longProfile", "Longitudinal profile;t (mm);#LTE#GT (MeV)", cellNumZ,
                            -0.5 * cellSizeZ, (cellNumZ - 0.5) * cellSizeZ);
  analysisManager->CreateH1("transProfile", "Transverse profile;r (mm);#LTE#GT (MeV)", cellNumRho,
                            -0.5 * cellSizeRho, (cellNumRho - 0.5) * cellSizeRho);
  analysisManager->CreateH1("longFirstMoment",
                            "First moment of longitudinal distribution;#LT#lambda#GT (mm);Entries",
                            1024, -0.5 * cellSizeZ,
                            cellNumZ * cellSizeZ / 2);  // arbitrary scaling of max value on axis
  analysisManager->CreateH1("transFirstMoment",
                            "First moment of transverse distribution;#LTr#GT "
                            "(mm);Entries",
                            1024, -0.5 * cellSizeRho,
                            cellNumRho * cellSizeRho /
                              1);  // arbitrary scaling of max value on axis
  analysisManager->CreateH1(
    "longSecondMoment",
    "Second moment of longitudinal distribution;#LT#lambda^{2}#GT "
    "(mm^{2});Entries",
    1024, 0, std::pow(cellNumZ * cellSizeZ, 2) / 25);  // arbitrary scaling of max value on axis
  analysisManager->CreateH1(
    "transSecondMoment", "Second moment of transverse distribution;#LTr^{2}#GT (mm^{2});Entries",
    1024, 0, std::pow(cellNumRho * cellSizeRho, 2) / 5);  // arbitrary scaling of max value on axis
  analysisManager->CreateH1("hitType", "hit type;type (0=full, 1= fast);Entries", 2, -0.5, 1.5);
  analysisManager->CreateH1("phiProfile", "Azimuthal angle profile, centred at mean;phi;#LTE#GT (MeV)", cellNumPhi,
                            - (cellNumPhi - 0.5) * cellSizePhi, (cellNumPhi - 0.5) * cellSizePhi);
  analysisManager->CreateH1("numHits", "Number of hits above 0.5 keV", 4048, 0, 40500);
  analysisManager->CreateH1("cellEnergy", "Cell energy distribution;log10(E/MeV);Entries", 1024, -4, 2);

  // Creating ntuple
  analysisManager->CreateNtuple("events", "per event data");
  analysisManager->CreateNtupleDColumn("EnergyMC");
  analysisManager->CreateNtupleDColumn("EnergyCell", fEventAction->GetCalEdep());
  analysisManager->CreateNtupleIColumn("rhoCell", fEventAction->GetCalRho());
  analysisManager->CreateNtupleIColumn("phiCell", fEventAction->GetCalPhi());
  analysisManager->CreateNtupleIColumn("zCell", fEventAction->GetCalZ());
  analysisManager->CreateNtupleDColumn("SimTime");
  analysisManager->FinishNtuple();

  analysisManager->OpenFile();
}

void Par04RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
}
