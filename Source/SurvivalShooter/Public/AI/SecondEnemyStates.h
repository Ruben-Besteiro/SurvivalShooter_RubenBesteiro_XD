// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class ESecondEnemyStates : uint8
{
	Wait   UMETA(DisplayName = "Wait"),
	Chase    UMETA(DisplayName = "Chase"),
	Attack	UMETA(DisplayName= "Attack"),
	Flee     UMETA(DisplayName = "Flee")
};
