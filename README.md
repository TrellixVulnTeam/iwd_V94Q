# IWD - I Want Deps

## Intro

iwd is a simple tool that allows user to specify and install requirements for c++ projects.
It means to solve an issue, where user wants to quickstart a project, to check if doing something is viable.
It relies on cmake, to configure, build, and install dependencies defined in json file.

IWD is *NOT* a fully-fledged package manager, and it is not intended to be used this way.
If you want more flexibility and features, i recommend to use [conan](https://conan.io/).

## Requirements

1. CMake 3.13 +
2. Python 3.6 +

### Why not just use cmake ExternalProject/FetchContent tools

1. ExternalProject does not satisfy dependencies during pre-configure/configure stage.
2. FetchContent requires the user to call add_subdirectory, which can interact with your desired settings.

IWD provides isolation of builds and allows the user to write cmake file with assumption that all
dependencies are already installed.

### Why not just use conan

Writing conan recepies takes time, if they are not provided by the community.
I would recommend writing a recepie, but handling its creation is time consuming, and sometimes you just do not have it.

### Why not just use XXX

I have worked with plenty of the packaging tools: vcpkg, hunter to name some.
I found none of them as quick in usage as just downloading source code, and building it:
sometimes some of the libraries I wanted to use were missing, sometimes the configuration was not to my liking, and sometimes
version was not up to date. The configuration of tool to handle downloading packages for me was time consuming, and often
I found myself doing same thing over, and over - downloading from source and building it myself.

### Why use iwd

0. Speed of configuration.
1. You provide explicitly all of the dependencies you want to build.
2. You do not force others to use this solution at all (CMake will still find library if installed properly in the system).
3. You have nice list of dependencies used in the project which can be consumed by other programs/package creators.
4. Easily override settings, without disturbing requirements file.

### Notice
Please keep in mind, that IWD works only when libraries you want to use, have proper CMakeLists
defined.

## How it works

For each requirement specified in this way:
```json
{
    "name": "zlib",
    "version": "1.2.11",
    "url": "https://github.com/madler/zlib/archive/v1.2.11.tar.gz",
    "configuration": {}
}
```
The script:
1. Downloads source at given url (may be git repository, or tar.gz file url) and extracts it if necessary.
2. Calls `cmake -S {source_dir} -B {build_dir} [-D{configuration}...]`
Where `configuration` are arguments provided in configuration property of object, updated with cmd line provided args.
3. Calls `cmake --build {build_dir}`
4. Calls `cmake --install`

Additionally in `configuration` user may provide special variables with following schema
`$(VARIABLE_NAME)`. They are later overridden with global configuration args provided when invoking
`iwd`. More about that is described in `Important` section.

## Usage

In order to use it:

1. Ensure you have cmake, python3.7+, some c++ toolchain installed
2. Clone the repository, call python setup.py
3. Go to your project, and create a `iwd.json` file, with similar content:
```json
    {
        "iwd-version" : "1.0.0a0",
        "requirements": [
            {
                "name": "zlib",
                "version": "1.2.11",
                "url": "https://github.com/madler/zlib/archive/v1.2.11.tar.gz",
                "configuration": {}
            },
            {
                "name": "c-ares",
                "version": "1.15.0",
                "url": "https://github.com/c-ares/c-ares/releases/download/cares-1_15_0/c-ares-1.15.0.tar.gz",
                "configuration": {}
            },
            {
                "name": "protobuf",
                "version": "3.6.1",
                "url": "https://github.com/protocolbuffers/protobuf/archive/v3.6.1.tar.gz",
                "cmake_directory" : "cmake",
                "configuration": {
                    "protobuf_BUILD_TESTS": "OFF",
                    "protobuf_WITH_ZLIB": "OFF"
                }
            },
            {
                "name": "grpc",
                "version": "1.17.2",
                "url": "https://github.com/grpc/grpc/archive/v1.17.2.tar.gz",
                "configuration": {
                    "CMAKE_BUILD_TYPE": "build_type",
                    "gRPC_INSTALL": "ON",
                    "gRPC_BUILD_TESTS": "OFF",
                    "gRPC_PROTOBUF_PROVIDER": "package",
                    "gRPC_PROTOBUF_PACKAGE_TYPE": "CONFIG",
                    "gRPC_ZLIB_PROVIDER": "package",
                    "ZLIB_ROOT": "$(CMAKE_INSTALL_PREFIX)",
                    "gRPC_CARES_PROVIDER": "package",
                    "gRPC_SSL_PROVIDER": "package",
                    "OPENSSL_ROOT_DIR": "$(CMAKE_INSTALL_PREFIX)",
                    "gRPC_BUILD_CODEGEN": "ON",
                    "gRPC_BUILD_CSHARP_EXT": "ON",
                    "CMAKE_PREFIX_PATH": "$(CMAKE_INSTALL_PREFIX)"
                }
            },
            {
                "name" : "glm",
                "version" : "0.9.9.5",
                "url" : "https://github.com/g-truc/glm.git"
            }
        ]
    }
```
4. In same directory call `iwd [-B build_dir] [--config CONFIG] [ARGS]`
The args must be defined in `KEY=VALUE` schema, they are then passed to configure step for each package.
The build dir(by default `build`) can be used to override the build directory, in which sources will be downloaded, build and
installed.
The CONFIG argument can be used on when your platform default generator is a multi-config generator,
it can be used to force specific build type.

## Important

1. KeyValue pairs inside `configuration` are passed during cmake configuration step to target
2. If value matches pattern `\$\(\w+\)`, the `\w+` part is replaced with configuration arguments provided during script invokation
3. Url must point to tar.gz file or git repository
4. If url points to git repository, the repository must contain a branch or tag matching the version.
5. `cmake_directory` is optional, and if provided should point to dir with CMakeLists.txt file, relative to source dir.
6. Args should match the pattern `KEY=VALUE`, they are passed to every cmake configuration step
7. Args provided contribute to name of the directory inside .deps/install, which is unique for every configuration


## Roadmap

1. Fix all todos in the project
```
TODO - Detect if tar contains only one folder, or packs sources without it
TODO - When possible, this should print nice download status
```
2. Add a tool to inspect/clean build packages
3. Add support for binary packages
4. Add support for platform-specific configurations
5. Add support for copying files from source dir to install dir
6. Add support for file patching

## Future considerations

1. Add plain makefile project support
2. Allow to package the install directory for specific configuration/requirements.
3. Allow custom configuration/build/install steps.
