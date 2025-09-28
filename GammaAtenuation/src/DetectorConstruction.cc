/* ------- GAMMA ATTENUATION SIMULATION -------
Autor: @isabelnieto900, @PoPPop21
Fecha: Octubre 2025
-----------------------------------------------
*/
#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
// Construcción del sentiveDetector para el volumen
#include "G4SDManager.hh"
#include "MiSensitiveDetector.hh" 




DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

/* Creación de la geometría del mundo y el detector */

G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Mundo mínimo de aire para que compile 
    G4NistManager* nist = G4NistManager::Instance();
    
    // --- 1. Mundo ---
    G4double world_size = 1.0*m;
    G4Material * world_mat = nist->FindOrBuildMaterial("G4_AIR"); // Aquí se modifica el material del mundo
    auto solidWorld = new G4Box("World", world_size, world_size, world_size);
    auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    auto physWorld = new G4PVPlacement(0, {}, logicWorld, "World", 0, false, 0);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible()); // Invisible

    // --- 2. Materiales absorbentes --- 
    G4Material* absorber_mat = nist->FindOrBuildMaterial("G4_WATER"); // Material absorbente 
    G4double absorber_thickness = 5*cm;
    auto solidAbs = new G4Box("Absorber", 10*cm, 10*cm, absorber_thickness/2.0);
    auto logicAbs = new G4LogicalVolume(solidAbs, absorber_mat, "Absorber");
    new G4PVPlacement(0, G4ThreeVector(0,0,0), logicAbs, "Absorber", logicWorld, false, 0);

    auto visAbs = new G4VisAttributes(G4Colour(0,0,1,0.4)); // Azul
    logicAbs->SetVisAttributes(visAbs);
 
    // --- 3. Detector ---
    G4Material* detector_mat = nist->FindOrBuildMaterial("G4_AIR"); // Material del detector
    auto solidDet = new G4Box("Detector", 10*cm, 10*cm, 1*mm);
    auto logicDet = new G4LogicalVolume(solidDet, detector_mat, "Detector");
    new G4PVPlacement(0, G4ThreeVector(0,0, absorber_thickness/2.0 + 2*cm), 
                      logicDet, "Detector", logicWorld, false, 0);
    auto visDet = new G4VisAttributes(G4Colour(1,0,0,0.6)); // Rojo
    logicDet->SetVisAttributes(visDet);

    // --- 4. Detector lógico: sensitivedetector. ---
    auto sd = new MiSensitiveDetector("MyDetectorSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    logicDet->SetSensitiveDetector(sd);

    return physWorld;

}
