// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsFighter.h"


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

void APhysicsFighter::constructBody()
{
	skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	skeleton->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  sm_object(TEXT("/Game/PhysicsFighter/skeletalMeshes/featherBotNoArm.featherBotNoArm"));
	skeleton->SetSkeletalMesh(sm_object.Object);
	//body->SetPhysicsAsset(jointSKeletalMesh.Object->PhysicsAsset);
	//body->SetSimulatePhysics(true);

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

	wep_attachment = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WeaponAttachment"));
	wep_attachment->SetupAttachment(grip);
	wep_attachment->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

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

	OnCalculateControlGripPhysics.BindUObject(this, &APhysicsFighter::ControlGripPhysics);
	OnCalculateCustomInitGripPhysics.BindUObject(this, &APhysicsFighter::customInitGripPhysics);
	OnCalculateControlArmJointDirectionPhysics.BindUObject(this, &APhysicsFighter::ControlArmJointDirectionPhysics);
	OnCalculateControlWeaponTwistPhysics.BindUObject(this, &APhysicsFighter::ControlWeaponTwistPhysics);
	OnCalculateControlArmTwistPhysics.BindUObject(this, &APhysicsFighter::ControlArmTwistPhysics);
	OnCalculateWeaponGrabControl.BindUObject(this, &APhysicsFighter::weaponGrabControl);

	CalculateControlBody.BindUObject(this, &APhysicsFighter::ControlBody);
	OnCalculateCustomHoverPhysics.BindUObject(this, &APhysicsFighter::customHoverPhysics);

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

	//arm and weapon ------------------------------------------------
	ajdc_targets.SetNum(2);
	ajdc_targets[0].dir = FVector::UpVector;
	ajdc_targets[0].prev_dir_xy = FVector::ForwardVector;

	ajdc_targets[1].dir = FVector::UpVector;
	ajdc_targets[1].prev_dir = FVector::ForwardVector;
	ajdc_targets[1].prev_dir_xy = FVector::ForwardVector;

	weapon_twist_solder = FVector::UpVector;
	weapon_twist_target = FVector::RightVector;

	arm_BIs.Add(grip_axis->GetBodyInstance());
	arm_BIs.Add(grip->GetBodyInstance());

	abis.SetNum(arm_BIs.Num());
	abis[0].prev_up = FVector::UpVector;
	abis[1].prev_up = FVector::UpVector;

	//grip_axis_bi = grip_axis->GetBodyInstance();
	//grip_bi = grip->GetBodyInstance();
	g_pos_offset = FVector::ZeroVector;

	ga_prev_up = FVector::ZeroVector;
	g_prev_up = FVector::ZeroVector;
	w_prev_up = FVector::ZeroVector;
	//weapon_handle_2_bi = weapon_handle_2->GetBodyInstance();

	//grabbing  ------------------------------------------------

}

// Called every frame
void APhysicsFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cameraCalculations(DeltaTime);

	if (char_states.alive)
	{
		camera_axis->SetWorldLocation(torsoBI->GetCOMPosition() + FVector::UpVector*40.f);
		//grip_axis->SetWorldLocation(torso->GetCenterOfMass() + FVector(0.f, 0.f, -15.f));
		//grip_axis_bi->SetBodyTransform(FTransform(torso_bi->GetCOMPosition() + FVector(0.f, 0.f, -15.f)), true);


		arm_BIs[1]->AddCustomPhysics(OnCalculateControlGripPhysics);

		if (!arm_states.holding_weapon)
		{
			gripIndicatorCalculations(DeltaTime);
		}

		torsoBI->AddCustomPhysics(OnCalculateCustomHoverPhysics);
		torsoBI->AddCustomPhysics(CalculateControlBody);

		if (char_states.can_move)
		{
			movementCalculations(DeltaTime);
		}
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

	FVector start = g_pos;
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
		grip_indicator_beam->SetBeamTargetPoint(0, g_pos - FVector::UpVector*rv_hit.Distance, 0);

	}
	else
	{
		grip_indicator_beam->SetBeamTargetPoint(0, g_pos - FVector::UpVector*200.f, 0);
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
	//if (mortal)
	//{
	//	if (OtherActor != this)
	//	{
	//		AWeapon* other_weapon = Cast<AWeapon>(OtherActor);
	//		if (other_weapon && other_weapon != held_weapon)
	//		{
	//			if (NormalImpulse.Size() > 10000.f)
	//			{


	//				release();

	//				//body->BreakConstraint(FVector::ZeroVector, FVector::ZeroVector, Hit.BoneName);
	//				FHitResult other_hit;
	//				Hit.GetReversedHit(other_hit);
	//				FConstraintInstance* tmp_const = body->FindConstraintInstance(other_hit.BoneName);
	//				if (tmp_const)
	//				{
	//					//tmp_const->TermConstraint();
	//					body->BreakConstraint(FVector::ZeroVector, FVector::ZeroVector, other_hit.BoneName);
	//				}


	//				alive = false;
	//				fight_mode = false;
	//				body->OnComponentHit.RemoveAll(this);

	//				AFightModeBase* tmp_mode = Cast<AFightModeBase>(GetWorld()->GetAuthGameMode());
	//				if (tmp_mode)
	//				{
	//					if (this->GetNetOwningPlayer())
	//						tmp_mode->registerDeath(this->GetNetOwningPlayer()->PlayerController);
	//					else
	//						tmp_mode->registerDeath(nullptr);
	//				}
	//				//HitComp->DestroyComponent();
	//			}
	//		}

	//	}
	//}
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

	limb->bi->AddTorque(limb->state.up*limb->pid->adjustment.Z, false);

}


void APhysicsFighter::ControlGripPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	customInitGripPhysics(DeltaTime, BodyInstance);
	if (arm_states.disabled)
	{
		setDisabledStanceTargets();
		ControlArmTwistPhysics(DeltaTime, BodyInstance);
		ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[1]);
		ControlWeaponTwistPhysics(DeltaTime, BodyInstance);

		if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - arm_states.disable_start > arm_states.disable_duration)
		{
			ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[0]);
			if (1.f - FVector::DotProduct(ajdc_targets[0].dir, abis[0].up) < 0.1f && FVector::Parallel(RS_3D, FVector::UpVector, 0.25f))
				arm_states.disabled = false;
		}
	}
	else if (arm_states.guard_locked)
	{
		setGuardingStanceTargets();
		ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[0]);
		ControlArmTwistPhysics(DeltaTime, BodyInstance);
	}
	else if (arm_states.guarding)
	{
		setGuardingStanceTargets();
		ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[0]);
		ControlArmTwistPhysics(DeltaTime, BodyInstance);
		ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[1]);
		if (!arm_states.guard_locked && (1.f - FVector::DotProduct(ajdc_targets[0].dir, ga_up)) < 0.01f && (1.f - FVector::DotProduct(ajdc_targets[1].dir, g_up)) < 0.01f && FMath::Abs(atc.error) < 0.01f)
		{
			lockGuard();
			char_states.fight_stance = 2;
		}
		ControlWeaponTwistPhysics(DeltaTime, BodyInstance);
	}
	else
	{
		setNormalStanceTargets();
		ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[0]);
		ControlArmTwistPhysics(DeltaTime, BodyInstance);
		ControlArmJointDirectionPhysics(DeltaTime, arm_BIs[1]);
		ControlWeaponTwistPhysics(DeltaTime, BodyInstance);

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
	if (!char_states.fight_mode) {
		arm_states.extended = false;
		ajdc_targets[1].dir = (-camera_axis->GetRightVector()*0.3f + camera_axis->GetForwardVector() + camera_axis->GetUpVector()*0.4f).GetSafeNormal();
		ajdc_targets[0].dir = (camera_axis->GetRightVector() - camera_axis->GetUpVector()*0.3f + camera_axis->GetForwardVector()*0.4f).GetSafeNormal();
	}
	else {
		ajdc_targets[1].dir = RS_3D;
		if (ajdc_targets[1].dir_xy.IsNearlyZero()) {
			ajdc_targets[1].dir_xy = ajdc_targets[1].prev_dir_xy;
		}
		else
		{
			ajdc_targets[1].prev_dir_xy = ajdc_targets[1].dir_xy;
		}

		ajdc_targets[1].dir_xy = ajdc_targets[1].dir - FVector::DotProduct(ajdc_targets[1].dir, camera_axis->GetUpVector())*camera_axis->GetUpVector();

		ajdc_targets[0].dir = RS_3D;
		ajdc_targets[0].dir_xy = ajdc_targets[0].dir - FVector::DotProduct(ajdc_targets[0].dir, camera_axis->GetUpVector())*camera_axis->GetUpVector();
	}
}
void APhysicsFighter::setGuardingStanceTargets()
{
	if (!char_states.fight_mode) {
		RS_3D = camera_axis->GetForwardVector();
		arm_states.extended = false;
		ajdc_targets[1].dir = RS_3D;
		ajdc_targets[0].dir = RS_3D;

		ajdc_targets[1].dir_xy = ajdc_targets[1].dir - FVector::DotProduct(ajdc_targets[1].dir, camera_axis->GetUpVector())*camera_axis->GetUpVector();

		ajdc_targets[0].dir_xy = ajdc_targets[0].dir - FVector::DotProduct(ajdc_targets[0].dir, camera_axis->GetUpVector())*camera_axis->GetUpVector();
		ajdc_targets[0].dir_xy.Normalize();

		if (ajdc_targets[1].dir_xy.IsNearlyZero())
			ajdc_targets[1].dir_xy = ajdc_targets[1].prev_dir_xy;

		FVector guard_pos(FMath::Cos(PI / 2.8f), 0.f, FMath::Sin(PI / 2.8f));
		guard_pos.Normalize();

		float tmp_angle = FMath::Acos(FVector::DotProduct(ajdc_targets[1].dir_xy.GetSafeNormal(), FVector::ForwardVector))*180.f / PI;
		if (FVector::CrossProduct(FVector::ForwardVector, ajdc_targets[1].dir_xy.GetSafeNormal()).Z < 0)
			tmp_angle = -tmp_angle;
		ajdc_targets[0].dir = guard_pos.RotateAngleAxis(tmp_angle, FVector::UpVector);

		FVector guard_dir(FMath::Cos(-PI / 3.f), 0.f, FMath::Sin(-PI / 3.f));
		guard_dir.Normalize();
		ajdc_targets[1].dir = guard_dir.RotateAngleAxis(tmp_angle, FVector::UpVector);
	}
	else {
		arm_states.extended = false;
		ajdc_targets[1].dir = RS_3D;
		ajdc_targets[0].dir = RS_3D;

		ajdc_targets[1].dir_xy = ajdc_targets[1].dir - FVector::DotProduct(ajdc_targets[1].dir, camera_axis->GetUpVector())*camera_axis->GetUpVector();

		ajdc_targets[0].dir_xy = ajdc_targets[0].dir - FVector::DotProduct(ajdc_targets[1].dir, camera_axis->GetUpVector())*camera_axis->GetUpVector();
		ajdc_targets[0].dir_xy.Normalize();

		if (ajdc_targets[1].dir_xy.IsNearlyZero())
			ajdc_targets[1].dir_xy = ajdc_targets[1].prev_dir_xy;

		FVector guard_pos(FMath::Cos(PI / 2.8f), 0.f, FMath::Sin(PI / 2.8f));
		guard_pos.Normalize();

		float tmp_angle = FMath::Acos(FVector::DotProduct(ajdc_targets[1].dir_xy.GetSafeNormal(), FVector::ForwardVector))*180.f / PI;
		if (FVector::CrossProduct(FVector::ForwardVector, ajdc_targets[1].dir_xy.GetSafeNormal()).Z < 0)
			tmp_angle = -tmp_angle;
		ajdc_targets[0].dir = guard_pos.RotateAngleAxis(tmp_angle, FVector::UpVector);

		FVector guard_dir(FMath::Cos(-PI / 3.f), 0.f, FMath::Sin(-PI / 3.f));
		guard_dir.Normalize();
		ajdc_targets[1].dir = guard_dir.RotateAngleAxis(tmp_angle, FVector::UpVector);
	}
}

void APhysicsFighter::setDisabledStanceTargets()
{
	arm_states.extended = false;
	ajdc_targets[0].dir = FVector::UpVector;
	ajdc_targets[1].dir = abis[0].up;
}

void APhysicsFighter::setArmTwistTargets()
{
	FVector ga_up_xy = ga_up;
	ga_up_xy.Z = 0.f;
	//ga_up_xy = ga_up_xy.GetSafeNormal(0.001f);
	if (!ga_up_xy.IsNearlyZero(0.09f))
	{
		ga_up_xy.Normalize();
		FVector projection_vector = (ga_forward - FVector::DotProduct(ga_forward, ga_up_xy)*ga_up_xy).GetSafeNormal();

		ajdc_targets[0].twist_dir = projection_vector;
		atc.error = 1.f - FVector::DotProduct(projection_vector, -FVector::UpVector);
		if (FVector::DotProduct(ga_up_xy, FVector::CrossProduct(-FVector::UpVector, projection_vector).GetSafeNormal()) > 0.f)
			ajdc_targets[0].twist_dir = -ajdc_targets[0].twist_dir;
	}
	else
	{
		FVector ga_forward_xy = ga_forward;
		ga_forward_xy.Z = 0.f;
		ga_forward_xy.Normalize();

		atc.error = 1.f - FVector::DotProduct(ga_forward_xy, camera_axis->GetForwardVector());
		if (FVector::CrossProduct(ga_forward_xy, camera_axis->GetForwardVector()).GetSafeNormal().Z < 0.f)
			atc.error = -atc.error;
	}
}


void APhysicsFighter::customInitGripPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	//grip_axis_bi->SetBodyTransform(FTransform(torso_bi->GetCOMPosition() + FVector(0.f, 0.f, -15.f)), true);
	ga_pos = arm_BIs[0]->GetUnrealWorldTransform().GetLocation();
	ga_forward = arm_BIs[0]->GetUnrealWorldTransform().GetUnitAxis(EAxis::X);
	ga_right = arm_BIs[0]->GetUnrealWorldTransform().GetUnitAxis(EAxis::Y);
	ga_prev_up = ga_up;
	ga_up = arm_BIs[0]->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);

	g_pos = arm_BIs[1]->GetUnrealWorldTransform().GetLocation();
	g_forward = arm_BIs[1]->GetUnrealWorldTransform().GetUnitAxis(EAxis::X);
	g_right = arm_BIs[1]->GetUnrealWorldTransform().GetUnitAxis(EAxis::Y);
	g_prev_up = g_up;
	g_up = arm_BIs[1]->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);

	for (int i = 0; i < arm_BIs.Num(); i++)
	{

		abis[i].pos = arm_BIs[i]->GetUnrealWorldTransform().GetLocation();
		abis[i].forward = arm_BIs[i]->GetUnrealWorldTransform().GetUnitAxis(EAxis::X);
		abis[i].right = arm_BIs[i]->GetUnrealWorldTransform().GetUnitAxis(EAxis::Y);
		abis[i].prev_up = abis[i].up;
		abis[i].up = arm_BIs[i]->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);

	}

}

void APhysicsFighter::ControlArmJointDirectionPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	//find joint index
	int ji = arm_BIs.Num();
	for (int i = 0; i < arm_BIs.Num(); i++) {
		if (arm_BIs[i] == BodyInstance) {
			ji = i;
			break;
		}
	}

	ajdc[ji].error.X = 1.f - FVector::DotProduct(ajdc_targets[ji].dir, abis[ji].up);
	FVector rot_ref = FVector::CrossProduct(abis[ji].up, ajdc_targets[ji].dir);
	rot_ref = rot_ref.GetSafeNormal();

	//calculate rotational speed around rotation point
	FVector vel_axis = (abis[ji].up - abis[ji].prev_up) / DeltaTime;
	ajdc[ji].error.Y = -vel_axis.Size();
	vel_axis = vel_axis.GetSafeNormal();

	//Joint inertia around velocity and ref axis
	float jvi = 0.f, jri = 0.f;
	FMatrix joint_inertia;

	for (int i = ji; i < arm_BIs.Num(); i++) {
		calculateRelativeInertia(arm_BIs[i], abis[ji].pos, &joint_inertia);
		jvi += inertiaAboutAxis(joint_inertia, FVector::CrossProduct(abis[ji].up, vel_axis).GetSafeNormal());
		jri += inertiaAboutAxis(joint_inertia, rot_ref);
	}

	//calculate PID
	ajdc[ji].error.X = ajdc[ji].error.X*(jri);
	ajdc[ji].error.Y = ajdc[ji].error.Y*(jvi);

	ajdc[ji].integral = ajdc[ji].integral + ajdc[ji].error * DeltaTime;
	ajdc[ji].derivative = (ajdc[ji].error - ajdc[ji].prev_err) / DeltaTime;

	ajdc[ji].adjustment = ajdc[ji].P * ajdc[ji].error +
		ajdc[ji].I * ajdc[ji].integral +
		ajdc[ji].D * ajdc[ji].derivative;
	ajdc[ji].prev_err = ajdc[ji].error;


	//apply pid adjusted forces
	rot_ref = FVector::CrossProduct(rot_ref, abis[ji].up).GetSafeNormal();
	arm_BIs[ji]->AddForceAtPosition(rot_ref*ajdc[ji].adjustment.X, abis[ji].pos + abis[ji].up, false);
	arm_BIs[ji]->AddForceAtPosition(rot_ref*-ajdc[ji].adjustment.X, abis[ji].pos - abis[ji].up, false);

	arm_BIs[ji]->AddForceAtPosition(vel_axis*ajdc[ji].adjustment.Y, abis[ji].pos + abis[ji].up, false);
	arm_BIs[ji]->AddForceAtPosition(vel_axis*-ajdc[ji].adjustment.Y, abis[ji].pos - abis[ji].up, false);
}

void APhysicsFighter::ControlArmTwistPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	FVector ga_up_xy = ga_up;
	ga_up_xy.Z = 0.f;
	//ga_up_xy = ga_up_xy.GetSafeNormal(0.001f);
	if (!ga_up_xy.IsNearlyZero(0.09f))
	{
		ga_up_xy.Normalize();
		FVector projection_vector = (ga_forward - FVector::DotProduct(ga_forward, ga_up_xy)*ga_up_xy).GetSafeNormal();

		atc.error = 1.f - FVector::DotProduct(projection_vector, -FVector::UpVector);
		if (FVector::DotProduct(ga_up_xy, FVector::CrossProduct(-FVector::UpVector, projection_vector).GetSafeNormal()) > 0.f)
			atc.error = -atc.error;
	}
	else
	{
		FVector ga_forward_xy = ga_forward;
		ga_forward_xy.Z = 0.f;
		ga_forward_xy.Normalize();

		atc.error = 1.f - FVector::DotProduct(ga_forward_xy, camera_axis->GetForwardVector());
		if (FVector::CrossProduct(ga_forward_xy, camera_axis->GetForwardVector()).GetSafeNormal().Z < 0.f)
			atc.error = -atc.error;
	}



	FMatrix grip_inertia;
	calculateRelativeInertia(arm_BIs[1], ga_pos, &grip_inertia);
	float gi_ref_i = inertiaAboutAxis(grip_inertia, ga_up);

	//atc.error = atc.error*(gi_ref_i + grip_axis_bi->GetBodyInertiaTensor().Z);
	//arm twist control error with inertia
	float atcewi = atc.error*(gi_ref_i + arm_BIs[0]->GetBodyInertiaTensor().Z);

	atc.integral = atc.integral + atcewi * DeltaTime;
	atc.derivative = (atcewi - atc.prev_err) / DeltaTime;

	atc.adjustment = atc.P * atcewi +
		atc.I * atc.integral +
		atc.D * atc.derivative;
	atc.prev_err = atcewi;

	arm_BIs[0]->AddTorque(ga_up*atc.adjustment, false);
}

void APhysicsFighter::ControlWeaponTwistPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	FVector solder_dir = g_up - weapon_twist_solder;
	if (solder_dir.Size() > 0.05f)
	{
		weapon_twist_target = solder_dir.GetSafeNormal();
		weapon_twist_solder = weapon_twist_solder + solder_dir*(0.05f / solder_dir.Size());
	}

	weapon_twist_target = weapon_twist_target.GetSafeNormal() - FVector::DotProduct(weapon_twist_target.GetSafeNormal(), g_up)*g_up;
	weapon_twist_target = weapon_twist_target.GetSafeNormal();

	wtc.error = 1 - FVector::DotProduct(g_forward, weapon_twist_target);
	float tmp_forward_angle_ref = 1 - FVector::DotProduct(-g_forward, weapon_twist_target);

	if (tmp_forward_angle_ref < wtc.error)
	{
		wtc.error = tmp_forward_angle_ref;
		g_forward = -g_forward;
	}
	FVector tmp_ref_vec = FVector::CrossProduct(g_forward, weapon_twist_target);

	if (!FVector::Coincident(tmp_ref_vec, g_up, 0.0001f))
		wtc.error = -wtc.error;

	wtc.integral = wtc.integral + wtc.error * DeltaTime;
	wtc.derivative = (wtc.error - wtc.prev_err) / DeltaTime;

	wtc.adjustment = wtc.P * wtc.error +
		wtc.I * wtc.integral +
		wtc.D * wtc.derivative;
	wtc.prev_err = wtc.error;

	float tmp_inertia = arm_BIs[1]->GetBodyInertiaTensor().Z;
	arm_BIs[1]->AddTorque(g_up*wtc.adjustment*tmp_inertia, false);

}


void APhysicsFighter::weaponGrabControl(float DeltaTime, FBodyInstance* BodyInstance)
{
	//FBodyInstance* tmp_shaft = held_weapon->getShaftComponent()->GetBodyInstance();
	//w_prev_up = w_up;
	//w_up = tmp_shaft->GetUnrealWorldTransform().GetUnitAxis(EAxis::Z);
	//w_pos = tmp_shaft->GetUnrealWorldTransform().GetLocation();
	//USceneComponent* tmp_attachment = held_weapon->getAttachmentPoint();

	//wgc.error = g_pos - tmp_attachment->GetComponentLocation();

	////weapon position --------------------------------------------------------------
	//wgc.integral = wgc.integral + wgc.error * DeltaTime;
	//wgc.derivative = (wgc.error - wgc.prev_err) / DeltaTime;

	//wgc.adjustment = wgc.P * wgc.error +
	//	wgc.I * wgc.integral +
	//	wgc.D * wgc.derivative;
	//wgc.prev_err = wgc.error;

	//float mass = tmp_shaft->GetBodyMass();
	////tmp_shaft->AddForceAtPosition(wgc.adjustment*(mass)/*+mass*980.f*FVector::UpVector*/, tmp_attachment->GetComponentLocation(), false);
	//tmp_shaft->AddForce(wgc.adjustment*(mass), false);
	//////weapon rotation --------------------------------------------------------------
	//wgdc.error.X = 1.f - FVector::DotProduct(g_up, w_up);
	//UE_LOG(LogTemp, Warning, TEXT("wgdc.error.X  %f"), wgdc.error.X);
	//FVector rot_ref = FVector::CrossProduct(w_up, g_up);
	//rot_ref = rot_ref.GetSafeNormal();

	//if (wgc.error.Size() < 10.f && FMath::Abs(wgdc.error.X) < 0.1f)
	//{
	//	held_weapon->getShaftComponent()->SetAngularDamping(0.0f);
	//	held_weapon->getShaftComponent()->SetLinearDamping(0.01f);
	//	attachWeapon(held_weapon);

	//	grabbing_weapon = false;
	//}

	//FVector vel_axis = (w_up - w_prev_up) / DeltaTime;
	//wgdc.error.Y = -vel_axis.Size();
	//vel_axis = vel_axis.GetSafeNormal();

	//FMatrix wep_inertia;
	//calculateRelativeInertia(tmp_shaft, w_pos, &wep_inertia);
	//float wi_ref_i = inertiaAboutAxis(wep_inertia, rot_ref);
	//float wi_va_i = inertiaAboutAxis(wep_inertia, FVector::CrossProduct(w_up, vel_axis).GetSafeNormal());

	//wgdc.error.X = wgdc.error.X*(wi_ref_i);
	//wgdc.error.Y = wgdc.error.Y*(wi_va_i);

	//wgdc.integral = wgdc.integral + wgdc.error * DeltaTime;
	//wgdc.derivative = (wgdc.error - wgdc.prev_err) / DeltaTime;

	//wgdc.adjustment = wgdc.P * wgdc.error +
	//	wgdc.I * wgdc.integral +
	//	wgdc.D * wgdc.derivative;
	//wgdc.prev_err = wgdc.error;

	//rot_ref = FVector::CrossProduct(rot_ref, w_up).GetSafeNormal();
	//tmp_shaft->AddForceAtPosition(rot_ref*wgdc.adjustment.X, w_pos + w_up, false);
	//tmp_shaft->AddForceAtPosition(rot_ref*-wgdc.adjustment.X, w_pos - w_up, false);

	//tmp_shaft->AddForceAtPosition(vel_axis*wgdc.adjustment.Y, w_pos + w_up, false);
	//tmp_shaft->AddForceAtPosition(vel_axis*-wgdc.adjustment.Y, w_pos - w_up, false);
	////if (!grabbing_weapon)
	////{
	////	wgc.prev_err = FVector::ZeroVector;
	////	wgrc.prev_err = 0.f;
	////	wgic.prev_err = 0.f;
	////}

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

	//InputComponent->BindAction("Grab", IE_Pressed, this, &APhysicsFighter::release);
	//InputComponent->BindAction("Grab", IE_Repeat, this, &APhysicsFighter::grab);
	//InputComponent->BindAction("Grab", IE_Released, this, &APhysicsFighter::abortGrab);

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
//void AJointCharacterTest::release()
//{
//	wep_extended = false;
//	if (holding_weapon)
//	{
//		/*UE_LOG(LogTemp, Warning, TEXT("----------------------"));
//		UE_LOG(LogTemp, Warning, TEXT("grip_vmass before unweld: %f"), grip_v_bi->GetBodyMass());
//		UE_LOG(LogTemp, Warning, TEXT("grip_v inertia before unweld: %s"), *grip_v_bi->GetBodyInertiaTensor().ToString());*/
//
//		//wep_attachment->BreakConstraint();
//		grip->UnWeldChildren();
//		grip->UpdateBodySetup();
//		held_weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		held_weapon->getShaftComponent()->SetSimulatePhysics(true);
//		held_weapon->getShaftComponent()->SetPhysicsLinearVelocity(grip->GetPhysicsLinearVelocity());
//		held_weapon->getShaftComponent()->SetPhysicsAngularVelocity(grip->GetPhysicsAngularVelocity());
//		held_weapon->getShaftComponent()->SetAngularDamping(0.0f);
//
//		held_weapon->getShaftComponent()->SetEnableGravity(true);
//
//		held_weapon->deInitGrabbed();
//
//		holding_weapon = false;
//		wep_extended = false;
//
//		/*UE_LOG(LogTemp, Warning, TEXT("----------------------"));*/
//		grip_indicator_beam->SetVisibility(true);
//		grip_indicator_decal->SetVisibility(true);
//		return;
//	}
//	else if (holding_object)
//	{
//		grip_indicator_beam->SetVisibility(true);
//		grip_indicator_decal->SetVisibility(true);
//		/*grip->UnWeldChildren();
//		grip->UpdateBodySetup();*/
//		holding_object = false;
//		return;
//	}
//
//}
//
//void AJointCharacterTest::grab()
//{
//	if (!grabbing_weapon && !holding_weapon)
//	{
//		//TSet<AActor*> overlaps;
//		//grip->GetOverlappingActors(overlaps);
//
//		TArray<FHitResult> traces;
//		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
//		RV_TraceParams.bTraceComplex = false;
//		RV_TraceParams.bTraceAsyncScene = true;
//		RV_TraceParams.bReturnPhysicalMaterial = false;
//
//		//GetWorld()->LineTraceMultiByChannel(traces, grip->GetComponentLocation() + FVector::UpVector*150.f, grip->GetComponentLocation() - FVector::UpVector*300.f, ECollisionChannel::ECC_PhysicsBody, RV_TraceParams);
//		GetWorld()->SweepMultiByProfile(traces, grip->GetComponentLocation() + FVector::UpVector*150.f, grip->GetComponentLocation() - FVector::UpVector*300.f, FQuat(), FName("GrabbableObjects"), FCollisionShape::MakeSphere(20.f), RV_TraceParams);
//
//		//GetWorld()->SweepMultiByChannel(traces, grip->GetComponentLocation() + FVector::UpVector*150.f, grip->GetComponentLocation() - FVector::UpVector*300.f, FQuat(), ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeSphere(grip->GetUnscaledSphereRadius()), RV_TraceParams);
//		if (traces.Num() > 0)
//		{
//			held_weapon = dynamic_cast<AWeapon*>(traces[0].GetActor());
//			if (held_weapon)
//			{
//				grabbing_weapon = true;
//				held_weapon->getShaftComponent()->SetEnableGravity(false);
//				held_weapon->getShaftComponent()->SetAngularDamping(2.0f);
//				//held_weapon->getShaftComponent()->SetAngularDamping(1.0f);
//				//held_weapon->getShaftComponent()->SetLinearDamping(1.0f);
//			}
//		}
//	}
//}
//
//void AJointCharacterTest::attachWeapon(AWeapon* _wep)
//{
//	// if attachWeapon was called outside of weapongrabcontrol, for example from a blueprint
//	held_weapon = _wep;
//
//	UCapsuleComponent* tmp_shaft = dynamic_cast<UCapsuleComponent*>(held_weapon->GetRootComponent());
//	tmp_shaft->SetSimulatePhysics(false);
//	tmp_shaft->SetAngularDamping(0.0f);
//	USceneComponent* tmp_handle = dynamic_cast<USceneComponent*>(held_weapon->GetComponentsByTag(USceneComponent::StaticClass(), "handle_point").Top());
//	held_weapon->SetActorRotation(grip->GetComponentRotation());
//
//	FVector handle_offset = held_weapon->GetActorLocation() - tmp_handle->GetComponentLocation();
//	held_weapon->SetActorLocation(grip->GetComponentLocation() + (handle_offset));
//
//	//wep_attachment->ConstraintActor2 = held_weapon;
//	//wep_attachment->SetConstrainedComponents(grip, NAME_None, tmp_shaft, NAME_None);
//	held_weapon->AttachRootComponentTo(grip, NAME_None, EAttachLocation::KeepWorldPosition, true);
//	grip->UpdateBodySetup();
//
//	object_attach_audio->Activate();
//
//	grip_indicator_beam->SetVisibility(false);
//	grip_indicator_decal->SetVisibility(false);
//
//	w_up = FVector::ZeroVector;
//
//	holding_weapon = true;
//	wep_extended = false;
//	held_weapon->initGrabbed(this);
//}
//
//
//void AJointCharacterTest::abortGrab()
//{
//	if (grabbing_weapon)
//	{
//		held_weapon->getShaftComponent()->SetEnableGravity(true);
//		held_weapon = nullptr;
//		grabbing_weapon = false;
//	}
//
//}