#include "game.h"

#ifndef M_PI
#define M_PI (atan(1) * 4)
#endif

using namespace std;

// Constructor for Game:
// initializes highscore list and initial player object
Game::Game()
{
    highscores = new Highscores();
    nextShot = 0;
    nextAlien = 0;
    nextEnemy = 0;
    shotTimer = 50;
    untrackedShots = 0;
}

Player *Game::getPlayer(QString name)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i]->getPeerName() == name)
        {
            return players[i];
        }
    }
}

void Game::addAlien(int rotation)
{
    int startX, startY;
    switch(rotation)
    {
    case 90:
        startX = 740;
        startY = 5;
        break;
    case 180:
        startX = 740;
        startY = 527;
        break;
    case 270:
        startX = 5;
        startY = 527;
        break;
    default:
        startX = 5;
        startY = 5;
        break;
    }

    aliens.push_back(new Alien(startX, startY, nextAlien, rotation));
    nextAlien++;
}

void Game::addBoss() {
    bosses.emplace_back(new Boss());
}

void Game::addBoss(double X, double Y) {
    bosses.emplace_back(new Boss(X, Y));
}

// Wipes the current game and reinstantiates
// all the member variables
void Game::newGame()
{
    players.clear();
    aliens.clear();
    shots.clear();
    enemies.clear();
    bosses.clear();
    nextShot = 0;
    nextAlien = 0;
    nextEnemy = 0;
    untrackedShots = 0;
}

// Updates player & enemies, for use with a timer
void Game::updateField()
{
    for (size_t i = 0; i < players.size(); i++)
    {
        players[i]->move();
    }
    for (size_t i = 0; i < aliens.size(); i++)
    {
        aliens[i]->move();
        aliens[i]->shoot();
    }
    for (size_t i = 0; i < shots.size(); i++)
    {
        shots[i]->move();
    }
    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i]->move();
    }
}

vector<string> splitString(string input, char delim)
{
    vector<string> result;
    string line = "";
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == delim)
        {
            result.push_back(line);
            line = "";
        }
        else
        {
            line += input[i];
        }
    }
    result.push_back(line);
    return result;
}

vector<QString> splitQString(QString input, char delim)
{
    vector<QString> result;
    QString line = "";
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == delim)
        {
            result.push_back(line);
            line = "";
        }
        else
        {
            line += input[i];
        }
    }
    result.push_back(line);
    return result;
}

// Methods to save/load game.
// Reads file and instantiates member variables with the info
void Game::load()
{
    string line;
    ifstream myfile ("save.txt");
    if (myfile.is_open())
    {
        newGame();
        // Load the data
        getline(myfile, line);
        nextShot = stoi(line);
        getline(myfile, line);
        nextAlien = stoi(line);
        getline(myfile, line);
        nextEnemy = stoi(line);
        getline(myfile, line);
        shotTimer = stoi(line);
        while (getline(myfile, line))
        {
            switch (line[0])
            {
            case 'P':
            {
                line = line.substr(1, line.size() - 1);
                vector<string> data = splitString(line, ',');
                int x = stoi(data[0]);
                int y = stoi(data[1]);
                int rotation = stoi(data[2]);
                int speed = stoi(data[3]);
                int angle = stoi(data[4]);
                QString name = QString::fromStdString(data[5]);
                QString pixmapName = QString::fromStdString(data[6]);
                players.push_back(new Player(x, y, name));
                players[players.size() - 1]->setRot(rotation);
                players[players.size() - 1]->setSpeed(speed);
                players[players.size() - 1]->setAngle(angle);
                players[players.size() - 1]->setPixmapName(pixmapName);
                break;
            }
            case 'A':
            {
                line = line.substr(1, line.size() - 1);
                vector<string> data = splitString(line, ',');
                int x = stoi(data[0]);
                int y = stoi(data[1]);
                int id = stoi(data[2]);
                int rotation = stoi(data[3]);
                int timedShot = stoi(data[4]);
                aliens.push_back(new Alien(x, y, id, rotation));
                aliens[aliens.size() - 1]->setTimedShot(timedShot);
                break;
            }
            case 'S':
            {
                line = line.substr(1, line.size() - 1);
                vector<string> data = splitString(line, ',');
                int x = stoi(data[0]);
                int y = stoi(data[1]);
                int angle = stoi(data[2]);
                int id = stoi(data[3]);
                bool isAlienShot;
                if (data[4] == "1")
                {
                    isAlienShot = true;
                }
                else
                {
                    isAlienShot = false;
                }
                shots.push_back(new Shot(x, y, angle, id, isAlienShot));
                break;
            }
            case 'E':
            {
                line = line.substr(1, line.size() - 1);
                vector<string> data = splitString(line, ',');
                int x = stoi(data[0]);
                int y = stoi(data[1]);
                int dX = stoi(data[2]);
                int dY = stoi(data[3]);
                int id = stoi(data[4]);
                enemies.push_back(new Enemy(x, y, dX, dY, id));
                break;
            }
            }
        }
        myfile.close();
    }
    else
    {
        // Couldn't open file - maybe print an error message or debug message?
    }
}

// Writes game to file
void Game::save()
{
    ofstream myfile;
    myfile.open("save.txt");
    myfile << to_string(nextShot) << "\n";
    myfile << to_string(nextAlien) << "\n";
    myfile << to_string(nextEnemy) << "\n";
    myfile << to_string(shotTimer) << "\n";
    for (size_t i = 0; i < players.size(); i++)
    {
        myfile << players[i]->getSave();
    }
    for (size_t i = 0; i < aliens.size(); i++)
    {
        myfile << aliens[i]->getSave();
    }
    for (size_t i = 0; i < shots.size(); i++)
    {
        myfile << shots[i]->getSave();
    }
    for (size_t i = 0; i < enemies.size(); i++)
    {
        myfile << enemies[i]->getSave();
    }
    myfile.close();
}

Shot *Game::getShot(int id)
{
    for (size_t i = 0; i < shots.size(); i++)
    {
        if (shots[i]->getID() == id)
        {
            return shots[i];
        }
    }
    return nullptr;
}

Alien *Game::getAlien(int id)
{
    for (size_t i = 0; i < aliens.size(); i++)
    {
        if (aliens[i]->getID() == id)
        {
            return aliens[i];
        }
    }
    return nullptr;
}

Enemy *Game::getEnemy(int id)
{
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (enemies[i]->getID() == id)
        {
            return enemies[i];
        }
    }
    return nullptr;
}

void Game::deleteShot(int shotID)
{
    int index = -1;
    for (size_t i = 0; i < shots.size(); i++)
    {
        if (shots[i]->getID() == shotID)
        {
            index = i;
        }
    }
    if (index >= 0)
    {
        delete shots[index];
        shots.erase(shots.begin() + index);
    }
}

void Game::deleteAlien(int alienID)
{
    int index = -1;
    for (size_t i = 0; i < aliens.size(); i++)
    {
        if (aliens[i]->getID() == alienID)
        {
            index = i;
        }
    }
    if (index >= 0)
    {
        delete aliens[index];
        aliens.erase(aliens.begin() + index);
    }
}

void Game::deleteEnemy(int enemyID)
{
    int index = -1;
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (enemies[i]->getID() == enemyID)
        {
            index = i;
        }
    }
    if (index >= 0)
    {
        delete enemies[index];
        enemies.erase(enemies.begin() + index);
    }
}

void Game::deletePlayer(QString playerName)
{
    int index = -1;
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i]->getPeerName() == playerName)
        {
            index = i;
        }
    }
    if (index >= 0)
    {
        delete players[index];
        players.erase(players.begin() + index);
    }
}

// Delete any pointers
Game::~Game()
{
    delete highscores;
    for (size_t i = 0; i < players.size(); i++)
    {
        delete players[i];
    }
    for (size_t i = 0; i < aliens.size(); i++)
    {
        delete aliens[i];
    }
    for (size_t i = 0; i < shots.size(); i++)
    {
        delete shots[i];
    }
    for (size_t i = 0; i < enemies.size(); i++)
    {
        delete enemies[i];
    }
}

// Singleton implementation
Game *Game::instance_ = nullptr;
Game *Game::instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Game();
    }
    return instance_;
}

string Shot::getSave()
{
    string result = "S";
    result += to_string(x);
    result += ",";
    result += to_string(y);
    result += ",";
    result += to_string(angle);
    result += ",";
    result += to_string(id);
    result += ",";
    if (isAlienShot)
    {
        result += "1";
    }
    else
    {
        result += "0";
    }
    result += "\n";
    return result;
}

void Shot::move()
{
    double yInc;
    if (angle >= 90 && angle <= 270)
    {
        yInc = sqrt(30 + (30 * sin(angle * M_PI / 180)));
    }
    else
    {
        yInc = -1 * sqrt(30 - (30 * sin(angle * M_PI / 180)));
    }
    double xInc = 30 * sin(angle * M_PI / 180);
    x += xInc;
    y += yInc;
}

// Default constructor for Highscore object
// Loads the highscores from a file.
// If the file is not found, creates
// new highscore list.
Highscores::Highscores()
{

}

// Tests <score> to see if it's higher than
// the last score in the list. If so, it will
// be inserted at the proper spot and the last
// score in the list will be deleted.
// In case it's needed, the method will return
// whether the score was added to the list or
// not. If it's unused, change the return type
// to void.
bool Highscores::addScore(int score)
{
    return true;
}

// Load the highscores from a file.
// Normally called by the constructor itself,
// but can be forced when necessary.
void Highscores::load()
{

}

// Save the highscores to a file.
void Highscores::save()
{

}
