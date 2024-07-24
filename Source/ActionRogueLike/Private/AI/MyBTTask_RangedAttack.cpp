// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework\Character.h"
#include "BehaviorTree\BlackboardComponent.h"

//spawn a projetile in the direction of the actor (target actor)
EBTNodeResult::Type UMyBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		//we do this bc we want to get the muzzle location
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}


	return EBTNodeResult::Failed;
}
