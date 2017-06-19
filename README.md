# UM17 Gearchange

## Introduction
This is the automatic/semi-automatic gearbox for the UVigo Motorsport UM17 car. It is written for an Arduino Nano or compatible, but should be relatively easy to move to other platforms.

All settings that are changable save to EEPROM and are persistant throughout resets.

## Arduino specific libraries
The gearchange leans heavily on the Servo, Serial and EEPROM libraries of Arduino. These should be portable to most ATMegas but care should be taken.

## Wiring
**D2**: Gear up button, put a pull down resistor  
**D3**: Gear down button, put a pull down resistor  
**D6**: Gearchange servo output  
**D7**: Blip output to ETC (provisional)  
**D8**: Cut output to ETC (provisional)  
**D9**: Clutch servo output  
**D12**: Neutral switch input, put a pull down resistor  
**A0**: Potientometer input for clutch control  
**A1**: Clutch position input from servo  
**A2**: Gearchange position input from servo  

## Normal use
When the gearbox turns on, it will check if the gearbox is in neutral. If not, it first searches for neutral by changing down, changing halfway up, and continuing this pattern until it finds neutral. If you prefer to put the gearbox in neutral yourself, send a "!" and put the gearbox in neutral. The gearbox will then be ready to use.

The gearbox defaults to 100% declutched. It will refuse to move the clutch until you pull the clutch level all the way to 100%, and start to let go. 

The gear pattern is N-1-2-3-4-5-6.

Changing up and down is as expected, but on downchange if the gearbox is in 1st, the gearbox requires 2 change requests within 5 seconds to change down into neutral. 

The gearbox starts up by default in stupid mode (Level 0). To add more automotion, send an "a".

## Current automatic gearchange support (v0.3)
Currently, Level 1 is somewhat implemented and can change in the form declutch->change->blip or cut for a set period of time->clutch. It's kinda hacky and slow.

## Commands
There are quite a few commands in the gearbox, spit across 3 modes. Keep this section handy as to save space some of the rather verbose responses the gearbox gives may start to dispear as features are added to save space.

### How to send commands
Use the arduino serial monitor or any other qualified RS-232 terminal emulator and send 1 character at a time. The software does not accept any numbers. It will accept a chain of characers (say, 20 +s in a row) quite well should you need to make big changes.

### Neutral-search mode
**!**: Quit searching automatically let the human put the gearbox in neutral.  

### Normal mode
**a**: Increase automation level. Currently goes up to 1.  
**d**: Decrease automation level. Currently does down to 0.  
**r**: Do a hard reset on the gearbox controller.  
**n**: Do a soft reset on the gearbox controller. Doesn't actually reset the microprocessor, just places the gearbox in a known state and requires you to manually put the gearbox in neutral.  
**e**: Increases the bias in the clutch position function toward the exponential side by 10%. The form of the equation is 1-EXPDIV * LIN(x) + EXPDIV * EXP(x), where EXPDIV varies from 0 to 1.  
**q**: Decreases the bias in the clutch position function toward the exponential side by 10%.  
**t**: Increases the initial deadzone (from 0% declutched) in the clutch control by 1%. This maxes out at 100%. Don't do that.  
**y**: Decreases the initial deadzone (from 0% declutched) in the clutch control by 1%. This mins out at 0%.  
**o**: This increases the factor within the exponential function by .1. Increasing this makes the hip later and the change in slope more severe, but is less sensitive to changes below the hip. This maxes out at 10 because higher than that the function is basically digital.  
**p**: This decreases the factor within the exponential function by .1. Decreasing this makes the hip early and the change in slope less severe, but is more sensitive to changes below the hip. This mins out at 1 because lower than that and the function is basically linear.  
**!**: Clear all saved data in the EEPROM.  
**s**: Enter set mode.  

### Set mode
**q**: Quit set mode.  
**p**: Enter clutch input lever mode.  
#### Clutch input lever mode
1. Put the lever to one maximum, send **d**.
2. Put the lever to the minimum, send **d**.
3. Put the lever to the other maximum, send **d** a final time.

If both maximums have voltages above or both below the minimum, both will be set to the value of the first maximum  

**c**: Clutch servo set mode.  
#### Clutch servo set mode
##### Universal commands
**+**: Increase the microseconds. More rotates the servo more in a clockwise direction (normally). The limit of the servo is usually 2000ms, the software limits you to 2500.  
**-**: Decrease the microseconds. Less rotates the servo more in a counter-clockwise direction (normally). The limit of the servo is usually 1000ms, the software limits you to 500.  
**q**: Quit this specific setting and go back up a level. Changes are saved.  
**!**: No matter the level, quit Set mode and go back to normal mode. Changes are saved.  
##### Specific settings
**+**: Maximum declutched position. This is where the lever on the steering wheel would be all the way pulled.  
**-**: Minimum declutched position. This is when nobody is touching the lever.  
**d**: Deadzone position. This is the initial position the servo jumps to when you leave the lever's deadzone.  

**g**: Gearchange servo set mode.  
#### Gearchange servo set mode
##### Universal commands
**+**: Increase the microseconds. More rotates the servo more in a clockwise direction (normally). The limit of the servo is usually 2000ms, the software limits you to 2500.  
**-**: Decrease the microseconds. Less rotates the servo more in a counter-clockwise direction (normally). The limit of the servo is usually 1000ms, the software limits you to 500.  
**q**: Quit this specific setting and go back up a level. Changes are saved.  
**!**: No matter the level, quit Set mode and go back to normal mode. Changes are saved.  
##### Specific settings
**+**: Upchange  
**-**: Downchange  
**u**: Halfup - from 1 to N.  
**d**: Halfdown - from 2 to N.  
**m**: Midpoint - This is where the servo should be normally, when there is no gearchange currently in progress.  

**b**: Blip set mode (sets length of blip or cut)  
#### Blip set mode
##### Universal commands
**+**: Increase the milliseconds. This increases the length of time the throttle is shut or open. Limit is 500ms.  
**-**: Decrease the milliseconds. This decreases the length of time the throttle is shut or open. Limit is 0ms.
**t**: Sends a blip or cut for the amount of time set.  
**q**: Quit this specific setting and go back up a level. Changes are saved.  
**!**: No matter the level, quit Set mode and go back to normal mode. Changes are saved.  
##### Specific settings
**u**: Set the amount of time to cut the throttle on upchange  
**d**: Set the amount of time to blip on downchange  
