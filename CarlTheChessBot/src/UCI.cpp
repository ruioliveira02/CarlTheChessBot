#include "UCI.h"

UCI::UCI()
{
    running = true;
    currentGame = game();
    game::initialize();
    searchEngine = searcher();

}

UCI::~UCI()
{
    //dtor
}


void UCI::run()
{
    while(running)
    {
        string input = "";
        cin >> input;

        processCommand(input);
    }
}



void UCI::processCommand(string str)
{
    if(str == "")
        return;

    vector<string> splitStr = splitCommand(str);

    switch(splitStr[0])
    {
        case "uci":
            break;
        case "debug":
            //DEBUG = (splitStr[1] == 'on');
            break;
        case "isready":
            cout << "readyok" << endl;
            break;
        case "setoption":
            throw new exception("No options can be set");
            break;
        case "register":
            throw exception("Registration not supported");
            break;
        case "ucinewgame":
            break;
        case "position":
            string fen = (splitStr[1] == "startpos") ? "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" : splitStr[1];
            currentGame.position = position(fen);

            if((int)splitStr.size() > 2)
                processMoves(splitStr);

            break;
        case "go":
            //TODO:: Go parameters
            auto res = searchEngine.search(game);

            cout << "best move " << res.first.toString() << endl;

            break;
        case "stop":
            break;
        case "ponderhit":
            throw exception("Ponders not supported");
            break;
        case "quit":
            running = false;
            break;
        default:
            throw exception("Command does not follow UCI standard");
            break;
    }
}

void UCI::processMoves(vector<string> moves)
{
    for(int i = 3; i < (int)moves.size(); i++)
    {
        currentGame.makeMove(moves[i]);
    }
}

vector<string> UCI::splitCommand(string str)
{
    str += " "; // Adding a space at the end of the string to make sure all the words are included in the vector

    string cur = "";
    vector<string> res = vector<string>();

    for(int i = 0; i < (int)str.size(); i++)
    {
        if(str[i] == ' ' && cur != "")
        {
            res.push_back(cur);
            cur = "";
        }
    }

    return res;
}
