/**********************************************************************************
 *          Project
 *  _______ _________ _______  _______  __    ______
 * (  ____ \\__   __/(  ____ \(  __   )/  \  (  __  \
 * | (    \/   ) (   | (    \/| (  )  |\/) ) | (  \  )
 * | |         | |   | (__    | | /   |  | | | |   ) |
 * | |         | |   |  __)   | (/ /) |  | | | |   | |
 * | |         | |   | (      |   / | |  | | | |   ) |
 * | (____/\   | |   | )      |  (__) |__) (_| (__/  )
 * (_______/   )_(   |/       (_______)\____/(______/
 *
 * MIT License
 * Copyright (c) 2018-2023 Evgenii Sopov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include "argument_processor_clean.h"
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>
#include <employ_config.h>
// ---------------------------------------------------------------------
// ArgumentProcessorClean

ArgumentProcessorClean::ArgumentProcessorClean() 
: WsjcppArgumentProcessor({"clean"}, "Clean all flags from storage", "Clean all flags from storage") {
    TAG = "ArgumentProcessorClean";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorClean::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorClean::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorClean::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::info(TAG, "Cleaning flags from storage...");
    if (!WsjcppEmployees::init({})) {
        WsjcppLog::err(TAG, "Failed");
        return 0;
    }
    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();
    // pEmployConfig->storage()->clean();
    // WsjcppLog::ok(TAG, "Clean completed.");
    WsjcppLog::warn(TAG, "TODO");
    return -1;
}

