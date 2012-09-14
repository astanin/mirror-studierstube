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
 * @file   OTQtConfigFileEditor.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c OTQtConfigFileEditor
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#ifndef OTQT_OTQTCONFIGFILEEDITOR_H
#define OTQT_OTQTCONFIGFILEEDITOR_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <list>
#include <string>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class OTQtConfigFileEditor
 * @ingroup group_otqt_classes
 * @brief Simple line-based file editor
 *
 * Provides some methods to query, manipulate and output an imported file on
 * line-basis. The existance and count of string expressions can be queried with methods
 * such as countOfExpr(). Lines of text can be killed with the killLine() method or
 * inserted with insertLines() method at specified line index.
 *
 * An object is first created specifying a file name in the constructor. Next, the object
 * must be initialized calling the init() method. Within this method the refered file
 * contents are imported to an interal list of lines. This list of lines can then be
 * edited with the methods mentioned above and finally printed to standard output stream
 * or another destination file location with methods such as writeToFile().
 */
class OPENTRACKER_API OTQtConfigFileEditor {

public:
  /// configuration file line container
  typedef std::list<std::string> LineList;
  /// iterator on file line container
  typedef LineList::iterator LineIterator;
  /// constant iterator on file line container
  typedef LineList::const_iterator ConstLineIterator;

public:
  /**
   * Stores the given input file name. Returns object in state @e not @e initialized (file
   * contexts are not imported).
   *
   * @param input_filename input XML configuration file name
   */
  OTQtConfigFileEditor(std::string & input_filename);
  /**
   * Clears the file line container datastructure.
   */
  ~OTQtConfigFileEditor();
  /**
   * Returns a copy of the given object.
   * @param ref copy object
   */
  OTQtConfigFileEditor(OTQtConfigFileEditor const & ref);

private:
  OTQtConfigFileEditor();
  OTQtConfigFileEditor operator = (OTQtConfigFileEditor const & ref);

public:
  /**
   * Initializes the object. Imports the file contents refered to by the previously set
   * filename. Switches state to @e initialized. Throws an exception if import fails (eg
   * file does not exist, is not readable etc).
   */
  void init();
  /**
   * Returns true if input file has been sucessfully imported (equals object which is in
   * @e initialized state).
   * @return true if input file successfully imported
   */
  inline bool isInitialized() const { return is_initialized_; }
  /**
   * Returns true if given expression is contained in the imported file.
   * @param expr arbitrary string expressing
   * @return true if expression is contained in imported file
   */
  bool containsExpr(std::string expr) const;
  /**
   * Returns the line number of the first occurrence of given expression within imported
   * file. Starts search at given line number.
   * @param expr arbitrary string expression
   * @param start_index search start index: 0 <= index <= number of lines - 1
   * @return line number of first occurrence
   */
  int firstLineNoOfExpr(std::string expr, int start_index = 0) const;
  /**
   * Returns number of lines containing the given expression within the imported file.
   * @param expr arbitrary string expression
   * @return number of lines containing the given expression
   */
  int countOfExpr(std::string expr) const;
  /**
   * Inserts given lines of text into the internal list of lines, starting before (!)
   * given line index (eg index equal zero, then the lines are inserted before the first
   * line). The optional indent argument (default value equal zero) defines the number of
   * additional spaces prepended each inserted line. Returns true if operation was
   * successful. Otherwise returns false (eg if given index is out of bounds).
   *
   * @param index_at insert start line index:  0 <= index <= number of lines - 1
   * @param ins_lines lines to insert
   * @param num_indent_spaces optional number of additional spaces prepended to each line
   *
   * @return true if insert opertion successful
   */
  bool insertLines(int index_at, LineList & ins_lines, int num_indent_spaces = 0);
  /**
   * Kills the line at given index from the interal list of lines. Returns true if
   * operation was successful, otherwise returns false (eg if given index is out of
   * bounds).
   *
   * @param index line index: 0 <= index <= number of lines - 1
   * @return true if kill opertion successful
   */
  bool killLine(int index);
  /**
   * Writes the current list of lines contents to standard output.
   */
  void writeToStdOut() const;
  /**
   * Writes the current list of lines contents to a file specifed by the given
   * name. Throws an exception in case of failur (file cannot be opened for writing etc).
   */
  void writeToFile(std::string const & output_filename) const;


  /**
   * Returns the current number of lines stored in the internal list of lines.
   * @return number of text lines
   */
  inline int getLineCount() const { return lines_.size(); };
  /**
   * Returns the text line stored at the given index in the interal list of lines.
   * @param index line index: 0 <= index <= number of lines - 1
   * @return text line
   */
  std::string const & getLine(int index) const;


private:
  /**
   * Imports file refered to by previously set filename. Reads line after line storing
   * each line in a dedicated line container datastructure. Throws an exception on failure
   * (file does not exist, is not readable, etc).
   */
  void import();

private:
  /// object initialized state (true if file was imported successfully)
  bool is_initialized_;
  /// input file name
  std::string input_filename_;
  /// file line container
  LineList lines_;

};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_OTQTCONFIGFILEEDITOR_H


/* 
 * ------------------------------------------------------------
 *   End of OTQtConfigFileEditor.h
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
