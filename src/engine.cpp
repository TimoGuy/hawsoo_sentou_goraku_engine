#include "engine.h"

#include <atomic>
#include <vector>
#include "monolithic_renderer_public.h"
#include "world_entity_public.h"


// Impl.
class Engine::Impl
{
public:
    Impl(const std::string& app_name)
        : m_app_name(app_name)
    {
    }

    bool run(
        const std::string& app_name,
        uint32_t num_threads,
        int32_t content_width,
        int32_t content_height)
    {
        // Instantiate job sources.
        std::vector<Job_source*> job_sources;
        Monolithic_renderer renderer{ m_app_name, content_width, content_height };
        job_sources.emplace_back(&renderer);

        // Create job system with job sources.
        Job_system job_system{ num_threads, std::move(job_sources) };
        return job_system.run();
    }

    std::string m_app_name;
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
    int32_t content_width,
    int32_t content_height)
{
    return m_pimpl->run(app_name, num_threads, content_width, content_height);
}
