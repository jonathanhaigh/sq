#!/usr/bin/env python3

import jinja2
import json
import pathlib
import sys

THIS_DIR = pathlib.Path(__file__).parent.absolute()
SCHEMA_PATH =  THIS_DIR / "schema.json"
TEMPLATE_PATH = THIS_DIR / "templates"
TEMPLATE_NAME = sys.argv[1]
OUTPUT_PATH_FORMAT = sys.argv[2];

class SqParam:
    def __init__(self, index, schema):
        self.name = schema["name"]
        self.index = index
        self.base_type = schema["type"]
        self.required = schema["required"]
        if self.required:
            self.type = f"const {self.base_type}&"
        else:
            self.type = f"const {self.base_type}*"

class SqField:
    def __init__(self, name, schema):
        self.name = name
        self.return_type = schema["return"]
        self.params = [SqParam(i, s) for i, s in enumerate(schema["params"])]

class SqType:
    def __init__(self, name, schema):
        self.name = name
        self.fields = [SqField(n, s) for n, s in schema.items()]


def get_schema():
    with SCHEMA_PATH.open(mode="rb") as s:
        return json.load(s)

def generate_system_files():

    sq_types = [SqType(n, s) for n, s in get_schema().items()]

    jinja_env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(TEMPLATE_PATH),
        autoescape=False,
        undefined=jinja2.StrictUndefined
    )
    template = jinja_env.get_template(f"{TEMPLATE_NAME}.jinja");

    for sq_type in sq_types:
        path = pathlib.Path(OUTPUT_PATH_FORMAT % sq_type.name)
        path.parent.mkdir(parents=True, exist_ok=True);
        with open(path, "wt") as f:
            f.write(template.render(sq_type=sq_type))

generate_system_files()
