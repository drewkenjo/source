/// \file MTrackingAction.h
/// Defines the gemc Tracking Action class.\n
/// \author \n Maurizio Ungaro
/// \author mail: ungaro@jlab.org\n\n\n

#ifndef MTrackingAction_h
#define MTrackingAction_h 1

// G4 headers
#include "G4UserTrackingAction.hh"


class MTrackingAction : public G4UserTrackingAction
{
	public:
		MTrackingAction();
		~MTrackingAction(){};

		void PreUserTrackingAction(const G4Track*);	
		void PostUserTrackingAction(const G4Track*);	
};

#endif
