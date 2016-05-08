# libsrcdemo

Library for parsing the original source demo format. It's used by most games based on the Half-Life 2 engine. Dota 2 and CS:GO use a newer protobuf protocol and are unsupported, but have their own official demoinfo tools (demoinfo2, demoinfogo). Focus is on Team Fortress 2, because that's what I'm personally most interested in. Currently the library is in a very early development stage and should only be used for looking at it. In fact, it's not even a library but rather a bunch of source files and a main function.

## Building

Run the usual CMake commands:

    mkdir build
    cd build
    cmake ..
    make
    ./demoinfosrc

## Dependencies

These are automatically cloned using submodules, make sure to enable them.

* [vdfparser]
* [snappy]

## Credits

* SizzlingCalamari - Reverse engineering some of the messages and helping me a lot with general stuff. Check out his own tool [demboyz]!

## License

Libsrcdemo uses the vdfparser library, which is released under the GNU GPLv2. Therefore the project has to be released under the same license. My own code can be used under the GNU LGPLv2 as well, if you replace the VDF parser with an alternative or remove its features completely.

[vdfparser]: https://github.com/Yepoleb/vdfparser
[snappy]: https://github.com/google/snappy
[demboyz]: https://github.com/SizzlingStats/demboyz

