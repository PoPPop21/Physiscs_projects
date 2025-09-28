/* ------- GAMMA ATTENUATION SIMULATION -------
Autor: @isabelnieto900, @PoPPop21
Fecha: Octubre 2025
-----------------------------------------------
*/
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include <iostream>
#include <fstream>

#ifdef USE_ROOT
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"
#endif

RunAction::RunAction(DetectorConstruction *det)
    : G4UserRunAction(), detector(det), totalEvents(0), transmittedEvents(0)
{
#ifdef USE_ROOT
  rootFile = nullptr;
  attenuationTree = nullptr;
  attenuationHist = nullptr;
  G4cout << "RunAction: ROOT support enabled" << G4endl;
#else
  G4cout << "RunAction: ROOT support not available" << G4endl;
#endif
}

RunAction::~RunAction()
{
#ifdef USE_ROOT
  if (rootFile)
  {
    rootFile->Close();
    delete rootFile;
  }
#endif
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
  totalEvents = run->GetNumberOfEventToBeProcessed();
  transmittedEvents = 0;

  std::cout << "=== Comenzando Run " << run->GetRunID() << " ===" << std::endl;
  std::cout << "Material: " << detector->GetMaterial() << std::endl;
  std::cout << "Espesor: " << detector->GetThickness() / CLHEP::cm << " cm" << std::endl;
  std::cout << "Eventos totales: " << totalEvents << std::endl;

#ifdef USE_ROOT
  // Crear archivo ROOT
  TString rootFileName = TString::Format("gamma_attenuation_run%d.root", run->GetRunID());
  rootFile = new TFile(rootFileName.Data(), "RECREATE");

  // Crear Tree para datos de atenuación
  attenuationTree = new TTree("attenuationData", "Gamma Attenuation Results");

  // Variables para el Tree
  runData.runID = run->GetRunID();
  strncpy(runData.material, detector->GetMaterial().c_str(), 49);
  runData.material[49] = '\0'; // Asegurar terminación null
  runData.thickness = detector->GetThickness() / CLHEP::cm;
  runData.totalEvents = totalEvents;

  // Configurar branches
  attenuationTree->Branch("runID", &runData.runID, "runID/I");
  attenuationTree->Branch("material", runData.material, "material/C");
  attenuationTree->Branch("thickness", &runData.thickness, "thickness/F");
  attenuationTree->Branch("totalEvents", &runData.totalEvents, "totalEvents/I");
  attenuationTree->Branch("transmittedEvents", &runData.transmittedEvents, "transmittedEvents/I");
  attenuationTree->Branch("transmissionRatio", &runData.transmissionRatio, "transmissionRatio/F");
  attenuationTree->Branch("attenuationCoeff", &runData.attenuationCoeff, "attenuationCoeff/F");

  // Crear histograma para coeficientes de atenuación
  attenuationHist = new TH1F("attenuationCoeff", "Coeficiente de Atenuacion;Coeficiente (cm^{-1});Frecuencia", 100, 0, 10);

  G4cout << "ROOT: Archivo " << rootFileName << " creado" << G4endl;
#endif

  // Preparar archivo de resultados
  std::ofstream resultsFile("results_summary.txt", std::ios::app);
  resultsFile << "\n=== RUN " << run->GetRunID() << " ===\n";
  resultsFile << "Material: " << detector->GetMaterial() << "\n";
  resultsFile << "Espesor: " << detector->GetThickness() / CLHEP::cm << " cm\n";
  resultsFile << "Eventos: " << totalEvents << "\n";
  resultsFile.close();
}

void RunAction::EndOfRunAction(const G4Run *run)
{
  G4double transmissionRatio = (G4double)transmittedEvents / totalEvents;
  G4double attenuationCoeff = (transmittedEvents > 0) ? -std::log(transmissionRatio) / (detector->GetThickness() / CLHEP::cm) : 999.0;

  std::cout << "=== Finalizando Run " << run->GetRunID() << " ===" << std::endl;
  std::cout << "Eventos transmitidos: " << transmittedEvents << std::endl;
  std::cout << "Razón de transmisión: " << transmissionRatio << std::endl;
  std::cout << "Coeficiente de atenuación: " << attenuationCoeff << " cm^-1" << std::endl;

#ifdef USE_ROOT
  // Llenar datos en ROOT
  runData.transmittedEvents = transmittedEvents;
  runData.transmissionRatio = transmissionRatio;
  runData.attenuationCoeff = attenuationCoeff;

  // Llenar Tree
  attenuationTree->Fill();

  // Llenar histograma (solo si el coeficiente es válido)
  if (attenuationCoeff < 999.0)
  {
    attenuationHist->Fill(attenuationCoeff);
  }

  // Guardar y cerrar archivo ROOT
  rootFile->cd();
  attenuationTree->Write();
  attenuationHist->Write();
  rootFile->Close();

  G4cout << "ROOT: Datos guardados en archivo ROOT" << G4endl;
  G4cout << "ROOT: Para analizar: root gamma_attenuation_run" << run->GetRunID() << ".root" << G4endl;
#endif

  // Guardar resultados finales
  std::ofstream resultsFile("results_summary.txt", std::ios::app);
  resultsFile << "Transmitidos: " << transmittedEvents << "\n";
  resultsFile << "Transmisión: " << transmissionRatio << "\n";
  resultsFile << "Coef. atenuación: " << attenuationCoeff << " cm^-1\n";
  resultsFile.close();

  // Archivo CSV para ROOT
  std::ofstream csvFile("attenuation_data.csv", std::ios::app);
  csvFile << detector->GetMaterial() << ","
          << detector->GetThickness() / CLHEP::cm << ","
          << totalEvents << ","
          << transmittedEvents << ","
          << transmissionRatio << ","
          << attenuationCoeff << "\n";
  csvFile.close();
}

void RunAction::AddTransmittedEvent()
{
  transmittedEvents++;
}
