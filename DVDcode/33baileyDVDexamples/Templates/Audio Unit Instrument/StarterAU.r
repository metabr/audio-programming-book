#include <AudioUnit/AudioUnit.r>
#include "«PROJECTNAME»Version.h"

// Note that resource IDs must be spaced 2 apart for the 'STR ' name and description
#define kAudioUnitResID_«PROJECTNAME»				13470

// So you need to define these appropriately for your audio unit.
// For the name the convention is to provide your company name and end it with a ':',
// then provide the name of the AudioUnit.
// The Description can be whatever you want.
// For an effect unit the Type and SubType should be left the way they are defined here...
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// AnalogSynth AudioUnit
#define RES_ID			kAudioUnitResID_«PROJECTNAME»
#define COMP_TYPE		kAudioUnitType_MusicDevice
#define COMP_SUBTYPE	«PROJECTNAME»_COMP_SUBTYPE
#define COMP_MANUF		«PROJECTNAME»_COMP_MANF	

#define VERSION			k«PROJECTNAME»Version
#define NAME			"«ORGANIZATIONNAME»: «PROJECTNAME»"
#define DESCRIPTION		"«PROJECTNAME» AudioUnit"
#define ENTRY_POINT		"«PROJECTNAME»Entry"

#include "AUResources.r"
