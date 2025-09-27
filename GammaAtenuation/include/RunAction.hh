#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "DetectorConstruction.hh"
#include "globals.hh"

class RunAction : public G4UserRunAction {
public:
  RunAction(DetectorConstruction* detector);
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);

private:
  DetectorConstruction* detector;
};

#endif // RUNACTION_HH