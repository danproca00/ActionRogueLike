// Fill out your copyright notice in the Description page of Project Settings.
#include "SAttributeComponent.h"
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}
bool USAttributeComponent::Kill(AActor InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}
bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
float USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}
float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


//bool USAttributeComponent::ApplyHealthChange(float Delta)
bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	//god mode
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	//OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta); // @fixme: Still nullptr for InstigatorActor parameter
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}


bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}