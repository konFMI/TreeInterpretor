#ifndef ENGINE_H
#define ENGINE_H

#include <string>

enum JOB {
    LOAD = 0,
    SAVE,
    CONTAINS,
    REMOVE,
    INVALID_JOB
};

struct engine_params_t
{
    JOB job;
    std::string objectName;
    std::string file_path;
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
        Engine(){}
        ~Engine(){}
        void Run();

    private:
        int ReadRequest(engine_request_t &request);
        int ExecuteRequest(engine_request_t &request);
        void TerminateProgram();

        session_data_t session_data_;
};

#endif //ENGINE_H