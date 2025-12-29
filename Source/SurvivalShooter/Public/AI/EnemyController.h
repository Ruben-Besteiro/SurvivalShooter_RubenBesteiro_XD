// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	void ChangeState(uint8 State);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;


	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;

	UFUNCTION()
	void OnSensed(AActor* Actor, FAIStimulus Stimulus);

};
