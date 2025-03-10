#include "ImpGameMode.h"
#include "ImpCharacter.h"
#include "ImpPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AImpGameMode::AImpGameMode() {
    PlayerControllerClass = AImpPlayerController::StaticClass();
}