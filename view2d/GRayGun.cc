// G4 headers
#include "G4DecayTable.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


// gemc headers
#include "GRayGun.h"
#include "string_utilities.h"

GRayGun::GRayGun()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
	G4int n_particle = 1;
	fParticleGun  = new G4ParticleGun(n_particle);

	// default particle kinematic
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("geantino");
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentum(1);
}


GRayGun::~GRayGun()
{
	delete fParticleGun;
}


void GRayGun::GeneratePrimaries(G4Event* anEvent)
{
  double tmp;
  int nparts;
  int id, type, pdg;
  double px, py, pz, ptot, mass;
  double vx, vy, vz, charge;
  char line[1024];

  G4ThreeVector pi0mom(0,0,1);
  fParticleGun->SetParticleMomentumDirection(pi0mom.unit());
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, vz));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

