// G4 headers
#include "G4ParticleTypes.hh"
#include "G4VProcess.hh"
// gemc headers
#include "MTrackingAction.h"
#include "MyOutput.h"

MTrackingAction::MTrackingAction(){
}

void MTrackingAction::PreUserTrackingAction(const G4Track* aTrk){
	MyOutput* mout = MyOutput::getInstance();
	mout->SetTidPidPdg(aTrk->GetTrackID(), aTrk->GetParentID(), aTrk->GetDefinition()->GetPDGEncoding());
	if(aTrk->GetParentID()>0)
		mout->SetNames(aTrk->GetCreatorProcess()->GetProcessName(), aTrk->GetVolume()->GetName());
	else
		mout->SetNames("GEN", aTrk->GetVolume()->GetName());
	mout->SetPosition(aTrk->GetPosition());
	mout->SetMomentum(aTrk->GetMomentum());
	mout->FillTrack();
}

void MTrackingAction::PostUserTrackingAction(const G4Track* aTrk){
}

