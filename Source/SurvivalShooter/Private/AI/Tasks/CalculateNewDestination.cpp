// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/CalculateNewDestination.h"

#include "AIController.h"
#include "AI/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCalculateNewDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//Tengo acceso tanto al controlador del enemigo, como al enemigo en sí.
	AAIController* MyController = OwnerComp.GetAIOwner();
	APawn* EnemyPawn = MyController->GetPawn();
	
	if (auto ShooterEnemy = Cast<ABaseEnemy>(EnemyPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			"CurrentDestination", ShooterEnemy->GetNextPatrolPoint());
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
	
}
