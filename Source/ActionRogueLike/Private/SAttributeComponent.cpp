// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	//default value to health
	Health = 100;
}


//iplementation of a function that we see if the character is alive or not
bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

//implementation of the health changes
bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	//whenever the health is changed, call on health change and in order to trigger this - we call broadcast & we have to pass in the parameters from the macro
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}


