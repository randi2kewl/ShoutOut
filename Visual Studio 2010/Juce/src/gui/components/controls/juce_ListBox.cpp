/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "../../../core/juce_StandardHeader.h"

BEGIN_JUCE_NAMESPACE

#include "juce_ListBox.h"
#include "../../graphics/geometry/juce_RectangleList.h"
#include "../../graphics/imaging/juce_Image.h"
#include "../mouse/juce_DragAndDropContainer.h"


//==============================================================================
class ListBoxRowComponent  : public Component,
                             public TooltipClient
{
public:
    ListBoxRowComponent (ListBox& owner_)
        : owner (owner_), row (-1),
          selected (false), isDragging (false), selectRowOnMouseUp (false)
    {
    }

    void paint (Graphics& g)
    {
        if (owner.getModel() != nullptr)
            owner.getModel()->paintListBoxItem (row, g, getWidth(), getHeight(), selected);
    }

    void update (const int row_, const bool selected_)
    {
        if (row != row_ || selected != selected_)
        {
            repaint();
            row = row_;
            selected = selected_;
        }

        if (owner.getModel() != nullptr)
        {
            customComponent = owner.getModel()->refreshComponentForRow (row_, selected_, customComponent.release());

            if (customComponent != nullptr)
            {
                addAndMakeVisible (customComponent);
                customComponent->setBounds (getLocalBounds());
            }
        }
    }

    void mouseDown (const MouseEvent& e)
    {
        isDragging = false;
        selectRowOnMouseUp = false;

        if (isEnabled())
        {
            if (! selected)
            {
                owner.selectRowsBasedOnModifierKeys (row, e.mods, false);

                if (owner.getModel() != nullptr)
                    owner.getModel()->listBoxItemClicked (row, e);
            }
            else
            {
                selectRowOnMouseUp = true;
            }
        }
    }

    void mouseUp (const MouseEvent& e)
    {
        if (isEnabled() && selectRowOnMouseUp && ! isDragging)
        {
            owner.selectRowsBasedOnModifierKeys (row, e.mods, true);

            if (owner.getModel() != nullptr)
                owner.getModel()->listBoxItemClicked (row, e);
        }
    }

    void mouseDoubleClick (const MouseEvent& e)
    {
        if (owner.getModel() != nullptr && isEnabled())
            owner.getModel()->listBoxItemDoubleClicked (row, e);
    }

    void mouseDrag (const MouseEvent& e)
    {
        if (isEnabled() && owner.getModel() != nullptr && ! (e.mouseWasClicked() || isDragging))
        {
            const SparseSet<int> selectedRows (owner.getSelectedRows());

            if (selectedRows.size() > 0)
            {
                const var dragDescription (owner.getModel()->getDragSourceDescription (selectedRows));

                if (! (dragDescription.isVoid() || (dragDescription.isString() && dragDescription.toString().isEmpty())))
                {
                    isDragging = true;
                    owner.startDragAndDrop (e, dragDescription);
                }
            }
        }
    }

    void resized()
    {
        if (customComponent != nullptr)
            customComponent->setBounds (getLocalBounds());
    }

    const String getTooltip()
    {
        if (owner.getModel() != nullptr)
            return owner.getModel()->getTooltipForRow (row);

        return String::empty;
    }

    ScopedPointer<Component> customComponent;

private:
    ListBox& owner;
    int row;
    bool selected, isDragging, selectRowOnMouseUp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListBoxRowComponent);
};


//==============================================================================
class ListViewport  : public Viewport
{
public:
    //==============================================================================
    ListViewport (ListBox& owner_)
        : owner (owner_)
    {
        setWantsKeyboardFocus (false);

        Component* const content = new Component();
        setViewedComponent (content);
        content->addMouseListener (this, false);
        content->setWantsKeyboardFocus (false);
    }

    ListBoxRowComponent* getComponentForRow (const int row) const noexcept
    {
        return rows [row % jmax (1, rows.size())];
    }

    ListBoxRowComponent* getComponentForRowIfOnscreen (const int row) const noexcept
    {
        return (row >= firstIndex && row < firstIndex + rows.size())
                 ? getComponentForRow (row) : nullptr;
    }

    int getRowNumberOfComponent (Component* const rowComponent) const noexcept
    {
        const int index = getIndexOfChildComponent (rowComponent);
        const int num = rows.size();

        for (int i = num; --i >= 0;)
            if (((firstIndex + i) % jmax (1, num)) == index)
                return firstIndex + i;

        return -1;
    }

    void visibleAreaChanged (const Rectangle<int>&)
    {
        updateVisibleArea (true);

        if (owner.getModel() != nullptr)
            owner.getModel()->listWasScrolled();
    }

    void updateVisibleArea (const bool makeSureItUpdatesContent)
    {
        hasUpdated = false;

        const int newX = getViewedComponent()->getX();
        int newY = getViewedComponent()->getY();
        const int newW = jmax (owner.minimumRowWidth, getMaximumVisibleWidth());
        const int newH = owner.totalItems * owner.getRowHeight();

        if (newY + newH < getMaximumVisibleHeight() && newH > getMaximumVisibleHeight())
            newY = getMaximumVisibleHeight() - newH;

        getViewedComponent()->setBounds (newX, newY, newW, newH);

        if (makeSureItUpdatesContent && ! hasUpdated)
            updateContents();
    }

    void updateContents()
    {
        hasUpdated = true;
        const int rowHeight = owner.getRowHeight();

        if (rowHeight > 0)
        {
            const int y = getViewPositionY();
            const int w = getViewedComponent()->getWidth();

            const int numNeeded = 2 + getMaximumVisibleHeight() / rowHeight;
            rows.removeRange (numNeeded, rows.size());

            while (numNeeded > rows.size())
            {
                ListBoxRowComponent* newRow = new ListBoxRowComponent (owner);
                rows.add (newRow);
                getViewedComponent()->addAndMakeVisible (newRow);
            }

            firstIndex = y / rowHeight;
            firstWholeIndex = (y + rowHeight - 1) / rowHeight;
            lastWholeIndex = (y + getMaximumVisibleHeight() - 1) / rowHeight;

            for (int i = 0; i < numNeeded; ++i)
            {
                const int row = i + firstIndex;
                ListBoxRowComponent* const rowComp = getComponentForRow (row);

                if (rowComp != nullptr)
                {
                    rowComp->setBounds (0, row * rowHeight, w, rowHeight);
                    rowComp->update (row, owner.isRowSelected (row));
                }
            }
        }

        if (owner.headerComponent != nullptr)
            owner.headerComponent->setBounds (owner.outlineThickness + getViewedComponent()->getX(),
                                              owner.outlineThickness,
                                              jmax (owner.getWidth() - owner.outlineThickness * 2,
                                                    getViewedComponent()->getWidth()),
                                              owner.headerComponent->getHeight());
    }

    void selectRow (const int row, const int rowHeight, const bool dontScroll,
                    const int lastRowSelected, const int totalItems, const bool isMouseClick)
    {
        hasUpdated = false;

        if (row < firstWholeIndex && ! dontScroll)
        {
            setViewPosition (getViewPositionX(), row * rowHeight);
        }
        else if (row >= lastWholeIndex && ! dontScroll)
        {
            const int rowsOnScreen = lastWholeIndex - firstWholeIndex;

            if (row >= lastRowSelected + rowsOnScreen
                 && rowsOnScreen < totalItems - 1
                 && ! isMouseClick)
            {
                setViewPosition (getViewPositionX(),
                                 jlimit (0, jmax (0, totalItems - rowsOnScreen), row) * rowHeight);
            }
            else
            {
                setViewPosition (getViewPositionX(),
                                 jmax (0, (row  + 1) * rowHeight - getMaximumVisibleHeight()));
            }
        }

        if (! hasUpdated)
            updateContents();
    }

    void scrollToEnsureRowIsOnscreen (const int row, const int rowHeight)
    {
        if (row < firstWholeIndex)
        {
            setViewPosition (getViewPositionX(), row * rowHeight);
        }
        else if (row >= lastWholeIndex)
        {
            setViewPosition (getViewPositionX(),
                             jmax (0, (row  + 1) * rowHeight - getMaximumVisibleHeight()));
        }
    }

    void paint (Graphics& g)
    {
        if (isOpaque())
            g.fillAll (owner.findColour (ListBox::backgroundColourId));
    }

    bool keyPressed (const KeyPress& key)
    {
        if (key.isKeyCode (KeyPress::upKey)
            || key.isKeyCode (KeyPress::downKey)
            || key.isKeyCode (KeyPress::pageUpKey)
            || key.isKeyCode (KeyPress::pageDownKey)
            || key.isKeyCode (KeyPress::homeKey)
            || key.isKeyCode (KeyPress::endKey))
        {
            // we want to avoid these keypresses going to the viewport, and instead allow
            // them to pass up to our listbox..
            return false;
        }

        return Viewport::keyPressed (key);
    }

private:
    ListBox& owner;
    OwnedArray<ListBoxRowComponent> rows;
    int firstIndex, firstWholeIndex, lastWholeIndex;
    bool hasUpdated;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListViewport);
};


//==============================================================================
ListBox::ListBox (const String& name, ListBoxModel* const model_)
    : Component (name),
      model (model_),
      totalItems (0),
      rowHeight (22),
      minimumRowWidth (0),
      outlineThickness (0),
      lastRowSelected (-1),
      mouseMoveSelects (false),
      multipleSelection (false),
      hasDoneInitialUpdate (false)
{
    addAndMakeVisible (viewport = new ListViewport (*this));

    ListBox::setWantsKeyboardFocus (true);
    ListBox::colourChanged();
}

ListBox::~ListBox()
{
    headerComponent = nullptr;
    viewport = nullptr;
}

void ListBox::setModel (ListBoxModel* const newModel)
{
    if (model != newModel)
    {
        model = newModel;
        repaint();
        updateContent();
    }
}

void ListBox::setMultipleSelectionEnabled (bool b)
{
    multipleSelection = b;
}

void ListBox::setMouseMoveSelectsRows (bool b)
{
    mouseMoveSelects = b;

    if (b)
        addMouseListener (this, true);
}

//==============================================================================
void ListBox::paint (Graphics& g)
{
    if (! hasDoneInitialUpdate)
        updateContent();

    g.fillAll (findColour (backgroundColourId));
}

void ListBox::paintOverChildren (Graphics& g)
{
    if (outlineThickness > 0)
    {
        g.setColour (findColour (outlineColourId));
        g.drawRect (0, 0, getWidth(), getHeight(), outlineThickness);
    }
}

void ListBox::resized()
{
    viewport->setBoundsInset (BorderSize<int> (outlineThickness + ((headerComponent != nullptr) ? headerComponent->getHeight() : 0),
                                               outlineThickness, outlineThickness, outlineThickness));

    viewport->setSingleStepSizes (20, getRowHeight());

    viewport->updateVisibleArea (false);
}

void ListBox::visibilityChanged()
{
    viewport->updateVisibleArea (true);
}

Viewport* ListBox::getViewport() const noexcept
{
    return viewport;
}

//==============================================================================
void ListBox::updateContent()
{
    hasDoneInitialUpdate = true;
    totalItems = (model != nullptr) ? model->getNumRows() : 0;

    bool selectionChanged = false;

    if (selected.size() > 0 && selected [selected.size() - 1] >= totalItems)
    {
        selected.removeRange (Range <int> (totalItems, std::numeric_limits<int>::max()));
        lastRowSelected = getSelectedRow (0);
        selectionChanged = true;
    }

    viewport->updateVisibleArea (isVisible());
    viewport->resized();

    if (selectionChanged && model != nullptr)
        model->selectedRowsChanged (lastRowSelected);
}

//==============================================================================
void ListBox::selectRow (const int row,
                         bool dontScroll,
                         bool deselectOthersFirst)
{
    selectRowInternal (row, dontScroll, deselectOthersFirst, false);
}

void ListBox::selectRowInternal (const int row,
                                 bool dontScroll,
                                 bool deselectOthersFirst,
                                 bool isMouseClick)
{
    if (! multipleSelection)
        deselectOthersFirst = true;

    if ((! isRowSelected (row))
         || (deselectOthersFirst && getNumSelectedRows() > 1))
    {
        if (isPositiveAndBelow (row, totalItems))
        {
            if (deselectOthersFirst)
                selected.clear();

            selected.addRange (Range<int> (row, row + 1));

            if (getHeight() == 0 || getWidth() == 0)
                dontScroll = true;

            viewport->selectRow (row, getRowHeight(), dontScroll,
                                 lastRowSelected, totalItems, isMouseClick);

            lastRowSelected = row;
            model->selectedRowsChanged (row);
        }
        else
        {
            if (deselectOthersFirst)
                deselectAllRows();
        }
    }
}

void ListBox::deselectRow (const int row)
{
    if (selected.contains (row))
    {
        selected.removeRange (Range <int> (row, row + 1));

        if (row == lastRowSelected)
            lastRowSelected = getSelectedRow (0);

        viewport->updateContents();
        model->selectedRowsChanged (lastRowSelected);
    }
}

void ListBox::setSelectedRows (const SparseSet<int>& setOfRowsToBeSelected,
                               const bool sendNotificationEventToModel)
{
    selected = setOfRowsToBeSelected;
    selected.removeRange (Range <int> (totalItems, std::numeric_limits<int>::max()));

    if (! isRowSelected (lastRowSelected))
        lastRowSelected = getSelectedRow (0);

    viewport->updateContents();

    if ((model != nullptr) && sendNotificationEventToModel)
        model->selectedRowsChanged (lastRowSelected);
}

const SparseSet<int> ListBox::getSelectedRows() const
{
    return selected;
}

void ListBox::selectRangeOfRows (int firstRow, int lastRow)
{
    if (multipleSelection && (firstRow != lastRow))
    {
        const int numRows = totalItems - 1;
        firstRow = jlimit (0, jmax (0, numRows), firstRow);
        lastRow = jlimit (0, jmax (0, numRows), lastRow);

        selected.addRange (Range <int> (jmin (firstRow, lastRow),
                                        jmax (firstRow, lastRow) + 1));

        selected.removeRange (Range <int> (lastRow, lastRow + 1));
    }

    selectRowInternal (lastRow, false, false, true);
}

void ListBox::flipRowSelection (const int row)
{
    if (isRowSelected (row))
        deselectRow (row);
    else
        selectRowInternal (row, false, false, true);
}

void ListBox::deselectAllRows()
{
    if (! selected.isEmpty())
    {
        selected.clear();
        lastRowSelected = -1;

        viewport->updateContents();

        if (model != nullptr)
            model->selectedRowsChanged (lastRowSelected);
    }
}

void ListBox::selectRowsBasedOnModifierKeys (const int row,
                                             const ModifierKeys& mods,
                                             const bool isMouseUpEvent)
{
    if (multipleSelection && mods.isCommandDown())
    {
        flipRowSelection (row);
    }
    else if (multipleSelection && mods.isShiftDown() && lastRowSelected >= 0)
    {
        selectRangeOfRows (lastRowSelected, row);
    }
    else if ((! mods.isPopupMenu()) || ! isRowSelected (row))
    {
        selectRowInternal (row, false, ! (multipleSelection && (! isMouseUpEvent) && isRowSelected (row)), true);
    }
}

int ListBox::getNumSelectedRows() const
{
    return selected.size();
}

int ListBox::getSelectedRow (const int index) const
{
    return (isPositiveAndBelow (index, selected.size()))
                ? selected [index] : -1;
}

bool ListBox::isRowSelected (const int row) const
{
    return selected.contains (row);
}

int ListBox::getLastRowSelected() const
{
    return (isRowSelected (lastRowSelected)) ? lastRowSelected : -1;
}

//==============================================================================
int ListBox::getRowContainingPosition (const int x, const int y) const noexcept
{
    if (isPositiveAndBelow (x, getWidth()))
    {
        const int row = (viewport->getViewPositionY() + y - viewport->getY()) / rowHeight;

        if (isPositiveAndBelow (row, totalItems))
            return row;
    }

    return -1;
}

int ListBox::getInsertionIndexForPosition (const int x, const int y) const noexcept
{
    if (isPositiveAndBelow (x, getWidth()))
    {
        const int row = (viewport->getViewPositionY() + y + rowHeight / 2 - viewport->getY()) / rowHeight;
        return jlimit (0, totalItems, row);
    }

    return -1;
}

Component* ListBox::getComponentForRowNumber (const int row) const noexcept
{
    ListBoxRowComponent* const listRowComp = viewport->getComponentForRowIfOnscreen (row);
    return listRowComp != nullptr ? static_cast <Component*> (listRowComp->customComponent) : nullptr;
}

int ListBox::getRowNumberOfComponent (Component* const rowComponent) const noexcept
{
    return viewport->getRowNumberOfComponent (rowComponent);
}

const Rectangle<int> ListBox::getRowPosition (const int rowNumber,
                                              const bool relativeToComponentTopLeft) const noexcept
{
    int y = viewport->getY() + rowHeight * rowNumber;

    if (relativeToComponentTopLeft)
        y -= viewport->getViewPositionY();

    return Rectangle<int> (viewport->getX(), y,
                           viewport->getViewedComponent()->getWidth(), rowHeight);
}

void ListBox::setVerticalPosition (const double proportion)
{
    const int offscreen = viewport->getViewedComponent()->getHeight() - viewport->getHeight();

    viewport->setViewPosition (viewport->getViewPositionX(),
                               jmax (0, roundToInt (proportion * offscreen)));
}

double ListBox::getVerticalPosition() const
{
    const int offscreen = viewport->getViewedComponent()->getHeight() - viewport->getHeight();

    return (offscreen > 0) ? viewport->getViewPositionY() / (double) offscreen
                           : 0;
}

int ListBox::getVisibleRowWidth() const noexcept
{
    return viewport->getViewWidth();
}

void ListBox::scrollToEnsureRowIsOnscreen (const int row)
{
    viewport->scrollToEnsureRowIsOnscreen (row, getRowHeight());
}

//==============================================================================
bool ListBox::keyPressed (const KeyPress& key)
{
    const int numVisibleRows = viewport->getHeight() / getRowHeight();

    const bool multiple = multipleSelection
                            && (lastRowSelected >= 0)
                            && (key.getModifiers().isShiftDown()
                                 || key.getModifiers().isCtrlDown()
                                 || key.getModifiers().isCommandDown());

    if (key.isKeyCode (KeyPress::upKey))
    {
        if (multiple)
            selectRangeOfRows (lastRowSelected, lastRowSelected - 1);
        else
            selectRow (jmax (0, lastRowSelected - 1));
    }
    else if (key.isKeyCode (KeyPress::returnKey)
              && isRowSelected (lastRowSelected))
    {
        if (model != nullptr)
            model->returnKeyPressed (lastRowSelected);
    }
    else if (key.isKeyCode (KeyPress::pageUpKey))
    {
        if (multiple)
            selectRangeOfRows (lastRowSelected, lastRowSelected - numVisibleRows);
        else
            selectRow (jmax (0, jmax (0, lastRowSelected) - numVisibleRows));
    }
    else if (key.isKeyCode (KeyPress::pageDownKey))
    {
        if (multiple)
            selectRangeOfRows (lastRowSelected, lastRowSelected + numVisibleRows);
        else
            selectRow (jmin (totalItems - 1, jmax (0, lastRowSelected) + numVisibleRows));
    }
    else if (key.isKeyCode (KeyPress::homeKey))
    {
        if (multiple && key.getModifiers().isShiftDown())
            selectRangeOfRows (lastRowSelected, 0);
        else
            selectRow (0);
    }
    else if (key.isKeyCode (KeyPress::endKey))
    {
        if (multiple && key.getModifiers().isShiftDown())
            selectRangeOfRows (lastRowSelected, totalItems - 1);
        else
            selectRow (totalItems - 1);
    }
    else if (key.isKeyCode (KeyPress::downKey))
    {
        if (multiple)
            selectRangeOfRows (lastRowSelected, lastRowSelected + 1);
        else
            selectRow (jmin (totalItems - 1, jmax (0, lastRowSelected) + 1));
    }
    else if ((key.isKeyCode (KeyPress::deleteKey) || key.isKeyCode (KeyPress::backspaceKey))
               && isRowSelected (lastRowSelected))
    {
        if (model != nullptr)
            model->deleteKeyPressed (lastRowSelected);
    }
    else if (multiple && key == KeyPress ('a', ModifierKeys::commandModifier, 0))
    {
        selectRangeOfRows (0, std::numeric_limits<int>::max());
    }
    else
    {
        return false;
    }

    return true;
}

bool ListBox::keyStateChanged (const bool isKeyDown)
{
    return isKeyDown
            && (KeyPress::isKeyCurrentlyDown (KeyPress::upKey)
                || KeyPress::isKeyCurrentlyDown (KeyPress::pageUpKey)
                || KeyPress::isKeyCurrentlyDown (KeyPress::downKey)
                || KeyPress::isKeyCurrentlyDown (KeyPress::pageDownKey)
                || KeyPress::isKeyCurrentlyDown (KeyPress::homeKey)
                || KeyPress::isKeyCurrentlyDown (KeyPress::endKey)
                || KeyPress::isKeyCurrentlyDown (KeyPress::returnKey));
}

void ListBox::mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
{
    bool eventWasUsed = false;

    if (viewport->getHorizontalScrollBar()->isVisible() && wheelIncrementX != 0)
    {
        eventWasUsed = true;
        viewport->getHorizontalScrollBar()->mouseWheelMove (e, wheelIncrementX, 0);
    }

    if (viewport->getVerticalScrollBar()->isVisible() && wheelIncrementY != 0)
    {
        eventWasUsed = true;
        viewport->getVerticalScrollBar()->mouseWheelMove (e, 0, wheelIncrementY);
    }

    if (! eventWasUsed)
        Component::mouseWheelMove (e, wheelIncrementX, wheelIncrementY);
}

void ListBox::mouseMove (const MouseEvent& e)
{
    if (mouseMoveSelects)
    {
        const MouseEvent e2 (e.getEventRelativeTo (this));
        selectRow (getRowContainingPosition (e2.x, e2.y), true);
    }
}

void ListBox::mouseExit (const MouseEvent& e)
{
    mouseMove (e);
}

void ListBox::mouseUp (const MouseEvent& e)
{
    if (e.mouseWasClicked() && model != nullptr)
        model->backgroundClicked();
}

//==============================================================================
void ListBox::setRowHeight (const int newHeight)
{
    rowHeight = jmax (1, newHeight);
    viewport->setSingleStepSizes (20, rowHeight);
    updateContent();
}

int ListBox::getNumRowsOnScreen() const noexcept
{
    return viewport->getMaximumVisibleHeight() / rowHeight;
}

void ListBox::setMinimumContentWidth (const int newMinimumWidth)
{
    minimumRowWidth = newMinimumWidth;
    updateContent();
}

int ListBox::getVisibleContentWidth() const noexcept
{
    return viewport->getMaximumVisibleWidth();
}

ScrollBar* ListBox::getVerticalScrollBar() const noexcept
{
    return viewport->getVerticalScrollBar();
}

ScrollBar* ListBox::getHorizontalScrollBar() const noexcept
{
    return viewport->getHorizontalScrollBar();
}

void ListBox::colourChanged()
{
    setOpaque (findColour (backgroundColourId).isOpaque());
    viewport->setOpaque (isOpaque());
    repaint();
}

void ListBox::setOutlineThickness (const int outlineThickness_)
{
    outlineThickness = outlineThickness_;
    resized();
}

void ListBox::setHeaderComponent (Component* const newHeaderComponent)
{
    if (headerComponent != newHeaderComponent)
    {
        headerComponent = newHeaderComponent;

        addAndMakeVisible (newHeaderComponent);
        ListBox::resized();
    }
}

void ListBox::repaintRow (const int rowNumber) noexcept
{
    repaint (getRowPosition (rowNumber, true));
}

const Image ListBox::createSnapshotOfSelectedRows (int& imageX, int& imageY)
{
    Rectangle<int> imageArea;
    const int firstRow = getRowContainingPosition (0, 0);

    int i;
    for (i = getNumRowsOnScreen() + 2; --i >= 0;)
    {
        Component* rowComp = viewport->getComponentForRowIfOnscreen (firstRow + i);

        if (rowComp != nullptr && isRowSelected (firstRow + i))
        {
            const Point<int> pos (getLocalPoint (rowComp, Point<int>()));
            const Rectangle<int> rowRect (pos.getX(), pos.getY(), rowComp->getWidth(), rowComp->getHeight());
            imageArea = imageArea.getUnion (rowRect);
        }
    }

    imageArea = imageArea.getIntersection (getLocalBounds());
    imageX = imageArea.getX();
    imageY = imageArea.getY();
    Image snapshot (Image::ARGB, imageArea.getWidth(), imageArea.getHeight(), true, Image::NativeImage);

    for (i = getNumRowsOnScreen() + 2; --i >= 0;)
    {
        Component* rowComp = viewport->getComponentForRowIfOnscreen (firstRow + i);

        if (rowComp != nullptr && isRowSelected (firstRow + i))
        {
            const Point<int> pos (getLocalPoint (rowComp, Point<int>()));

            Graphics g (snapshot);
            g.setOrigin (pos.getX() - imageX, pos.getY() - imageY);

            if (g.reduceClipRegion (rowComp->getLocalBounds()))
            {
                g.beginTransparencyLayer (0.6f);
                rowComp->paintEntireComponent (g, false);
                g.endTransparencyLayer();
            }
        }
    }

    return snapshot;
}

void ListBox::startDragAndDrop (const MouseEvent& e, const var& dragDescription, bool allowDraggingToOtherWindows)
{
    DragAndDropContainer* const dragContainer
        = DragAndDropContainer::findParentDragContainerFor (this);

    if (dragContainer != nullptr)
    {
        int x, y;
        Image dragImage (createSnapshotOfSelectedRows (x, y));

        MouseEvent e2 (e.getEventRelativeTo (this));
        const Point<int> p (x - e2.x, y - e2.y);
        dragContainer->startDragging (dragDescription, this, dragImage, allowDraggingToOtherWindows, &p);
    }
    else
    {
        // to be able to do a drag-and-drop operation, the listbox needs to
        // be inside a component which is also a DragAndDropContainer.
        jassertfalse;
    }
}

//==============================================================================
Component* ListBoxModel::refreshComponentForRow (int, bool, Component* existingComponentToUpdate)
{
    (void) existingComponentToUpdate;
    jassert (existingComponentToUpdate == nullptr); // indicates a failure in the code the recycles the components
    return nullptr;
}

void ListBoxModel::listBoxItemClicked (int, const MouseEvent&) {}
void ListBoxModel::listBoxItemDoubleClicked (int, const MouseEvent&) {}
void ListBoxModel::backgroundClicked() {}
void ListBoxModel::selectedRowsChanged (int) {}
void ListBoxModel::deleteKeyPressed (int) {}
void ListBoxModel::returnKeyPressed (int) {}
void ListBoxModel::listWasScrolled() {}
const var ListBoxModel::getDragSourceDescription (const SparseSet<int>&)    { return var::null; }
const String ListBoxModel::getTooltipForRow (int)                           { return String::empty; }


END_JUCE_NAMESPACE
