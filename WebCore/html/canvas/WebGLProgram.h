/*
 * Copyright (C) 2009-2017 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#if ENABLE(WEBGL)

#include "WebGLSharedObject.h"
#include <wtf/Forward.h>

namespace WebCore {

class WebGLShader;

class WebGLProgram final : public WebGLSharedObject {
public:
    static Ref<WebGLProgram> create(WebGLRenderingContextBase&);
    virtual ~WebGLProgram();

    static HashMap<WebGLProgram*, WebGLRenderingContextBase*>& instances(const LockHolder&);
    static Lock& instancesMutex();

    unsigned numActiveAttribLocations();
    GC3Dint getActiveAttribLocation(GC3Duint index);

    bool isUsingVertexAttrib0();

    bool getLinkStatus();
    void setLinkStatus(bool);

    unsigned getLinkCount() const { return m_linkCount; }

    // This is to be called everytime after the program is successfully linked.
    // We don't deal with integer overflow here, assuming in reality a program
    // will never be linked so many times.
    // Also, we invalidate the cached program info.
    void increaseLinkCount();

    WebGLShader* getAttachedShader(GC3Denum);
    bool attachShader(WebGLShader*);
    bool detachShader(WebGLShader*);

protected:
    WebGLProgram(WebGLRenderingContextBase&);

    void deleteObjectImpl(GraphicsContext3D*, Platform3DObject) override;

private:
    void cacheActiveAttribLocations(GraphicsContext3D*);
    void cacheInfoIfNeeded();

    Vector<GC3Dint> m_activeAttribLocations;

    GC3Dint m_linkStatus { 0 };

    // This is used to track whether a WebGLUniformLocation belongs to this program or not.
    unsigned m_linkCount { 0 };

    RefPtr<WebGLShader> m_vertexShader;
    RefPtr<WebGLShader> m_fragmentShader;

    bool m_infoValid { true };
};

} // namespace WebCore

#endif
