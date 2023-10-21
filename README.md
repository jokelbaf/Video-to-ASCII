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

[<img src="https://i.ytimg.com/vi/lRmQkoRsXAo/maxresdefault.jpg" width="65%">](https://www.youtube.com/watch?v=lRmQkoRsXAo "Play ANY YouTube Video in a Terminal Window")


## Dependencies

During project development all dependencies were installed using [vcpgk](https://github.com/microsoft/vcpkg) which guarantees that the project will build and run on Windows. If you are using another OS, you may need to install the dependencies manually.

- FFmpeg (or FFPlay)
- OpenCV
- SFML / audio
- HTTPLib
- Nlohmann JSON

## Usage

### Building manually
1. Install dependencies;
2. Link dependencies to project;
3. Build project;
4. Open console and navigate to the folder where your exe file is located;
5. Run the file in your console with either **path to your video file** or **link to the YouTube video** as an argument.

A few examples of how to run the program:

```bash
video_to_ascii.exe "C:\Users\user\Videos\video.mp4"
```

```bash
video_to_ascii.exe "https://youtu.be/mgh07DuvAWM?si=dK12xaNkWardKpGW"
```

### Downloading pre-built executable

1. Download the latest release from [here](https://github.com/JokelBaf/Video-to-ASCII/releases/latest) (currently only Windows executables are available);
2. Open console and navigate to the folder where your exe file is located;
3. Run the file in your console with either **path to your video file** or **link to the YouTube video** as an argument.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change. Note that all libraries **must** be linked statically, to build a single exe file.