#pragma once

#include <memory>
#include <string>
#include "multithreaded_job_system_public.h"


class Engine
{
public:
    Engine();
    ~Engine();

    bool run(
        const std::string& app_name,
        uint32_t num_threads,
        int32_t content_width,
        int32_t content_height);

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
};
