#include "PluginResourceTracker.hpp" // Additional include for GPU resource tracking

void unloadPlugin() {
    g_pConfigManager->removePluginConfig();
    // === GPU Resource Cleanup ===
    if (g_pPluginResourceTracker) {
        auto gpuResources = g_pPluginResourceTracker->getResourcesForPlugin(PLHANDLE);
        
        if (!gpuResources.empty()) {
            Log::logger->log(Log::INFO, 
                " [PluginSystem] Plugin {} has {} GPU resources, forcing cleanup",
                PLNAME, gpuResources.size());
            
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
                " [PluginSystem] Plugin {} GPU cleanup complete", PLNAME);
        }
    }
}