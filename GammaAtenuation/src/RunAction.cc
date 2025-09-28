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

RunAction::RunAction(DetectorConstruction* det)
 : G4UserRunAction(), detector(det), totalEvents(0), transmittedEvents(0) {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
  totalEvents = run->GetNumberOfEventToBeProcessed();
  transmittedEvents = 0;

  std::cout << "=== Comenzando Run " << run->GetRunID() << " ===" << std::endl;
    std::cout << "Material: " << detector->GetMaterial() << std::endl;
    std::cout << "Espesor: " << detector->GetThickness()/CLHEP::cm << " cm" << std::endl;
    std::cout << "Eventos totales: " << totalEvents << std::endl;
    
    // Preparar archivo de resultados
    std::ofstream resultsFile("results_summary.txt", std::ios::app);
    resultsFile << "\n=== RUN " << run->GetRunID() << " ===\n";
    resultsFile << "Material: " << detector->GetMaterial() << "\n";
    resultsFile << "Espesor: " << detector->GetThickness()/CLHEP::cm << " cm\n";
    resultsFile << "Eventos: " << totalEvents << "\n";
    resultsFile.close();
}

void RunAction::EndOfRunAction(const G4Run* run) {
  G4double transmissionRatio = (G4double)transmittedEvents / totalEvents;
  G4double attenuationCoeff = -std::log(transmissionRatio) / (detector->GetThickness()/CLHEP::cm);
    
  std::cout << "=== Finalizando Run " << run->GetRunID() << " ===" << std::endl;
  std::cout << "Eventos transmitidos: " << transmittedEvents << std::endl;
  std::cout << "Razón de transmisión: " << transmissionRatio << std::endl;
  std::cout << "Coeficiente de atenuación: " << attenuationCoeff << " cm^-1" << std::endl;
    
  // Guardar resultados finales
  std::ofstream resultsFile("results_summary.txt", std::ios::app);
  resultsFile << "Transmitidos: " << transmittedEvents << "\n";
  resultsFile << "Transmisión: " << transmissionRatio << "\n";
  resultsFile << "Coef. atenuación: " << attenuationCoeff << " cm^-1\n";
  resultsFile.close();
    
  // Archivo CSV para ROOT
  std::ofstream csvFile("attenuation_data.csv", std::ios::app);
  csvFile << detector->GetMaterial() << ","
          << detector->GetThickness()/CLHEP::cm << ","
          << totalEvents << ","
          << transmittedEvents << ","
          << transmissionRatio << ","
          << attenuationCoeff << "\n";
  csvFile.close();
}

void RunAction::AddTransmittedEvent() {
  transmittedEvents++;
}

