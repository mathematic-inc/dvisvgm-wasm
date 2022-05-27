/*************************************************************************
** SVGOutput.hpp                                                        **
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

#ifndef SVGOUTPUT_HPP
#define SVGOUTPUT_HPP

#include "FilePath.hpp"
#include <memory>
#include <ostream>
#include <string>
#include <sstream>

struct SVGOutputBase {
  class HashTriple {
  public:
    HashTriple() = default;
    HashTriple(std::string dviHash, std::string optHash, std::string cmbHash)
        : _dviHash(std::move(dviHash)), _optHash(std::move(optHash)),
          _cmbHash(std::move(cmbHash)) {}
    std::string dviHash() const { return _dviHash; }
    std::string optHash() const { return _optHash; }
    std::string cmbHash() const { return _cmbHash; }
    bool empty() const { return _dviHash.empty(); }

  private:
    std::string _dviHash; ///< hash of plain DVI data
    std::string _optHash; ///< hash of options affecting the SVG document
    std::string _cmbHash; ///< combined hash of DVI data and options
  };

  virtual ~SVGOutputBase() = default;
  virtual std::ostream &
  getPageStream(int page, int numPages,
                const HashTriple &hashes = HashTriple()) const = 0;
  virtual FilePath filepath(int page, int numPages,
                            const HashTriple &hashes = HashTriple()) const = 0;
  virtual bool ignoresHashes() const { return true; }
};

class SVGOutput : public SVGOutputBase {
public:
  SVGOutput() = default;
  std::ostream &
  getPageStream(int page, int numPages,
                const HashTriple &hash = HashTriple()) const override {
    return stream_;
  }
  FilePath filepath(int page, int numPages,
                    const HashTriple &hash = HashTriple()) const override {
    FilePath outpath;
    return outpath;
  }
  std::string toString() const { return stream_.str(); }

private:
  mutable std::stringstream stream_;
};

#endif
