###################################################
Recycle
###################################################

Additional archetypes for the Cyclus nuclear fuel cycle simulator from the
University of Wisconsin - Madison are intended to support innovative
fuel cycle simulations with Cyclus. The archetypes in this library are 
more advanced than those present in the `Cycamore Library <https://github.com/cyclus/cycamore>`_.

This README is intended primarily for those who intend to contribute to the
development of Recycle archetypes. If you are interested in Cyclus as a user
or in developing your own Cyclus archetypes, you may want to consult `Getting
Started with Cyclus <http://fuelcycle.org/user/install.html>`_.

This README provides basic information about:
 - the dependency required by Recycle
 - installation of Recycle from the command line
 - how to run Recycle unit tests

- **For general information about Cyclus, visit the**  `Cyclus Homepage`_,

- **For detailed installation instructions, visit the**
  `INSTALLATION Guide <INSTALL.rst>`_,

- **To see user and developer documentation for this code, please visit
  the** `Users Guide <http://fuelcycle.org/user/index.html>`_,

- **If you would like to contribute to Recycle, please check our**
  `Contribution Guidelines <https://github.com/cyclus/cyclus/blob/master/CONTRIBUTING.rst>`_.


.. contents:: Table of Contents


************
Dependencies
************

Recycle's only dependency is the Cyclus Core.

====================   ==================
Package                Minimum Version
====================   ==================
`Cyclus`               1.6
====================   ==================

There are a number of ways to install the Cyclus core:

- To install from source code, see the `Cyclus Core repository
  <http://github.com/cyclus/cyclus>`_

- To install from a binary distribution, see the instructions for
  `Installing Cyclus from Binaries <DEPENDENCIES.rst>`_

******************************
Quick Recycle Installation
******************************

Assuming you have the dependencies installed correctly, installing Recycle using
github is fairly straightforward:

- clone the Recycle Repo: ``git clone https://github.com/cyclus/recyle.git``,

- to install Recycle locally (in ``~/.local/``) just run: ``python install.py``
  from recycle folder,

For more detailed installation procedure, and/or custom installation please
refer to the `INSTALLATION guide <INSTALL.rst>`_.


******************************
Running Tests
******************************

Installing Recycle will also install a test driver (i.e., an executable of all of
our tests). You can run the tests yourself via:

.. code-block:: bash

    $ recyle_unit_tests

******************************
Contributing
******************************

We happily welcome new developers into the Cyclus Developer Team. If you are willing
to contribute into Cyclus, please follow this procedure:

#. Fork Recycle repository,

#. Create a working branch on you fork from the ``master`` branch,

#. Implement your modification of the Recycle source code,

#. Submit a Pull request into ``recycle/master`` branch,

#. Wait for reviews/merge (the Puller cannot be the Merger).

You may also want to read our `Contribution Guidelines <CONTRIBUTING.rst>`_.

.. _`CMake`: https://cmake.org
.. _`Cyclus Homepage`: http://fuelcycle.org/
.. _`Cyclus User Guide`: http://fuelcycle.org/user/index.html
.. _`Cyclus repo`: https://github.com/cyclus/cyclus
.. _`Cycamore Repo`: https://github.com/cyclus/recyle
.. _`INSTALL`: INSTALL.rst
.. _`CONTRIBUTING`: CONTRIBUTING.rst

