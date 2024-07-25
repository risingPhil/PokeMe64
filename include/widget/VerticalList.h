#ifndef _VERTICALLIST_H
#define _VERTICALLIST_H

#include "animations/IAnimation.h"
#include "widget/IWidget.h"
#include "core/Sprite.h"

#include <vector>

class RDPQGraphics;
class IWidget;
class VerticalList;
class AnimationManager;

typedef std::vector<IWidget*> IWidgetList;
typedef std::vector<Rectangle> WidgetBoundsList;

struct FocusChangeStatus;
class IFocusListener;
typedef std::vector<IFocusListener*> FocusListenerList;

/**
 * @brief This Animation implementation is used internal in VerticalList
 * to move the "view window"'s y-coordinate.
 * 
 * It's specific to the VerticalList widget
 */
class MoveVerticalListWindowAnimation : public AbstractAnimation
{
public:
    MoveVerticalListWindowAnimation(VerticalList* list);
    virtual ~MoveVerticalListWindowAnimation();

    AnimationDistanceTimeFunctionType getDistanceTimeFunctionType() const override;

    uint32_t getDurationInMs() const override;

    void start(uint32_t windowStartY, uint32_t windowEndY);
protected:
    void apply(float pos) override;
private:
    VerticalList* list_;
    int32_t windowStartY_;
    int32_t windowEndY_;
};

/**
 * @brief This struct defines the "style" of the widget. This means
 * that it defines properties that influence the visual characteristics
 * of the entire list component.
 * 
 * Use verticalList.setStyle() to apply a style onto the list.
 */
typedef struct VerticalListStyle
{
    struct {
        /**
         * @brief (optional) a background sprite to render the background of 
         * the widget
         */
        sprite_t* sprite;
        /**
         * @brief (optional) render settings for rendering the background sprite (if any)
         * 
         */
        SpriteRenderSettings spriteSettings;
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

    /**
     * @brief the amount of spacing (in pixels) between 2 list widgets (default: 0)
     */
    int verticalSpacingBetweenWidgets;

    /**
     * @brief should grow automaticaly with each item
     */
    struct {
        /**
         * Whether the list should grow its bounds whenever items are added
         */
        bool enabled;
        
        /**
         * defines the direction the list should grow in. true for upwards, false for downwards
         */
        bool shouldGrowUpWards;

        /**
         * Maximum height the list should grow towards
         */
        uint16_t maxHeight;
    } autogrow;
} VerticalListStyle;

/**
 * @brief This widget implements a vertical list.
 * It can be used for menu-like UI elements.
 * 
 * It's very flexible in the sense that you can use it for any kind of IWidget.
 * 
 * You can influence spacing and background with the VerticalListStyle.
 * 
 * In concept, internally it keeps an infinitely growable list of widgets of which the 
 * vertical y-coordinate will be increased with each widget. These coordinates remain static.
 * 
 * However, it also has a "view window" that is used to translate the widget coordinates to the actual
 * coordinates the visible widgets need to be rendered at.
 * 
 * A vertical move to a widget that is outside of the view window is implemented with an animation of
 * the view window y-coordinate.
 */
class VerticalList : public IWidget
{
public:
    VerticalList(AnimationManager& animationManager);
    virtual ~VerticalList();

    bool focusNext();
    bool focusPrevious();

    void addWidget(IWidget* widget);
    void clearWidgets();

    VerticalListStyle& getStyle();
    void setStyle(const VerticalListStyle& style);
    
    void setViewWindowStartY(uint32_t windowStartY);

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;
    Dimensions getSize() const override;

    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;

    void registerFocusListener(IFocusListener* listener);
    void unregisterFocusListener(IFocusListener* listener);
protected:
private:
    void rebuildLayout();
    /**
     * Scrolls the window until the focused widget is fully visible with an animation
     * @return returns the number of vertical pixels that will be scrolled
     */
    int32_t scrollWindowToFocusedWidget();
    void moveWindow(int32_t yAmount);
    void notifyFocusListeners(const FocusChangeStatus& status);
    void autoGrowBounds();

    MoveVerticalListWindowAnimation moveWindowAnimation_;
    IWidgetList widgetList_;
    WidgetBoundsList widgetBoundsList_;
    FocusListenerList focusListeners_;
    VerticalListStyle listStyle_;
    Rectangle bounds_;
    uint32_t windowMinY_;
    uint32_t focusedWidgetIndex_;
    AnimationManager& animManager_;
    bool focused_;
    bool visible_;
};

#endif