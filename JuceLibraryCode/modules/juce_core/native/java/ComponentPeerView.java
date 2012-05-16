/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

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

package com.juce;

import android.content.Context;
import android.view.*;
import android.graphics.*;
import android.opengl.*;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

//==============================================================================
public class ComponentPeerView extends GLSurfaceView
							   implements View.OnFocusChangeListener
{
    public ComponentPeerView (Context context, boolean opaque_)
    {
        super (context);
    	opaque = opaque_;

       // setEGLContextClientVersion (2);
        setRenderer (new Renderer());

        setFocusable (true);
        setFocusableInTouchMode (true);
        setOnFocusChangeListener (this);
        requestFocus();
    }

    //==============================================================================
    private native void handlePaint (Canvas canvas);

    @Override
    public void draw (Canvas canvas)
    {
        handlePaint (canvas);
    }

    @Override
    public boolean isOpaque()
    {
    	return opaque;
    }

    private boolean opaque;

    //==============================================================================
    private native void handleMouseDown (float x, float y, long time);
    private native void handleMouseDrag (float x, float y, long time);
    private native void handleMouseUp (float x, float y, long time);

    @Override
    public boolean onTouchEvent (MotionEvent event)
    {
        switch (event.getAction())
        {
            case MotionEvent.ACTION_DOWN:  handleMouseDown (event.getX(), event.getY(), event.getEventTime()); return true;
            case MotionEvent.ACTION_MOVE:  handleMouseDrag (event.getX(), event.getY(), event.getEventTime()); return true;
            case MotionEvent.ACTION_CANCEL:
            case MotionEvent.ACTION_UP:    handleMouseUp (event.getX(), event.getY(), event.getEventTime()); return true;
            default: break;
        }

        return false;
    }

    //==============================================================================
    @Override
    protected void onSizeChanged (int w, int h, int oldw, int oldh)
    {
    	viewSizeChanged();
    }

    @Override
    protected void onLayout (boolean changed, int left, int top, int right, int bottom)
    {
    }

    private native void viewSizeChanged();

    @Override
    public void onFocusChange (View v, boolean hasFocus)
    {
    	if (v == this)
    		focusChanged (hasFocus);
    }

    private native void focusChanged (boolean hasFocus);

    public void setViewName (String newName)
    {
    }

    public boolean isVisible()
    {
        return getVisibility() == VISIBLE;
    }

    public void setVisible (boolean b)
    {
    	setVisibility (b ? VISIBLE : INVISIBLE);
    }

    public boolean containsPoint (int x, int y)
    {
    	return true; //xxx needs to check overlapping views
    }

	public class Renderer implements GLSurfaceView.Renderer
	{
	    public void onSurfaceCreated (GL10 unused, EGLConfig config)
	    {
	    }

	    public void onDrawFrame (GL10 unused)
	    {
	        GLES20.glClearColor (1.0f, 0.5f, 0.0f, 1.0f);
	        GLES20.glClear (GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
	    }

	    public void onSurfaceChanged (GL10 unused, int width, int height)
	    {
	        GLES20.glViewport(0, 0, width, height);
	    }
	}
}
