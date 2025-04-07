#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ImpPlayerState.generated.h"


class UImpAbilitySystemComponent;
class UImpAttributeSet;

/**
 * 
 */
UCLASS()
class IMP_API AImpPlayerState : public APlayerState, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	AImpPlayerState();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	UImpAbilitySystemComponent* GetImpAbilitySystemComponent() const;
	UImpAttributeSet* GetImpAttributeSet() const; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = true))
	TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta  = (AllowPrivateAccess = true))
	TObjectPtr<UImpAttributeSet> ImpAttributeSet;


};
