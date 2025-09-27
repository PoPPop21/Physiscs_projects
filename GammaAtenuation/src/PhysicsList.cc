#include "PhysicsList.hh"
#include "G4EmStandardPhysics.hh"

PhysicsList::PhysicsList()
    : G4VModularPhysicsList() {
        // Registrar la REM estándar para fotones en rango médico.
        RegisterPhysics(new G4EmStandardPhysics());

        // Registrar decaimientos
        decayPhysics = new G4DecayPhysics();
        RegisterPhysics(decayPhysics);
    }

PhysicsList::~PhysicsList() {
    delete decayPhysics;
}
