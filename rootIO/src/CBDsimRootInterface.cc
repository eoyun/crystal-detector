#include "CBDsimRootInterface.h"

CBDsimRootInterface::CBDsimRootInterface(const std::string& filename)
: fFilename(filename), fNumEvt(0) {
  init();
}

CBDsimRootInterface::~CBDsimRootInterface() {}

void CBDsimRootInterface::PrepareChain(){
  fEventData = new CBDsimInterface::CBDsimEventData();
}

void CBDsimRootInterface::GetChain(const std::string& treename){
  fChain = new TChain(treename.c_str());
  fChain->Add((fFilename+"/*.root").c_str());
  fTree = fChain;
  fTree->SetBranchAddress((treename+"EventData").c_str(),&fEventData);
}

void CBDsimRootInterface::init() {
  fEventData = new CBDsimInterface::CBDsimEventData();
  fFile = TFile::Open(fFilename.c_str(),"UPDATE");
}

void CBDsimRootInterface::create() {
  fTree = new TTree("CBDsim","CBDsim");
  fTree->Branch("CBDsimEventData",fEventData);
}

void CBDsimRootInterface::set() {
  fTree = (TTree*)fFile->Get("CBDsim");
  fTree->SetBranchAddress("CBDsimEventData",&fEventData);
}

void CBDsimRootInterface::fill(const CBDsimInterface::CBDsimEventData* evt) {
  *fEventData = *evt;
  fTree->Fill();
}

void CBDsimRootInterface::read(CBDsimInterface::CBDsimEventData& evt) {
  fTree->GetEntry(fNumEvt);
  evt = *fEventData;
  fNumEvt++;
}

void CBDsimRootInterface::write() {
  fFile->WriteTObject(fTree);
}

void CBDsimRootInterface::close() {
  if (fFile) fFile->Close();
  if (fEventData) delete fEventData;
  if (fChain) delete fChain;
}
