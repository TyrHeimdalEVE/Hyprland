#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "../defines.hpp"

struct SPluginGPUResource {
    enum eResourceType {
        RESOURCE_TEXTURE,
        RESOURCE_FRAMEBUFFER,
        RESOURCE_RENDERBUFFER,
        RESOURCE_SHADER,
        RESOURCE_BUFFER,
        RESOURCE_UNKNOWN
    };

    eResourceType type;
    uintptr_t     handle;
    size_t        estimatedSize;
    HANDLE        pluginHandle;
    std::string   name;
};

class CPluginResourceTracker {
  public:
    CPluginResourceTracker();
    ~CPluginResourceTracker();

    void trackResourceAllocation(const SPluginGPUResource& resource);
    void untrackResourceAllocation(uintptr_t handle);
    std::vector<SPluginGPUResource> getResourcesForPlugin(HANDLE pluginHandle);
    size_t getVRAMUsageForPlugin(HANDLE pluginHandle);
    void forceCleanupPlugin(HANDLE pluginHandle);
    const std::vector<SPluginGPUResource>& getAllResources() const;
    void clearAll();

  private:
    std::vector<SPluginGPUResource>              m_resources;
    std::unordered_map<uintptr_t, size_t>        m_handleToIndexMap;
};

inline UP<CPluginResourceTracker> g_pPluginResourceTracker;