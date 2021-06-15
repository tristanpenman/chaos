# Chaos

A level editor for Sonic The Hedgehog Mega Drive / Genesis ROMs. This is an incomplete C++ port of Brett Kosinski's [Chaos level editor](https://github.com/fancypantalons/chaos). It can be used to read Sonic The Hedgehog 2 and 3 ROM files.

Here's a screenshot of Hydrocity Zone from [Sonic The Hedgehog 3](https://en.wikipedia.org/wiki/Sonic_the_Hedgehog_3):

![Hydrocity Zone](./doc/screenshot.png)

Included in the [doc](./doc) directory is Brett Kosinski's [write up](./doc/kosinski.txt) of the compression algorithm used.

## Background

I first discovered Brett Kosinski's Chaos level editor around 2001. Remarkably, this open source level editor made it possible to edit levels directly within a ROM file for Sonic The Hedgehog 2. Not only did Brett manage to extract the compressed level and art data from the ROM, his program also allowed new data to be saved to the ROM file, relocating level data to the end of the file and updating the necessary metadata.

As a pretty average high-schooler in 2001, I found this sophistication mind-blowing. I spent many hours (probably days) learning how it all worked, and eventually produced my own rudimentary level editor (written in Visual Basic) that worked on emulator save states. Working with save states was a different angle, and a few people in the SEGA reverse engineering community seemed to appreciate it.

## Current State

In the years that followed, I came back to this challenge as a way to learn new languages and to refine my skills as a programmer. First with Delphi/Pascal, and then with C++/Win32 API. Many years later, I even had a go [with Haskell](./haskell).

The version of the code you're looking at now is a partially completed C++ port, that can read level data from Sonic 2 and Sonic 3 ROM files.

Unfortunately, other commitments distracted me from completing more editor functionality.

## Structure

The code is currently structured as a Visual Studio project. This is a pretty traditional Win32 project, so there aren't any dependencies outside of the Windows SDK. I think it may even build with the headers/libraries that are included with Visual Studio / Visual C++.

The solution and project files can be found in [msvc](./msvc).

## License

This code is licensed under the MIT License.

See the LICENSE file for more information.
