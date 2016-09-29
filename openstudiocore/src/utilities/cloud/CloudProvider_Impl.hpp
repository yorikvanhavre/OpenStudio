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

#ifndef UTILITIES_CLOUD_CLOUDPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_CLOUDPROVIDER_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Optional.hpp"
#include "../core/UUID.hpp"
#include "../core/Url.hpp"
#include "../core/Logger.hpp"

#include <QObject>
#include <QProcess>

namespace openstudio{
  
  class CloudSettings;
  class CloudSession;

namespace detail{
  struct UTILITIES_API ProcessResults {

    ProcessResults(int t_exitCode, QProcess::ExitStatus t_exitStatus, const QString &t_output, const QString &t_error)
      : exitCode(t_exitCode), exitStatus(t_exitStatus), output(t_output), error(t_error)
    {
    }

    int exitCode;
    QProcess::ExitStatus exitStatus;
    QString output;
    QString error; 
  };

  /// CloudSettings_Impl is an abstract base class that returns the information needed to use a CloudProvider (e.g. username, password, etc)
  class UTILITIES_API CloudSettings_Impl {
  public:

    /** Constructor provided for deserialization; not for general use. */
    CloudSettings_Impl(const UUID& uuid,
                       const UUID& versionUUID);

    virtual ~CloudSettings_Impl();

    /** Unique identifier for OSP. */
    UUID uuid() const;

    /** Unique identifier of version for OSP. (To avoid unnecessary overhead when nothing 
     *  has changed.) */
    UUID versionUUID() const;

    virtual std::string cloudProviderType() const = 0;

    virtual std::string userAgreementText() const = 0;

    virtual bool userAgreementSigned() const = 0;

    virtual void signUserAgreement(bool agree) = 0;

    virtual bool loadSettings(bool overwriteExisting = false) = 0;

    virtual bool saveToSettings(bool overwriteExisting = false) const = 0;

  protected:

    CloudSettings_Impl();

    /** Changes the versionUUID. */
    void onChange();

  private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudSettings");

    UUID m_uuid;
    UUID m_versionUUID;
  };

  /// CloudSession_Impl is an abstract base class for the information needed to identify and reconnect to compute nodes started by a previous CloudProvider.
  class UTILITIES_API CloudSession_Impl {
   public:

    /** Constructor provided for deserialization; not for general use. */
    CloudSession_Impl(const UUID& uuid,
                      const UUID& versionUUID,
                      const std::string& sessionId,
                      const boost::optional<Url>& serverUrl,
                      const std::vector<Url>& workerUrls);

    virtual ~CloudSession_Impl();

    /** Unique identifier for OSP. */
    UUID uuid() const;

    /** Unique identifier of version for OSP. (To avoid unnecessary overhead when nothing 
     *  has changed.) */
    UUID versionUUID() const;

    virtual std::string cloudProviderType() const = 0;
    
    std::string sessionId() const;
    
    boost::optional<Url> serverUrl() const;
    
    void setServerUrl(const Url& serverUrl);
    
    void resetServerUrl();
    
    std::vector<Url> workerUrls() const;
    
    void addWorkerUrl(const Url& workerUrl);
    
    void clearWorkerUrls();
  
   protected:
    CloudSession_Impl(const std::string& sessionId, 
                      const boost::optional<Url>& serverUrl, 
                      const std::vector<Url>& workerUrls);

    /** Changes the versionUUID. */
    void onChange();

   private:
    UUID m_uuid;
    UUID m_versionUUID;
    std::string m_sessionId;
    boost::optional<Url> m_serverUrl;
    std::vector<Url> m_workerUrls;

    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudSession");
  };

  /// CloudProvider_Impl is an abstract base class for classes that provide cloud resources.
  class UTILITIES_API CloudProvider_Impl : public QObject {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// constructor
    CloudProvider_Impl();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~CloudProvider_Impl();

    //@}
    /** @name Accessor class members */
    //@{

    virtual std::string type() const = 0;

    virtual unsigned numWorkers() const = 0;

    virtual CloudSettings settings() const = 0;

    virtual bool setSettings(const CloudSettings& settings) = 0;

    virtual CloudSession session() const = 0;

    virtual bool setSession(const CloudSession& session) = 0;

    virtual bool lastInternetAvailable() const = 0;

    virtual bool lastServiceAvailable() const = 0;

    virtual bool lastValidateCredentials() const = 0;

    virtual bool lastResourcesAvailableToStart() const = 0;

    virtual bool serverStarted() const = 0;

    virtual bool workersStarted() const = 0;

    virtual bool lastServerRunning() const = 0;

    virtual bool lastWorkersRunning() const = 0;

    virtual bool lastRunning() const;

    virtual bool terminateStarted() const = 0;

    virtual bool lastTerminateCompleted() const = 0;

    virtual std::vector<std::string> errors() const = 0;
    
    virtual std::vector<std::string> warnings() const = 0;

    //@}
    /** @name Blocking class members */
    //@{

    virtual bool internetAvailable(int msec) = 0;

    virtual bool serviceAvailable(int msec) = 0;

    virtual bool validateCredentials(int msec) = 0;

    virtual bool resourcesAvailableToStart(int msec) = 0;

    virtual bool waitForServer(int msec) = 0;

    virtual bool waitForWorkers(int msec) = 0;

    virtual bool serverRunning(int msec) = 0;

    virtual bool workersRunning(int msec) = 0;

    virtual bool waitForTerminated(int msec) = 0;

    virtual bool terminateCompleted(int msec) = 0;

    //@}
    /** @name Non-blocking class members */
    //@{

    virtual bool requestInternetAvailable() = 0;

    virtual bool requestServiceAvailable() = 0;

    virtual bool requestValidateCredentials() = 0;

    virtual bool requestResourcesAvailableToStart() = 0;

    virtual bool requestStartServer() = 0;

    virtual bool requestStartWorkers() = 0;

    virtual bool requestServerRunning() = 0;

    virtual bool requestWorkersRunning() = 0;

    virtual bool requestTerminate() = 0;

    virtual bool requestTerminateCompleted() = 0;

    //@}

  signals:
    
    /// emitted when the server is starting
    void serverStarting();

    /// emitted when the server has been started
    void serverStarted(const Url& url);

    /// emitted when a worker is starting
    void workerStarting();

    /// emitted when a worker has been started
    void workerStarted(const Url& url);

    /// emitted when all nodes have been started
    void allWorkersStarted();

    /// emitted when all nodes have begun to shut down
    void terminating();

    /// emitted when all nodes have been shut down
    void terminated();

  private:
  
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudProvider");
  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_CLOUDPROVIDER_IMPL_HPP
