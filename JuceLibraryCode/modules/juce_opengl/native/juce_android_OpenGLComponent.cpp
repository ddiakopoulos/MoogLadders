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

class AndroidGLContext   : public OpenGLContext
{
public:
    AndroidGLContext (Component* const component_,
                      const OpenGLPixelFormat& pixelFormat,
                      const AndroidGLContext* const sharedContext,
                      const bool isGLES2_)
        : component (component_),
          lastWidth (0), lastHeight (0)
    {
    }

    ~AndroidGLContext()
    {
        properties.clear(); // to release any stored programs, etc that may be held in properties.

        makeInactive();

    }

    bool makeActive() const noexcept
    {

        return true;
    }

    void swapBuffers()
    {
    }

    bool makeInactive() const noexcept
    {

        return true;
    }

    bool isActive() const noexcept                  { return false; }

    void* getRawContext() const noexcept            { return 0; }
    unsigned int getFrameBufferID() const           { return 0; }

    int getWidth() const                            { return lastWidth; }
    int getHeight() const                           { return lastHeight; }

    bool areShadersAvailable() const                { return isGLES2; }

    void updateWindowPosition (const Rectangle<int>& bounds)
    {
        if (lastWidth != bounds.getWidth() || lastHeight != bounds.getHeight())
        {
            lastWidth  = bounds.getWidth();
            lastHeight = bounds.getHeight();


        }
    }

    bool setSwapInterval (const int numFramesPerSwap)
    {
        return true;
    }

    int getSwapInterval() const
    {
        return 0;
    }

private:
    WeakReference<Component> component;
    bool isGLES2;
    int lastWidth, lastHeight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndroidGLContext);
};

OpenGLContext* OpenGLComponent::createContext()
{
    ComponentPeer* peer = getPeer();

//    if (peer != nullptr)
//        return new AndroidGLContext ((UIView*) peer->getNativeHandle(), this, preferredPixelFormat,
//                                dynamic_cast <const AndroidGLContext*> (contextToShareListsWith),
//                                (flags & openGLES2) != 0);

    return nullptr;
}

void OpenGLComponent::updateEmbeddedPosition (const Rectangle<int>& bounds)
{
    const ScopedLock sl (contextLock);

    if (context != nullptr)
        static_cast <AndroidGLContext*> (context.get())->updateWindowPosition (bounds);
}

bool OpenGLHelpers::isContextActive()
{
    return false;
}
