# Automatically generates resource files

win32 {
    PYTHON_EXISTS_CHECK = where python
} else {
    PYTHON_EXISTS_CHECK = whereis python
}

!system($${PYTHON_EXISTS_CHECK}) {
    error("Failed to check existence of Python, or Python cannot be found. Will fail to automatically generate .qrc resources. Is Python in the path?")
}

QRC_GENERATION_COMMAND = python ../scripts/generate_geometrize_qrcs.py

system($${QRC_GENERATION_COMMAND})|error("Automatic qrc resource file generation failed. Try running the Python build scripts manually.")

RESOURCES += $$files($$PWD/*.qrc)

RC_ICONS = $$PWD/app_icons/icon.ico
RC_FILE = $$PWD/app_icons/winicon.rc
