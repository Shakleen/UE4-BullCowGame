// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

private:
	TArray<FString> IsogramWordList;
	FString HiddenWord;
	int32 Lives;
	bool bGameOver;

	struct BullCowCount
	{
		int8 bulls = 0;
		int8 cows = 0;
	};

private:
	void SetupGame();
	void EndGame();
	FString& GetRandomWord();
	bool IsIsogram(const FString &word) const;
	void ProcessGuess(const FString& word);
	void HandleWrongGuess(const FString& word);
	void DecrementLives();
	void LoadWordsFromDisk();
	bool HasInvalidLength(const FString& word) const;
	const BullCowCount GetBullCows(const FString& word) const;
};
