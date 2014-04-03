.. index::
    single: PHPCR; Bundles
    single: DoctrinePHPCRBundle

DoctrinePHPCRBundle
===================

.. include:: _outdate-caution.rst.inc

The `DoctrinePHPCRBundle`_ provides integration with the PHP content
repository and optionally with Doctrine PHPCR-ODM to provide the ODM document
manager in symfony.

Out of the box, this bundle supports the following PHPCR implementations:

* `Jackalope`_ (both Jackrabbit and Doctrine Dbal transports)
* `Midgard2`_

.. tip::

    This reference only explains the Symfony2 integration of PHPCR and
    PHPCR-ODM. To learn how to use PHPCR, refer to `the PHPCR website`_ and
    for Doctrine PHPCR-ODM to the `PHPCR-ODM documentation`_.

Setup and Requirements
----------------------

See :doc:`../cookbook/installing_configuring_doctrine_phpcr_odm`

Configuration
-------------

.. tip::

    If you want to only use plain PHPCR without the PHPCR-ODM, you can simply
    not configure the ``odm`` section to avoid loading the services at all.
    Note that most CMF bundles by default use PHPCR-ODM documents and thus
    need ODM enabled.

PHPCR Session Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The session needs a PHPCR implementation specified in the ``backend`` section
by the ``type`` field, along with configuration options to bootstrap the
implementation. Currently we support ``jackrabbit``, ``doctrinedbal`` and
``midgard2``. Regardless of the backend, every PHPCR session needs a
workspace, username and password.

.. tip::

    Every PHPCR implementation should provide the workspace called *default*,
    but you can choose a different one. There is the
    ``doctrine:phpcr:workspace:create`` command to initialize a new workspace.
    See also :ref:`bundle-phpcr-odm-commands`.

The username and password you specify here are what is used on the PHPCR layer
in the ``PHPCR\SimpleCredentials``. They will usually be different from the
username and password used by Midgard2 or Doctrine DBAL to connect to the
underlying RDBMS where the data is actually stored.

If you are using one of the Jackalope backends, you can set a couple of
parameters:

* ``jackalope.factory``: (string or object) Use a custom factory class for
  Jackalope objects;
* ``jackalope.check_login_on_server``: (boolean) If set to ``false``, skip initial
  check whether repository exists. Disabled by default, you will only notice
  connectivity problems on the first attempt to use the repository;
* ``jackalope.disable_stream_wrapper``: (boolean) If set to ``false``, streams are
  read immediately instead of first access. If you run into problems with
  streams this might be useful for debugging. Otherwise you probably don't want
  to disable the wrappers, or all binaries will be loaded each time their
  containing document is loaded;
* ``jackalope.auto_lastmodified``: (boolean) Whether to automatically update
  nodes having mix:lastModified. Defaults to ``true``;
* See below for backend specific configuration options.

With a Jackalope backend, you can also
`activate profiling and tune many parameters <Profiling and Performance of Jackalope>`_.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    # to configure logging
                    logging: true
                    # to configure profiling for the debug toolbar.
                    profiling: true
                    # optional parameters for Jackalope
                    parameters:
                        jackalope.factory: "Jackalope\Factory"
                        jackalope.check_login_on_server: false
                        jackalope.disable_stream_wrapper: false
                        jackalope.auto_lastmodified: true
                        # see below for how to configure the backend of your choice
                workspace: default
                username: admin
                password: admin
                # tweak options for Jackalope (all versions)
                options:
                    jackalope.fetch_depth: 1

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

                <session
                    workspace="default"
                    username="admin"
                    password="admin"
                >

                    <backend
                        logging="true"
                        profiling="true"
                    >
                        <parameter key="jackalope.factory">Jackalope\Factory</parameter>
                        <parameter key="jackalope.check_login_on_server">false</parameter>
                        <parameter key="jackalope.disable_stream_wrapper">false</parameter>
                        <parameter key="jackalope.auto_lastmodified">true</parameter>
                    </backend>

                    <options
                        jackalope.fetch_depth="1"
                    />
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    'logging'    => true,
                    'profiling'  => true,
                    'parameters' => array(
                        'jackalope.factory'                => 'Jackalope\Factory',
                        'jackalope.check_login_on_server'  => false,
                        'jackalope.disable_stream_wrapper' => false,
                        'jackalope.auto_lastmodified'      => true,
                    ),
                ),
                'workspace' => 'default',
                'username'  => 'admin',
                'password'  => 'admin',
                'options'   => array(
                    'jackalope.fetch_depth' => 1,
                ),
            ),
        ));

If you want control over the last modified timestamp, disable
``jackalope.auto_lastmodified`` and read the `last modified listener cookbook entry`_
of the PHPCR-ODM documentation.

.. versionadded:: 1.1

    Since DoctrinePhpcrBundle 1.1, backend configuration flags are configured
    in the ``parameters`` section. They are passed as-is to Jackalope. See the
    ``RepositoryFactory`` for some more documentation on the meaning of those
    parameters.

    For backwards compatibility reason, the options on ``backend`` for
    ``check_login_on_server``, ``disable_stream_wrapper`` and
    ``disable_transactions`` still work, but it is recommended to move them
    into the parameters section with the ``jackalope.`` part in front of them.
    Note that only Jackalope Doctrine Dbal supports transactions.

PHPCR Session with Jackalope Jackrabbit
"""""""""""""""""""""""""""""""""""""""

The only setup required is to install Apache Jackrabbit (see
:ref:`installing Jackrabbit <cookbook-installing-phpcr-jackrabbit>`).

The configuration needs the ``url`` parameter to point to your Jackrabbit.
You can tune the behaviour with the general Jackalope parameters listed above,
as well as some Jackrabbit-specific options:

* ``jackalope.default_header``: (string) Set a default header to send on each
  request to the backend (i.e. for load balancers to identify sessions);
* ``jackalope.jackrabbit_expect``: (boolean) Send the "Expect: 100-continue"
  header on larger PUT and POST requests. Disabled by default to avoid issues
  with proxies and load balancers.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: jackrabbit
                    url: http://localhost:8080/server/
                    parameters:
                        # general parameters and options
                        # ...
                        # optional parameters specific to Jackalope Jackrabbit
                        jackalope.default_header: 'X-ID: %serverid%'
                        jackalope.jackrabbit_expect: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

                <session>

                    <backend
                        type="jackrabbit"
                        url="http://localhost:8080/server/"
                    >
                        <parameter key="jackalope.default_header">X-ID: %serverid%</parameter>
                        <parameter key="jackalope.jackrabbit_expect">true</parameter>
                    </backend>
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    'type'       => 'jackrabbit',
                    'url'        => 'http://localhost:8080/server/',
                    'parameters' => array(
                        'jackalope.default_header'    => 'X-ID: %serverid%',
                        'jackalope.jackrabbit_expect' => true,
                    ),
                ),
            ),
        ));

.. _bundle-phpcr-odm-doctrinedbal:

PHPCR Session with Jackalope Doctrine DBAL
""""""""""""""""""""""""""""""""""""""""""

This type uses Jackalope with a Doctrine database abstraction layer transport
to provide PHPCR without any installation requirements beyond any of the RDBMS
supported by Doctrine.

You can specify the connection name to use if you don't want to use the default
connection. The name must be one of the names of the dbal section in your
Doctrine configuration, see `the Symfony2 Doctrine documentation`_.

You can tune the behaviour with the general Jackalope parameters listed above,
as well as enable transactions (Jackrabbit does not support transactions):

* ``jackalope.disable_transactions``: (boolean) Set to ``false`` to disabled
  transactions. If transactions are enabled but not actively used, every save
  operation is wrapped into a transaction.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: doctrinedbal
                    # if no explicit connection is specified, the default connection is used.
                    connection: default
                    # to configure caching
                    caches:
                        meta: doctrine_cache.providers.phpcr_meta
                        nodes: doctrine_cache.providers.phpcr_nodes
                    parameters:
                        # ... general parameters and options

                        # optional parameters specific to Jackalope Doctrine Dbal
                        jackalope.disable_transactions: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

                <session>

                    <backend type="doctrinedbal" connection="default">
                        <caches
                            meta="doctrine_cache.providers.phpcr_meta"
                            nodes="doctrine_cache.providers.phpcr_nodes"
                        />

                        <!-- ... general parameters and options -->

                        <!-- optional parameters specific to Jackalope Doctrine Dbal -->
                        <parameter key="jackalope.disable_transactions">false</parameter>
                    </backend>
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    'type' => 'doctrinedbal',
                    'connection' => 'default',
                    'caches' => array(
                        'meta' => 'doctrine_cache.providers.phpcr_meta'
                        'nodes' => 'doctrine_cache.providers.phpcr_nodes'
                    ),
                    'parameters' => array(
                        // ... general parameters and options

                        // optional parameters specific to Jackalope Doctrine Dbal
                        'jackalope.disable_transactions' => false,
                    ),
                ),
            ),
        ));

Once the connection is configured, make sure the database exists and initialize
it. If you are using Doctrine ORM on the same connection, the schema is
integrated into ``doctrine:schema:create|update|drop`` and also `DoctrineMigrationsBundle`_.

.. code-block:: bash

    # Using Doctrine ORM
    $ php app/console doctrine:database:create
    $ php app/console doctrine:schema:create

If you do not use the ORM, there is the specific ``doctrine:phpcr:init:dbal``
command.

.. code-block:: bash

    # without Doctrine ORM
    $ php app/console doctrine:database:create
    $ php app/console doctrine:phpcr:init:dbal

.. tip::

    Of course, you can also use a different connection instead of the default.
    It is recommended to use a separate connection to a separate database if
    you also use Doctrine ORM or direct DBAL access to data, rather than
    mixing this data with the tables generated by Jackalope Doctrine Dbal.  If
    you have a separate connection, you need to pass the alternate connection
    name to the ``doctrine:database:create`` command with the ``--connection``
    option. For Doctrine PHPCR commands, this parameter is not needed as you
    configured the connection to use.

To use the cache, install and configure the `DoctrineCacheBundle`_. Then
uncomment the cache meta and nodes settings.

PHPCR Session with Midgard2
"""""""""""""""""""""""""""

Midgard2 is an application that provides a compiled PHP extension. It
implements the PHPCR API on top of a standard RDBMS.

To use the Midgard2 PHPCR provider, you must have both the
`midgard2 PHP extension`_ and `the midgard/phpcr package`_ installed. The
settings here correspond to Midgard2 repository parameters as explained in
`the getting started document`_.

The session backend configuration looks as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: midgard2
                    config: /path/to/midgard-config.ini
                    db_type: MySQL
                    db_name: midgard2_test
                    db_host: "0.0.0.0"
                    db_port: 3306
                    db_username: ""
                    db_password: ""
                    db_init: true
                    blobdir: /tmp/cmf-blobs
                    loglevel: debug

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

                <session>

                    <backend type="midgard2"
                        config="/path/to/midgard-config.ini"
                        db-type="MySQL"
                        db-name="midgard2_test"
                        db-host="0.0.0.0"
                        db-port="3306"
                        db-username=""
                        db-password=""
                        db-init="true"
                        blobdir="/tmp/cmf-blobs"
                        loglevel="debug"
                    />
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => array(
                    'type'        => 'midgard2',
                    'config'      => '/path/to/midgard-config.ini',
                    'db_name'     => 'midgard2_test',
                    'db_host'     => '0.0.0.0',
                    'db_port'     => 3306,
                    'db_username' => '',
                    'db_password' => '',
                    'db_init'     => true,
                    'blobdir'     => '/tmp/cmf-blobs",
                    'loglevel'    => 'debug',
                ),
            ),
        ));

For more information, please refer to `the official Midgard PHPCR documentation`_.

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

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

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

Once you can see the effects in profiling, you can try if adjusting the global
fetch depth reduces the number and duration for queries. Set the option
``jackalope.fetch_depth`` to something bigger than 0 to have Jackalope prefetch
children or whole subtrees.

When using Jackalope Doctrine Dbal, it is highly recommended to activate the
caching options. Set ``session.backend.caches`` as explained in
`PHPCR Session with Jackalope Doctrine DBAL`_.

Note that you can also set the fetchDepth on the session on the fly for
specific calls, or use the fetchDepth option on children mappings of your
documents.

The parameter ``jackalope.check_login_on_server`` can be set to false to save
an initial call to the database to check if the connection works.

.. _bundle-phpcr-odm-configuration:

Doctrine PHPCR-ODM Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This configuration section manages the Doctrine PHPCR-ODM system. If you do
not configure anything here, the ODM services will not be loaded.

If you enable ``auto_mapping``, you can place your mappings in
``<Bundle>/Resources/config/doctrine/<Document>.phpcr.xml`` resp. ``*.phpcr.yml``
to configure mappings for documents you provide in the ``<Bundle>/Document``
folder. Otherwise you need to manually configure the mappings section.

If ``auto_generate_proxy_classes`` is false, you need to run the
``cache:warmup`` command in order to have the proxy classes generated after
you modified a document. You can also tune how and where to generate the proxy
classes with the ``proxy_dir`` and ``proxy_namespace`` settings. The the
defaults are usually fine here.

You can also enable `metadata caching`_.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            odm:
                configuration_id: ~
                auto_mapping: true
                mappings:
                    <name>:
                        mapping:   true
                        type:      ~
                        dir:       ~
                        alias:     ~
                        prefix:    ~
                        is_bundle: ~
                auto_generate_proxy_classes: "%kernel.debug%"
                proxy_dir:       "%kernel.cache_dir%/doctrine/PHPCRProxies"
                proxy_namespace: PHPCRProxies

                metadata_cache_driver:
                    type:           array
                    host:           ~
                    port:           ~
                    instance_class: ~
                    class:          ~
                    id:             ~


    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

                <odm
                    configuration-id="null"
                    auto-mapping="true"
                    auto-generate-proxy-classes="%kernel.debug%"
                    proxy-dir="%kernel.cache_dir%/doctrine/PHPCRProxies"
                    proxy-namespace="PHPCRProxies"
                >
                    <mappings>
                        <"name"
                            mapping="true"
                            type="null"
                            dir="null"
                            alias="null"
                            prefix="null"
                            is-bundle="null"
                        />
                    </mappings>

                    <metadata-cache-driver
                        type="array"
                        host="null"
                        port="null"
                        instance-class="null"
                        class="null"
                        id="null"
                    />
                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'odm' => array(
                'configuration_id' => null,
                'auto_mapping' => true,
                'auto_generate_proxy_classes' => '%kernel.debug%',
                'proxy-dir' => '%kernel.cache_dir%/doctrine/PHPCRProxies',
                'proxy_namespace' => 'PHPCRProxies',
                'mappings' => array(
                    '<name>' => array(
                        'mapping'   => true,
                        'type'      => null,
                        'dir'       => null,
                        'alias'     => null,
                        'prefix'    => null,
                        'is-bundle' => null,
                    ),
                ),
                'metadata_cache_driver' => array(
                    'type'           => 'array',
                    'host'           => null,
                    'port'           => null,
                    'instance_class' => null,
                    'class'          => null,
                    'id'             => null,
                ),
            ),
        ));

.. index:: I18N, Multilanguage

.. _bundle-phpcr-odm-multilang-config:

Translation Configuration
"""""""""""""""""""""""""

If you are using multilingual documents, you need to configure the available
languages. For more information on how to map multilingual documents, see the
`PHPCR-ODM documentation on Multilanguage`_.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            odm:
                # ...
                locales:
                    en: [de, fr]
                    de: [en, fr]
                    fr: [en, de]
                locale_fallback: hardcoded

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">

                <odm locale-fallback="hardcoded">
                    <!-- ... -->
                    <locale name="en">
                        <fallback>de</fallback>
                        <fallback>fr</fallback>
                    </locale>

                    <locale name="de">
                        <fallback>en</fallback>
                        <fallback>fr</fallback>
                    </locale>

                    <locale name="fr">
                        <fallback>en</fallback>
                        <fallback>de</fallback>
                    </locale>
                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'odm' => array(
                // ...
                'locales' => array(
                    'en' => array('de', 'fr'),
                    'de' => array('en', 'fr'),
                    'fr' => array('en', 'de'),
                ),
                'locale_fallback' => 'hardcoded',
            )
        );

The ``locales`` is a list of alternative locales to look up if a document
is not translated to the requested locale.

This bundle provides a request listener that gets activated when any locales
are configured. This listener updates PHPCR-ODM to use the locale symfony
determined for this request, if that locale is in the list of keys defined
under ``locales``.

There are several strategies to adjust the fallback order for the selected
locale based on the accepted languages of the request (determined by Symfony
from the ``Accept-Language`` HTML header). All of them will never add any
locales that where not configured in the ``locales`` to avoid a request
injecting unexpected things into your repository:

* ``hardcoded``: The default strategy does not update the fallback order from
  the request;
* ``replace`` takes the accepted locales from the request and updates the
  fallback order with them, removing any locales not found in the request;
* ``merge`` does the same as ``replace`` but then adds locales not found in
  the request but on the ``locales`` configuration back to the end of the
  fallback list. This reorders the locales without losing any of them.

General Settings
~~~~~~~~~~~~~~~~

If the `jackrabbit_jar` path is set, you can use the ``doctrine:phpcr:jackrabbit``
console command to start and stop Jackrabbit.

You can tune the output of the ``doctrine:phpcr:dump`` command with
``dump_max_line_length``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            jackrabbit_jar:       /path/to/jackrabbit.jar
            dump_max_line_length: 120

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/doctrine_phpcr"
                jackrabbit-jar="/path/to/jackrabbit.jar"
                dump-max-line-length="120"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'jackrabbit_jar'       => '/path/to/jackrabbit.jar',
            'dump_max_line_length' => 120,
        ));

.. _bundle-phpcr-odm-multiple-phpcr-sessions:

Configuring Multiple Sessions
-----------------------------

If you need more than one PHPCR backend, you can define ``sessions`` as child
of the ``session`` information. Each session has a name and the configuration
as you can use directly in ``session``. You can also overwrite which session
to use as ``default_session``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                default_session: ~
                sessions:
                    <name>:
                        workspace: ... # Required
                        username:  ~
                        password:  ~
                        backend:
                            # ...
                        options:
                            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">
                <session default-session="null">
                    <!-- workspace: Required -->
                    <session name="<name>"
                        workspace="..."
                        username="null"
                        password="null"
                    >
                        <backend>
                            <!-- ... -->
                        </backend>
                        <option>
                            <!-- ... -->
                        </option>
                    </session>
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'default_session' => null,
                'sessions' => array(
                    '<name>' => array(
                        'workspace' => '...', // Required
                        'username'  => null,
                        'password'  => null,
                        'backend'   => array(
                            // ...
                        ),
                        'options'   => array(
                            // ...
                        ),
                    ),
                ),
            ),
        ));

If you are using the ODM, you will also want to configure multiple document
managers.

Inside the odm section, you can add named entries in the
``document_managers``.  To use the non-default session, specify the session
attribute.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        odm:
            default_document_manager:  ~
            document_managers:
                <name>:
                    session: <sessionname>
                    # ... configuration as above

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">
                <odm default-document-manager="null">
                    <document-manager
                        name="<name>"
                        session="<sessionname>"
                    >
                        <!-- ... configuration as above -->
                    </document-manager>
                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'odm' => array(
                'default_document_manager' => null,
                'document_managers' => array(
                    '<name>' => array(
                        'session' => '<sessionname>',
                        // ... configuration as above
                    ),
                ),
            ),
        ));

A full example looks as follows:

.. configuration-block::

    .. code-block:: yaml

        doctrine_phpcr:
            # configure the PHPCR sessions
            session:
                sessions:
                    default:
                        backend: "%phpcr_backend%"
                        workspace: "%phpcr_workspace%"
                        username: "%phpcr_user%"
                        password: "%phpcr_pass%"

                    website:
                        backend:
                            type: jackrabbit
                            url: "%magnolia_url%"
                        workspace: website
                        username: "%magnolia_user%"
                        password: "%magnolia_pass%"

                    dms:
                        backend:
                            type: jackrabbit
                            url: "%magnolia_url%"
                        workspace: dms
                        username: "%magnolia_user%"
                        password: "%magnolia_pass%"

            # enable the ODM layer
            odm:
                auto_generate_proxy_classes: "%kernel.debug%"
                document_managers:
                    default:
                        session: default
                        mappings:
                            SandboxMainBundle: ~
                            CmfContentBundle: ~
                            CmfMenuBundle: ~
                            CmfRoutingBundle: ~

                    website:
                        session: website
                        configuration_id: sandbox_magnolia.odm_configuration
                        mappings:
                            SandboxMagnoliaBundle: ~

                    dms:
                        session: dms
                        configuration_id: sandbox_magnolia.odm_configuration
                        mappings:
                            SandboxMagnoliaBundle: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/schema/dic/doctrine_phpcr">
                <session>
                    <session name="default"
                        backend="%phpcr_backend%"
                        workspace="%phpcr_workspace%"
                        username="%phpcr_user%"
                        password="%phpcr_pass%"
                    />
                    <session name="website"
                        workspace="website"
                        username="%magnolia_user%"
                        password="%magnolia_pass%"
                    >
                        <backend type="jackrabbit" url="%magnolia_url%"/>
                    </session>
                    <session name="dms"
                        workspace="dms"
                        username="%magnolia_user%"
                        password="%magnolia_pass%"
                    >
                        <backend type="jackrabbit" url="%magnolia_url%"/>
                    </session>
                </session>

                <!-- enable the ODM layer -->
                <odm auto-generate-proxy-classes="%kernel.debug%">
                    <document-manager
                        name="default"
                        session="default"
                    >
                        <mapping name="SandboxMainBundle" />
                        <mapping name="CmfContentBundle" />
                        <mapping name="CmfMenuBundle" />
                        <mapping name="CmfRoutingBundle" />
                    </document-manager>

                    <document-manager
                        name="website"
                        session="website"
                        configuration-id="sandbox_magnolia.odm_configuration"
                    >
                        <mapping name="SandboxMagnoliaBundle" />
                    </document-manager>

                    <document-manager
                        name="dms"
                        session="dms"
                        configuration-id="sandbox_magnolia.odm_configuration"
                    >
                        <mapping name="SandboxMagnoliaBundle" />
                    </document-manager>

                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'sessions' => array(
                    'default' => array(
                        'backend'   => '%phpcr_backend%',
                        'workspace' => '%phpcr_workspace%',
                        'username'  => '%phpcr_user%',
                        'password'  => '%phpcr_pass%',
                    ),
                    'website' => array(
                        'backend' => array(
                            'type' => 'jackrabbit',
                            'url'  => '%magnolia_url%',
                        ),
                        'workspace' => 'website',
                        'username'  => '%magnolia_user%',
                        'password'  => '%magnolia_pass%',
                    ),
                    'dms' => array(
                        'backend' => array(
                            'type' => 'jackrabbit',
                            'url'  => '%magnolia_url%',
                        ),
                        'workspace' => 'dms',
                        'username'  => '%magnolia_user%',
                        'password'  => '%magnolia_pass%',
                    ),
                ),
            ),

            // enable the ODM layer
            'odm' => array(
                'auto_generate_proxy_classes' => '%kernel.debug%',
                'document_managers' => array(
                    'default' => array(
                        'session'  => 'default',
                        'mappings' => array(
                            'SandboxMainBundle' => null,
                            'CmfContentBundle'  => null,
                            'CmfMenuBundle'     => null,
                            'CmfRoutingBundle'  => null,
                        ),
                    ),
                    'website' => array(
                        'session'          => 'website',
                        'configuration_id' => 'sandbox_magnolia.odm_configuration',
                        'mappings'         => array(
                            'SandboxMagnoliaBundle' => null,
                        ),
                    ),
                    'dms' => array(
                        'session'          => 'dms',
                        'configuration_id' => 'sandbox_magnolia.odm_configuration',
                        'mappings'         => array(
                            'SandboxMagnoliaBundle' => null,
                        ),
                    ),
                ),
            ),
        ));

.. tip::

    This example also uses different configurations per repository (see the
    ``repository_id`` attribute).

.. _bundle-phpcr-odm-commands:

Services
--------

There are 3 main services provided by this bundle:

* ``doctrine_phpcr``- The ``ManagerRegistry`` instance with references to all
  sessions and document manager instances;
* ``doctrine_phpcr.default_session`` - The PHPCR session instance;
* ``doctrine_phpcr.odm.default_document_manager`` - The PHPCR-ODM document
  manager instance.

Events
------

You can tag services to listen to Doctrine PHPCR-ODM events. It works the same
way as for Doctrine ORM. The only differences are:

* use the tag name ``doctrine_phpcr.event_listener`` resp.
  ``doctrine_phpcr.event_subscriber`` instead of ``doctrine.event_listener``.
* expect the argument to be of class
* ``Doctrine\ODM\PHPCR\Event\LifecycleEventArgs`` rather than in the ORM
  namespace. (this is subject to change, as doctrine commons 2.4 provides a
  common class for this event).

You can register for the events as described in
`the PHPCR-ODM documentation`_. Or you can tag your services as event
listeners resp. event subscribers.

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_search.listener.search:
                class: Acme\SearchBundle\EventListener\SearchIndexer
                    tags:
                        - { name: doctrine_phpcr.event_listener, event: postPersist }

            acme_search.subscriber.fancy:
                class: Acme\SearchBundle\EventSubscriber\MySubscriber
                    tags:
                        - { name: doctrine_phpcr.event_subscriber }

    .. code-block:: xml

        <!-- src/Acme/SearchBundle/Resources/config/services.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <services>
                <service id="acme_search.listener.search" class="Acme\SearchBundle\EventListener\SearchIndexer">
                    <tag name="doctrine_phpcr.event_listener" event="postPersist" />
                </service>
                <service id="acme_search.subscriber.fancy" class="Acme\SearchBundle\EventSubscriber\MySubscriber">
                    <tag name="doctrine_phpcr.event_subscriber" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Reference;

        $container
            ->register(
                'acme_search.listener.search',
                'Acme\SearchBundle\EventListener\SearchIndexer'
            )
            ->addTag('doctrine_phpcr.event_listener', array(
                'event' => 'postPersist',
            ))
        ;

        $container
            ->register(
                'acme_search.subscriber.fancy',
                'Acme\SearchBundle\EventSubscriber\FancySubscriber'
            )
            ->addTag('doctrine_phpcr.event_subscriber', array(
                'event' => 'postPersist',
            ))
        ;

.. tip::

    Doctrine event subscribers (both ORM and PHPCR-ODM) can not return a
    flexible array of methods to call like the `Symfony event subscriber`_ can
    do. Doctrine event subscribers must return a simple array of the event
    names they subscribe to. Doctrine will then expect methods on the
    subscriber with the names of the subscribed events, just as when using an
    event listener.

More information with PHP code examples for the doctrine event system
integration is in this `Symfony cookbook entry`_.

Constraint Validator
--------------------

The bundle provides a ``ValidPhpcrOdm`` constraint validator you can use to
check if your document ``Id`` or ``Nodename`` and ``Parent`` fields are
correct.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            constraints:
                - Doctrine\Bundle\PHPCRBundle\Validator\Constraints\ValidPhpcrOdm

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php

        // ...
        use Doctrine\Bundle\PHPCRBundle\Validator\Constraints as OdmAssert;

        /**
         * @OdmAssert\ValidPhpcrOdm
         */
        class Author
        {
            // ...
        }

    .. code-block:: xml

        <!-- Resources/config/validation.xml -->
        <?xml version="1.0" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping
                http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route">
                <constraint name="Doctrine\Bundle\PHPCRBundle\Validator\Constraints\ValidPhpcrOdm" />
            </class>

        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Doctrine\Bundle\PHPCRBundle\Validator\Constraints as OdmAssert;

        /**
         * @OdmAssert\ValidPhpcrOdm
         */
        class Author
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new OdmAssert\ValidPhpcrOdm());
            }
        }

.. index::
    single: Form Types; DoctrinePHPCRBundle

Form Types
----------

The bundle provides a couple of handy form types for PHPCR and PHPCR-ODM
specific cases, along with form type guessers.

phpcr_document
~~~~~~~~~~~~~~

This form type is suitable to edit associations of PHPCR-ODM documents. It
works for ReferenceOne, ReferenceMany and Referrers but also for
ParentDocument associations. Make sure to set the ``multiple`` option
for ReferenceMany and Referrers, and to not set it for the others.

.. note::

    While Children is also an association, it makes no sense to edit it
    with this form type. Children are automatically attached to its parent.
    MixedReferrers could be shown as a ``disabled`` but never edited,
    because this association is immutable.

This form type is equivalent to the ``entity`` form type provided by Symfony
for Doctrine ORM. It has the same options as the ``entity`` type, including
that the option for the document manager is called ``em``.

A simple example of using the ``phpcr_document`` form type looks as follows::

    $form
        ->add(
            'speakers',
            'phpcr_document',
            array(
                'property' => 'title',
                'class'    => 'Acme\DemoBundle\Document\TargetClass',
                'multiple' => true,
            )
        )
    ;

This will produce a multiple choice select field with the value of
``getTitle`` called on each instance of ``TargetClass`` found in the
content repository. Alternatively, you can set the ``choices`` option
to a list of allowed managed documents. Please refer to the
`Symfony documentation on the entity form type`_ for more details,
including how you can configure a query.

If you are using SonataDoctrinePHPCRAdminBundle_, you might want to look into
``sonata_type_collection``. This form type allows to edit related
documents (references as well as children) inline and also to create
and remove them on the fly.

phpcr_odm_reference_collection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. caution::

    This form type was deprecated in DoctrinePHPCRBundle 1.1 and will be
    removed in DoctrinePHPCRBundle 1.2. You should use the `phpcr_document`_
    type instead, which can do the same but better.

This form type handles editing ``ReferenceMany`` collections on PHPCR-ODM
documents.  It is a choice field with an added ``referenced_class`` required
option that specifies the class of the referenced target document.

To use this form type, you also need to specify the list of possible reference
targets as an array of PHPCR-ODM ids or PHPCR paths.

The minimal code required to use this type looks as follows::

    $dataArr = array(
        '/some/phpcr/path/item_1' => 'first item',
        '/some/phpcr/path/item_2' => 'second item',
    );

    $formMapper
        ->with('form.group_general')
            ->add('myCollection', 'phpcr_odm_reference_collection', array(
                'choices'   => $dataArr,
                'referenced_class'  => 'Class\Of\My\Referenced\Documents',
            ))
        ->end();

.. tip::

    When building an admin interface with the SonataDoctrinePHPCRAdminBundle_
    there is also the ``sonata_type_model`` that is more powerful, allowing to
    add to the referenced documents on the fly. Unfortunately it is
    `currently broken`_.

phpcr_reference
~~~~~~~~~~~~~~~

The ``phpcr_reference`` represents a PHPCR Property of type REFERENCE or
WEAKREFERENCE within a form.  The input will be rendered as a text field
containing either the PATH or the UUID as per the configuration. The form will
resolve the path or id back to a PHPCR node to set the reference.

This type extends the ``text`` form type. It adds an option
``transformer_type`` that can be set to either ``path`` or ``uuid``.

.. _phpcr-odm-repository-initializers:

Repository Initializers
-----------------------

The Initializer is the PHPCR equivalent of the ORM schema tools. It
is used to let bundles register PHPCR node types and to create required base
paths in the repository. Initializers have to implement
``Doctrine\Bundle\PHPCRBundle\Initializer``. If you don't need any special
logic, you can simply configure the ``GenericInitializer`` as service and don't
need to write any code. The generic initializer expects a name to identify
the initializer, an array of base paths it will create if they do not exist
and an optional string defining namespaces and primary / mixin node types in
the CND language.

A service to use the generic initializer looks like this:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ContentBundle/Resources/config/services.yml
        acme.phpcr.initializer:
            class: Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer
            arguments:
                - AcmeContentBundle Basepaths
                - { "%acme.content_basepath%", "%acme.menu_basepath%" }
                - { "%acme.cnd%" }
            tags:
                - { name: "doctrine_phpcr.initializer" }

    .. code-block:: xml

        <!-- src/Acme/ContentBundle/Resources/config/services.xml -->
        <service id="acme.phpcr.initializer" class="Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer">
            <argument>AcmeContentBundle Basepaths</argument>
            <argument type="collection">
                <argument>%acme.content_basepath%</argument>
                <argument>%acme.menu_basepath%</argument>
            </argument>
            <argument>%acme.cnd%</argument>
            <tag name="doctrine_phpcr.initializer"/>
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition

        // ...

        $definition = new Definition(
            'Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer',
            array(
                'AcmeContentBundle Basepaths',
                array('%acme.content_basepath%', '%acme.menu_basepath%'),
                '%acme.cnd%',
            )
        ));
        $definition->addTag('doctrine_phpcr.initializer');
        $container->setDefinition('acme.phpcr.initializer', $definition);

You can execute your initializers using the following command:

.. code-block:: bash

    $ php app/console doctrine:phpcr:repository:init

.. versionadded:: 1.1

    Since DoctrinePHPCRBundle 1.1 the load data fixtures command will automatically
    execute the initializers after purging the database.

Fixture Loading
---------------

To use the ``doctrine:phpcr:fixtures:load`` command, you additionally need to
install the `DoctrineFixturesBundle`_ which brings the
`Doctrine data-fixtures`_ into Symfony2.

Fixtures work the same way they work for Doctrine ORM. You write fixture
classes implementing ``Doctrine\Common\DataFixtures\FixtureInterface``. If you
place them in ``<Bundle>\DataFixtures\PHPCR``, they will be auto detected if you
specify no path to the fixture loading command.

A simple example fixture class looks like this::

    // src/Acme/MainBundle/DataFixtures/PHPCR/LoadPageData.php
    namespace Acme\MainBundle\DataFixtures\PHPCR;

    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\Common\DataFixtures\FixtureInterface;

    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $manager)
        {
            // ... create and persist your data here
        }
    }

For more on fixtures, see the `documentation of the DoctrineFixturesBundle`_.

Migration Loading
-----------------

The DoctrinePHPCRBundle also ships with a simple command to run migration
scripts. Migrations should implement the
``Doctrine\Bundle\PHPCRBundle\Migrator\MigratorInterface`` and registered as a
service with a ``doctrine_phpcr.migrator`` tag contains an ``alias`` attribute
uniquely identifying the migrator. There is an optional
``Doctrine\Bundle\PHPCRBundle\Migrator\AbstractMigrator`` class to use as a
basis:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ContentBundle/Resources/config/services.yml
        acme.demo.migration.foo:
            class: Acme\DemoBundle\Migration\Foo
            arguments:
                - { "%acme.content_basepath%", "%acme.menu_basepath%" }
            tags:
                - { name: "doctrine_phpcr.migrator", alias: "acme.demo.migration.foo" }

    .. code-block:: xml

        <!-- src/Acme/ContentBundle/Resources/config/services.xml -->
        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service id="acme.demo.migration.foo" class="Acme\DemoBundle\Migration\Foo">
                <argument type="collection">
                    <argument>%acme.content_basepath%</argument>
                    <argument>%acme.menu_basepath%</argument>
                </argument>

                <tag name="doctrine_phpcr.migrator" alias="acme.demo.migration.foo"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition

        // ...
        $definition = new Definition('Acme\DemoBundle\Migration\Foo', array(
            array(
                '%acme.content_basepath%',
                '%acme.menu_basepath%',
            ),
        )));
        $definition->addTag('doctrine_phpcr.migrator', array('alias' => 'acme.demo.migration.foo'));

        $container->setDefinition('acme.demo.migration.foo', $definition);

To find out available migrations run:

.. code-block:: bash

    $ php app/console doctrine:phpcr:migrator

Then pass in the name of the migrator to run it, optionally passing in an
``--identifier``, ``--depth`` or ``--session`` argument. The later argument
determines which session name to set on the migrator, while the first two
arguments will simply be passed to the ``migrate()`` method. You can find an
example migrator in the SimpleCmsBundle.

Doctrine PHPCR Commands
-----------------------

All commands about PHPCR are prefixed with ``doctrine:phpcr`` and you can use
the --session argument to use a non-default session if you configured several
PHPCR sessions.

Some of these commands are specific to a backend or to the ODM. Those commands
will only be available if such a backend is configured.

Use ``app/console help <command>`` to see all options each of the commands
has.

* **doctrine:phpcr:document:migrate-class**: Command to migrate document classes;
* **doctrine:phpcr:fixtures:load**: Load data fixtures to your PHPCR database;
* **doctrine:phpcr:init:dbal**: Prepare the database for Jackalope Doctrine-Dbal;
* **doctrine:phpcr:jackrabbit**: Start and stop the Jackrabbit server;
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
    to install the `DoctrineFixturesBundle`_ and its dependencies. See that
    documentation article for how to use fixtures.

Some Example Command Runs
~~~~~~~~~~~~~~~~~~~~~~~~~

Running `SQL2 queries`_ against the repository:

.. code-block:: bash

    $ php app/console doctrine:phpcr:workspace:query "SELECT title FROM [nt:unstructured] WHERE NAME() = 'home'"

Dumping nodes under ``/cms/simple`` including their properties:

.. code-block:: bash

    $ php app/console doctrine:phpcr:node:dump /cms/simple --props

.. _`DoctrinePHPCRBundle`: https://github.com/doctrine/DoctrinePHPCRBundle
.. _`Jackalope`: http://jackalope.github.com/
.. _`Midgard2`: http://midgard-project.org/phpcr/
.. _`the PHPCR website`: http://phpcr.github.com/
.. _`PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/
.. _`Symfony2 Doctrine documentation`: http://symfony.com/doc/current/book/doctrine.html
.. _`midgard2 PHP extension`: http://midgard-project.org/midgard2/#download
.. _`the midgard/phpcr package`: http://packagist.org/packages/midgard/phpcr
.. _`the getting started document`: http://midgard-project.org/phpcr/#getting_started
.. _`official Midgard PHPCR documentation`: http://midgard-project.org/phpcr/
.. _`metadata caching`: http://symfony.com/doc/master/reference/configuration/doctrine.html
.. _`PHPCR-ODM documentation on Multilanguage`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html
.. _`the PHPCR-ODM documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/events.html
.. _`Symfony event subscriber`: http://symfony.com/doc/master/components/event_dispatcher/introduction.html#using-event-subscribers
.. _`Symfony cookbook entry`: http://symfony.com/doc/current/cookbook/doctrine/event_listeners_subscribers.html
.. _`Symfony documentation on the entity form type`: http://symfony.com/doc/current/reference/forms/types/entity.html
.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
.. _`currently broken`: https://github.com/sonata-project/SonataDoctrineORMAdminBundle/issues/145
.. _`DoctrineMigrationsBundle`: http://symfony.com/doc/current/bundles/DoctrineMigrationsBundle/index.html
.. _`DoctrineFixturesBundle`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`Doctrine data-fixtures`: https://github.com/doctrine/data-fixtures
.. _`documentation of the DoctrineFixturesBundle`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`SQL2 queries`: http://www.h2database.com/jcr/grammar.html
.. _`DoctrineCacheBundle`: https://github.com/doctrine/DoctrineCacheBundle/
.. _`last modified listener cookbook entry`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/cookbook/last-modified.html
