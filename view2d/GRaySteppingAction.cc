#include "GRaySteppingAction.h"


void GRaySteppingAction::UserSteppingAction(const G4Step* aStep)
{
	G4StepPoint* stp = aStep->GetPostStepPoint();
	if(stp){
		points.push_back(stp->GetPosition());
/*
		if(stp->GetPhysicalVolume()){
//			std::cout<<stp->GetPhysicalVolume()->GetName()<<" ";
			std::cout<<stp->GetPosition().x()<<std::endl;
			std::cout<<stp->GetPosition().y()<<std::endl;
			std::cout<<stp->GetPosition().z()<<std::endl;
		}
*/
	}

}

