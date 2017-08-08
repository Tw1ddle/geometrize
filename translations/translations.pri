# Supported languages
LANGUAGES = de en fr it ja zh

# Make the ts files show up in the Qt Creator file browser
OTHER_FILES += $$files($${PWD}/*.ts, true)

# Set the ts file paths based on the specified supported languages
defineReplace(prependAll) {
    for(a,$$1):result ''= $$2$${a}$$3
    return($$result)
}
TRANSLATIONS = $$prependAll(LANGUAGES, $${PWD}/app/, .ts)

# TODO need to run: lupdate "C:\Users\admin\Desktop\Cpp Coding\geometrize\geometrize\geometrize.pro" # -ts "C:\Users\admin\Desktop\Cpp Coding\geometrize\geometrize\translations\de_untranslated.ts" for each language
# Automatically update the ts files for the given languages
#qtPrepareTool(LUPDATE, lupdate)
#LUPDATE += -locations relative -no-ui-lines
#TSFILES = $$TRANSLATIONS
#for(file, TSFILES) {
#    message($$file)
#    command = cd $$shell_quote($$_PRO_FILE_PWD_) && $$LUPDATE $$shell_quote($$_PRO_FILE_) -ts $$shell_quote($${file})
#    system($$command)
#}

# Generate qm files from the ts files for the supported languages and place them in the resources folder, ready to be bundled as resources
qtPrepareTool(LRELEASE, lrelease)
for(language, LANGUAGES) {
    tsfile = $$shell_path($$shell_quote($${PWD}/app/geometrize_$${language}.ts))
    qmfile = $$shell_path($$shell_quote($${PWD}/../resources/translations/app/geometrize_$${language}.qm))
    command = $$shell_path($$shell_quote($${LRELEASE})) -removeidentical $${tsfile} -qm $${qmfile}
    system($${command})|error("Failed to generate qm file")
}
