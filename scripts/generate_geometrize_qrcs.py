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

# Returns the immediate subdirectories of the given directory
def get_immediate_subdirectories(dir):
    return [name for name in os.listdir(dir)
            if os.path.isdir(os.path.join(dir, name))]

def generate_geometrize_qrc(subdir, include_regex, out_dir = None):
    if out_dir is None:
        out_dir = os.path.join(resources_directory, subdir + ".qrc")

    result = generate_qrc.generate_qrc(os.path.join(resources_directory, subdir), out_dir, subdir, include_regex)
    if result != 0:
        print("Failed to generate qrc for resource directory: " + subdir)
        sys.exit(1)

# Generates .qrcs for every Geometrize template in the given directory
# This is used to create a separate resource file for every single Geometrize template
# This is necessary because MSVC chokes on large binary resource files
# We could also work around this issue by shipping the templates as a single external binary
def generate_geometrize_template_qrcs(dir, include_regex):
    template_dirs = get_immediate_subdirectories(dir)
    for template_dir in template_dirs:
        generate_geometrize_qrc(os.path.join(dir, template_dir), include_regex, os.path.join(resources_directory, "template_" + template_dir + ".qrc"))

generate_geometrize_qrc("flags", ".*")
generate_geometrize_qrc("icons", ".*")
generate_geometrize_qrc("licenses", ".*\.txt")
generate_geometrize_qrc("logos", ".*")
generate_geometrize_qrc("scripts", ".*\.chai")
generate_geometrize_qrc("settings", ".*\.json")

generate_geometrize_template_qrcs("templates/templates", ".*")

generate_geometrize_qrc("translations", ".*\.qm")
generate_geometrize_qrc("web_export", "web_export/templates/.*\.(html|js)")

sys.exit(0)