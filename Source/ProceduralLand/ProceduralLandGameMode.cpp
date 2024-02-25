// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProceduralLandGameMode.h"
#include "ProceduralLandCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProceduralLandGameMode::AProceduralLandGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
