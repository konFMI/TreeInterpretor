#include "engine.h"
#include <iostream>
#include <fstream>

#define print(str)(std::cout << str << std::endl)
class Log {
    public:
    Log(std::string funcName) : funcName(funcName) {
        print("Enter::" + funcName);
    }
    ~Log() {
        print("~Exit::" + funcName);
    }

    private:
    std::string funcName;
};

#define ENTER_BLOCK Log enter_block_logger(__func__);

EngineResult Engine::IsValidCommand(std::string &command) {
    ENTER_BLOCK
    EngineResult result = InvalidInput;

    if (m_enumarete_jobs.find(command) != m_enumarete_jobs.end())
    {
        result = Success;
    }

    return result;
}

static EngineResult IsValidTreeName(std::string &name) {
    ENTER_BLOCK

    EngineResult result = Success;

    for (auto &l : name)
    {
        if (std::isspace(l) != 0) {
            result = InvalidInput;
            break;
        }
    }

    return result;
}

static void PrintErrorMessage(const char *message) {
    ENTER_BLOCK

    std::cout << "ERROR: " << message << std::endl;
}

void Engine::Initilize() {
    ENTER_BLOCK

    m_enumarete_jobs["load"] = JOB::LOAD;
    m_enumarete_jobs["save"] = JOB::SAVE;
    m_enumarete_jobs["contains"] = JOB::CONTAINS;
    m_enumarete_jobs["remove"] = JOB::REMOVE;
    m_enumarete_jobs["exit"] = JOB::EXIT;
    m_enumarete_jobs["help"] = JOB::HELP;
}

void Engine::Run(){
    ENTER_BLOCK

    engine_request_t request;
    EngineResult result = Success;

    while(((result = ReadRequest(request))   == Success) &&
          ((result = ExecuteRequest(request)) == Success));

    TerminateProgram();
}

EngineResult Engine::ReadRequest(engine_request_t &request){
    ENTER_BLOCK

    EngineResult result = Success;

    do
    {
        std::getline(std::cin, request.inputLine);
        auto params = ParseInput(request.inputLine);
        result = ConfigureRequest(request, params);
    } while(result == InvalidInput);

    return result;
}

EngineResult Engine::ExecuteRequest(engine_request_t &request){
    EngineResult result = Success;

    switch (request.params.job)
    {
    case JOB::LOAD:
        result = Load(request.params.data[0],
                      request.params.data[1]);
        /* code */
        break;
    
    case JOB::SAVE:
        result = Save(request.params.data[0],
                      request.params.data[1]);
        /* code */
        break;
    
    case JOB::CONTAINS:
        {
            auto sTree1 = request.params.data[0];
            auto sTree2 = request.params.data[1];
            if (ExistTree(sTree1) && ExistTree(sTree2)) {
                result = Contains(m_enumarate_trees[sTree1],
                                  m_enumarate_trees[sTree2]);
            } else 
            {
                //todo
            }
            /* code */
        }
        break;
    
    case JOB::REMOVE:
        {
            auto sTree1 = request.params.data[0];
            auto sTree2 = request.params.data[1];
            if (ExistTree(sTree1) && ExistTree(sTree2)) {
                result = Remove(m_enumarate_trees[sTree1],
                                  m_enumarate_trees[sTree2]);
            } else 
            {
                //todo
            }
            /* code */
        }
        break;
    
    case JOB::EXIT:
        result = Exit();
        /* code */
        break;
    default:
        break;
    }

    HandleResult(result);

    return result;
}

EngineResult Engine::ConfigureRequest(engine_request_t &request, std::vector<std::string> &params) {
    ENTER_BLOCK

    
    EngineResult result = VerifyParams(params);

    if (result == Success)
    {
        request.params.job = m_enumarete_jobs[params[0]];
        request.params.data = params;
    } else 
    {
        
        request.inputLine.clear();
        request.params.job = JOB::INVALID_JOB;
        request.params.data.clear();
    }

    return result;
}

EngineResult Engine::VerifyParams(std::vector<std::string> &params) {
    ENTER_BLOCK

    EngineResult result = InvalidInput;

    if (((params.size() == 1)                             &&
         ((result = IsValidCommand(params[0])) == Success) &&
         ((m_enumarete_jobs[params[0]] == JOB::EXIT) ||
          (m_enumarete_jobs[params[0]] == JOB::HELP))) ||
        ((params.size() == 3)                              &&
        ((result = IsValidCommand(params[0])) == Success)   &&
        ((result = IsValidTreeName(params[1])) == Success) &&
        ((result = IsValidTreeName(params[2])) == Success)))
    {
        result = Success;
    }

    if (result != Success)
    {
        PrintErrorMessage("Invalid command.");
    }

    return result;
}

std::vector<std::string> Engine::ParseInput(std::string input) {
    ENTER_BLOCK


    std::vector<std::string> params;

    std::string param;

    for (auto &l : input)
    {
        if (std::isspace(l) != 0)
        {
            params.push_back(param);
            param.clear();
        } else
        {
            param += l;
        }
    }

    if (param.length() != 0) {
        params.push_back(param);
        param.clear();
    }

    return params;
}

EngineResult Engine::Load(std::string treeName, std::string fileName) {
    ENTER_BLOCK

    EngineResult result = Success;

    if (ExistTree(treeName)) {
        m_enumarate_trees[treeName].Clear();
    }

    m_enumarate_trees[treeName].Import(fileName);

    return result;
}

EngineResult Engine::Save(std::string treeName, std::string fileName) {
    ENTER_BLOCK

    EngineResult result = Success;

    if (ExistTree(treeName)) {
        std::string input;
        {
            std::ifstream fileToSaveAt(fileName);

            if (fileToSaveAt.is_open()) {
                print("File exists.");
                do
                {
                    print("Press y/n for override of file..");
                    std::getline(std::cin, input);
                } while (input.length() == 1 &&
                         (input == "y" || input == "n"));
            }

            fileToSaveAt.close();
        }

        if (input == "y") {
            m_enumarate_trees[treeName].Export(fileName);
        } else {
            print("Aborting save operation.");
            result = EngineResult::Abort;
        }
    }
    else
    {
        print("Tree does not exist.");
        result = InvalidInput;
    }

    return result;
}

EngineResult Engine::Contains(Tree tree1, Tree tree2) {
    ENTER_BLOCK

    EngineResult result = Success;
    return result;
}

EngineResult Engine::Remove(Tree tree1, Tree tree2) {
    ENTER_BLOCK
    EngineResult result = Success;
    return result;
}

EngineResult Engine::Exit() {
    ENTER_BLOCK
    EngineResult result = Success;

    result = EngineResult::Exit;
    return result;
}

void Engine::TerminateProgram(){
    ENTER_BLOCK
}
