#ifndef _DATACOPYSCENE_H
#define _DATACOPYSCENE_H

#include "scenes/SceneWithProgressBar.h"
#include "transferpak/TransferPakManager.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"
#include "transferpak/TransferPakDataCopier.h"

enum class DataCopyOperation
{
    BACKUP_SAVE,
    BACKUP_ROM,
    RESTORE_SAVE
};

typedef struct DataCopySceneContext
{
    DataCopyOperation operation;
} DataCopySceneContext;

class DataCopyScene : public SceneWithProgressBar
{
public:
    DataCopyScene(SceneDependencies& deps, void* context);
    virtual ~DataCopyScene();

    void init() override;
    void destroy() override;

    void processUserInput() override;
    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void onDialogDone();
protected:
    void setupDialog(DialogWidgetStyle& style) override;
    void setupProgressBar(ProgressBarWidgetStyle& style) override;
private:
    TransferPakRomReader romReader_;
    TransferPakSaveManager saveManager_;
    DataCopySceneContext* sceneContext_;
    ITransferPakDataCopySource* copySource_;
    ITransferPakDataCopyDestination* copyDestination_;
    TransferPakDataCopier* copier_;
    sprite_t* dialogWidgetSprite_;
    sprite_t* progressBackgroundSprite_;
    DialogData diag_;
    uint32_t totalBytesToCopy_;
};

void deleteDataCopySceneContext(void* context);

#endif