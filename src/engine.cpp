#include "engine.h"

// Only 64 bit system is supported.
#include <cstdint>
#if INTPTR_MAX != INT64_MAX
    #error "Only 64 bit environment is supported"
#endif

#include <atomic>
#include <vector>
#include "monolithic_renderer_public.h"
#include "ticking_world_simulation_public.h"
#include "world_entity_public.h"


// Impl.
class Engine::Impl
{
public:
    bool run(
        const std::string& app_name,
        uint32_t num_threads,
        int32_t screen_width,
        int32_t screen_height,
        int32_t fallback_screen_width,
        int32_t fallback_screen_height)
    {
        // Instantiate job sources.
        std::vector<Job_source*> job_sources;
        Monolithic_renderer renderer{
            app_name,
            screen_width,
            screen_height,
            fallback_screen_width,
            fallback_screen_height
        };
        World_simulation simulation{ };
        job_sources.emplace_back(&renderer);
        job_sources.emplace_back(&simulation);

        // Create job system with job sources.
        Job_system job_system{ num_threads, std::move(job_sources) };
        return job_system.run();
    }

    std::vector<World_entity> m_world_entities;
};

// Wrapper implementation.
Engine::Engine()
    : m_pimpl(std::make_unique<Impl>())
{
}

Engine::~Engine() = default;  // Define in .cpp for smart ptr pimpl.

bool Engine::run(
    const std::string& app_name,
    uint32_t num_threads,
    int32_t screen_width,
    int32_t screen_height,
    int32_t fallback_screen_width,
    int32_t fallback_screen_height)
{
    return m_pimpl->run(app_name,
                        num_threads,
                        screen_width,
                        screen_height,
                        fallback_screen_width,
                        fallback_screen_height);
}
