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
    uintptr_t     handle;        // GLuint or other GPU handle
    size_t        estimatedSize; // in bytes
    HANDLE        pluginHandle;  // Which plugin allocated this
    std::string   name;          // Debug name
};

class CPluginResourceTracker {
  public:
    CPluginResourceTracker();
    ~CPluginResourceTracker();

    // Called when a resource is allocated
    void trackResourceAllocation(const SPluginGPUResource& resource);
    
    // Called when a resource is freed
    void untrackResourceAllocation(uintptr_t handle);
    
    // Get all resources allocated by a specific plugin
    std::vector<SPluginGPUResource> getResourcesForPlugin(HANDLE pluginHandle);
    
    // Calculate total VRAM used by a plugin
    size_t getVRAMUsageForPlugin(HANDLE pluginHandle);
    
    // Force-free all resources for a plugin
    void forceCleanupPlugin(HANDLE pluginHandle);
    
    // Get all tracked resources (for debugging)
    const std::vector<SPluginGPUResource>& getAllResources() const;
    
    // Clear all tracking (for debugging/reset)
    void clearAll();

  private:
    std::vector<SPluginGPUResource>              m_resources;
    std::unordered_map<uintptr_t, size_t>        m_handleToIndexMap; // handle -> index in m_resources
};

inline UP<CPluginResourceTracker> g_pPluginResourceTracker;