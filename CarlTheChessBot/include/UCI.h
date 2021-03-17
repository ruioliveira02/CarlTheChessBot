#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <thread>

#include "game.h"
#include "search.h"

using namespace std;

class UCI
{
    public:
        UCI();
        virtual ~UCI();

        void run();

    public:
        static void runSearch(game, long long, int);
    private:


        void processCommand(string);

        void ident();

        void processMoves(vector<string>);

        void processGoArguments(vector<string>, int, long long*, int*);

        vector<string> splitCommand(string);
        string joinString(vector<string>, int);

    private:
        bool running;
        game currentGame;
};

#endif // UCI_H
