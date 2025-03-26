// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImpWidget.generated.h"

class UImpWidgetController;


UCLASS()
class IMP_API UImpWidget : public UUserWidget {
	GENERATED_BODY()

public:

	void SetWidgetController(UImpWidgetController* InWidgetController); 

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UImpWidgetController> WidgetController;
};
