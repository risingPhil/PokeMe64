#ifndef _DIALOGWIDGET_H
#define _DIALOGWIDGET_H

#include "widget/VerticalList.h"
#include "widget/MenuItemWidget.h"
#include "core/Sprite.h"
#include "core/RDPQGraphics.h"

#define DIALOG_TEXT_SIZE 512

class AnimationManager;

typedef struct DialogData
{
    char text[DIALOG_TEXT_SIZE];
    // optional sprite of a character that is saying the dialog text
    struct {
        sprite_t* sprite;
        SpriteRenderSettings spriteSettings;
        // bounds of the character sprite relative to the widget
        Rectangle spriteBounds;
        bool spriteVisible;
    } character;
    // optional button sprite
    struct {
        sprite_t* sprite;
        SpriteRenderSettings spriteSettings;
        // bounds of the button sprite relative to the widget
        Rectangle spriteBounds;
        bool spriteVisible;
    } button;
    // use this struct if you want to provide dialog options
    struct {
        MenuItemData* items;
        uint8_t number;
        bool shouldDeleteWhenDone;
    } options;

    // The next Dialog
    struct DialogData* next;
    bool shouldDeleteWhenDone;
    bool userAdvanceBlocked;
    //TODO: dialog sound
} DialogData;

typedef struct DialogWidgetStyle
{
    struct {
        sprite_t* sprite;
        SpriteRenderSettings spriteSettings;
    } background;
    struct {
        Rectangle bounds;
        MenuItemStyle style;
    } dialogOptions;
    TextRenderSettings textSettings;
    struct {
        int left;
        int right;
        int top;
        int bottom;
    } margin;
} DialogWidgetStyle;

/**
 * This widget is used to display dialog text (usually at the bottom of the screen)
 * You can specify a dialog sequence with the DialogData struct to be shown to the user.
 * 
 * When the dialog has finished (after the user presses A when the last DialogData entry was shown)
 * the onDialogFinished callback function (if any) will be triggered.
 * 
 * If you press the A button, the DialogWidget advances to the next DialogData entry (if any)
 * or (like I said before) triggers the onDialogFinished callback.
 */
class DialogWidget : public IWidget
{
public:
    DialogWidget(AnimationManager& animationManager);
    virtual ~DialogWidget();

    const DialogWidgetStyle& getStyle() const;
    void setStyle(const DialogWidgetStyle& style);
    
    void setData(DialogData* data);
    void appendDialogData(DialogData* data);

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;
    Dimensions getSize() const override;

    /**
     * @brief Sets a callback function that will be called when we run out of dialog
     */
    void setOnDialogFinishedCallback(void (*onDialogFinishedCb)(void*), void* context);

    /**
     * @brief Advances the current dialog -> the next DialogData entry (if any) will be shown
     * or the onDialogFinished callback will be triggered
     */
    void advanceDialog();

    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;
protected:
private:
    /**
     * @brief Indicates if the user is allowed to advance the dialog (yet)
     * This could be used -for example- to restrict advancing until after a certain amount of time
     * For example: waiting until a sound has played. (not implemented yet though)
     */
    bool isAdvanceAllowed() const;

    VerticalList dialogOptionList_;
    AnimationManager& animationManager_;
    Rectangle bounds_;
    DialogWidgetStyle style_;
    DialogData* data_;
    void (*onDialogFinishedCb_)(void*);
    void *onDialogFinishedCbContext_;
    bool focused_;
    bool visible_;
    bool btnAPressedOnPrevCheck_;
};

/**
 * @brief This function sets the text field of the DialogData struct with snprintf
 * 
 * @param data the DialogData struct to fill
 * @param format the printf format string
 * @param ... variable arguments to use within the snprintf call
 */
void setDialogDataText(DialogData& data, const char* format, ...);

#endif