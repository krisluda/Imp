#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ImpGameMode.generated.h"

class UImpCharacterClassInfo;
class UProjectileInfo;

UCLASS()
class IMP_API AImpGameMode : public AGameMode {
    GENERATED_BODY()

public:

    AImpGameMode();

    void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    void StartPlay() override;
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void PostLogin(APlayerController* NewPlayer) override;
    void Logout(AController* Exiting) override;

    //These getters somehow interact with abstract getters in the ImpAbilitySystemLibrary. 
    //We don't use these directly, but via the blueprint library that then gets these off the game mode.
    UImpCharacterClassInfo* GetImpCharacterClassDefaultInfo() const;
    UProjectileInfo* GetProjectileInfo() const;

private:
    //kjører dette i private fordi Uhr gjør det i tutorialen på youtube, 
    //og han kommer dermed til å bruke public getter-funksjonen senere, 
    //på et tidspunkt jeg sikkert kommer til å bli forvirret over å ikke gjøre det likt.
    //Jeg mistenker likevel at dette ikke TRENGER å være i private av den grunn, men akk.

    UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Class Defaults")
    TObjectPtr<UImpCharacterClassInfo> ClassDefaults;

    UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Projectiles")
    TObjectPtr<UProjectileInfo> ProjectileInfo;
};