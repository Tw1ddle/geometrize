#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include <QObject>

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{
class Shape;
}

namespace geometrize
{

namespace script
{

/**
 * @brief The GeometrizerEngine class encapsulates script-based setup and mutation methods for geometrizing shapes.
 */
class GeometrizerEngine : public QObject
{
    Q_OBJECT

public:
    GeometrizerEngine();
    GeometrizerEngine& operator=(const GeometrizerEngine&) = delete;
    GeometrizerEngine(const GeometrizerEngine&) = delete;
    virtual ~GeometrizerEngine();

    /**
     * @brief makeShapeCreator Returns a function that generates shapes for the core geometrization algorithm
     * This is passed to the geometrization algorithm and called across many threads. So it's unsafe to change any
     * state used by the script engine while we're busy adding shapes.
     * @return A function that generates shapes for the core geometrization algorithm, for passing to an ImageRunner.
     */
    std::function<std::shared_ptr<geometrize::Shape>()> makeShapeCreator();

    /**
     * @brief getEngine Gets a pointer to the script engine used by the shape mutation engine.
     * @return A pointer to the script engine used by the shape mutation engine.
     */
    chaiscript::ChaiScript* getEngine();

    /**
     * @brief setupScripts Sets the scripted setup and mutation rules on the current shape mutator.
     * @param functions A map of function identifiers to Chaiscript code that will be added to the engine.
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void setupScripts(const std::map<std::string, std::string>& functions);

    /**
     * @brief resetEngine Resets the scripts and the script engine (including global variables) to their defaults.
     * @param functions A map of function identifiers to Chaiscript code that will be added to the engine (in case we have to override any of the defaults).
     * Note that this is not threadsafe. It must only be called when nothing is using the functions i.e. when related tasks are stopped.
     */
    void resetEngine(const std::map<std::string, std::string>& functions);

signals:
    /**
     * @brief signal_scriptEvaluationSucceeded Signal dispatched when a script is successfully parsed/evaluated.
     * @param functionName The name of the function.
     * @param code The code of the function.
     */
    void signal_scriptEvaluationSucceeded(const std::string& functionName, const std::string& code);

    /**
     * @brief signal_scriptEvaluationFailed Signal dispatched when a script fails to parse/evaluate.
     * @param functionName The name of the function.
     * @param code The code of the function.
     * @param error The text of the error message, if any.
     */
    void signal_scriptEvaluationFailed(const std::string& functionName, const std::string& code, const std::string& errorMessage);

    /**
     * @brief signal_didResetState Signal emitted when the state of the scripting engine is reset.
     * This is when this happens, any globals that were set externally need to be set once again.
     */
    void signal_didResetState();

private:
    class GeometrizerEngineImpl;
    std::unique_ptr<GeometrizerEngineImpl> d;
};

}

}
