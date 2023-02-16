# Audio plugin development with deep learning models

**Still a work-in-progress. Nothing to see here yet.**

## Build

First, install [CMake](https://cmake.org/install/) and a generator (like [Ninja](https://github.com/ninja-build/ninja)).

Then generate a project and build it.

```sh
# Generate a build project (with e.g. Ninja).
mkdir build && cd build
cmake -S=.. -B=. -G=Ninja --fresh

# Build project.
cmake --build .
```

### References

- https://github.com/sudara/pamplejuce/
- https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md
- https://www.tensorflow.org/lite/guide/build_cmake
