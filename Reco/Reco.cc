#include "CBDsimInterface.h"
#include "CBDsimRootInterface.h"

#include <iostream>

int main(int argc, char* argv[]) {
  std::string filenum = std::string(argv[1]);
  std::string filename = std::string(argv[2]);

  CBDsimRootInterface* drInterface = new CBDsimRootInterface(filename+"_"+filenum+".root");
  drInterface->set();

  unsigned int entries = drInterface->entries();
  while (drInterface->numEvt() < entries) {
    CBDsimInterface::CBDsimEventData evt;
    drInterface->read(evt);

    for (auto towerItr = evt.towers.begin(); towerItr != evt.towers.end(); ++towerItr) {
      auto tower = *towerItr;

      // do something on the tower
      std::cout << "Tower num = " << tower.towerNum << std::endl;

      for (auto sipmItr = tower.SiPMs.begin(); sipmItr != tower.SiPMs.end(); ++sipmItr) {
        auto sipm = *sipmItr;

        // do something on the sipm
        std::cout << "SiPM num = " << sipm.SiPMnum << " | Count = " << sipm.count << std::endl;

      } // sipm loop
    } // tower loop

    for (auto edepItr = evt.Edeps.begin(); edepItr != evt.Edeps.end(); ++edepItr) {
      auto edep = *edepItr;

      // do something on the Edeps
      std::cout << "Tower num = " << edep.towerNum << " | Edep = " << edep.Edep << " (MeV)" << std::endl;
    }
  } // event loop

  return 0;
}
