#ifndef MyOutput_h
#define MyOutput_h 1

#include "TTree.h"
#include "TFile.h"
#include "G4String.hh"
#include "G4ThreeVector.hh"

class MyOutput
{
	private:
		static MyOutput* instance;
		MyOutput();

		TFile* ff;
		TTree *trk22, *stp22, *dgt22;

		Char_t ievent, sec, layer, wire;
		float vx, vy, vz;
		float px, py, pz;
		int tid, pid, pdg;
		char procName[222];
		char detName[222];

	public:
		static MyOutput* getInstance();

		void EndOfEvent();
		void SetPosition(G4ThreeVector);
		void SetMomentum(G4ThreeVector);
		void SetTid(int);
		void SetTidPidPdg(int,int,int);
		void SetDC(int,int,int,int);
		void SetNames(G4String, G4String);
		void FillTrack();
		void FillStep();
		void FillDgtz();
		void Write();
};


#endif

