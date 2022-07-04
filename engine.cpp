#include "engine.h"
#include <iostream>

void Engine::Run(){
    engine_request_t request;
    int result = ReadRequest(request); 

    while(result == 0)
    {
        result = ExecuteRequest(request);
        if (result == 0)
        {
            result = ReadRequest(request);
        }

        if (result != 0)
        {
            TerminateProgram();
        }
    }
}

int Engine::ReadRequest(engine_request_t &request){

    std::getline(std::cin, request.inputLine);
    //std::cout << request.inputLine << std::endl;

    return 0;
}

int Engine::ExecuteRequest(engine_request_t &request){
    if (request.inputLine.compare("END") == 0)
    {
        //std::cout << "TUK";
        return 1;
    }
    return 0;
}

void Engine::TerminateProgram(){

}
