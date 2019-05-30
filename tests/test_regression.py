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

class _Recycle(TestRegression):
    """This class tests the input/recycle.xml file.
    """
    def __init__(self, *args, **kwargs):
        super(_Recycle, self).__init__(*args, **kwargs)

        # this test requires separations which isn't supported by hdf5
        # so we force sqlite:
        base, _ = os.path.splitext(self.outf)
        self.ext = '.sqlite'
        self.outf = base + self.ext
        self.sql = """
            SELECT t.time as time,SUM(c.massfrac*r.quantity) as qty FROM transactions as t
            JOIN resources as r ON t.resourceid=r.resourceid AND r.simid=t.simid
            JOIN agententry as send ON t.senderid=send.agentid AND send.simid=t.simid
            JOIN agententry as recv ON t.receiverid=recv.agentid AND recv.simid=t.simid
            JOIN compositions as c ON c.qualid=r.qualid AND c.simid=r.simid
            WHERE send.prototype=? AND recv.prototype=? AND c.nucid=?
            GROUP BY t.time;"""

    def do_compare(self, fromfac, tofac, nuclide, exp_invs):
        conn = sqlite3.connect(self.outf)
        c = conn.cursor()
        eps = 1e-10
        simdur = len(exp_invs)

        invs = [0.0] * simdur
        for i, row in enumerate(c.execute(self.sql, (fromfac, tofac, nuclide))):
            t = row[0]
            invs[t] = row[1]

        expfname = 'exp_recycle_{0}-{1}-{2}.dat'.format(fromfac, tofac, nuclide)
        with open(expfname, 'w') as f:
            for t, val in enumerate(exp_invs):
                f.write('{0} {1}\n'.format(t, val))
        obsfname = 'obs_recycle_{0}-{1}-{2}.dat'.format(fromfac, tofac, nuclide)
        with open(obsfname, 'w') as f:
            for t, val in enumerate(invs):
                f.write('{0} {1}\n'.format(t, val))

        i = 0
        for exp, obs in zip(invs, exp_invs):
            assert_almost_equal(
                exp, obs, err_msg='mismatch at t={}, {} != {}'.format(i, exp, obs))
            i += 1

        os.remove(expfname)
        os.remove(obsfname)

    def test_pu239_sep_repo(self):
        simdur = 600
        exp = [0.0] * simdur
        exp[18] = 1.70022267
        exp[37] = 1.70022267
        exp[56] = 1.70022267
        exp[75] = 1.70022267
        exp[94] = 1.70022267
        exp[113] = 1.70022267
        exp[132] = 1.70022267
        exp[151] = 1.70022267
        exp[170] = 1.70022267
        exp[189] = 1.70022267
        exp[208] = 1.70022267
        exp[246] = 1.70022267
        exp[265] = 1.70022267
        exp[284] = 1.70022267
        exp[303] = 1.70022267
        exp[322] = 1.70022267
        exp[341] = 1.70022267
        exp[360] = 1.70022267
        exp[379] = 1.70022267
        exp[417] = 1.70022267
        exp[436] = 1.70022267
        exp[455] = 1.70022267
        exp[474] = 1.70022267
        exp[493] = 1.70022267
        exp[512] = 1.70022267
        exp[531] = 1.70022267
        exp[569] = 1.70022267
        exp[588] = 1.70022267

        self.do_compare('separations', 'repo', 942390000, exp)

    def test_pu239_reactor_repo(self):
        simdur = 600
        exp = [0.0] * simdur
        exp[226] = 420.42772559790944
        exp[397] = 420.42772559790944
        exp[549] = 420.42772559790944
        self.do_compare('reactor', 'repo', 942390000, exp)
