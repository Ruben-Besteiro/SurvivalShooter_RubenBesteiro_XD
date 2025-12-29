// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ShooterController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API AShooterController : public APlayerController
{
	GENERATED_BODY()
public:
	void PawnDead();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmo(int CurrentReserveAmmo, int CurrentChargerAmmo);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateKills(int K);

	int Kills = 0;
	
	void ActualizarMunicionDesdeFueraPorqueSiNoNoFunciona(int CurrentReserveAmmo, int CurrentChargerAmmo);
	void ActualizarVidaDesdeFueraPorqueSiNoNoFunciona(float e);
	void ActualizarKillsDesdeFueraPorqueSiNoNoFunciona();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MappingContext;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthBar; //Referencia a una clase en sí
	

	//BlueprintReadOnly: Para poder interpretar esta variable desde BPs.
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HealthBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* AmmoWidget;

	FTimerHandle TimerHandle;
	void DestroyDelayed();
};
