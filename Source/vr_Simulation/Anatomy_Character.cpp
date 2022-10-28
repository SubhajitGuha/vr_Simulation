// Fill out your copyright notice in the Description page of Project Settings.


#include "Anatomy_Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "RHIDefinitions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

// Sets default values
AAnatomy_Character::AAnatomy_Character()
{
	Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if(SkeletalMesh!=NULL)
		Spring->SetupAttachment(SkeletalMesh);
	Spring->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(Spring);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAnatomy_Character::BeginPlay()
{
	Super::BeginPlay();
	if (Spring)
		MaxArmLength = Spring->TargetArmLength;
	SpringArmIniLoc = Spring->GetRelativeLocation();
	SpawnLocation = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("SpwanIndividual")))->GetComponentLocation();
	
}

// Called every frame
void AAnatomy_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAnatomy_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAnatomy_Character::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAnatomy_Character::LookUp);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"),EInputEvent::IE_Pressed,this,&AAnatomy_Character::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AAnatomy_Character::ZoomOut);
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AAnatomy_Character::MoveCameraUp);
	//PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAnatomy_Character::MoveCameraLeft);
	PlayerInputComponent->BindAction(TEXT("PointARay"), EInputEvent::IE_Pressed, this, &AAnatomy_Character::RayCast);
	PlayerInputComponent->BindAction(TEXT("ViewIndividual"), EInputEvent::IE_Pressed, this, &AAnatomy_Character::ViewIndividual);
	PlayerInputComponent->BindAction(TEXT("ResetCamera"), EInputEvent::IE_Pressed, this, &AAnatomy_Character::ResetCamera);
	PlayerInputComponent->BindAction(TEXT("ToggleCursor"), EInputEvent::IE_Pressed, this, &AAnatomy_Character::EnableCursor);
	PlayerInputComponent->BindAction(TEXT("ToggleCursor"), EInputEvent::IE_Released, this, &AAnatomy_Character::DisableCursor);
	PlayerInputComponent->BindAction(TEXT("TriggerMoveUP"), EInputEvent::IE_Pressed, this, &AAnatomy_Character::MoveUp);
	PlayerInputComponent->BindAction(TEXT("TriggerMoveUP"), EInputEvent::IE_Released, this, &AAnatomy_Character::bIsInMoveUpState_false);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &AAnatomy_Character::MoveCameraUp);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &AAnatomy_Character::MoveCameraLeft);
	//PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed,this,)
}

void AAnatomy_Character::ZoomIn() 
{
	Spring->TargetArmLength = FMath::Clamp(Spring->TargetArmLength-5, (float)10.0, (float)300.0);
}

void AAnatomy_Character::ZoomOut()
{
	Spring->TargetArmLength = FMath::Clamp(Spring->TargetArmLength + 5, (float)10.0, (float)300.0);

}

void AAnatomy_Character::UIZoom(float val)
{
	Spring->TargetArmLength = FMath::Clamp(MaxArmLength * (1 - val), (float)10.0, (float)300.0);
}

void AAnatomy_Character::MoveCameraUp(float val)
{
	if (bOnCursorVisibleState && bIsInMoveUpState)
	//	Spring->AddRelativeLocation(FVector(0, 0, 1 * val));
		Spring->AddRelativeLocation(Camera->GetUpVector()*val);
	//Spring->AddLocalOffset(FVector(0, 0, 1 * val));
}

void AAnatomy_Character::MoveCameraLeft(float val)
{
	
	if (bOnCursorVisibleState && bIsInMoveUpState)
		Spring->AddRelativeLocation(Camera->GetForwardVector()*val);
	//Spring->AddLocalOffset(FVector(1 * val, 0, 0));
}

void AAnatomy_Character::RayCast()
{
	AController* control = GetController();
	if (!control && !Camera)
		return;
	trace = Cast<APlayerController>(control)->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	MobTrace=Cast<APlayerController>(control)->GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, MobHit);
	UE_LOG(LogTemp, Warning, TEXT("isTouch %d"), MobTrace);
	UE_LOG(LogTemp, Warning, TEXT("isClick %d"), trace);
	//control->GetPlayerViewPoint(StartLoc,StartRot);
	//EndLoc = StartLoc + Camera->GetForwardVector() * RayRange;
	//bool trace=GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility);
	//if (!trace)
	//	return;
	//if (!MobTrace)
	//	return;
	//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::MakeRandomColor(), true, 2, 2, 5);
	if (trace && Hit.GetActor()->GetParentActor() != UGameplayStatics::GetPlayerPawn(this, 0))
		return;
	
	if (MobTrace && MobHit.GetActor()->GetParentActor() != UGameplayStatics::GetPlayerPawn(this, 0))
		return;
	
	if (MobTrace) 
	{
		Name = *MobHit.GetComponent()->GetName();
		StaticMesh = Cast<UStaticMeshComponent>(MobHit.GetComponent());
	}
	if (trace) {
		Name = *Hit.GetComponent()->GetName();
		StaticMesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
	}
	//StaticMesh->SetRenderCustomDepth(true);
	if (StaticMesh == NULL)
		return;

	for (int i = 0; i < StaticMesh->GetNumMaterials(); i++)//push all the initial materials of the static mesh in to the array
		InitialMaterial.Push(StaticMesh->GetMaterial(i));

	if (Material) {
		for (int i = 0; i < StaticMesh->GetNumMaterials(); i++)
			StaticMesh->SetMaterial(i, Material);
	}

	
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AAnatomy_Character::ResetMaterial, 3, false);//delay for 3 sec....
}

void AAnatomy_Character::ResetCamera()
{
	if (StaticMesh) {

		StaticMesh->SetRelativeLocation(FVector(0, 0, 0));
	}
	//StaticMesh->SetWorldScale3D(FVector(1, 1, 1));
	if (Spring == NULL)
		return;
	Spring->SetRelativeLocation(SpringArmIniLoc);
}

FString AAnatomy_Character::ReadFromFile(const FString& Path)
{
	std::string destination = std::string(TCHAR_TO_UTF8(*Path));//convert to std string;
	std::ifstream stream(destination);
	if (&stream == nullptr)
		UE_LOG(LogTemp, Error, TEXT("File not found"));
	std::string data;//tostore each line
	std::string information="";//to store the entire file data;
	while(std::getline(stream,data))
	{
		information.append(data + "\n");
	}
	return UTF8_TO_TCHAR(information.c_str());
}

FString AAnatomy_Character::FilterString(const FString& ComponentName)
{
	std::string str = TCHAR_TO_UTF8(*ComponentName);
	
	std::string SSub = "";
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i]=='_')
		{
			SSub = str.substr(i);
			break;
		}
	
	}
	for (int i = 0; i < SSub.size(); i++) {
		if (SSub[i] == '_' || (SSub[i]>='0' && SSub[i]<='9'))
			SSub[i] = ' ';
	}

	//std::replace_if(SSub.begin(), SSub.end(), [](char c) {
	//	return (c == '_' || c == 'l' || c == 'r'); }, ' ');

	int last = SSub.size() - 1;
	while (last >= 0 && (SSub[last] == ' ' || SSub[last]=='l' || SSub[last] == 'r'))
		last--;
	SSub = SSub.substr(0, last + 1);
	return UTF8_TO_TCHAR(SSub.c_str());
}

void AAnatomy_Character::ResetMaterial()
{
	int i = 0;
	for (UMaterialInterface* initMat : InitialMaterial) {//reset all the materials to their init Material
		if(initMat!=NULL)
			StaticMesh->SetMaterial(i, initMat);
		i++;
	}
	InitialMaterial.Empty();
}

void AAnatomy_Character::ViewIndividual()
{
	if (bOnCursorVisibleState) //First check for cursor
		return;
		if (trace && Hit.GetActor()->GetParentActor() != UGameplayStatics::GetPlayerPawn(this, 0))
			return;
		if (MobTrace && MobHit.GetActor()->GetParentActor() != UGameplayStatics::GetPlayerPawn(this, 0))//if hit anywhere other than the player
			return;																			//the no need to view Individual
		if (!StaticMesh)
			return;
		StaticMesh->SetWorldLocation(FVector(StaticMesh->GetComponentLocation().X, SpawnLocation.Y, StaticMesh->GetComponentLocation().Z));
		Spring->TargetArmLength = 30;
		if(trace)
		Spring->SetWorldLocation(FVector(StaticMesh->GetComponentLocation().X, SpawnLocation.Y, Hit.ImpactPoint.Z));//include the mobile hit on touch
		if(MobTrace)
		Spring->SetWorldLocation(FVector(StaticMesh->GetComponentLocation().X, SpawnLocation.Y, MobHit.ImpactPoint.Z));
}

void AAnatomy_Character::EnableCursor()
{
	bOnCursorVisibleState = true;
	AController* control = GetController();
	if(control)
	Cast<APlayerController>(control)->bShowMouseCursor = true;
}

void AAnatomy_Character::DisableCursor()
{
	bOnCursorVisibleState = false;
	AController* control = GetController();
	if (control)
		Cast<APlayerController>(control)->bShowMouseCursor = false;
}

void AAnatomy_Character::Turn(float value)
{
	
	if (!bOnCursorVisibleState)
		AddControllerYawInput(value);
}

void AAnatomy_Character::LookUp(float value)
{
	
	if (!bOnCursorVisibleState)
		AddControllerPitchInput(value);
}

void AAnatomy_Character::MoveUp() // triggers the moveCameraup and MoveCameraLeft
{
	bIsInMoveUpState = true;

}
