// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsFighter.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "gameModes/FightModeBase.h"

// Sets default values
APhysicsFighter::APhysicsFighter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	constructBody();
	constructWeapon();
	constructCamera();
}
APhysicsFighter::~APhysicsFighter()
{
	if (alr) {
		delete alr->next;
		delete alr;
	}
	if(upbr.next)
		delete upbr.next;
	if (rlr.next)
		delete rlr.next;
	if (llr.next)
		delete llr.next;
}

void APhysicsFighter::constructBody()
{
	skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	skeleton->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  sm_object(TEXT("/Game/PhysicsFighter/skeletalMeshes/featherBotNoArm"));
	skeleton->SetSkeletalMesh(sm_object.Object);
	//body->SetPhysicsAsset(jointSKeletalMesh.Object->PhysicsAsset);
	//skeleton->SetSimulatePhysics(true);

	dash_trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BodyTrail"));
	dash_trail->SetupAttachment(skeleton, FName("Spine02"));

	lthruster = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftThruster"));
	lthruster->SetupAttachment(skeleton, FName("lFoot"));

	rthruster = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RighThruster"));
	rthruster->SetupAttachment(skeleton, FName("rFoot"));

}
void APhysicsFighter::constructWeapon()
{
	// Weapon settings
	grip_axis = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponAxis"));
	grip_axis->SetupAttachment(skeleton, FName("Spine02"));
	grip_axis->SetRelativeLocation(FVector(0.f, 0.f, 20));
	grip_axis->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	grip_axis->SetSphereRadius(30.f);

	grip_axis_vis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponAxisVis"));
	grip_axis_vis->SetupAttachment(grip_axis);
	grip_axis_attachment = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("GripAxisAttachment"));
	grip_axis_attachment->SetupAttachment(grip_axis);
	grip_axis_attachment->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	grip = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponHandle1"));
	grip->SetupAttachment(grip_axis);
	grip->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	grip->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	grip->SetSphereRadius(20.f);

	grip_vis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponHandle1Vis"));
	grip_vis->SetupAttachment(grip);

	grip_attachment = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("GripAttachment"));
	grip_attachment->SetupAttachment(grip);
	grip_attachment->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	grip_indicator_decal = CreateDefaultSubobject<UDecalComponent>(TEXT("GripIndicatorDecal"));
	grip_indicator_decal->SetupAttachment(grip);
	grip_indicator_decal->bAbsoluteRotation = true;

	grip_indicator_beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GripIndicatorBeam"));
	grip_indicator_beam->SetupAttachment(grip);
	grip_indicator_beam->bAbsoluteRotation = true;

	object_attach_audio = CreateDefaultSubobject<UAudioComponent>(TEXT("GripAttachmentAudio"));
	object_attach_audio->SetupAttachment(grip);

	//grip_axis_attachment->SetConstrainedComponents(skeleton, FName("Spine02"), grip_axis, NAME_None);
	//grip_axis_attachment->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 4.0f);
	//grip_axis_attachment->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 4.0f);
	//grip_axis_attachment->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 4.0f);
	//grip_axis_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_axis_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_axis_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_axis_attachment->SetDisableCollision(true);

	//grip_attachment->SetConstrainedComponents(grip_axis, NAME_None, grip, NAME_None);
	//grip_attachment->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//grip_attachment->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//grip_attachment->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//grip_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_attachment->SetDisableCollision(true);

	/*wep_attachment->SetConstrainedComponents(grip, NAME_None, grip, NAME_None);
	wep_attachment->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	wep_attachment->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	wep_attachment->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	wep_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	wep_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	wep_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);
	wep_attachment->SetDisableCollision(true);*/
}
void APhysicsFighter::constructCamera()
{
	camera_axis = CreateDefaultSubobject<USceneComponent>(TEXT("CameraAxis"));
	camera_axis->SetupAttachment(RootComponent);

	camera_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	camera_spring_arm->SetupAttachment(camera_axis);
	camera_spring_arm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(0.f, 0.0f, 0.0f));
	camera_spring_arm->TargetArmLength = 400.f;
	camera_spring_arm->bEnableCameraLag = true;
	camera_spring_arm->CameraLagSpeed = 6.0f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	camera->SetupAttachment(camera_spring_arm, USpringArmComponent::SocketName);

	targeting_aura = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingAura"));
	targeting_aura->SetupAttachment(camera_axis);
	targeting_aura->SetSphereRadius(1500);

	targeting_point = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingPoint"));
	targeting_point->SetupAttachment(camera_axis);
	targeting_point->SetSphereRadius(5);
}

// Called when the game starts or when spawned
void APhysicsFighter::BeginPlay()
{
	Super::BeginPlay();
	initCamera();
	initBody();
	initWeapon();
	initCustomPhysics();
	initPIDs();
}

void APhysicsFighter::initCamera()
{

}
void APhysicsFighter::initBody()
{
	/*skeletonkele = body->SetPhysicsAsset(jointSKeletalMesh.Object->PhysicsAsset);
	skeletonkele->SetSimulatePhysics(true);*/
}
void APhysicsFighter::initWeapon()
{


	//grip_axis->SetSimulatePhysics(true);
	//grip_axis->SetEnableGravity(false);

	//grip->SetSimulatePhysics(true);
	//grip->SetEnableGravity(false);

	//grip->SetMassOverrideInKg(NAME_None, 15.f, true);
	//grip->SetPhysicsMaxAngularVelocity(5000.f);

	//grip_axis->SetMassOverrideInKg(NAME_None, 20.f, true);
	//grip_axis->SetPhysicsMaxAngularVelocity(5000.f);

	//grip_axis_attachment->SetConstrainedComponents(skeletonkele, FName("Spine02"), grip_axis, NAME_None);
	//grip_axis_attachment->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 4.0f);
	//grip_axis_attachment->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 4.0f);
	//grip_axis_attachment->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 4.0f);
	//grip_axis_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_axis_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_axis_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_axis_attachment->SetDisableCollision(true);

	//grip_attachment->SetConstrainedComponents(grip_axis, NAME_None, grip, NAME_None);
	//grip_attachment->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 0.0f);
	//grip_attachment->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 0.0f);
	//grip_attachment->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 0.0f);
	//grip_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//grip_attachment->SetDisableCollision(true);

	////wep_attachment->SetConstrainedComponents(grip_axis, NAME_None, grip, NAME_None);
	//wep_attachment->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//wep_attachment->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//wep_attachment->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//wep_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	//wep_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	//wep_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	//wep_attachment->SetDisableCollision(true);
}
void APhysicsFighter::initCustomPhysics()
{	

	torsoBI = skeleton->GetBodyInstance(FName("Spine02"));

	//pelvis
	alr = new FLimbNode;
	alr->bi = grip_axis->GetBodyInstance();
	alr->pid = &upper_arm_controller;
	//torso
	alr->next = new FLimbNode;
	alr->next->bi = grip->GetBodyInstance();
	alr->next->pid = &hand_controller;
	alr->next->next = nullptr;

	//pelvis
	upbr.bi = skeleton->GetBodyInstance(FName("Spine01"));
	upbr.pid = &pelvis_controller;
	//torso
	upbr.next = new FLimbNode;
	upbr.next->bi = skeleton->GetBodyInstance(FName("Spine02"));
	upbr.next->pid = &torso_controller;
	upbr.next->next = nullptr;

	//right thigh
	rlr.bi = skeleton->GetBodyInstance(FName("rThigh"));
	rlr.pid = &right_thigh_controller;
	//right shin
	rlr.next = new FLimbNode;
	rlr.next->bi = skeleton->GetBodyInstance(FName("rShin"));
	rlr.next->pid = &right_shin_controller;
	rlr.next->next = nullptr;

	//left thigh
	llr.bi = skeleton->GetBodyInstance(FName("lThigh"));
	llr.pid = &left_thigh_controller;
	//left shin
	llr.next = new FLimbNode;
	llr.next->bi = skeleton->GetBodyInstance(FName("lShin"));
	llr.next->pid = &left_shin_controller;
	llr.next->next = nullptr;

	gwep_states.up = FVector::ZeroVector;
	gwep_states.prev_up = FVector::ZeroVector;

	CalculateControlBody.BindUObject(this, &APhysicsFighter::ControlBody);
	OnCalculateCustomHoverPhysics.BindUObject(this, &APhysicsFighter::customHoverPhysics);

	CalculateControlArm.BindUObject(this, &APhysicsFighter::ControlArm);
	OnCalculateWeaponGrabControl.BindUObject(this, &APhysicsFighter::weaponGrabControl);

	skeleton->OnComponentHit.AddDynamic(this, &APhysicsFighter::OnBodyHit);
	targeting_aura->OnComponentBeginOverlap.AddDynamic(this, &APhysicsFighter::addPotentialTarget);
	targeting_aura->OnComponentEndOverlap.AddDynamic(this, &APhysicsFighter::removePotentialTarget);

}
void APhysicsFighter::initPIDs()
{
	//camera ------------------------------------------------
	axis_target_rot = FRotator(0.f, 150.f, 0.f);
	arm_target_rot = FRotator(-35.f, 0.f, 0.f);
	target_arm_length = 500.f;



	hand_solder = FVector::UpVector;

}

// Called every frame
void APhysicsFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cameraCalculations(DeltaTime);

	if (char_states.alive)
	{
		camera_axis->SetWorldLocation(torsoBI->GetCOMPosition() + FVector::UpVector*40.f);

		alr->bi->AddCustomPhysics(CalculateControlArm); //TODO: this function makes PhysicsCharacter despawn when build is packaged
		torsoBI->AddCustomPhysics(CalculateControlBody); //TODO: this function makes PhysicsCharacter despawn when build is packaged
		torsoBI->AddCustomPhysics(OnCalculateCustomHoverPhysics);
		
		if (char_states.can_move)
			movementCalculations(DeltaTime);
		
		if (!arm_states.holding_weapon)
			gripIndicatorCalculations(DeltaTime);
	}
}

void APhysicsFighter::cameraCalculations(float DeltaTime)
{
	if (char_states.fight_mode) {
		if (char_states.fm_state == 1)
		{
			//camera_spring_arm->SetRelativeRotation(FMath::Lerp<float>(arm_rot_before_fight , arm_target_rot, fight_t));
			camera_spring_arm->TargetArmLength = FMath::Lerp<float>(arm_length_before_fight, target_arm_length, char_states.f_t);
			char_states.f_t += DeltaTime / char_states.fm_engage_time;

			if (char_states.f_t > 1.0f)
			{
				char_states.fm_state = 2;
				char_states.f_t = 1.0f;

				//camera_spring_arm->SetRelativeRotation(arm_target_rot);
				camera_spring_arm->TargetArmLength = target_arm_length;

			}
		}

		if (locked_target && !locked_target->IsPendingKill())
		{
			controlCameraDirection(DeltaTime);
		}
		else
		{
			aquireTarget();
		}

	}
	else {
		if (char_states.fm_state == 3)
		{
			camera_spring_arm->TargetArmLength = FMath::Lerp<float>(arm_length_before_fight, target_arm_length, char_states.f_t);
			//camera_spring_arm->SetRelativeRotation(FMath::Lerp<float>(arm_rot_before_fight, arm_target_rot, fight_t));
			char_states.f_t -= DeltaTime / char_states.fm_state;
			if (char_states.f_t < 0.0f)
			{
				char_states.fm_state = 0;
				camera_spring_arm->TargetArmLength = arm_length_before_fight;
				//	camera_spring_arm->SetRelativeRotation(arm_rot_before_fight);
			}
		}


		FRotator axis_rotation = camera_axis->GetComponentRotation();
		axis_rotation.Yaw += RS.X*2.0f;
		camera_axis->SetWorldRotation(axis_rotation);

		FRotator arm_rotation = camera_spring_arm->GetComponentRotation();
		arm_rotation.Pitch = FMath::Clamp(arm_rotation.Pitch + RS.Y*2.0f, -80.0f, 80.0f);
		camera_spring_arm->SetWorldRotation(arm_rotation);

	}

}
void APhysicsFighter::setFOV(int _fov)
{
	camera->SetFieldOfView(_fov);
}


void APhysicsFighter::movementCalculations(float DeltaTime)
{

	//Planar movement speed -------------------------------------------------------------------------------------

	if (!LS.IsZero())
		target_direction = (camera_axis->GetForwardVector()*LS.X + camera_axis->GetRightVector()* LS.Y).GetSafeNormal();

	//FVector2D LS_norm = LS.GetSafeNormal();
	FVector curr_vel = torsoBI->GetUnrealWorldVelocity();
	FVector2D curr_vel2D = FVector2D(curr_vel.X, curr_vel.Y);

	FVector target_vel;
	target_vel = (camera_axis->GetForwardVector()*LS.X + camera_axis->GetRightVector()* LS.Y)*target_speed;
	FVector2D target_vel2D = FVector2D(target_vel.X, target_vel.Y);
	mvc.error = target_vel2D - curr_vel2D;

	mvc.integral = mvc.integral + mvc.error * DeltaTime;
	mvc.derivative = (mvc.error - mvc.prev_err) / DeltaTime;

	mvc.adjustment = mvc.P * mvc.error +
		mvc.I * mvc.integral +
		mvc.D * mvc.derivative;
	mvc.prev_err = mvc.error;


	FVector move_force = FVector(mvc.adjustment.X, mvc.adjustment.Y, 0.f)*torsoBI->GetBodyMass();
	torsoBI->AddForce(move_force);

	//dashing -------------------------------------------------------------------------------------

	if (dash_data.dashing) {
		if (dash_data.force_timer == 0.f) {//the factor of 100 is because unreal apparently applies forces in kg*cm*s^(-2)
			dash_data.force = (skeleton->GetMass()*dash_data.speed - skeleton->GetMass()*curr_vel2D.Size()) / dash_data.force_time;
		}
		if (dash_data.force_timer < dash_data.force_time) {
			torsoBI->AddForce(target_direction*dash_data.force*DeltaTime);
			dash_data.force_timer += DeltaTime;
		}
		else
		{

		}

		dash_data.cd_timer += DeltaTime;
		dash_trail->SetFloatParameter(FName("trailLifetime"), FMath::Lerp(1.0f, 0.0f, dash_data.cd_timer / dash_data.cd));

		if (dash_data.cd_timer > dash_data.cd) {
			dash_data.dashing = false;
			dash_data.force_timer = 0.0f;
			dash_data.cd_timer = 0.0f;
			dash_trail->EndTrails();
		}
	}

	//Jumping -------------------------------------------------------------------------------------

	if (jump_data.jumping) {
		if (jump_data.curr_jump_time == 0.f) {//the factor of 100 is because unreal apparently applies forces in kg*cm*s^(-2)
			jump_data.force = (((FMath::Sqrt(2.f*1000.f*jump_data.height) - torsoBI->GetUnrealWorldVelocity().Z) / jump_data.force_time) + 1000.f)*(skeleton->GetMass()) * 100.f;
		}

		torsoBI->AddForce(FVector::UpVector*jump_data.force*DeltaTime);

		jump_data.curr_jump_time += DeltaTime;
		if (jump_data.curr_jump_time > jump_data.force_time) {
			jump_data.jumping = false;
			jump_data.curr_jump_time = 0.0f;
		}
	}
}


void  APhysicsFighter::gripIndicatorCalculations(float DeltaTime)
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	FCollisionObjectQueryParams rv_objects;
	rv_objects.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	rv_objects.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	rv_objects.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);


	//Re-initialize hit info
	FHitResult rv_hit(ForceInit);

	FVector start = alr->next->state.pos;
	FVector end = start - FVector::UpVector*200.f;

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByObjectType(rv_hit,        //result
		start,    //start
		end, //end
		rv_objects, //collision channel
		RV_TraceParams);

	if (rv_hit.bBlockingHit)
	{
		grip_indicator_decal->SetVisibility(true);
		grip_indicator_decal->SetWorldLocation(rv_hit.Location);
		grip_indicator_beam->SetBeamTargetPoint(0, alr->next->state.pos - FVector::UpVector*rv_hit.Distance, 0);

	}
	else
	{
		grip_indicator_beam->SetBeamTargetPoint(0, alr->next->state.pos - FVector::UpVector*200.f, 0);
		grip_indicator_decal->SetVisibility(false);
	}

}


void APhysicsFighter::setCanMove(bool new_state)
{
	char_states.can_move = new_state;
}
void APhysicsFighter::setArmDisabled(bool new_state)
{
	arm_states.disabled = new_state;
}
void APhysicsFighter::setAlive(bool new_state)
{
	char_states.alive = new_state;
}
void APhysicsFighter::disableArm(float duration = 0.5f)
{
	arm_states.disabled = true;
	arm_states.disable_start = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	arm_states.disable_duration = duration;
}
void APhysicsFighter::enableArm()
{
	arm_states.disabled = false;
}
void APhysicsFighter::setInputDir(FVector _dir)
{
	RS_3D = _dir;
}

void APhysicsFighter::setPlayerSpecificMaterial(UMaterial* mat)
{
	skeleton->SetMaterial(3, mat);
}

bool APhysicsFighter::isAlive()
{
	return char_states.alive;
}

bool APhysicsFighter::isGuarding()
{
	return arm_states.guarding;
}


void APhysicsFighter::OnBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (char_states.mortal)
	{
		if (OtherActor != this)
		{

			AWeapon* other_weapon = Cast<AWeapon>(OtherActor);
			if (other_weapon && other_weapon != held_weapon)
			{
				if (NormalImpulse.Size() > char_states.impact_resistance)
				{


					release();

					//body->BreakConstraint(FVector::ZeroVector, FVector::ZeroVector, Hit.BoneName);
					FHitResult other_hit;
					Hit.GetReversedHit(other_hit);
					FConstraintInstance* tmp_const = skeleton->FindConstraintInstance(other_hit.BoneName);
					if (tmp_const)
					{
						//tmp_const->TermConstraint();
						skeleton->BreakConstraint(FVector::ZeroVector, FVector::ZeroVector, other_hit.BoneName);
					}


					char_states.alive = false;
					char_states.fight_mode = false;
					skeleton->OnComponentHit.RemoveAll(this);

					AFightModeBase* tmp_mode = Cast<AFightModeBase>(GetWorld()->GetAuthGameMode());
					if (tmp_mode)
					{
						if (this->GetNetOwningPlayer())
							tmp_mode->registerDeath(this->GetNetOwningPlayer()->PlayerController);
						else
							tmp_mode->registerDeath(nullptr);
					}
					//HitComp->DestroyComponent();
				}
			}

		}
	}
}


void APhysicsFighter::FellOutOfWorld(const class UDamageType& dmgType)
{
	/*Super::FellOutOfWorld(dmgType);
	AFightModeBase* tmp_mode = Cast<AFightModeBase>(GetWorld()->GetAuthGameMode());
	if (tmp_mode)
	{
		if (this->GetNetOwningPlayer())
			tmp_mode->registerDeath(this->GetNetOwningPlayer()->PlayerController);
		else
			tmp_mode->registerDeath(nullptr);
	}*/
}

//CAMERA CONTROL =======================================================================================================
void APhysicsFighter::controlCameraDirection(float DeltaTime)
{

	locked_target_dir = (locked_target->GetComponentLocation() - camera_axis->GetComponentLocation()).GetSafeNormal();
	locked_target_dir_xy = locked_target_dir; locked_target_dir_xy.Z = 0.f;
	locked_target_dir_xy = locked_target_dir_xy.GetSafeNormal();

	FVector cam_forward = camera_axis->GetForwardVector();
	FVector cam_up = camera_axis->GetUpVector();

	FVector cam_arm_forward = camera_spring_arm->GetForwardVector();

	cdc.error.X = 1.f - FVector::DotProduct(locked_target_dir_xy, cam_forward);
	if (FVector::DotProduct(FVector::CrossProduct(locked_target_dir_xy, cam_forward), cam_up) > 0.f)
		cdc.error.X = -cdc.error.X;

	cdc.error.Y = FVector::DotProduct(locked_target_dir, FVector::UpVector) - FVector::DotProduct(cam_arm_forward, FVector::UpVector);

	cdc.integral = cdc.integral + cdc.error * DeltaTime;
	cdc.derivative = (cdc.error - cdc.prev_err) / DeltaTime;

	cdc.adjustment = cdc.P * cdc.error +
		cdc.I * cdc.integral +
		cdc.D * cdc.derivative;
	cdc.prev_err = cdc.error;

	camera_axis->AddLocalRotation(FRotator(0.f, cdc.adjustment.X, 0.f));

	camera_spring_arm->AddRelativeRotation(FRotator(cdc.adjustment.Y, 0.f, 0.f));
}


void APhysicsFighter::addPotentialTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
		lock_on_targets.Add(OtherComp);
}
void APhysicsFighter::removePotentialTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
		lock_on_targets.Remove(OtherComp);

	if (OtherComp == locked_target)
	{
		locked_target = nullptr;
		aquireTarget();
	}
}


void APhysicsFighter::aquireTarget()
{
	if (lock_on_targets.Num() > 0)
	{
		FVector u;
		FVector v = camera->GetForwardVector();

		float perp_dist;
		float cangle;
		float a = 2000.f;
		float b = 0.1f;
		float mix;
		float min_mix;
		int min_index;

		float large_nr = 100000000.f;

		//top of queue;
		lock_on_targets[0];
		u = lock_on_targets[0]->GetComponentLocation() - camera->GetComponentLocation();
		cangle = FVector::DotProduct(u.GetSafeNormal(), v);
		if (cangle < 0) {
			min_mix = large_nr;
		}
		else {
			cangle = 1 - cangle;
			perp_dist = FVector::DotProduct(u, v);
			min_mix = a * cangle + b*perp_dist;
		}
		min_index = 0;

		//rest...
		for (int i = 1; i < lock_on_targets.Num(); i++) {
			lock_on_targets[i];
			u = lock_on_targets[i]->GetComponentLocation() - camera->GetComponentLocation();
			cangle = FVector::DotProduct(u.GetSafeNormal(), v);
			if (cangle < 0) {
				mix = large_nr;
				if (mix < min_mix) {
					min_index = i;
					min_mix = mix;
				}

			}
			else {
				cangle = 1 - cangle;
				perp_dist = FVector::DotProduct(u, v);
				mix = a * cangle + b*perp_dist;
				if (mix < min_mix) {
					min_index = i;
					min_mix = mix;
				}
			}
		}
		locked_target = lock_on_targets[min_index];
	}
	//locked_target = this->GetNetOwningPlayer()->PlayerController->StartSpot->GetRootComponent();
}


void APhysicsFighter::customHoverPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.MobilityType = EQueryMobilityType::Any;
	//Re-initialize hit info
	FHitResult rv_hit(ForceInit);

	FVector start = BodyInstance->GetCOMPosition();
	FVector end = start - FVector::UpVector*hhc.target*2.f;

	GetWorld()->SweepSingleByProfile(rv_hit, start, end, FQuat(), FName("WalkableOverlap"), FCollisionShape::MakeSphere(20.f), RV_TraceParams);

	if (rv_hit.bBlockingHit)
	{
		hhc.error = hhc.target - rv_hit.Distance;
		hhc.integral = hhc.integral + hhc.error * DeltaTime;
		hhc.derivative = (hhc.error - hhc.prev_err) / DeltaTime;

		hhc.adjustment = 
			hhc.P * hhc.error +
			hhc.I * hhc.integral +
			hhc.D * hhc.derivative;
		hhc.adjustment = FMath::Min(FMath::Max(0.0f, hhc.adjustment), hhc.max_adjustment) * skeleton->GetMass();

		hhc.prev_err = hhc.error;
		BodyInstance->AddForce(FVector::UpVector * hhc.adjustment, false, false);

	}
	else
	{
		hhc.prev_err = 0.0;
		hhc.integral = 0.0;
	}
}


void APhysicsFighter::ControlBody(float DeltaTime, FBodyInstance* BodyInstance)
{

	//upperbody
	updateLimbStates(&upbr);
	setPelvisTargets();
	ControlLimb(DeltaTime, &upbr);

	updateLimbStates(upbr.next);
	setTorsoTargets();
	ControlLimb(DeltaTime, upbr.next);

	//right leg
	updateLimbStates(&rlr);
	setRThighTargets();
	ControlLimb(DeltaTime, &rlr);

	updateLimbStates(rlr.next);
	setRShinTargets();
	ControlLimb(DeltaTime, rlr.next);
	//left leg
	updateLimbStates(&llr);
	setLThighTargets();
	ControlLimb(DeltaTime, &llr);

	updateLimbStates(llr.next);
	setLShinTargets();
	ControlLimb(DeltaTime, llr.next);
}


void APhysicsFighter::setTorsoTargets()
{
	FLimbTarget* torso_tar = &upbr.next->target;
	torso_tar->prev_dir = torso_tar->dir;
	torso_tar->dir = FVector::UpVector;
	torso_tar->prev_dir_xy = torso_tar->dir_xy;
	torso_tar->dir_xy = FVector::ZeroVector;

	torso_tar->twist_dir = (camera_axis->GetForwardVector() - FVector::DotProduct(camera_axis->GetForwardVector(), upbr.next->state.up)* upbr.next->state.up).GetSafeNormal();

}
void APhysicsFighter::setPelvisTargets()
{
	FLimbTarget* pelv_tar = &upbr.target;
	pelv_tar->prev_dir = pelv_tar->dir;
	pelv_tar->dir = FVector::UpVector;
	pelv_tar->prev_dir_xy = pelv_tar->dir_xy;
	pelv_tar->dir_xy = FVector::ZeroVector;


	pelv_tar->twist_dir = (camera_axis->GetForwardVector() - FVector::DotProduct(camera_axis->GetForwardVector(), upbr.state.up)* upbr.state.up).GetSafeNormal();

}

void APhysicsFighter::setRThighTargets()
{
	FLimbTarget* tmp_rThigh = &rlr.target;
	tmp_rThigh->prev_dir = tmp_rThigh->dir;
	tmp_rThigh->dir = -FVector::UpVector;
	tmp_rThigh->prev_dir_xy = tmp_rThigh->dir_xy;
	tmp_rThigh->dir_xy = FVector::ZeroVector;

	tmp_rThigh->twist_dir = (upbr.state.forward - FVector::DotProduct(upbr.state.forward, rlr.state.up)* rlr.state.up).GetSafeNormal();

}
void APhysicsFighter::setRShinTargets()
{

}

void APhysicsFighter::setLThighTargets()
{
	FLimbTarget* tmp_lThigh = &llr.target;
	tmp_lThigh->prev_dir = tmp_lThigh->dir;
	tmp_lThigh->dir = -FVector::UpVector;
	tmp_lThigh->prev_dir_xy = tmp_lThigh->dir_xy;
	tmp_lThigh->dir_xy = FVector::ZeroVector;

	tmp_lThigh->twist_dir = (upbr.state.forward - FVector::DotProduct(upbr.state.forward, llr.state.up)* llr.state.up).GetSafeNormal();
}
void APhysicsFighter::setLShinTargets()
{

}

void APhysicsFighter::updateLimbStates(FLimbNode* limb)
{
	limb->state.pos = limb->bi->GetUnrealWorldTransform().GetLocation();
	//x and z axis are swapped
	limb->state.forward = limb->bi->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);
	limb->state.right = limb->bi->GetUnrealWorldTransform().GetUnitAxis(EAxis::Y);
	limb->state.prev_up = limb->state.up;
	limb->state.up = limb->bi->GetUnrealWorldTransform().GetUnitAxis(EAxis::X);
}

void APhysicsFighter::updateArmLimbStates(FLimbNode* limb)
{
	limb->state.pos = limb->bi->GetUnrealWorldTransform().GetLocation();
	//x and z axis are swapped
	limb->state.forward = limb->bi->GetUnrealWorldTransform().GetUnitAxis(EAxis::X);
	limb->state.right = limb->bi->GetUnrealWorldTransform().GetUnitAxis(EAxis::Y);
	limb->state.prev_up = limb->state.up;
	limb->state.up = limb->bi->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);
}

void APhysicsFighter::ControlLimb(float DeltaTime, FLimbNode* limb)
{
	limb->pid->error.X = 1.f - FVector::DotProduct(limb->target.dir, limb->state.up);
	FVector rot_ref = FVector::CrossProduct(limb->state.up, limb->target.dir);
	rot_ref = rot_ref.GetSafeNormal();

	//calculate rotational speed around rotation point
	FVector vel_axis = (limb->state.up - limb->state.prev_up) / DeltaTime;
	limb->pid->error.Y = -vel_axis.Size();
	vel_axis = vel_axis.GetSafeNormal();

	//twist
	limb->pid->error.Z = 1.f - FVector::DotProduct(limb->target.twist_dir, limb->state.forward);
	if (FVector::DotProduct(-limb->state.up, FVector::CrossProduct(limb->state.forward, limb->target.twist_dir).GetSafeNormal()) > 0.f)
		limb->pid->error.Z = -limb->pid->error.Z;

	//Limb chain inertia around velocity and ref axis
	float jvi = 0.f, jri = 0.f, lui = 0.f;
	FMatrix limb_inertia;


	FLimbNode* limb_it = limb;
	calculateRelativeInertia(limb_it->bi, limb->state.pos, &limb_inertia);
	jvi += inertiaAboutAxis(limb_inertia, FVector::CrossProduct(limb->state.up, vel_axis).GetSafeNormal());
	jri += inertiaAboutAxis(limb_inertia, rot_ref);
	lui += limb->bi->GetBodyInertiaTensor().Z;
	limb_it = limb_it->next;
	while (limb_it)
	{
		calculateRelativeInertia(limb_it->bi, limb->state.pos, &limb_inertia);
		jvi += inertiaAboutAxis(limb_inertia, FVector::CrossProduct(limb->state.up, vel_axis).GetSafeNormal());
		jri += inertiaAboutAxis(limb_inertia, rot_ref);
		lui += inertiaAboutAxis(limb_inertia, limb->state.up);
		limb_it = limb_it->next;
	}

	//calculate PID
	limb->pid->error.X = limb->pid->error.X*(jri);
	limb->pid->error.Y = limb->pid->error.Y*(jvi);
	limb->pid->error.Z = limb->pid->error.Z*lui;


	limb->pid->integral = limb->pid->integral + limb->pid->error * DeltaTime;
	limb->pid->derivative = (limb->pid->error - limb->pid->prev_err) / DeltaTime;

	limb->pid->adjustment = limb->pid->P * limb->pid->error +
		limb->pid->I * limb->pid->integral +
		limb->pid->D * limb->pid->derivative;
	limb->pid->prev_err = limb->pid->error;

	rot_ref = FVector::CrossProduct(rot_ref, limb->state.up).GetSafeNormal();
	limb->bi->AddForceAtPosition(rot_ref*limb->pid->adjustment.X, limb->state.pos + limb->state.up, false);
	limb->bi->AddForceAtPosition(rot_ref*-limb->pid->adjustment.X, limb->state.pos - limb->state.up, false);

	limb->bi->AddForceAtPosition(vel_axis*limb->pid->adjustment.Y, limb->state.pos + limb->state.up, false);
	limb->bi->AddForceAtPosition(vel_axis*-limb->pid->adjustment.Y, limb->state.pos - limb->state.up, false);

	limb->bi->AddTorque(limb->state.up*limb->pid->adjustment.Z, false, false);

}


void APhysicsFighter::ControlArm(float DeltaTime, FBodyInstance* BodyInstance)
{
	updateArmLimbStates(alr);
	updateArmLimbStates(alr->next);

	hand_solder_dir = alr->next->state.up - hand_solder;
	if (hand_solder_dir.Size() > 0.5f)
		hand_solder = hand_solder + hand_solder_dir*(0.5f / hand_solder_dir.Size());

	hand_solder.Normalize();
	hand_solder_dir.Normalize();


	if (arm_states.disabled)
	{
		setDisabledStanceTargets();
		ControlLimb(DeltaTime, alr->next);

		if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - arm_states.disable_start > arm_states.disable_duration)
		{
			ControlLimb(DeltaTime, alr);
			if (1.f - FVector::DotProduct(alr->target.dir, alr->state.up) < 0.1f && FVector::Parallel(RS_3D, FVector::UpVector, 0.25f))
				arm_states.disabled = false;
		}
	}
	else if (arm_states.guard_locked)
	{
		setGuardingStanceTargets();
		ControlLimb(DeltaTime, alr);
		ControlLimb(DeltaTime, alr->next);
	}
	else if (arm_states.guarding)
	{
		setGuardingStanceTargets();
		ControlLimb(DeltaTime, alr);
		ControlLimb(DeltaTime, alr->next);
		/*if (!arm_states.guard_locked && (1.f - FVector::DotProduct(ajdc_targets[0].dir, ga_up)) < 0.01f && (1.f - FVector::DotProduct(ajdc_targets[1].dir, g_up)) < 0.01f && FMath::Abs(atc.error) < 0.01f)
		{
			lockGuard();
			char_states.fight_stance = 2;
		}*/
	}
	else
	{
		setNormalStanceTargets();
		ControlLimb(DeltaTime, alr);
		ControlLimb(DeltaTime, alr->next);

		//Grip_h
		FVector WPri = grip_attachment->ComponentToWorld.GetUnitAxis(EAxis::X);
		FVector WOrth = grip_attachment->ComponentToWorld.GetUnitAxis(EAxis::Y);

		FVector PriAxis1 = grip_axis->GetComponentTransform().InverseTransformVectorNoScale(WPri);
		FVector SecAxis1 = grip_axis->GetComponentTransform().InverseTransformVectorNoScale(WOrth);
		grip_attachment->SetConstraintReferenceOrientation(EConstraintFrame::Frame1, PriAxis1, SecAxis1);
	}

	if (arm_states.grabbing_weapon)
	{
		weaponGrabControl(DeltaTime, BodyInstance);
	}
}


void APhysicsFighter::setNormalStanceTargets()
{
	//twist
	if (FVector::Parallel(alr->state.up, upbr.state.up, 0.95f)) {
		alr->target.twist_dir = (upbr.state.forward - FVector::DotProduct(upbr.state.forward, alr->state.up)* alr->state.up).GetSafeNormal();
	}
	else {
		alr->target.twist_dir = (upbr.state.up - FVector::DotProduct(upbr.state.up, alr->state.up)* alr->state.up).GetSafeNormal();
	}

	FVector tmp_twist = hand_solder_dir.GetSafeNormal();
	tmp_twist = tmp_twist - FVector::DotProduct(tmp_twist, alr->next->state.up)*alr->next->state.up;
	tmp_twist.Normalize();

	float forward_error = 1.f - FVector::DotProduct(alr->next->state.forward, tmp_twist);
	if (1.f - FVector::DotProduct(-alr->next->state.forward, tmp_twist) < forward_error)
	{
		alr->next->target.twist_dir = tmp_twist;
		alr->next->state.forward = -alr->next->state.forward;
	}
	else
	{
		alr->next->target.twist_dir = tmp_twist;
	}
		
	DrawDebugLine(
		GetWorld(),
		alr->next->state.pos,
		alr->next->state.pos + alr->next->target.twist_dir*100.f,
		FColor(255, 0, 0),
		false, -1, 0,
		3.f
	);
	DrawDebugLine(
		GetWorld(),
		alr->next->state.pos,
		alr->next->state.pos + alr->next->state.forward*100.f,
		FColor(0, 0, 255),
		false, -1, 0,
		3.f
	);

	
	if (!char_states.fight_mode) {
		//Direction
		arm_states.extended = false;
		alr->next->target.dir = (-camera_axis->GetRightVector()*0.3f + camera_axis->GetForwardVector() + camera_axis->GetUpVector()*0.4f).GetSafeNormal();
		alr->target.dir = (camera_axis->GetRightVector() - camera_axis->GetUpVector()*0.3f + camera_axis->GetForwardVector()*0.4f).GetSafeNormal();
	}
	else {
		//direction
		alr->next->target.dir = RS_3D;
		alr->target.dir = RS_3D;
		
	}
}
void APhysicsFighter::setGuardingStanceTargets()
{

	//twist
	if (FVector::Parallel(alr->state.up, upbr.state.up, 0.95f)) {
		alr->target.twist_dir = (upbr.state.forward - FVector::DotProduct(upbr.state.forward, alr->state.up)* alr->state.up).GetSafeNormal();
	}
	else {
		alr->target.twist_dir = (upbr.state.up - FVector::DotProduct(upbr.state.up, alr->state.up)* alr->state.up).GetSafeNormal();
	}

	FVector tmp_twist = hand_solder_dir.GetSafeNormal();
	tmp_twist = tmp_twist - FVector::DotProduct(tmp_twist, alr->next->state.up)*alr->next->state.up;
	alr->next->target.twist_dir = tmp_twist.GetSafeNormal();

	//directions
	FVector camf = camera_axis->GetForwardVector();
	FVector camu = camera_axis->GetUpVector();
	if (!char_states.fight_mode) {
		
		float tmp_angle = FMath::Acos(FVector::DotProduct(camf, FVector::ForwardVector))*180.f / PI;
		if (FVector::CrossProduct(FVector::ForwardVector, camf).Z < 0)
			tmp_angle = -tmp_angle;

		arm_states.extended = false;
		
		FVector upper_arm_dir(FMath::Cos(PI / 4.f), 0.f, FMath::Sin(PI / 4.f));
		upper_arm_dir.Normalize();
		alr->target.dir = upper_arm_dir.RotateAngleAxis(tmp_angle, FVector::UpVector);

		FVector hand_dir(FMath::Cos(-PI / 3.f), 0.f, FMath::Sin(-PI / 3.f));
		hand_dir.Normalize();
		alr->next->target.dir = hand_dir.RotateAngleAxis(tmp_angle, FVector::UpVector);
	}
	else {
		arm_states.extended = false;

		FVector tmp_xy = RS_3D - FVector::DotProduct(RS_3D, camu)*camu;

		if (tmp_xy.IsNearlyZero(0.001f)){
			tmp_xy = alr->target.prev_dir_xy;
		}
		else{
			tmp_xy.Normalize();
			alr->target.prev_dir_xy = tmp_xy;
		}
		float tmp_angle = FMath::Acos(FVector::DotProduct(tmp_xy, FVector::ForwardVector))*180.f / PI;
		if (FVector::CrossProduct(FVector::ForwardVector, tmp_xy).Z < 0)
			tmp_angle = -tmp_angle;

		FVector guard_pos(FMath::Cos(PI / 2.8f), 0.f, FMath::Sin(PI / 2.8f));
		guard_pos.Normalize();
		alr->target.dir = guard_pos.RotateAngleAxis(tmp_angle, FVector::UpVector);

		FVector arm_xy = alr->state.up;
		arm_xy.Z = 0.f; arm_xy.Normalize();
		float arm_angle = FMath::Acos(FVector::DotProduct(arm_xy, FVector::ForwardVector))*180.f / PI;
		if (FVector::CrossProduct(FVector::ForwardVector, arm_xy).Z < 0)
			arm_angle = -arm_angle;

		FVector guard_dir(FMath::Cos(-PI / 3.f), 0.f, FMath::Sin(-PI / 3.f));
		guard_dir.Normalize();
		alr->next->target.dir = guard_dir.RotateAngleAxis(arm_angle, FVector::UpVector);
	}
}

void APhysicsFighter::setDisabledStanceTargets()
{
	arm_states.extended = false;
	alr->target.dir = FVector::UpVector;
	alr->next->target.dir = alr->state.up;
}

void APhysicsFighter::weaponGrabControl(float DeltaTime, FBodyInstance* BodyInstance)
{
	FBodyInstance* tmp_shaft = held_weapon->getShaftComponent()->GetBodyInstance();
	gwep_states.prev_up = gwep_states.up;
	gwep_states.up = tmp_shaft->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);
	gwep_states.pos = tmp_shaft->GetUnrealWorldTransform().GetLocation();
	USceneComponent* tmp_attachment = held_weapon->getAttachmentPoint();

	wgc.error = alr->next->state.pos - tmp_attachment->GetComponentLocation();

	//weapon position --------------------------------------------------------------
	wgc.integral = wgc.integral + wgc.error * DeltaTime;
	wgc.derivative = (wgc.error - wgc.prev_err) / DeltaTime;

	wgc.adjustment = wgc.P * wgc.error +
		wgc.I * wgc.integral +
		wgc.D * wgc.derivative;
	wgc.prev_err = wgc.error;

	float mass = tmp_shaft->GetBodyMass();
	//tmp_shaft->AddForceAtPosition(wgc.adjustment*(mass)/*+mass*980.f*FVector::UpVector*/, tmp_attachment->GetComponentLocation(), false);
	tmp_shaft->AddForce(wgc.adjustment*(mass), false);
	////weapon rotation --------------------------------------------------------------
	wgdc.error.X = 1.f - FVector::DotProduct(alr->next->state.up, gwep_states.up);
	UE_LOG(LogTemp, Warning, TEXT("wgdc.error.X  %f"), wgdc.error.X);
	FVector rot_ref = FVector::CrossProduct(gwep_states.up, alr->next->state.up);
	rot_ref = rot_ref.GetSafeNormal();

	if (wgc.error.Size() < 10.f && FMath::Abs(wgdc.error.X) < 0.1f)
	{
		held_weapon->getShaftComponent()->SetAngularDamping(0.0f);
		held_weapon->getShaftComponent()->SetLinearDamping(0.01f);
		attachWeapon(held_weapon);

		arm_states.grabbing_weapon = false;
	}

	FVector vel_axis = (gwep_states.up - gwep_states.prev_up) / DeltaTime;
	wgdc.error.Y = -vel_axis.Size();
	vel_axis = vel_axis.GetSafeNormal();

	FMatrix wep_inertia;
	calculateRelativeInertia(tmp_shaft, gwep_states.pos, &wep_inertia);
	float wi_ref_i = inertiaAboutAxis(wep_inertia, rot_ref);
	float wi_va_i = inertiaAboutAxis(wep_inertia, FVector::CrossProduct(gwep_states.up, vel_axis).GetSafeNormal());

	wgdc.error.X = wgdc.error.X*(wi_ref_i);
	wgdc.error.Y = wgdc.error.Y*(wi_va_i);

	wgdc.integral = wgdc.integral + wgdc.error * DeltaTime;
	wgdc.derivative = (wgdc.error - wgdc.prev_err) / DeltaTime;

	wgdc.adjustment = wgdc.P * wgdc.error +
		wgdc.I * wgdc.integral +
		wgdc.D * wgdc.derivative;
	wgdc.prev_err = wgdc.error;

	rot_ref = FVector::CrossProduct(rot_ref, gwep_states.up).GetSafeNormal();
	tmp_shaft->AddForceAtPosition(rot_ref*wgdc.adjustment.X, gwep_states.pos + gwep_states.up, false);
	tmp_shaft->AddForceAtPosition(rot_ref*-wgdc.adjustment.X, gwep_states.pos - gwep_states.up, false);

	tmp_shaft->AddForceAtPosition(vel_axis*wgdc.adjustment.Y, gwep_states.pos + gwep_states.up, false);
	tmp_shaft->AddForceAtPosition(vel_axis*-wgdc.adjustment.Y, gwep_states.pos - gwep_states.up, false);
	//if (!grabbing_weapon)
	//{
	//	wgc.prev_err = FVector::ZeroVector;
	//	wgrc.prev_err = 0.f;
	//	wgic.prev_err = 0.f;
	//}

}


void APhysicsFighter::calculateRelativeInertia(FBodyInstance* offset_bi, const FVector& cor, FMatrix* out_inertia)
{
	FVector bi_inrt = offset_bi->GetBodyInertiaTensor();
	FMatrix bi_T = offset_bi->GetUnrealWorldTransform().ToMatrixWithScale();
	//bi_T.ScaleTranslation(FVector(0.f, 0.f, 0.f));
	FMatrix bi_di = FMatrix(FVector(bi_inrt.X, 0.f, 0.f), FVector(0.f, bi_inrt.Y, 0.f), FVector(0.f, 0.f, bi_inrt.Z), FVector(0.f, 0.f, 0.f));
	*out_inertia = bi_T*bi_di*bi_T.GetTransposed();
	FVector r = offset_bi->GetCOMPosition() - cor;
	float bi_m = offset_bi->GetBodyMass();
	out_inertia->M[0][0] = out_inertia->M[0][0] + bi_m*(r.Y*r.Y + r.Z*r.Z);
	out_inertia->M[1][1] = out_inertia->M[1][1] + bi_m*(r.X*r.X + r.Z*r.Z);
	out_inertia->M[2][2] = out_inertia->M[2][2] + bi_m*(r.X*r.X + r.Y*r.Y);
	out_inertia->M[0][1] = out_inertia->M[0][1] + bi_m*(r.X*r.Y);
	out_inertia->M[0][2] = out_inertia->M[0][2] + bi_m*(r.X*r.Z);
	out_inertia->M[1][2] = out_inertia->M[1][2] + bi_m*(r.Y*r.Z);
}

float APhysicsFighter::inertiaAboutAxis(const FMatrix& inertia_mat, const FVector& axis)
{
	return inertia_mat.M[0][0] * axis.X*axis.X + inertia_mat.M[1][1] * axis.Y*axis.Y + inertia_mat.M[2][2] * axis.Z*axis.Z
		- 2.f*inertia_mat.M[0][1] * axis.X*axis.Y - 2.f*inertia_mat.M[1][2] * axis.Y*axis.Z - 2.f*inertia_mat.M[0][2] * axis.X*axis.Z;
}

void APhysicsFighter::lockGuard()
{
	//Grip_h
	FVector WPri = grip_attachment->ComponentToWorld.GetUnitAxis(EAxis::X);
	FVector WOrth = grip_attachment->ComponentToWorld.GetUnitAxis(EAxis::Y);

	FVector PriAxis1 = grip_axis->GetComponentTransform().InverseTransformVectorNoScale(WPri);
	FVector SecAxis1 = grip_axis->GetComponentTransform().InverseTransformVectorNoScale(WOrth);
	grip_attachment->SetConstraintReferenceOrientation(EConstraintFrame::Frame1, PriAxis1, SecAxis1);

	grip_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	grip_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	grip_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);


	arm_states.guard_locked = true;
}

void APhysicsFighter::unlockGuard()
{

	grip_attachment->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	grip_attachment->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	grip_attachment->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);

	arm_states.guard_locked = false;
}

void APhysicsFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Hook up events for "ZoomIn"
	InputComponent->BindAction("FightMode", IE_Pressed, this, &APhysicsFighter::fightModeOn);
	InputComponent->BindAction("FightMode", IE_Released, this, &APhysicsFighter::fightModeOff);

	InputComponent->BindAction("Guard", IE_Pressed, this, &APhysicsFighter::guard);
	InputComponent->BindAction("Guard", IE_Released, this, &APhysicsFighter::abortGuard);

	InputComponent->BindAction("Grab", IE_Pressed, this, &APhysicsFighter::release);
	InputComponent->BindAction("Grab", IE_Repeat, this, &APhysicsFighter::grab);
	InputComponent->BindAction("Grab", IE_Released, this, &APhysicsFighter::abortGrab);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APhysicsFighter::jump);

	InputComponent->BindAction("Dash", IE_Pressed, this, &APhysicsFighter::dash);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("TurnRate", this, &APhysicsFighter::RSX);
	InputComponent->BindAxis("LookUpRate", this, &APhysicsFighter::RSY);
	InputComponent->BindAxis("MoveForward", this, &APhysicsFighter::LSX);
	InputComponent->BindAxis("MoveRight", this, &APhysicsFighter::LSY);

}


//Input functions
void APhysicsFighter::LSX(float AxisValue)
{
	LS.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APhysicsFighter::LSY(float AxisValue)
{
	LS.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APhysicsFighter::dash()
{
	if (!dash_data.dashing)
	{
		dash_data.dashing = true;
		dash_data.force_timer = 0.f;
		dash_trail->BeginTrails(FName("Spine01"), FName("Neck"), ETrailWidthMode::ETrailWidthMode_FromCentre, 1.0f);
	}

	//torsoBI->AddImpulse(target_direction*skeleton->GetMass()*13.f, true);
}

void APhysicsFighter::jump()
{
	jump_data.jumping = true;
	jump_data.curr_jump_time = 0.0f;
}

void APhysicsFighter::RSY(float AxisValue)
{
	RS.Y = AxisValue;
	scaled_inverted_cam_input_size = FMath::Pow(1 - RS.Size(), 2.0f);
	prev_RS_3D = RS_3D;
	RS_3D = (camera_axis->GetForwardVector()*RS.Y + camera_axis->GetRightVector()* RS.X) + camera_axis->GetUpVector()*scaled_inverted_cam_input_size;
	RS_3D.Normalize();
}

void APhysicsFighter::RSX(float AxisValue)
{
	RS.X = AxisValue;
	scaled_inverted_cam_input_size = FMath::Pow(1 - RS.Size(), 2.0f);
	prev_RS_3D = RS_3D;
	RS_3D = (camera_axis->GetForwardVector()*RS.Y + camera_axis->GetRightVector()* RS.X) + camera_axis->GetUpVector()*scaled_inverted_cam_input_size;
	RS_3D.Normalize();
}

void APhysicsFighter::guard()
{
	arm_states.guarding = true;
}

void APhysicsFighter::abortGuard()
{
	arm_states.guarding = false;
	//unlockGuard(); TODO: WEAPON
}

void APhysicsFighter::fightModeOn()
{

	char_states.fight_mode = true;
	char_states.fm_state = 1;
	char_states.f_t = 0.0f;

	target_speed = 220.f;

	axis_rot_before_fight = camera_axis->GetComponentRotation();

	arm_rot_before_fight = FRotator(camera_spring_arm->GetRelativeTransform().GetRotation());
	arm_length_before_fight = camera_spring_arm->TargetArmLength;

	aquireTarget();
}

void APhysicsFighter::fightModeOff()
{
	char_states.fight_mode = false;
	char_states.fm_state = 3;

	locked_target = nullptr;

	target_speed = 400.f;
}

//TODO: WEAPON
void APhysicsFighter::release()
{
	arm_states.extended = false;
	if (arm_states.holding_weapon)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("----------------------"));
		UE_LOG(LogTemp, Warning, TEXT("grip_vmass before unweld: %f"), grip_v_bi->GetBodyMass());
		UE_LOG(LogTemp, Warning, TEXT("grip_v inertia before unweld: %s"), *grip_v_bi->GetBodyInertiaTensor().ToString());*/

		//wep_attachment->BreakConstraint();
		grip->UnWeldChildren();
		grip->UpdateBodySetup();
		held_weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		held_weapon->getShaftComponent()->SetSimulatePhysics(true);
		held_weapon->getShaftComponent()->SetPhysicsLinearVelocity(grip->GetPhysicsLinearVelocity());
		held_weapon->getShaftComponent()->SetPhysicsAngularVelocity(grip->GetPhysicsAngularVelocity());
		held_weapon->getShaftComponent()->SetAngularDamping(0.0f);

		held_weapon->getShaftComponent()->SetEnableGravity(true);

		held_weapon->deInitGrabbed();

		arm_states.holding_weapon = false;
		arm_states.extended = false;

		/*UE_LOG(LogTemp, Warning, TEXT("----------------------"));*/
		grip_indicator_beam->SetVisibility(true);
		grip_indicator_decal->SetVisibility(true);
		return;
	}
	else if (arm_states.holding_object)
	{
		grip_indicator_beam->SetVisibility(true);
		grip_indicator_decal->SetVisibility(true);
		/*grip->UnWeldChildren();
		grip->UpdateBodySetup();*/
		arm_states.holding_object = false;
		return;
	}

}

void APhysicsFighter::grab()
{
	if (!arm_states.grabbing_weapon && !arm_states.holding_weapon)
	{
		//TSet<AActor*> overlaps;
		//grip->GetOverlappingActors(overlaps);

		TArray<FHitResult> traces;
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = false;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		//GetWorld()->LineTraceMultiByChannel(traces, grip->GetComponentLocation() + FVector::UpVector*150.f, grip->GetComponentLocation() - FVector::UpVector*300.f, ECollisionChannel::ECC_PhysicsBody, RV_TraceParams);
		GetWorld()->SweepMultiByProfile(traces, grip->GetComponentLocation() + FVector::UpVector*150.f, grip->GetComponentLocation() - FVector::UpVector*300.f, FQuat(), FName("GrabbableObjects"), FCollisionShape::MakeSphere(20.f), RV_TraceParams);

		//GetWorld()->SweepMultiByChannel(traces, grip->GetComponentLocation() + FVector::UpVector*150.f, grip->GetComponentLocation() - FVector::UpVector*300.f, FQuat(), ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeSphere(grip->GetUnscaledSphereRadius()), RV_TraceParams);
		if (traces.Num() > 0)
		{
			held_weapon = dynamic_cast<AWeapon*>(traces[0].GetActor());
			if (held_weapon)
			{
				arm_states.grabbing_weapon = true;
				held_weapon->getShaftComponent()->SetEnableGravity(false);
				held_weapon->getShaftComponent()->SetAngularDamping(2.0f);
				//held_weapon->getShaftComponent()->SetAngularDamping(1.0f);
				//held_weapon->getShaftComponent()->SetLinearDamping(1.0f);
			}
		}
	}
}

void APhysicsFighter::attachWeapon(AWeapon* _wep)
{
	// if attachWeapon was called outside of weapongrabcontrol, for example from a blueprint
	held_weapon = _wep;

	UCapsuleComponent* tmp_shaft = held_weapon->getShaftComponent();
	tmp_shaft->SetSimulatePhysics(false);
	tmp_shaft->SetAngularDamping(0.0f);
	USceneComponent* tmp_handle = held_weapon->getAttachmentPoint();
	held_weapon->SetActorRotation(grip->GetComponentRotation());

	FVector handle_offset = held_weapon->GetActorLocation() - tmp_handle->GetComponentLocation();
	held_weapon->SetActorLocation(grip->GetComponentLocation() + (handle_offset));

	//wep_attachment->ConstraintActor2 = held_weapon;
	//wep_attachment->SetConstrainedComponents(grip, NAME_None, tmp_shaft, NAME_None);
	FAttachmentTransformRules wep_attach_rules(EAttachmentRule::KeepWorld,true);
	held_weapon->AttachToComponent(grip, wep_attach_rules, NAME_None);
	grip->UpdateBodySetup();

	object_attach_audio->Activate();

	grip_indicator_beam->SetVisibility(false);
	grip_indicator_decal->SetVisibility(false);

	gwep_states.up = FVector::ZeroVector;

	arm_states.holding_weapon = true;
	arm_states.extended = false;
	held_weapon->initGrabbed(this);
}


void APhysicsFighter::abortGrab()
{
	if (arm_states.grabbing_weapon)
	{
		held_weapon->getShaftComponent()->SetEnableGravity(true);
		held_weapon = nullptr;
		arm_states.grabbing_weapon = false;
	}

}