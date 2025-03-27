// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

ACharacterBase::ACharacterBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay() {
	Super::BeginPlay();
}

void ACharacterBase::InitAbilityActorInfo() {
	//Intentionally blank for now
}
