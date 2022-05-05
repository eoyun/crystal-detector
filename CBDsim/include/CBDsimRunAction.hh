#ifndef CBDsimRunAction_h
#define CBDsimRunAction_h 1

#include "CBDsimRootInterface.h"
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class CBDsimRunAction : public G4UserRunAction {
public:
  CBDsimRunAction(G4int seed, G4String filename);
  virtual ~CBDsimRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  static CBDsimRootInterface* sRootIO;
  static int sNumEvt;

private:
  G4int fSeed;
  G4String fFilename;
};

#endif
