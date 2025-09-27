#include "MiSensitiveDetector.hh"
#include "MiHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

MiSensitiveDetector::MiSensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

MiSensitiveDetector::~MiSensitiveDetector() {}

void MiSensitiveDetector::Initialize(G4HCofThisEvent* hce) {
    auto hitsCollection = new G4THitsCollection<MiHit>(SensitiveDetectorName, "DetectorHitsCollection");
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("DetectorHitsCollection");
    hce->AddHitsCollection(hcID, hitsCollection);
}

G4bool MiSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/) {
    // Registrar un hit básico
    auto hit = new MiHit();
    GetHitsCollection()->insert(hit);
    return true;
}