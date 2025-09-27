#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Mundo mínimo de aire para que compile 
    G4NistManager* niost = G4NistManager::Instance();
    G4Material* air = niost->FindOrBuildMaterial("G4_AIR");

    G4double world_sizeXY = 1.0*m;
    G4double world_sizeZ  = 1.0*m;
    G4Box* solidWorld = new G4Box("World", world_sizeXY, world_sizeXY, world_sizeZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, {}, logicWorld, "World", 0, false, 0, true);

    return physWorld;
}
