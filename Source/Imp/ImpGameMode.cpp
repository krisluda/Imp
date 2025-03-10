#include "ImpGameMode.h"
#include "ImpCharacter.h"
#include "ImpPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AImpGameMode::AImpGameMode() {
    DefaultPawnClass = nullptr;
    PlayerControllerClass = AImpPlayerController::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
}

void AImpGameMode::PostLogin(APlayerController* NewPlayer) {
    Super::PostLogin(NewPlayer);

    if (NewPlayer) {
        APawn* NewPawn = GetWorld()->SpawnActor<AImpCharacter>(DefaultPawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (NewPawn) {
            NewPlayer->Possess(NewPawn);
        }
    }
}