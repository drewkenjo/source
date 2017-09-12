const char *GEMC_VERSION = "gemc 2.6";

// G4 headers
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4VisExecutive.hh"
#include "G4VModularPhysicsList.hh"
#include "G4PropagatorInField.hh"
#include "G4TransportationManager.hh"
#include "G4UIQt.hh"
#include "G4Qt.hh"

// Qt headers
#include <QApplication>
#include <QSplashScreen>

// gemc headers
#include "HitProcess_MapRegister.h"
#include "detector_factory.h"
#include "gemc_MainGui.h"
#include "gbank.h"
#include "MDetectorConstruction.h"
#include "outputFactory.h"
#include "HitProcess.h"
#include "PhysicsList.h"
#include "options.h"
#include "dmesg_init.h"
#include "run_conditions.h"
#include "fieldFactory.h"
#include "material_factory.h"
#include "mirrors_factory.h"
#include "parameter_factory.h"
#include "string_utilities.h"
#include "utils.h"
#include "window.h"
#include "GRayGun.h"
#include "GRaySteppingAction.h"

// c++ headers
#include <unistd.h>  // needed for get_pid

#ifdef _MSC_VER
#include <stdio.h>
#include <process.h>
	//int get_pid(){return __get_pid();}
	int get_pid(){return 0;}
#endif

int main( int argc, char **argv )
{
	goptions gemcOpt;
	gemcOpt.setGoptions();
	gemcOpt.optMap["USE_GUI"].arg = 0;
	gemcOpt.optMap["NO_FIELD"].args = "all";
	gemcOpt.setOptMap(argc, argv);
	
	gui_splash gemc_splash(gemcOpt);
	
	
	// random seed initialization
	CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
	G4int seed;
	
	if(gemcOpt.optMap["RANDOM"].args=="TIME")
	{
		gemc_splash.message(" Initializing CLHEP Random Engine from local time " \
							+ stringify((double) time(NULL)) \
							+ ", cpu clock "        \
							+ stringify((double) clock())    \
							+ " and process id "    \
							+ stringify(getpid()) + ".");
		seed = (G4int) ( (double) time(NULL)- (double) clock()-getpid() );
	}
	else
	{
		seed = atoi(gemcOpt.optMap["RANDOM"].args.c_str());
		gemc_splash.message(" Initializing CLHEP Random Engine from user defined seed.");
	}
	
	CLHEP::HepRandom::setTheSeed(seed);
	gemc_splash.message(" Seed initialized to: " + stringify(seed));
	
	// Construct the default G4 run manager
	gemc_splash.message(" Instantiating Run Manager...");
	G4RunManager *runManager = new G4RunManager;
	
	// Initializing run_condition class
	gemc_splash.message(" Instantiating Run Conditions...");
	runConditions runConds(gemcOpt);
	
	
	// GEMC Detector Map
	gemc_splash.message(" Registering Detectors Factories...");
	// Initializing Detector Factory
	map<string, detectorFactoryInMap> detectorFactoryMap = registerDetectorFactory();
	// Building detector with factories
	map<string, detector> hallMap = buildDetector(detectorFactoryMap, gemcOpt, runConds);
	
	
	// Initialize Materials Map Factory
	gemc_splash.message(" Initializing Material Factories..." );
	map<string, materialFactory> materialFactoriesMap = registerMaterialFactories();
	// Build all materials
	map<string, G4Material*> mats = buildMaterials(materialFactoriesMap, gemcOpt, runConds);
	
	// Initialize Mirrors Map Factory
	gemc_splash.message(" Initializing Mirrors Factories..." );
	map<string, mirrorFactory> mirrorFactoriesMap = registerMirrorFactories();
	// Build all mirrors
	map<string, mirror*> mirs = buildMirrors(mirrorFactoriesMap, gemcOpt, runConds);
	
	
	// Initialize Parameters Map Factory
	gemc_splash.message(" Registering Parameters Factories...");
	map<string, parameterFactoryInMap> parameterFactoriesMap = registerParameterFactories();
	// All Parameters with factories
	map<string, double> gParameters = loadAllParameters(parameterFactoriesMap, gemcOpt, runConds);
	
	
	// Process Hit Map
	gemc_splash.message(" Building gemc Process Hit Factory...");
	map<string, HitProcess_Factory> hitProcessMap = HitProcess_Map(gemcOpt.optMap["HIT_PROCESS_LIST"].args);
	
	///< magnetic Field Map
	gemc_splash.message(" Creating fields Map...");
	map<string, fieldFactoryInMap> fieldFactoryMap = registerFieldFactories();
	map<string, gfield> fieldsMap = loadAllFields(fieldFactoryMap, gemcOpt);
	
	// Build the detector
	gemc_splash.message(" Building Detector Map...");
	MDetectorConstruction* ExpHall = new MDetectorConstruction(gemcOpt);
	ExpHall->hallMap   = &hallMap;
	ExpHall->mirs      = &mirs;
	ExpHall->mats      = &mats;
	ExpHall->fieldsMap = &fieldsMap;
	// this is what calls Construct inside MDetectorConstruction
	runManager->SetUserInitialization(ExpHall);
	
	
	///< Physics List
	string phys_list = gemcOpt.optMap["PHYSICS"].args  ;
	gemc_splash.message(" Initializing Physics List " + phys_list + "...");
	runManager->SetUserInitialization(new PhysicsList(gemcOpt));

	// Setting Max step for all the simulation.
	// Notice: on the forum:
	// http://hypernews.slac.stanford.edu/HyperNews/geant4/get/emfields/183/1.html
	// it is mentioned that going through volumes of different materials could create problems.
	// this is verified in clas12 when going from target to "hall" - even when vacuum was not involved.
	// the solution to that was to create a transitional tube from target to the vacuum line
	// This solution allowed to avoid setting MAX_FIELD_STEP to a value that would slow down the
	// simulation by a factor of 5
	
	
	double max_step = gemcOpt.optMap["MAX_FIELD_STEP"].arg;
	if(max_step != 0)
		G4TransportationManager::GetTransportationManager()->GetPropagatorInField()->SetLargestAcceptableStep(max_step);
	
	
	// User action initialization
	gemc_splash.message(" Initializing User Actions...");

	GRayGun* gGun = new GRayGun();
	runManager->SetUserAction(gGun);
	GRaySteppingAction* stpAction = new GRaySteppingAction();
	runManager->SetUserAction(stpAction);

	///< User Interface manager
	gemc_splash.message(" Initializing User Interface...");

	// Output File: registering output type, output process factory,
	// sensitive detectors into Event Action
	gemc_splash.message(" Initializing Output Action...");
	outputContainer outContainer(gemcOpt);
	map<string, outputFactoryInMap> outputFactoryMap = registerOutputFactories();

	// Initialize G4 kernel
	gemc_splash.message(" Initializing Run Manager...\n");
	// physical volumes, sensitive detectors are built here
	runManager->Initialize();

	
	
	// registering activated field in the option so they're written out
	if(ExpHall->activeFields.size())
	gemcOpt.optMap["ACTIVEFIELDS"].args = "";
	
	for(set<string>::iterator fit = ExpHall->activeFields.begin(); fit != ExpHall->activeFields.end(); fit++)
		gemcOpt.optMap["ACTIVEFIELDS"].args = gemcOpt.optMap["ACTIVEFIELDS"].args + *fit + " ";
	
	
	// Creating the sim_condition map to save to the output
	gemc_splash.message(" Writing simulation parameters in the output...");
	
	// filling gcard option content
	map<string, string> sim_condition = gemcOpt.getOptMap();
	// adding detectors conditions to sim_condition
	mergeMaps(sim_condition, runConds.getDetectorConditionsMap());
	// adding parameters value to sim_condition
	mergeMaps(sim_condition, getParametersMap(gParameters));

	
	// Bank Map, derived from sensitive detector map
	gemc_splash.message(" Creating gemc Banks Map...");
	map<string, gBank> banksMap = read_banks(gemcOpt, runConds.get_systems());
		
	// Getting UI manager, restoring G4Out to cout
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	UImanager->SetCoutDestination(NULL);

	// saving simulation condition in the output file
	if(outContainer.outType != "no")
	{
		outputFactory *processOutputFactory  = getOutputFactory(&outputFactoryMap, outContainer.outType);
		processOutputFactory->recordSimConditions(&outContainer, sim_condition);
		// then deleting process output pointer, not needed anymore
		delete processOutputFactory;
	}
	
	///< passing output process factory to sensitive detectors
	map<string, sensitiveDetector*>::iterator it;
	for(it = ExpHall->SeDe_Map.begin(); it != ExpHall->SeDe_Map.end(); it++)
		it->second->hitProcessMap = &hitProcessMap;

	gemc_splash.message(" Executing initial directives...\n");
	vector<string> init_commands = init_dmesg(gemcOpt);
	for(unsigned int i=0; i<init_commands.size(); i++)
		UImanager->ApplyCommand(init_commands[i].c_str());
	string exec_macro = "/control/execute " + gemcOpt.optMap["EXEC_MACRO"].args;
	
	QApplication app(argc, argv);
	Window window(UImanager, gGun, stpAction);
	window.show();
	return app.exec();
}








