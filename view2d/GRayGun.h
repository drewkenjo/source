#ifndef GRayGun_h
#define GRayGun_h 1

// Geant4
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"


class G4ParticleGun;

// Class definition
class GRayGun : public G4VUserPrimaryGeneratorAction
{
public:
	GRayGun();
	~GRayGun();

public:
	void GeneratePrimaries(G4Event* anEvent);
	G4ParticleGun* fParticleGun;

	bool isFileOpen() {return false;}
};

#endif


