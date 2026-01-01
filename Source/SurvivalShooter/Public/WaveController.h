// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpAmmo.h"
#include "PowerUpBerserk.h"
#include "PowerUpHealth2.h"
#include "AI/BaseEnemy.h"
#include "AI/SecondEnemy.h"
#include "GameFramework/Actor.h"
#include "Player/ShooterCharacter.h"
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
	void ReduceInterval();

	bool PlayerIsAlive = true;

private:
	// Lo que quiero incluir es el controller, pero si lo hiciese se crearía una dependencia circular
	UPROPERTY(EditAnywhere)
	AShooterCharacter* Player;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseEnemy> FirstEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASecondEnemy> SecondEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpHealth2> PowerUpHealth;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpAmmo> PowerUpAmmo;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpBerserk> PowerUpBerserk;

	UPROPERTY(EditAnywhere)
	USoundBase* VictoryRoyale;

	// Contadores para spawnear enemigos
	float Counter = 0;
	float Counter2 = -2;

	// Contadores para spawnear powerups
	float Counter3 = 0;
	float Counter4 = -3;
	float Counter5 = -6;

	float Interval = 4;
	float IntervalReductionPerKill = 0.2f;

	bool Wave2 = false;
	bool IsInIntermission = false;

	int Wave2MaxPerType = 50;
};
