// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Anatomy_Character.generated.h"

UCLASS()
class VR_SIMULATION_API AAnatomy_Character : public ACharacter
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
			class USpringArmComponent* Spring;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			class UCameraComponent* Camera;
		USkeletalMeshComponent* SkeletalMesh = GetMesh();
public:
	// Sets default values for this character's properties
	AAnatomy_Character();
	
	UPROPERTY(EditAnywhere, Category="Materials")
		UMaterialInterface* Material;//set the material that you will apply when clicked!!
	
	TArray<UMaterialInterface*> InitialMaterial;// array to store all initial materials of a mesh
	FVector StartLoc=FVector(0,0,0), EndLoc = FVector(0, 0, 0);
	FRotator StartRot = FRotator(0, 0, 0);
	FHitResult Hit;
	FHitResult MobHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RayRange = 100;
	FVector SpringArmIniLoc;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FString Name=TEXT("Click To View");
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;

	FTimerHandle Timer;
	FVector SpawnLocation;
	float MaxArmLength = 300;

	UFUNCTION(BlueprintCallable)
		void ZoomIn();
	UFUNCTION(BlueprintCallable)
		void ZoomOut();
	UFUNCTION(BlueprintCallable)
		void UIZoom(float val);
	UFUNCTION(BlueprintCallable)
		void ViewIndividual();
	UFUNCTION(BlueprintCallable)
		void ResetCamera();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UInputComponent* Inp = NULL;
	UPROPERTY(VisibleAnywhere ,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bOnCursorVisibleState = false;// for notifying the visibility of the cursor
	UPROPERTY(VisibleAnywhere ,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsInMoveUpState = false; // for notifying the activation of MoveUp state 

	bool trace = false;
	bool MobTrace = false;

	void MoveCameraUp(float val);
	void MoveCameraLeft(float);
	void RayCast();
	
	void ResetMaterial();
	
	void EnableCursor();
	void DisableCursor();
	void Turn(float);
	void LookUp(float);
	void MoveUp();
	void bIsInMoveUpState_false() { bIsInMoveUpState = false; }
};
