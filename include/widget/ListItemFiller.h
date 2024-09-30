#ifndef _LISTITEMFILLER_H
#define _LISTITEMFILLER_H

#include <vector>
#include <libdragon.h>

/**
 * This template class simply serves the purpose of filling the specified ListType with widgets of the given ListItemWidgetType by creating
 * these list item widgets based on a list of ListDataType entries and a ListItemWidgetStyleType
 * 
 * ListType must have a function called addWidget(ListItemWidgetType)
 * ListItemWidgetStyleType must have a setData(ListDataType) function
 * AND a setStyle(ListItemWidgetStyleType) function
 * 
 * ... in order to be able to use the ListItemFiller template class
 */
template<typename ListType, typename ListDataType, typename ListItemWidgetType, typename ListItemWidgetStyleType>
class ListItemFiller
{
public:
    ListItemFiller(ListType& list)
        : list_(list)
        , widgets_()
    {
    }

    ~ListItemFiller()
    {
        deleteWidgets();
    }

    void addItems(ListDataType* dataList, size_t dataListSize, const ListItemWidgetStyleType& itemStyle)
    {
        ListItemWidgetType* itemWidget;
        for(size_t i = 0; i < dataListSize; ++i)
        {
            itemWidget = new ListItemWidgetType();
            itemWidget->setData(dataList[i]);
            itemWidget->setStyle(itemStyle);
            list_.addWidget(itemWidget);
        }
    }

    void deleteWidgets()
    {
        for(ListItemWidgetType* item : widgets_)
        {
            delete item;
        }
        widgets_.clear();
    }
protected:
private:
    ListType& list_;
    std::vector<ListItemWidgetType*> widgets_;
};

#endif
