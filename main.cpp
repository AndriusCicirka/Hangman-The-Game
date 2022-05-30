#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string.h>
#include <string>
#include <conio.h>
#include <windows.h>
#include <time.h>

using namespace std;

class activeDisplay
{
protected:
    const int displayX = 81;
    const int displayY = 21;

    char activeDisplay[81][21];

public:
    void refreshActiveDisplay(int sleepTime, bool firstTimeLoading)
    {

        system("cls");
        for (int y = 1; y <= displayY; y++)
        {
            for (int x = 1; x <= displayX; x++)
            {
                if (y == 1 || y == displayY)
                    activeDisplay[x][y] = '#';
                else if (x == 1 || x == displayX)
                    activeDisplay[x][y] = '#';
                else if (firstTimeLoading == true && x == 30)
                    activeDisplay[x][y] = '#';
                else if (firstTimeLoading == true && x > 30 && y == 13)
                    activeDisplay[x][y] = '#';
                else if (firstTimeLoading == true)
                    activeDisplay[x][y] = ' ';

                if (!firstTimeLoading)
                    cout << activeDisplay[x][y];
            }

            if (!firstTimeLoading)
            {
                Sleep(sleepTime);
                cout << "\n";
            }
        }
    }
};

class gameLogic : public activeDisplay
{
private:
    map<int, string> words;
    vector<char> allowedLetters;

    int n, wordLenght, lettersLeft, failedAttCount = 0;
    string wordToGuess, line;
    char wordDisplay[32] = {'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
                            '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_'};

public:
    gameLogic() : n(0), wordLenght(0), lettersLeft(0), failedAttCount(0), wordToGuess(), line() {}
    gameLogic(int x, int y, int z, int w, string l, string m) : n(x), wordLenght(y), lettersLeft(z), failedAttCount(w), wordToGuess(l), line(m) {}
    ~gameLogic() {}

    void displayStatic(string fileName, int sleepTime)
    {
        ifstream read;
        read.open(fileName);

        system("cls");
        while (getline(read, line))
        {
            Sleep(sleepTime);
            cout << line << "\n";
        }
        read.close();
    }

    void operator++(int)
    {
        system("chcp 65001");
        system("color 02");
        system("cls");

        // Preload Words
        ifstream read;

        read.open("WordList.txt");
        read >> n;

        string readWord;
        for (int i = 1; i <= n; i++)
        {
            read >> readWord;
            words[i] = readWord;
        }
        read.close();

        char tempC;
        read.open("legalletters.txt");
        while (!read.eof())
        {
            read >> tempC;
            allowedLetters.push_back(tempC);
        }
        read.close();

        // Loading Menu
        displayStatic("LoadingMenu.txt", 0);
        Sleep(3000);
        displayStatic("LoadingMenuContinue.txt", 0);

        while (!kbhit())
        {
            if (getch() == 27)
                exit(0);
            break;
        }
        displayStatic("RuleMenu.txt", 200);

        while (!kbhit())
        {
            if (getch() == 27)
                exit(0);
            break;
        }
        displayStatic("AllowedLetterList.txt", 0);

        while (!kbhit())
        {
            if (getch() == 27)
                exit(0);
            break;
        }
    }

    void checkForErrors()
    {
        try
        {
            if (words.size() == 0)
                throw "WORD-LIST";
        }
        catch (string err)
        {
            system("cls");
            cout << "Error: " << err;
            Sleep(5000);
            exit(1);
        }

        try
        {
            if (allowedLetters.size() == 0)
                throw "LETTER-LIST";
        }
        catch (string err)
        {
            system("cls");
            cout << "Error: " << err;
            Sleep(5000);
            exit(1);
        }
    }

    void randomWordSelector()
    {
        int temp;
        srand(time(0));
        temp = (rand() % n) + 1;

        wordToGuess = words[temp];
        wordLenght = wordToGuess.size();
        lettersLeft = wordLenght;

        for (int i = 0; i < wordLenght; i++)
            wordToGuess[i] = toupper(wordToGuess[i]);
    }

    void updateDisplayInfo(int attCount)
    {
        for (int y = 1; y <= displayY; y++)
        {
            for (int x = 1; x <= displayX; x++)
            {
                if (y == 17 && x >= 50 && x < 50 + wordLenght)
                {
                    activeDisplay[x][y] = wordDisplay[x - 50];
                }
            }
        }

        switch (attCount)
        {
        case 1:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (y == 3)
                    {
                        if (x > 6 && x <= 19)
                            activeDisplay[x][y] = '_';
                    }
                    if (y == 4)
                    {
                        if (x == 6)
                            activeDisplay[x][y] = '|';
                        if (x == 7)
                            activeDisplay[x][y] = '_';
                        else if (x > 7 && x <= 19)
                            activeDisplay[x][y] = '_';
                        if (x == 20)
                            activeDisplay[x][y] = '|';
                    }
                    if (y > 4 && y < 18)
                    {
                        if (y == 5 && x == 10 || y == 5 && x == 12)
                            activeDisplay[x][y] = '/';
                        if (y == 6 && x == 9 || y == 6 && x == 11)
                            activeDisplay[x][y] = '/';
                        if (y == 7 && x == 10)
                            activeDisplay[x][y] = '/';
                        if (y == 8 && x == 9)
                            activeDisplay[x][y] = '/';
                        if (x == 6 || x == 8)
                            activeDisplay[x][y] = '|';
                        if (x == 7)
                            activeDisplay[x][y] = '_';
                        if (y == 14 && x > 8 && x < 20)
                            activeDisplay[x][y] = '_';
                    }
                }
            }

            break;
        case 2:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (y == 6 || y == 7)
                    {
                        if (x >= 15 && x <= 17)
                            activeDisplay[x][y] = '#';
                    }
                }
            }

            break;
        case 3:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (y == 8 && x == 16)
                        activeDisplay[x][y] = '|';
                    else if (y > 8 && y <= 11)
                    {
                        if (x >= 15 && x <= 17)
                            activeDisplay[x][y] = '#';
                    }
                }
            }

            break;
        case 4:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (y == 9 && x == 14)
                        activeDisplay[x][y] = '#';
                    else if (y == 10 || y == 11)
                    {
                        if (x == 13)
                            activeDisplay[x][y] = '#';
                    }
                }
            }

            break;
        case 5:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (y == 9 && x == 18)
                        activeDisplay[x][y] = '#';
                    else if (y == 10 || y == 11)
                    {
                        if (x == 19)
                            activeDisplay[x][y] = '#';
                    }
                }
            }

            break;
        case 6:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (x == 15 && y >= 12 && y <= 14)
                        activeDisplay[x][y] = '#';
                }
            }

            break;
        case 7:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (x == 17 && y >= 12 && y <= 14)
                        activeDisplay[x][y] = '#';
                }
            }

            break;
        case 8:
            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (x == 16)
                    {
                        if (y == 5)
                            activeDisplay[x][y] = '|';
                        if (y == 8)
                            activeDisplay[x][y] = '=';
                    }
                    if (y == 14)
                    {
                        if (x > 8 && x < 15 || x == 16 || x > 17 && x < 20)
                            activeDisplay[x][y] = ' ';
                    }
                }
            }

            break;
        case 9:

            for (int y = 1; y <= displayY; y++)
            {
                for (int x = 1; x <= displayX; x++)
                {
                    if (y == 1 || y == displayY)
                        activeDisplay[x][y] = '#';
                    else if (x == 1 || x == displayX)
                        activeDisplay[x][y] = '#';
                    else
                        activeDisplay[x][y] = ' ';
                }
            }

            for (int i = 36; i <= 36 + wordLenght; i++)
                activeDisplay[i][10] = wordToGuess[i - 36];

            refreshActiveDisplay(0, false);
            Sleep(3000);
            displayStatic("GameOver.txt", 0);

            break;
        case 10:

            Sleep(3000);
            displayStatic("GameWon.txt", 0);
        }
    }

    int returnFailledAtt()
    {
        return failedAttCount;
    }

    int returnLettersLeft()
    {
        return lettersLeft;
    }

    void playerGuess(char guess)
    {
        bool failedAttempt = true;
        guess = toupper(guess);
        int nonemptyGuess = 0;

        for (int j = 0; j < allowedLetters.size(); j++)
        {
            if (guess == allowedLetters[j])
            {
                allowedLetters.erase(allowedLetters.begin() + j);
                for (int i = 0; i < wordLenght; i++)
                {
                    if (wordToGuess[i] == guess)
                    {
                        wordDisplay[i] = guess;
                        failedAttempt = false;
                        lettersLeft--;
                    }
                }

                nonemptyGuess++;
                j = allowedLetters.size();
            }
        }

        if (nonemptyGuess == 0)
            failedAttempt = false;
        if (failedAttempt == true)
            failedAttCount++;

        cout << "Debug: " << guess << " | " << failedAttCount << " | " << wordToGuess;
    }

    int overMenu()
    {
        char menuSelection;
        bool selected = false;

        while (!selected)
        {
            FlushConsoleInputBuffer;
            kbhit();
            menuSelection = getch();
            cout << menuSelection;

            if (menuSelection == 27)
            {
                selected = true;
                exit(1);
            }
            else if (menuSelection == 13)
            {
                selected = true;
                return 1;
            }
        }
    }
};

int main()
{
    gameLogic action;

    action++;
    action.checkForErrors();
    action.randomWordSelector();
    action.refreshActiveDisplay(250, true);
    action.updateDisplayInfo(0);
    action.refreshActiveDisplay(0, false);

    char guess;
    while (true)
    {
        while (!kbhit()) Sleep(50);

        guess = getch();
        if (guess == 27)
            exit(0);

        action.playerGuess(guess);
        action.updateDisplayInfo(action.returnFailledAtt());
        action.refreshActiveDisplay(0, false);

        if (action.returnLettersLeft() <= 0)
        {
            // GAME WON
            Sleep(1000);
            action.updateDisplayInfo(10);

            int temp = 0;
            temp = action.overMenu();
            if (temp == 1) // restart;
        }

        if (action.returnFailledAtt() > 7)
        {
            // GAME LOST
            Sleep(1000);
            action.updateDisplayInfo(9);

            int temp = 0;
            temp = action.overMenu();
            if (temp == 1) // restart;
        }
    }

    return 0;
}