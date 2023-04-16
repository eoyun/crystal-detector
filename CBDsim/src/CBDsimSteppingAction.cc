#include "CBDsimSteppingAction.hh"

#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"
#include <iomanip>



CBDsimSteppingAction::CBDsimSteppingAction(CBDsimEventAction* evtAct)
: G4UserSteppingAction(), fEventAction(evtAct) {
}

CBDsimSteppingAction::~CBDsimSteppingAction() {}
void CBDsimSteppingAction::UserSteppingAction(const G4Step* step)
{
  if ( step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) return;
  G4Track* track = step->GetTrack();

  G4StepPoint* presteppoint = step->GetPreStepPoint();
  G4StepPoint* poststeppoint = step->GetPostStepPoint();
  G4LogicalVolume* preVol = presteppoint->GetPhysicalVolume()->GetLogicalVolume();
  G4TouchableHandle theTouchable = presteppoint->GetTouchableHandle();

  if (poststeppoint->GetStepStatus() == fWorldBoundary) {
    fLeak.E = track->GetTotalEnergy();
    fLeak.px = track->GetMomentum().x();
    fLeak.py = track->GetMomentum().y();
    fLeak.pz = track->GetMomentum().z();
    fLeak.vx = presteppoint->GetPosition().x();
    fLeak.vy = presteppoint->GetPosition().y();
    fLeak.vz = presteppoint->GetPosition().z();
    fLeak.vt = presteppoint->GetGlobalTime();
    fLeak.pdgId = track->GetDefinition()->GetPDGEncoding();

    fEventAction->fillLeaks(fLeak);
  }


  G4String matName = preVol->GetMaterial()->GetName();

  if ( matName=="G4_Galactic" || matName=="Air" ) return;

  fEdep.Edep = step->GetTotalEnergyDeposit();
  fEdep.towerNum = theTouchable->GetCopyNumber(theTouchable->GetHistoryDepth()-1);

  if ( fEdep.Edep > 0. ) {
    fEventAction->fillEdeps(fEdep);
    fstep.Edep = step->GetTotalEnergyDeposit();
    fstep.E = step->GetPreStepPoint()->GetKineticEnergy();
    fstep.px = (step->GetPostStepPoint()->GetMomentum().getX()+step->GetPreStepPoint()->GetMomentum().getX())/2.;  
    fstep.py = (step->GetPostStepPoint()->GetMomentum().getY()+step->GetPreStepPoint()->GetMomentum().getY())/2.;  
    fstep.pz = (step->GetPostStepPoint()->GetMomentum().getZ()+step->GetPreStepPoint()->GetMomentum().getZ())/2.;  
    fstep.vx = (step->GetPostStepPoint()->GetPosition().getX()+step->GetPreStepPoint()->GetPosition().getX())/2.;  
    fstep.vy = (step->GetPostStepPoint()->GetPosition().getY()+step->GetPreStepPoint()->GetPosition().getY())/2.;  
    fstep.vz = (step->GetPostStepPoint()->GetPosition().getZ()+step->GetPreStepPoint()->GetPosition().getZ())/2.; 
    fstep.vt =  step->GetPostStepPoint()->GetGlobalTime();
    fstep.pdgId = track->GetDefinition()->GetPDGEncoding();
    fstep.trackId = step->GetTrack()->GetTrackID();
  
    fEventAction->fillSteps(fstep);

  }
  G4StepStatus stepStatus = fpSteppingManager->GetfStepStatus();

  //G4cout << "The step status is " << stepStatus << G4endl;

  G4int nSecAtRest = fpSteppingManager->GetfN2ndariesAtRestDoIt();
  G4int nSecAlong  = fpSteppingManager->GetfN2ndariesAlongStepDoIt();
  G4int nSecPost   = fpSteppingManager->GetfN2ndariesPostStepDoIt();
  G4int nSecTotal  = nSecAtRest+nSecAlong+nSecPost;
  G4TrackVector* secVec = fpSteppingManager->GetfSecondary();

  // get volume of the current step
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  // G4cout << volume->GetName() << G4endl;

  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  auto deltaE = step->GetDeltaEnergy();

  const G4ParticleDefinition* particle;
  particle = step->GetTrack()->GetParticleDefinition();

  // Process name
  G4String proc_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();


  G4cout << "******************************" << G4endl;
  G4cout << "Step is limited by "
  << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()
  << G4endl;
  G4cout << "Processes involved to the step" << G4endl;
  G4cout << "******************************" << G4endl;


  G4cout << "Particle Name is " << particle->GetParticleName() <<
  " and its kinetic energy is " << step->GetPreStepPoint()->GetKineticEnergy()/MeV <<
  " and the total energy is " << step->GetPreStepPoint()->GetTotalEnergy()/MeV <<
  " Energy deposit is " << edep <<
  " Track ID is " << step->GetTrack()->GetTrackID() <<
  " parent ID is " << step->GetTrack()->GetParentID() <<
  " lepton # " << particle->GetLeptonNumber() <<
  " Baryon # " << particle->GetBaryonNumber() <<
  " Type " << particle->GetParticleType() << G4endl;

  // G4cout << "----------------------------------------" << G4endl;

  G4double sum_all_ke = 0;
  G4double sum_all_e = 0;
  //G4int num_test=0;
  G4int oPnumber =0;

  if(nSecTotal>0)
  {
    /*
    G4cout << "  :----- List of 2ndaries - " << std::setw(3) << nSecTotal
    << " (Rest=" << std::setw(2) << nSecAtRest
    << ",Along=" << std::setw(2) << nSecAlong
    << ",Post="  << std::setw(2) << nSecPost << ")" << G4endl;
    */
      for(size_t lp1=(*secVec).size()-nSecTotal; lp1<(*secVec).size(); lp1++)
      {
          if ( (*secVec)[lp1]->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ){ 
            oPnumber+=1;
            continue;
            }
          G4double x= (*secVec)[lp1]->GetPosition().getX();
          G4double y=(*secVec)[lp1]->GetPosition().getY();
          G4double z=(*secVec)[lp1]->GetPosition().getZ();
          G4double energy=(*secVec)[lp1]->GetTotalEnergy();
          G4String PartName=(*secVec)[lp1]->GetDefinition()->GetParticleName();
          G4String PhysicName=(*secVec)[lp1]->GetCreatorProcess()->GetProcessName();
          fEventAction->fillPhysics(physical,x,y,z,energy,PartName,PhysicName);
          num_test+=1;
          
	  fsec.E = (*secVec)[lp1]->GetTotalEnergy();
	  fsec.px =  (*secVec)[lp1]->GetMomentum().getX();  
	  fsec.py =  (*secVec)[lp1]->GetMomentum().getY();  
	  fsec.pz =  (*secVec)[lp1]->GetMomentum().getZ();  
	  fsec.vx =  (*secVec)[lp1]->GetPosition().getX();  
	  fsec.vy =  (*secVec)[lp1]->GetPosition().getY();  
	  fsec.vz =  (*secVec)[lp1]->GetPosition().getZ(); 
	  fsec.vt =  (*secVec)[lp1]->GetGlobalTime();
	  fsec.pdgId = track->GetDefinition()->GetPDGEncoding();
	  fsec.trackId = (*secVec)[lp1]->GetTrackID();

	  fEventAction->fillSecondaries(fsec);
          // G4cout<<"##########################"<<G4endl;
          // G4cout<<"this is debug line "<<G4endl;
          // G4cout<<"test number is "<<num_test<<G4endl;
          // G4cout<<"##########################"<<G4endl;
          /*G4cout << "    : "
          << G4BestUnit((*secVec)[lp1]->GetPosition(), "Length") << " "
          << std::setw( 9) << G4BestUnit((*secVec)[lp1]->GetKineticEnergy() , "Energy") << " "
          << std::setw( 9) << G4BestUnit((*secVec)[lp1]->GetTotalEnergy() , "Energy") << " "
          << std::setw(18) << (*secVec)[lp1]->GetDefinition()->GetParticleName()
          << " generated by " << (*secVec)[lp1]->GetCreatorProcess()->GetProcessName() << "  "
          << " Track ID " << (*secVec)[lp1]->GetTrackID() << G4endl;
          */
	  sum_all_ke += (*secVec)[lp1]->GetKineticEnergy();
          sum_all_e += (*secVec)[lp1]->GetTotalEnergy();

          if ( (*secVec)[lp1]->GetDefinition()->GetParticleType() == "nucleus" ){
	      /*
              G4cout << (*secVec)[lp1]->GetDefinition()->GetParticleName() << "   "
              << (*secVec)[lp1]->GetDefinition()->GetAtomicMass() << "   "
              << (*secVec)[lp1]->GetDefinition()->GetAtomicNumber() << "   "
              << (*secVec)[lp1]->GetDefinition()->GetParticleSubType() << G4endl;
              */
	  }
      }
      fEventAction->fillOpticalPhoton(photon,oPnumber);
      G4cout<<"infinity loop?"<<G4endl;
    }

}
