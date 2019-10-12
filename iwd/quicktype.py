# To use this code, make sure you
#
#     import json
#
# and then, to convert JSON from a string, do
#
#     result = iwd_from_dict(json.loads(json_string))

from typing import Optional, Any, List, Dict, TypeVar, Callable, Type, cast
from enum import Enum


T = TypeVar("T")
EnumT = TypeVar("EnumT", bound=Enum)


def from_str(x: Any) -> str:
    assert isinstance(x, str)
    return x


def from_bool(x: Any) -> bool:
    assert isinstance(x, bool)
    return x


def from_none(x: Any) -> Any:
    assert x is None
    return x


def from_union(fs, x):
    for f in fs:
        try:
            return f(x)
        except:
            pass
    assert False


def from_list(f: Callable[[Any], T], x: Any) -> List[T]:
    assert isinstance(x, list)
    return [f(y) for y in x]


def to_enum(c: Type[EnumT], x: Any) -> EnumT:
    assert isinstance(x, c)
    return x.value


def from_dict(f: Callable[[Any], T], x: Any) -> Dict[str, T]:
    assert isinstance(x, dict)
    return {k: f(v) for (k, v) in x.items()}


def to_class(c: Type[T], x: Any) -> dict:
    assert isinstance(x, c)
    return cast(Any, x).to_dict()


class Copy:
    """Relative path to the iwd install directory, defines where are the found files copied"""
    destination: str
    """Controls whether to keep paths when copying found files"""
    keep_paths: Optional[bool]
    """Provide a path to the directory, which will be treated as a root directory for any
    expression and path resolving.
    If the value is not provided, if defaults to the dependency source directory.
    """
    source_directory: Optional[str]
    """python glob expression , controls what kind of files should be copied"""
    sources: str

    def __init__(self, destination: str, keep_paths: Optional[bool], source_directory: Optional[str], sources: str) -> None:
        self.destination = destination
        self.keep_paths = keep_paths
        self.source_directory = source_directory
        self.sources = sources

    @staticmethod
    def from_dict(obj: Any) -> 'Copy':
        assert isinstance(obj, dict)
        destination = from_str(obj.get("destination"))
        keep_paths = from_union([from_bool, from_none], obj.get("keep-paths"))
        source_directory = from_union(
            [from_str, from_none], obj.get("source-directory"))
        sources = from_str(obj.get("sources"))
        return Copy(destination, keep_paths, source_directory, sources)

    def to_dict(self) -> dict:
        result: dict = {}
        result["destination"] = from_str(self.destination)
        result["keep-paths"] = from_union([from_bool,
                                           from_none], self.keep_paths)
        result["source-directory"] = from_union(
            [from_str, from_none], self.source_directory)
        result["sources"] = from_str(self.sources)
        return result


class TypeEnum(Enum):
    """Describes what kind of patch command is this"""
    APPEND = "append"
    CREATE = "create"
    REPLACE = "replace"


class Patch:
    """Describes the relative path from source directory to the file on which the operation
    should be executed.
    """
    file: str
    """Required only for replace command. Regex pattern that will serve as a target for
    replacement.
    """
    pattern: Optional[str]
    """Lines, which will be added to the file"""
    text: List[str]
    """Describes what kind of patch command is this"""
    type: Optional[TypeEnum]

    def __init__(self, file: str, pattern: Optional[str], text: List[str], type: Optional[TypeEnum]) -> None:
        self.file = file
        self.pattern = pattern
        self.text = text
        self.type = type

    @staticmethod
    def from_dict(obj: Any) -> 'Patch':
        assert isinstance(obj, dict)
        file = from_str(obj.get("file"))
        pattern = from_union([from_str, from_none], obj.get("pattern"))
        text = from_list(from_str, obj.get("text"))
        type = from_union([TypeEnum, from_none], obj.get("type"))
        return Patch(file, pattern, text, type)

    def to_dict(self) -> dict:
        result: dict = {}
        result["file"] = from_str(self.file)
        result["pattern"] = from_union([from_str, from_none], self.pattern)
        result["text"] = from_list(from_str, self.text)
        result["type"] = from_union(
            [lambda x: to_enum(TypeEnum, x), from_none], self.type)
        return result


class Requirement:
    """Controls whether the project should be build and installed with cmake. Defaults to true.
    Set it to false when some library does not provide CMakelists.txt or is header-only.
    """
    cmake_build: Optional[bool]
    """Some of the projects have CMakeLists.txt inside non project-root directory. If that's the
    case provide relative path to the project root that points to the directory containing
    CMakeLists
    """
    cmake_directory: Optional[str]
    """Dictionary of key value pairs, where keys are
    cmake variable names.
    Those values strictly correspond to the -D{key}={value} args provided in the command line
    when configuring with cmake
    """
    configuration: Optional[Dict[str, Any]]
    """After sourcing the file, provide a list of copy objects
    that describe what to copy to the install directory
    """
    copy: Optional[List[Copy]]
    """Name of the library that will be used"""
    name: str
    patch: Optional[List[Patch]]
    """Target url that points to either git repository or
    tar gz file with the sources
    """
    url: str
    """Target version of the library that should be used.
    When url points to git repository, the version should match the release tag of the
    repository
    """
    version: str

    def __init__(self, cmake_build: Optional[bool], cmake_directory: Optional[str], configuration: Optional[Dict[str, Any]], copy: Optional[List[Copy]], name: str, patch: Optional[List[Patch]], url: str, version: str) -> None:
        self.cmake_build = cmake_build
        self.cmake_directory = cmake_directory
        self.configuration = configuration
        self.copy = copy
        self.name = name
        self.patch = patch
        self.url = url
        self.version = version

    @staticmethod
    def from_dict(obj: Any) -> 'Requirement':
        assert isinstance(obj, dict)
        cmake_build = from_union(
            [from_bool, from_none], obj.get("cmake_build"))
        cmake_directory = from_union(
            [from_str, from_none], obj.get("cmake_directory"))
        configuration = from_union([lambda x: from_dict(
            lambda x: x, x), from_none], obj.get("configuration"))
        copy = from_union([lambda x: from_list(
            Copy.from_dict, x), from_none], obj.get("copy"))
        name = from_str(obj.get("name"))
        patch = from_union([lambda x: from_list(
            Patch.from_dict, x), from_none], obj.get("patch"))
        url = from_str(obj.get("url"))
        version = from_str(obj.get("version"))
        return Requirement(cmake_build, cmake_directory, configuration, copy, name, patch, url, version)

    def to_dict(self) -> dict:
        result: dict = {}
        result["cmake_build"] = from_union(
            [from_bool, from_none], self.cmake_build)
        result["cmake_directory"] = from_union(
            [from_str, from_none], self.cmake_directory)
        result["configuration"] = from_union(
            [lambda x: from_dict(lambda x: x, x), from_none], self.configuration)
        result["copy"] = from_union([lambda x: from_list(
            lambda x: to_class(Copy, x), x), from_none], self.copy)
        result["name"] = from_str(self.name)
        result["patch"] = from_union([lambda x: from_list(
            lambda x: to_class(Patch, x), x), from_none], self.patch)
        result["url"] = from_str(self.url)
        result["version"] = from_str(self.version)
        return result


class Iwd:
    """Minimal iwd version required"""
    iwd_version: str
    """List of requirements"""
    requirements: List[Requirement]

    def __init__(self, iwd_version: str, requirements: List[Requirement]) -> None:
        self.iwd_version = iwd_version
        self.requirements = requirements

    @staticmethod
    def from_dict(obj: Any) -> 'Iwd':
        assert isinstance(obj, dict)
        iwd_version = from_str(obj.get("iwd_version"))
        requirements = from_list(
            Requirement.from_dict, obj.get("requirements"))
        return Iwd(iwd_version, requirements)

    def to_dict(self) -> dict:
        result: dict = {}
        result["iwd_version"] = from_str(self.iwd_version)
        result["requirements"] = from_list(
            lambda x: to_class(Requirement, x), self.requirements)
        return result


def iwd_from_dict(s: Any) -> Iwd:
    return Iwd.from_dict(s)


def iwd_to_dict(x: Iwd) -> Any:
    return to_class(Iwd, x)
