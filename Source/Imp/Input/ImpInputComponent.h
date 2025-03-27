// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "ImpInputConfig.h"
#include "ImpInputComponent.generated.h"

class UImpInputConfig;
/**
 * 
 */
UCLASS()
class IMP_API UImpInputComponent : public UEnhancedInputComponent {

	GENERATED_BODY()
	
public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(UImpInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void UImpInputComponent::BindAbilityActions(UImpInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc) {
	
	check(InputConfig);

	for (const FImpInputAction& Action : InputConfig->ImpInputActions) {
		if (IsValid(Action.InputAction) && Action.InputTag.IsValid()) {
			if (PressedFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc) {
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

		}
	}
}
