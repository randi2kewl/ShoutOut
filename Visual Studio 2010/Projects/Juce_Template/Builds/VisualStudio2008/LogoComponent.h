/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  2 May 2012 6:30:20am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_LOGOCOMPONENT_LOGOCOMPONENT_F51C2937__
#define __JUCER_HEADER_LOGOCOMPONENT_LOGOCOMPONENT_F51C2937__

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LogoComponent  : public Component
{
public:
    //==============================================================================
    LogoComponent ();
    ~LogoComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();

    // Binary resources:
    static const char* so_png;
    static const int so_pngSize;


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Image cachedImage_so_png;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    LogoComponent (const LogoComponent&);
    const LogoComponent& operator= (const LogoComponent&);
};


#endif   // __JUCER_HEADER_LOGOCOMPONENT_LOGOCOMPONENT_F51C2937__
