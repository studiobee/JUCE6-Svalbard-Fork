/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "../UI/PluginWindow.h"


//==============================================================================
/**
    A collection of plugins and some connections between them.
*/
class PluginGraph   : public FileBasedDocument,
                      public AudioProcessorListener,
                      private ChangeListener
{
public:
    //==============================================================================
    PluginGraph (AudioPluginFormatManager&);
    ~PluginGraph() override;

    //==============================================================================
    using NodeID = AudioProcessorGraph::NodeID;

    void addPlugin (const PluginDescription&, Point<double>);

    AudioProcessorGraph::Node::Ptr getNodeForName (const String& name) const;

    void setNodePosition (NodeID, Point<double>);
    Point<double> getNodePosition (NodeID) const;

    //==============================================================================
    void clear();

    bool windowPositionFitsOnScreen(PluginWindow * w);

    void fitWindowOnScreen(PluginWindow * w);

    PluginWindow* getOrCreateWindowFor (AudioProcessorGraph::Node*, PluginWindow::Type);
    void closeCurrentlyOpenWindowsFor (AudioProcessorGraph::NodeID);
    bool closeAnyOpenPluginWindows();

    //==============================================================================
    void audioProcessorParameterChanged (AudioProcessor*, int, float) override {}
    void audioProcessorChanged (AudioProcessor*) override { changed(); }

    //==============================================================================
    std::unique_ptr<XmlElement> createXml() const;
    void restoreFromXml (const XmlElement&);

    static const char* getFilenameSuffix()      { return ".filtergraph"; }
    static const char* getFilenameWildcard()    { return "*.filtergraph"; }

    //==============================================================================
    void newDocument();
    String getDocumentTitle() override;
    Result loadDocument (const File& file) override;
    Result saveDocument (const File& file) override;
    File getLastDocumentOpened() override;
    void setLastDocumentOpened (const File& file) override;

    static File getDefaultGraphDocumentOnMobile();

    //==============================================================================
    AudioProcessorGraph graph;

private:
    //==============================================================================
    AudioPluginFormatManager& formatManager;
    OwnedArray<PluginWindow> activePluginWindows;

    NodeID lastUID;
    NodeID getNextUID() noexcept;

    void createNodeFromXml (const XmlElement&);
    void addPluginCallback (std::unique_ptr<AudioPluginInstance>, const String& error, Point<double>);
    void changeListenerCallback (ChangeBroadcaster*) override;

    //LOGGER
    struct FileWithTime
    {
       FileWithTime(const File& f) : file(f), time(f.getLastModificationTime()) {}
       FileWithTime() {}

       bool operator<  (const FileWithTime& other) const { return time < other.time; }
       bool operator== (const FileWithTime& other) const { return time == other.time; }

       File file;
       Time time;
    };


    std::unique_ptr<FileLogger> logger;
    bool initialiseLogger(const char* filePrefix)
    {
       if (logger == nullptr)
       {
#if JUCE_LINUX
          String folder = "~/.config/Projucer/Logs";
#else
          String folder = "AudioPluginHostLogger";
#endif

          logger.reset(FileLogger::createDateStampedLogger(folder, filePrefix, ".txt", "  ---  Build date: " __DATE__));
          Logger::setCurrentLogger(logger.get());
       }

       return logger != nullptr;
    }

    void deleteLogger()
    {
       const int maxNumLogFilesToKeep = 50;

       Logger::setCurrentLogger(nullptr);

       if (logger != nullptr)
       {
          auto logFiles = logger->getLogFile().getParentDirectory().findChildFiles(File::findFiles, false);

          if (logFiles.size() > maxNumLogFilesToKeep)
          {
             Array<FileWithTime> files;

             for (auto& f : logFiles)
                files.addUsingDefaultSort(f);

             for (int i = 0; i < files.size() - maxNumLogFilesToKeep; ++i)
                files.getReference(i).file.deleteFile();
          }
       }

       logger.reset();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginGraph)
};
