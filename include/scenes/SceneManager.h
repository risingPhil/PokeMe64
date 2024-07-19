#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "scenes/IScene.h"
#include <vector>

class RDPQGraphics;
class AnimationManager;
class FontManager;
class TransferPakManager;

typedef struct Rectangle Rectangle;

enum class SceneType;

typedef struct SceneHistorySegment
{
    SceneType type;
    void* context;
    void (*deleteContextFunc)(void*);
} SceneHistorySegment;

/**
 * @brief The SceneManager handles switching between IScene objects. (loading, unloading, forwarding input and render requests)
 * 
 */
class SceneManager
{
public:
    SceneManager(RDPQGraphics& gfx, AnimationManager& animationManager, FontManager& fontManager, TransferPakManager& tpakManager);
    ~SceneManager();

    /**
     * This function stores the given scenetype to be loaded
     * on the next render() call.
     * 
     * The reason for this deferred loading is that you're usually triggering the switchScene call from within the current Scene.
     * If not implemented this way, the current Scene object would be free'd while a member function would still be running on it (use-after-free => kaboom)
     * So, by deferring the scene switch, we make sure the current Scene instance is done executing whatever before we swipe away the carpet from beneath its feet.
     * 
     * WARNING: If you specify a sceneContext, you MUST also specify a deleteContextFunc callback function pointer.
     * This is needed because you can't call delete() on a void*. And we need to keep the context around in the sceneHistory for as long as it needs to
     */
    void switchScene(SceneType sceneType, void (*deleteContextFunc)(void*) = nullptr, void* sceneContext = nullptr);
    
    /**
     * @brief Switch back to the previous scene in the history stack
     */
    void goBackToPreviousScene();

    /**
     * @brief Clears the history stack
     */
    void clearHistory();
    
    void handleUserInput();
    void render(const Rectangle& sceneBounds);
protected:
private:
    void loadScene();
    void unloadScene(IScene* scene);

    std::vector<SceneHistorySegment> sceneHistory_;
    SceneDependencies sceneDeps_;
    IScene* scene_;
    SceneType newSceneType_;
    void* newSceneContext_;
    void* contextToDelete_;
    void (*deleteContextFunc_)(void*);
};

#endif