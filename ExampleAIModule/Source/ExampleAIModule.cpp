#include "ExampleAIModule.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
  // Hello World!
  Broodwar << "SuperStarcraftBot354 is now activated!" << std::endl;
  Broodwar << Broodwar->enemy()->getStartLocation() << std::endl;
  Broodwar->setLocalSpeed(5);

  Broodwar->setFrameSkip(0);

    //Find starting CC
  Unit homeBase;

  for (Unit u : Broodwar->self()->getUnits()) {
	  if (u->getType() == UnitTypes::Terran_Command_Center) {
		  homeBase = u;
	  }
  }

  //Create subagents
  workerManager = *new WorkerManager();
  producer = *new Producer();
  unitManager = *new UnitManager();
  builder = *new Builder(&workerManager, &producer, homeBase);

  //Add minerals to lists
  for (auto &u : Broodwar->getStaticMinerals()) {
	  addUnit(u);
  }

  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

  // Enable the UserInput flag, which allows us to control the bot and type messages.
  Broodwar->enableFlag(Flag::UserInput);

  // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
  //Broodwar->enableFlag(Flag::CompleteMapInformation);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Broodwar->setCommandOptimizationLevel(2);

  // Check if this is a replay
  if ( Broodwar->isReplay() )
  {

    // Announce the players in the replay
    Broodwar << "The following players are in this replay:" << std::endl;
    
    // Iterate all the players in the game using a std:: iterator
    Playerset players = Broodwar->getPlayers();
    for(auto p : players)
    {
      // Only print the player if they are not an observer
      if ( !p->isObserver() )
        Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
    }

  }
  else // if this is not a replay
  {
    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Broodwar->enemy() ) // First make sure there is an enemy
      Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
  }

}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
	// Called once every game frame

	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
	Broodwar->drawBoxMap(Position(Broodwar->enemy()->getStartLocation()), Position(Broodwar->enemy()->getStartLocation() + BWAPI::UnitTypes::Terran_Command_Center.tileSize()), Colors::Red);
	Broodwar->drawBoxMap(Position(Broodwar->self()->getStartLocation()), Position(Broodwar->self()->getStartLocation() + BWAPI::UnitTypes::Terran_Command_Center.tileSize()), Colors::White);

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	//Manage workers
	workerManager.manageWorkers();

	//Produce Troops and research
	producer.trainTroops();
	producer.research();
	
	//Command troops (TO BE DONE)
	unitManager.commandUnits();

	//Build
	builder.checkBuild();
}

void ExampleAIModule::buildSupply(Unit supplyBuilder, UnitType supplyProviderType) {
	if (supplyProviderType.isBuilding())
	{
		TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
		if (targetBuildLocation)
		{
			// Register an event that draws the target build location
			Broodwar->registerEvent([targetBuildLocation, supplyProviderType](Game*)
			{
				Broodwar->drawBoxMap(Position(targetBuildLocation),
					Position(targetBuildLocation + supplyProviderType.tileSize()),
					Colors::Blue);
			},
				nullptr,  // condition
				supplyProviderType.buildTime() + 100);  // frames to run

														// Order the builder to construct the supply structure
			supplyBuilder->build(supplyProviderType, targetBuildLocation);
		}
	}
	else
	{
		// Train the supply provider (Overlord) if the provider is not a structure
		supplyBuilder->train(supplyProviderType);
	}
}

void ExampleAIModule::onSendText(std::string text)
{

	// Send the text to the game if it is not being processed.
	//Broodwar->sendText("%s", text.c_str());


	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	//Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar << "Goodbye " << player->getName().c_str() << "!" << std::endl;
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar << "Where's the nuke?" <<std::endl;
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
	std::list<BWAPI::Unit> theirUnits;
	if (std::find(theirUnits.begin(), theirUnits.end(), unit) != theirUnits.end()) {
		unitManager.allAttack(unit);
	}
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)  //DON'T USE THIS, it is called when production is started, use onUnitComplete instead
{
  
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self()) {
		addUnit(unit);
	}
}

void ExampleAIModule::addUnit(BWAPI::Unit unit) {
	if (unit->getType() == Broodwar->self()->getRace().getWorker()) {
		workerManager.addWorker(unit);
	}

	if (unit->getType() == BWAPI::UnitTypes::Terran_Refinery) {
		workerManager.addResource(unit);
	}

	if (unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) {
		workerManager.addResource(unit);
	}

	if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
		workerManager.addCC(unit);
		producer.addBuilding(unit);
	}
	
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks) {
		producer.addBuilding(unit);
		builder.barracksCount += 1;
	
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Factory) {
		producer.addBuilding(unit);
		builder.factoriesCount += 1;

	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Starport) {
		producer.addBuilding(unit);

	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop) {
		producer.addBuilding(unit);

	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Armory) {
		producer.addBuilding(unit);
	
	}
	else if (unitManager.sortUnit(unit)) {
		unitManager.addUnit(unit);
	}
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
	unitManager.retaliate(unit->getPosition());
	removeUnit(unit);
}

void ExampleAIModule::removeUnit(BWAPI::Unit unit) {
	if (unit->getType() == Broodwar->self()->getRace().getWorker()) {
		workerManager.removeWorker(unit);
	}

	if (unit->getType() == BWAPI::UnitTypes::Terran_Refinery) {
		workerManager.removeResource(unit);
	}

	if (unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) {
		workerManager.removeResource(unit);
	}

	if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
		workerManager.addCC(unit);
		producer.removeBuilding(unit);
	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks) {
		producer.removeBuilding(unit);

	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Factory) {
		producer.removeBuilding(unit);

	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Starport) {
		producer.removeBuilding(unit);

	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop) {
		producer.removeBuilding(unit);

	}

	else if (unit->getType() == BWAPI::UnitTypes::Terran_Armory) {
		producer.removeBuilding(unit);

	}

	
	else if (unitManager.removeUnit(unit)) {
		//Function will return true or false depending on if the unit is a combat unit.
	}
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}
