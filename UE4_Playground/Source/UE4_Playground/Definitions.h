// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class ETILETYPE : uint8
{
	
	VE_BLOCKED 	UMETA(DisplayName = "Blocked"),
	VE_BASE 	UMETA(DisplayName = "Base"),
	VE_TURN_RIGHT		UMETA(DisplayName = "TurnRight"),
	VE_TURN_LEFT		UMETA(DisplayName = "TurnRight"),
	VE_STRAIGHT		UMETA(DisplayName = "Straight"),
	VE_START       UMETA(DisplayName = "Start"),
	VE_END       UMETA(DisplayName = "End"),
};



template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
};

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return (EnumType)Enum->FindEnumIndex(FName(*String));
};



/**
 * 
 */
class UE4_PLAYGROUND_API Definitions
{
public:
	Definitions();
	~Definitions();
};
