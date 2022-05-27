/*************************************************************************
** dvisvgm.cpp                                                          **
**                                                                      **
** This file is part of dvisvgm -- a fast DVI to SVG converter          **
** Copyright (C) 2005-2022 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#include "CommandLine.hpp"
#include "DVIToSVG.hpp"
#include "DVIToSVGActions.hpp"
#include "EPSToSVG.hpp"
#include "FileFinder.hpp"
#include "FileSystem.hpp"
#include "Font.hpp"
#include "FontEngine.hpp"
#include "Ghostscript.hpp"
#include "HashFunction.hpp"
#include "HyperlinkManager.hpp"
#include "Message.hpp"
#include "PDFToSVG.hpp"
#include "PSInterpreter.hpp"
#include "PageSize.hpp"
#include "PsSpecialHandler.hpp"
#include "SVGOutput.hpp"
#include "SignalHandler.hpp"
#include "SourceInput.hpp"
#include "System.hpp"
#include "XXHashFunction.hpp"
#include "optimizer/SVGOptimizer.hpp"
#include "utility.hpp"
#include "version.hpp"
#include <algorithm>
#include <clipper.hpp>
#include <config.h>
#include <emscripten/bind.h>
#include <fstream>
#include <iostream>
#include <potracelib.h>
#include <sstream>
#include <string_view>
#include <vector>
#include <zlib.h>

#ifndef DISABLE_WOFF
#include <brotli/encode.h>
//#include <woff2/version.h>
#include "TTFAutohint.hpp"
#include "ffwrapper.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////

static void init_fontmap() {
  bool found = false;
  for (string mapfile : {"ps2pk", "pdftex", "dvipdfm", "psfonts"}) {
    if ((found = FontMap::instance().read(mapfile + ".map")))
      break;
  }
  if (!found)
    Message::wstream(true) << "none of the default map files could be found\n";
}

static void set_variables(const CommandLine &cmdline) {
  Message::COLORIZE = cmdline.colorOpt.given();
  if (cmdline.progressOpt.given()) {
    DVIToSVG::COMPUTE_PROGRESS = true;
    SpecialActions::PROGRESSBAR_DELAY = cmdline.progressOpt.value();
  }
  Color::SUPPRESS_COLOR_NAMES = !cmdline.colornamesOpt.given();
  SVGTree::CREATE_CSS = !cmdline.noStylesOpt.given();
  SVGTree::USE_FONTS = !cmdline.noFontsOpt.given();
  if (!SVGTree::setFontFormat(cmdline.fontFormatOpt.value())) {
    string msg = "unknown font format '" + cmdline.fontFormatOpt.value() +
                 "' (supported formats: ";
    for (const string &format : FontWriter::supportedFormats())
      msg += format + ", ";
    msg.erase(msg.end() - 2, msg.end());
    msg += ")";
    throw CL::CommandLineException(msg);
  }
  SVGTree::CREATE_USE_ELEMENTS = cmdline.noFontsOpt.value() < 1;
  SVGTree::ZOOM_FACTOR = cmdline.zoomOpt.value();
  SVGTree::RELATIVE_PATH_CMDS = cmdline.relativeOpt.given();
  SVGTree::MERGE_CHARS = !cmdline.noMergeOpt.given();
  SVGTree::ADD_COMMENTS = cmdline.commentsOpt.given();
  DVIToSVG::TRACE_MODE = cmdline.traceAllOpt.given()
                             ? (cmdline.traceAllOpt.value() ? 'a' : 'm')
                             : 0;
  Message::LEVEL = cmdline.verbosityOpt.value();
  PhysicalFont::EXACT_BBOX = cmdline.exactBboxOpt.given();
  PhysicalFont::KEEP_TEMP_FILES = cmdline.keepOpt.given();
  PhysicalFont::METAFONT_MAG = max(1.0, cmdline.magOpt.value());
  XMLString::DECIMAL_PLACES = max(0, min(6, cmdline.precisionOpt.value()));
  XMLNode::KEEP_ENCODED_FILES = cmdline.keepOpt.given();
  PsSpecialHandler::COMPUTE_CLIPPATHS_INTERSECTIONS =
      cmdline.clipjoinOpt.given();
  PsSpecialHandler::SHADING_SEGMENT_OVERLAP = cmdline.gradOverlapOpt.given();
  PsSpecialHandler::SHADING_SEGMENT_SIZE =
      max(1, cmdline.gradSegmentsOpt.value());
  PsSpecialHandler::SHADING_SIMPLIFY_DELTA = cmdline.gradSimplifyOpt.value();
  PsSpecialHandler::BITMAP_FORMAT =
      util::tolower(cmdline.bitmapFormatOpt.value());
  if (!PSInterpreter::imageDeviceKnown(PsSpecialHandler::BITMAP_FORMAT)) {
    ostringstream oss;
    oss << "unknown image format '" << PsSpecialHandler::BITMAP_FORMAT
        << "'\nknown formats:\n";
    PSInterpreter::listImageDeviceInfos(oss);
    throw CL::CommandLineException(oss.str());
  }
  if (cmdline.optimizeOpt.given()) {
    SVGOptimizer::MODULE_SEQUENCE = cmdline.optimizeOpt.value();
    vector<string> modnames;
    if (!SVGOptimizer().checkModuleString(SVGOptimizer::MODULE_SEQUENCE,
                                          modnames)) {
      string msg = "invalid optimizer module";
      if (modnames.size() > 1)
        msg += 's';
      msg += ": ";
      for (const string &modname : modnames)
        msg += modname + ", ";
      msg.erase(msg.end() - 2);
      throw CL::CommandLineException(msg);
    }
  }
}

static void timer_message(double start_time, const pair<int, int> *pageinfo) {
  Message::mstream().indent(0);
  if (!pageinfo)
    Message::mstream(false, Message::MC_PAGE_NUMBER) << "\n"
                                                     << "file";
  else {
    Message::mstream(false, Message::MC_PAGE_NUMBER)
        << "\n"
        << pageinfo->first << " of " << pageinfo->second << " page";
    if (pageinfo->second > 1)
      Message::mstream(false, Message::MC_PAGE_NUMBER) << 's';
  }
  Message::mstream(false, Message::MC_PAGE_NUMBER)
      << " converted in " << (System::time() - start_time) << " seconds\n";
}

void init() {
  FileFinder::init("/usr/bin/dvisvgm", "dvisvgm", false);

  PhysicalFont::CACHE_PATH = "~/.cache/dvisvgm";
  if (!FileSystem::exists(PhysicalFont::CACHE_PATH)) {
    FileSystem::mkdir(PhysicalFont::CACHE_PATH);
  }

  FileSystem::TMPDIR = "/tmp";
  if (!FileSystem::exists(FileSystem::TMPDIR)) {
    FileSystem::mkdir(FileSystem::TMPDIR);
  }

  HyperlinkManager::setLinkMarker("box");

  CommandLine cmdline;
  set_variables(cmdline);
  init_fontmap();

  SignalHandler::instance().start();
}
 
std::string toSVG(std::string input) {
  try { 
    TemporaryFile tmpfile_in;
    tmpfile_in.create();
    tmpfile_in.write(input.data(), input.size());

    SourceInput srcin(tmpfile_in.path());

    double start_time = System::time();

    SVGOutput out;
    DVIToSVG dvi2svg(srcin.getInputStream(), out);
    dvi2svg.setProcessSpecials(nullptr, true);
    dvi2svg.setPageSize("papersize");

    pair<int, int> pageinfo;
    dvi2svg.convert("1", &pageinfo);
    timer_message(start_time, &pageinfo);

    return out.toString();
  } catch (DVIException &e) {
    Message::estream() << "\nDVI error: " << e.what() << '\n';
  } catch (PSException &e) {
    Message::estream() << "\nPostScript error: " << e.what() << '\n';
  } catch (SignalException &e) {
    Message::wstream().clearline();
    Message::wstream(true) << "execution interrupted by user\n";
  } catch (exception &e) {
    Message::estream(true) << e.what() << '\n';
  }
}

EMSCRIPTEN_BINDINGS(dvisvgm) {
  emscripten::function("toSVG", &toSVG);
  emscripten::function("init", &init);
}