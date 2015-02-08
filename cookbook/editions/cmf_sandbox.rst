.. index::
    single: CMF Sandbox Installation

Installing the CMF sandbox
==========================

.. tip::

    The `CMF sandbox github repository`_ contains a README file with an up to date
    installation instruction.

This tutorial shows how to install the Symfony CMF Sandbox, a demo platform
aimed at showing the tool's basic features running on a demo environment.
This can be used to evaluate the platform or to see actual code in action,
helping you understand the tool's internals.

While it can be used as such, this sandbox does not intend to be a development
platform. If you are looking for installation instructions for a development
setup, please refer to:

* :doc:`../../book/installation` page for instructions on
  how to quickly install the CMF (recommended for development)
* :doc:`cmf_core` for step-by-step installation and
  configuration details (if you want to know all the details)

.. index:: sandbox, install

Requirements
------------

As Symfony CMF Sandbox is based on Symfony2, you should make sure you meet the
`Requirements for running Symfony2`_. `Git 1.6+`_, and the PHP Intl extension are
also needed to follow the installation steps listed below.

Installation
------------

Getting the Sandbox Code: Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The easiest way to install the CMF sandbox is is using `Composer`_. Get it using

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php

and then get the Symfony CMF code with it (this may take a while):

.. code-block:: bash

    $ composer create-project --no-install symfony-cmf/sandbox <path-to-install> ~1.2
    $ cd <path-to-install>

Getting the Sandbox Code: GIT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Alternatively you can get the sandbox from GIT. If you want to contribute to
the sandbox, you need the GIT information. Just clone the repository from
github:

.. code-block:: bash

    $ git clone git://github.com/symfony-cmf/cmf-sandbox.git <path-to-install>
    $ cd <path-to-install>

Configuration
~~~~~~~~~~~~~

Change into the root folder of the sandbox and copy the default configuration
files:

.. code-block:: bash

    $ cp app/config/parameters.yml.dist app/config/parameters.yml
    $ cp app/config/phpcr_doctrine_dbal.yml.dist app/config/phpcr.yml

These two files include the default configuration parameters for the sandbox
storage mechanism. You can modify them to better fit your needs

.. note::

    The second configuration file refers to specific jackalope + doctrine dbal
    configuration. There are other files available for
    :doc:`different PHPCR implementations <../database/choosing_phpcr_implementation>`.

Next, use composer to install the necessary bundles (this may take a while):

.. code-block:: bash

    $ composer install

.. note::

    On Windows you need to run the shell as Administrator or edit the
    ``composer.json`` and change the line ``"symfony-assets-install":
    "symlink"`` to ``"symfony-assets-install": ""``. If you fail to do this
    you might receive:

    .. code-block:: text

        [Symfony\Component\Filesystem\Exception\IOException]
        Unable to create symlink due to error code 1314: 'A required privilege is not held
        by the client'. Do you have the required Administrator-rights?

Preparing the PHPCR Repository
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now that you have all the code, you need to setup your PHPCR repository.
PHPCR organizes data in workspaces and sandbox uses the "default" workspace,
which is exists automatically on a new repository.

Now you need to create the database. The default database specified in
parameters.yml is sqlite. If you have the sqlite PHP extension, simply
run:

.. code-block:: bash

    $ php app/console doctrine:database:create

If you don't have sqlite, you can specify ``pdo_mysql`` or ``pdo_pgsql`` and
provide the database name and login credentials to use.

Then you have to set up your database with:

.. code-block:: bash

    $ php app/console doctrine:phpcr:init:dbal

Once your database is set up, you need to `register the node types`_ for
phpcr-odm:

.. code-block:: bash

    $ php app/console doctrine:phpcr:repository:init

Import the Fixtures
~~~~~~~~~~~~~~~~~~~

The sandbox provides a set of demo content to show various use cases.
They are loaded using the fixture loading concept of PHPCR-ODM.

.. code-block:: bash

    $ php app/console -v doctrine:phpcr:fixtures:load

This command loads fixtures from all bundles that provide them in the
``DataFixtures/PHPCR`` folder. The sandbox has fixtures in the
MainBundle. Note that loading fixtures from non-default locations is
possible as well, just not needed in this case.

Accessing your Sandbox
~~~~~~~~~~~~~~~~~~~~~~

The sandbox should now be accessible on your web server.

.. code-block:: text

    http://localhost/app_dev.php

In order to run the sandbox in production mode you need to generate the
doctrine proxies and dump the Assetic assets:

.. code-block:: text

    $ php app/console cache:clear --env=prod --no-debug
    $ php app/console assetic:dump --env=prod --no-debug

.. _`Composer`: http://getcomposer.org
.. _`CMF sandbox github repository`: https://github.com/symfony-cmf/cmf-sandbox
.. _`Requirements for running Symfony2`: http://symfony.com/doc/current/reference/requirements.html
.. _`Git 1.6+`: http://git-scm.com/
.. _`register the node types`: https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged
