.. index::
    single: Sandbox Installation

Installing the CMF sandbox
==========================

.. include:: ../_outdate-caution.rst.inc

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
`Requirements for running Symfony2`_. `Git 1.6+`_, `Curl`_ and PHP Intl are
also needed to follow the installation steps listed below.

For the PHPCR repository requirements, see
:ref:`PHPCR-ODM requirements <cookbook-phpcr-odm-requirements>`

Installation
------------

Apache Jackrabbit
~~~~~~~~~~~~~~~~~

The Symfony CMF Sandbox uses Jackalope with Apache JackRabbit by default.
Alternative storage methods can be configured, but this is the most tested,
and should be the easiest to setup.

You can get the latest Apache Jackrabbit version from the project's
`official download page`_. To start it, use the following command

.. code-block:: bash

    $ java -jar jackrabbit-standalone-*.jar

By default the server is listening on the 8080 port, you can change this
by specifying the port on the command line.

.. code-block:: bash

    $ java -jar jackrabbit-standalone-*.jar --port 8888

For unix systems, you can get the start-stop script for ``/etc/init.d``
`here`_.

Getting the Sandbox Code
~~~~~~~~~~~~~~~~~~~~~~~~

The Symfony CMF Sandbox source code is available on github. To get it use:

.. code-block:: bash

    $ git clone git://github.com/symfony-cmf/cmf-sandbox.git

Move into the folder and copy the default configuration files:

.. code-block:: bash

    $ cd cmf-sandbox
    $ cp app/config/parameters.yml.dist app/config/parameters.yml
    $ cp app/config/phpcr_jackrabbit.yml.dist app/config/phpcr.yml

These two files include the default configuration parameters for the sandbox
storage mechanism. You can modify them to better fit your needs

.. note::

    The second configuration file refers to specific jackalope + jackrabbit
    configuration. There are other files available for different stack setups.

Next, get composer and install and the necessary bundles (this may take a
while):

.. code-block:: bash

    # get composer
    $ curl -s http://getcomposer.org/installer | php --

    # install bundles
    $ php composer.phar install

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
which is exists by default in Jackrabbit. If you use other applications that
require Jackrabbit or if you just wish to change the workspace name, you
can do so in ``app/config/phpcr.yml``. The following command will create
a new workspace named "sandbox" in Jackrabbit. If you decide to use the
"default" workspace, you can skip it.

.. code-block:: bash

    $ php app/console doctrine:phpcr:workspace:create sandbox

Once your workspace is set up, you need to `register the node types`_ for
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
doctrine proxies and dump the assetic assets:

.. code-block:: text

    $ php app/console cache:clear --env=prod --no-debug
    $ php app/console assetic:dump --env=prod --no-debug

Alternative Storage Mechanisms
------------------------------

Symfony CMF and the sandbox are storage agnostic, which means you can change
the storage mechanism without having to change your code. The default storage
mechanism for the sandbox is Jackalope + Apache Jackrabbit, as it's the most
tested and stable setup. However, other alternatives are available.

Jackalope + Doctrine DBAL
~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::

    By default, when using Doctrine DBAL, data is stored using a `Sqlite`_
    database.  Refer to the project's page for installation instructions.  If
    you wish to use other database systems, change the configuration
    parameters in ``app/config/parameters.yml``. Refer to
    `Symfony's page on Doctrine DBAL configuration`_ or
    `Doctrine's documentation`_ for more information.

Move into the sandbox folder and copy the default configuration file for
Doctrine DBAL setup:

.. code-block:: bash

    $ cd cmf-sandbox
    $ cp app/config/phpcr_doctrine_dbal.yml.dist app/config/phpcr.yml

Next, you need to install the actual Doctrine DBAL bundle required by jackalope:

.. code-block:: bash

    $ php composer.phar require jackalope/jackalope-doctrine-dbal:1.0.*

And create and init your database:

.. code-block:: bash

    $ php app/console doctrine:database:create
    $ php app/console doctrine:phpcr:init:dbal

After this, your should follow the steps in `Preparing the PHPCR repository`_.

Doctrine caching
................

Optionally, to improve performance, you can install DoctrineCacheBundle by
typing the following command:

.. code-block:: bash

    $ php composer.phar require doctrine/cache-bundle:1.0.*

And adding the following entry to your ``app/AppKernel.php``::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
      $bundles = array(
          // ...
          new Doctrine\Bundle\DoctrineCacheBundle\DoctrineCacheBundle(),
          // ...
      );
    }

Finally, uncomment the caches settings in the ``phpcr.yml`` as well as the
``doctrine_cache`` settings in ``config.yml``.

.. code-block:: yaml

    # app/config/phpcr.yml
    caches:
        meta: doctrine_cache.providers.phpcr_meta
        nodes: doctrine_cache.providers.phpcr_nodes

.. code-block:: yaml

    # app/config/config.yml

    # ...

    doctrine_cache:
        providers:
            phpcr_meta:
                type: file_system
            phpcr_nodes:
                type: file_system

Midgard2 PHPCR Provider
~~~~~~~~~~~~~~~~~~~~~~~

If you want to run the CMF sandbox with the `Midgard2 PHPCR`_ provider instead
of Jackrabbit, you need to install the midgard2 PHP extension. On current
Debian/Ubuntu systems, this is simply done with:

.. code-block:: bash

    $ sudo apt-get install php5-midgard2

On OS X you can install it using either `Homebrew`_ with:

.. code-block:: bash

    $ brew install midgard2-php

or `MacPorts`_ with:

.. code-block:: bash

    $ sudo port install php5-midgard2

You also need to download the `midgard_tree_node.xml`_ and
`midgard_namespace_registery.xml`_ schema files and place them into
``<your-midgard2-folder>/schema`` (defaults to
``"/usr/share/midgard2/schema"``)

To have the Midgard2 PHPCR implementation installed run the following additional command:

.. code-block:: bash

    $ php composer.phar require midgard/phpcr:dev-master

Finally, switch to one of the Midgard2 configuration file:

.. code-block:: bash

    $ cp app/config/phpcr_midgard_mysql.yml.dist app/config/phpcr.yml

or:

.. code-block:: bash

    $ cp app/config/phpcr_midgard_sqlite.yml.dist app/config/phpcr.yml

After this, your should follow the steps in `Preparing the PHPCR repository`_
to continue the installation process.

.. _`CMF sandbox github repository`: https://github.com/symfony-cmf/cmf-sandbox
.. _`Requirements for running Symfony2`: http://symfony.com/doc/current/reference/requirements.html
.. _`Git 1.6+`: http://git-scm.com/
.. _`Curl`: http://curl.haxx.se/
.. _`official download page`: http://jackrabbit.apache.org/downloads.html
.. _`here`: https://github.com/sixty-nine/Jackrabbit-startup-script
.. _`register the node types`: https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged
.. _`Sqlite`: http://www.sqlite.org/
.. _`Symfony's page on Doctrine DBAL configuration`: http://symfony.com/doc/current/reference/configuration/doctrine.html#doctrine-dbal-configuration
.. _`Doctrine's documentation`: http://docs.doctrine-project.org/projects/doctrine-dbal/en/latest/reference/configuration.html
.. _`Midgard2 PHPCR`: http://midgard-project.org/phpcr/
.. _`Homebrew`: http://mxcl.github.com/homebrew/
.. _`MacPorts`: http://www.macports.org/
.. _`midgard_tree_node.xml`: https://raw.github.com/midgardproject/phpcr-midgard2/master/data/share/schema/midgard_tree_node.xml
.. _`midgard_namespace_registery.xml`: https://github.com/midgardproject/phpcr-midgard2/raw/master/data/share/schema/midgard_namespace_registry.xml
