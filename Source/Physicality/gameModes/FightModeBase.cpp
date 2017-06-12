// Fill out your copyright notice in the Description page of Project Settings.

#include "FightModeBase.h"

#include "EngineUtils.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "GameFramework/GameStateBase.h"
//#include "TextRenderActor.h"

#include "InitPawn.h"
#include "physicsFighter/PhysicsFighter.h"
#include "playerControllers/ThirdPersonController.h"
#include "weapons/WeaponSpawner.h"

#include <sstream>
#include <string>


AFightModeBase::AFightModeBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UClass> char_obj(TEXT("Class'/Game/physicsFighter/BPs/PhysicsFighterBP.PhysicsFighterBP_c'"));
	if (char_obj.Succeeded())
		fight_char = char_obj.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find character asset"));


	initPlayerMats();
}

void AFightModeBase::InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bPauseable = true;
	//PlayerControllerClass = AThirdPersonController::StaticClass();
	//DefaultPawnClass = AInitPawn::StaticClass();
}

void AFightModeBase::BeginPlay()
{
	Super::BeginPlay();

	while (GameState->PlayerArray.Num() < 1)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
	}

	for (TActorIterator<AWeaponSpawner> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		weapon_spawns.Add(*ActorItr);
	}
}

AActor* AFightModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	int tmp_player_index;
	GameState->PlayerArray.Find(Player->PlayerState, tmp_player_index);


	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (std::stoi(*ActorItr->PlayerStartTag.ToString()) == tmp_player_index)
		{
			Player->StartSpot = *ActorItr;
			return *ActorItr;
		}
	}


	UE_LOG(LogTemp, Warning, TEXT("No spawnpoint found"));
	return nullptr;
}

void AFightModeBase::registerDeath(APlayerController* round_loser)
{

}

void AFightModeBase::spawnPlayer(APlayerController* pc)
{
	APawn* currentPawn = pc->GetPawn();

	if (currentPawn)
	{
		currentPawn->Destroy();
	}

	FActorSpawnParameters SpawnInfo;
	APhysicsFighter* new_char = GetWorld()->SpawnActor<APhysicsFighter>(fight_char, pc->StartSpot->GetActorTransform(), SpawnInfo);
	if (new_char)
	{
		if (GameState->PlayerArray.Num() < 2)
			new_char->setFOV(90);
		else
			new_char->setFOV(50);

		int player_id;
		GameState->PlayerArray.Find(pc->PlayerState, player_id);
		new_char->setPlayerSpecificMaterial(player_mats[player_id]);

		pc->Possess(new_char);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("failed to spawn character"));
	}
}

void AFightModeBase::despawnPlayer(APlayerController* pc)
{
	//UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
	APawn* currentPawn = pc->GetPawn();

	if (currentPawn)
	{
		currentPawn->Destroy();
	}
	FActorSpawnParameters SpawnInfo;

	AInitPawn* new_char = GetWorld()->SpawnActor<AInitPawn>(AInitPawn::StaticClass(), pc->StartSpot->GetActorTransform(), SpawnInfo);
	if (new_char)
	{
		if (GameState->PlayerArray.Num() < 2)
			new_char->setFOV(90);
		else
			new_char->setFOV(50);;
		pc->Possess(new_char);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("failed to spawn initpawn"));
	}
}

void  AFightModeBase::initPlayerMats()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> blue_mat(TEXT("/Game/physicsFighter/materials/blue_player_mat.blue_player_mat"));
	if (blue_mat.Succeeded())
		player_mats.Add(blue_mat.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find player material asset"));

	static ConstructorHelpers::FObjectFinder<UMaterial> orange_mat(TEXT("/Game/physicsFighter/materials/orange_player_mat.orange_player_mat"));
	if (orange_mat.Succeeded())
		player_mats.Add(orange_mat.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find player material asset"));

	static ConstructorHelpers::FObjectFinder<UMaterial> green_mat(TEXT("/Game/physicsFighter/materials/green_player_mat.green_player_mat"));
	if (green_mat.Succeeded())
		player_mats.Add(green_mat.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find player material asset"));

	static ConstructorHelpers::FObjectFinder<UMaterial> purple_mat(TEXT("/Game/physicsFighter/materials/purple_player_mat.purple_player_mat"));
	if (purple_mat.Succeeded())
		player_mats.Add(purple_mat.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find player material asset"));
}

