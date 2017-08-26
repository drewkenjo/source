/// \file ActionInitialization.cc
/// \brief Implementation of the ActionInitialization class

#include "MyOutput.h"

MyOutput* MyOutput::instance = 0;

MyOutput* MyOutput::getInstance()
{
	if(instance==0)
		instance = new MyOutput();

	return instance;
}

MyOutput::MyOutput()
{
	ff = new TFile("out.root", "recreate");
	trk22 = new TTree("trk22", "track info");
	trk22->Branch("ievent",&ievent,"ievent/B");
	trk22->Branch("tid",&tid,"tid/I");
	trk22->Branch("pid",&pid,"pid/I");
	trk22->Branch("pdg",&pdg,"pdg/I");
	trk22->Branch("vx",&vx,"vx/F");
	trk22->Branch("vy",&vy,"vy/F");
	trk22->Branch("vz",&vz,"vz/F");
	trk22->Branch("px",&px,"px/F");
	trk22->Branch("py",&py,"py/F");
	trk22->Branch("pz",&pz,"pz/F");
	trk22->Branch("procName",procName,"procName/C");
	trk22->Branch("detName",detName,"detName/C");

	stp22 = new TTree("stp22", "step info");
	stp22->Branch("ievent",&ievent,"ievent/B");
	stp22->Branch("tid",&tid,"tid/I");
	stp22->Branch("vx",&vx,"vx/F");
	stp22->Branch("vy",&vy,"vy/F");
	stp22->Branch("vz",&vz,"vz/F");
	stp22->Branch("px",&px,"px/F");
	stp22->Branch("py",&py,"py/F");
	stp22->Branch("pz",&pz,"pz/F");
	stp22->Branch("procName",procName,"procName/C");
	stp22->Branch("detName",detName,"detName/C");

	dgt22 = new TTree("dgt22", "dgtz info");
	dgt22->Branch("ievent",&ievent,"ievent/B");
	dgt22->Branch("tid",&tid,"tid/I");
	dgt22->Branch("sec",&sec,"sec/B");
	dgt22->Branch("layer",&layer,"layer/B");
	dgt22->Branch("wire",&wire,"wire/B");

	ievent=0;
}

void MyOutput::Write()
{
	trk22->Write();
	stp22->Write();
	dgt22->Write();
	delete ff;
}

void MyOutput::SetTid(int _tid){
	tid = _tid;
}

void MyOutput::SetTidPidPdg(int _tid, int _pid, int _pdg){
	tid = _tid;
	pid = _pid;
	pdg = _pdg;
}

void MyOutput::SetPosition(G4ThreeVector pos){
	vx=pos.x(); vy=pos.y(); vz=pos.z();
}

void MyOutput::SetMomentum(G4ThreeVector mom){
	px=mom.x(); py=mom.y(); pz=mom.z();
}

void MyOutput::SetNames(G4String _procName, G4String _detName){
	strcpy(procName, _procName.data());
	strcpy(detName, _detName.data());
}

void MyOutput::SetDC(int _tid, int _sec, int _layer, int _wire){
	tid = _tid;
	sec = (Char_t) _sec;
	layer = (Char_t) _layer;
	wire = (Char_t) _wire;
}

void MyOutput::EndOfEvent(){
	ievent++;
}

void MyOutput::FillTrack(){
	trk22->Fill();
}

void MyOutput::FillStep(){
	stp22->Fill();
}

void MyOutput::FillDgtz(){
	dgt22->Fill();
}
