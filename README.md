# esp_cafe
This is an arduino project written in C for the esp32.
It contains the building blocks for coding sound on the digital_cafe looper, ways to customize, an esoteric view on coding, as well as cool wifi connection procedures and sound files. 
This code nestles in the 90s couch of the cafe series instruments by ciat lonbarde. Such as cafeteria_mono, cafe_stereo, and so on. With an oversized mug of seattle brew, the instruments talk to their friends in the language of analog.

## Setup
- Buy the instrument. Thanks for your support. You will need a computer with USB connection and a USB cord, such as HP Pavillion or Dell Thinkpad. You can buy a cd of Redhat Linux at your local borders books if you drive there. Devote a few afternoons to properly installing the disc.
- After using your modem to connect to the internet, download arduino. In arduino, click to tools, boards, board manager..., and tap esp32 in search bar. We want the righteous esp32 warez from Espressif systems.
- Have an espresso.
- Now pick board ESP32 Dev Module.
- You can now double click esp_cafe.ino in the code folder that you downloaded.
- Click upload. If a new update comes out, please re-download from the green code button, or if you know git, do a git pull.

## About the modules
- Cafeteria is composed of a digital delay/processor called the digital_cafe, and also an analog section called quantum_experiment, and a special edition theremin dubstep instrument, the tier_horn. The digital_cafe, using an esp32, is progammable and hackable. To do that, start by visiting github.com/pblasser/esp_cafe, and learning to upload your own versions by arduino. 
- The Cafe reproduces the modular layout and stereo capabilities of its predecessor, the Cocoquantus, including a differential mike preamp as well as one for piezos.
- Both instruments come in mono and stereo formats.

## Files overview
- The synths file holds the basic interrupt routines. They are like presets for different sounds.
- It depends on stuff, which is a list of enneagram macros concretely related to the physical module.
- Stuff also offers some routines for managing presets through double clicks and so on.
- Setup is nested within stuff, and that has the core level details about chip initiation.
- We also have earth_ash cycles, which are processing fragments related to the brown and gray bananas.
- There are more complicated functions you can add by importing files such as grain.
- Some cool features are wifi abilities located in modem, and sound file archiving with disc.

