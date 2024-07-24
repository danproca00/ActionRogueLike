// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "AIController.h"

//#include "SBTService_CheckAttackRange.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//call super to not override any base logic
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//check distance between AI pawn and TargetActor
	
	//passing it by reference
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.0f;

					bool bHasLOS = false;

					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}
					//to not think that if he has a wall, he can attack us, even though it s not possible
					//bool bHasLOS = MyController->LineOfSightTo(TargetActor);

					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
