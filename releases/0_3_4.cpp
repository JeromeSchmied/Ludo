/* 2023-04-27
 * The GPLv3 License (GPLv3)

Copyright (c) 2023 Jero

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * so this is ludo / ki nevet a végén / Mensch, ärgere dich nicht!
 * this version includes a bug fix! which was pretty bad and also very obvious,
 * but no worries I fixed it. Although it would be nice to know whether it works
 * or just looks like it does. But that'd require many many tests which I don't
 * want to perform. I'd like to have some kind of smooth_out.
 */

// includes
#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

/* the following are UBUNTU/LINUX, and macOS ONLY terminal color codes,
 * but it works in Windows 10 too under VS2019.*/
constexpr auto RESET = "\033[0m";

constexpr auto RED = "\033[31m" /* Red */;
constexpr auto GREEN = "\033[32m" /* Green */;
constexpr auto YELLOW = "\033[33m" /* Yellow */;
constexpr auto BLUE = "\033[34m" /* Blue */;

constexpr auto BOLDRED = "\033[1m\033[31m" /* Bold Red */;
constexpr auto BOLDGREEN = "\033[1m\033[32m" /* Bold Green */;
constexpr auto BOLDYELLOW = "\033[1m\033[33m" /* Bold Yellow */;
constexpr auto BOLDBLUE = "\033[1m\033[34m" /* Bold Blue */;

// gameboard, nothing: _ = 0, Blue: B = 1, Red: R = 2, Green: G = 3, Yellow: Y =
// 4
// int board[52]{};
std::array<int, 52> board{};

// the structure that contains data for each colour
struct colour {
    // bool waitingboard[5]{}, waitingroom[4]{true, true, true, true},
    // house[4]{}, was_at_the_end{};
    std::array<bool, 5> waitingboard{};
    std::array<bool, 4> waitingroom{true, true, true, true}, house{};
    bool was_at_the_end{};
    int current{}, onboard{}, idx{}, idx_house{}, knock_out_num{};
    int start{}, end{}, colour_num{};
    char ch{};
    std::string upper;
};

colour blue, red, green, yellow;
int round_num = 0;

/* functions */
/* It's a function that writes the game stats to the terminal. */
// void out();

/* some kind of fill thing for pretty out. */
std::string fill(const int &num, const bool &basic, const std::string &str);

/* print the waitingboard for pretty_out */
std::string print_wboard(const colour &color, int num);

/* It's a function that throws the cube. */
void throw_cube(int &current);

/* It's checking if an array of four is empty. */
// bool full_four(const bool arr[4]);
bool full_four(std::array<bool, 4> arr);

/* It's checking if the game is over. */
bool end();

/* It's playing one round. */
void round();

/* It's clearing the terminal. And it's CROSS-PLATFORM! */
void clear_term();

// cross-platform wait
void wait_term();

// is there a player from this colour on the board?
bool is_on_board(const int &color);

// please kick each other, because there is not enough space for two on only one
// field of the board
void knock_out(colour &tmp);

// array of five is not empty
// bool not_empty_five(const bool array[5]);
bool not_empty_five(std::array<bool, 5> arr);

// round for red, green, yellow
void rgy(colour &tmp);

// needs to go home, because it's been kicked out.
void go_home(colour &color);

/* It should be the newer version of out() and be able to show the game as if it
 * was real */
void pretty_out();

/* printing board for pretty_out */
std::string print_board(const int &num);

/* printing waiting rooms */
std::string print_wroom(const colour &color, int num);

/* printing houses for pretty_out */
std::string print_house(const colour &color, int num);

int main() {
    srand(time(nullptr));

    // setting stuff
    blue.start = 0;
    red.start = 13;
    green.start = 26;
    yellow.start = 39;

    blue.end = 50;
    red.end = 11;
    green.end = 24;
    yellow.end = 37;

    blue.colour_num = 1;
    red.colour_num = 2;
    green.colour_num = 3;
    yellow.colour_num = 4;

    blue.ch = 'B';
    red.ch = 'R';
    green.ch = 'G';
    yellow.ch = 'Y';

    // blue.upper = "██"▇;
    blue.upper = "▇▇";
    // blue.lower = "█▄█";

    red.upper = "▇▇";
    // red.lower = "█▀▄";

    green.upper = "▇▇";
    // green.lower = "▀▄█";

    yellow.upper = "▇▇";
    // yellow.lower = " █ ";

    clear_term();

    std::cout << "Are you ready to start this beautiful game? [Y/n] ";
    // char choice = '\0';
    std::string choice = "y";
    // std::cin >> choice;
    getline(std::cin, choice, '\n');
    if (choice.empty() || choice[0] == 'y' || choice[0] == 'Y') {
        choice = "Y";
        std::cout
            << "Do you want to play a full game without doing anything? [Y/n] ";
        getline(std::cin, choice, '\n');
        if (choice.empty() || choice[0] == 'y' || choice[0] == 'Y') {
            while (!end()) {
                round();
                pretty_out();
            }
        } else {
            choice[0] = 'y';
            while (!end() &&
                   (choice[0] == 'y' || choice[0] == 'Y' || choice.empty())) {
                round();
                pretty_out();
                wait_term();
                std::cout << "Can the next round begin? [Y/n] ";
                // std::cin >> choice[0];
                getline(std::cin, choice, '\n');
            }
            pretty_out();
        }
    }
    return 0;
}

// writes game stats to terminal
/* void out() {
    wait_term();
    clear_term();

    std::cout << "board:\n";

    for (int i : board) {
        if (i == 0)
            std::cout << "_  ";
        else if (i == blue.colour_num)
            std::cout << BOLDBLUE << blue.ch << "  " << RESET;
        else if (i == red.colour_num)
            std::cout << BOLDRED << red.ch << "  " << RESET;
        else if (i == green.colour_num)
            std::cout << BOLDGREEN << green.ch << "  " << RESET;
        else if (i == yellow.colour_num)
            std::cout << BOLDYELLOW << yellow.ch << "  " << RESET;
        else
            std::cout << "___!!!error!!!___";
    }
    // numbers under board
    std::cout << '\n';
    for (int i = 0; i < 52; i++) {
        if (i < 10)
            std::cout << i << "  ";
        else
            std::cout << i << " ";
    }
    // showing current numbers of the dice
    std::cout << "\n\ndice:\n";
    {
        std::cout << BLUE << "\tblue:\t" << blue.current << RED << "\n\tred:\t"
                  << red.current << GREEN << "\n\tgreen:\t" << green.current
                  << YELLOW << "\n\tyellow:\t" << yellow.current << RESET;
    }

    std::cout << "\n\nwaiting rooms:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i : blue.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i : red.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i : green.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i : yellow.waitingroom) {
            std::cout << i << " ";
        }
        std::cout << RESET;
    }

    std::cout << "\n\nwaiting boards:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i : blue.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i : red.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i : green.waitingboard) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i : yellow.waitingboard) {
            std::cout << i << " ";
        }
    }

    std::cout << RESET << "\n\nhouses:\n";
    {
        std::cout << BLUE << "\tblue:\t";
        for (bool i : blue.house) {
            std::cout << i << " ";
        }
        std::cout << RED << "\n\tred:\t";
        for (bool i : red.house) {
            std::cout << i << " ";
        }
        std::cout << GREEN << "\n\tgreen:\t";
        for (bool i : green.house) {
            std::cout << i << " ";
        }
        std::cout << YELLOW << "\n\tyellow:\t";
        for (bool i : yellow.house) {
            std::cout << i << " ";
        }
    }

    std::cout << RESET << "\n\nknockouts:\n";
    std::cout << BLUE << "\tblue:\t" << blue.knock_out_num << RED
              << "\n\tred:\t" << red.knock_out_num << GREEN << "\n\tgreen:\t"
              << green.knock_out_num << YELLOW << "\n\tyellow:\t"
              << yellow.knock_out_num << RESET << std::endl;
} */

// plays one round, in the future it should not be so long, it rather should be
// made by making a function, that does it for each colour
void round() {
    throw_cube(blue.current);
    // blue
    {
        // its waiting board is not empty
        if (not_empty_five(blue.waitingboard)) {
            if (blue.onboard + blue.current >= 5) {
                blue.waitingboard[blue.onboard] = false;
                blue.house[blue.idx_house] = true;
                blue.onboard = 0;
                blue.idx_house++;
            } else {
                blue.waitingboard[blue.onboard] = false;
                blue.onboard = blue.current + blue.onboard;
                blue.waitingboard[blue.onboard] = true;
            }
        }
        // board is empty
        else if (!is_on_board(blue.colour_num)) {
            // can go to the board from its waiting room
            if (blue.current == 6) {
                blue.onboard = blue.start;
                knock_out(blue);
                board[blue.onboard] = blue.colour_num;
                blue.waitingroom[blue.idx] = false;
                blue.idx++;
            }
        }
        // waitingroom not full_four
        // not near its house
        else if (blue.onboard + blue.current <= blue.end) {
            board[blue.onboard] = 0;
            blue.onboard += blue.current;
            knock_out(blue);
            board[blue.onboard] = blue.colour_num;
        }
        // so cool, that from the board it goes directly into the house
        else if (blue.onboard + blue.current == blue.end + 6) {
            board[blue.onboard] = 0;
            blue.house[blue.idx_house] = true;
            blue.idx_house++;
        }
        // near the house, needs to go to waitingboard (pl.:48-ason van 5ot
        // dobott)
        else if (blue.onboard + blue.current <= blue.end + 5) {
            board[blue.onboard] = 0;
            blue.onboard = blue.current - (blue.end - blue.onboard) - 1;
            blue.waitingboard[blue.onboard] = true;
        }
    }
    pretty_out();
    rgy(red);
    pretty_out();
    rgy(green);
    pretty_out();
    rgy(yellow);
    round_num++;
}

// clears terminal
void clear_term() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

// is there a figure from this colour on the board?
bool is_on_board(const int &color) {
    int i = 0;
    while (board[i] != color && i < 52)
        i++;
    return i < 52;
}

// checks if an array of four is full
bool full_four(std::array<bool, 4> arr) {
    if (arr[0] && arr[1] && arr[2] && arr[3])
        return true;
    else
        return false;
}

// checks if it's the end of the game
bool end() {
    if (full_four(blue.house) || full_four(red.house) ||
        full_four(green.house) || full_four(yellow.house)) {
        // clear_term();
        std::cout << "\nONE OF THE ANIMALS GOT TO THE END, HOORAY, "
                     "HOORAY!\nNUMBER "
                     "OF ALL KNOCKOUTS: "
                  << blue.knock_out_num + red.knock_out_num +
                         green.knock_out_num + yellow.knock_out_num
                  << "\n";
        return true;
    } else
        return false;
}

// cross-platform wait
void wait_term() {
#ifdef _WIN32
    system("timeout \t 1");
#else
    system("sleep 0.08");
#endif
}

// functions that doesn't let two figures be on the same field
void knock_out(colour &tmp) {
    if (board[tmp.onboard] != 0) {
        switch (board[tmp.onboard]) {
        case 1:
            go_home(blue);
            break;
        case 2:
            go_home(red);
            break;
        case 3:
            go_home(green);
            break;
        case 4:
            go_home(yellow);
            break;
        default:
            std::cout << "___ERROR!___\n";
            break;
        }
        std::cout << "\nA KNOCKOUT HAS HAPPENED (KO) \n";
        wait_term();
    }
}

// checks if an array of five is not empty
bool not_empty_five(std::array<bool, 5> arr) {
    if (arr[0] || arr[1] || arr[2] || arr[3] || arr[4])
        return true;
    else
        return false;
    // return arr[0] || arr[1] || arr[2] || arr[3];
}

// one round, unfortunately doesn't work for blue, so its red, green, yellow
void rgy(colour &tmp) {
    throw_cube(tmp.current);
    // its waiting board is not empty
    if (not_empty_five(tmp.waitingboard)) {
        // goes directly into its house
        if (tmp.onboard + tmp.current >= 5) {
            tmp.waitingboard[tmp.onboard] = false;
            tmp.house[tmp.idx_house] = true;
            tmp.onboard = 0;
            tmp.idx_house++;
        }
        // goes into its house slowly
        else {
            tmp.waitingboard[tmp.onboard] = false;
            tmp.onboard = tmp.onboard + tmp.current;
            tmp.waitingboard[tmp.onboard] = true;
        }
    }
    // board is empty
    else if (!is_on_board(tmp.colour_num)) {
        // can go to the board from its waiting room
        if (tmp.current == 6) {
            tmp.onboard = tmp.start;
            knock_out(tmp);
            board[tmp.onboard] = tmp.colour_num;
            tmp.waitingroom[tmp.idx] = false;
            tmp.idx++;
        }
        // board is not empty
    } else if (!tmp.was_at_the_end) {
        // start movement
        if (tmp.onboard + tmp.current < 52) {
            board[tmp.onboard] = 0;
            tmp.onboard += tmp.current;
            knock_out(tmp);
            board[tmp.onboard] = tmp.colour_num;
        }
        // needs to go to the start of the board
        else if (tmp.onboard + tmp.current > 51) {
            board[tmp.onboard] = 0;
            tmp.onboard = tmp.current - (52 - tmp.onboard);
            knock_out(tmp);
            board[tmp.onboard] = tmp.colour_num;
            tmp.was_at_the_end = true;
        }
    }
    // start movement until it reaches the waiting board
    else if (tmp.onboard + tmp.current <= tmp.end) {
        board[tmp.onboard] = 0;
        tmp.onboard += tmp.current;
        knock_out(tmp);
        board[tmp.onboard] = tmp.colour_num;
    }
    // goes right into house
    else if (tmp.onboard + tmp.current == tmp.end + 6) {
        board[tmp.onboard] = 0;
        tmp.house[tmp.idx_house] = true;
        tmp.idx_house++;
        tmp.was_at_the_end = false;
    }
    // goes into its waiting board
    else if (tmp.onboard + tmp.current <= tmp.end + 5) {
        board[tmp.onboard] = 0;
        tmp.onboard = tmp.current - (tmp.end - tmp.onboard) - 1;
        tmp.waitingboard[tmp.onboard] = true;
        tmp.was_at_the_end = false;
    }
}

// if a figure's been kicked out, it needs to go back to its waitingroom
void go_home(colour &color) {
    // col.onboard = col.start;
    color.was_at_the_end = false;
    color.onboard = 0;
    color.idx--;
    color.waitingroom[color.idx] = true;
    color.knock_out_num++;
}

// it throws the dice, can easily be changed to manual dice roll
void throw_cube(int &current) {
    current = rand() % 6 + 1;
    /* current = 0;
    do {
        std::cout << "What now: ";
        std::cin >> current;
        // 2147483647
    } while (current < 1 || current > 6); */
}

// pretty out
void pretty_out() {
    wait_term();
    clear_term();

    // 0
    // std::cout << "╔" << fill(60, false, "═") << "╗\n";
    // 1: 1/2
    std::cout /*<< "║"*/ << RED << " " << red.current << RESET << "  "
                         << fill(5, true, "") << print_board(23)
                         << print_board(24) << print_board(25)
                         << fill(5, true, "") << "  " << GREEN << green.current
                         << " " << RESET << "\t" << round_num << "\n\n";

    // 2: 1/2
    std::cout << fill(6, true, "") << print_board(22) << print_wboard(green, 0)
              << print_board(26) << fill(6, true, "") << "\n\n";

    // 3: 1/2
    std::cout << fill(2, true, "") << print_wroom(red, 0) << print_wroom(red, 1)
              << fill(2, true, "") << print_board(21) << print_wboard(green, 1)
              << print_board(27) << fill(2, true, "") << print_wroom(green, 0)
              << print_wroom(green, 1) << fill(2, true, "") << "\n\n";

    // 4: 1/2
    std::cout << fill(2, true, "") << print_wroom(red, 2) << print_wroom(red, 3)
              << fill(2, true, "") << print_board(20) << print_wboard(green, 2)
              << print_board(28) << fill(2, true, "") << print_wroom(green, 2)
              << print_wroom(green, 3) << fill(2, true, "") << "\n\n";

    // 5: 1/2
    std::cout << fill(6, true, "") << print_board(19) << print_wboard(green, 3)
              << print_board(29) << fill(6, true, "") << "\n\n";

    // 6: 1/2
    std::cout << fill(6, true, "") << print_board(18) << print_wboard(green, 4)
              << print_board(30) << fill(6, true, "") << "\n\n";

    // 7: 1/2
    std::cout << print_board(12) << print_board(13) << print_board(14)
              << print_board(15) << print_board(16) << print_board(17)
              << fill(3, true, "") << print_board(31) << print_board(32)
              << print_board(33) << print_board(34) << print_board(35)
              << print_board(36) << "\n\n";

    // 8: 1/2
    std::cout << print_board(11);
    for (int i = 0; i < 5; i++) {
        std::cout << print_wboard(red, i);
    }
    std::cout << fill(3, true, "");
    for (int i = 5 - 1; i >= 0; i--) {
        std::cout << print_wboard(yellow, i);
    }
    std::cout << print_board(37) << "\n\n";

    // 9: 1/2
    std::cout << print_board(10) << print_board(9) << print_board(8)
              << print_board(7) << print_board(6) << print_board(5)
              << fill(3, true, "") << print_board(43) << print_board(42)
              << print_board(41) << print_board(40) << print_board(39)
              << print_board(38) << "\n\n";

    // 10: 1/2
    std::cout << fill(6, true, "") << print_board(4) << print_wboard(blue, 4)
              << print_board(44) << fill(6, true, "") << "\n\n";

    // 11: 1/2
    std::cout << fill(6, true, "") << print_board(3) << print_wboard(blue, 3)
              << print_board(45) << fill(6, true, "") << "\n\n";

    // 12: 1/2
    std::cout << fill(2, true, "") << print_wroom(blue, 0)
              << print_wroom(blue, 1) << fill(2, true, "") << print_board(2)
              << print_wboard(blue, 2) << print_board(46) << fill(2, true, "")
              << print_wroom(yellow, 0) << print_wroom(yellow, 1)
              << fill(2, true, "") << "\n\n";

    // 13: 1/2
    std::cout << fill(2, true, "") << print_wroom(blue, 2)
              << print_wroom(blue, 3) << fill(2, true, "") << print_board(1)
              << print_wboard(blue, 1) << print_board(47) << fill(2, true, "")
              << print_wroom(yellow, 2) << print_wroom(yellow, 3)
              << fill(2, true, "") << "\n\n";

    // 14: 1/2
    std::cout << fill(6, true, "") << print_board(0) << print_wboard(blue, 0)
              << print_board(48) << fill(6, true, "") << "\n\n";

    // 15: 1/2
    std::cout << " " << BLUE << blue.current << RESET << "  "
              << fill(5, true, "") << print_board(51) << print_board(50)
              << print_board(49) << fill(5, true, "") << YELLOW << "  "
              << yellow.current << RESET << " "
              << "\n\n";
}

std::string fill(const int &num, const bool &basic, const std::string &str) {
    std::string fill_with;
    if (basic) {
        for (int i = 0; i < num; i++) {
            // fill_with += " ████ ";
            // fill_with += " ░░ ";
            fill_with += "    ";
            // fill_with += " ▆▆ ";
        }
    } else {
        for (int i = 0; i < num; i++) {
            fill_with += str;
        }
    }
    return fill_with;
}

std::string print_board(const int &num) {
    std::string print_this = " ";
    switch (board[num]) {
    case 0:
        print_this = " __";
        break;
    case 1:
        print_this += BLUE;
        print_this += blue.upper;
        break;
    case 2:
        print_this += RED;
        print_this += red.upper;
        break;
    case 3:
        print_this += GREEN;
        print_this += green.upper;
        break;
    case 4:
        print_this += YELLOW;
        print_this += yellow.upper;
        break;
    default:
        print_this = "___!!!error!!!___";
    }
    /*  else {
        switch (board[num]) {
        case 0:
            print_this = " __";
            break;
        case 1:
            print_this += BLUE;
            print_this += blue.upper;
            break;
        case 2:
            print_this += RED;
            print_this += red.upper;
            break;
        case 3:
            print_this += GREEN;
            print_this += green.upper;
            break;
        case 4:
            print_this += YELLOW;
            print_this += yellow.upper;
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    } */
    print_this += " ";
    print_this += RESET;
    return print_this;
}

std::string print_wboard(const colour &color, int num) {
    std::string print_this = " ..";
    if (color.waitingboard[num]) {
        print_this = " ";
        switch (color.colour_num) {
        case 1:
            print_this += BLUE;
            print_this += blue.upper;
            break;
        case 2:
            print_this += RED;
            print_this += red.upper;
            break;
        case 3:
            print_this += GREEN;
            print_this += green.upper;
            break;
        case 4:
            print_this += YELLOW;
            print_this += yellow.upper;
            break;
        default:
            print_this = "___!!!error!!!___";
        }
        /* if (first) {
            print_this = " ";
            switch (color.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.upper;
                break;
            case 2:
                print_this += RED;
                print_this += red.upper;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.upper;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.upper;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        } else {
            print_this = " ";
            switch (color.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.upper;
                break;
            case 2:
                print_this += RED;
                print_this += red.upper;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.upper;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.upper;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        } */
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

std::string print_wroom(const colour &color, int num) {
    std::string print_this = " ..";
    if (color.waitingroom[num]) {
        print_this = " ";
        switch (color.colour_num) {
        case 1:
            print_this += BLUE;
            print_this += blue.upper;
            break;
        case 2:
            print_this += RED;
            print_this += red.upper;
            break;
        case 3:
            print_this += GREEN;
            print_this += green.upper;
            break;
        case 4:
            print_this += YELLOW;
            print_this += yellow.upper;
            break;
        default:
            print_this = "___!!!error!!!___";
        }

        /* if (first) {
            print_this = " ";
            switch (color.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.upper;
                break;
            case 2:
                print_this += RED;
                print_this += red.upper;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.upper;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.upper;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        } else {
            print_this = " ";
            switch (color.colour_num) {
            case 1:
                print_this += BLUE;
                print_this += blue.upper;
                break;
            case 2:
                print_this += RED;
                print_this += red.upper;
                break;
            case 3:
                print_this += GREEN;
                print_this += green.upper;
                break;
            case 4:
                print_this += YELLOW;
                print_this += yellow.upper;
                break;
            default:
                print_this = "___!!!error!!!___";
            }
        } */
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}

std::string print_house(const colour &color, int num) {
    std::string print_this = " ";
    if (color.house[num]) {
        switch (color.colour_num) {
        case 1:
            print_this += BLUE;
            print_this += "▇";
            break;
        case 2:
            print_this += RED;
            print_this += "▇";
            break;
        case 3:
            print_this += GREEN;
            print_this += "▇";
            break;
        case 4:
            print_this += YELLOW;
            print_this += "▇";
            break;
        default:
            print_this = "___!!!error!!!___";
        }
    }
    print_this += " ";
    print_this += RESET;
    return print_this;
}
