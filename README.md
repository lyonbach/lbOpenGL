This repository serves as a place for the intention  to implement OpenGL without following a video series.

Project Structure


Project Root
|
|-- Source
|   |-- *.cpp files.
|   `-- *.h   files.
|
`-- Vendor
    |-- glfw
    |   |-- library
    |   `-- headers
    `-- glew
        |-- library
        `-- headers

All cpp and header files are located under the Source folder.
"glew" was chosen as the binder library for interacting with the OpenGL.
For the API for creating windows, contexts and surfaces, receiving input and events, glfw was used.
Both libraries are located under Vendor folder.
