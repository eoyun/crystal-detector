#include "CBDsimSiPMSD.hh"
#include "CBDsimSiPMHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

using namespace std;

CBDsimSiPMSD::CBDsimSiPMSD(const G4String& name, const G4String& hitsCollectionName, std::pair<int,int> xy)
: G4VSensitiveDetector(name), fHitCollection(0), fHCID(-1),fIsFront(0), fTowerXY(xy),fWavBin(600),fTimeBin(24000),fWavlenStart(900.),fWavlenEnd(300.),fTimeStart(0.),fTimeEnd(240.)
{
  collectionName.insert(hitsCollectionName);
  fWavlenStep = (fWavlenStart-fWavlenEnd)/(float)fWavBin;
  fTimeStep = (fTimeEnd-fTimeStart)/(float)fTimeBin;
}

CBDsimSiPMSD::~CBDsimSiPMSD() {}

void CBDsimSiPMSD::Initialize(G4HCofThisEvent* hce) {
  fHitCollection = new CBDsimSiPMHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) { fHCID = GetCollectionID(0); }
  hce->AddHitsCollection(fHCID,fHitCollection);
}

G4bool CBDsimSiPMSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  if (step->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
  G4int SiPMnum = step->GetPostStepPoint()->GetTouchable()->GetVolume(2)->GetCopyNo();
  G4int towernum = step->GetPostStepPoint()->GetTouchable()->GetVolume(3)->GetCopyNo();
  G4double hitTime  = step->GetPostStepPoint()->GetGlobalTime();
  G4double energy = step->GetTrack()->GetTotalEnergy();
  G4int towerX = fTowerXY.first;
  G4int towerY = fTowerXY.second;
  G4int sipmX = SiPMnum/towerY;
  G4int sipmY = SiPMnum%towerY;
  G4int nofHits = fHitCollection->entries();
  CBDsimSiPMHit* hit = NULL;


  for (G4int i = 0; i < nofHits; i++) {
    if ( ((*fHitCollection)[i]->GetSiPMnum()==SiPMnum) && ((*fHitCollection)[i]->GetTowernum()==towernum)&&((*fHitCollection)[i]->GetIsFront()==fIsFront) ) {
      hit = (*fHitCollection)[i];
      break;
    }
  }
  if (hit==NULL) {

    hit = new CBDsimSiPMHit(fWavBin,fTimeBin);
    if (fIsFront==1) hit->SetFront();
    hit->SetSiPMnum(SiPMnum);
    hit->SetTowernum(towernum);
    hit->SetX(sipmX);
    hit->SetY(sipmY);
    hit->SetTowerX(towerX);
    hit->SetTowerY(towerY);

    fHitCollection->insert(hit);
  }

  hit->photonCount();

  CBDsimInterface::hitRange wavRange = findWavRange(energy);
  hit->CountWavlenSpectrum(wavRange);


  CBDsimInterface::hitRange timeRange = findTimeRange(hitTime);
  hit->CountTimeStruct(timeRange);


  return true;
}

void CBDsimSiPMSD::EndOfEvent(G4HCofThisEvent*) {
  if ( verboseLevel>1 ) {
    G4int nofHits = fHitCollection->entries();
    G4cout
    << G4endl
    << "-------->Hits Collection: in this event they are " << nofHits
    << " hits in the tracker chambers: " << G4endl;
    for ( G4int i=0; i<nofHits; i++ ) (*fHitCollection)[i]->Print();
  }
}

CBDsimInterface::hitRange CBDsimSiPMSD::findWavRange(G4double en) {
  int i = 0;
  for ( ; i < fWavBin+1; i++) {
    if ( en < wavToE( (fWavlenStart - (float)i*fWavlenStep)*nm ) ) break;
  }

  if (i==0) return std::make_pair(fWavlenStart,99999.);
  else if (i==fWavBin+1) return std::make_pair(0.,fWavlenEnd);

  return std::make_pair( fWavlenStart-(float)i*fWavlenStep, fWavlenStart-(float)(i-1)*fWavlenStep );
}

CBDsimInterface::hitRange CBDsimSiPMSD::findTimeRange(G4double stepTime) {
  int i = 0;
  for ( ; i < fTimeBin+1; i++) {
    if ( stepTime < ( (fTimeStart + (float)i*fTimeStep)*ns ) ) break;
  }

  if (i==0) return std::make_pair(0.,fTimeStart);
  else if (i==fTimeBin+1) return std::make_pair(fTimeEnd,99999.);

  return std::make_pair( fTimeStart+(float)(i-1)*fTimeStep, fTimeStart+(float)i*fTimeStep );
}
