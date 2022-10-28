// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadFromFile_Component.h"
#include <string>
#include <algorithm>

// Sets default values for this component's properties
UReadFromFile_Component::UReadFromFile_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

// Called when the game starts
void UReadFromFile_Component::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	//if (!GetOwner())
	//	return;
	//GetOwner()->GetComponents(ActorComponents);
	//for (auto Components : ActorComponents)
	//{
	//	FString s = Components->GetName();
	//	std::string str = TCHAR_TO_UTF8(*s);
	//
	//	std::string SSub = "";
	//	for (int i = 0; i < str.size(); i++)
	//	{
	//		if (str[i]=='_')
	//		{
	//			SSub = str.substr(i);
	//			break;
	//		}
	//	
	//	}
	//	std::replace_if(SSub.begin(), SSub.end(), [](char c) {
	//		return (c == '_' || c == 'l' || c == 'r'); }, ' ');
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *UTF8_TO_TCHAR(SSub.c_str()));
	//}
}


// Called every frame
void UReadFromFile_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

