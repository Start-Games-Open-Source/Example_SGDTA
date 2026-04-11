// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ShooterUIMessage.h"

#include "ShooterUIManagerComponent.generated.h"

class UShooterUIManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUIMessageDelegate, UShooterUIManagerComponent*, ManagerComponent, FShooterUIMessage, UiMessage);

/**
 * UI Manager component for shooter game variant intended to be placed on the game state actor.
 */
UCLASS(Abstract, ClassGroup="Example SGDTA", Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class EXAMPLE_SGDTA_API UShooterUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UShooterUIManagerComponent();

	UPROPERTY(BlueprintAssignable, Category = "Shooter UI Manager")
	FUIMessageDelegate OnMessageReceived;
};
