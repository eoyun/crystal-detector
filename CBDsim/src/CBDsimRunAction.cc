#include "CBDsimRunAction.hh"

#include "G4AutoLock.hh"
#include "G4Threading.hh"

#include <iostream>

namespace { G4Mutex CBDsimRunActionMutex = G4MUTEX_INITIALIZER; }
CBDsimRootInterface* CBDsimRunAction::sRootIO = 0;
int CBDsimRunAction::sNumEvt = 0;

CBDsimRunAction::CBDsimRunAction(G4int seed, G4String filename)
: G4UserRunAction() {
  fSeed = seed;
  fFilename = filename;

  G4AutoLock lock(&CBDsimRunActionMutex);

  if (!sRootIO) {
    sRootIO = new CBDsimRootInterface(fFilename+"_"+std::to_string(fSeed)+".root");
    sRootIO->create();
  }
}

CBDsimRunAction::~CBDsimRunAction() {
  if (IsMaster()) {
    G4AutoLock lock(&CBDsimRunActionMutex);

    if (sRootIO) {
      sRootIO->write();
      sRootIO->close();
      delete sRootIO;
      sRootIO = 0;
    }
  }
}

void CBDsimRunAction::BeginOfRunAction(const G4Run*) {}

void CBDsimRunAction::EndOfRunAction(const G4Run*) {}
