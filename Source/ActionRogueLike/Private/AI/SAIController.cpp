// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ASAIController::BeginPlay()
{
	//call super to not lose any of the logic
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		//get blackboard component and set that move to location
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		//move towards the character
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
}


