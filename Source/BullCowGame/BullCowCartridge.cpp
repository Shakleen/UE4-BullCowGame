// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include <array>

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    LoadWordsFromDisk();
    SetupGame();
}

void UBullCowCartridge::LoadWordsFromDisk()
{
    const FString wordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(
        IsogramWordList,
        *wordListPath,
        [&](const FString& word) {
            if (HasInvalidLength(word))
                return false;
            if (!IsIsogram(word))
                return false;
            return true;
        }
    );

    //PrintLine(FString::Printf(TEXT("%d words loaded"), IsogramWordList.Num()));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }
    
    ProcessGuess(Input);
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = GetRandomWord();
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull-Cow Game!"));
    PrintLine(FString::Printf(TEXT("Can you guess a %d letter word in %d tries?"), HiddenWord.Len(), Lives));
    //PrintLine(FString::Printf(TEXT("The Hidden Word: %s"), *HiddenWord));
}

inline FString& UBullCowCartridge::GetRandomWord()
{
    return IsogramWordList[FMath::RandRange(0, IsogramWordList.Num() - 1)];
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to continue"));
}

void UBullCowCartridge::ProcessGuess(const FString& word)
{
    if (!IsIsogram(word))
    {
        PrintLine(TEXT("Your guess isn't an Isogram!"));
        PrintLine(TEXT("Remember the hidden word is an isogram."));
        return;
    }

    if (word == HiddenWord)
    {
        PrintLine(TEXT("You guessed correctly! Congratulations!"));
        EndGame();
        return;
    }

    HandleWrongGuess(word);
}

void UBullCowCartridge::HandleWrongGuess(const FString& word)
{
    PrintLine(TEXT("Oh NO! Your guess was wrong!"));

    if (word.Len() != HiddenWord.Len())
        PrintLine(FString::Printf(TEXT("The word has %d letters"), HiddenWord.Len()));

    const BullCowCount count = GetBullCows(word);
    PrintLine(FString::Printf(TEXT("You got %d bulls & %d cows"), count.bulls, count.cows));

    DecrementLives();
}

void UBullCowCartridge::DecrementLives()
{
    Lives--;

    if (Lives > 0)
        PrintLine(FString::Printf(TEXT("You have %d tries left"), Lives));
    else
    {
        PrintLine(TEXT("You Lost!"));
        PrintLine(FString::Printf(TEXT("The word was %s"), *HiddenWord));
        EndGame();
    }
}

inline bool UBullCowCartridge::HasInvalidLength(const FString& word) const
{
    return word.Len() < 4 || word.Len() > 6;
}

const UBullCowCartridge::BullCowCount UBullCowCartridge::GetBullCows(const FString& word) const
{
    BullCowCount count;

    for (int i = 0; i < word.Len(); i++)
    {
        if (word[i] == HiddenWord[i])
        {
            count.bulls++;
            continue;
        }

        for (int j = 0; j < HiddenWord.Len(); j++)
        {
            if (word[i] == HiddenWord[j] && i != j)
            {
                count.cows++;
                break;
            }
        }
    }

    return count;
}

bool UBullCowCartridge::IsIsogram(const FString& word) const
{
    std::array<bool, 27> visited{};

    for (unsigned short int i = 0; i < visited.size(); i++)
        visited[i] = false;

    for (unsigned short int i = 0; i < word.Len(); ++i)
    {
        if (visited[word[i] - 'a'])
            return false;

        visited[word[i] - 'a'] = true;
    }

    return true;
}
