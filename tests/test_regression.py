#! /usr/bin/env python
import os
import uuid
import sqlite3
import platform

import tables
import numpy as np
from numpy.testing import assert_array_almost_equal
from numpy.testing import assert_almost_equal
from cyclus.lib import Env

from nose.plugins.skip import SkipTest
from nose.tools import assert_equal, assert_true


import helper
from helper import check_cmd, run_cyclus, table_exist, cyclus_has_coin


ALLOW_MILPS = Env().allow_milps


def skip_if_dont_allow_milps():
    """A don't run certain tests if MILPs are disabled."""
    if not ALLOW_MILPS:
        raise SkipTest("Cyclus was compiled without MILPS support or the "
                       "ALLOW_MILPS env var was not set to true.")


class TestRegression(object):
    """A base class for all regression tests. A derived class is required for
    each new input file to be tested. Each derived class *must* declare an `inf`
    member in their `__init__` function that points to the input file to be
    tested, e.g., `self.inf_ = ./path/to/my/input_file.xml. See below for
    examples.
    """
    def __init__(self, *args, **kwargs):
        self.ext = '.sqlite'
        self.outf = str(uuid.uuid4()) + self.ext
        self.inf = None

    def setUp(self):
        if not self.inf:
            raise TypeError(("self.inf must be set in derived classes "
                             "to run regression tests."))
        run_cyclus("cyclus", os.getcwd(), self.inf, self.outf)

        # Get specific tables and columns
        if self.ext == '.h5':
            with tables.open_file(self.outf, mode="r") as f:
                # Get specific tables and columns
                self.agent_entry = f.get_node("/AgentEntry")[:]
                self.agent_exit = f.get_node("/AgentExit")[:] \
                    if "/AgentExit" in f \
                    else None
                self.enrichments = f.get_node("/Enrichments")[:] \
                    if "/Enrichments" in f \
                    else None
                self.resources = f.get_node("/Resources")[:]
                self.transactions = f.get_node("/Transactions")[:]
                self.compositions = f.get_node("/Compositions")[:]
                self.info = f.get_node("/Info")[:]
                self.rsrc_qtys = {
                    x["ResourceId"]: x["Quantity"] for x in self.resources}
        else:
            self.conn = sqlite3.connect(self.outf)
            self.conn.row_factory = sqlite3.Row
            self.cur = self.conn.cursor()
            exc = self.cur.execute
            self.agent_entry = exc('SELECT * FROM AgentEntry').fetchall()
            self.agent_exit = exc('SELECT * FROM AgentExit').fetchall() \
                if len(exc(
                    ("SELECT * FROM sqlite_master WHERE "
                     "type='table' AND name='AgentExit'")).fetchall()) > 0 \
                     else None
            self.enrichments = exc('SELECT * FROM Enrichments').fetchall() \
                if len(exc(
                    ("SELECT * FROM sqlite_master WHERE "
                     "type='table' AND name='Enrichments'")).fetchall()) > 0 \
                     else None
            self.resources = exc('SELECT * FROM Resources').fetchall()
            self.transactions = exc('SELECT * FROM Transactions').fetchall()
            self.compositions = exc('SELECT * FROM Compositions').fetchall()
            self.info = exc('SELECT * FROM Info').fetchall()
            self.rsrc_qtys = {
                x["ResourceId"]: x["Quantity"] for x in self.resources}

    def find_ids(self, spec, a, spec_col="Spec", id_col="AgentId"):
        if self.ext == '.h5':
            return helper.find_ids(spec, a[spec_col], a[id_col])
        else:
            return [x[id_col] for x in a if x[spec_col] == spec]

    def to_ary(self, a, k):
        if self.ext == '.sqlite':
            return np.array([x[k] for x in a])
        else:
            return a[k]

    def tearDown(self):
        if self.ext == '.sqlite':
            self.conn.close()
        if os.path.isfile(self.outf):
            print("removing {0}".format(self.outf))
            os.remove(self.outf)
