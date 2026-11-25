#include <cctype>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>
#include <string>
#include <random>
#include <limits>

std::vector<std::string> words {"cat", "panda", "fish", "dog", "fox", "eagle", "bear", "shark", "cout", "print", "header", "input", "C", "python", "java"};

struct Player {
    int lives {10};
    int guesses {0};
    std::vector<char> lettersUsed;
};

bool inVec(const std::vector<char>& vec, const char& target) {
    for (const char i : vec) {
        if (i == target) return true;
    }
    return false;
}

int randomNumberRange(int a, int b){
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_int_distribution<> distr(a, b); // define the range

  return distr(gen);
}

std::string pickWord() {
    int it {randomNumberRange(0, words.size()-1)};
    std::string word {words[it]};
    return word;
}

void guess(Player& player, const std::string& word, std::vector<char>& current) {
    char guess;
    while (true){
        try {
            std::cin >> guess;
            if (std::cin.fail()) throw std::runtime_error("Not a char!\n");
        }
        catch (const std::exception& e) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // From Grok
            std::cout << e.what();
            continue;
        }
        break;
    }
    guess = std::tolower(guess);
    player.guesses ++;
    bool guessed {inVec(current, guess)};
    std::vector<int> iter;
    for (int i = 0; i < word.size(); i++) {
        if (guess == word[i]) iter.push_back(i);
    }
    if (inVec(player.lettersUsed, guess)) {
        std::cout << "Already guessed this letter!\n";
    }
    else if (!iter.empty()) {
        std::cout << "Correct letter!\n";
        player.lettersUsed.push_back(guess);
        for (int i : iter) {
            current[i] = guess;
        }
    }
    else {
        std::cout << "Wrong guess!\n";
        player.lives --;
    }
}

int main() {
    Player player;
    const std::string& word {pickWord()};
    std::vector<char> current(word.size());
    for (int i = 0; i < current.size(); i ++) {
        current[i] = '_';
    }
    while (true) {
        std::string currStr;
        for (int i = 0; i < current.size(); i ++) {
            currStr += current[i];
        }

        if (player.lives == 0) {
            std::cout << "Loose!\n";
            break;
        }

        if (currStr == word) {
            std::cout << "Win!\n";
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::system("cls");

        std::cout << currStr << '\n' << "Lives: " << player.lives << '\n';

        guess(player, word, current);
    }

    std::cout << "The word was: " << word << '\n';

    return 0;
}