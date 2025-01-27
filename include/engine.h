#pragma once

#include <memory>
#include <string>


class Engine
{
public:
    Engine();
    ~Engine();

    bool run(
        const std::string& app_name,
        uint32_t num_threads,
        int32_t screen_width,
        int32_t screen_height,
        int32_t fallback_screen_width,
        int32_t fallback_screen_height);

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
};
