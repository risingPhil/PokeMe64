#ifndef _TEXTWIDGET_H
#define _TEXTWIDGET_H

#include "widget/IWidget.h"
#include "core/RDPQGraphics.h"

typedef struct TextWidgetStyle
{
    TextRenderSettings renderSettingsFocused;
    TextRenderSettings renderSettingsNotFocused;
    uint16_t backgroundColor_; // RGBA16
} TextWidgetStyle;

/**
 * @brief This widget is a way to render text, but in widget form.
 * That allows it to get added to other widgets, such as a VerticalListWidget
 */
class TextWidget : public IWidget
{
public:
    TextWidget();
    virtual ~TextWidget();

    const TextWidgetStyle& getStyle() const;
    void setStyle(const TextWidgetStyle& style);
    
    void setData(const char* data);
    const char* getData() const;

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;
    Dimensions getSize() const override;

    void setConfirmAction(void (*onConfirmAction)(void*), void* context);

    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;
protected:
private:
    TextWidgetStyle style_;
    Rectangle bounds_;
    const char* text_;
    void (*onConfirmAction_)(void*);
    void* onConfirmActionContext_;
    bool focused_;
    bool visible_;
    bool aButtonPressed_;
};

#endif