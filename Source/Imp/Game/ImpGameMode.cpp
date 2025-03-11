#include "ImpGameMode.h"
#include "ImpCharacter.h"
#include "ImpPlayerController.h"
#include "Log.h"
#include "UObject/ConstructorHelpers.h"

AImpGameMode::AImpGameMode() {
    DefaultPawnClass = nullptr;
    PlayerControllerClass = AImpPlayerController::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
    bUseSeamlessTravel = true;
}

void AImpGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
    Super::InitGame(MapName, Options, ErrorMessage);
    IMP_LOG("AImpGameMode::InitGame: MapName: %s, Options: %s", *MapName, *Options);
}

void AImpGameMode::StartPlay() {
    Super::StartPlay();
    IMP_LOG("AImpGameMode::StartPlay");
}

void AImpGameMode::BeginPlay() {
    Super::BeginPlay();
    IMP_LOG("AImpGameMode::BeginPlay");
}

void AImpGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    IMP_LOG("AImpGameMode::EndPlay: EndPlayReason: %d", (int)EndPlayReason);
}

void AImpGameMode::PostLogin(APlayerController* NewPlayer) {
    Super::PostLogin(NewPlayer);
    IMP_LOG("AImpGameMode::PostLogin: NewPlayer: %s", *NewPlayer->GetName());
}

void AImpGameMode::Logout(AController* Exiting) {
    Super::Logout(Exiting);
    IMP_LOG("AImpGameMode::Logout: Exiting: %s", *Exiting->GetName());
}
