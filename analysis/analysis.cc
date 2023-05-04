#include "CBDsimRootInterface.h"
#include "CBDsimInterface.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TPaveStats.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TH2.h"
#include "TH2Poly.h"
#include "TH3.h"
#include "TMath.h"


#include <iostream>
#include <string>
#include <cmath>


int main(int argc, char* argv[]){
  
  TString filename =argv[1];
  float low = std::stof(argv[2]);
  float high = std::stof(argv[3]);

  TH1F* tEdep = new TH1F("totEdep","Total Energy deposit;MeV;Evt",100,low,high);
  tEdep->Sumw2(); tEdep->SetLineColor(kRed); tEdep->SetLineWidth(2);



 
  TH1F* tT = new TH1F("time","time;ns;p.e.",24000,0,240);
  tT->Sumw2(); tT->SetLineColor(kBlue); tT->SetLineWidth(2);

  TH1F* tWav = new TH1F("wavlen","wavelength;nm;p.e.",60,300.,900.);
  tWav->Sumw2(); tWav->SetLineColor(kRed); tWav->SetLineWidth(2);


  TH1F* tNhit = new TH1F("nHits","Number of p.e./SiPM;p.e.;n",200,0.,2000.);
  tNhit->Sumw2(); tNhit->SetLineColor(kRed); tNhit->SetLineWidth(2);

  TH1I* thits = new TH1I("p.e.","p.e./evt;p.e.;evt",100,0,1500);thits->Sumw2(); thits->SetLineColor(kBlack); thits->SetLineWidth(2);
  //std::cout<<"11"<<std::endl;

  TH1F* tTurnOnTime = new TH1F("risetime","risetime;t(ns);evt",24000,0,240);
  TH1F* tTurnOnTimefront = new TH1F("risetimefront","risetimefront;t(ns);evt",24000,0,240);
  TH1F* tDeltaTime = new TH1F("deltatime","delta time; t(ns);evt",2000,-10,10);
  
  TH2I *tsipmXY = new TH2I("sipmXY","sipmXY;X;Y",30,0,30,30,0,30);
  TH2I *tsipmXYfront = new TH2I("sipmXYfront","sipmXY;X;Y",30,0,30,30,0,30);

  TH2F *tXYriseTime = new TH2F("XYturnontime","turn on time by sipm;X;Y",30,0,30,30,0,30);
  TH2F *tXYriseTimefront = new TH2F("XYturnontimefront","turn on time by front-sipm;X;Y",30,0,30,30,0,30);


  TH1F* tTfront = new TH1F("time_front","time(front);ns;p.e.",700,0,7);
  tTfront->Sumw2(); tTfront->SetLineColor(kBlue); tTfront->SetLineWidth(2);

  TH1F* tWavfront = new TH1F("wavlen_front","wavelength(front);nm;p.e.",60,300.,900.);
  tWavfront->Sumw2(); tWavfront->SetLineColor(kRed); tWavfront->SetLineWidth(2);

  TH1F* tNhitfront = new TH1F("nHits_front","Number of p.e./SiPM(front);p.e.;n",200,0.,2000.);
  tNhitfront->Sumw2(); tNhitfront->SetLineColor(kRed); tNhitfront->SetLineWidth(2);

  TH1I* thitsfront = new TH1I("p.e._front","p.e./evt(front);p.e.;evt",100,0,1500);thitsfront->Sumw2(); thitsfront->SetLineColor(kBlack); thitsfront->SetLineWidth(2);
  //std::cout<<"11"<<std::endl;

  TH1F* tenergy = new TH1F("energy detected", "energy detected;MeV;evt",200,low,high);

  TF1 *genergy = new TF1("genergy","gaus",10.3,10.7);genergy->SetLineColor(2);genergy->SetLineWidth(2);

  TH1I* thitstotal = new TH1I("p.e._total","p.e./evt(total);p.e.;evt",3000,0,3000);thitstotal->Sumw2(); thitstotal->SetLineColor(kBlack); thitstotal->SetLineWidth(2);

  TH2I* genparticle2D = new TH2I("2Dgenparticle","2D generated particle;x;y",30,0,30,30,0,30);
  TH2I* gengamma2D = new TH2I("2Dgengamma","generated gamma;x;y",30,0,30,30,0,30);
  TH2I* genelectron2D = new TH2I("2Dgenelectron","generated electron;x;y",30,0,30,30,0,30);
  TH2I* genpositron2D = new TH2I("2Dgenppositron","generated positron;x;y",30,0,30,30,0,30);
  TH1F* risetimesipm = new TH1F("","1sipm;t(ns);p.e.",24000,0,240);
  TH1F* risetimesipmfront = new TH1F("","1sipm;t(ns);p.e.",24000,0,240);
  TH3F* sec_pos = new TH3F("sec_3D","secondary;x(mm);y(mm);z(mm)",100,0,100,1000,-100,100,100,-100,100);
  TH3F* step_pos = new TH3F("step_3D","step;x(mm);y(mm);z(mm)",100,450,550,200,-100,100,200,-100,100);
  TH1F* step_x = new TH1F("step_x","step;x(mm);E(MeV)",10000,0,1000);
  TH1F* step_y = new TH1F("step_y","step;y(mm);E(MeV)",2000,-100,100);
  TH1F* step_z = new TH1F("step_z","step;z(mm);E(MeV)",2000,-100,100);
  TH1F* PDGID = new TH1F("sec_ID","secondary PdgID",10000,0,10000);
  TH1F* Leak_PDGID = new TH1F("leak_ID","leak PdgID",10000,0,10000);

  TH1F* Leak_neutron_p = new TH1F("leak_neutron_momentum","neutron momentum;P(MeV/C);a.u.",200,0,4000);
  TH1F* Leak_electron_p = new TH1F("leak_electron_momentum","electron momentum;P(MeV/C);a.u.",200,0,4000);
  TH1F* Leak_gamma_p = new TH1F("leak_gamma_momentum","gamma momentum;P(MeV/C);a.u.",200,0,4000);
  TH1F* Leak_proton_p = new TH1F("leak_proton_momentum","proton momentum;P(MeV/C);a.u.",200,0,4000);
  TH1F* Leak_H_p = new TH1F("leak_H_momentum","H momentum;P(MeV/C);a.u.",200,0,4000);
  TH1F* Leak_He_p = new TH1F("leak_He_momentum","He momentum;P(MeV/C);a.u.",200,0,4000);
  TH1F* ratio_fired_ele_front = new TH1F("ratio fired ele front","num of fired SiPM / num of cells include ele;ratio;evt",25,0,25);
  TH1F* ratio_fired_ele = new TH1F("ratio fired ele","num of fired SiPM / num of cells include ele;ratio;evt",25,0,25);
  TH1F *time1evt = new TH1F("","time;ns;p.e.",24000,0,240);
  TH1F *MCoptical = new TH1F("mc truth optical photon","mc truth optical photon; number of optical photon;evt",100,0,300000);
  CBDsimRootInterface* cbdInterface = new CBDsimRootInterface(std::string("../CBDsim/"+filename));
  // cbdInterface->set();
  cbdInterface->GetChain("CBDsim");
  
  CBDsimInterface::CBDsimTimeStruct tmp;
  unsigned int entries =cbdInterface->entries();
  float totenergy=0.;
  int tothit=0;
  TCanvas* c1 = new TCanvas("c1","");
  int zedepnum=0;
  int ele=0, posi=0, proton=0, neutron=0, carbon=0, deutron=0, secondary=0,gamma=0,sec_evt=0,flag=1; 
  TFile *timeL_file = new TFile(filename+"_timeL.root","recreate");
  while(cbdInterface->numEvt()<entries){
    if (cbdInterface->numEvt()%50 ==0)printf("Analyzing %dth event ...\n", cbdInterface->numEvt());
    CBDsimInterface::CBDsimEventData cbdEvt;
    cbdInterface->read(cbdEvt);
    int nhit=0;
    int nhitfront=0;
    int fired_sipm =0;
    int fired_sipm_front =0;
    float energy=0;

    float Edep =0;

    TH1F* turnontime= new TH1F("","1MeV 1evt;t(ns);p.e.",24000,0,240);
    TH1F* turnontimefront= new TH1F("","1MeV 1evt;t(ns);p.e.",24000,0,240);
    int i_evt=(cbdInterface->numEvt()-1);
    std::stringstream ss_evt;
    ss_evt<<i_evt;
    TString histoName="timeL"+ss_evt.str();
    TH2F *timeL2D= new TH2F(histoName,"timeL;x;y",30,0,30,30,0,30);
    for (auto edep : cbdEvt.Edeps){
      Edep += edep.Edep;
    }
    tEdep->Fill(Edep);
    for (auto step : cbdEvt.steps){
      step_pos->Fill(step.vx,step.vx,step.vx,step.Edep/entries);
      step_x->Fill(step.vx,step.Edep/entries);
      step_y->Fill(step.vy,step.Edep/entries);
      step_z->Fill(step.vz,step.Edep/entries);
    }
    for (auto leak : cbdEvt.leaks){
      Leak_PDGID->Fill(leak.pdgId);
      //std::cout << "leak id is "<<leak.pdgId<<std::endl;
      if(leak.pdgId>1000010000&&leak.pdgId<1000020000) Leak_H_p->Fill(sqrt(leak.px*leak.px+leak.py*leak.py+leak.pz*leak.pz));
      if(leak.pdgId>1000020000&&leak.pdgId<1000030000) Leak_He_p->Fill(sqrt(leak.px*leak.px+leak.py*leak.py+leak.pz*leak.pz));
      if(leak.pdgId==11)Leak_electron_p->Fill(sqrt(leak.px*leak.px+leak.py*leak.py+leak.pz*leak.pz));
      if(leak.pdgId==22)Leak_gamma_p->Fill(sqrt(leak.px*leak.px+leak.py*leak.py+leak.pz*leak.pz));
      if(leak.pdgId==2112)Leak_neutron_p->Fill(sqrt(leak.px*leak.px+leak.py*leak.py+leak.pz*leak.pz));
      if(leak.pdgId==2212)Leak_proton_p->Fill(sqrt(leak.px*leak.px+leak.py*leak.py+leak.pz*leak.pz));
    }
    for (auto second : cbdEvt.seconds){
      sec_pos->Fill(second.vx,second.vy,second.vz); 
      PDGID->Fill(second.pdgId);
      //std::cout<< "pdg id is "<<second.pdgId<<std::endl;
      secondary+=1;
      if (flag) {
        sec_evt+=1; 
        flag=0;
      }
      if (second.pdgId == 11) ele +=1;
      if (second.pdgId == -11) posi +=1;
      if (second.pdgId == 2212) neutron +=1;
      if (second.pdgId == 2112) proton +=1;
      if (second.pdgId == 1000060120) carbon +=1;
      if (second.pdgId == 1000010020) deutron +=1;
      if (second.pdgId == 22) gamma +=1;
    }
    flag=1;
    for (auto tower: cbdEvt.towers){
      for (auto sipm:tower.SiPMs){
        tsipmXY->Fill(sipm.x,sipm.y,sipm.count);
        tNhit->Fill(sipm.count);
        int hit = sipm.count;
        nhit+=hit;
        fired_sipm+=1;
        for (const auto timepair:sipm.timeStruct){
          tT->Fill(timepair.first.first+0.005,timepair.second);
          turnontime->Fill(timepair.first.first+0.005,timepair.second);
          if(cbdInterface->numEvt()==982)risetimesipm->Fill(timepair.first.first+0.005,timepair.second);
        }
        for (const auto wavpair: sipm.wavlenSpectrum){
          tWav->Fill(wavpair.first.first,wavpair.second);
        }
        if(cbdInterface->numEvt()==982){
          int risetimesipmbin=risetimesipm->FindFirstBinAbove(0.5);
          if(Edep!=0) tXYriseTime->Fill(sipm.x,sipm.y,risetimesipm->GetBinCenter(risetimesipmbin));
          risetimesipm->Reset();
        }
      }
      for (auto sipmfront:tower.SiPMFronts){
        tsipmXYfront->Fill(sipmfront.x,sipmfront.y,sipmfront.count);
        tNhitfront->Fill(sipmfront.count);
        int hit = sipmfront.count;
        nhitfront+=hit;
        fired_sipm_front+=1;
        for (const auto timepair:sipmfront.timeStruct){
          tTfront->Fill(timepair.first.first+0.005,timepair.second);
          turnontimefront->Fill(timepair.first.first+0.005,timepair.second);
          time1evt->Fill(timepair.first.first+0.005,timepair.second);
          if(cbdInterface->numEvt()==982)risetimesipmfront->Fill(timepair.first.first+0.005,timepair.second);
        }
        for (const auto wavpair: sipmfront.wavlenSpectrum){
          tWavfront->Fill(wavpair.first.first,wavpair.second);
        }
        if(cbdInterface->numEvt()==982){

          int risetimesipmbinfront=risetimesipmfront->FindFirstBinAbove(0.5);
          if(Edep!=0) tXYriseTimefront->Fill(sipmfront.x,sipmfront.y,risetimesipmfront->GetBinCenter(risetimesipmbinfront));
          risetimesipmfront->Reset();
        }
        int turnontimebin_sipm = time1evt->FindFirstBinAbove(0.5);
        timeL2D->Fill(sipmfront.x,sipmfront.y,time1evt->GetBinCenter(turnontimebin_sipm));
        time1evt->Reset();
      }
    }
    for (auto physical:cbdEvt.totPhysicals){

     
      if (cbdInterface->numEvt()==982){
        double xphy =30-(physical.x-424.22)/5.052;
        double yphy = (physical.y+75.78)/5.052;
        double zphy = physical.z/10;
        genparticle2D->Fill(yphy,xphy);
        if(physical.particleID==0)gengamma2D->Fill(yphy,xphy,10);
        if(physical.particleID==1)genelectron2D->Fill(yphy,xphy,20);
        if(physical.particleID==2)genpositron2D->Fill(yphy,xphy,30);
        std::cout<<"particle ID is "<<physical.particleID<<"| particle position is "<<yphy<<","<<xphy<<"| particle energy is "<<physical.energy<<std::endl;

      }

    //timeL_file->WriteTObject(timeL2D);
    }
    int opticalPhotonNumber=0;
    for (auto optical : cbdEvt.opticalPhotons){
      opticalPhotonNumber+=optical.opticalPhotonNumber;

    }
    MCoptical->Fill(opticalPhotonNumber);
    int num_ele_cell=0;
    for (int ii=0; ii<30; ii++){
      for (int jj=0;jj<30;jj++){
        if (genelectron2D->GetBinContent(ii,jj)>1) num_ele_cell+=1;
      }
    }
    ratio_fired_ele->Fill((float)fired_sipm/num_ele_cell);
    ratio_fired_ele_front->Fill((float)fired_sipm_front/num_ele_cell);
    tothit+=nhit;
    totenergy+=energy;
    timeL_file->WriteTObject(timeL2D);
    thits->Fill(nhit);
    thitsfront->Fill(nhitfront);
    //if(Edep!=0)thitstotal->Fill(nhitfront+nhit);
    //if(Edep!=0) tenergy->Fill((nhit+nhitfront)*0.000333838);//21.07.20 calib const 0.00033838
    //if(Edep!=0) tenergy->Fill((nhit+nhitfront)*0.000355723);//LYSO calib const 0.00033838
    //if(Edep!=0) tenergy->Fill((nhit+nhitfront)*0.002009715);//BGO calib const 0.00033838
    //if(Edep!=0) tenergy->Fill((nhit+nhitfront)*0.010096081);//CsI calib const 0.00033838
    if(Edep!=0) tenergy->Fill((nhit)*0.0005188);//LYSO one calib const 0.00033838
    //if(Edep!=0) tenergy->Fill((nhit)*0.003266);//BGO one calib const 0.00033838
    //if(Edep!=0) tenergy->Fill((nhit)*0.01648);//CsI one calib const 0.00033838
    //onecell 0.000333501 total 0.000322373
   
    int risetimebin=turnontime->FindFirstBinAbove(1.5);
    double t_back = turnontime->GetBinCenter(risetimebin);
    if(Edep!=0) tTurnOnTime->Fill(turnontime->GetBinCenter(risetimebin));
    int risetimebinfront=turnontimefront->FindFirstBinAbove(1.5);
    double t_front = turnontimefront->GetBinCenter(risetimebinfront);
    if(Edep!=0) tTurnOnTimefront->Fill(turnontimefront->GetBinCenter(risetimebinfront));
    if(Edep>0.6) tDeltaTime->Fill(t_back-t_front);
  }
  timeL_file->Close();
  cbdInterface->close();
  std::cout << "total evt : "<<entries<<", secondary evt : "<<sec_evt<<std::endl;
  std::cout << "total secondary : "<<secondary<< ", electron :"<<ele<<",positron : "<<posi<< ", gamma : "<<gamma<<", proton : "<<proton<<" , neutron : "<<neutron<<", deutron : "<<deutron<<",carbon : "<<carbon<<std::endl;
  TFile *root_file = new TFile(filename+".root","recreate");


  tXYriseTime->SetStats(kFALSE);
  tXYriseTimefront->SetStats(kFALSE);
   
  thitstotal->Write();
  tTurnOnTime->Write();
  tTurnOnTimefront->Write();
  sec_pos->Write();
  PDGID->Write();
  Leak_PDGID->Write();
  tenergy->Write();
  tEdep->Write();
  tT->Write();
  tWav->Write();
  tNhit->Write();
  thits->Write();
  tTfront->Write();
  tWavfront->Write();
  tNhitfront->Write();
  thitsfront->Write();
  genparticle2D->Write();
  gengamma2D->Write();
  genelectron2D->Write();
  genpositron2D->Write();
  tXYriseTime->Write();
  tXYriseTimefront->Write();
  ratio_fired_ele->Write();
  ratio_fired_ele_front->Write();
  MCoptical->Write();
  tDeltaTime->Write();
  step_pos->Write();
  step_x->Write();
  step_y->Write();
  step_z->Write();
  Leak_electron_p->Write();
  Leak_H_p->Write();
  Leak_He_p->Write();
  Leak_gamma_p->Write();
  Leak_neutron_p->Write();
  Leak_proton_p->Write();
  root_file->Close();

  TFile* deltatimeFile = new TFile(filename+"_deltatime.root","RECREATE");
  deltatimeFile->WriteTObject(tDeltaTime);
  deltatimeFile->Close();

  TFile* riseFile = new TFile(filename+"_risetime0.5.root","RECREATE");
  riseFile->WriteTObject(tTurnOnTime);
  riseFile->WriteTObject(tTurnOnTimefront);
  riseFile->Close();
  
}
