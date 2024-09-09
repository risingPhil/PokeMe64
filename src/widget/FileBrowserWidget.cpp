#include "widget/FileBrowserWidget.h"
#include <errno.h>

static void confirmDirectoryCallback(void* context, const void* itemParam)
{
    auto fileBrowser = (FileBrowserWidget*)context;
    fileBrowser->onConfirmDirectory((const char*)itemParam);
}

static void confirmFileCallback(void* context, const void* itemParam)
{
    auto fileBrowser = (FileBrowserWidget*)context;
    fileBrowser->onConfirmFile((const char*)itemParam);
}

FileBrowserWidget::FileBrowserWidget(AnimationManager& animManager)
    : duplicatedDirEntNameList_()
    , itemWidgetList_()
    , listWidget_(animManager)
    , style_({0})
    , status_({
        .itemList = itemWidgetList_,
        .err = 0
    })
    , bounds_({0})
    , pathBuffer_()
    , onItemConfirmedCallback_(nullptr)
    , onItemConfirmedCallbackContext_(nullptr)
    , fileExtensionFilter_(nullptr)
    , focused_(false)
    , visible_(true)
    , bButtonPressed_(false)
{
    pathBuffer_[0] = '\0';
}

FileBrowserWidget::~FileBrowserWidget()
{
    clearList();
}

bool FileBrowserWidget::isFocused() const
{
    return focused_;
}

void FileBrowserWidget::setFocused(bool focused)
{
    focused_ = focused;
    listWidget_.setFocused(focused);
}

bool FileBrowserWidget::isVisible() const
{
    return visible_;
}

void FileBrowserWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle FileBrowserWidget::getBounds() const
{
    return bounds_;
}

void FileBrowserWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
    listWidget_.setBounds(Rectangle{0, 0, bounds.width, bounds.height});
}

Dimensions FileBrowserWidget::getSize() const
{
    return Dimensions{bounds_.width, bounds_.height};
}

void FileBrowserWidget::setStyle(const FileBrowserWidgetStyle& style)
{
    style_ = style;

    const VerticalListStyle listStyle = {
        .background = {
            .sprite = style.background.sprite,
            .spriteSettings = style.background.renderSettings
        },
        .margin = {
            .left = style.margin.left,
            .right = style.margin.right,
            .top = style.margin.top,
            .bottom = style.margin.bottom
        },
    };
    listWidget_.setStyle(listStyle);
}

bool FileBrowserWidget::handleUserInput(const joypad_inputs_t& userInput)
{
    bool handled = listWidget_.handleUserInput(userInput);

    if(!handled)
    {
        if(userInput.btn.b)
        {
            if(!bButtonPressed_)
            {
                bButtonPressed_ = true;
                handled = true;
            }
        }
        else
        {
            if(bButtonPressed_)
            {
                bButtonPressed_ = false;
                handled = goToParentDirectory();
            }
        }
    }
    return handled;
}

void FileBrowserWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    const Rectangle absoluteBounds = addOffset(bounds_, parentBounds);

    listWidget_.render(gfx, absoluteBounds);
}

const FileBrowserWidgetStatus& FileBrowserWidget::getStatus() const
{
    return status_;
}

const char* FileBrowserWidget::getPath() const
{
    return pathBuffer_;
}

void FileBrowserWidget::setPath(const char* path)
{
    strncpy(pathBuffer_, path, sizeof(pathBuffer_) - 1);

    clearList();
    loadDirectoryItems();
}

void FileBrowserWidget::onConfirmDirectory(const char* path)
{
    size_t pathLength = strnlen(pathBuffer_, sizeof(pathBuffer_));
    if(pathBuffer_[pathLength - 1] != '/')
    {
        pathBuffer_[pathLength] = '/';
        ++pathLength;
    }
    strncpy(pathBuffer_ + pathLength, path, sizeof(pathBuffer_) - 1 - pathLength);

    // reload items
    clearList();
    loadDirectoryItems();
}

void FileBrowserWidget::onConfirmFile(const char* path)
{
    char fullPath[sizeof(pathBuffer_)];
    size_t pathLength = strnlen(pathBuffer_, sizeof(pathBuffer_));

    memcpy(fullPath, pathBuffer_, sizeof(pathBuffer_));

    if(fullPath[pathLength - 1] != '/')
    {
        fullPath[pathLength] = '/';
        ++pathLength;
    }
    strncpy(fullPath + pathLength, path, sizeof(fullPath) - 1 - pathLength);

    if(onItemConfirmedCallback_)
    {
        onItemConfirmedCallback_(onItemConfirmedCallbackContext_, fullPath);
    }
}

void FileBrowserWidget::setItemConfirmedCallback(void (*onItemConfirmed)(void*, const char*), void* context)
{
    onItemConfirmedCallback_ = onItemConfirmed;
    onItemConfirmedCallbackContext_ = context;
}

void FileBrowserWidget::setFileExtensionToFilter(const char* fileExtensionFilter)
{
    fileExtensionFilter_ = fileExtensionFilter;

    // reload items
    clearList();
    loadDirectoryItems();
}

void FileBrowserWidget::clearList()
{
    listWidget_.clearWidgets();
    for(MenuItemWidget* widget : itemWidgetList_)
    {
        delete widget;
    }
    itemWidgetList_.clear();

    for(char* dirEntName : duplicatedDirEntNameList_)
    {
        free(dirEntName);
    }
    duplicatedDirEntNameList_.clear();
}

void FileBrowserWidget::loadDirectoryItems()
{
    MenuItemWidget* itemWidget;
    MenuItemData itemData;
    MenuItemStyle itemStyle = style_.itemStyle;
    dir_t dirEnt;
    int ret;
    char* titleString;
    size_t fileExtensionFilterLength;
    size_t dirNameLength;

    if(pathBuffer_[0] == '\0')
    {
        // empty path. don't do anything
        return;
    }

    ret = dir_findfirst(pathBuffer_, &dirEnt);
    if(ret != 0)
    {
        status_.err = errno;
        return;
    }

    fileExtensionFilterLength = (fileExtensionFilter_) ? strlen(fileExtensionFilter_) : 0;

    while(ret == 0)
    {
        dirNameLength = strnlen(dirEnt.d_name, sizeof(dirEnt.d_name));

        // apply file extension filter if one has been specified
        if(dirEnt.d_type == DT_REG && fileExtensionFilter_ && dirNameLength > fileExtensionFilterLength)
        {
            if(strncmp(dirEnt.d_name + dirNameLength - fileExtensionFilterLength, fileExtensionFilter_, fileExtensionFilterLength))
            {
                // file extension doesn't matching, discard result
                ret = dir_findnext(pathBuffer_, &dirEnt);
                continue;
            }
        }
        // libdragon overwrites a dir_t instance on every dir_findnext call
        // and the dir_t instance has a static allocated d_name entry
        // which means it gets overwritten on every dir_findnext() call.
        // so to avoid losing the name, we need to duplicate it.
        titleString = strdup(dirEnt.d_name);
        // we also need to track this duplicated string in order to free() it when done
        duplicatedDirEntNameList_.push_back(titleString);

        itemData.title = titleString;
        itemData.onConfirmAction = (dirEnt.d_type == DT_REG) ? confirmFileCallback : confirmDirectoryCallback;
        itemData.context = this;
        itemData.itemParam = titleString;

        itemWidget = new MenuItemWidget();
        itemWidget->setData(itemData);
        itemWidget->setStyle(itemStyle);
        listWidget_.addWidget(itemWidget);
        // we are responsible for delete'ing the MenuItemWidget instance
        // so we need to keep track of it.
        itemWidgetList_.push_back(itemWidget);

        ret = dir_findnext(pathBuffer_, &dirEnt);
    }
}

bool FileBrowserWidget::goToParentDirectory()
{
    bool gotMoreThanOneForwardSlash = false;
    const size_t pathLength = strlen(pathBuffer_);

    // figure out the last forward slash offset AND whether there's more than one forward slash
    char* cur = pathBuffer_ + pathLength;
    char* lastSlash = nullptr;
    while(cur >= pathBuffer_)
    {
        if(*cur == '/')
        {
            if(!lastSlash)
            {
                lastSlash = cur;
            }
            else
            {
                gotMoreThanOneForwardSlash = true;
                break;
            }
        }
        --cur;
    }

    if(!gotMoreThanOneForwardSlash)
    {
        // We're either in the first subdirectory level (example: sd:/Wallpapers)
        // OR we're already in the root dir (sd:/)
        if(pathBuffer_[pathLength - 1] == '/')
        {
            // already in root dir
            return false;
        }
        else
        {
            // in 1st level subdir
            // finish path string directly after the forward slash
            *(lastSlash + 1) = '\0';
        }
    }
    else
    {
        // subdir deeper than 1st level: (example: sd:/Music/Europe)
        // replace the last forward slash with a 0 character to end the string there
        *lastSlash = '\0';
    }

    // now reload the FileBrowserWidget items.
    clearList();
    loadDirectoryItems();
    return true;
}