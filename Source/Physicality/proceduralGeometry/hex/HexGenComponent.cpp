// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGenComponent.h"
#include "HexInstancer.h"

// Sets default values for this component's properties
UHexGenComponent::UHexGenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UHexGenComponent::UHexGenComponent(UInstancedStaticMeshComponent* _instance, float hex_radius, FVector position) : instance(_instance), hrad(hex_radius)
{
	SetWorldLocation(position);
}

// Called when the game starts
void UHexGenComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UHexGenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

