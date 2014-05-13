.. index::
    single: Testing; Components
    single: Testing

Testing
=======

The Testing component is an **internal tool** for testing Symfony CMF bundles.
It provides a way to easily bootstrap a consistent functional test environment.

Configuration
-------------

Composer
~~~~~~~~

Add the following dependency to the ``require-dev`` section of ``composer.json``:

.. code-block:: javascript

    "require-dev": {
        "symfony-cmf/testing": "1.1.*"
    },

.. note::

    The Testing component does not automatically include the SonataAdminBundle. You
    will need to manually add this dependency if required.

PHPUnit
~~~~~~~

The following file should be placed in the root directory of your bundle and
named ``phpunit.xml.dist``:

.. code-block:: xml

    <?xml version="1.0" encoding="UTF-8"?>
    <phpunit
        colors="true"
        bootstrap="vendor/symfony-cmf/testing/bootstrap/bootstrap.php"
    >

        <testsuites>
            <testsuite name="Symfony <your bundle>Bundle Test Suite">
                <directory>./Tests</directory>
            </testsuite>
        </testsuites>

        <filter>
            <whitelist addUncoveredFilesFromWhitelist="true">
                <directory>.</directory>
                <exclude>
                    <directory>Resources/</directory>
                    <directory>Tests/</directory>
                    <directory>vendor/</directory>
                </exclude>
            </whitelist>
        </filter>

        <php>
            <server name="KERNEL_DIR" value="Tests/Resources/app" />
        </php>

    </phpunit>

AppKernel
~~~~~~~~~

The ``AppKernel`` should be placed in the ``./Tests/Resources/app`` folder.

Below is the minimal ``AppKernel.php``::

    <?php

    use Symfony\Cmf\Component\Testing\HttpKernel\TestKernel;
    use Symfony\Component\Config\Loader\LoaderInterface;

    class AppKernel extends TestKernel
    {
        public function configure()
        {
            $this->requireBundleSets(array(
                'default',
            ));

            $this->addBundles(array(
                new \Symfony\Cmf\Bundle\MyBundle\CmfMyBundle(),
            ));
        }

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/config.php');
        }
    }

Use ``$this->requireBundleSets('bundle_set_name')`` to include pre-configured
sets of bundles:

* **default**: Symfony's FrameworkBundle, TwigBundle and MonologBundle;
* **phpcr_odm**: Doctrines DoctrineBundle and DoctrinePHPCRBundle;
* **sonata_admin**: Sonata AdminBundle, BlockBundle and SonataDoctrinePHPCRAdminBundle.

For any other bundle requirements simply use ``$this->addBundles(array())`` as in
the example above.

Git
~~~

Place the following ``.gitignore`` file in your root directory:

.. code-block:: text

    Tests/Resources/app/cache
    Tests/Resources/app/logs
    composer.lock
    vendor

Travis
~~~~~~

The following file should be named ``.travis.yml`` (note the leading ".") and placed
in the root directory of your bundle:

.. code-block:: yaml

    language: php

    php:
      - 5.3
      - 5.4
      - 5.5

    env:
      - SYMFONY_VERSION=2.2.*
      - SYMFONY_VERSION=2.3.*
      - SYMFONY_VERSION=dev-master

    before_script:
      - composer require symfony/symfony:${SYMFONY_VERSION}
      - vendor/symfony-cmf/testing/bin/travis/phpcr_odm_doctrine_dbal.sh

    script: phpunit --coverage-text

    notifications:
      irc: "irc.freenode.org#symfony-cmf"
      email: "symfony-cmf-devs@googlegroups.com"

    matrix:
      allow_failures:
        - env: SYMFONY_VERSION=dev-master

Implementing the Component
--------------------------

You should try and build a working application for testing your bundle. The
application can be accessed using the `server` command detailed in this
document.

Test Types
~~~~~~~~~~

* **Unit** - The scope of a unit test should be limited testing a single class
  instance. All other dependencies should be mocked;
* **Functional** - Functional tests will test a *single service* as
  retrieved from the dependency injection container;
* **Web** - Web test cases are the most holistic tests. They use the browser
  kit to make web requests on the kernel, testing the whole stack.

.. _testing_test_file_organization:

Test File Organization
~~~~~~~~~~~~~~~~~~~~~~

Test files and tests should be organized as follows:

.. code-block:: text

    ./Tests/
        ./Unit
            ./Full/Namespace/<test>Test.php
            ./Document/BlogTest.php
            ./Document/PostTest.php
            [...]
        ./Functional
            ./MyService/SomeServiceTest.php
            [...]
        ./WebTest
            ./Admin/SomeAdminTest.php
            ./Controller/MyControllerTest.php
        ./Resources
            ./app
                ./AppKernel.php
                ./config/
                    ./config.php
            ./DataFixtures/
                ./Phpcr/
                     ./LoadFoobarFixtures.php
            ./Document/
                Foobar.php

Custom Documents
~~~~~~~~~~~~~~~~

The Testing component will automatically include PHPCR-ODM documents that are placed in
``Tests/Resources/Document``.

Configuration
~~~~~~~~~~~~~

The Testing component includes some pre-defined configurations to get things
going with a minimum of effort and repetition.

To implement the default configurations create the following PHP file::

    // Tests/Resources/app/config/config.php
    $loader->import(CMF_TEST_CONFIG_DIR.'/default.php');
    $loader->import(__DIR__.'/mybundleconfig.yml');

Here you include the Testing component's **default** configuration, which will
get everything up-and-running. You can then optionally import configurations
specific to your bundle.

The available default configurations are as follows, and correspond to the bundle sets
above:

* **default.php**: framework, doctrine, security;
* **sonata_admin.php**: sonata_admin, sonata_block;
* **phpcr-odm.php**: doctrine_phpcr.

Note that each must be prefixed with the ``CMF_TEST_CONFIG_DIR`` constant.

Routing Configuration
~~~~~~~~~~~~~~~~~~~~~

You must include a ``routing.php`` file in the same directory as the
configuration above::

    <?php

    use Symfony\Component\Routing\RouteCollection;

    $collection = new RouteCollection();
    $collection->addCollection(
        $loader->import(CMF_TEST_CONFIG_DIR.'/routing/sonata_routing.yml')
    );
    $collection->addCollection(
        $loader->import(__DIR__.'/routing/my_test_routing.yml')
    );

    return $collection;

The following default routing configurations are available:

* **sonata_routing.yml**: sonata admin and dashboard.

The above files must be prefixed with ``CMF_TEST_CONFIG_DIR.'/routing/'`` as
in the example above.

The Console
~~~~~~~~~~~

The console for your test application can be accessed as follows:

.. code-block:: bash

    $ php vendor/symfony-cmf/testing/bin/console

Test Web Server
~~~~~~~~~~~~~~~

The Testing component provides a wrapper for the Symfony ``server:run`` command.

.. code-block:: bash

    $ php vendor/symfony-cmf/testing/bin/server

Which basically does the following:

.. code-block:: bash

    $ php vendor/symfony-cmf/testing/bin/console server:run \
        --router=vendor/symfony-cmf/testing/resources/web/router.php \
        --docroot=vendor/symfony-cmf/testing/resources/web

You can then access your test application in your browser at
``http://localhost:8000``.

.. note::

    The server and the requests default to using the ``phpcr`` configuration environment.
    Its possible to use a different environment by using ``--env``/``-e`` as arguments
    for the CLI command and a GET parameter ``env`` for web requests.

Publish assets in the directory named above using the testing console as
follows:

.. code-block:: bash

    $ php vendor/symfony-cmf/testing/bin/console assets:install \
        vendor/symfony-cmf/testing/resources/web

Initializing the Test Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Before running your (functional) tests you will need to initialize the test
environment (i.e. the database). You could do this manually, but it is easier
to do this the same way that *travis* will do it, as follows:

.. code-block:: bash

    $ ./vendor/symfony-cmf/testing/bin/travis/phpcr_odm_doctrine_dbal.sh

Functional and Web Testing
==========================

In general your functional tests should extend
``Symfony\Cmf\Component\Testing\Functional\BaseTestCase``. This class will
provide you with some helpers to make testing easier.

PHPCR-ODM
---------

Accessing the Document Manager
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Access as::

    $manager = $this->db('PHPCR');
    $documentManager = $this->db('PHPCR')->getOm();

    // create a test node /test
    $this->db('PHPCR')->createTestNode();

    // load fixtures
    $this->db('PHPCR')->loadFixtures(array(
        // ... fixture classes here
    ));

Support Files
~~~~~~~~~~~~~

The Testing component includes some basic documents which will automatically be
mapped by PHPCR-ODM:

* ``Symfony\Cmf\Testing\Document\Content``: Minimal referenceable content document.

ORM
---

The Testing component also provides support for testing the ORM.

Accessing the Entity Manager
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can access the Entity Manager and repository using::

    $entityManager = $this->db('ORM')->getOm();

Seperating PHPCR and ORM configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Most of the time, you cannot load both the PHPCR and ORM configuration. The
Testing component uses environments to seperate these configurations. First,
create a file ``config_phpcr.php`` which loads all needed configuration for
the PHPCR and a file ``config_orm.php`` which loads all needed configuration
for the ORM. After that, edit the
``AppKernel::registerContainerConfiguration`` to be::

    use Symfony\Cmf\Component\Testing\HttpKernel\TestKernel;
    use Symfony\Component\Config\Loader\LoaderInterface;

    class AppKernel extends TestKernel
    {
        // ...

        public function registerContainerConfiguration(LoaderInterface $loader)
        {
            $loader->load(__DIR__.'/config/config_'.$this->environment.'.php');
        }
    }

After that, all ORM tests should override the ``getKernelConfiguration``
method of the ``BaseTestCase`` to set the environment to ``orm``::

    // ...
    use Symfony\Cmf\Component\Testing\Functional\BaseTestCase;

    class MyOrmTest extends BaseTestCase
    {
        protected function getKernelConfiguration()
        {
            return array(
                'environment' => 'orm',
            );
        }
    }

.. tip::

    It's recommend to put this in a ``OrmTestCase`` class which lives inside
    your bundle and let all Orm tests extends this class.

.. note::

    This isn't needed for PHPCR, as the environment automatically defaults to
    ``phpcr``.

Running Commands using the ORM Configuration
............................................

Add ``--env=orm`` to the commands which need to ORM configuration as well as
``&env=orm`` to web requests requiring the ORM configuration.

Editing Travis Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If a bundle adds functional tests for ORM, the travis configuration needs to
be changed in order to correctly setup the environment.

Change the ``before_script`` section to:

.. code-block:: yaml

    before_script:
      - composer require symfony/framework-bundle:${SYMFONY_VERSION}
      - vendor/symfony-cmf/testing/bin/travis/phpcr_odm_doctrine_orm_dbal.sh

.. caution::

    This script executes the ``doctrine:schema:create`` command in the ``orm``
    environment. This will set up both the PHPCR and ORM schemas. To make it
    work, you have to include the PHPCR configuration in the ORM
    configuration.
