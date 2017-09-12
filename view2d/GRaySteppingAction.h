#ifndef GRaySteppingAction_h
#define GRaySteppingAction_h 1

// G4 headers
#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class GRaySteppingAction : public G4UserSteppingAction
{
	public:
		GRaySteppingAction(){};
		virtual ~GRaySteppingAction(){};
		
		void UserSteppingAction(const G4Step*);
};

#endif
