package com.VanvelzenSoftware.gucefANDROIDGLUE;

public class GUCEFApplicationGlue
{
    static 
    {
        System.loadLibrary( "gucefCORE" );
    }

    /** 
     *  Starts the application run loop
     */
    public native int Main();

    /**
     * Stops the application run loop
     */
    public native Stop();
}