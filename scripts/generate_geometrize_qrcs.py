# Helper script that generates the .qrc resource files for Geometrize.
# Invoked during builds to ensure embedded resources always stay up-to-date.

import os
import sys

import generate_qrc

# Figure out where the Geometrize resources directory is
# Warning this is hardcoded, if relative positions of the script and resources change, this breaks
script_directory = os.path.dirname(os.path.realpath(__file__))
resources_directory = os.path.abspath(script_directory + "/../resources/")

if not os.path.isdir(resources_directory):
    print("Failed to find Geometrize resources directory")
    sys.exit(1)

def generate_geometrize_qrc(subdir, include_regex):
    result = generate_qrc.generate_qrc(os.path.join(resources_directory, subdir), os.path.join(resources_directory, subdir + ".qrc"), subdir, include_regex)
    if result != 0:
        print("Failed to generate qrc for resource directory: " + subdir)
        sys.exit(1)

generate_geometrize_qrc("flags", ".*")
generate_geometrize_qrc("icons", ".*")
generate_geometrize_qrc("licenses", ".*\.txt")
generate_geometrize_qrc("logos", ".*")
generate_geometrize_qrc("scripts", ".*\.chai")
generate_geometrize_qrc("settings", ".*\.json")
generate_geometrize_qrc("templates", "templates/templates/.*")
generate_geometrize_qrc("translations", ".*\.qm")
generate_geometrize_qrc("web_export", "web_export/templates/.*\.(html|js)")

sys.exit(0)