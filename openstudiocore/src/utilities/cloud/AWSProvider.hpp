/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_CLOUD_AWSPROVIDER_HPP
#define UTILITIES_CLOUD_AWSPROVIDER_HPP

#include <utilities/cloud/CloudProvider.hpp>

namespace openstudio {
namespace detail {

  class AWSSettings_Impl;
  class AWSSession_Impl;
  class AWSProvider_Impl;

}

  /// AWSSettings is a CloudSettings.
  class UTILITIES_API AWSSettings : public CloudSettings {
   public:
    /** @name Constructor */
    //@{

    /// default constructor, loads defaults from settings
    AWSSettings();

    /** Constructor provided for deserialization; not for general use. */
    AWSSettings(const UUID& uuid,
                const UUID& versionUUID,
                bool userAgreementSigned,
                unsigned numWorkers,
                bool terminationDelayEnabled,
                unsigned terminationDelay);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSSettings() {};

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

    // returns the AWS access key
    std::string accessKey() const;

    // sets the AWS access key if it's valid
    bool setAccessKey(const std::string& accessKey);

    // returns the AWS secret key
    std::string secretKey() const;

    // sets the AWS secret key if it's valid
    bool setSecretKey(const std::string& secretKey);

    // performs a cursory regex and returns true if it's valid
    bool validAccessKey(const std::string& accessKey) const;

    // performs a cursory regex and returns true if it's valid
    bool validSecretKey(const std::string& secretKey) const;

    // returns the saved default number of workers
    unsigned numWorkers() const;

    // set the number of worker nodes to start (and returns the new number)
    unsigned setNumWorkers(const unsigned numWorkers);

    // returns true if there should be a delay before terminating after simulations are complete
    bool terminationDelayEnabled() const;

    // sets whether a termination delay should occur
    void setTerminationDelayEnabled(bool enabled);

    // returns the termination delay in minutes
    unsigned terminationDelay() const;

    // sets the termination delay in minutes
    void setTerminationDelay(const unsigned delay);

    //@}
   protected:

    AWSSettings(const boost::shared_ptr<detail::AWSSettings_Impl>& impl);

    typedef detail::AWSSettings_Impl ImplType;

    friend class CloudSettings;

   private:

  };

  /// \relates AWSSettings
  typedef boost::optional<AWSSettings> OptionalAWSSettings;

  /// AWSSession is a CloudSession.
  class UTILITIES_API AWSSession : public CloudSession {
   public:

    /** @name Constructor */
    //@{

    //constructor
    AWSSession(const std::string& sessionId,
               const boost::optional<Url>& serverUrl,
               const std::vector<Url>& workerUrls);

    /** Constructor provided for deserialization; not for general use. */
    AWSSession(const UUID& uuid,
               const UUID& versionUUID,
               const std::string& sessionId,
               const boost::optional<Url>& serverUrl,
               const std::vector<Url>& workerUrls);

    /** Full constructor */
    AWSSession(const UUID& uuid,
               const UUID& versionUUID,
               const std::string& sessionId,
               const boost::optional<Url>& serverUrl,
               const std::string& serverId,
               const unsigned numServerProcessors,
               const std::vector<Url>& workerUrls,
               const std::vector<std::string>& workerIds,
               const unsigned numWorkerProcessors,
               const std::string& privateKey,
               const std::string& timestamp,
               const std::string& region,
               const std::string& serverInstanceType,
               const std::string& workerInstanceType);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSSession() {};

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

    // returns the url of the server node
    Url serverUrl() const;

    // sets the url of the server node
    void setServerUrl(const Url& serverUrl);

    // returns the server instance ID
    std::string serverId() const;

    // sets the server instance ID
    void setServerId(const std::string& serverId);

    // returns the number of server processor cores
    unsigned numServerProcessors() const;

    // sets the number of server processor cores
    void setNumServerProcessors(const unsigned numServerProcessors);

    // returns the urls of all worker nodes 
    std::vector<Url> workerUrls() const;

    // set the urls of all worker nodes
    void setWorkerUrls(const std::vector<Url>& workerUrls);

    // returns the worker instance IDs
    std::vector<std::string> workerIds() const;

    // sets the worker instance IDs
    void setWorkerIds(const std::vector<std::string>& workerIds);

    // returns the number of processor cores per worker
    unsigned numWorkerProcessors() const;

    // sets the number of processor cores per worker
    void setNumWorkerProcessors(const unsigned numWorkerProcessors);

    // returns the key pair's private key
    std::string privateKey() const;

    // sets the key pair's private key
    void setPrivateKey(const std::string& privateKey);

    // returns the timestamp associated with the security group and key pair
    std::string timestamp() const;
    
    // sets the timestamp
    void setTimestamp(const std::string& timestamp);

    // returns the AWS region
    std::string region() const;

    // sets the AWS region
    void setRegion(const std::string& region);

    // returns the server instance type
    std::string serverInstanceType() const;

    // sets the server instance type
    void setServerInstanceType(const std::string& instanceType);

    // returns the worker instance type
    std::string workerInstanceType() const;

    // sets the worker instance type
    void setWorkerInstanceType(const std::string& instanceType);

    // returns the number of workers for this session
    unsigned numWorkers() const;

    // returns the total uptime in minutes of this session
    unsigned totalSessionUptime() const;

    // returns the total number of instances running on EC2 associated with this session
    unsigned totalSessionInstances() const;

    //@}

   protected:

    AWSSession(const boost::shared_ptr<detail::AWSSession_Impl>& impl);

    typedef detail::AWSSession_Impl ImplType;

    friend class CloudSession;

   private:

  };

  /// \relates AWSSession
  typedef boost::optional<AWSSession> OptionalAWSSession;

  /// AWSProvider is a CloudProvider that provides access to Amazon EC2 and CloudWatch services.
  class UTILITIES_API AWSProvider : public CloudProvider {
  public:

    /** @name Constructor */
    //@{

    /// default constructor
    AWSProvider();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSProvider() {};

    //@}
    /** @name Class members */
    //@{

    std::string userAgreementText() const;

    bool userAgreementSigned() const;

    void signUserAgreement(bool agree);

    // returns the AWS access key
    std::string accessKey() const;

    // sets the AWS access key if it's valid
    bool setAccessKey(const std::string& accessKey);

    // returns the AWS secret key
    std::string secretKey() const;

    // sets the AWS secret key if it's valid
    bool setSecretKey(const std::string& secretKey);

    // returns the number of worker nodes
    unsigned numWorkers() const;

    // set the number of worker nodes to start (and returns the new number)
    unsigned setNumWorkers(const unsigned numWorkers);

    // return a list of available AWS regions
    std::vector<std::string> availableRegions() const;

    // return the recommended default region
    std::string defaultRegion() const;

    // returns the AWS region
    std::string region() const;

    // sets the AWS region
    void setRegion(const std::string& region);

    // returns a list of server instance types
    std::vector<std::string> serverInstanceTypes() const;

    // returns the recommended default server instance type
    std::string defaultServerInstanceType() const;

    // returns the server instance type
    std::string serverInstanceType() const;

    // sets the server instance type
    void setServerInstanceType(const std::string& instanceType);

    // returns a list of worker instance types
    std::vector<std::string> workerInstanceTypes() const;

    // returns the recommended default worker instance type
    std::string defaultWorkerInstanceType() const;

    // returns the worker instance type
    std::string workerInstanceType() const;

    // sets the worker instance type
    void setWorkerInstanceType(const std::string& instanceType);

    // returns true if there should be a delay before terminating after simulations are complete
    bool terminationDelayEnabled() const;

    // sets whether a termination delay should occur
    void setTerminationDelayEnabled(bool enabled);

    // returns the termination delay in minutes
    unsigned terminationDelay() const;

    // sets the termination delay in minutes
    void setTerminationDelay(const unsigned delay);

    // returns the number of workers for this session
    unsigned numSessionWorkers() const;

    // returns the EC2 estimated charges from CloudWatch in USD
    double estimatedCharges() const;

    // returns the total uptime in minutes of this session
    unsigned totalSessionUptime() const;

    // returns the total number of instances running on EC2 associated with this session
    unsigned totalSessionInstances() const;

    // returns the total number of instances running on EC2
    unsigned totalInstances() const;


    //@}
  protected:

    AWSProvider(const boost::shared_ptr<detail::AWSProvider_Impl>& impl);

    typedef detail::AWSProvider_Impl ImplType;

    friend class CloudProvider;

  private:

    // no body on purpose, do not want this generated
    AWSProvider(const AWSProvider& other);

  };

} // openstudio

#endif // UTILITIES_CLOUD_AWSPROVIDER_HPP
