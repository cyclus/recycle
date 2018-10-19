
Each subdirectory contains a Dockerfile that does something useful:

* ``develop-ci`` is the Dockerfile used for running Recycle on a continuous
  integration service.  This Dockerfile assumes that the current working
  directory is a Recycle repository - and that version of Recycle is copied
  into the Docker container and used for the build.  The Dockerfile in the
  Recycle repository root is a symbolic link to this Dockerfile.  This
  Dockerfile uses the base image ``cyclus/cyclus:latest`` from the Docker hub
  repository. This Docker container is uploaded as ``cyclus/recycle:latest`` on
  the Docker hub when merging a PR on the `develop` branch of the Recycle Github
  repository.

* ``master-ci`` is the Dockerfile used for running last stable Recycle version
  a continuous integration service. This Dockerfile works exactly as the
  develop-ci one, except that it builds Recycle against the
  ``cyclus/cyclus:stable`` from the Docker hub repository. This Docker container
  is also uploaded on the Docker hub as the ``cyclus/recycle:stable`` when
  merging a PR on the `master` branch of Recycle Github repository.

* ``deb-ci`` contains a script which allows to build the Dockerfile require
  to generate a Recycle Debian package for the installation of Recycle on
  ubuntu. The script take the major version number of ubuntu as a parameter. It
  builds Recycle on the corresponding Ubuntu-LTS version, after installing all
  Recycle dependencies -including Cyclus- and generates the Recycle Debian
  installation package that need to be extracted from the Docker container
  (tested on Ubuntu 14.04 & 16.04)


