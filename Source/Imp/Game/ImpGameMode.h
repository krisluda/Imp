#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ImpGameMode.generated.h"

UCLASS(minimalapi)
class AImpGameMode : public AGameModeBase {
    GENERATED_BODY()

public:
    AImpGameMode();

    void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    void StartPlay() override;
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void PostLogin(APlayerController* NewPlayer) override;
    void Logout(AController* Exiting) override;
};