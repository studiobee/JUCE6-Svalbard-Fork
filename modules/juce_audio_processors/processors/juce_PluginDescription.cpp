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

namespace juce
{

bool PluginDescription::isDuplicateOf (const PluginDescription& other) const noexcept
{
    return fileOrIdentifier == other.fileOrIdentifier
            && uid == other.uid;
}

static String getPluginDescSuffix (const PluginDescription& d)
{
    return "-" + String::toHexString (d.fileOrIdentifier.hashCode())
         + "-" + String::toHexString (d.uid);
}

bool PluginDescription::matchesIdentifierString (const String& identifierString) const
{
    return identifierString.endsWithIgnoreCase (getPluginDescSuffix (*this));
}

String PluginDescription::createIdentifierString() const
{
    return pluginFormatName + "-" + name + getPluginDescSuffix (*this);
}

std::unique_ptr<XmlElement> PluginDescription::createXml() const
{
    auto e = std::make_unique<XmlElement> ("PLUGIN");

    e->setAttribute ("name", name);

    if (descriptiveName != name)
        e->setAttribute ("descriptiveName", descriptiveName);

    e->setAttribute ("format", pluginFormatName);
    e->setAttribute ("category", category);
    e->setAttribute ("manufacturer", manufacturerName);
    e->setAttribute ("version", version);
    e->setAttribute ("file", fileOrIdentifier);
    e->setAttribute ("uid", String::toHexString (uid));
    e->setAttribute ("isInstrument", isInstrument);
    e->setAttribute ("fileTime", String::toHexString (lastFileModTime.toMilliseconds()));
    e->setAttribute ("infoUpdateTime", String::toHexString (lastInfoUpdateTime.toMilliseconds()));
    e->setAttribute ("numInputs", numInputChannels);
    e->setAttribute ("numOutputs", numOutputChannels);
    e->setAttribute ("isShell", hasSharedContainer);

    return e;
}

bool PluginDescription::loadFromXml (const XmlElement& xml)
{
    if (xml.hasTagName ("PLUGIN"))
    {
        name                = xml.getStringAttribute ("name");
        descriptiveName     = xml.getStringAttribute ("descriptiveName", name);
        pluginFormatName    = xml.getStringAttribute ("format");
        category            = xml.getStringAttribute ("category");
        manufacturerName    = xml.getStringAttribute ("manufacturer");
        version             = xml.getStringAttribute ("version");
        fileOrIdentifier    = xml.getStringAttribute ("file");
        uid                 = xml.getStringAttribute ("uid").getHexValue32();
        isInstrument        = xml.getBoolAttribute ("isInstrument", false);
        lastFileModTime     = Time (xml.getStringAttribute ("fileTime").getHexValue64());
        lastInfoUpdateTime  = Time (xml.getStringAttribute ("infoUpdateTime").getHexValue64());
        numInputChannels    = xml.getIntAttribute ("numInputs");
        numOutputChannels   = xml.getIntAttribute ("numOutputs");
        hasSharedContainer  = xml.getBoolAttribute ("isShell", false);

        return true;
    }

    return false;
}

} // namespace juce
