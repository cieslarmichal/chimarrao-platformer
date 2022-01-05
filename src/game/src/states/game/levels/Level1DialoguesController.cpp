#include "Level1DialoguesController.h"
#include "ProjectPathReader.h"

namespace game
{
namespace
{
const std::string dialoguesDirectory = utils::ProjectPathReader::getProjectRootPath() + "dialogues/";
const std::string playerWithRabbitDialogueFile1 = dialoguesDirectory + "player_with_rabbit1.txt";
const std::string playerWithDruidDialogueFile = dialoguesDirectory + "player_with_druid.txt";
}

Level1DialoguesController::Level1DialoguesController(Level1MainCharacters* level1MainCharacters,
                                                     std::unique_ptr<DialoguesReader> dialoguesReaderInit)
    : mainCharacters{level1MainCharacters}, dialoguesReader{std::move(dialoguesReaderInit)}
{
    playerWithRabbitDialogueTrack1 = dialoguesReader->read(playerWithRabbitDialogueFile1);
    playerWithDruidDialogueTrack = dialoguesReader->read(playerWithDruidDialogueFile);
}

void Level1DialoguesController::startPlayerWithRabbitDialogue() {}

void Level1DialoguesController::startPlayerWithDruidDialogue() {}

}
