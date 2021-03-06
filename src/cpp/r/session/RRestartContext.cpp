/*
 * RRestartContext.cpp
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * This program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#include "RRestartContext.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include <core/Log.hpp>
#include <core/Error.hpp>
#include <core/FileUtils.hpp>
#include <core/system/System.hpp>

#include "RSessionState.hpp"

using namespace core ;

namespace r {
namespace session {

namespace {

const char * const kContext = "ctx-";

FilePath restartContextsPath(const FilePath& scopePath)
{
   FilePath contextsPath = scopePath.complete("ctx");
   Error error = contextsPath.ensureDirectory();
   if (error)
      LOG_ERROR(error);
   return contextsPath;
}

} // anonymous namespace

// singleton
RestartContext& restartContext()
{
   static RestartContext instance;
   return instance;
}

RestartContext::RestartContext()
{
}

void RestartContext::initialize(const FilePath& scopePath,
                                const std::string& contextId)
{
   FilePath contextsPath = restartContextsPath(scopePath);
   FilePath statePath = contextsPath.complete(kContext + contextId);
   if (statePath.exists())
      sessionStatePath_ = statePath;
}

bool RestartContext::hasSessionState() const
{
   return !sessionStatePath().empty();
}

FilePath RestartContext::sessionStatePath() const
{
   return sessionStatePath_;
}

void RestartContext::removeSessionState()
{
   r::session::state::destroy(sessionStatePath_);
}

FilePath RestartContext::createSessionStatePath(const FilePath& scopePath,
                                                const std::string& contextId)
{
   FilePath contextsPath = restartContextsPath(scopePath);
   FilePath statePath = contextsPath.complete(kContext + contextId);

   Error error = statePath.ensureDirectory();
   if (error)
      LOG_ERROR(error);
   return statePath;
}

} // namespace session
} // namespace r



