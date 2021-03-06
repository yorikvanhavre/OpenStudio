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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "XMLPreprocessorJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/Version_FieldEnums.hxx>
#
#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  XMLPreprocessorJob::XMLPreprocessorJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::XMLPreprocessor, t_tools, "xmlpreprocessor", 
        t_params, t_files, true, t_restoreData)
  {
    getFiles(t_files);

    m_description = buildDescription("xml");
  }

  XMLPreprocessorJob::~XMLPreprocessorJob()
  {
    shutdownJob();
  }

  std::string XMLPreprocessorJob::descriptionImpl() const
  {
    return m_description;
  }

  ToolVersion XMLPreprocessorJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "xmlpreprocessor") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    return ToolVersion();
  }

  void XMLPreprocessorJob::startHandlerImpl()
  {
    getFiles(allInputFiles());
    addRequiredFile(*m_xml, toPath("in.xml"));
    copyRequiredFiles(*m_xml, "idf", toPath("in.epw"));
  }

  void XMLPreprocessorJob::basePathChanged()
  {
    m_xml.reset();
  }

  void XMLPreprocessorJob::getFiles(const Files &t_files) const
  {
    if (!m_xml)
    {
      try {
        m_xml = t_files.getLastByExtension("xml");
        m_xml->addRequiredFile(toPath("include"), toPath("include"));
      } catch (const std::runtime_error &) {
      }
    }
  }

} // detail
} // runmanager
} // openstudio



