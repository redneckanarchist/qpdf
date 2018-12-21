// Copyright (c) 2005-2018 Jay Berkenbilt
//
// This file is part of qpdf.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Versions of qpdf prior to version 7 were released under the terms
// of version 2.0 of the Artistic License. At your option, you may
// continue to consider qpdf to be licensed under those terms. Please
// see the manual for additional information.

#ifndef QPDFPAGEOBJECTHELPER_HH
#define QPDFPAGEOBJECTHELPER_HH

#include <qpdf/QPDFObjectHelper.hh>
#include <qpdf/QPDFAnnotationObjectHelper.hh>

#include <qpdf/DLL.h>

#include <qpdf/QPDFObjectHandle.hh>

class QPDFPageObjectHelper: public QPDFObjectHelper
{
  public:
    QPDF_DLL
    QPDFPageObjectHelper(QPDFObjectHandle);
    QPDF_DLL
    virtual ~QPDFPageObjectHelper()
    {
    }

    // Returns an empty map if there are no images or no resources.
    // This function does not presently support inherited resources.
    // If this is a significant concern, call
    // pushInheritedAttributesToPage() on the QPDF object that owns
    // this page. See comment in the source for details. Return value
    // is a map from XObject name to the image object, which is always
    // a stream.
    QPDF_DLL
    std::map<std::string, QPDFObjectHandle> getPageImages();

    // Return the annotations in the page's "/Annots" list, if any. If
    // only_subtype is non-empty, only include annotations of the
    // given subtype.
    QPDF_DLL
    std::vector<QPDFAnnotationObjectHelper> getAnnotations(
        std::string const& only_subtype = "");

    // Returns a vector of stream objects representing the content
    // streams for the given page.  This routine allows the caller to
    // not care whether there are one or more than one content streams
    // for a page.
    QPDF_DLL
    std::vector<QPDFObjectHandle> getPageContents();

    // Add the given object as a new content stream for this page. If
    // parameter 'first' is true, add to the beginning. Otherwise, add
    // to the end. This routine automatically converts the page
    // contents to an array if it is a scalar, allowing the caller not
    // to care what the initial structure is. You can call
    // coalesceContentStreams() afterwards if you want to force it to
    // be a single stream.
    QPDF_DLL
    void addPageContents(QPDFObjectHandle contents, bool first);

    // Rotate a page. If relative is false, set the rotation of the
    // page to angle. Otherwise, add angle to the rotation of the
    // page. Angle must be a multiple of 90. Adding 90 to the rotation
    // rotates clockwise by 90 degrees.
    QPDF_DLL
    void rotatePage(int angle, bool relative);

    // Coalesce a page's content streams. A page's content may be a
    // stream or an array of streams. If this page's content is an
    // array, concatenate the streams into a single stream. This can
    // be useful when working with files that split content streams in
    // arbitrary spots, such as in the middle of a token, as that can
    // confuse some software. You could also call this after calling
    // addPageContents.
    QPDF_DLL
    void coalesceContentStreams();

    //
    // Content stream handling
    //

    // Parse a page's contents through ParserCallbacks, described
    // above. This method works whether the contents are a single
    // stream or an array of streams. Call on a page object.
    QPDF_DLL
    void parsePageContents(QPDFObjectHandle::ParserCallbacks* callbacks);

    // Pass a page's contents through the given TokenFilter. If a
    // pipeline is also provided, it will be the target of the write
    // methods from the token filter. If a pipeline is not specified,
    // any output generated by the token filter will be discarded. Use
    // this interface if you need to pass a page's contents through
    // filter for work purposes without having that filter
    // automatically applied to the page's contents, as happens with
    // addContentTokenFilter. See examples/pdf-count-strings.cc for an
    // example.
    QPDF_DLL
    void filterPageContents(QPDFObjectHandle::TokenFilter* filter,
                            Pipeline* next = 0);

    // Pipe a page's contents through the given pipeline. This method
    // works whether the contents are a single stream or an array of
    // streams. Call on a page object.
    QPDF_DLL
    void pipePageContents(Pipeline* p);

    // Attach a token filter to a page's contents. If the page's
    // contents is an array of streams, it is automatically coalesced.
    // The token filter is applied to the page's contents as a single
    // stream.
    QPDF_DLL
    void addContentTokenFilter(
        PointerHolder<QPDFObjectHandle::TokenFilter> token_filter);

    // A page's resources dictionary maps names to objects elsewhere
    // in the file. This method walks through a page's contents and
    // keeps tracks of which resources are referenced somewhere in the
    // contents. Then it removes from the resources dictionary any
    // object that is not referenced in the contents. This operation
    // is most useful after calling
    // QPDFPageDocumentHelper::pushInheritedAttributesToPage(). This
    // method is used by page splitting code to avoid copying unused
    // objects in files that used shared resource dictionaries across
    // multiple pages.
    QPDF_DLL
    void removeUnreferencedResources();

  private:
    class Members
    {
        friend class QPDFPageObjectHelper;

      public:
        QPDF_DLL
        ~Members();

      private:
        Members();
        Members(Members const&);
    };

    PointerHolder<Members> m;
};

#endif // QPDFPAGEOBJECTHELPER_HH
