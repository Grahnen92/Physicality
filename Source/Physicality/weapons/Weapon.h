// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class APhysicsFighter;

UCLASS()
class PHYSICALITY_API AWeapon : public AActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(Category = "WeaponPart", VisibleAnywhere)
	UCapsuleComponent* weapon_shaft;
	UPROPERTY(Category = "WeaponPart", VisibleAnywhere)
	UStaticMeshComponent* weapon_shaft_vis;
	FBodyInstance* weapon_shaft_bi;

	UPROPERTY(Category = "WeaponPart", VisibleAnywhere)
	USceneComponent* weapon_handle_point;

	UPROPERTY(Category = "WeaponPart", VisibleAnywhere)
	UBoxComponent* weapon_head;
	UPROPERTY(Category = "WeaponPart", VisibleAnywhere)
	UStaticMeshComponent* weapon_head_vis;
	FBodyInstance* weapon_head_bi;

	UPROPERTY(Category = "WeaponPart", VisibleAnywhere)
	UParticleSystemComponent* weapon_trail;

	UPROPERTY(Category = "WeaponAudio", VisibleAnywhere)
	UAudioComponent* weapon_wood_impact_audio;


	UForceFeedbackEffect* weapon_impact;

	bool held = false;
	APhysicsFighter* holder;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APhysicsFighter* getHolder();

	UFUNCTION()
		void OnWeaponHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UCapsuleComponent* getShaftComponent();
	UBoxComponent* getHeadComponent();
	USceneComponent* getAttachmentPoint();

	void initGrabbed(APhysicsFighter* _holder);
	void deInitGrabbed();

private:

	void constructWeapon();
	void initWeapon();

};
