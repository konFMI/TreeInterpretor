#include "engine.h"
#include "common.h"


EngineResult Engine::IsValidCommand(std::string &command) {
    ENTER_BLOCK;
    EngineResult result = InvalidInput;

    if (m_enumarete_jobs.find(command) != m_enumarete_jobs.end())
    {
        result = Success;
    }

    return result;
}

static EngineResult IsValidTreeName(std::string &name) {
    ENTER_BLOCK;

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

void Engine::Initilize() {
    ENTER_BLOCK;

    m_enumarete_jobs["LOAD"] = JOB::LOAD;
    m_enumarete_jobs["SAVE"] = JOB::SAVE;
    m_enumarete_jobs["CONTAINS"] = JOB::CONTAINS;
    m_enumarete_jobs["REMOVE"] = JOB::REMOVE;
    m_enumarete_jobs["EXIT"] = JOB::EXIT;
    m_enumarete_jobs["HELP"] = JOB::HELP;
}

void Engine::Run(){
    ENTER_BLOCK;


    engine_request_t request;
    EngineResult result = Success;

    // todo: Should wrap up the end condition.
    do
    {

        while(((result = ReadRequest(request))   == Success) &&
              ((result = ExecuteRequest(request)) == Success)) {
              }

    } while ((result != EngineResult::Exit) &&
             (result != EngineResult::Abort));

    TerminateProgram();
}

EngineResult Engine::ReadRequest(engine_request_t &request){
    ENTER_BLOCK;

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
    ENTER_BLOCK;

    EngineResult result = Success;

    // The check if these elements are available is
    // already done in ConfigureRequest().
    auto arg1 = request.params.data[1];
    auto arg2 = request.params.data[2];

    switch (request.params.job)
    {
    case JOB::LOAD:
        result = Load(arg1,arg2);
        break;
    
    case JOB::SAVE:
        result = Save(arg1,arg2);
        break;
    
    case JOB::CONTAINS:
        result = Contains(arg1,arg2);
        break;
    
    case JOB::REMOVE:
        result = Remove(arg1,arg2);
        break;
    
    case JOB::EXIT:
        result = Exit();
        break;
    case JOB::HELP:
        result = Help();
        break;
    default:
        break;
    }

    HandleResult(result);

    return result;
}

EngineResult Engine::ConfigureRequest(engine_request_t &request, std::vector<std::string> &params) {
    ENTER_BLOCK;

    EngineResult result = VerifyParams(params);

    if (result == Success)
    {
        std::string job = params[0];
        request.params.job = m_enumarete_jobs[job];
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
    ENTER_BLOCK;

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
    else {
        PRINT("Invalid command.");
        result = InvalidInput;
    }

    return result;
}

std::vector<std::string> Engine::ParseInput(std::string input) {
    ENTER_BLOCK;

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
    ENTER_BLOCK;

    EngineResult result = Success;

    if (ExistTree(treeName)) {
        m_enumarate_trees[treeName].Clear();
    }
    else
    {
        PRINT("Tree does not exist.");
        result = InvalidInput;
    }
    m_enumarate_trees[treeName].Import(fileName);

    return result;
}

EngineResult Engine::Save(std::string treeName, std::string fileName) {
    ENTER_BLOCK;

    EngineResult result = Success;

    if (ExistTree(treeName)) {
        std::string input;
        {
            std::ifstream fileToSaveAt(fileName);

            if (fileToSaveAt.is_open()) {
                PRINT("File exists.");
                do
                {
                    PRINT("Press y/n for override of file..");
                    std::getline(std::cin, input);
                } while (input.length() == 1 &&
                         (input == "y" || input == "n"));
            }

            fileToSaveAt.close();
        }

        if (input == "y") {
            m_enumarate_trees[treeName].Export(fileName);
        } else {
            PRINT("Aborting SAVE operation.");
            result = EngineResult::Abort;
        }
    }
    else
    {
        PRINT("Tree does not exist.");
        result = InvalidInput;
    }

    return result;
}

EngineResult Engine::Contains(std::string sTree1, std::string sTree2) {
    ENTER_BLOCK;

    EngineResult result = Success;

    if (ExistTree(sTree1) && ExistTree(sTree2)) {
        result = (m_enumarate_trees[sTree1].Subtree(m_enumarate_trees[sTree2]))? EngineResult::Success : EngineResult::Fail;
    } else
    {
        PRINT("Tree does not exist.");
        result = InvalidInput;
    }

    if (result == EngineResult::Fail)
    {
        PRINT("%s does not contain %s as a subtree.", sTree1.c_str(), sTree2.c_str());
        result = InvalidInput;
    }

    return result;
}

EngineResult Engine::Remove(std::string sTree1, std::string sTree2) {
    ENTER_BLOCK;
    EngineResult result = Success;

    if (Contains(sTree1, sTree2) == EngineResult::Success) {
        result = (m_enumarate_trees[sTree1].Remove(m_enumarate_trees[sTree2]))? EngineResult::Success : EngineResult::Fail;
    }

    if (result == EngineResult::Fail)
    {
        PRINT("Failed to REMOVE %s from %s", sTree2.c_str(), sTree1.c_str());
        result = InvalidInput;
    }

    return result;
}

EngineResult Engine::Exit() {
    ENTER_BLOCK;
    EngineResult result = Success;

    result = EngineResult::Exit;
    return result;
}

EngineResult Engine::Help() {
    ENTER_BLOCK;
    EngineResult result = Success;
    const char *msg = "*************************************************************\n"\
                      "*                      Project tree                         *\n"\
                      "*                                                           *\n"\
                      "* Brief: This is an interpreter for basic tree commands.    *\n"\
                      "*                                                           *\n"\
                      "* Commands:                                                 *\n"\
                      "*                                                           *\n"\
                      "* LOAD treeName fileName : Load tree from file.             *\n"\
                      "* SAVE treeName fileName : Saves tree to file.              *\n"\
                      "* CONTAINS tree1 tree2   : Checks if tree1                  *\n"\
                      "*                          CONTAINS tree2 as a subtree.     *\n"\
                      "* REMOVE tree1 tree2     : Removes from tree1 tree2 if      *\n"\
                      "*                          present.                         *\n"\
                      "* EXIT                   : Exits the program.               *\n"\
                      "* HELP                   : Print this message.              *\n"\
                      "*                                                           *\n"\
                      "*************************************************************";

    PRINT("%s", msg);

    return result;
}

void Engine::TerminateProgram(){
    ENTER_BLOCK;
}
