#include "EventAction.hh"
#include "G4Event.hh"
#include <iostream>

EventAction::EventAction(RunAction* runAct)
: G4UserEventAction(), runAction(runAct) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
  // Inicializar contadores o banderas por evento
  // std::cout << "Iniciando evento " << event->GetEventID() << std::endl;
}

void EventAction::EndOfEventAction(const G4Event* event) {
  // Procesar resultados del evento
  // std::cout << "Finalizando evento " << event->GetEventID() << std::endl;
}
