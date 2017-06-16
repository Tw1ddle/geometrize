## TODO NEEDS WORK

# Supported languages
LANGUAGES = de en fr it ja zh

# Make the ts files show up in the Qt Creator file browser
OTHER_FILES += $$files($$PWD/*.ts, true)

# Get the TS files based on the supported languages
defineReplace(prependAll) {
    for(a,$$1):result ''= $$2$${a}$$3
    return($$result)
}
TRANSLATIONS = $$prependAll(LANGUAGES, $$PWD/translations/app/, .ts)

#wd = $$replace($$_PRO_FILE_PWD_, /, $$QMAKE_DIR_SEP)

# need to run: lupdate "C:\Users\admin\Desktop\Cpp Coding\geometrize\geometrize\geometrize.pro" # -ts "C:\Users\admin\Desktop\Cpp Coding\geometrize\geometrize\translations\de_untranslated.ts"

# Automatically create ts files for the given languages
#qtPrepareTool(LUPDATE, lupdate)
#LUPDATE += -locations relative -no-ui-lines

#TSFILES = $$files($$shell_quote($$_PRO_FILE_PWD_/translations/app/*.ts))
#for(file, TSFILES)
#{
#    command = cd $$shell_quote($$_PRO_FILE_PWD_) && $$LUPDATE $$shell_quote($$_PRO_FILE_) -ts $${file}
#    system($$command)|error("Failed to run: $$command")
#}

# Generate qm files from the ts files for the supported languages and place them in the resources folder, ready to be bundled as resources
# Note that qm files for new languages need to be added to the translations resource file manually
qtPrepareTool(LRELEASE, lrelease)
for(filename, LANGUAGES) {
    tsfile = $$shell_quote($$_PRO_FILE_PWD_/../translations/app/geometrize_$${filename}.ts)
    qmfile = $$shell_quote($$_PRO_FILE_PWD_/resources/translations/app/geometrize_$${filename}.qm)
    qmfile ~= s,.ts$,.qm,
    qmdir = $$dirname(qmfile)
    command = $$LRELEASE -removeidentical $${tsfile} -qm $${qmfile}
    system($$command)|error("Failed to run: $$command")
}
