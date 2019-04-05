# IWD - I Want Deps 

iwd is a simple tool that allows user to specify and install requirements for c++ projects.
It relies on cmake, to configure, build, and install dependencies defined in json file.

In order to use it:

1. Ensure you have cmake, python3.7+, some c++ toolchain installed
2. clone the repo, call python setup.py 
3. Go to your project, and create a requirements.txt file, with json content:
```json
    {
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
4. In same directory call `iwd [ARGS]`

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
# TODO - This can be done more flexible with regex matching
# TODO - Create and validate schema
# TODO - Use list comprehension
# TODO - This should allow for text modification, i.e. usage of $(VALUE)/hardcoded
# TODO - When possible, this should print nice download status
# TODO - Use logging
# TODO - Handle windows case, when there is need to avoid multiconfig generator
# TODO - Store the install_prefix, and configuration info in some nice readable text file
```
2. Allow to pull git repositories, not just source packages
3. Better cmake integration 
4. Allow the user to define directories, where to install stuff 

## Future considerations 

1. Add plain makefile project support 




