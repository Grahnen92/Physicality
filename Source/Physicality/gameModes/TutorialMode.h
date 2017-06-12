// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "proceduralGeometry/hex/HexInstancer.h"

#include "Runtime/Engine/Classes/Engine/TriggerBox.h"
#include "weapons/SwordLock.h"

#include "FightModeBase.h"

#include "GameFramework/GameModeBase.h"
#include "TutorialMode.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICALITY_API ATutorialMode : public AFightModeBase
{
	GENERATED_BODY()
	

public:
	ATutorialMode(const FObjectInitializer& ObjectInitializer);

	virtual void InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage) override;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void registerDeath(APlayerController* round_loser);

protected:
	virtual void BeginPlay() override;

	//UPROPERTY(Category = "InstancedMesh", BlueprintReadWrite, VisibleAnywhere)
	//UHexInstancerComponent* instancer;

	AHexInstancer* instancer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "asset")
	UClass* weapon_asset;

	UPROPERTY(VisibleAnywhere)
	UClass* horiz_lock;
	ASwordLock* horiz_lock_inst;

	UPROPERTY(VisibleAnywhere)
	UClass* vert_lock;
	ASwordLock* vert_lock_inst;

	UPROPERTY(VisibleAnywhere)
	UClass* hitting_dummy;

	UPROPERTY(VisibleAnywhere)
	UClass* guarding_dummy;

	UBoxComponent* killzBox;
	UFUNCTION()
	void killzBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	void eventOne(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void eventTwo(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void eventThree(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void eventFour(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void eventFive(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void eventSex(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void eventSeven(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void delayedPlayerSpawn(APlayerController* pc);
private:

	class ATriggeredWeaponSpawner* deferred_spawner;
	void timedWeaponSpawn();

	APhysicsFighter* practice_bot;
	void delayedDummySpawn();



	float hex_radius = 100.f;
	float hex_height;

	float initial_pos = -4000.f;

	// 0 == initial platform
	// 1 == jump tutorial starting platform
	// 2 == after jump completed
	// 3 == after dash jump completed
	// 4 == weapon grab platform
	// 5 == final platform with dummies
	TArray<UHexGenComponent*> platforms;
	// 0 == bridge to jump tutorial
	// 1 == bridge to weapon grab tutorial
	// 2 == bridge to first lock
	// 3 == bridge to second lock
	// 4 == bridge to final platform
	TArray<UHexGenComponent*> bridges;
	// 0 == initiates jump tutorial
	// 1 == initiates jump dash tutorial
	// 2 == initiates grab tutorial
	// 3 == initiates first lock tutorial
	// 4 == initiates second lock tutorial
	// 5 == initiates final dummy tutorial
	TArray<UBoxComponent*> event_colliders;

	
	
};
