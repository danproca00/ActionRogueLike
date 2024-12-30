// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"


class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	USActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTag;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

	UFUNCTION()
	void OnRep_RepData();

public:

	void Initialize(USActionComponent* NewActionComp);

	//start immediately when added to an action component
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	//Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const
	{
		return true;
	}
};
