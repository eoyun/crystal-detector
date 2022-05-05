#ifndef CBDsimPrimaryGeneratorAction_h
#define CBDsimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "G4GenericMessenger.hh"
#include "G4Threading.hh"

class G4ParticleDefinition;

class CBDsimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  CBDsimPrimaryGeneratorAction(G4int seed);
  virtual ~CBDsimPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  void SetTheta(G4double theta) { fTheta = theta; }
  G4double GetTheta() const { return fTheta; }

  void SetPhi(G4double phi) { fPhi = phi; }

  void SetY0(G4double y0) { fY_0 = y0; }
  void SetZ0(G4double z0) { fZ_0 = z0; }

  void SetRandX(G4double randx) { fRandX = randx; }
  void SetRandY(G4double randy) { fRandY = randy; }

  static G4ThreadLocal int sIdxEvt;
  static int sNumEvt;

private:
  void DefineCommands();

  G4GenericMessenger* fMessenger;

  G4ParticleGun* fParticleGun;
  G4ParticleDefinition* fElectron;
  G4ParticleDefinition* fMuon;
  G4ParticleDefinition* fPion;
  G4ParticleDefinition* fKaon0L;
  G4ParticleDefinition* fProton;
  G4ParticleDefinition* fOptPhoton;
  G4ParticleDefinition* fGamma;

  G4int fSeed;
  G4int fNumPtc;

  G4double fTheta;
  G4double fPhi;
  G4double fRandX;
  G4double fRandY;
  G4double fY_0;
  G4double fZ_0;
  G4double fY;
  G4double fZ;

  G4ThreeVector fOrigin;
  G4ThreeVector fDirection;
};

#endif
