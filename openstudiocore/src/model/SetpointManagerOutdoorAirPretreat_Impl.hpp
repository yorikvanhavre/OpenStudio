/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_IMPL_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager_Impl.hpp>

namespace openstudio {
namespace model {

class Node;

namespace detail {

  /** SetpointManagerOutdoorAirPretreat_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerOutdoorAirPretreat.*/
  class MODEL_API SetpointManagerOutdoorAirPretreat_Impl : public SetpointManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerOutdoorAirPretreat_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    SetpointManagerOutdoorAirPretreat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    SetpointManagerOutdoorAirPretreat_Impl(const SetpointManagerOutdoorAirPretreat_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~SetpointManagerOutdoorAirPretreat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> controlVariable() const;

    double minimumSetpointTemperature() const;

    bool isMinimumSetpointTemperatureDefaulted() const;

    double maximumSetpointTemperature() const;

    bool isMaximumSetpointTemperatureDefaulted() const;

    double minimumSetpointHumidityRatio() const;

    bool isMinimumSetpointHumidityRatioDefaulted() const;

    double maximumSetpointHumidityRatio() const;

    bool isMaximumSetpointHumidityRatioDefaulted() const;

    boost::optional<Node> referenceSetpointNode() const;

    boost::optional<Node> mixedAirStreamNode() const;

    boost::optional<Node> outdoorAirStreamNode() const;

    boost::optional<Node> returnAirStreamNode() const;

    virtual boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(boost::optional<std::string> controlVariable);

    void resetControlVariable();

    void setMinimumSetpointTemperature(double minimumSetpointTemperature);

    void resetMinimumSetpointTemperature();

    void setMaximumSetpointTemperature(double maximumSetpointTemperature);

    void resetMaximumSetpointTemperature();

    bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

    void resetMinimumSetpointHumidityRatio();

    bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

    void resetMaximumSetpointHumidityRatio();

    bool setReferenceSetpointNode(const boost::optional<Node>& node);

    void resetReferenceSetpointNode();

    bool setMixedAirStreamNode(const Node& node);

    bool setOutdoorAirStreamNode(const Node& node);

    bool setReturnAirStreamNode(const Node& node);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    virtual bool setSetpointNode(const Node& node);

    virtual void resetSetpointNode();

    REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirPretreat");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    // boost::optional<Node> optionalMixedAirStreamNode() const;
    // boost::optional<Node> optionalOutdoorAirStreamNode() const;
    // boost::optional<Node> optionalReturnAirStreamNode() const;
    // boost::optional<Node> optionalSetpointNodeorNodeList() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_IMPL_HPP

