# Video to ASCII

Code for video playback was taken from [theriseof9/ascii-video-player](https://github.com/theriseof9/ascii-video-player). Improved performance and added a few more features.

## About

Plays a video file as ASCII text on your terminal in realtime. High performance C++ code allows realtime smooth 60fps playback (depending on terminal size) with audio playback and A/V sync.

## Support

Operating system compatibility:

| OS                 | Status                                             |
|--------------------|----------------------------------------------------|
| Windows            | ✅　Verified: Builds and runs.                       |
| MacOS              | ❓　Unknown: **Should** work in theory but untested. |
| POSIX-compliant OS | ❓　Unknown: **Should** work in theory but untested. |

## Features

- Smooth 60fps playback (depending on terminal size);
- Audio playback;
- A/V sync;
- Supports YouTube video URLs;

## Preview

[Click to View YouTube video](https://www.youtube.com/watch?v=lRmQkoRsXAo)

[<img src="https://i.ytimg.com/vi/lRmQkoRsXAo/maxresdefault.jpg" width="50%">](https://www.youtube.com/watch?v=lRmQkoRsXAo "Play ANY YouTube Video in a Terminal Window")


## Dependencies

During project development all dependencies were installed using [vcpgk](https://github.com/microsoft/vcpkg) which guarantees that the project will build and run on Windows. If you are using another OS, you may need to install the dependencies manually.

- FFmpeg (or FFPlay)
- OpenCV
- SFML / audio
- HTTPLib
- Nlohmann JSON
