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

#ifndef ANALYSIS_DESIGNOFEXPERIMENTSOPTIONS_HPP
#define ANALYSIS_DESIGNOFEXPERIMENTSOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "AlgorithmOptions.hpp"

#include "../utilities/core/Enum.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class DesignOfExperimentsOptions_Impl;

} // detail

/** \class DesignOfExperimentsType 
 *
 *  \relates DesignOfExperimentsOptions */
OPENSTUDIO_ENUM( DesignOfExperimentsType,
  ((FullFactorial)(full factorial))
);

/** DesignOfExperimentsOptions is an AlgorithmOptions class for use with DesignOfExperiments.
 *  \relates DesignOfExperiments */
class ANALYSIS_API DesignOfExperimentsOptions : public AlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DesignOfExperimentsOptions(const DesignOfExperimentsType& designType);

  /** Constructor provided for deserialization; not for general use. */
  DesignOfExperimentsOptions(const DesignOfExperimentsType& designType,
                             const std::vector<Attribute>& options);

  virtual ~DesignOfExperimentsOptions() {}

  //@}
  /** @name Getters and Queries */
  //@{

  DesignOfExperimentsType designType() const;

  //@}
  /** @name Setters */
  //@{

  void setDesignType(const DesignOfExperimentsType& designType);

  //@}
 protected:
  /// @cond
  typedef detail::DesignOfExperimentsOptions_Impl ImplType;

  explicit DesignOfExperimentsOptions(std::shared_ptr<detail::DesignOfExperimentsOptions_Impl> impl);

  friend class detail::DesignOfExperimentsOptions_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DesignOfExperimentsOptions");
};

/** \relates DesignOfExperimentsOptions*/
typedef boost::optional<DesignOfExperimentsOptions> OptionalDesignOfExperimentsOptions;

/** \relates DesignOfExperimentsOptions*/
typedef std::vector<DesignOfExperimentsOptions> DesignOfExperimentsOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DESIGNOFEXPERIMENTSOPTIONS_HPP

