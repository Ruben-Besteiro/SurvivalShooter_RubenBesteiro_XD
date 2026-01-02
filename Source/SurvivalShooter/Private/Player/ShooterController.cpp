// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterController.h"

#include "AIController.h"
#include "EnhancedInputSubsystems.h"
#include "AI/BaseEnemy.h"
#include "AI/SecondEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"

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

void AShooterController::UpdateAmmoPublic(int CurrentReserveAmmo, int CurrentChargerAmmo)
{
	if (!IsValid(GetPawn()) || IsDead) return;
	UpdateAmmo(CurrentReserveAmmo, CurrentChargerAmmo);
}

void AShooterController::UpdateHealthPublic(float e)
{
	if (!IsValid(GetPawn()) || IsDead) return;
	UpdateHealth(e);
}

void AShooterController::UpdateKillsPublic()
{
	if (!IsValid(GetPawn()) || IsDead) return;
	Kills++;
	UpdateKills(Kills);
}

// Cuando muere el jugador, hacemos que los enemigos salgan volando
void AShooterController::PawnDead()
{
	DeadWidget->AddToViewport();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	
	MusicComponent->Stop();
	
	APawn* MyPawn = GetPawn();
	if (MyPawn)
	{
		// Por la razón que sea no me deja asignar el WaveController desde el blueprint, así que lo busco desde el código
		if (!WaveController)
		{
			for (TActorIterator<AWaveController> It(GetWorld()); It;)
			{
				WaveController = *It;
				break;
			}
		}
		WaveController->PlayerIsAlive = false;
		UnPossess();
	}
	
	UWorld* World = GetWorld();
	
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(World, ABaseEnemy::StaticClass(), Enemies);

	TArray<AActor*> SecondEnemies;
	UGameplayStatics::GetAllActorsOfClass(World, ASecondEnemy::StaticClass(), SecondEnemies);
	Enemies.Append(SecondEnemies);

	for (AActor* Actor : Enemies)
	{
		if (APawn* EnemyPawn = Cast<APawn>(Actor))
		{
			if (AAIController* AICon = Cast<AAIController>(EnemyPawn->GetController()))
			{
				if (UBrainComponent* Brain = AICon->GetBrainComponent())
				{
					Brain->StopLogic("C Murió XD");
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
												FMath::FRandRange(0.5f,1.f)).GetSafeNormal() * 1500;

				Mesh->AddImpulse(RandomImpulse, NAME_None, true);
			}

			EnemyPawn->SetLifeSpan(5.f);
		}
	}
}
