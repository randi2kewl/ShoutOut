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

#ifndef __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__
#define __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__

#include "../io/files/juce_File.h"
#include "../gui/components/menus/juce_PopupMenu.h"


//==============================================================================
/**
    Manages a set of files for use as a list of recently-opened documents.

    This is a handy class for holding your list of recently-opened documents, with
    helpful methods for things like purging any non-existent files, automatically
    adding them to a menu, and making persistence easy.

    @see File, FileBasedDocument
*/
class JUCE_API  RecentlyOpenedFilesList
{
public:
    //==============================================================================
    /** Creates an empty list.
    */
    RecentlyOpenedFilesList();

    /** Destructor. */
    ~RecentlyOpenedFilesList();

    //==============================================================================
    /** Sets a limit for the number of files that will be stored in the list.

        When addFile() is called, then if there is no more space in the list, the
        least-recently added file will be dropped.

        @see getMaxNumberOfItems
    */
    void setMaxNumberOfItems (int newMaxNumber);

    /** Returns the number of items that this list will store.
        @see setMaxNumberOfItems
    */
    int getMaxNumberOfItems() const noexcept                            { return maxNumberOfItems; }

    /** Returns the number of files in the list.

        The most recently added file is always at index 0.
    */
    int getNumFiles() const;

    /** Returns one of the files in the list.

        The most recently added file is always at index 0.
    */
    File getFile (int index) const;

    /** Returns an array of all the absolute pathnames in the list.
    */
    const StringArray& getAllFilenames() const noexcept                 { return files; }

    /** Clears all the files from the list. */
    void clear();

    /** Adds a file to the list.

        The file will be added at index 0. If this file is already in the list, it will
        be moved up to index 0, but a file can only appear once in the list.

        If the list already contains the maximum number of items that is permitted, the
        least-recently added file will be dropped from the end.
    */
    void addFile (const File& file);

    /** Removes a file from the list. */
    void removeFile (const File& file);

    /** Checks each of the files in the list, removing any that don't exist.

        You might want to call this after reloading a list of files, or before putting them
        on a menu.
    */
    void removeNonExistentFiles();

    //==============================================================================
    /** Adds entries to a menu, representing each of the files in the list.

        This is handy for creating an "open recent file..." menu in your app. The
        menu items are numbered consecutively starting with the baseItemId value,
        and can either be added as complete pathnames, or just the last part of the
        filename.

        If dontAddNonExistentFiles is true, then each file will be checked and only those
        that exist will be added.

        If filesToAvoid is non-zero, then it is considered to be a zero-terminated array of
        pointers to file objects. Any files that appear in this list will not be added to the
        menu - the reason for this is that you might have a number of files already open, so
        might not want these to be shown in the menu.

        It returns the number of items that were added.
    */
    int createPopupMenuItems (PopupMenu& menuToAddItemsTo,
                              int baseItemId,
                              bool showFullPaths,
                              bool dontAddNonExistentFiles,
                              const File** filesToAvoid = nullptr);

    //==============================================================================
    /** Returns a string that encapsulates all the files in the list.

        The string that is returned can later be passed into restoreFromString() in
        order to recreate the list. This is handy for persisting your list, e.g. in
        a PropertiesFile object.

        @see restoreFromString
    */
    String toString() const;

    /** Restores the list from a previously stringified version of the list.

        Pass in a stringified version created with toString() in order to persist/restore
        your list.

        @see toString
    */
    void restoreFromString (const String& stringifiedVersion);


private:
    //==============================================================================
    StringArray files;
    int maxNumberOfItems;

    JUCE_LEAK_DETECTOR (RecentlyOpenedFilesList);
};


#endif   // __JUCE_RECENTLYOPENEDFILESLIST_JUCEHEADER__
