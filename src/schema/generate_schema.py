#!/usr/bin/env python3

import contextlib
import json
import pathlib
import sys

THIS_DIR = pathlib.Path(__file__).parent.absolute()
SCHEMA_PATH =  THIS_DIR / "schema.json"
TYPES_HEADER_DIR = pathlib.Path(sys.argv[1])
TYPES_SRC_DIR = pathlib.Path(sys.argv[2])

class FileWriter:
    def __init__(self, path):
        self._path = path

    def __enter__(self):
        self._f = self._path.open(mode="wt")
        return self

    def __exit__(self, ext_type, exc_val, exc_tb):
        self._f.close()
        return False

    def write(self, text=""):
        self._f.write(text)

    def writel(self, text=""):
        self._f.write(text)
        self._f.write("\n")

    def writei(self, indent_level=0, text=""):
        self._f.write("    " * indent_level)
        self._f.write(text)

    def writeli(self, indent_level=0, text=""):
        self.writei(indent_level, text)
        self._f.write("\n")
        
class TypeHeaderWriter(FileWriter):

    def __init__(self, name, info):
        self._name = name
        self._info = info
        self._impl_name = f"{self._name}Impl"
        super().__init__(TYPES_HEADER_DIR / f"{self._name}.gen.h")

    def write_all(self):
        with include_guard(self, f"field_types_{self._name}"):
            self.writel()
            self.write_includes()
            with namespace(self, "sq::field_types"):
                self.writel()
                self.write_fwd_decls()
                with class_decl(self, self._name, "Field"):
                    self.write_public_members()
                    self.writel()
                    self.write_private_members()

    def write_includes(self):
        self.writel("#include <memory>")
        self.writel()
        self.writel("#include \"field_types/Field.h\"")

    def write_fwd_decls(self):
        self.writel(f"class {self._impl_name};")

    def write_public_members(self):
        self.writel("public:")
        self.write_typedefs();
        self.writel()
        self.write_ctor()
        self.writel()
        self.write_create()
        self.writel()
        self.write_dtor_decl()
        self.write_generic_getter_decl()
        for member, member_info in self._info.items():
            self.write_getter_decl(member)
        self.write_to_primitive_decl()

    def write_typedefs(self):
        self.writeli(1, f"using ImplPtr = std::unique_ptr<{self._impl_name}>;")

    def write_ctor(self):
        self.writeli(1, "template <typename... Ts>")
        self.writeli(1, f"explicit {self._name}(Ts&&... args)")
        self.writeli(2, f": impl_{{std::make_unique<{self._impl_name}>(std::forward<Ts>(args)...)}}")
        self.writeli(1, "{}")

    def write_create(self):
        self.writeli(1, "template <typename... Ts>")
        self.writeli(1, "static FieldPtr create(Ts&&... args)")
        self.writeli(1, "{")
        self.writeli(2, f"return std::make_unique<{self._name}>(std::forward<Ts>(args)...);")
        self.writeli(1, "}")

    def write_dtor_decl(self):
        self.writeli(1, f"~{self._name}() noexcept override;")

    def write_getter_decl(self, member):
        self.writeli(1, f"Result get_{member}() const;")

    def write_generic_getter_decl(self):
        self.writeli(1, "Result get(std::string_view member) const override;")

    def write_to_primitive_decl(self):
        self.writeli(1, "Primitive to_primitive() const override;")

    def write_private_members(self):
        self.writeli(0, "private:")
        self.writeli(1, f"ImplPtr impl_;")


class TypeCppWriter(FileWriter):

    def __init__(self, name, info):
        self._name = name
        self._info = info
        super().__init__(TYPES_SRC_DIR / f"{self._name}.gen.cpp")

    def write_all(self):
        self.write_includes()
        with namespace(self, "sq::field_types"):
            self.writel()
            self.write_dtor()
            self.writel()
            self.write_generic_getter()

    def write_includes(self):
        self.writel(f"#include \"field_types/InvalidFieldError.h\"")
        self.writel()
        self.writel(f"#include \"field_types/{self._name}.gen.h\"")
        self.writel(f"#include \"field_types/{self._name}Impl.h\"")

    def write_dtor(self):
        self.writel(f"{self._name}::~{self._name}()")
        self.writel("{}")

    def write_generic_getter(self):
        self.writel(f"Result {self._name}::get(std::string_view member) const")
        self.writel("{")
        for cand, cand_info in self._info.items():
            self.writeli(1, f"if (member.compare(\"{cand}\") == 0)")
            self.writeli(1, "{")
            self.writeli(2, f"return get_{cand}();")
            self.writeli(1, "}")
        self.writel()
        self.writeli(1, f"throw InvalidFieldError(\"{self._name}\", member);")
        self.writeli(1, "return nullptr;")
        self.writel("}")

@contextlib.contextmanager
def include_guard(writer, name):
    guard_name = f"SQ_INCLUDE_GUARD_{name}_GEN_H_"
    try:
        writer.writel(f"#ifndef {guard_name}")
        writer.writel(f"#define {guard_name}")
        yield
    finally:
        writer.writel()
        writer.writel(f"#endif // {guard_name}")

@contextlib.contextmanager
def namespace(writer, name):
    try:
        writer.writel()
        writer.writel(f"namespace {name} {{")
        yield
    finally:
        writer.writel()
        writer.writel(f"}} // namespace {name}")

@contextlib.contextmanager
def class_decl(writer, name, parent):
    try:
        writer.writel()
        writer.writel(f"class {name}")
        if parent:
            writer.writeli(1, f": public {parent}")
        writer.writel("{")
        yield
    finally:
        writer.writel("};")

def get_schema():
    with SCHEMA_PATH.open(mode="rb") as s:
        return json.load(s)

def generate_schema_files():
    schema = get_schema()

    TYPES_HEADER_DIR.mkdir(parents=True, exist_ok=True)
    TYPES_SRC_DIR.mkdir(parents=True, exist_ok=True)
    for name, info in schema.items():
        with TypeHeaderWriter(name, info) as header:
            header.write_all()
        with TypeCppWriter(name, info) as cpp:
            cpp.write_all()

generate_schema_files()
