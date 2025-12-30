// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseEnemy.h"
#include "AI/SecondEnemy.h"
#include "GameFramework/Actor.h"
#include "WaveController.generated.h"

UCLASS()
class SURVIVALSHOOTER_API AWaveController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseEnemy> FirstEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASecondEnemy> SecondEnemy;

	float Counter = 0;
	float Counter2 = -1.5f;
};
