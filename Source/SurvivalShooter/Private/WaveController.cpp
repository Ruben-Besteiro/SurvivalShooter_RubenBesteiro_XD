// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveController.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWaveController::AWaveController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaveController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Counter += DeltaTime;
	Counter2 += DeltaTime;

	int Rand = UKismetMathLibrary::RandomIntegerInRange(-1000, 1000);
	int Rand2 = UKismetMathLibrary::RandomIntegerInRange(-1000, 1000);

	if (Counter >= 3)
	{
		Counter = 0;
		GetWorld()->SpawnActor<ABaseEnemy>(FirstEnemy, FVector(Rand, Rand2, 500), FRotator(0, 0, 0));
	}
	if (Counter2 >= 3)
	{
		Counter2 = 0;
		GetWorld()->SpawnActor<ASecondEnemy>(SecondEnemy, FVector(Rand, Rand2, 500), FRotator(0, 0, 0));
	}
}

