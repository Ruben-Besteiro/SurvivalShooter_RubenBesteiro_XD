// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveController.h"

#include "PowerUpHealth2.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ShooterController.h"
#include "Blueprint/UserWidget.h"

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
	Player = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void AWaveController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerIsAlive && !IsInIntermission)
	{
		Counter += DeltaTime;
		Counter2 += DeltaTime;
		Counter3 += DeltaTime;
		Counter4 += DeltaTime;
		Counter5 += DeltaTime;

		int Rand = UKismetMathLibrary::RandomIntegerInRange(-1500, 5500);
		int Rand2 = UKismetMathLibrary::RandomIntegerInRange(-5500, 1500);

		// En la oleada 1, spawneamos enemigos y powerups más rápido según cuántas kills llevemos
		if (!Wave2)
		{
			if (Counter >= Interval)
			{
				Counter = 0;
				GetWorld()->SpawnActor<ABaseEnemy>(FirstEnemy, FVector(Rand, Rand2, 300), FRotator(0, 0, 0));
			}
			if (Counter2 >= Interval)
			{
				Counter2 = 0;
				GetWorld()->SpawnActor<ASecondEnemy>(SecondEnemy, FVector(Rand, Rand2, 300), FRotator(0, 0, 0));
			}

			if (Counter3 >= 9)
			{
				Counter3 = 0;
				GetWorld()->SpawnActor<APowerUpHealth2>(PowerUpHealth, FVector(Rand, Rand2, 100), FRotator(0, 0, 0));
			}

			if (Counter4 >= 9)
			{
				Counter4 = 0;
				GetWorld()->SpawnActor<APowerUpAmmo>(PowerUpAmmo, FVector(Rand, Rand2, 100), FRotator(0, 0, 0));
			}

			if (Counter5 >= 9)
			{
				Counter5 = 0;
				GetWorld()->SpawnActor<APowerUpBerserk>(PowerUpBerserk, FVector(Rand, Rand2, 100), FRotator(0, 0, 0));
			}
		}

		// Esto es cuando el jugador gana la primera oleada (ya no aparecen más enemigos)
		if (!Wave2 && Cast<AShooterController>(Player->Controller)->Kills >= 15)
		{
			IsInIntermission = true;
			UGameplayStatics::PlaySound2D(GetWorld(), VictoryRoyale);

			// Destruimos todos los enemigos y powerups
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundActors);
			for (AActor* Actor : FoundActors) Actor->Destroy();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASecondEnemy::StaticClass(), FoundActors);
			for (AActor* Actor : FoundActors) Actor->Destroy();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUp::StaticClass(), FoundActors);
			for (AActor* Actor : FoundActors) Actor->Destroy();

			GGWidget->AddToViewport();
			Player->CurrentHealth = Player->MaxHealth;
			Cast<AShooterController>(Player->Controller)->UpdateHealthPublic(Player->CurrentHealth);
			if (Player->CurrentReserveAmmo < Player->MaxReserveAmmo) Player->CurrentReserveAmmo = Player->MaxReserveAmmo;
			Player->CurrentChargerAmmo = Player->MaxChargerAmmo;
			Cast<AShooterController>(Player->Controller)->UpdateAmmoPublic(Player->MaxReserveAmmo, Player->MaxChargerAmmo);

			// Dejamos un descansito
			FTimerHandle Wave2TimerHandle;
			GetWorldTimerManager().SetTimer(Wave2TimerHandle, [this]()
			{
				IsInIntermission = false;
				Wave2 = true;
			}, IntermissionSeconds, false);
		}

		// En la oleada 2, spawneamos 30 de cada enemigo y 30 de cada powerup instantáneamente, y siempre habrá esa cantidad en el mapa
		if (Wave2)
		{
			TArray<AActor*> FoundStuff;
			
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundStuff);
			int Missing = Wave2MaxPerType - FoundStuff.Num();
			for (int i = 0; i < Missing; i++)
			{
				// Hay que regenerar Rand y Rand2 para que no salgan todos los enemigos en el mismo sitio
				Rand = UKismetMathLibrary::RandomIntegerInRange(-1500, 5500);
				Rand2 = UKismetMathLibrary::RandomIntegerInRange(-5500, 1500);
				GetWorld()->SpawnActor<ABaseEnemy>(FirstEnemy,FVector(Rand, Rand2, 300),FRotator(0,0,0));
			}

			FoundStuff.Reset();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASecondEnemy::StaticClass(), FoundStuff);
			Missing = Wave2MaxPerType - FoundStuff.Num();
			for (int i = 0; i < Missing; i++)
			{
				Rand = UKismetMathLibrary::RandomIntegerInRange(-1500, 5500);
				Rand2 = UKismetMathLibrary::RandomIntegerInRange(-5500, 1500);
				GetWorld()->SpawnActor<ASecondEnemy>(SecondEnemy,FVector(Rand, Rand2, 300),FRotator(0,0,0));
			}

			FoundStuff.Reset();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUpHealth2::StaticClass(), FoundStuff);
			Missing = Wave2MaxPerType - FoundStuff.Num();
			for (int i = 0; i < Missing; i++)
			{
				Rand = UKismetMathLibrary::RandomIntegerInRange(-1500, 5500);
				Rand2 = UKismetMathLibrary::RandomIntegerInRange(-5500, 1500);
				GetWorld()->SpawnActor<APowerUpHealth2>(PowerUpHealth,FVector(Rand, Rand2, 100), FRotator(0,0,0));
			}
			
			FoundStuff.Reset();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUpAmmo::StaticClass(), FoundStuff);
			Missing = Wave2MaxPerType - FoundStuff.Num();
			for (int i = 0; i < Missing; i++)
			{
				Rand = UKismetMathLibrary::RandomIntegerInRange(-1500, 5500);
				Rand2 = UKismetMathLibrary::RandomIntegerInRange(-5500, 1500);
				GetWorld()->SpawnActor<APowerUpAmmo>(PowerUpAmmo,FVector(Rand, Rand2, 100),FRotator(0,0,0));
			}

			FoundStuff.Reset();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUpBerserk::StaticClass(), FoundStuff);
			Missing = Wave2MaxPerType - FoundStuff.Num();
			for (int i = 0; i < Missing; i++)
			{
				Rand = UKismetMathLibrary::RandomIntegerInRange(-1500, 5500);
				Rand2 = UKismetMathLibrary::RandomIntegerInRange(-5500, 1500);
				GetWorld()->SpawnActor<APowerUpBerserk>(PowerUpBerserk,FVector(Rand, Rand2, 100),FRotator(0,0,0));
			}
		}
	}
}

// Cuantos más enemigos matemos, más rápido irán apareciendo
void AWaveController::ReduceInterval()
{
	Interval -= IntervalReductionPerKill;
}

