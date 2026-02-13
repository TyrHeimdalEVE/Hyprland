#include <other_includes>
#include "PluginResourceTracker.hpp"

void unloadPlugin(Plugin *plugin) {
    // Existing unloadPlugin code...

    // === GPU Resource Cleanup ===
    if (g_pPluginResourceTracker) {
        auto gpuResources = g_pPluginResourceTracker->getResourcesForPlugin(plugin->m_handle);
        
        if (!gpuResources.empty()) {
            Log::logger->log(Log::INFO, 
                " [PluginSystem] Plugin {} has {} GPU resources, forcing cleanup",
                plugin->m_name, gpuResources.size());
            
            // Ensure GPU is idle before cleanup
            glFinish();
            
            // Force-free all GPU resources owned by this plugin
            for (const auto& res : gpuResources) {
                switch (res.type) {
                    case SPluginGPUResource::RESOURCE_TEXTURE:
                        glDeleteTextures(1, (GLuint*)&res.handle);
                        break;
                    case SPluginGPUResource::RESOURCE_FRAMEBUFFER:
                        glDeleteFramebuffers(1, (GLuint*)&res.handle);
                        break;
                    case SPluginGPUResource::RESOURCE_BUFFER:
                        glDeleteBuffers(1, (GLuint*)&res.handle);
                        break;
                    case SPluginGPUResource::RESOURCE_RENDERBUFFER:
                        glDeleteRenderbuffers(1, (GLuint*)&res.handle);
                        break;
                    default:
                        break;
                }
                g_pPluginResourceTracker->untrackResourceAllocation(res.handle);
            }
            
            // Final GPU sync to ensure all deletions are complete
            glFinish();
            
            Log::logger->log(Log::DEBUG, 
                " [PluginSystem] Plugin {} GPU cleanup complete", plugin->m_name);
        }
    }
    
    g_pConfigManager->removePluginConfig(plugin);
}