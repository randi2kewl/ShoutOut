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

#include "juce_PreferencesPanel.h"
#include "../windows/juce_DialogWindow.h"
#include "../../graphics/imaging/juce_ImageCache.h"
#include "../../graphics/drawables/juce_DrawableImage.h"


//==============================================================================
PreferencesPanel::PreferencesPanel()
    : buttonSize (70)
{
}

PreferencesPanel::~PreferencesPanel()
{
}

int PreferencesPanel::getButtonSize() const noexcept
{
    return buttonSize;
}

void PreferencesPanel::setButtonSize (int newSize)
{
    buttonSize = newSize;
    resized();
}

//==============================================================================
void PreferencesPanel::addSettingsPage (const String& title,
                                        const Drawable* icon,
                                        const Drawable* overIcon,
                                        const Drawable* downIcon)
{
    DrawableButton* const button = new DrawableButton (title, DrawableButton::ImageAboveTextLabel);
    buttons.add (button);

    button->setImages (icon, overIcon, downIcon);
    button->setRadioGroupId (1);
    button->addListener (this);
    button->setClickingTogglesState (true);
    button->setWantsKeyboardFocus (false);
    addAndMakeVisible (button);

    resized();

    if (currentPage == nullptr)
        setCurrentPage (title);
}

void PreferencesPanel::addSettingsPage (const String& title, const void* imageData, const int imageDataSize)
{
    DrawableImage icon, iconOver, iconDown;
    icon.setImage (ImageCache::getFromMemory (imageData, imageDataSize));

    iconOver.setImage (ImageCache::getFromMemory (imageData, imageDataSize));
    iconOver.setOverlayColour (Colours::black.withAlpha (0.12f));

    iconDown.setImage (ImageCache::getFromMemory (imageData, imageDataSize));
    iconDown.setOverlayColour (Colours::black.withAlpha (0.25f));

    addSettingsPage (title, &icon, &iconOver, &iconDown);
}

//==============================================================================
void PreferencesPanel::showInDialogBox (const String& dialogTitle, int dialogWidth, int dialogHeight, const Colour& backgroundColour)
{
    setSize (dialogWidth, dialogHeight);
    DialogWindow::showDialog (dialogTitle, this, 0, backgroundColour, false);
}

//==============================================================================
void PreferencesPanel::resized()
{
    for (int i = 0; i < buttons.size(); ++i)
        buttons.getUnchecked(i)->setBounds (i * buttonSize, 0, buttonSize, buttonSize);

    if (currentPage != nullptr)
        currentPage->setBounds (getLocalBounds().withTop (buttonSize + 5));
}

void PreferencesPanel::paint (Graphics& g)
{
    g.setColour (Colours::grey);
    g.fillRect (0, buttonSize + 2, getWidth(), 1);
}

void PreferencesPanel::setCurrentPage (const String& pageName)
{
    if (currentPageName != pageName)
    {
        currentPageName = pageName;

        currentPage = nullptr;
        currentPage = createComponentForPage (pageName);

        if (currentPage != nullptr)
        {
            addAndMakeVisible (currentPage);
            currentPage->toBack();
            resized();
        }

        for (int i = 0; i < buttons.size(); ++i)
        {
            if (buttons.getUnchecked(i)->getName() == pageName)
            {
                buttons.getUnchecked(i)->setToggleState (true, false);
                break;
            }
        }
    }
}

void PreferencesPanel::buttonClicked (Button*)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (buttons.getUnchecked(i)->getToggleState())
        {
            setCurrentPage (buttons.getUnchecked(i)->getName());
            break;
        }
    }
}

END_JUCE_NAMESPACE
