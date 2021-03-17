#include "UCI.h"

UCI::UCI()
{
    running = true;
    currentGame = game();
    game::initialize();
}

UCI::~UCI()
{
    //dtor
}


void UCI::run()
{
    while(running)
    {
        std::string input;
        std::getline(std::cin,input);
        processCommand(input);
    }
}


void UCI::runSearch(game g, long long duration, int depth)
{
    auto res = search(g, duration, depth);
    cout << "bestmove " << res.first.toString() << endl;
}


void UCI::processCommand(string str)
{
    if(str == "")
        return;

    vector<string> splitStr = splitCommand(str);

    if(splitStr[0] == "uci")
    {
        ident();
    }
    else if(splitStr[0] == "debug")
    {
        DEBUG = (splitStr[1] == "on");
    }
    else if(splitStr[0] == "isready")
    {
        cout << "readyok" << endl;
    }
    else if(splitStr[0] == "setoption")
    {
        throw new std::invalid_argument("No options can be set");
    }
    else if(splitStr[0] == "register")
    {
        throw std::invalid_argument("Registration not supported");
    }
    else if(splitStr[0] == "ucinewgame")
    {
    }
    else if(splitStr[0] == "position")
    {
        string fen = (splitStr[1] == "startpos") ? "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" : joinString(splitStr, 2);
        currentGame.position = Position(fen.c_str());

        if((int)splitStr.size() > 2)
            processMoves(splitStr);
    }
    else if(splitStr[0] == "go")
    {
        //TODO:: Go parameters

        long long duration = 1e18;
        int maxDepth = 100;
        processGoArguments(splitStr, 1, &duration, &maxDepth);

        thread(UCI::runSearch,currentGame, duration, maxDepth).detach();
    }
    else if(splitStr[0] == "stop")
    {
        end_time = std::chrono::system_clock::now();
    }
    else if(splitStr[0] == "ponderhit")
    {
        throw std::invalid_argument("Ponders not supported");
    }
    else if(splitStr[0] == "quit")
    {
        running = false;
    }
    else
    {
        throw std::invalid_argument("Command does not follow UCI standard");
    }
}

void UCI::ident()
{
    std::cout << "id CarlTheChessBot Rui Oliveira Tiago Bacelar" << std::endl;
}

void UCI::processMoves(vector<string> moves)
{
    for(int i = 3; i < (int)moves.size(); i++)
    {
        currentGame.makeMove(currentGame.position.moveFromString(moves[i]));
    }
}

void UCI::processGoArguments(vector<string> args, int index, long long* duration, int* maxDepth)
{
    if(index == args.size())
        return;

    if(args[index] == "movetime")
    {
        *duration = stoll(args[++index]);
    }
    else if(args[index] == "depth")
    {
        *maxDepth = stoi(args[++index]);
    }

    processGoArguments(args, ++index, duration, maxDepth);
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
        else
        {
            cur += str[i];
        }
    }

    return res;
}


string UCI::joinString(vector<string> list, int index)
{
    string res = "";

    for(int i = index; i < list.size(); i++)
        res += list[i];

    return res;
}
