// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


//to declare the classes bc the compiler will actually not know where to look
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;


UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	//let s us assign a class
	/*UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;*/

	//attack animation
	/*UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;*/

	/*UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;*/


	/*FTimerHandle TimerHandle_PrimaryAttack; 
	FTimerHandle TimerHandle_BlackholeAttack;
	FTimerHandle TimerHandle_Dash;*/
	

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	//expose the components
	UPROPERTY(VisibleAnywhere)
	//third person component
	USpringArmComponent* SpringArmComp;

	//expose the components
	UPROPERTY(VisibleAnywhere)
	//Add the components to the camera for the character
	UCameraComponent* CameraComp;

	//add the component
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//declare the MoveForward function
	void MoveForward(float Value /*0 to 1 and 0 to -1*/);

	//declare the MoveRight function
	void MoveRight(float Value /*0 to 1 and 0 to -1*/);

	void SprintStart();

	void SprintStop();

	//bc it is an action, it doesn t need any parameters
	void PrimaryAttack();
	
	void PrimaryInteract();

	void BlackHoleAttack();

	void Dash();

	// Re-use spawn logic between attacks
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	//bind OnHealthChaned
	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	

	//ASCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
