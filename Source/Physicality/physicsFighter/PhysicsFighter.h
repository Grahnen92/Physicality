// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

#include "weapons/Weapon.h" //TODO: WEP

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhysicsFighter.generated.h"

USTRUCT(BlueprintType)
struct FPIDData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float target;

	float error;
	float prev_err;
	float integral;
	float derivative;
	float adjustment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float max_adjustment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float I;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float D;
};
USTRUCT(BlueprintType)
struct FPIDData2D
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D target;

	FVector2D error;
	FVector2D prev_err;
	FVector2D integral;
	FVector2D derivative;
	FVector2D adjustment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D max_adjustment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D I;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D D;
};
USTRUCT(BlueprintType)
struct FPIDData3D
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector target;

	FVector error;
	FVector prev_err;
	FVector integral;
	FVector derivative;
	FVector adjustment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector max_adjustment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector I;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector D;
};
USTRUCT()
struct FLimbState
{
	GENERATED_USTRUCT_BODY()

	FVector pos;
	FVector forward;
	FVector right;
	FVector up;
	FVector prev_up;
};
USTRUCT()
struct FLimbTarget
{
	GENERATED_USTRUCT_BODY()

		FVector dir;
	FVector prev_dir;
	FVector dir_xy;
	FVector prev_dir_xy;

	FVector twist_dir;
};
USTRUCT()
struct FLimbNode
{
	GENERATED_USTRUCT_BODY()

	FBodyInstance* bi;
	FLimbNode* next = nullptr;

	FLimbState state;

	FPIDData3D* pid;
	FLimbTarget target;

};

USTRUCT(BlueprintType)
struct FPFStates
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "CharacterStates")
	bool mortal = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool alive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	float impact_resistance = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool can_move = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool fight_mode;
	// 0 disengaged
	// 1 engaging
	// 2 engaged
	// 3 disengaging
	int fm_state;
	float f_t;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fm_engage_time = 0.25;
	// 0 normal
	// 1 guarding
	// 2 guarding locked
	// 3 disabled
	int fight_stance = 0;
};

USTRUCT(BlueprintType)
struct FPFJump
{
	GENERATED_USTRUCT_BODY()

	bool ready = true;
	float cd = 1.5f;
	float cd_timer = 1.5f;

	bool jumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float height = 100;
	float force;
	float force_time = 0.1f;
	float curr_jump_time = 0.f;
	//int jump_frame_count;
	//const int jump_frame_nr = 3;

};

USTRUCT(BlueprintType)
struct FPFDash
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ready = true;
	float cd = 1.5f;
	float cd_timer = 0.0f;

	bool dashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed = 50000;
	float force;
	float force_time = 0.1f;
	float force_timer = 0.f;

};


USTRUCT(BlueprintType)
struct FPFWeaponStates
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool disabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float disable_duration = 0.5f;
	float disable_start;

	bool was_standing_still = true;
	bool extended = false;
	bool rot_forward = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool guarding = false;
	bool guard_locked = false;
	bool grabbing_weapon = false;
	bool holding_weapon = false;
	bool holding_object = false;
};


UCLASS()
class PHYSICALITY_API APhysicsFighter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APhysicsFighter();
	// Sets default values for this pawn's properties
	~APhysicsFighter();

protected:

	//body components ==============================================================================
	UPROPERTY(Category = "Body", VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* skeleton;

	UPROPERTY(Category = "Body", VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* dash_trail;

	UPROPERTY(Category = "Body", VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* lthruster;
	UPROPERTY(Category = "Body", VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* rthruster;

	//weapon components ==============================================================================
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* grip_axis;
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* grip_axis_vis;
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UPhysicsConstraintComponent* grip_axis_attachment;

	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* grip;
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* grip_vis;
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UPhysicsConstraintComponent* grip_attachment;

	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UDecalComponent* grip_indicator_decal;
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* grip_indicator_beam;
	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* object_attach_audio;

	//camera Components==============================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* camera_spring_arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USceneComponent* camera_axis;

	//targeting
	TArray<USceneComponent* >lock_on_targets;
	USceneComponent* locked_target;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USphereComponent* targeting_aura;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USphereComponent* targeting_point;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPFStates char_states;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPFDash dash_data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPFJump jump_data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPFWeaponStates arm_states;
	
	UFUNCTION()
	void addPotentialTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void removePotentialTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void aquireTarget();

	//INPUT ==============================================================================
	//Right stick / camera / sword direction
	FVector2D RS;
	float scaled_inverted_cam_input_size = 0.f;

	void RSX(float AxisValue);
	void RSY(float AxisValue);
	void cameraCalculations(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputVars")
	FVector RS_3D;
	FVector prev_RS_3D;

	//Left stick / movement
	FVector2D LS;
	FVector target_direction;

	void LSX(float AxisValue);
	void LSY(float AxisValue);
	
	void movementCalculations(float DeltaTime);

	//abilities
	void dash();
	void jump();

	void fightModeOn();
	void fightModeOff();
	void release();// TODO: WEAPON
	void grab();// TODO: WEAPON
	void abortGrab();
	UFUNCTION(BlueprintCallable)
	void attachWeapon(AWeapon* _wep); //TODO: WEAPON
	AWeapon* held_weapon;// TODO: WEAPON
	UObject* held_object; //TODO: WEAPON
	
	void guard();
	void abortGuard();
	void lockGuard();
	void unlockGuard();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void tickCDs(float DeltaTime);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// STATE checkers/setters/getters ==============================================================================
	void setPlayerSpecificMaterial(UMaterial* mat);
	void setAlive(bool new_state);
	bool isAlive();

	bool isGuarding();

	void setCanMove(bool new_state);

	void setArmDisabled(bool new_state);
	void disableArm(float duration);
	void enableArm();
	void setInputDir(FVector _dir);

	void setFOV(int _fov);

	//PIDS =================================================================================
	//CAMERA -----------------------------------------------------------------
	//camera_direction_control
	//X = horizontal rotation
	//Y = vertical rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPIDData2D cdc;

	FVector locked_target_dir;
	FVector locked_target_dir_xy;

	FRotator axis_target_rot;
	FRotator arm_target_rot;
	FRotator axis_rot_before_fight;
	FRotator arm_rot_before_fight;
	float arm_length_before_fight;
	float target_arm_length;

	//WEAPON -----------------------------------------------------------------

	//arm limb root
	FLimbNode* alr = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmPID")
	FPIDData3D upper_arm_controller;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmPID")
	FPIDData3D hand_controller;

	//weapon_grab_direction_controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPIDData2D wgdc;
	//weapon_grab_controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPIDData3D wgc;

	FLimbState gwep_states;

	//general variables used across several function
	FVector hand_solder;
	FVector hand_solder_dir;

	//Movement control --------------------------------------------------------------
	//hover height controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPIDData hhc;

	UPROPERTY(EditAnywhere)
	float target_speed = 400;
	UPROPERTY(EditAnywhere)
	float time_to_target_speed = 0.05f;

	//movement velocity controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPIDData2D mvc;

	//BODY PIDS -----------------------------------------------------

	//upper body joint chain root
	FLimbNode upbr;
	void setTorsoTargets();
	void setPelvisTargets();

	//right leg joint chain root
	FLimbNode rlr;
	void setRThighTargets();
	void setRShinTargets();
	//left leg joint chain root
	FLimbNode llr;
	void setLThighTargets();
	void setLShinTargets();

	FBodyInstance* torsoBI;

	//Right thigh controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPID")
	FPIDData3D right_thigh_controller;
	//Right shin controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPID")
	FPIDData3D right_shin_controller;

	//Left thigh controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPID")
	FPIDData3D left_thigh_controller;
	//Left shin controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPID")
	FPIDData3D left_shin_controller;

	//pelvis controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPID")
	FPIDData3D pelvis_controller;
	//torso controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyPID")
	FPIDData3D torso_controller;

protected:

	//COLLISION FUNCTIONS =================================================================================
	void FellOutOfWorld(const class UDamageType& dmgType);
	UFUNCTION()
	void OnBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//CONTROL FUNCTIONS =================================================================================
	void setNormalStanceTargets();
	void setGuardingStanceTargets();
	void setDisabledStanceTargets();

	FCalculateCustomPhysics OnCalculateCustomHoverPhysics; 
	void customHoverPhysics(float DeltaTime, FBodyInstance* BodyInstance);

	FCalculateCustomPhysics CalculateControlBody;
	void ControlBody(float DeltaTime, FBodyInstance* BodyInstance);
	
	FCalculateCustomPhysics CalculateControlArm; 
	void ControlArm(float DeltaTime, FBodyInstance* BodyInstance);
	
	FCalculateCustomPhysics OnCalculateWeaponGrabControl; 
	void weaponGrabControl(float DeltaTime, FBodyInstance* BodyInstance);
	


	void updateLimbStates(FLimbNode* limb);
	void updateArmLimbStates(FLimbNode* limb);
	void ControlLimb(float DeltaTime, FLimbNode* limb);

	void controlCameraDirection(float DeltaTime);

	void gripIndicatorCalculations(float DeltaTime);

	//calculates the inertia of a bodyinstance relative to a point in worldspace
	void calculateRelativeInertia(FBodyInstance* offset_bi, const FVector& cor, FMatrix* out_inertia);
	float inertiaAboutAxis(const FMatrix& inertia_mat, const FVector& axis);

	// INIT FUNCTIONS ====================================================================================================
	void constructCamera();
	void initCamera();
	void constructBody();
	void initBody();
	void constructWeapon();
	void initWeapon();
	void initCustomPhysics();
	void initPIDs();
	
};
