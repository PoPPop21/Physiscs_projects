/* ------- GAMMA ATTENUATION SIMULATION -------
Autor: @isabelnieto900, @PoPPop21
Fecha: Octubre 2025
-----------------------------------------------
*/
#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "DetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

class RunAction : public G4UserRunAction {
public:
  RunAction(DetectorConstruction* detector);
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);

  void AddTransmittedEvent();

private:
  DetectorConstruction* detector;
  G4int totalEvents;
  G4int transmittedEvents;
};

#endif // RUNACTION_HH