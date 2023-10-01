
#include "LeaderboardClass.h"
using namespace std;


// Constructor of Leaderboard Class that initializes the leaderboard by loading entries from a file
LeaderboardClass::LeaderboardClass(const string& filename) {
    LoadFromFile(filename);
}

int LeaderboardClass::TimeToSeconds(const string& timeStr) {

    // Finds the colon's position in the time string and extracts toe minutes and seconds to convert them to integers
    size_t colonPos = timeStr.find(":");
    int minutes = stoi(timeStr.substr(0, colonPos));
    int seconds = stoi(timeStr.substr(colonPos + 1));

    // Converts the minutes to seconds and adds the extra seconds
    return minutes * 60 + seconds;
}

// Adds a new entry to the leaderboard and sorts it appropriately
void LeaderboardClass::AddEntry(const string& playerName, string time) {


    Entry newEntry = { playerName, time, false };
    int totalSeconds = TimeToSeconds(time);
    vector<Entry>::iterator it = _entries.end();
    for (vector<Entry>::iterator iter = _entries.begin(); iter != _entries.end(); iter++) {
        if (TimeToSeconds(iter->_time) > TimeToSeconds(newEntry._time)) {
            it = iter;
            break;
        }
    }
    // Checks if the new entry is faster than the previous entries
    if (it != _entries.end() && TimeToSeconds(it->_time) > TimeToSeconds(newEntry._time)) {
        newEntry._isNew = true;
        _entries.insert(it, newEntry);
    }

    // Keeps only the top 5 entries
    if (_entries.size() > 5) {
        _entries.pop_back();
    }
}

void LeaderboardClass::Display(sf::RenderWindow& window) {

    // Sets up the leaderboard window
    sf::RenderWindow leaderboardWindow(sf::VideoMode(window.getSize().x / 2, window.getSize().y / 2), "Leaderboard", sf::Style::Titlebar | sf::Style::Close);
    leaderboardWindow.setPosition(window.getPosition() + sf::Vector2i(window.getSize().x / 4, window.getSize().y / 4));
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        throw runtime_error("Failed to load font");
    }
    // Draws the title
    sf::Text title("LEADERBOARD", font, 20);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    SetText(title, leaderboardWindow.getSize().x / 2.0f, leaderboardWindow.getSize().y / 2.0f - 120);

    // Draws the leaderboard entries
    string leaderboardContent = "";
    int entryNumber = 1;
    for (vector<Entry>::const_iterator iter = _entries.begin(); iter != _entries.end(); ++iter) {
        const Entry& entry = *iter;
        leaderboardContent += to_string(entryNumber) + ".\t" + entry._time + "\t" + entry._name;
        if (entry._isNew) {
            leaderboardContent += " *";
        }
        leaderboardContent += "\n\n";
        entryNumber++;
    }
    sf::Text content(leaderboardContent, font, 18);
    content.setStyle(sf::Text::Bold);
    content.setFillColor(sf::Color::White);
    SetText(content, leaderboardWindow.getSize().x / 2.0f, leaderboardWindow.getSize().y / 2.0f + 20);
    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboardWindow.close();
            }
        }
        leaderboardWindow.clear(sf::Color::Blue);
        leaderboardWindow.draw(title);
        leaderboardWindow.draw(content);
        leaderboardWindow.display();
    }
}

void LeaderboardClass::SetText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void LeaderboardClass::SaveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open leaderboard file");
    }
    for (vector<Entry>::const_iterator iter = _entries.begin(); iter != _entries.end(); ++iter) {
        const Entry& entry = *iter;
        file << entry._time << "," << entry._name << endl;
    }
}

void LeaderboardClass::LoadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open leaderboard file");
    }
    string line;
    while (getline(file, line)) {
        size_t commaPos = line.find(",");
        if (commaPos == string::npos) {
            continue;
        }
        string time = line.substr(0, commaPos);
        string name = line.substr(commaPos + 1);
        _entries.push_back({ name, time, false });
    }
    vector<Entry>::iterator i, j;
    for (i = _entries.begin(); i != _entries.end(); i++) {
        for (j = i + 1; j != _entries.end(); j++) {
            if (j->_time < i->_time) {
                swap(*i, *j);
            }
        }
    }
}

bool LeaderboardClass::LeaderboardAfterWon(sf::RenderWindow& window, LeaderboardClass& leaderboard, const string& playerName, string time) {
    if (!playerName.empty() && time >= "") {
        leaderboard.AddEntry(playerName, time);
        leaderboard.SaveToFile("files/leaderboard.txt");
    }
    leaderboard.Display(window);
    window.display();
    return true;
}

bool LeaderboardClass::DisplayLeaderBoardOnly(sf::RenderWindow& window, LeaderboardClass& leaderboard, const string& playerName, string time) {
    leaderboard.Display(window);
    window.display();
    return true;
}