#ifndef CBDsimRootInterface_h
#define CBDsimRootInterface_h 1

#include "CBDsimInterface.h"
#include "TFile.h"
#include "TTree.h"

class CBDsimRootInterface {
public:
  CBDsimRootInterface(const std::string& filename);
  ~CBDsimRootInterface();

  void fill(const CBDsimInterface::CBDsimEventData* evt);
  void read(CBDsimInterface::CBDsimEventData& evt);
  void create();
  void set();
  void write();
  void close();

  unsigned int entries() { return fTree->GetEntries(); }
  unsigned int numEvt() { return fNumEvt; }

private:
  void init();

  TFile* fFile;
  TTree* fTree;
  std::string fFilename;
  CBDsimInterface::CBDsimEventData* fEventData;
  unsigned int fNumEvt;
};

#endif
