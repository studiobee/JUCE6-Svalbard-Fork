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

//==============================================================================
/**
    A speech-bubble component that displays a short message.

    This can be used to show a message with the tail of the speech bubble
    pointing to a particular component or location on the screen.

    @see BubbleComponent

    @tags{GUI}
*/
class JUCE_API  BubbleMessageComponent  : public BubbleComponent,
                                          private Timer
{
public:
    //==============================================================================
    /** Creates a bubble component.

        After creating one a BubbleComponent, do the following:
        - add it to an appropriate parent component, or put it on the
          desktop with Component::addToDesktop (0).
        - use the showAt() method to show a message.
        - it will make itself invisible after it times-out (and can optionally
          also delete itself), or you can reuse it somewhere else by calling
          showAt() again.
    */
    BubbleMessageComponent (int fadeOutLengthMs = 150);

    /** Destructor. */
    ~BubbleMessageComponent() override;

    //==============================================================================
    /** Shows a message bubble at a particular position.

        This shows the bubble with its stem pointing to the given location
        (coordinates being relative to its parent component).

        For details about exactly how it decides where to position itself, see
        BubbleComponent::updatePosition().

        @param position                         the coords of the object to point to
        @param message                          the text to display
        @param numMillisecondsBeforeRemoving    how long to leave it on the screen before removing itself
                                                from its parent component. If this is 0 or less, it
                                                will stay there until manually removed.
        @param removeWhenMouseClicked           if this is true, the bubble will disappear as soon as a
                                                mouse button is pressed (anywhere on the screen)
        @param deleteSelfAfterUse               if true, then the component will delete itself after
                                                it becomes invisible
    */
    void showAt (const Rectangle<int>& position,
                 const AttributedString& message,
                 int numMillisecondsBeforeRemoving,
                 bool removeWhenMouseClicked = true,
                 bool deleteSelfAfterUse = false);

    /** Shows a message bubble next to a particular component.

        This shows the bubble with its stem pointing at the given component.

        For details about exactly how it decides where to position itself, see
        BubbleComponent::updatePosition().

        @param component                        the component that you want to point at
        @param message                          the text to display
        @param numMillisecondsBeforeRemoving    how long to leave it on the screen before removing itself
                                                from its parent component. If this is 0 or less, it
                                                will stay there until manually removed.
        @param removeWhenMouseClicked           if this is true, the bubble will disappear as soon as a
                                                mouse button is pressed (anywhere on the screen)
        @param deleteSelfAfterUse               if true, then the component will delete itself after
                                                it becomes invisible
    */
    void showAt (Component* component,
                 const AttributedString& message,
                 int numMillisecondsBeforeRemoving,
                 bool removeWhenMouseClicked = true,
                 bool deleteSelfAfterUse = false);


    //==============================================================================
    /** @internal */
    void getContentSize (int& w, int& h) override;
    /** @internal */
    void paintContent (Graphics& g, int w, int h) override;
    /** @internal */
    void timerCallback() override;

private:
    //==============================================================================
    int fadeOutLength, mouseClickCounter;
    TextLayout textLayout;
    int64 expiryTime;
    bool deleteAfterUse;

    void createLayout (const AttributedString&);
    void init (int, bool, bool);
    void hide (bool fadeOut);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BubbleMessageComponent)
};

} // namespace juce
