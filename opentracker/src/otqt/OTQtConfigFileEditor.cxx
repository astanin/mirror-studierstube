/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/**
 * @file   OTQtConfigFileEditor.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c OTQtConfigFileEditor
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/OTQtConfigFileEditor.h>
#include <OpenTracker/otqt/QtMouseEventCalibModule.h>
#include <stdexcept>

namespace ot {

//--------------------------------------------------------------------------------
OTQtConfigFileEditor::OTQtConfigFileEditor(std::string & input_filename)
  : is_initialized_(false),
    input_filename_(input_filename)
{
}

//--------------------------------------------------------------------------------
OTQtConfigFileEditor::~OTQtConfigFileEditor()
{
  lines_.clear();
}


//--------------------------------------------------------------------------------
OTQtConfigFileEditor::OTQtConfigFileEditor(OTQtConfigFileEditor const & ref)
  : is_initialized_(ref.is_initialized_),
    input_filename_(ref.input_filename_),
    lines_(ref.lines_)
{
}

//--------------------------------------------------------------------------------
void OTQtConfigFileEditor::init() {
  // import input config file
  import();
  is_initialized_ = true;
}

//--------------------------------------------------------------------------------
void OTQtConfigFileEditor::import() {
  // open input file for reading
  FILE * fp = fopen(input_filename_.c_str(), "r");
  if (fp == NULL) {
    std::string err_msg = "Could not read/import config inputfile '";
    err_msg += input_filename_;
    err_msg += "'.";
    throw std::invalid_argument(err_msg);
  }
  // read file line per line
  char c;
  std::string one_line;
  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n' || c == '\r') {
      lines_.push_back(one_line);
      one_line.clear();
      continue;
    }
    one_line += c;
  }
  lines_.push_back(one_line);
  fclose(fp);
}

//--------------------------------------------------------------------------------
int OTQtConfigFileEditor::countOfExpr(std::string expr) const {
  int count = 0;
  for (ConstLineIterator iter = lines_.begin(); iter != lines_.end(); iter++) {
    if (iter->find(expr) != std::string::npos)
      count++;
  }
  return count;
}

//--------------------------------------------------------------------------------
int OTQtConfigFileEditor::firstLineNoOfExpr(std::string expr, int start_index) const {
  int line_index = -1;
  for (ConstLineIterator iter = lines_.begin(); iter != lines_.end(); iter++) {
    line_index++;
    if (line_index < start_index)
      continue;
    if (iter->find(expr) != std::string::npos)
      return line_index;
  }
  return -1;
}

//--------------------------------------------------------------------------------
bool OTQtConfigFileEditor::containsExpr(std::string expr) const {
  return (firstLineNoOfExpr(expr) >= 0);
}

//--------------------------------------------------------------------------------
std::string const & OTQtConfigFileEditor::getLine(int index) const {
  if (index < 0 || index >= getLineCount()) {
    throw std::invalid_argument("OTQtConfigFileEditor::getLine():: Invalid line index.");
  }
  ConstLineIterator it = lines_.begin();
  int count = 0;
  while (it != lines_.end()) {
    if (count == index) {
      break;
    }
    count++; it++;
  }
  return (*it);
}

//--------------------------------------------------------------------------------
bool OTQtConfigFileEditor::killLine(int index)
{
  LineIterator it = lines_.begin();
  int count = 0;
  while (it != lines_.end()) {
    if (count == index) {
      lines_.erase(it);
      return true;
    }
    count++; it++;
  }
  return false;
}

//--------------------------------------------------------------------------------
bool OTQtConfigFileEditor::insertLines(int index_at, LineList & ins_lines, int num_indent_spaces)
{
  if (num_indent_spaces > 0) {
    char const SPACE = ' ';
#ifdef WIN32
    char * indent = new char[num_indent_spaces + 1];
#else
	char indent[num_indent_spaces + 1];
#endif
    for (int i = 0; i < num_indent_spaces; i++) {
      indent[i] = SPACE;
    }
    indent[num_indent_spaces] = '\0';
    // add indent spaces
    LineIterator it = ins_lines.begin();
    while (it != ins_lines.end()) {
      (*it) = std::string(indent) + (*it);
      it++;
    }
#ifdef WIN32
    delete indent;
#endif
  }
  // insert
  LineIterator it = lines_.begin();
  int count = 0;
  while (it != lines_.end()) {
    if (index_at == count) {
      lines_.insert(it, ins_lines.begin(), ins_lines.end());
      return true;
    }
    count++; it++;
  }
  return false;
}

//--------------------------------------------------------------------------------
void OTQtConfigFileEditor::writeToStdOut() const
{
  fprintf(stdout, "----- OT configuration START -----\n");
  ConstLineIterator it = lines_.begin();
  while (it != lines_.end()) {
    fprintf(stdout, "%s\n", (*it).c_str());
    it++;
  }
  fprintf(stdout, "----- OT configuration EOF -----\n");
  fflush(stdout);
}

//--------------------------------------------------------------------------------
void OTQtConfigFileEditor::writeToFile(std::string const & output_filename) const
{
  // open file for writing
  FILE * fp = fopen(output_filename.c_str(), "w");
  if (fp == NULL) {
    std::string err_msg = "Could not write config outputfile '";
    err_msg += output_filename;
    err_msg += "'.";
    throw std::invalid_argument(err_msg);
  }
  // write output file line per line
  ConstLineIterator it = lines_.begin();
  while (it != lines_.end()) {
    fprintf(fp, "%s\n", (*it).c_str());
    it++;
  }
  fflush(fp);
  fclose(fp);
}

} // namespace ot

#endif // USE_OTQT

/* 
 * ------------------------------------------------------------
 *   End of OTQtConfigFileEditor.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
