/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(SERVICE_WORKER)

#include "SWClientConnection.h"

namespace WebCore {

class WorkerGlobalScope;
class WorkerThread;

class WorkerSWClientConnection final : public SWClientConnection {
public:
    static Ref<WorkerSWClientConnection> create(WorkerGlobalScope& scope) { return adoptRef(*new WorkerSWClientConnection { scope }); }
    ~WorkerSWClientConnection();

private:
    explicit WorkerSWClientConnection(WorkerGlobalScope&);

    void matchRegistration(SecurityOriginData&& topOrigin, const URL& clientURL, RegistrationCallback&&) final;
    void getRegistrations(SecurityOriginData&& topOrigin, const URL& clientURL, GetRegistrationsCallback&&) final;
    void whenRegistrationReady(const SecurityOriginData& topOrigin, const URL& clientURL, WhenRegistrationReadyCallback&&) final;
    void addServiceWorkerRegistrationInServer(ServiceWorkerRegistrationIdentifier) final;
    void removeServiceWorkerRegistrationInServer(ServiceWorkerRegistrationIdentifier) final;
    void didResolveRegistrationPromise(const ServiceWorkerRegistrationKey&) final;
    void postMessageToServiceWorker(ServiceWorkerIdentifier destination, MessageWithMessagePorts&&, const ServiceWorkerOrClientIdentifier& source) final;
    SWServerConnectionIdentifier serverConnectionIdentifier() const final;
    bool mayHaveServiceWorkerRegisteredForOrigin(const SecurityOriginData&) const final;
    void syncTerminateWorker(ServiceWorkerIdentifier) final;
    void registerServiceWorkerClient(const SecurityOrigin& topOrigin, const ServiceWorkerClientData&, const Optional<ServiceWorkerRegistrationIdentifier>&, const String& userAgent) final;
    void unregisterServiceWorkerClient(DocumentIdentifier) final;
    void finishFetchingScriptInServer(const ServiceWorkerFetchResult&) final;
    bool isThrottleable() const final;
    void updateThrottleState() final;
    void scheduleJobInServer(const ServiceWorkerJobData&) final;
    void scheduleJob(DocumentOrWorkerIdentifier, const ServiceWorkerJobData&) final;

    PAL::SessionID m_sessionID;
    Ref<WorkerThread> m_thread;

    uint64_t m_lastRequestIdentifier { 0 };
    HashMap<uint64_t, RegistrationCallback> m_matchRegistrationRequests;
    HashMap<uint64_t, GetRegistrationsCallback> m_getRegistrationsRequests;
    HashMap<uint64_t, WhenRegistrationReadyCallback> m_whenRegistrationReadyRequests;
};

} // namespace WebCore

#endif // ENABLE(SERVICE_WORKER)
