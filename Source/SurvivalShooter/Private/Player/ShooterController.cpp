// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShooterController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"

void AShooterController::PawnDead()
{
	GetPawn()->SetLifeSpan(3.0f);
	UnPossess(); //Desvinculo el pawn del controlador. No hay inputs.
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterController::DestroyDelayed, 3.0f, false);
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
}

void AShooterController::ActualizarMunicionDesdeFueraPorqueSiNoNoFunciona(int CurrentReserveAmmo, int CurrentChargerAmmo)
{
	UpdateAmmo(CurrentReserveAmmo, CurrentChargerAmmo);
}

void AShooterController::ActualizarVidaDesdeFueraPorqueSiNoNoFunciona(float e)
{
	UpdateHealth(e);
}

void AShooterController::ActualizarKillsDesdeFueraPorqueSiNoNoFunciona()
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
