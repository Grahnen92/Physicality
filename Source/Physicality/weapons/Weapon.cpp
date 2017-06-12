// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

#include "physicsFighter/PhysicsFighter.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	constructWeapon();
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	initWeapon();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APhysicsFighter* AWeapon::getHolder()
{
	return holder;
}

void AWeapon::OnWeaponHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	FVector hit_vel = weapon_head->GetBodyInstance()->GetUnrealWorldVelocity() + FVector::CrossProduct(weapon_head->GetBodyInstance()->GetUnrealWorldAngularVelocity(), Hit.Location - weapon_head->GetBodyInstance()->GetCOMPosition());

	if (holder)
	{
		if (hit_vel.Size() > 3500.f)
		{
			FLatentActionInfo actionInfo;
			actionInfo.CallbackTarget = this;
			APlayerController* tmp_controller = Cast<APlayerController>(holder->GetController());
			if (tmp_controller)
				tmp_controller->PlayDynamicForceFeedback(0.2f, 0.2f, false, true, false, true, EDynamicForceFeedbackAction::Start, actionInfo);
		}
	}


	if (hit_vel.Size() > 11000.f && NormalImpulse.Size() > 4000.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("NormalImpulse.Size() 11 %f"), NormalImpulse.Size());
		//UE_LOG(LogTemp, Warning, TEXT("hit_vel: %f"), hit_vel.Size());
		FMath::Min(weapon_wood_impact_audio->VolumeMultiplier = NormalImpulse.Size() / 40000.f, 0.6f);
		//weapon_wood_impact_audio->Deactivate();
		weapon_wood_impact_audio->Activate();

		if (holder)
		{
			FLatentActionInfo actionInfo;
			APlayerController* tmp_controller = Cast<APlayerController>(holder->GetController());
			if (tmp_controller)
				tmp_controller->PlayDynamicForceFeedback(0.6f, 0.6f, true, true, true, true, EDynamicForceFeedbackAction::Start, actionInfo);

				//tmp_controller->ClientPlayForceFeedback(weapon_impact, false, FName("SwordImpact"));
			//GetController()->CastToPlayerController()->DynamicForceFeedbacks
		}
	}

	if (holder)
	{
		if (NormalImpulse.Size() > 30000.f && !holder->isGuarding())
		{
			//holder->disableArm();
			FTimerHandle unused_handle;
			//GetWorldTimerManager().SetTimer(unused_handle, holder, &APhysicsFighter::enableArm, 0.7f, false);
		}

		//AWeapon* tmp_wep = Cast<AWeapon>(OtherActor);
		//if (tmp_wep)
		//{
		//	//if (tmp_wep->holder->isGuarding() && !holder->isGuarding())
		//	//{
		//	//	//holder->disableArm(0.7f);
		//	//}
		//}

		if (holder->isGuarding()) {
			//HitComp->AddImpulseAtLocation(NormalImpulse, Hit.Location);
			weapon_shaft->SetPhysicsLinearVelocity(FVector::ZeroVector);
			weapon_shaft->SetPhysicsAngularVelocity(FVector::ZeroVector);
		}
	}
}


void AWeapon::constructWeapon()
{
	Tags.Add("weapon");
	weapon_shaft = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponShaft"));
	weapon_shaft->ComponentTags.Add("shaft");
	//weapon->SetupAttachment(weapon_axis);
	RootComponent = weapon_shaft;

	weapon_shaft->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	weapon_shaft->SetCapsuleHalfHeight(75);
	weapon_shaft->SetCapsuleRadius(2.5f);
	

	weapon_shaft_vis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponShaftVis"));
	weapon_shaft_vis->SetupAttachment(weapon_shaft);

	weapon_handle_point = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponHandle"));
	weapon_handle_point->SetupAttachment(weapon_shaft);
	weapon_handle_point->SetRelativeLocation(FVector(0.f, 0.f, -75.f));
	weapon_handle_point->ComponentTags.Add("handle_point");

	weapon_head = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHead"));
	weapon_head->ComponentTags.Add("head");
	weapon_head->SetupAttachment(weapon_shaft);
	weapon_head->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	weapon_head->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	weapon_head->SetBoxExtent(FVector(10.f, 2.f, 75.f));
	//weapon_head->SetCapsuleHalfHeight(75.f);
	//weapon_head->SetCapsuleRadius(10.f);

	weapon_head_vis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponHeadVis"));
	weapon_head_vis->SetupAttachment(weapon_head);

	weapon_trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponHeadTrail"));
	weapon_trail->SetupAttachment(weapon_head_vis);

	weapon_wood_impact_audio = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponWoodImpactAudioTest"));
	weapon_wood_impact_audio->SetupAttachment(weapon_shaft);
	weapon_wood_impact_audio->VolumeMultiplier = 0.5f;

	//static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> SwordImpactObj(TEXT("/Game/JointCharacter/weapon/sword_impact"));

	//weapon_impact = SwordImpactObj.Object;
}

void AWeapon::initWeapon()
{
	/*weapon_shaft->SetSimulatePhysics(true);
	weapon_shaft->SetEnableGravity(false);
	weapon_shaft->SetPhysicsMaxAngularVelocity(5000.f);
	weapon_shaft->SetMassOverrideInKg(NAME_None, 2.f, true);

	weapon_head->SetSimulatePhysics(false);
	weapon_head->SetEnableGravity(false);
	weapon_head->SetPhysicsMaxAngularVelocity(5000.f);
	weapon_head->SetMassOverrideInKg(NAME_None, 10.f, true);*/

	
	weapon_head->OnComponentHit.AddDynamic(this, &AWeapon::OnWeaponHit);
}

UCapsuleComponent* AWeapon::getShaftComponent()
{
	return weapon_shaft;
}
UBoxComponent* AWeapon::getHeadComponent()
{
	return weapon_head;
}
USceneComponent* AWeapon::getAttachmentPoint()
{
	return weapon_handle_point;
}

void AWeapon::initGrabbed(APhysicsFighter* _holder)
{
	holder = _holder;
	weapon_trail->BeginTrails(FName("top_trail"), FName("bot_trail"), ETrailWidthMode::ETrailWidthMode_FromCentre, 1.0f);
}

void AWeapon::deInitGrabbed()
{
	holder = nullptr;
	//weapon_head->OnComponentHit.RemoveAll(this);
	weapon_trail->EndTrails();
}
