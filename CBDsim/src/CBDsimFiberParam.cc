#include "CBDsimFiberParam.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"

CBDsimFiberParam::CBDsimFiberParam(const G4int numx_, const G4int numy_, const fiberSpec* spec)
: G4VPVParameterisation() {
  numx = numx_;
  numy = numy_;
  for (G4int copyNo=0; copyNo<numx*numy; copyNo++) {
    G4int column = copyNo / numy;
    G4int row = copyNo % numy;
    X.push_back( (column-numx/2)*5.052*mm + ( numx%2==0 ? 2.526*mm : 0 ) );
    Y.push_back( (row-numy/2)*5.052*mm + ( numy%2==0 ? 2.526*mm : 0 ) );
  }

  fFiberSpec = spec;
}

CBDsimFiberParam::~CBDsimFiberParam() {}

void CBDsimFiberParam::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const {
  physVol->SetTranslation(G4ThreeVector(X[copyNo],Y[copyNo],0.));
}

void CBDsimFiberParam::ComputeDimensions(G4Tubs& fiber, const G4int copyNo, const G4VPhysicalVolume* physVol) const {
  G4int column = copyNo / numy;
  G4int row = copyNo % numy;

  if (IsCerenkov(column,row)) {
  //if (false){
    physVol->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetMaterial(fFiberSpec->matPMMA);
    physVol->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetVisAttributes(fFiberSpec->visAttrBlue);
  } else {
    physVol->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetMaterial(fFiberSpec->matPS);
    physVol->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetVisAttributes(fFiberSpec->visAttrOrange);
  }
}

G4Material* CBDsimFiberParam::ComputeMaterial(const G4int copyNo, G4VPhysicalVolume* physVol) const {
  G4int column = copyNo / numy;
  G4int row = copyNo % numy;

  if (IsCerenkov(column,row)) return fFiberSpec->matFluoPoly;
  else return fFiberSpec->matPMMA;
}

bool CBDsimFiberParam::IsCerenkov(G4int col, G4int row) const {
  bool isCeren = false;
  if ( col%2 == 1 ) { isCeren = !isCeren; }
  if ( row%2 == 1 ) { isCeren = !isCeren; }
  return isCeren;
}

CBDsimCellParam::CBDsimCellParam(const CBDsimFiberParam* fiberParam) {
  fFiberParam = fiberParam;
}

CBDsimCellParam::~CBDsimCellParam() {}

void CBDsimCellParam::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const {
  physVol->SetTranslation(G4ThreeVector(fFiberParam->X[copyNo],fFiberParam->Y[copyNo],0.));
}
