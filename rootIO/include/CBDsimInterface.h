#ifndef CBDsimInterface_h
#define CBDsimInterface_h 1

#include <vector>
#include <utility>
#include <map>

class CBDsimInterface {
public:
  CBDsimInterface() {};
  ~CBDsimInterface() {};

  typedef std::pair<float,float> hitRange;
  typedef std::pair<int,int> hitXY;
  typedef std::map<hitRange, int> CBDsimTimeStruct;
  typedef std::map<hitRange, float> CBDsimWaveForm;
  typedef std::map<hitRange, int> CBDsimWavlenSpectrum;

  struct CBDsimSiPMData {
    CBDsimSiPMData() {};
    virtual ~CBDsimSiPMData() {};

    int count;
    int SiPMnum;
    int x;
    int y;
    //threeVector pos;
    CBDsimTimeStruct timeStruct;
    CBDsimWaveForm waveForm;
    CBDsimWavlenSpectrum wavlenSpectrum;
  };

  struct CBDsimPhysicalevent{
    CBDsimPhysicalevent() {};
    virtual ~CBDsimPhysicalevent() {};

    double x;
    double y;
    double z;
    int physicalID;
    int particleID;
    double energy;
  };

  struct CBDsimTowerData {
    CBDsimTowerData() {};
    virtual ~CBDsimTowerData() {};

    int numx;
    int numy;
    int towerNum;
    std::vector<CBDsimSiPMData> SiPMs;
    std::vector<CBDsimSiPMData> SiPMFronts;
  };

  struct CBDsimEdepData {
    CBDsimEdepData() {};
    virtual ~CBDsimEdepData() {};

    float Edep;
    int towerNum;
  };

  struct CBDsimPhoton {
    CBDsimPhoton() {};
    virtual ~CBDsimPhoton() {};

    int opticalPhotonNumber;

  };

  struct CBDsimLeakageData {
    CBDsimLeakageData() {};
    virtual ~CBDsimLeakageData() {};

    float E;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;
    float vt;
    int pdgId;
  };

  struct CBDsimSecondaryData {
    CBDsimSecondaryData() {};
    virtual ~CBDsimSecondaryData() {};

    float E;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;
    float vt;
    int pdgId;
    int trackId;
  };
  
  struct CBDsimStepData {
    CBDsimStepData() {};
    virtual ~CBDsimStepData() {};

    float E;
    float Edep;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;
    float vt;
    int pdgId;
    int trackId;
  };


  struct CBDsimGenData {
    CBDsimGenData() {};
    virtual ~CBDsimGenData() {};

    float E;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;
    float vt;
    int pdgId;
  };


  struct CBDsimEventData {
    CBDsimEventData() {};
    virtual ~CBDsimEventData() {};

    int event_number;
    std::vector<CBDsimTowerData> towers;
    std::vector<CBDsimEdepData> Edeps;
    std::vector<CBDsimLeakageData> leaks;
    std::vector<CBDsimSecondaryData> seconds;
    std::vector<CBDsimStepData> steps;
    std::vector<CBDsimGenData> GenPtcs;
    std::vector<CBDsimPhoton> opticalPhotons;
    std::vector<CBDsimPhysicalevent> totPhysicals;
  };

};

#endif
