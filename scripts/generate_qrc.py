# Build script that generates a Qt Resource .qrc file by recursively scanning a directory for files.
# Returns 0 if the .qrc file was generated successfully, else 1.

import os
import re

def generate_qrc(input_directory, output_file, prefix = '/', include_pattern = '.*'):
    # Compile inclusion regex
    include_regex = re.compile(include_pattern)

    # Validate arguments
    if not os.path.isdir(input_directory):
        print("Input directory does not exist")
        return 1

    if not os.path.isdir(os.path.dirname(os.path.realpath(output_file))):
        print("Output directory does not exist")
        return 1

    # Try to remove any file at destination
    if os.path.isfile(output_file):
        print("Output file already exists: " + output_file)
        print("Deleting existing .qrc file")
        os.remove(output_file)

    # Open .qrc output file
    out = open(output_file, 'w')

    # Write .qrc file contents
    out.write('<RCC>\n')
    out.write('    <qresource prefix="/">\n')
    for (dirpath, dirnames, filenames) in os.walk(input_directory):
        for f in filenames:
            # Note we save forward slashes instead of backslashes, for cross-platform consistency
            filepath = os.path.join(dirpath, f)
            relpath = (prefix + "/" + os.path.relpath(filepath, input_directory)).replace("\\","/")
            if include_regex.search(relpath):
                out.write('        <file>' + relpath + '</file>\n')
    out.write('    </qresource>\n')
    out.write('</RCC>')

    print("Successfully wrote generated .qrc file to: " + output_file)
    return 0