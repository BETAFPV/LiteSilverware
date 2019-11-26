LiteSilverware project is based on the NotFastEnuf Silverware firmware, AKA [NFE_Silverware](https://github.com/NotFastEnuf/NFE_Silverware), which has a big success on the brushed whoop FC board like Alienwhoop ZER0. Several features are included in order to support brushless drones.

1. Support brushless motor ESC firmware BLHeli_S.
2. Support OSD interface, based on the [LiteOSD](https://github.com/BETAFPV/LiteOSD) project.
3. Change parameters via OSD menu and original stick gestures are not supported.
4. Support external receiver SBUS and DSMX protocol.
5. Built in Bayang receiver or Frsky D8 protocol

The full [user manual](https://github.com/BETAFPV/LiteSilverware/tree/master/docs) is here.

_end LiteSilverware notes_

## NotFastEnuf Notes

Last major update 9.11.18

Testing a new layout for config.h  - feel free to let me know what you think.  Your feedback is appreciated.

There are two main steps to prep this file for flash.  The first will be in config.h and the second will be in pid.c file.

STEP 1:  Open config.h with keil and make edits to set your selections in 6 categories.

         _Hardware Selection_ - select your target: alienwhoop zer0, bwhoop, e011, or H8mini_blue_board based hardware

         _Receiver Settings_ - set rates, expo, radio protocol, transmitter type and assign features to auxiliary channel switches

         _Voltage Settings_ - low voltage cutoff and other voltage related settings - UPDATE: LVC can now be switched on via stick gesture.  See notes for Berzerker release on the releases page.

         _Filter Settings_ -set gyro filter and D term filter frequencies and orders - UPDATE: reccomendation is to use alienwhoop switchable filtering.  See notes for Berzerker release on releases page.

         _Motor Output Settings_ - set pwm rate, motor curves, inverted yaw for props out, and features related to motor output - UPDATE: props in/props out is now switchable via stick gesture with alienwhoop settings.  See notes for Berzerker release for instructions.

         _Additional Features_ - remaining special features

STEP 2:  Open pid.c with keil and either edit pids to your preferred values or select a set of pids that I have prepared.   I plan to keep all my tunes in pid.c and will have them labeled and noted with the associated filter settings for each type of build.  All you have to do to use one of my tunes is uncomment the pid group you want to use and make sure the others are commented out.  I will keep whatever pids I'm using updated here if I change them and the default set will always be for a whoop with fpv gear installed.  Setpoint weight values for P are also just below pids - if you want a sharper stick feel on one of my tunes then change these values closer to 1.0.  Stick Accelerator and transition values are here too.  There are two profiles you can populate which are switchable via auxillary channel.

## Current Experimental Features and DEVIATIONS FROM STOCK SILVERWARE

 - Target Selection:  define your board type and hardware settings are automatically selected for E011 (OLD STYLE), Bwhoop, bwhoop pro, E011c, beta fpv lite, and ALIENWHOOP ZER0.  
 - Radio Type Selection:  Renamed aux channels as chan_5 through chan_10 and proper mapping is now controlled by defining your transmitter type.  Simply select devo, multi (for taranis or other multimodule), or stock (for toy tx) and assign the features you want to chan_#.  Your aux channel mapping will be adjusted in software to match your radio channel numbering.
 - Racemode:  flight mode with leveling on roll, acro on pitch, and yaw axis rotates about gravity vector.  Can be activated on an aux channel when in level mode.
 - Racemode Horizon: same as above with horizon on roll.
 - Horizon: leveling when upright, will flip or roll.
 - Kalman Gyro Filter:  adjustable gyro filter that's very similar to a 1st order filter but to me feels a little faster and a little weaker.  You decide...
 - Kalman Motor Filter:  adjustable motor output filter.  Same evaluation as the gyro version.
 - Idle up and Arming on seperate aux channels:  idle speed is also adjustable.  Paired with mix increase throttle feature it behaves like airmode
 - Arming safety (when arming feature is turned on)  - Props will not spin when arming unless your throttle is below THROTTLE_SAFETY variable value.  Attempting to arm with throttle above this limit will rapid flash the led to indicate a safety override condition.
 - MIX_INCREASE_THROTTLE_3 will now also automatically not activate during pre-takeoff staging after arming untill throttle has crossed the THROTTLE_SAFETY value to improve on ground behavior prior to takeoff.
 - Sticks Deadband:  adjustable deadband for transmitter inputs to compensate for gimbals that don't perfectly center
 - Motor Filter Frequencies:  motor filters are adjustable by a frequency instead of a decimal value.
 - AUTOBIND Has been added.  Set a fixed ID in your TX.  Stick gesture comand up-up-up to turn on autobind. Quad will flash once.  Stick gesture Down-Down-Down will save your fixed ID.  Unplug battery and plug in to verify.  No more power cycling TX to bind.
 - Voltage Telemetry Correction Factor has been added to make it easy to fine tune your telemetry voltage to read exactly accurate.  To use this feature - measure the voltage of a freshly charged pack with a trusted multimeter.  Make note of this value.  Then connect the battery and make note of the reported voltage by telemetry.  Now go back to config and update the reported telemetry voltage and actual battery voltage values.  Compile and reflash.  Your reported telemetry voltage will be automatically rescaled withing the adc calculation and will now be displayed perfectly.
 - Buzzer Code has been changed to combine automatic buzzer features and aux channel buzzer features into one define.  Enabeling this feature with with the aux channel set to CHAN_OFF will only activate all automatic buzzer features (like beeping on low battery or failsafe).  Changing the CHAN_OFF to CHAN_# (you pick your aux #) will allow both automatic buzzer functions and the ability to turn on the buzzer with your selected aux channel.
 - JOELUCID's YAW FIX has been added to the code on a define in config.  Feel free to turn it on and off and test the "feel" of this fix for yourself.
 - SBUS receiver code has been updated to include missing channels
 - STICK_TRAVEL_CHECK feature has been added to give the ability to make sure your sticks are reaching 100% throws in software.  This feature when activated will override the function of the stick gesture aux channel.  RIGHT-RIGHT-DOWN will enter a mode where the throttle is inactive and the led will rapid blink when you move the sticks to 100% throws.  If you do not see a rapid led blink at stick extents - scale up your throws in your transmitter untill you do.  LEFT-LEFT-DOWN will exit this mode.  Suggestion to DEVO 7e users - you may want to define this check once, then reflash after checking with the feature off so that you can use the stick gesture channel as a supliment for other features since you only have 2 switches.
 - EXPO VALUES have now been split into a seperate variable for each individual axis AND a seperate expo set is available for acro mode and leveled modes.  This gives the the pilot the ability to run high expo in acro with fast rotational rates for freestyle but also switch into level mode with a completely different set of expo variables where more linear controls may be desired for racing.
 - Alienwhoop Zer0 target has been added and work continues to optomize its feature set.  This target was origionaly intended for SBUS receiver but now supports SPI based XN297L receiver modules.  One can be found inside most toy transmitters and removed to become an affordable but capable micro receiver with telemetry.  Selecting any of the Bayang protocols will automatically populate code for the SPI receiver option - alternatively selecting SBUS will deactivate SPI and enable the SBUS appropriate code.
 - Additional notes have been added to the releases page for Alienwhoop ZER0 firmware including new switchable features via stick gestures.  Please read these notes for further instructions.

 ## Bikemike's Blheli 4way Interface Support(from Betaflight) 15.03.18 (Yets)

This commit enables flashing and configuring Blheli using the Silverware FC. **May not work with all ESCs**

 - FC must have MOSFETS and motor pulldown resistors removed
 - In hardware.h file comment in either define USE_ESC_DRIVER or define USE_DSHOT_DRIVER_BETA coupled with define USE_SERIAL_4WAY_BLHELI_INTERFACE
 - Quad must be onground and not armed
 - Connect TX/RX of a usb TTL adapter/Arduino to DAT/CLK and GND to GND in Blheli suite, select the 4way-IF (D) interface
 - Connect and configure/flash ESCs as usual
 - After disconnecting, normal FC operation should resume

 Credit to Bikemike https://www.rcgroups.com/forums/showpost.php?p=38505698&postcount=11950

_Easiest way to find me for feedback & discussion is here https://community.micro-motor-warehouse.com/t/notfastenuf-e011-bwhoop-silverware-fork/5501?u=notfastenuf_

_end NFE notes_

_The note about original Silverware is not included here. You could check it on [silver13/BoldClash-BWHOOP-B-03](https://github.com/silver13/BoldClash-BWHOOP-B-03) if needed._
