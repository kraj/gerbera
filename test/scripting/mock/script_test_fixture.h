#ifdef HAVE_JS
#ifndef GERBERA_SCRIPTTESTFIXTURE_H
#define GERBERA_SCRIPTTESTFIXTURE_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <duktape.h>
#include <memory>

#include "util/string_converter.h"

#include "common_script_mock.h"

using namespace ::testing;
using namespace std;

struct addCdsObjectParams {
    map<string, string> objectValues;
    string containerChain;
    string objectType;
};

struct abcBoxParams {
    string inputValue;
    int boxType;
    string divChar;
};

struct getRootPathParams {
    string objScriptPath;
    string origObjLocation;
};

struct copyObjectParams {
    bool isObject;
};

struct getCdsObjectParams {
    string location;
};

// The class provides a way to test the Duktape scripts
// providing various c++ translations of script functions
// useful for mocking said functions with expectations.
class ScriptTestFixture : public ::testing::Test {
    // Loads common.js if running test for another script
    void loadCommon(duk_context* ctx);

public:
    // Builds up the Duktape context
    // Reads in the script under test
    void SetUp() override;

    // Destroys the Duktape context
    void TearDown() override;

    // Creates a mock item(orig) global object in Duktape context
    duk_ret_t dukMockItem(duk_context* ctx, const string& mimetype, const string& id, int theora, const string& title,
        const map<string, string>& meta, const map<string, string>& aux, const map<string, string>& res,
        const string& location, int online_service);

    // Creates a mock playlist global object in Duktape context
    duk_ret_t dukMockPlaylist(duk_context* ctx, const string& title, const string& location, const string& mimetype);

    // Add global Duktape methods to proxy into c++ layer
    void addGlobalFunctions(duk_context* ctx, const duk_function_list_entry* funcs, const std::map<std::string_view, std::string_view>& config = {});

    // Add config entries to global context
    static void addConfig(duk_context* ctx, const std::map<std::string_view, std::string_view>& config);

    // Access the global object(script) by name, and execute
    void executeScript(duk_context* ctx);

    // Proxy the common.js script with `createContainerChain`
    // Mimics the creation of a directory chain
    // Returns the chain as a vector of strings (for easy expectations)
    static vector<string> createContainerChain(duk_context* ctx);

    // Proxy the common.js script with `getLastPath`
    // Mimics finding the last path of the item
    // Pushes the last path value to the Duktape stack
    // Returns the inputPath parameter sent by the script
    static string getLastPath(duk_context* ctx);

    // Proxy the common.js script with `getPlaylistType`
    // Mimics determination of the playlist type
    // Pushes the playlist type `m3u`, `pls` to the duktape stack
    // Returns the input parameter matching the mimetype
    // sent by the script
    static string getPlaylistType(duk_context* ctx);

    // Proxy the common.js script with `print`
    // Mimics the print of text
    // Returns the string sent by the script to print
    static string print(duk_context* ctx);

    // Proxy the common.js script with `getYear`
    // Mimics the parsing of YYYY
    // Pushes the year in YYYY format to the duktape context
    // Returns the full date sent by the script
    static string getYear(duk_context* ctx);

    // Proxy the Duktape script with `addCdsObject` global function.
    // Translates the Duktape value stack to c++
    static addCdsObjectParams addCdsObject(duk_context* ctx, const vector<string>& objectKeys);

    // Proxy the Duktape script with `addContainerTree` C function.
    // Translates the Duktape value stack to c++
    static vector<string> addContainerTree(duk_context* ctx, map<string, string> resMap);

    // Proxy the Duktape script with `abcbox` common.js function
    static abcBoxParams abcBox(duk_context* ctx);

    // Proxy the Duktape script with `getRootPath` common.js function
    static getRootPathParams getRootPath(duk_context* ctx);

    // Proxy the Duktape script with `copyObject` global function
    static copyObjectParams copyObject(duk_context* ctx, const map<string, string>& obj, const map<string, string>& meta);

    // Proxy the Duktape script with `copyObject` global function
    static getCdsObjectParams getCdsObject(duk_context* ctx, const map<string, string>& obj, const map<string, string>& meta);

    // Script file name under test
    // System defaults to known project path `/scripts/js/<scriptName>`
    string scriptName;
    // Select audio layout to test
    string audioLayout;

    // The Duktape Context
    duk_context* ctx;
};

#endif //GERBERA_SCRIPTTESTFIXTURE_H
#endif //HAVE_JS
