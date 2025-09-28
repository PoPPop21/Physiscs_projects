#include "RunAction.hh"
#include "G4Run.hh"
#include "G4ios.hh"

RunAction::RunAction(DetectorConstruction* det)
 : G4UserRunAction(), detector(det)
{}

RunAction::~RunAction()
{
  G4cout << "RunAction deleted " << this << G4endl;
}

void RunAction::BeginOfRunAction(const G4Run* run) {
  G4cout << "=== Comenzando Run " << run->GetRunID() << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run) {
  G4cout << "=== Finalizando Run " << run->GetRunID() << G4endl;
  // NO acceder a 'detector' aquí si crees que puede haber sido borrado.
  // Si necesitas info del detector, copia/almacena antes en BeginOfRunAction.
}

