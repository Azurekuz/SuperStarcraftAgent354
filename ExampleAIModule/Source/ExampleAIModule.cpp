#include "ExampleAIModule.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
  // Hello World!
  Broodwar << "SuperStarcraftBot354 is now activated!" << std::endl;

  Broodwar->setLocalSpeed(5);

  Broodwar->setFrameSkip(0);

  //Create subagents
  workerManager = *new WorkerManager();
  producer = *new Producer();


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

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	//Manage workers
	workerManager.manageWorkers();

	//Produce troops (not yet added)
	//producer.produceTroops();

	//Command troops (TO BE DONE)
	//unitManager.commandUnits();

	// Iterate through all the units that we own
	for (Unit u : Broodwar->self()->getUnits())
	{
		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if (!u->exists())
			continue;

		// Ignore the unit if it has one of the following status ailments
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!u->isCompleted() || u->isConstructing())
			continue;


		// Finally make the unit do some stuff!


		else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
		{

			// Order the depot to construct more workers! But only when it is idle.
			if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
			{
				// If that fails, draw the error at the location so that you can visibly see what went wrong!
				// However, drawing the error once will only appear for a single frame
				// so create an event that keeps it on the screen for some frames
				Position pos = u->getPosition();
				Error lastErr = Broodwar->getLastError();
				Broodwar->registerEvent([pos, lastErr](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
					nullptr,    // condition
					Broodwar->getLatencyFrames());  // frames to run

// Retrieve the supply provider type in the case that we have run out of supplies
				UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
				static int lastCheckedSupply = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastErr == Errors::Insufficient_Supply &&
					lastCheckedSupply + 400 < Broodwar->getFrameCount() &&
					Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
				{
					lastCheckedSupply = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit supplyBuilder = workerManager.getBuilder(u->getTilePosition());
					// If a unit was found
					if (supplyBuilder)
					{
						buildSupply(supplyBuilder, supplyProviderType);
					} // closure: supplyBuilder is valid
				} // closure: insufficient supply
			} // closure: failed to train idle unit

		}

		producer.produceTroops(u);

	} // closure: unit iterator

	static int lastCheckedBuild = 0;

	if (Broodwar->self()->minerals() >= 150 &&
		lastCheckedBuild + 400 < Broodwar->getFrameCount() &&
		Broodwar->self()->incompleteUnitCount(UnitTypes::Terran_Barracks) == 0) {

		lastCheckedBuild = Broodwar->getFrameCount();

		Unit mainBase = nullptr;

		for (auto &u : Broodwar->self()->getUnits())
		{
			if (u->getType().isResourceDepot()) {
				mainBase = u;
			}
		}

		if (mainBase != nullptr) {

			UnitType buildingType = UnitTypes::Terran_Barracks;
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(buildingType, mainBase->getTilePosition());

			Unit builder = workerManager.getBuilder(targetBuildLocation);

			// If a unit was found
			if (builder)
			{
				if (targetBuildLocation)
				{
					// Register an event that draws the target build location
					Broodwar->registerEvent([targetBuildLocation, buildingType](Game*)
					{
						Broodwar->drawBoxMap(Position(targetBuildLocation),
							Position(targetBuildLocation + buildingType.tileSize()),
							Colors::Green);
					},
						nullptr,  // condition
						buildingType.buildTime() + 100);  // frames to run

														  // Order the builder to construct the supply structure
					builder->build(buildingType, targetBuildLocation);
				}
			} // closure: supplyBuilder is valid

		}



	}
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
	addUnit(unit);
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
	}
	
	else if (unit->getType() == UnitTypes::Buildings) {
		producer.addBuilding(unit);
	}
	else if (unitManager.addUnit(unit)) {
		//Function will return true or false depending on if unit is combat unit.
	}
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
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

	else if (unit->getType() == UnitTypes::Buildings) {
		producer.removeBuilding(unit);
	}
	else if (unitManager.removeUnit(unit)) {
		//Function will return true or false depending on if unit is combat unit.
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
