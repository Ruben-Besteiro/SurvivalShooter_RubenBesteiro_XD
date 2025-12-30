// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterController.h"

#include "AIController.h"
#include "EnhancedInputSubsystems.h"
#include "AI/BaseEnemy.h"
#include "AI/SecondEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Components/AudioComponent.h"

// Cuando muere el jugador, hacemos que los enemigos salgan volando
void AShooterController::PawnDead()
{
	MusicComponent->Stop();
	APawn* MyPawn = GetPawn();
	if (MyPawn)
	{
		MyPawn->SetLifeSpan(3.0f); // destruye el jugador tras 3 seg
		UnPossess();
	}

	UWorld* World = GetWorld();
	if (!World) return;
	
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(World, ABaseEnemy::StaticClass(), Enemies);

	TArray<AActor*> SecondFound;
	UGameplayStatics::GetAllActorsOfClass(World, ASecondEnemy::StaticClass(), SecondFound);
	Enemies.Append(SecondFound);

	for (AActor* Actor : Enemies)
	{
		if (APawn* EnemyPawn = Cast<APawn>(Actor))
		{
			if (AAIController* AICon = Cast<AAIController>(EnemyPawn->GetController()))
			{
				if (UBrainComponent* Brain = AICon->GetBrainComponent())
				{
					Brain->StopLogic("PlayerDead");
				}
				AICon->UnPossess();
			}
			
			if (USkeletalMeshComponent* Mesh = EnemyPawn->FindComponentByClass<USkeletalMeshComponent>())
			{
				Mesh->SetSimulatePhysics(true);
				Mesh->SetCollisionProfileName("Ragdoll");

				// Vector random unitario multiplicado por fuerza
				FVector RandomImpulse = FVector(FMath::FRandRange(-1.f,1.f), 
												FMath::FRandRange(-1.f,1.f), 
												FMath::FRandRange(0.5f,1.f)).GetSafeNormal() * 1500.f;

				Mesh->AddImpulse(RandomImpulse, NAME_None, true);
			}

			EnemyPawn->SetLifeSpan(5.f);
		}
	}
	
	World->GetTimerManager().SetTimer(TimerHandle, this, &AShooterController::DestroyDelayed, 3.0f, false);
}



void AShooterController::BeginPlay()
{
	Super::BeginPlay();
	
	HealthBarWidget = CreateWidget(this, HealthBar);
	HealthBarWidget->AddToViewport();
	
	UEnhancedInputLocalPlayerSubsystem* Input =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	//Asociar mapa de controles en el controller
	Input->AddMappingContext(MappingContext, 0);

	//UGameplayStatics::PlaySound2D(GetWorld(), Music);
	MusicComponent = NewObject<UAudioComponent>(this);
	MusicComponent->bAutoActivate = false;
	MusicComponent->SetSound(Music);
	MusicComponent->SetVolumeMultiplier(0.3f);		// Esto es para que los SFX se oigan
	MusicComponent->RegisterComponent();

	// Esto es para reiniciar la música cuando se termine
	MusicComponent->OnAudioFinished.AddDynamic(this, &AShooterController::OnMusicFinished);
	
	MusicComponent->Play();
}

void AShooterController::OnMusicFinished()
{
	if (!IsValid(GetPawn())) return;
	if (Cast<AShooterCharacter>(GetPawn())->CurrentHealth > 0) MusicComponent->Play();		// En game over es mejor que se calle
}

void AShooterController::ActualizarMunicionDesdeAquiPorqueSiNoNoFunciona(int CurrentReserveAmmo, int CurrentChargerAmmo)
{
	if (!IsValid(GetPawn())) return;
	UpdateAmmo(CurrentReserveAmmo, CurrentChargerAmmo);
}

void AShooterController::ActualizarVidaDesdeAquiPorqueSiNoNoFunciona(float e)
{
	UpdateHealth(e);
}

void AShooterController::ActualizarKillsDesdeAquiPorqueSiNoNoFunciona()
{
	Kills++;
	UE_LOG(LogTemp, Log, TEXT("Matates al enemigo"));
	UpdateKills(Kills);
}

void AShooterController::DestroyDelayed()
{
	//Aviso al GameMode de que me reseteen.
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}
