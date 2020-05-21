This repository serves as a place for the intention  to implement OpenGL without following a video series.

Project Structure

```
lbOpenGL
├── GLEW
│   ├── include
│   │   └── GL
│   │       ├── glew.h
│   │       ├── glxew.h
│   │       └── wglew.h
│   ├── lib
│   │   └── pkgconfig
│   │       └── glew.pc
│   └── lib64
│       ├── libGLEW.a
│       ├── libGLEW.so -> libGLEW.so.2.1.0
│       ├── libGLEW.so.2.1 -> libGLEW.so.2.1.0
│       └── libGLEW.so.2.1.0
└── GLFW
    ├── include
    │   └── GLFW
    │       ├── glfw3.h
    │       └── glfw3native.h
    └── lib
        ├── cmake
        │   └── glfw3
        │       ├── glfw3Config.cmake
        │       ├── glfw3ConfigVersion.cmake
        │       ├── glfw3Targets.cmake
        │       └── glfw3Targets-noconfig.cmake
        ├── libglfw3.a
        └── pkgconfig
            └── glfw3.pc
```

All cpp and header files are located under the Source folder.
All Third-Party items are located under the Vendor folder.
Within the Vendor folder, there would be a folder for each api. Under directly this folder, there would be header files.
Libraries would be put inside one additional folder named as "Library".

"glew" was chosen as the binder library for interacting with the OpenGL.
For the API for creating windows, contexts and surfaces, receiving input and events, glfw was used.
Both apis are located under Vendor folder.
