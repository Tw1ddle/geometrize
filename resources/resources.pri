# Automatically generates resource files
# Requires a Python installation in the path
!system(where python) {
    error("Python cannot be found. Will fail to automatically generate .qrc resources. Is Python in the path?")
}

QRC_GENERATION_SCRIPT_PATH = $$shell_quote($${PWD}/../scripts/generate_geometrize_qrcs.py)
QRC_GENERATION_COMMAND = python $${QRC_GENERATION_SCRIPT_PATH}

system($${QRC_GENERATION_COMMAND})|error("Automatic qrc resource file generation failed. Try running the Python build scripts manually.")

RESOURCES += $$files($$PWD/*.qrc)

RC_ICONS = $$PWD/app_icons/icon.ico
RC_FILE = $$PWD/app_icons/winicon.rc
