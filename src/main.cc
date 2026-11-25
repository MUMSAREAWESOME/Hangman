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
#include <chrono>

std::vector<std::string> words {
    "despotically", "esperance", "hematin", "compunctions", "subtitled", "chasteners", "stavudines", "tightest", "leviathans", "scanted",
    "unintentionally", "roupy", "increase", "rechargeable", "codetermination", "upslope", "accredit", "keratotic", "facetiousness", "nonrecombinants",
    "phylacteries", "maraschino", "liquoring", "improvised", "hoover", "attitudinises", "asymmetrically", "upcurls", "overfavor", "armonicas",
    "despair", "vigilances", "henequens", "mitigate", "loyalties", "oping", "counterassaults", "cholestatic", "househusband", "mezquit",
    "upspringing", "national", "wordsmitheries", "derbies", "noviciate", "enfilade", "proceedings", "livery", "fabricated", "overliteral",
    "blanker", "bobolinks", "unsliced", "eloin", "winds", "complains", "mandrills", "skysurfing", "overelaborated", "highlands",
    "offstages", "overgovern", "outpopulating", "far", "stetting", "outserve", "grand", "kanzu", "brainchildren", "septicity",
    "troops", "wispier", "pyrolysis", "cliffs", "postimperial", "metastases", "exilable", "sequencer", "airiness", "efflorescences",
    "guyed", "price", "diecious", "interstratified", "hyperboloidal", "intertillage", "stolports", "coolies", "kindlinesses", "reinless",
    "bakers", "shelters", "plop", "bestsellerdoms", "woenesses", "slender", "trotyl", "refloated", "antibioses", "stinkhorns",
    "fizziest", "eulogiae", "greasing", "vandyked", "berhymed", "anviltop", "juristic", "preheats", "kestrels", "suburban",
    "impaling", "cataloes", "overbold", "autobahn", "scopulae", "outdrops", "ventails", "preciser", "decolors", "niggling",
    "telluric", "hempseed", "rewarmed", "capuchin", "drudging", "nonmoney", "reimaged", "hyponyms", "upscales", "shunners",
    "nitrolic", "splashes", "lovesick", "windbags", "chamfron", "stratify", "vuggiest", "divining", "quirkish", "foreword",
    "refallen", "evictees", "aplastic", "abuttals", "siameses", "solarism", "gravidae", "plaister", "clanking", "plaiters",
    "outdrive", "tintings", "trapdoor", "jarosite", "waybills", "windflaw", "windlass", "mouthers", "detracts", "glamours",
    "apostate", "wessands", "squasher", "dystonia", "ruptured", "claimers", "ecraseur", "nonagons", "bedstead", "quixotes",
    "hospitia", "sandshoe", "benzoles", "sapphism", "sludgier", "acolytes", "quayages", "wingding", "emitters", "shipyard",
    "millwork", "quillets", "fourchee", "haywards", "climatic", "lanknesses", "zwiebacks", "remodeling", "inhabitations",
    "craftswoman", "secretin", "haloperidols", "unselectively", "hoariness", "violinist", "programmability", "dahabiehs",
    "antihistorical", "pencilled", "ascendencies", "calorimetries", "xerographically", "endamebae", "sapropels", "apparentness",
    "temporality", "anaerobically", "tatterdemalions", "fenlands", "pinnation", "oilstones", "forlorner", "urbanised",
    "oesophagi", "ostiaries", "actiniums", "clenchers", "varicosis", "equivoque", "amygdales", "chloritic", "terpineol",
    "fusionist", "barbicels", "runtiness", "demurring", "fogfruits", "acropetal", "swordlike", "estivator", "lobbyisms",
    "oxidising", "retinting", "tarriness", "vanaspati", "greegrees", "shellfire", "vibriosis", "tsaritzas", "selenosis",
    "extricate", "birthwort", "teaseller", "multicell", "barraters", "tamoxifen", "chapteral", "nectarean"
};

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
    std::cout << "Enter the letter you want to guess: ";
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
    std::vector<int> iter;
    for (int i = 0; i < word.size(); i++) {
        if (guess == word[i]) iter.push_back(i);
    }
    if (inVec(player.lettersUsed, guess)) {
        std::cout << "Already guessed this letter!\n";
        return;
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
        player.lettersUsed.push_back(guess);
        player.lives --;
    }
    player.guesses ++;
}

int main() {
    while (true) {
        Player player;
        std::string word {pickWord()};
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
                std::cout << "Lose!\n";
                break;
            }

            if (currStr == word) {
                std::cout << "Win!\n";
                break;
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::system("cls");

            std::cout << currStr << '\n' << "Lives: " << player.lives << '\n' << "Letters used: ";

            for (const char& i : player.lettersUsed) {
                std::cout << i << ' ';
            }

            std::cout << std::endl;

            guess(player, word, current);
        }

        std::cout << "The word was: " << word << '\n';

        std::cout << "Would you like to play again? [Y/n]: ";
        char choice;
        std::cin >> choice;
        choice = std::tolower(choice);
        if (choice == 'n') break;
    }
    return 0;
}