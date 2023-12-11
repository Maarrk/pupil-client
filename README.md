# pupil-client

Client and mock server for [Pupil labs Network API](https://docs.pupil-labs.com/core/developer/network-api/)

## Building

```
conan install . --output-folder=build --build=missing
cmake -S . -B ./build --preset conan-default
cmake --build ./build
```
