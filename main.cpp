#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Board.h"
#include "TextureManager.h"
#include "Random.h"

using namespace std;

int main()
{
    /*========== Welcome to Minesweeper ==========*/
    Board board;
    board.configInitialize("../boards/config.cfg");
    int totalMines = board.getMineCount();

    // Load button and digits info from file--board images implemented in Board::Draw()
    sf::Sprite debug(TextureManager::GetTexture("debug"));
    sf::Sprite digits(TextureManager::GetTexture("digits"));
    sf::Sprite happyFace(TextureManager::GetTexture("face_happy"));
    sf::Sprite winFace(TextureManager::GetTexture("face_win"));
    sf::Sprite loseFace(TextureManager::GetTexture("face_lose"));
    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    sf::Sprite test3(TextureManager::GetTexture("test_3"));

    happyFace.setPosition((board.getColumns() * 32 / 2) - 32, (board.getRows() * 32));
    test1.setPosition((board.getColumns() * 32 / 2) + 160, (board.getRows() * 32));
    test2.setPosition((board.getColumns() * 32 / 2) + 224, (board.getRows() * 32));
    test3.setPosition((board.getColumns() * 32 / 2) + 288, (board.getRows() * 32));
    debug.setPosition((board.getColumns() * 32 / 2) + 96, (board.getRows() * 32));

    // Set up the window
    sf::RenderWindow window(sf::VideoMode(board.getColumns() * 32, board.getRows() * 32 + 100), "Minesweeper");

    while (window.isOpen())
    {
        // Event loop make Game class later to make this look cleaner
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosFloat(mousePos.x, mousePos.y);

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (happyFace.getGlobalBounds().contains(mousePosFloat))
                    {
                        cout << "Clicking on smiley face" << endl;
                        board.clear();
                        board.reset();
                        board.setFlagCount();
                        board.configInitialize("../boards/config.cfg");
                        board.setGameOver(false);
                        board.setGameWin(false);
                    }
                    else if (test1.getGlobalBounds().contains(mousePosFloat))
                    {
                        cout << "Test 1 Clicked" << endl;
                        board.clear();
                        board.reset();
                        board.setFlagCount();
                        board.initialize("../boards/testboard1.brd");
                        board.setGameOver(false);
                        board.setGameWin(false);
                    }
                    else if (test2.getGlobalBounds().contains(mousePosFloat))
                    {
                        cout << "Test 2 Clicked" << endl;
                        board.clear();
                        board.reset();
                        board.setFlagCount();
                        board.initialize("../boards/testboard2.brd");
                        board.setGameOver(false);
                        board.setGameWin(false);
                    }
                    else if (test3.getGlobalBounds().contains(mousePosFloat))
                    {
                        cout << "Test 3 Clicked" << endl;
                        board.clear();
                        board.reset();
                        board.setFlagCount();
                        board.initialize("../boards/testboard3.brd");
                        board.setGameOver(false);
                        board.setGameWin(false);
                    }
                    else if (debug.getGlobalBounds().contains(mousePosFloat))
                    {
                        cout << "Debug Clicked" << endl;
                        board.debug();
                    }
                    else if (!board.getGameOver())
                    {
                        // Reveal tiles only if game is not over
                        int row = mousePos.y / 32;
                        int column = mousePos.x / 32;
                        board.revealTile(row, column);
                        if (board.getGameOver())
                        {
                            board.setGameOver(true);
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (!happyFace.getGlobalBounds().contains(mousePosFloat))
                    {
                        // Handle tile flagging only if game is not over
                        if (!board.getGameOver())
                        {
                            int row = mousePos.y / 32;
                            int column = mousePos.x / 32;
                            board.flagTile(row, column);
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        // Draw buttons and mine counter - maybe make a Button class?
        if (board.checkWinCondition() and board.flagAllMines())
        {
            winFace.setPosition((board.getColumns() * 32 / 2) - 32, (board.getRows() * 32));
            window.draw(winFace);
            board.setGameOver(true);
        }
        else if (board.getGameOver())
        {
            loseFace.setPosition((board.getColumns() * 32 / 2) - 32, (board.getRows() * 32));
            window.draw(loseFace);
        }
        else
        {
            window.draw(happyFace);
        }

        // Define rectangles for each digit and hyphen (21x32 each)
        const int numDigits = 10;
        sf::IntRect digitRects[numDigits + 1];  // For negative sign at end of sprite sheet

        for (int i = 0; i < numDigits; i++) {
            digitRects[i] = sf::IntRect(i * 21, 0, 21, 32);
        }
        digitRects[numDigits] = sf::IntRect(numDigits * 21, 0, 21, 32); // Negative sign

        // Display the mineCount - flagCount
        int displayNumber = board.getMineCount() - board.getFlagCount();

        string numberStr = to_string(displayNumber);

        // Draw the digits or hyphen
        for (int i = 0; i < numberStr.size(); i++)
        {
            char digitChar = numberStr[i];
            int digitIndex = (digitChar == '-') ? numDigits : digitChar - '0';
            sf::Sprite minesRemaining(TextureManager::GetTexture("digits"), digitRects[digitIndex]);

            minesRemaining.setPosition(i * 21 + 30, (board.getRows() * 32));
            window.draw(minesRemaining);
        }

        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        window.draw(debug);

        // Draw game board after buttons are drawn!
        board.draw(window);
        window.display();
    }

    TextureManager::Clear();
    return 0;
}