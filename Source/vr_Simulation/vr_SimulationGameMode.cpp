// Copyright Epic Games, Inc. All Rights Reserved.

#include "vr_SimulationGameMode.h"
#include "vr_SimulationCharacter.h"
#include "UObject/ConstructorHelpers.h"

Avr_SimulationGameMode::Avr_SimulationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
