// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SecondEnemy.h"

// Sets default values
ASecondEnemy::ASecondEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackPoint = CreateDefaultSubobject<USceneComponent>("AttackPoint");
	AttackPoint->SetupAttachment(RootComponent); //Pongo el empty como hijo del comp. raíz.
}

// Called when the game starts or when spawned
void ASecondEnemy::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Hola desde el enemigo"));
}

// Called every frame
void ASecondEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASecondEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

