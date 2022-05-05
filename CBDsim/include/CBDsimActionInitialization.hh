#ifndef CBDsimActionInitialization_h
#define CBDsimActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class CBDsimActionInitialization : public G4VUserActionInitialization {
public:
  CBDsimActionInitialization(G4int seed, G4String filename);
  virtual ~CBDsimActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  G4int fSeed;
  G4String fFilename;
};

#endif
