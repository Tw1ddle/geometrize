# Build script that generates a Qt Resource .qrc file by recursively scanning a directory for files.
# Returns 0 if the .qrc file was generated successfully, else 1.

# Example usage:
# python generate_qrc_standalone.py "C:/app/scripts" "C:/app/test.qrc"
# python generate_qrc_standalone.py "C:/app/scripts" "C:/app/test.qrc" --include_pattern=".*\.txt"
# python generate_qrc_standalone.py directory_to_scan qrc_output_file --include_pattern="inclusion_regex"
# python generate_qrc_standalone.py directory_to_scan qrc_output_file --include_pattern="inclusion_regex" --prefix="images/"

import argparse
import sys

import generate_qrc

# Setup argument parser
parser = argparse.ArgumentParser(description='A script for generating a .qrc file by recursively scanning a directory')
parser.add_argument('input_directory', help = 'Directory that will be scanned for files to add to the .qrc')
parser.add_argument('output_file', help = 'Filepath where the generated .qrc will be placed')
parser.add_argument('--prefix', help = 'qrc file qresource prefix')
parser.add_argument('--include_pattern', help = 'File inclusion regex string')
args = parser.parse_args()

# Call generator
result = generate_qrc.generate_qrc(args.input_directory, args.output_file, args.prefix, args.include_pattern)

sys.exit(result)