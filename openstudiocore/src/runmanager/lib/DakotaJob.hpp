/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef RUNMANAGER_LIB_DAKOTAJOB_HPP
#define RUNMANAGER_LIB_DAKOTAJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include "ToolBasedJob.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {

namespace detail {

  /** Implementation of ToolBasedJob for executing Dakota jobs, which run in parallel to 
   *  related simualtion workflows. */
  class DakotaJob : public ToolBasedJob {
    Q_OBJECT;
   public:
    DakotaJob(const UUID &t_uuid,
              const Tools &tools,
              const JobParams &params,
              const Files &files,
      const JobState &t_restoreData);

     virtual ~DakotaJob();

    protected:
     virtual std::string descriptionImpl() const override;
     virtual std::string detailedDescriptionImpl() const override;

     /// Returns required version of Dakota
     virtual ToolVersion getToolVersionImpl(const std::string &t_toolName) const override;

     /// Actual execution implementation
     virtual void startHandlerImpl() override;

     virtual void basePathChanged() override;
    private:
     void getFiles(const Files &t_files, const JobParams &t_params) const;

     REGISTER_LOGGER("openstudio.runmanager.DakotaJob");

     /// May be updated and cached at any point
     mutable boost::optional<openstudio::runmanager::FileInfo> m_inFile;

     mutable boost::optional<std::string> m_filelocationname;

     std::string m_description; //< Description of job
  }; 

} // detail

} // runmanager
} // openstudio

#endif // RUNMANAGER_LIB_DAKOTAJOB_HPP
