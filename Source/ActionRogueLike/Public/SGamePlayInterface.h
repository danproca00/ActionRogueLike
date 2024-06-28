// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGamePlayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API ISGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//to expose it in the blueprints later on 
	UFUNCTION(BlueprintNativeEvent)//expects a cpp implementation, but we can still implement it in a blueprint

	void Interact(APawn* InstigatorPawn /*to find out who trigger this function -> pass in the player*/);
};
