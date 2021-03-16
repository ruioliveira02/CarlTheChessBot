#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>

#include "game.h"
#include "search.h"

using namespace std;

class UCI
{
    public:
        UCI();
        virtual ~UCI();

        void run();

    private:
        void processCommand(string);

        void ident();

        void processMoves(vector<string>);
        vector<string> splitCommand(string);

    private:
        bool running;
        game currentGame;
        searcher searchEngine;
};

#endif // UCI_H
