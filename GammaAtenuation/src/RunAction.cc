#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include <iostream>

RunAction::RunAction(DetectorConstruction* det)
: G4UserRunAction(), detector(det) {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
  std::cout << "=== Comenzando Run " << run->GetRunID() << " ===" << std::endl;
}

void RunAction::EndOfRunAction(const G4Run* run) {
  std::cout << "=== Finalizando Run " << run->GetRunID() << " ===" << std::endl;
}
