#ifndef CBDsimSteppingAction_h
#define CBDsimSteppingAction_h 1

#include "CBDsimInterface.h"
#include "CBDsimEventAction.hh"

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"

class CBDsimSteppingAction : public G4UserSteppingAction {
public:
  CBDsimSteppingAction(CBDsimEventAction* evtAct);
  virtual ~CBDsimSteppingAction();

  virtual void UserSteppingAction(const G4Step*);

private:
  G4int num_test=0;
  CBDsimEventAction* fEventAction;
  CBDsimInterface::CBDsimEdepData fEdep;
  CBDsimInterface::CBDsimPhoton photon;
  CBDsimInterface::CBDsimPhysicalevent physical;
  //physical.physicsNum=0;
};

#endif
