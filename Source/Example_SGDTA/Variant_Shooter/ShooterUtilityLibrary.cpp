// Copyright Start Games, Inc. All Rights Reserved.

#include "ShooterUtilityLibrary.h"

#include "ShooterUIManagerComponent.h"

#include "Engine/Engine.h"

#include "GameFramework/GameStateBase.h"

void UShooterUtilityLibrary::SendShooterUIMessage(const UObject* WorldContextObject, FShooterUIMessage Message)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!world)
	{
		return;
	}
	AGameStateBase* gamestate = world->GetGameState();
	if (!gamestate)
	{
		return;
	}
	UShooterUIManagerComponent* uiManager = gamestate->FindComponentByClass<UShooterUIManagerComponent>();
	if (!uiManager)
	{
		return;
	}
	uiManager->OnMessageReceived.Broadcast(uiManager, Message);
}
