#include "scenes/SceneManager.h"
#include "scenes/TestScene.h"
#include "scenes/StatsScene.h"
#include "scenes/PokeTransporterGBRefScene.h"
#include "scenes/AboutScene.h"
#include "scenes/InitTransferPakScene.h"
#include "scenes/DistributionPokemonListScene.h"

#include <libdragon.h>

static const uint16_t INPUT_BLOCK_TIME_AFTER_NEW_SCENE_IN_MS = 500;

SceneManager::SceneManager(RDPQGraphics& gfx, AnimationManager& animationManager, FontManager& fontManager, TransferPakManager& tpakManager)
    : sceneHistory_()
    , sceneDeps_(SceneDependencies{
        .gfx = gfx,
        .animationManager = animationManager,
        .fontManager = fontManager,
        .tpakManager = tpakManager,
        .sceneManager = (*this),
        .generation = 0,
        .specificGenVersion = 0
    })
    , scene_(nullptr)
    , newSceneType_(SceneType::NONE)
    , newSceneContext_(nullptr)
    , contextToDelete_(nullptr)
    , deleteContextFunc_(nullptr)
{
}

SceneManager::~SceneManager()
{
    unloadScene(scene_);
}

void SceneManager::switchScene(SceneType type, void (*deleteContextFunc)(void*), void* sceneContext, bool deleteHistory)
{
    newSceneType_ = type;
    newSceneContext_ = sceneContext;

    if(deleteHistory)
    {
        clearHistory();
    }

    sceneHistory_.push_back(SceneHistorySegment{
        .type = type,
        .context = sceneContext,
        .deleteContextFunc = deleteContextFunc
    });

    blockInputStartTime_ = get_ticks();
}

void SceneManager::goBackToPreviousScene()
{
    {
        SceneHistorySegment& curEntry = sceneHistory_.back();
        contextToDelete_ = curEntry.context;
        deleteContextFunc_ = curEntry.deleteContextFunc;
    }
    sceneHistory_.pop_back();
    
    {
        SceneHistorySegment& lastEntry = sceneHistory_.back();
        newSceneType_ = lastEntry.type;
        newSceneContext_ = lastEntry.context;
    }
}

void SceneManager::clearHistory()
{
    for(SceneHistorySegment& entry : sceneHistory_)
    {
        if(entry.context)
        {
            entry.deleteContextFunc(entry.context);
        }
    }
    sceneHistory_.clear();
}

void SceneManager::handleUserInput()
{
    if(!scene_)
    {
        return;
    }

    if(blockInputStartTime_)
    {
        const uint64_t now = get_ticks();
        if(TICKS_TO_MS(now - blockInputStartTime_) < INPUT_BLOCK_TIME_AFTER_NEW_SCENE_IN_MS)
        {
            return;
        }
        // enough time has passed. reset the blockInputStartTime_
        blockInputStartTime_ = 0;
    }

    scene_->processUserInput();
}

void SceneManager::render(const Rectangle& sceneBounds)
{
    if(newSceneType_ != SceneType::NONE)
    {
        loadScene();
    }
    if(!scene_)
    {
        return;
    }

    scene_->render(sceneDeps_.gfx, sceneBounds);
}

void SceneManager::loadScene()
{
    IScene* oldScene = scene_;

    switch(newSceneType_)
    {
        case SceneType::INIT_TRANSFERPAK:
            scene_ = new InitTransferPakScene(sceneDeps_, newSceneContext_);
            break;
        case SceneType::MENU:
            scene_ = new MenuScene(sceneDeps_, newSceneContext_);
            break;
        case SceneType::DISTRIBUTION_POKEMON_LIST:
            scene_ = new DistributionPokemonListScene(sceneDeps_, newSceneContext_);
            break;
        case SceneType::STATS:
            scene_ = new StatsScene(sceneDeps_, newSceneContext_);
            break;
        case SceneType::TEST:
            scene_ = new TestScene(sceneDeps_, newSceneContext_);
            break;
        case SceneType::POKETRANSPORTER_GB_REF:
            scene_ = new PokeTransporterGBRefScene(sceneDeps_, newSceneContext_);
            break;
        case SceneType::ABOUT:
            scene_ = new AboutScene(sceneDeps_, newSceneContext_);
            break;
        default:
            break;
    }

    if(!scene_)
    {
        scene_ = oldScene;
        return;
    }
    unloadScene(oldScene);

    if(contextToDelete_)
    {
        deleteContextFunc_(contextToDelete_);
        contextToDelete_ = nullptr;
        deleteContextFunc_ = nullptr;
    }

    scene_->init();
    newSceneType_ = SceneType::NONE;
}

void SceneManager::unloadScene(IScene* scene)
{
    if(!scene)
    {
        return;
    }

    scene->destroy();
    delete scene;
}