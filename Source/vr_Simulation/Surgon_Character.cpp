// Fill out your copyright notice in the Description page of Project Settings.

#include "Surgon_Character.h"
#include "Math/TransformNonVectorized.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASurgon_Character::ASurgon_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (GetMesh() != NULL)
		SpringArm->SetupAttachment(RootComponent);//getMesh() returns the Skeletal Mesh Sub object ref.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ASurgon_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASurgon_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	delta_time = DeltaTime;
}

// Called to bind functionality to input
void ASurgon_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASurgon_Character::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASurgon_Character::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACharacter::AddControllerYawInput);
	//PlayerInputComponent->BindAction(TEXT("TurnLeft"), EInputEvent::IE_Pressed, this, &ASurgon_Character::TurnLeft);
	//PlayerInputComponent->BindAction(TEXT("TurnRight"), EInputEvent::IE_Pressed, this, &ASurgon_Character::TurnRight);
}

void ASurgon_Character::MoveForward(float value)
{
	

	if ((Controller != nullptr) && (value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, value);
	}
}

void ASurgon_Character::MoveRight(float value)
{
	if ((Controller != nullptr) && (value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, value);
	}
}

void ASurgon_Character::TurnLeft()
{
	//FRotator IniRot = GetActorRotation();
	SetActorRotation(FMath::RInterpTo(IniRot,FRotator(0, -90, 0), delta_time, interpSpeed));
	
}

void ASurgon_Character::TurnRight()
{
	//FRotator IniRot = GetActorRotation();
	SetActorRotation(FMath::RInterpTo(IniRot, FRotator(0, 90, 0), delta_time, interpSpeed));
}
