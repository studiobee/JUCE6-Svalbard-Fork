/*
  ==============================================================================

   This file is part of the JUCE 6 technical preview.
   Copyright (c) 2020 - Raw Material Software Limited

   You may use this code under the terms of the GPL v3
   (see www.gnu.org/licenses).

   For this technical preview, this file is not subject to commercial licensing.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

class JucerDocument;

//==============================================================================
/**
    Manages a list of binary data objects that a JucerDocument wants to embed in
    the code it generates.
*/
class BinaryResources
{
public:
    //==============================================================================
    BinaryResources();
    ~BinaryResources();

    BinaryResources& operator= (const BinaryResources& other);

    void loadFromCpp (const File& cppFileLocation, const String& cpp);

    //==============================================================================
    struct BinaryResource
    {
        String name;
        String originalFilename;
        MemoryBlock data;
        std::unique_ptr<Drawable> drawable;
    };

    void clear();
    bool add (const String& name, const File& file);
    void add (const String& name, const String& originalFileName, const MemoryBlock& data);
    void remove (const int index);
    bool reload (const int index);
    String browseForResource (const String& title, const String& wildcard,
                              const File& fileToStartFrom, const String& resourceToReplace);

    String findUniqueName (const String& rootName) const;

    int size() const noexcept                                            { return resources.size(); }
    const BinaryResource* operator[] (const int index) const noexcept    { return resources [index]; }

    const BinaryResource* getResource (const String& resourceName) const;
    const BinaryResource* getResourceForFile (const File& file) const;

    StringArray getResourceNames() const;

    const Drawable* getDrawable (const String& name) const;
    Image getImageFromCache (const String& name) const;

    template <class ElementComparator>
    void sort (ElementComparator& sorter)
    {
        resources.sort (sorter, true);
        changed();
    }

    //==============================================================================
    void setDocument (JucerDocument* const doc)                   { document = doc; }
    JucerDocument* getDocument() const noexcept                   { return document; }

    void fillInGeneratedCode (GeneratedCode& code) const;


private:
    //==============================================================================
    JucerDocument* document;
    OwnedArray <BinaryResource> resources;

    BinaryResource* findResource (const String& name) const noexcept;
    void changed();
};
