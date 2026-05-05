#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <ctime>
using namespace std;

const int NUM_SNAKES = 5;
const int NUM_LADDERS = 5;

class Snake
{
    int m_head;
    int m_tail;

    public:
        Snake(int head, int tail) : m_head(head), m_tail(tail) {}
        int getHead() const { return m_head; }
        int getTail() const { return m_tail; }
};

class Ladder
{
    int m_bottom;
    int m_top;

    public:
        Ladder(int bottom, int top) : m_bottom(bottom), m_top(top) {}
        int getBottom() const { return m_bottom; }
        int getTop() const { return m_top; }
};

class Player {
    public:
        Player() {}
};

class Dice {

    public:
        Dice() {}
        int roll() {
            return rand() % 6 + 1; // Simulate rolling a six-sided die
        }
};

class Board
{
    vector<int> m_vecBoard;
    vector<Snake> m_vecSnakes;
    vector<Ladder> m_vecLadders;

public:
    Board(): m_vecBoard(100)
    {
        iota(m_vecBoard.begin(), m_vecBoard.end(), 1); // fills with 1, 2, 3, ..., 100
         
        // Initialize snakes
        for (int i = 0; i < NUM_SNAKES; ++i) {
            int head = rand() % 90 + 10; // Snake head between 10 and 99
            int tail = rand() % (head - 1) + 1; // Snake tail between 1 and head-1
            m_vecSnakes.emplace_back(head, tail);
        }

        // Initialize ladders
        for (int i = 0; i < NUM_LADDERS; ++i) {
            int bottom = rand() % 90 + 1; // Ladder bottom between 1 and 90
            int top = rand() % (100 - bottom) + bottom + 1; // Ladder top between bottom+1 and 100

            if(m_vecSnakes.end() != find_if(m_vecSnakes.begin(), m_vecSnakes.end(), [bottom, top](const Snake& snake) {
                return (snake.getHead() == bottom && snake.getTail() == top) || (snake.getHead() == top && snake.getTail() == bottom);
            })) {
                --i; // If ladder conflicts with a snake, retry
                continue;
            }
            m_vecLadders.emplace_back(bottom, top);
        }

        // Debug output to verify snakes and ladders
        cout << "Snakes:" << endl;
        for(const auto& snake : m_vecSnakes) {
            cout << "Head: " << snake.getHead() << ", Tail: " << snake.getTail() << endl;
        }
        cout << "Ladders:" << endl;
        for(const auto& ladder : m_vecLadders) {
            cout << "Bottom: " << ladder.getBottom() << ", Top: " << ladder.getTop() << endl;
        }
    }

    void updatePositions(int& newPosition) {
        if(newPosition > 100) {
                newPosition = 100 - (newPosition - 100); // Bounce back if overshooting
            }

            // check for snakes or ladders
            auto snakeIt = find_if(m_vecSnakes.begin(), m_vecSnakes.end(), [pos=newPosition](const Snake& snake) {
                return snake.getHead() == pos;
            });
            
            if(snakeIt != m_vecSnakes.end()) {
                newPosition = snakeIt->getTail(); // Move down the snake
            } else {
                auto ladderIt = find_if(m_vecLadders.begin(), m_vecLadders.end(), [pos=newPosition](const Ladder& ladder) {
                    return ladder.getBottom() == pos;
                });
                if(ladderIt != m_vecLadders.end()) {
                    newPosition = ladderIt->getTop(); // Move up the ladder
                }
            }
    }
};

class SnakeLadderGame
{
private:
    shared_ptr<Board> m_board;
    Dice m_dice;
    vector<Player> m_vecPlayers;

public:
    SnakeLadderGame(int numPlayers) {
        m_board = make_shared<Board>();
        m_vecPlayers.resize(numPlayers);
    }

    void play() {
        // Game logic to be implemented
        int playerIndex = 0;
        vector<int> playerPositions(m_vecPlayers.size(), 0); // Track player positions

        while(1)
        {
            // roll the dice
            int roll = m_dice.roll();

            cout<< "Player " << playerIndex + 1 << " rolls a " << roll << endl;

            // move the player
            playerPositions[playerIndex] += roll;
            m_board->updatePositions(playerPositions[playerIndex]);
            
            // Check for win condition
            if(playerPositions[playerIndex] == 100) {
                cout << "Player " << playerIndex + 1 << " wins!" << endl;
                break;
            }

            playerIndex++;
            if(playerIndex >= m_vecPlayers.size()) {
                playerIndex = 0; // Loop back to the first player
        }
        }
    }
};


int main()
{
    srand(time(0)); // Seed RNG so results differ each run
    SnakeLadderGame game(2); // Example with 2 players
    game.play();
    return 0;
}