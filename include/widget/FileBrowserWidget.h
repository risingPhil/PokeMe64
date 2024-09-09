#ifndef SDCARDFILEBROWSERWIDGET_H
#define SDCARDFILEBROWSERWIDGET_H

#include "core/Sprite.h"
#include "core/RDPQGraphics.h"
#include "widget/IWidget.h"
#include "widget/VerticalList.h"
#include "widget/MenuItemWidget.h"

#include <vector>

typedef struct FileBrowserWidgetStyle
{
    struct {
        sprite_t* sprite;
        SpriteRenderSettings renderSettings;
    } background;
    struct {
        /**
         * @brief left margin -> the widgets will start rendering after this x spacing offset from the left edge of the list
         */
        int left;
        /**
         * @brief right margin -> the widgets will stop rendering x pixels from the right of this list
         */
        int right;
        /**
         * @brief top margin -> the widgets will start rendering after this y spacing offset from the top edge of the list
         */
        int top;
        /**
         * @brief bottom margin -> the widgets will stop rendering y pixels from the bottom edge of this list.
         */
        int bottom;
    } margin;
    MenuItemStyle itemStyle;
} FileBrowserWidgetStyle;

typedef struct FileBrowserWidgetStatus
{
    std::vector<MenuItemWidget*>& itemList;
    int err;
} FileBrowserWidgetStatus;

/**
 * @brief This widget allows you to browse the SD card filesystem and
 * select a file
 * 
 */
class FileBrowserWidget : public IWidget
{
public:
    FileBrowserWidget(AnimationManager& animManager);
    virtual ~FileBrowserWidget();

    /**
     * @brief Returns whether the widget is currently focused
     */
    bool isFocused() const override;
    
    /**
     * @brief Sets whether the widget is currently focused
     * 
     */
    void setFocused(bool isFocused) override;

    /**
     * @brief Returns whether the widget is currently visible
     */
    bool isVisible() const override;
    
    /**
     * @brief Changes the visibility of the widget
     */
    void setVisible(bool visible) override;

    /**
     * @brief Returns the current (relative) bounds of the widget
     */
    Rectangle getBounds() const override;

    /**
     * @brief Changes the current (relative) bounds of the widget
     */
    void setBounds(const Rectangle& bounds) override;

    /**
     * @brief Returns the size (width/height) of the widget
     */
    Dimensions getSize() const override;
    
    /**
     * @brief Sets the style of this widget
     */
    void setStyle(const FileBrowserWidgetStyle& style);

    /**
     * @brief Handles user input
     * 
     * For button presses, it is advised to track button release situations instead of 
     * button presses for executing an action. Otherwise the key press might be handled again immediately
     * in the next scene/widget because the user wouldn't have had the time to actually release the key.
     */
    bool handleUserInput(const joypad_inputs_t& userInput) override;

    /**
     * @brief Renders the widget
     * 
     * @param gfx The graphics instance that must be used to render the widget
     * @param parentBounds The bounds of the parent widget or scene. You must add the x,y offset of your own bounds 
     * to the parentBounds to get the absolute bounds for rendering.
     * 
     * Getting the parentBounds as an argument of this function was done because a parent widget may be 
     * animated or change positions independent of the child widget. But when the parent widget moves, the child must as well!
     */
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;    

    const FileBrowserWidgetStatus& getStatus() const;

    /**
     * @brief Retrieves the currently set path
     */
    const char* getPath() const;
    /**
     * @brief Sets the current path of the FileBrowserWidget
     */
    void setPath(const char* path);

    /**
     * @brief Internal callback function for when you press A on a directory
     */
    void onConfirmDirectory(const char* path);
    
    /**
     * @brief Internal callback function for when you press A on a file
     */
    void onConfirmFile(const char* path);

    void setItemConfirmedCallback(void (*onItemConfirmed)(void*, const char*), void* context);

    /**
     * @brief Sets a file extension filter. The files that have such an extension will be shown,
     * whereas non-matching files WON'T be shown
     */
    void setFileExtensionToFilter(const char* fileExtensionFilter);
protected:
private:
    void clearList();
    void loadDirectoryItems();
    bool goToParentDirectory();

    std::vector<char*> duplicatedDirEntNameList_;
    // we are responsible for deleting the ItemMenuWidget instances
    // so we need to keep track of them.
    std::vector<MenuItemWidget*> itemWidgetList_;
    VerticalList listWidget_;
    FileBrowserWidgetStyle style_;
    FileBrowserWidgetStatus status_;
    Rectangle bounds_;
    char pathBuffer_[4096];
    void (*onItemConfirmedCallback_)(void*, const char*);
    void* onItemConfirmedCallbackContext_;
    const char* fileExtensionFilter_;
    bool focused_;
    bool visible_;
    bool bButtonPressed_;
};

#endif