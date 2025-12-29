// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MyBTService.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API UMyBTService : public UBTService
{
	GENERATED_BODY()
public:
	UMyBTService();

	// Se ejecuta en el primer frame en el que el servicio se activa.
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float NewSpeed = 999999.0f;
};
