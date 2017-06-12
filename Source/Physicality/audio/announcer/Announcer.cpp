// Fill out your copyright notice in the Description page of Project Settings.

#include "Announcer.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
AAnnouncer::AAnnouncer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	round1 = CreateDefaultSubobject<UAudioComponent>(TEXT("RoundOneAnnouncement"));
	rounds.Add(round1);
	round2 = CreateDefaultSubobject<UAudioComponent>(TEXT("RoundTwoAnnouncement"));
	rounds.Add(round2);
	round3 = CreateDefaultSubobject<UAudioComponent>(TEXT("RoundThreeAnnouncement"));
	rounds.Add(round3);
	round4 = CreateDefaultSubobject<UAudioComponent>(TEXT("RoundFourAnnouncement"));
	rounds.Add(round4);
	round5 = CreateDefaultSubobject<UAudioComponent>(TEXT("RoundFiveAnnouncement"));
	rounds.Add(round5);

	fight = CreateDefaultSubobject<UAudioComponent>(TEXT("FightAnnouncement"));



}

// Called when the game starts or when spawned
void AAnnouncer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnnouncer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnnouncer::announceRound(int round)
{
	rounds[round]->Activate();
}
void AAnnouncer::announceFight()
{
	fight->Activate();
}

