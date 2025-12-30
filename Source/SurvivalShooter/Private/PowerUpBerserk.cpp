// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpBerserk.h"
#include "Player/ShooterCharacter.h"

// Sets default values
APowerUpBerserk::APowerUpBerserk()
{
	PrimaryActorTick.bCanEverTick = true; 
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh;
}

// Called when the game starts or when spawned
void APowerUpBerserk::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APowerUpBerserk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void APowerUpBerserk::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor);
		Player->ApplyBerserkEffect(Seconds);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}
