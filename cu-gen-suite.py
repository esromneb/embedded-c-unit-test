#!/usr/bin/env python
##
# CU - C unit testing framework
# ---------------------------------
# Copyright (c)2010 Daniel Fiser <danfis@danfis.cz>
#
#
#  This file is part of CU.
#
#  CU is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as
#  published by the Free Software Foundation; either version 3 of
#  the License, or (at your option) any later version.
#
#  CU is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import sys
import re
import subprocess
import shlex
from getopt import gnu_getopt, GetoptError

patterns = {
    'TEST' : re.compile(r'TEST\(([a-zA-Z0-9_]+)\)'),
    'func' : re.compile(r'void ([a-zA-Z0-9_]+)\(void\)'),
}

class Options(object):
    def __init__(self):
        self.help            = False
        self.filenames       = []
        self.test_suite_name = None
        self.cflags          = ''
        self.cc              = 'gcc'

    def init(self):
        optlist, args = gnu_getopt(sys.argv[1:],
                                   'hs:c:',
                                   ['help', 'suite-name=', 'cflags=',
                                    'cc='])
        for opt in optlist:
            if opt[0] == '--help' or opt[0] == '-h':
                self.help = True
            elif opt[0] == '--suite-name' or opt[0] == '-s':
                self.test_suite_name = opt[1]
            elif opt[0] == '--cflags' or opt[0] == '-c':
                self.cflags = opt[1]
            elif opt[0] == '--cc':
                self.cc = opt[1]

        if len(args) > 0:
            self.filenames = args

options = Options()


class TestFile(object):
    def __init__(self, fn):
        self.fn = fn
        self.tests = []

    def gatherTests(self):
        """ Gathers all tests from file.
            It is done it two steps. In first step are gathered all
            functions encapsulated in TEST() macro. In second step, file
            is parsed using gcc's preprocessor and from list of functions
            got in previous step are kept only those that still remains in
            source code.
        """

        try:
            fin = open(self.fn, 'r')
        except:
            print >>sys.stderr, "Error: Can't open file '{0}'".format(self.fn)
            return

        # Step 1: store TEST() functions into tests
        tests = []
        for line in fin:
            match = patterns['TEST'].match(line)
            if match:
                tests.append(match.group(1))
        fin.close()

        # Step 2.1: store 'void fn(void)' functions into tests2
        tests2 = []
        cmd = '{0} {1} -E -o - {2}'.format(options.cc, options.cflags, self.fn)
        #print cmd
        fin = subprocess.Popen(shlex.split(cmd), stdout = subprocess.PIPE)
        for line in fin.stdout:
            match = patterns['func'].match(line)
            if match:
                tests2.append(match.group(1))
        fin.stdout.close()

        # Step 2.2: remove from tests all functions that are not in
        # tests2. Order of functions in tests must be preserved.
        self.tests = filter(lambda x: x in tests2, tests)


    def composeTestAddPart(self):
        s = '    /* From {0}: */\n'.format(self.fn)
        for t in self.tests:
            s += "    TEST_ADD({0}),\n".format(t)
        return s

    def composeDeclarationPart(self):
        s = '/* From {0}: */\n'.format(self.fn)
        for t in self.tests:
            s += 'TEST({0});\n'.format(t)
        return s

    def writeTestSuite(self):
        """ Writes whole test suite structure to separate .h file based on
            test suite name. Test suite name is based on .c file name.
        """
        dir, fn = os.path.split(os.path.abspath(self.fn))
        name = ''
        if fn[-2:] == '.c':
            name = fn[:-2]
            fn = fn[:-1] + 'h'
        else:
            name = fn.replace('.', '_')
            name = name.replace('-', '_')
            fn = fn + '.h'
        absfn = os.path.join(dir, fn)

        writeTestSuite(absfn, name, [self])


def writeTestSuite(fn, name, test_files):
    try:
        fout = open(fn, 'w')
    except:
        print >>sys.stderr, "Error: Can't open file '{0}'.".format(fn)
        return

    print >>sys.stderr, "Writing test suite '{0}' into file '{1}'...".format(name, fn)

    print >>fout, "#include <cu/cu.h>"
    print >>fout, ""

    for t in test_files:
        print >>fout, t.composeDeclarationPart()
    print >>fout, ""

    print >>fout, "TEST_SUITE({0}) {{".format(name)

    for t in test_files:
        print >>fout, t.composeTestAddPart()

    print >>fout, "    TEST_SUITE_CLOSURE"
    print >>fout, "};"
    print >>fout, ""
    fout.close()

def usage():
    print >>sys.stderr, "Usage: {0} file1 [,file2[,file3[,...]]]".format(sys.argv[0])
    print >>sys.stderr, ""
    print >>sys.stderr, "   -h / --help        Prints this help"
    print >>sys.stderr, "   -s / --suite-name  Specifies name of one test suite where should be gathered all tests from all files."
    print >>sys.stderr, "   -c / --cflags      Sets additional CFLAGS for compiler"
    print >>sys.stderr, "   --cc               Sets compiler binary"
    print >>sys.stderr, ""
    sys.exit(-1)


def main(options):
    if options.help:
        usage()

    # create TestFile instance for each filename
    test_files = map(lambda x: TestFile(x), options.filenames)

    # gather tests in each TestFile
    for tf in test_files:
        tf.gatherTests()

    if not options.test_suite_name:
        for tf in test_files:
            tf.writeTestSuite()
    else:
        fn = options.test_suite_name + '.h'
        writeTestSuite(fn, options.test_suite_name, test_files)

if __name__ == '__main__':
    options.init()
    main(options)
    sys.exit(0)
