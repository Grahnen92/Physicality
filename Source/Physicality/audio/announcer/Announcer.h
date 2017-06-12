// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Runtime/Engine/Classes/Components/AudioComponent.h"

#include "Announcer.generated.h"

UCLASS()
class PHYSICALITY_API AAnnouncer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnnouncer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* root;

	UPROPERTY(Category = "announcements", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* round1;
	UPROPERTY(Category = "announcements", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* round2;
	UPROPERTY(Category = "announcements", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* round3;
	UPROPERTY(Category = "announcements", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* round4;
	UPROPERTY(Category = "announcements", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* round5;

	UPROPERTY(Category = "announcements", VisibleAnywhere, BlueprintReadWrite)
	TArray<UAudioComponent*> rounds;
	UPROPERTY(Category = "fight_announcements", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* fight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void announceRound(int round);
	void announceFight();
	
};
