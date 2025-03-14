// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAbilitySystemLibrary.h"
#include "ImpGameMode.h"
#include "Kismet/GameplayStatics.h"


UImpCharacterClassInfo* UImpAbilitySystemLibrary::GetImpCharacterClassDefaultInfo(const UObject *WorldContextObject) {
    if (const AImpGameMode* ImpGameMode = Cast<AImpGameMode>(UGameplayStatics::GetGameMode(WorldContextObject))) {
        return ImpGameMode->GetImpCharacterClassDefaultInfo();
    }

    return nullptr;
}