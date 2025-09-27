#include "MiSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHit.hh"
#include "G4SDManager.hh"
#include "MiHit.hh"

MiSensitiveDetector::MiSensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

MiSensitiveDetector::~MiSensitiveDetector() {}

void MiSensitiveDetector::Initialize(G4HCofThisEvent* hce) {
    // Crear una colección de hits vacía
    auto hitsCollection = new G4HitsCollection<G4VHit>(SensitiveDetectorName, "DetectorHitsCollection");
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("DetectorHitsCollection");
    hce->AddHitsCollection(hcID, hitsCollection);
}

G4bool MiSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) {
    // Aquí puedes registrar información del hit, si lo necesitas
    return true;
}