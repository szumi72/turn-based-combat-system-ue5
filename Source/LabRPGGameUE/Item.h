// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LABRPGGAMEUE_API UItem : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 Value;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* Icon;


};
