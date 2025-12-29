// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpAmmo.h"
#include "Player/ShooterCharacter.h"

// Sets default values
APowerUpAmmo::APowerUpAmmo()
{
	PrimaryActorTick.bCanEverTick = true; 
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh;
}

// Called when the game starts or when spawned
void APowerUpAmmo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APowerUpAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APowerUpAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Munición pillada"));
		Player->AddAmmo(AmmoAmount);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}