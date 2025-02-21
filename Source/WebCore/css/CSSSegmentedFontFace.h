/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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

#ifndef CSSSegmentedFontFace_h
#define CSSSegmentedFontFace_h

#include "FontCache.h"
#include "FontRanges.h"
#include <wtf/HashMap.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>

namespace WebCore {

class CSSFontFace;
class CSSFontSelector;
class FontDescription;

class CSSSegmentedFontFace : public RefCounted<CSSSegmentedFontFace> {
public:
    static Ref<CSSSegmentedFontFace> create(CSSFontSelector* selector) { return adoptRef(*new CSSSegmentedFontFace(selector)); }
    ~CSSSegmentedFontFace();

    CSSFontSelector* fontSelector() const { return m_fontSelector; }

    void fontLoaded(CSSFontFace*);

    void appendFontFace(Ref<CSSFontFace>&&);

    FontRanges fontRanges(const FontDescription&);

#if ENABLE(FONT_LOAD_EVENTS)
    class LoadFontCallback : public RefCounted<LoadFontCallback> {
    public:
        virtual ~LoadFontCallback() { }
        virtual void notifyLoaded() = 0;
        virtual void notifyError() = 0;
    };

    bool checkFont() const;
    void loadFont(const FontDescription&, PassRefPtr<LoadFontCallback> loadCallback);
#endif

private:
    CSSSegmentedFontFace(CSSFontSelector*);

    void pruneTable();
#if ENABLE(FONT_LOAD_EVENTS)
    bool isLoading() const;
#endif

    CSSFontSelector* m_fontSelector;
    HashMap<FontDescriptionKey, FontRanges, FontDescriptionKeyHash, WTF::SimpleClassHashTraits<FontDescriptionKey>> m_descriptionToRangesMap;
    Vector<Ref<CSSFontFace>, 1> m_fontFaces;
#if ENABLE(FONT_LOAD_EVENTS)
    Vector<RefPtr<LoadFontCallback>> m_callbacks;
#endif
};

} // namespace WebCore

#endif // CSSSegmentedFontFace_h
