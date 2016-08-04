.. index::
    single: PHPCR; Bundles
    single: DoctrinePHPCRBundle

DoctrinePHPCRBundle
===================

The `DoctrinePHPCRBundle`_ provides integration with the PHP content
repository and optionally with Doctrine PHPCR-ODM to provide the ODM document
manager in symfony.

Out of the box, this bundle supports the following PHPCR implementations:

* `Jackalope`_ (Jackrabbit, Doctrine DBAL and prismic transports)

.. tip::

    This reference only explains the Symfony2 integration of PHPCR and
    PHPCR-ODM. To learn how to use PHPCR, refer to `the PHPCR website`_ and
    for Doctrine PHPCR-ODM to the `PHPCR-ODM documentation`_.

Setup
-----

Requirements
~~~~~~~~~~~~

* When using **jackalope-jackrabbit**: Java, Apache Jackalope and ``libxml``
  version >= 2.7.0 (due to a `bug in libxml`_)
* When using **jackalope-doctrine-dbal with MySQL**: MySQL >= 5.1.5
  (as you need the xml function ``ExtractValue``)

Installation
------------

You can install this bundle `with composer`_ using the
`doctrine/phpcr-bundle`_ package. You need a concrete implementation of
the PHPCR API. For this example, we assume that you require Jackalope Doctrine
DBAL. See :doc:`../../cookbook/database/choosing_phpcr_implementation` for
alternatives.

If you want to use PHPCR-ODM, you additionally need to require
``doctrine/phpcr-odm``.

.. code-block:: javascript

    require: {
        ...
        "jackalope/jackalope-doctrine-dbal": "1.2.*",
        "doctrine/phpcr-odm": "1.2.*",
        "doctrine/phpcr-bundle": "1.2.*",
        ...
    }

Besides the ``DoctrinePHPCRBundle`` you also need to instantiate the base
``DoctrineBundle`` in your kernel::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),
                new Doctrine\Bundle\DoctrineBundle\DoctrineBundle(),
            );

            // ...
        }

        // ...
    }

Configuration
-------------

PHPCR Session Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The session needs a PHPCR implementation specified in the ``backend`` section
by the ``type`` field, along with configuration options to bootstrap the
implementation. The examples here assume that you are using Jackalope Doctrine
DBAL. The full documentation is in the :doc:`configuration reference <configuration>`.

To use Jackalope Doctrine DBAL, you need to configure a database connection
with the DoctrineBundle. For detailed information, see the
`Symfony2 Doctrine documentation`_. A simple example is:

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   pdo_mysql
        database_host:     localhost
        database_name:     test_project
        database_user:     root
        database_password: password

    # ...

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   "%database_driver%"
                host:     "%database_host%"
                dbname:   "%database_name%"
                user:     "%database_user%"
                password: "%database_password%"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal
                    driver="%database_driver%"
                    host="%database_host%"
                    dbname="%database_name%"
                    user="%database_user%"
                    password="%database_password%"
                />
            </doctrine:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $configuration->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'   => '%database_driver%',
                'host'     => '%database_host%',
                'dbname'   => '%database_name%',
                'user'     => '%database_user%',
                'password' => '%database_password%',
            ),
        ));

Jackalope Doctrine DBAL provides a PHPCR implementation without any
installation requirements beyond any of the RDBMS supported by Doctrine.
Once you set up Doctrine DBAL, you can configure Jackalope:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: doctrinedbal
                    # connection: default

                    # requires DoctrineCacheBundle
                    # caches:
                    #     meta: doctrine_cache.providers.phpcr_meta
                    #     nodes: doctrine_cache.providers.phpcr_nodes
                    # enable logging
                    logging: true
                    # enable profiling in the debug toolbar.
                    profiling: true
                workspace: default
                username: admin
                password: admin

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">

                <session
                    workspace="default"
                    username="admin"
                    password="admin"
                >

                    <backend
                        type="doctrinedbal"
                        logging="true"
                        profiling="true"
                    >
                        <!-- connection="default" - option on <backend> to change dbal connection -->
                        <!--
                        <caches
                            meta="doctrine_cache.providers.phpcr_meta"
                            nodes="doctrine_cache.providers.phpcr_nodes"
                        />
                        -->
                    </backend>
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    'type'       => 'doctrinedbal',
                    //'connection': 'default',
                    'logging'    => true,
                    'profiling'  => true,
                    //'caches' => array(
                    //    'meta' => 'doctrine_cache.providers.phpcr_meta'
                    //    'nodes' => 'doctrine_cache.providers.phpcr_nodes'
                    //),
                ),
                'workspace' => 'default',
                'username'  => 'admin',
                'password'  => 'admin',
            ),
        ));

Now make sure the database exists and initialize it:

.. code-block:: bash

    # without Doctrine ORM
    php bin/console doctrine:database:create
    php bin/console doctrine:phpcr:init:dbal

.. tip::

    You can also use a different doctrine dbal connection instead of the
    default. Specify the dbal connection name in the ``connection`` option of
    the ``backend`` configuration.

    It is recommended to use a separate connection to a separate database if
    you also use Doctrine ORM or direct DBAL access to data, rather than
    mixing this data with the tables generated by Jackalope Doctrine Dbal.  If
    you have a separate connection, you need to pass the alternate connection
    name to the ``doctrine:database:create`` command with the ``--connection``
    option. For Doctrine PHPCR commands, this parameter is not needed as you
    configured the connection to use.

If you are using Doctrine ORM on the same connection, the schema is integrated
into ``doctrine:schema:create|update|drop`` and also `DoctrineMigrationsBundle`_
so that you can create migrations.

.. code-block:: bash

    # Using Doctrine ORM
    php bin/console doctrine:database:create
    php bin/console doctrine:schema:create

.. note::

    To use the cache, install and configure the
    :doc:`DoctrineCacheBundle <../../cookbook/database/doctrine_cache>`.
    Then uncomment the cache meta and nodes settings.

Doctrine PHPCR-ODM Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This configuration section manages the document mapper system that converts
your PHPCR nodes to domain model objects. If you do not configure anything
here, the ODM services will not be loaded.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            odm:
                auto_mapping: true
                auto_generate_proxy_classes: "%kernel.debug%"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">

                <odm
                    auto-mapping="true"
                    auto-generate-proxy-classes="%kernel.debug%"
                />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'odm' => array(
                'auto_mapping' => true,
                'auto_generate_proxy_classes' => '%kernel.debug%',
            ),
        ));

Unless you disable ``auto_mapping``, you can place your documents in the
``Document`` folder inside your bundles and use annotations or name the
mapping files following this schema:
``<Bundle>/Resources/config/doctrine/<DocumentClass>.phpcr.xml`` or ``*.phpcr.yml``.

If ``auto_generate_proxy_classes`` is false, you need to run the
``cache:warmup`` command in order to have the proxy classes generated after
you modified a document. This is usually done in production to gain some performance.


Registering System Node Types
"""""""""""""""""""""""""""""

PHPCR-ODM uses a `custom node type`_ to track meta information without
interfering with your content. There is a command that makes it trivial to
register this type and the PHPCR namespace, as well as all base paths of
bundles:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:repository:init

You only need to run this command once when you created a new repository. (But
nothing goes wrong if you run it on each deployment for example.)

Profiling and Performance of Jackalope
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When using any of the Jackalope PHPCR implementations, you can activate logging
to log to the symfony log, or profiling to show information in the Symfony2
debug toolbar:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    # ...
                    logging: true
                    profiling: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">

                <session>

                    <backend
                        logging="true"
                        profiling="true"
                    />
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.yml
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    // ...
                    'logging'   => true,
                    'profiling' => true,
                ),
            ),
        ));

Now that you can see the effects of changes, you can try if adjusting the global
fetch depth reduces the number and duration for queries. Set the option
``jackalope.fetch_depth`` to something bigger than 0 to have Jackalope pre-fetch
children or whole subtrees. This can reduce the number of queries needed, but
watch out for longer queries because more data is fetched.

When using Jackalope Doctrine DBAL, it is highly recommended to
:doc:`activate the caching options <../../cookbook/database/doctrine_cache>`.

Note that you can also set the fetch-depth on the session on the fly for
specific calls, or use the fetch-depth option on children mappings of your
documents.

The parameter ``jackalope.check_login_on_server`` can be set to false to save
an initial call to the database to check if the connection works.

Services
--------

There are 3 main services provided by this bundle:

* ``doctrine_phpcr``- The ``ManagerRegistry`` instance with references to all
  sessions and document manager instances;
* ``doctrine_phpcr.default_session`` - The PHPCR session instance;
* ``doctrine_phpcr.odm.default_document_manager`` - The PHPCR-ODM document
  manager instance.

.. _bundle-phpcr-odm-commands:

Doctrine PHPCR Commands
-----------------------

All commands about PHPCR are prefixed with ``doctrine:phpcr`` and you can use
the --session argument to use a non-default session if you configured several
PHPCR sessions.

Some of these commands are specific to a backend or to the ODM. Those commands
will only be available if such a backend is configured.

Use ``php bin/console help <command>`` to see all options each of the commands
has.

* **doctrine:phpcr:document:migrate-class**: Command to migrate document classes;
* **doctrine:phpcr:fixtures:load**: Load data fixtures to your PHPCR database;
* **doctrine:phpcr:init:dbal**: Prepare the database for Jackalope Doctrine-Dbal;
* **doctrine:phpcr:jackrabbit**: Start and stop the Jackrabbit server (see also
  :doc:`../../cookbook/database/running_jackrabbit`);
* **doctrine:phpcr:mapping:info**: Shows basic information about all mapped documents;
* **doctrine:phpcr:migrator:migrate**: Migrates PHPCR data;
* **doctrine:phpcr:node-type:list**: List all available node types in the repository;
* **doctrine:phpcr:node-type:register**: Register node types in the PHPCR repository;
* **doctrine:phpcr:node:dump**: Dump subtrees of the content repository;
* **doctrine:phpcr:node:move**: Moves a node from one path to another;
* **doctrine:phpcr:node:remove**: Remove content from the repository;
* **doctrine:phpcr:node:touch**: Create or modify a node;
* **doctrine:phpcr:nodes:update**: Command to manipulate the nodes in the workspace;
* **doctrine:phpcr:repository:init**: Initialize the PHPCR repository;
* **doctrine:phpcr:workspace:create**: Create a workspace in the configured repository;
* **doctrine:phpcr:workspace:export**: Export nodes from the repository,
  either to the JCR system view format or the document view format;
* **doctrine:phpcr:workspace:import**: Import xml data into the repository,
  either in JCR system view format or arbitrary xml;
* **doctrine:phpcr:workspace:list**: List all available workspaces in the configured repository;
* **doctrine:phpcr:workspace:purge**: Remove all nodes from a workspace;
* **doctrine:phpcr:workspace:query**: Execute a JCR SQL2 statement.

.. note::

    To use the ``doctrine:phpcr:fixtures:load`` command, you additionally need
    to install the `DoctrineFixturesBundle`_ and its dependencies. See
    :ref:`phpcr-odm-repository-fixtures` for how to use fixtures.

Some Example Command Runs
~~~~~~~~~~~~~~~~~~~~~~~~~

Running `SQL2 queries`_ against the repository:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:workspace:query "SELECT title FROM [nt:unstructured] WHERE NAME() = 'home'"

Dumping nodes under ``/cms/simple`` including their properties:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:node:dump /cms/simple --props

.. _phpcr-odm-backup-restore:

Simple Backup and Restore
~~~~~~~~~~~~~~~~~~~~~~~~~

To export all repository data into a file, you can use:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:workspace:export --path /cms /path/to/backup.xml

.. note::

    You always want to specify a path to export. Without any path you will
    export the root node of the repository, which will be imported later as
    ``jcr:root``.

To restore this backup you can run:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:workspace:import /path/to/backup.xml

Note that you can also export and import parts of your repository by choosing a
different path on export and specifying the ``--parentpath`` option to the
import.

If you already have data in your repository that you want to replace, you can
remove the target node first:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:node:remove /cms

Read On
-------

* :doc:`events`
* :doc:`forms`
* :doc:`fixtures_initializers`
* :doc:`multilang`
* :doc:`multiple_sessions`
* :doc:`configuration`

.. _`DoctrinePHPCRBundle`: https://github.com/doctrine/DoctrinePHPCRBundle
.. _`Symfony2 Doctrine documentation`: https://symfony.com/doc/current/book/doctrine.html
.. _`Jackalope`: http://jackalope.github.io/
.. _`the PHPCR website`: http://phpcr.github.io/
.. _`PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/
.. _`bug in libxml`: https://bugs.php.net/bug.php?id=36501
.. _`with composer`: https://getcomposer.org
.. _`doctrine/phpcr-bundle`: https://packagist.org/packages/doctrine/phpcr-bundle
.. _`metadata caching`: https://symfony.com/doc/master/reference/configuration/doctrine.html
.. _`PHPCR-ODM documentation on Multilanguage`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html
.. _`custom node type`: https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged
.. _`the PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/events.html
.. _`Symfony event subscriber`: https://symfony.com/doc/master/components/event_dispatcher/introduction.html#using-event-subscribers
.. _`Symfony cookbook entry`: https://symfony.com/doc/current/cookbook/doctrine/event_listeners_subscribers.html
.. _`Symfony documentation on the entity form type`: https://symfony.com/doc/current/reference/forms/types/entity.html
.. _SonataDoctrinePHPCRAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
.. _`currently broken`: https://github.com/sonata-project/SonataDoctrineORMAdminBundle/issues/145
.. _`DoctrineMigrationsBundle`: https://symfony.com/doc/current/bundles/DoctrineMigrationsBundle/index.html
.. _`DoctrineFixturesBundle`: https://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`Doctrine data-fixtures`: https://github.com/doctrine/data-fixtures
.. _`documentation of the DoctrineFixturesBundle`: https://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`SQL2 queries`: http://www.h2database.com/jcr/grammar.html
.. _`BurgovKeyValueFormBundle`: https://github.com/Burgov/KeyValueFormBundle
