#include "EventAction.hh"
#include "G4Event.hh"
#include <iostream>
#include <fstream>
#include "G4SDManager.hh"
#include "MiSensitiveDetector.hh"


EventAction::EventAction(RunAction* runAct)
: G4UserEventAction(), runAction(runAct) {
  // Abrir archivo en modo apend
  outputFile.open("event_data.csv", std::ios::app);
  if (!outputFile.is_open()) {
    G4Exception("EventAction", "EventAction001", FatalException, "Failed to open output file!");
  }
}

EventAction::~EventAction() {
  outputFile.close();
}

void EventAction::BeginOfEventAction(const G4Event* event) {
  G4int eventID = event->GetEventID();
  G4int detected = 0;
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if (HCE) {
      G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("DetectorHitsCollection");
      auto hitsCollection = HCE->GetHC(hcID); // Usar tipo base
      if (hitsCollection && hitsCollection->GetSize() > 0) {
          detected = 1;
      }
  }
  outputFile << eventID << " , " << detected << "\n";
}

void EventAction::EndOfEventAction(const G4Event* event) {
  // Procesar resultados del evento
  // std::cout << "Finalizando evento " << event->GetEventID() << std::endl;
}
