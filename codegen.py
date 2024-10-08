import json
import mmap
from pathlib import Path

import argparse
import re


def gen(spec_file_path: Path):
    output_file_name = spec_file_path.stem
    print(f"Generating %s from %s" % (output_file_name, spec_file_path))

    generate_header_guard = spec_file_path.suffixes[0] == '.h'
    header_guard_title = output_file_name.upper().replace(".", "_")

    output_file_path = spec_file_path.parent / output_file_name

    with open(spec_file_path, "r") as spec_file:
        mm = mmap.mmap(spec_file.fileno(), 0, prot=mmap.PROT_READ)

        spec = json.loads(spec_file.read())

        with open(output_file_path, "w") as out:
            out.writelines(f"%s generated from %s\n" % (spec["commentFormat"], spec_file_path))

            if generate_header_guard:
                out.writelines([f"#ifndef %s\n" % header_guard_title, f"#define %s\n" % header_guard_title])

            # evaluate the constants
            constants = dict(map(lambda c: (c[0], str(eval(str(c[1])))), spec["constants"].items()))

            for section in spec["sections"]:
                if section["type"] == "loop":
                    operation = process_expr(section["operation"], constants, None)
                    for i in eval(operation):
                        out.write(process_expr(section["expr"] + "\n", constants, i))
                elif section["type"] == "string":
                    out.writelines(process_expr(section["expr"] + "\n", constants, None))

            if generate_header_guard:
                out.writelines(f"#endif\n")

            out.close()

        mm.close()


def process_expr(expr: str, constants: dict, index: int = None):
    expr = re.sub(r'#\w+', lambda m: constants[m.group()[1:]], expr)

    if index is not None:
        expr = re.sub(r'(?<!@)@(?!@)', str(index), expr)

    return re.sub(r'\{\{.+}}', lambda m: str(eval(m.group()[2:-2])), expr)

def main(process_path: Path):
    if process_path.is_dir():
        [gen(path) for path in process_path.rglob("*.json") if path.is_file()]
    else:
        gen(process_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="GOOSE codegen", description="Generalized code generator for GOOSE" )
    parser.add_argument('process_path')

    args = parser.parse_args()
    main(Path(args.process_path))
