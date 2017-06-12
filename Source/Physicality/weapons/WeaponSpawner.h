// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Weapon.h"

#include "GameFramework/Actor.h"
#include "WeaponSpawner.generated.h"

UCLASS()
class PHYSICALITY_API AWeaponSpawner : public AActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AWeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> weapon_template;

	TArray<AWeapon*> weapons;

	void spawnWeapon();
	void despawnWeapon(int w_index);
	void despawnAllWeapons();
	
};
