// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpBerserk.h"
#include "Camera/CameraComponent.h"
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
		PlayerCamera = Player->PlayerCamera;
		
		UE_LOG(LogTemp, Warning, TEXT("Berserk pillado"));

		// Crear ajustes de postprocesado para simular como si el jugador estuviese drogado
		FPostProcessSettings& PPS = PlayerCamera->PostProcessSettings;

		PPS.bOverride_ColorGain = true;
		PPS.ColorGain = FVector4(0, 999, 0,1);

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}

	FTimerHandle BerserkTimer;
	GetWorldTimerManager().SetTimer(BerserkTimer,this,&APowerUpBerserk::BackToNormal,Seconds,false);
}

void APowerUpBerserk::BackToNormal()
{
	FPostProcessSettings& PPS = PlayerCamera->PostProcessSettings;
	PPS.bOverride_ColorGain = false;
	UE_LOG(LogTemp, Warning, TEXT("Nope"));
}