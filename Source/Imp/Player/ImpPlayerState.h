#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ImpAbilitySystemComponent.h"
#include "ImpPlayerState.generated.h"


class UImpAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class IMP_API AImpPlayerState : public APlayerState, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	AImpPlayerState();

	virtual void BeginPlay() override;


	//GAS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComponent;

	virtual UImpAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//GAS -- END
};
