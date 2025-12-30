// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RotateSecondEnemy.generated.h"

/**
 * BTTask que hace rotar al enemigo aleatoriamente
 */
UCLASS()
class SURVIVALSHOOTER_API URotateSecondEnemy : public UBTTaskNode
{
	GENERATED_BODY()
    
public:
	URotateSecondEnemy();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FRotator TargetRotation;
	float RotationSpeed;

	// Para debug final
	void LogRotationDebug(AActor* Pawn);
};
