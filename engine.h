#ifndef ENGINE_H
#define ENGINE_H

#include "tree.h"

#include <string>
#include <vector>
#include <map>
enum EngineResult {
    Success = 0,
    InvalidInput,
    Abort,
    Exit,
    Fail
};

enum JOB {
    LOAD = 0,
    SAVE,
    CONTAINS,
    REMOVE,
    EXIT,
    HELP,
    INVALID_JOB
};

struct engine_params_t
{
    JOB job;
    std::vector<std::string> data;
};


struct engine_request_t
{
    std::string inputLine;
    engine_params_t params;
};

struct session_data_t
{
    
};

class Engine
{
public:
        Engine(){
            Initilize();
        }

        ~Engine(){
            m_enumarate_trees.clear();
            m_enumarete_jobs.clear();
        }

        void Run();

private:
        void Initilize();
        EngineResult ReadRequest(engine_request_t &request);
        EngineResult ExecuteRequest(engine_request_t &request);

        EngineResult VerifyParams(std::vector<std::string> &params);
        EngineResult ConfigureRequest(engine_request_t &request, std::vector<std::string> &params);
        std::vector<std::string> ParseInput(std::string input);
        EngineResult IsValidCommand(std::string &command);


        EngineResult Load(std::string treeName, std::string fileName);
        EngineResult Save(std::string treeName, std::string fileName);
        EngineResult Contains(std::string tree1, std::string tree2);
        EngineResult Remove(std::string tree1, std::string tree2);
        EngineResult Exit();
        EngineResult Help();

        void TerminateProgram();

        bool ExistTree(std::string &tree) {
            auto iter = m_enumarate_trees.find(tree);

            return iter != m_enumarate_trees.end();
        }

        void HandleResult(EngineResult &result) {
            
            switch (result) {
            case EngineResult::Success:
            case EngineResult::InvalidInput:
            case EngineResult::Abort:
                result = Success;
                break;
            default:
                break;
            }
        }

        session_data_t session_data_;
        std::map<std::string, JOB> m_enumarete_jobs;
        std::map<std::string, Tree> m_enumarate_trees;
};

#endif //ENGINE_H