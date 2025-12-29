// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpHealth.h"
#include "Player/ShooterCharacter.h"

// Sets default values
APowerUpHealth::APowerUpHealth()
{
	PrimaryActorTick.bCanEverTick = true; 
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh;
}

// Called when the game starts or when spawned
void APowerUpHealth::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APowerUpHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APowerUpHealth::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Cura pillada"));
		Player->Cure(HPAmount);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}