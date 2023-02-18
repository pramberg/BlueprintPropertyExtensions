﻿// Copyright Viktor Pramberg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPE_MetadataCollection.h"
#include "GameplayTagContainer.h"
#include "Math/UnitConversion.h"
#include "BPE_CollectionTypes.generated.h"



/**
 * Controls what "Categories", or root gameplay tags can be selected on a GameplayTag
 * or GameplayTagContainer property. Use this if you only want specific tags to be selectable.
 *
 * @see FGameplayTag, FGameplayTagContainer, UGameplayTagsManager::StaticGetCategoriesMetaFromPropertyHandle
 */
UCLASS(meta=(DisplayName = "Gameplay Tag Categories"))
class UBPE_MetadataCollection_GameplayTagCategories : public UBPE_MetadataCollectionStruct
{
	GENERATED_BODY()

public:
	UBPE_MetadataCollection_GameplayTagCategories();

	// One or more root tags that are selectable in the GameplayTag widget.
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Categories;
	
protected:
	virtual TOptional<FString> GetValueForProperty(FProperty& Property) const override;
	virtual void SetValueForProperty(const FProperty& Property, const FString& Value) override;
};



/**
 * Controls what conditions need to be met in order to edit the current property. Also allows for hiding
 * properties that cannot be edited.
 */
UCLASS(meta=(DisplayName = "Edit Condition"))
class UBPE_MetadataCollection_EditCondition : public UBPE_MetadataCollection
{
	GENERATED_BODY()

public:
	// Condition used to enable editing this property. A basic expression would be `bMyBoolVariable`.
	// That would disable editing of the property if `bMyBoolVariable` is false.
	//
	// The syntax is fairly expressive, so you can also do things like:
	// `MyFloatVariable > 0.0`, `!bMyBoolVariable`, `MyAssetEnum == 2` or `MyCppEnum == EnumType::Value`.
	//
	// NOTE: Be aware of the difference between enums defined as an asset in the editor vs. enums defined in C++.
	// Enums defined in C++ can be referred to as `EnumType::Value`, while that's not possible for asset enums.
	// The name of asset enum names are all called `EnumType::NewEnumeratorX`, where X is an incremented index.
	// It is therefore best to refer to asset enum values by index instead.
	UPROPERTY(EditAnywhere)
	FString EditCondition;

	// If the edit condition isn't met, should the property be hidden?
	UPROPERTY(EditAnywhere)
	bool EditConditionHides;

	// If this boolean property is used as an edit condition, should it be inlined next to the property?
	UPROPERTY(EditAnywhere)
	bool InlineEditConditionToggle;
};



/**
 * Controls what unit this property represents. For example, a float variable can represent
 * distance, angles or speed. By specifying a unit, you can make it easier for a user to
 * understand the use of the property.
 */
UCLASS(meta=(DisplayName = "Units"))
class UBPE_MetadataCollection_Units : public UBPE_MetadataCollectionStruct
{
	GENERATED_BODY()

public:
	// Specifies what unit this property represents. The value of the property is automatically converted
	// from the user's preferred unit for that value type. If you specify `Centimeter` and the user's preferred
	// unit is `Yards`, Unreal will handle the conversion from yards to centimeters automatically.
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ForceUnits == EUnit::Unspecified", EditConditionHides))
	EUnit Units = EUnit::Unspecified;

	// Forces the unit of this property to be the selected type, regardless of user preferences.
	// For example, if you specify `Centimeters` and the user uses `Yards`, the property will still
	// use `Centimeters`.
	//
	// Note that this property is prioritized over `Units`. If you specify both `Units` and `ForceUnits`,
	// the behavior of `ForceUnits` will be applied.
	UPROPERTY(EditAnywhere)
	EUnit ForceUnits = EUnit::Unspecified;

protected:
	virtual bool IsRelevantForContainedProperty(const FProperty& InProperty) const override;
	virtual TOptional<FString> GetValueForProperty(FProperty& Property) const override;
	virtual void SetValueForProperty(const FProperty& Property, const FString& Value) override;
};



/** Allows you to specify the name of the axes on runtime curve properties. */
UCLASS(meta=(DisplayName = "Curves"))
class UBPE_MetadataCollection_Curves : public UBPE_MetadataCollectionStruct
{
	GENERATED_BODY()

public:
	UBPE_MetadataCollection_Curves();

	// The name of the X-axis of the curve.
	UPROPERTY(EditAnywhere)
	FString XAxisName;

	// The name of the Y-axis of the curve.
	UPROPERTY(EditAnywhere)
	FString YAxisName;
};



/**
 * Controls what Asset Bundles this property belongs to.
 * TODO: Improve documentation.
 *
 * @see https://docs.unrealengine.com/5.1/en-US/asset-management-in-unreal-engine/#assetbundles
 * @see UAssetManager::InitializeAssetBundlesFromMetadata
 */
UCLASS(meta=(DisplayName = "Asset Bundles"))
class UBPE_MetadataCollection_AssetBundles : public UBPE_MetadataCollection
{
	GENERATED_BODY()

public:
	// The bundles to add this property to.
	UPROPERTY(EditAnywhere)
	TArray<FString> AssetBundles;

protected:
	virtual bool IsRelevantForContainedProperty(const FProperty& InProperty) const override;
	virtual TOptional<FString> GetValueForProperty(FProperty& Property) const override;
	virtual void SetValueForProperty(const FProperty& Property, const FString& Value) override;
};



/** Metadata related to color properties. Currently only allows you to hide the alpha channel. */
UCLASS(meta=(DisplayName = "Color"))
class UBPE_MetadataCollection_Color : public UBPE_MetadataCollectionStruct
{
	GENERATED_BODY()

public:
	UBPE_MetadataCollection_Color();

	// Whether to hide the alpha channel from the color picker.
	UPROPERTY(EditAnywhere)
	bool HideAlphaChannel;
};



/** Controls the format of the header row on array elements. */
UCLASS(meta=(DisplayName = "Title Property", Group = "Array"))
class UBPE_MetadataCollection_TitleProperty : public UBPE_MetadataCollection
{
	GENERATED_BODY()

public:
	// Determines the format of the header on each array element.
	// You may specify a single property like this: "SomePropertyInStruct".
	// You may also specify a Text-like formatting: "{SomePropertyInStruct} - {SomeOtherPropertyInStruct}".
	UPROPERTY(EditAnywhere)
	FString TitleProperty;

protected:
	virtual bool IsRelevantForProperty(const FProperty& InProperty) const override;
	virtual bool IsRelevantForContainedProperty(const FProperty& InProperty) const override;
};



/** Exposes the possibility to specify a list of strings as an option to String or Name variables. */
UCLASS(meta=(DisplayName = "Get Options", Group = "General"))
class UBPE_MetadataCollection_GetOptions : public UBPE_MetadataCollectionStruct
{
	GENERATED_BODY()

public:
	// The name of a function that produces the array of Strings/Names to be used as options.
	//
	// There are a few caveats:
	// * The function can be either a function inside the Blueprint, or a static C++ function.
	// * The function CANNOT be defined in a BlueprintFunctionLibrary asset. C++ BlueprintFunctionLibraries are still okay.
	// * The function needs to return a single array of Strings or Names.
	// * The name of the output should be called "ReturnValue".
	// * The function may not take any input parameters.
	UPROPERTY(EditAnywhere)
	FString GetOptions;

protected:
	TOptional<FString> ValidateOptionsFunction(const FString& FunctionName) const;
	virtual TOptional<FString> GetValueForProperty(FProperty& Property) const override;;
	virtual bool IsRelevantForContainedProperty(const FProperty& InProperty) const override;
};