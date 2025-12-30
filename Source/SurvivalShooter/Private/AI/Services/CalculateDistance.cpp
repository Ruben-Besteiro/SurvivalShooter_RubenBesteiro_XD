// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/CalculateDistance.h"
#include "AIController.h"
#include "AI/BaseEnemy.h"
#include "AI/EnemyController.h"
#include "AI/EnemyStates.h"
#include "BehaviorTree/BlackboardComponent.h"

UCalculateDistance::UCalculateDistance()
{
	bNotifyTick = true;
}

void UCalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ABaseEnemy* MyPawn = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	AEnemyController* MyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	
	AActor * MyTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (MyTarget)
	{
		FVector TargetLocation = MyTarget->GetActorLocation();
		FVector MyLocation = MyPawn->GetActorLocation();
		FVector TargetDistance = TargetLocation - MyLocation;		// Distancia entre el objetivo y yo
		float DistanceToTarget = TargetDistance.Size();
		if (DistanceToTarget > MinDistanceForStrafing)		// Si la distancia es superior a la mínima en vez de hacer strafe se pone a correr
		{
			MyPawn->SetIsStrafing(false);
			MyController->ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Chase));
		}
	}
}
