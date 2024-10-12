#include <ncurses.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>


//TODO inputting capital letters
//TODO bigger dictionary

class Limb {
public:
    Limb(int row, int col, char glyph);
    int row;
    int col;
    char glyph;
};
Limb::Limb(int row, int col, char glyph) {
    this->row = row;
    this->col = col;
    this->glyph = glyph;
}

class Game {
public: 
    Game();
    ~Game();

    void setup();
    bool isOver();
    bool wordIsRevealed();
    void alertDeath();
    void alertWin();

    int randomline(std::string filename);
    void setupWord();
    void tryMatch(char ch);
    void renewPossibleGuesses();

    void redrawMain();
    void drawGallows();
    void drawRevealedCharacters();
    void drawPossibleGuesses();
    void drawHangedLimbs();
    void drawUsage();
    void showHangings();

    std::string guessword;
    std::string revealedCharacters;
    std::map<char, char> possibleGuesses;

    int currentlyHangedLimbs;
    std::vector<std::string> hangings;
    std::vector<Limb*> limbs;

    bool done;

    WINDOW* mainContainerWindow;
    WINDOW* playWindow;
    WINDOW* pastHangingsWindow;

    WINDOW* guesswordWindow;
    WINDOW* letterBankWindow;
    WINDOW* limbsWindow;
    WINDOW* usageWindow;
    WINDOW* alertWindow;
};

Game::Game() {
    this->done = false;
    this->limbs.push_back(new Limb(1, 3, 'o')); //head
    this->limbs.push_back(new Limb(2, 3, '|')); //torso
    this->limbs.push_back(new Limb(3, 2, '/')); //leg
    this->limbs.push_back(new Limb(3, 4, '\\'));//leg
    this->limbs.push_back(new Limb(2, 2, '/')); //arm
    this->limbs.push_back(new Limb(2, 4, '\\'));//arm
}

Game::~Game() {
    for (int i = this->limbs.size() - 1; i >= 0; i--) {
        delete(this->limbs[i]);
    }
}

void Game::setup() {
    this->revealedCharacters = "";
    this->currentlyHangedLimbs = 0;
    this->setupWord();
    this->renewPossibleGuesses();
}

void Game::redrawMain() {
    clear();
    this->drawRevealedCharacters();
    this->drawPossibleGuesses();
    this->drawGallows();
    this->drawHangedLimbs();
    this->drawUsage();
}

void Game::showHangings() {
    clear();
    int maxrow, maxcol;
    getmaxyx(pastHangingsWindow, maxrow, maxcol);

    mvwprintw(pastHangingsWindow, 0, 0, "HANGINGS                (Any key to return)");
    mvwprintw(pastHangingsWindow, 1, 0, "--------");

    for (int i = 0; i < maxrow && i < this->hangings.size(); i++) {
        mvwprintw(pastHangingsWindow, 2+i, 0, "%s", (hangings.at(i)).c_str());
    }

    wrefresh(pastHangingsWindow);
    int ch = getch();
    if (ch == KEY_F(6)) {
        this->done = true;
    }
}


void Game::tryMatch(char ch) {
    if (possibleGuesses[ch] == ' ') {
        return;
    }

    possibleGuesses[ch] = ' ';

    bool found = false;

    for (int i = 0; i < this->guessword.length(); i++) {
        if (toupper(ch) == toupper(guessword[i])) {
            revealedCharacters += toupper(ch);
            found = true;
        }
    }

    if (!found) {
        this->currentlyHangedLimbs++;
    }

}

bool Game::wordIsRevealed() {
    for (int i = 0; i < guessword.length(); i++) {
        if (std::string::npos == revealedCharacters.find(toupper(guessword.at(i)))) {
            return false;
        }
    }
    return true;
}

int Game::randomline(std::string filename) {
    int numberOfLines = 0;
    std::ifstream fileInput(filename.c_str());
    std::string ignore;
    while (std::getline(fileInput, ignore)) {
        numberOfLines++;
    }

    srand(time(NULL));

    return rand() % numberOfLines;
}

void Game::setupWord() {
    std::string filename = "./hmwords.txt";
    int randompos = this->randomline(filename);

    std::ifstream fileInput(filename.c_str());
    std::string ignore;

    for (int i = 0; i < randompos; i++) {
        std::getline(fileInput, ignore);
    }
    std::getline(fileInput, this->guessword);
}

bool Game::isOver() {
    return (this->currentlyHangedLimbs > 5);
}

void Game::alertDeath() {
    int ch = -1;
    mvwprintw(alertWindow, 0, 0, "The effigy is hanged.");
    mvwprintw(alertWindow, 1, 0, "Push enter.");
    refresh();
    while(ch != '\n') {
        ch = getch();
        if (ch == KEY_F(6)) {
            this->done = true;
            return;
        }
        if (ch == KEY_F(2)) {
            this->showHangings();
            return;
        }
    }
}

void Game::alertWin() {
    int ch = -1;
    mvwprintw(alertWindow, 0, 0, "The effigy is spared.");
    mvwprintw(alertWindow, 1, 0, "Push enter.");
    refresh();
    while(ch != '\n') {
        ch = getch();
        if (ch == KEY_F(6)) {
            this->done = true;
            return;
        }
        if (ch == KEY_F(2)) {
            this->showHangings();
            return;
        }
    }
}

void Game::renewPossibleGuesses() {
    possibleGuesses['a'] = 'A';
    possibleGuesses['b'] = 'B';
    possibleGuesses['c'] = 'C';
    possibleGuesses['d'] = 'D';
    possibleGuesses['e'] = 'E';
    possibleGuesses['f'] = 'F';
    possibleGuesses['g'] = 'G';
    possibleGuesses['h'] = 'H';
    possibleGuesses['i'] = 'I';
    possibleGuesses['j'] = 'J';
    possibleGuesses['k'] = 'K';
    possibleGuesses['l'] = 'L';
    possibleGuesses['m'] = 'M';
    possibleGuesses['n'] = 'N';
    possibleGuesses['o'] = 'O';
    possibleGuesses['p'] = 'P';
    possibleGuesses['q'] = 'Q';
    possibleGuesses['r'] = 'R';
    possibleGuesses['s'] = 'S';
    possibleGuesses['t'] = 'T';
    possibleGuesses['u'] = 'U';
    possibleGuesses['v'] = 'V';
    possibleGuesses['w'] = 'W';
    possibleGuesses['x'] = 'X';
    possibleGuesses['y'] = 'Y';
    possibleGuesses['z'] = 'Z';
}


void Game::drawRevealedCharacters() {
    int guessrow = 0;

    for (int i = 0; i < this->guessword.length(); i++) {
        if (std::string::npos != this->revealedCharacters.find(toupper(guessword[i]))) {
            mvwaddch(guesswordWindow, guessrow, i, guessword[i]);
        }
        else {
            mvwaddch(guesswordWindow, guessrow, i, '_');
        }
    }

    wrefresh(guesswordWindow);
}

void Game::drawPossibleGuesses() {
    int letterbankTop = 0;
    int letterbankLeft = 0;

    int row = letterbankTop;
    int col = letterbankLeft;

    mvwaddch(letterBankWindow, row, col++, possibleGuesses['a']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['b']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['c']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['d']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['e']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['f']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['g']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['h']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['i']);

    col = letterbankLeft;
    ++row;
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['j']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['k']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['l']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['m']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['n']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['o']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['p']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['q']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['r']);

    col = letterbankLeft;
    ++row;
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['s']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['t']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['u']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['v']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['w']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['x']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['y']);
    mvwaddch(letterBankWindow, row, col++, possibleGuesses['z']);

    wrefresh(letterBankWindow);
}

void Game::drawGallows() {
    mvwprintw(limbsWindow, 0, 0, "----");
    mvwprintw(limbsWindow, 1, 0, "|");
    mvwprintw(limbsWindow, 2, 0, "|");
    mvwprintw(limbsWindow, 3, 0, "|");
}

void Game::drawHangedLimbs() {
    for (int i = 0; i < this->currentlyHangedLimbs; i++) {
        mvwaddch(limbsWindow, (limbs.at(i))->row, (limbs.at(i))->col, (limbs.at(i))->glyph);
    }
    wrefresh(limbsWindow);
}

void Game::drawUsage() {
    mvwprintw(usageWindow, 0, 0, "(F2) Previous Hangings");
    mvwprintw(usageWindow, 1, 0, "(F6) Quit");
}


int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    Game* game = new Game();
    
    game->pastHangingsWindow = subwin(stdscr, LINES-2, COLS-2, 0, 0);

    game->guesswordWindow  = subwin(stdscr, 1, 14,  7,  3);
    game->limbsWindow      = subwin(stdscr, 5,  5,  1,  3);
    game->letterBankWindow = subwin(stdscr, 5, 14,  9,  3);
    game->usageWindow      = subwin(stdscr, 2, 22, 16,  3);
    game->alertWindow      = subwin(stdscr, 2, 21,  2, 11);

    game->setup();
    game->redrawMain();

    touchwin(stdscr);
    refresh();

    while (!(game->done)) {
        int ch = getch();

        switch(ch) {
            case KEY_F(2):
                game->showHangings();
            break;

            case KEY_F(6):
                game->done = true;
            break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                game->tryMatch(ch);
            break;

            default:
            break;
        }

        if (!(game->done)) {
            game->redrawMain();

            if (game->isOver()) {
                game->hangings.push_back(game->guessword);
                game->alertDeath();
                game->setup();
                game->redrawMain();
            }
        }

        if (game->wordIsRevealed()) {
            game->alertWin();
            game->setup();
            game->redrawMain();
        }

        refresh();
    } //done

    delwin(game->guesswordWindow);
    delwin(game->letterBankWindow);
    delwin(game->limbsWindow);
    delwin(game->usageWindow);
    delwin(game->alertWindow);
    delwin(game->pastHangingsWindow);

    delete(game);
    endwin();

    return 0;
}

