#include "MiSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"

MiSensitiveDetector::MiSensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

MiSensitiveDetector::~MiSensitiveDetector() {}

void MiSensitiveDetector::Initialize(G4HCofThisEvent* /*hce*/) {
    // Inicialización de la colección de hits si la necesitas
}

G4bool MiSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) {
    // Aquí puedes registrar información del hit, por ejemplo:
    // Si quieres solo saber si hubo un hit, puedes retornar true.
    return true;
}