// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "ThirdPersonController.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICALITY_API AThirdPersonController : public APlayerController
{
	GENERATED_BODY()
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	AThirdPersonController();
	AThirdPersonController(const class FObjectInitializer& ObjectInitializer);

	void pauseGame();

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> pauseWidgetTemplate;

	UUserWidget* PauseMenu;

	void spawnPlayer();
	void despawnPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
	
	
};
