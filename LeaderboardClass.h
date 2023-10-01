
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
using std::string;
using std::vector;

class LeaderboardClass {
public:
    LeaderboardClass(const string& filename);
    void AddEntry(const string& playerName, string time);
    void Display(sf::RenderWindow& window);
    void SaveToFile(const string& filename);
    void SetText(sf::Text& text, float x, float y);
    bool LeaderboardAfterWon(sf::RenderWindow& window, LeaderboardClass& leaderboard, const string& playerName = "", string time = "");
    bool DisplayLeaderBoardOnly(sf::RenderWindow& window, LeaderboardClass& leaderboard, const string& playerName = "", string time = "");


// Keeps track of information of individual entries of new times and names
private:
    struct Entry {
        string _name;
        string _time;
        bool _isNew;
    };
    vector<Entry> _entries;
    int TimeToSeconds(const string& timeStr);
    void LoadFromFile(const string& filename);
};