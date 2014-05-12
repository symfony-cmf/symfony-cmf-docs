.. index::
    single: Installing Doctrine PHPCR-ODM

Installing and Configuring Doctrine PHPCR-ODM
=============================================

.. include:: _outdate-caution.rst.inc

The Symfony2 CMF needs somewhere to store the content. Many of the bundles
provide documents and mappings for the PHP Content Repository - Object
Document Mapper (PHPCR-ODM), and the documentation is currently based around
using this. However, it should also be possible to use any other form of
content storage, such as another ORM/ODM or MongoDB.

The goal of this tutorial is to install and configure Doctrine PHPCR-ODM,
ready for you to get started with the CMF.

For more details see the `full PHPCR-ODM documentation`_. Some additional
information can be found in the :doc:`../bundles/phpcr_odm` reference, which
for the most part mimics the standard `DoctrineBundle`_.

Finally for information about PHPCR see the `official PHPCR website`_.

.. tip::

    If you just want to use PHPCR but not the PHPCR-ODM, you can skip the step
    about registering annotations and the part of the configuration section
    starting with ``odm``.

.. index:: PHPCR, ODM

.. _cookbook-phpcr-odm-requirements:

Requirements
------------

* Symfony2 (version 2.1 or newer)
* phpunit >= 3.6 (if you want to run the tests)
* When using **jackalope-jackrabbit**: Java, Apache Jackalope and libxml
  version >= 2.7.0 (due to a `bug in libxml`_)
* When using **jackalope-doctrine-dbal with MySQL**: MySQL >= 5.1.5
  (as you need the xml function ``ExtractValue``)

Installation
------------

Choosing a Content Repository
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first thing to decide is what content repository to use. A content
repository is essentially a database that will be responsible for storing all
the content you want to persist. It provides an API that is optimized for the
needs of a CMS (tree structures, references, versioning, full text search
etc.). While every content repository can have very different requirements and
performance characteristics, the API is the same for all of them.

Furthermore, since the API defines an export/import format, you can always
switch to a different content repository implementation later on.

These are the available choices:

* **Jackalope with Jackrabbit** (Jackrabbit requires Java, it can persist into
  the file system, a database etc.)
* **Jackalope with Doctrine DBAL** (requires an RDBMS like MySQL, PostgreSQL
  or SQLite)
* **Midgard** (requires the midgard2 PHP extension and an RDBMS like MySQL,
  PostgreSQL or SQLite)

The following documentation includes examples for all of the above options.

.. tip::

    If you are just getting started with the CMF, it is best to choose a
    content repository based on a storage engine that you are already familiar
    with. For example, **Jackalope with Doctrine DBAL** will work with your
    existing RDBMS and does not require you to install Java or the midgard2
    PHP extension. Once you have a working application it should be easy to
    switch to another option.

Download the Bundles
~~~~~~~~~~~~~~~~~~~~

Add the following to your ``composer.json`` file, depending on your chosen
content repository.

**Jackalope with Jackrabbit**

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "jackalope/jackalope-jackrabbit": "1.0.*",
        "doctrine/phpcr-bundle": "1.0.*",
        "doctrine/phpcr-odm": "1.0.*"
    }

**Jackalope with Doctrine DBAL**

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "jackalope/jackalope-doctrine-dbal": "1.0.*",
        "doctrine/phpcr-bundle": "1.0.*",
        "doctrine/phpcr-odm": "1.0.*"
    }

**Midgard**

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "midgard/phpcr": "dev-master",
        "doctrine/phpcr-bundle": "1.0.*",
        "doctrine/phpcr-odm": "1.0.*"
    }

.. note::

    For all of the above, if you are also using Doctrine ORM, make sure to use
    ``"doctrine/orm": "2.3.*"``, otherwise composer can't resolve the
    dependencies as Doctrine PHPCR-ODM depends on the newer 2.3 Doctrine
    Commons. (Symfony2.1 standard edition uses ``2.2.*``.)

To install the above dependencies, run:

.. code-block:: bash

    $ php composer.phar update

Register Annotations
~~~~~~~~~~~~~~~~~~~~

PHPCR-ODM uses annotations and these need to be registered in your
``app/autoload.php`` file. Add the following line, immediately after the last
``AnnotationRegistry::registerFile`` line::

    // app/autoload.php

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Initialize Bundles
~~~~~~~~~~~~~~~~~~

Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the
``registerBundle`` method::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // Doctrine PHPCR
            new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),

        );

        // ...
    }

Configuration
-------------

Next step is to configure the bundles.

PHPCR Session
~~~~~~~~~~~~~

Basic configuration for each content repository is shown below; add the
appropriate lines to your ``app/config/config.yml``. More information on
configuring this bundle can be found in the reference chapter
:doc:`../bundles/phpcr_odm`.

The workspace, username and password parameters are for the PHPCR repository
and should not be confused with possible database credentials. They come from
your content repository setup. If you want to use a different workspace than
*default* you have to create it first in your repository.

If you want to use the PHPCR-ODM as well, please also see the next section.

**Jackalope with Jackrabbit**

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: jackrabbit
                    url: http://localhost:8080/server/
                workspace: default
                username: admin
                password: admin
            # odm configuration see below

**Jackalope with Doctrine DBAL**

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: doctrinedbal
                    # connection: default
                workspace: default
                username: admin
                password: admin
            # odm configuration see below

.. note::

    Make sure you also configure the main ``doctrine:`` section for your
    chosen RDBMS.  If you want to use a different than the default connection,
    configure it in the dbal section and specify it in the connection
    parameter. A typical example configuration is:

    .. code-block:: yaml

        doctrine:
            dbal:
                driver:   "%database_driver%"
                host:     "%database_host%"
                port:     "%database_port%"
                dbname:   "%database_name%"
                user:     "%database_user%"
                password: "%database_password%"
                charset:  UTF8

    See `Databases and Doctrine`_ for more information.

**Midgard**

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: midgard2
                    db_type: MySQL
                    db_name: midgard2_test
                    db_host: "0.0.0.0"
                    db_port: 3306
                    db_username: ""
                    db_password: ""
                    db_init: true
                    blobdir: /tmp/cmf-blobs
                workspace: default
                username: admin
                password: admin
            # odm configuration see below

Doctrine PHPCR-ODM
~~~~~~~~~~~~~~~~~~

Any of the above configurations will give you a valid PHPCR session. If you
want to use the Object-Document manager, you need to configure it as well. The
simplest is to set ``auto_mapping: true`` to make the PHPCR bundle recognize
documents in the ``<Bundle>/Document`` folder and look for mappings in
``<Bundle>/Resources/config/doctrine/<Document>.phpcr.xml`` resp. ``...yml``.
Otherwise you need to manually configure the mappings section. See the
:ref:`configuration reference of the PHPCR-ODM bundle <bundle-phpcr-odm-configuration>`
for details.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                # ...
            odm:
                auto_mapping: true

Setting up the Content Repository
---------------------------------

.. index:: Jackrabbit

.. _cookbook-installing-phpcr-jackrabbit:

**Jackalope Jackrabbit**

These are the steps necessary to install Apache Jackrabbit:

* Make sure you have Java Virtual Machine installed on your box. If not, you
  can grab one from here: http://www.java.com/en/download/manual.jsp
* Download the latest version from the `Jackrabbit Downloads page`_
* Run the server. Go to the folder where you downloaded the ``.jar`` file and
  launch it

.. code-block:: bash

    $ java -jar jackrabbit-standalone-*.jar

Going to ``http://localhost:8080/`` should now display a Apache Jackrabbit page.

More information about `running a Jackrabbit server`_ can be found on the
Jackalope wiki.

.. index:: Doctrine, DBAL, RDBMS

.. _cookbook-installing-phpcr-doctrinedbal:

**Jackalope Doctrine DBAL**

Run the following commands to create the database and set up a default schema:

.. code-block:: bash

    $ php app/console doctrine:database:create
    $ php app/console doctrine:phpcr:init:dbal

For more information on how to configure Doctrine DBAL with Symfony2, see the
"`Databases and Doctrine`_" and the explanations in the
:ref:`reference of the PHPCR-ODM bundle <bundle-phpcr-odm-doctrinedbal>`.

.. index:: Midgard, RDBMS

.. _cookbook-installing-phpcr-midgard:

**Midgard**

Midgard is a C extension that implements the PHPCR API on top of a standard RDBMS.

See the `official Midgard PHPCR documentation`_.

Registering System Node Types
-----------------------------

PHPCR-ODM uses a `custom node type`_ to track meta information without
interfering with your content. There is a command that makes it trivial to
register this type and the PHPCR namespace, as well as all base paths of
bundles:

.. code-block:: bash

    $ php app/console doctrine:phpcr:repository:init

Using the ValidPhpcrOdm Constraint Validator
--------------------------------------------

The bundle provides a ``ValidPhpcrOdm`` constraint validator you can use to
check if your document ``Id`` or ``Nodename`` and ``Parent`` fields are
correct::

    <?php

    namespace Acme\DemoBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCRODM;
    use Doctrine\Bundle\PHPCRBundle\Validator\Constraints as Assert;

    /**
     * @PHPCRODM\Document
     * @Assert\ValidPhpcrOdm
     */
    class MyDocument
    {
        /** @PHPCRODM\Id(strategy="parent") */
        protected $id;

        /** @PHPCRODM\Nodename */
        protected $name;

        /** @PHPCRODM\ParentDocument */
        protected $parent;

        // ...
    }

.. _`bug in libxml`: http://bugs.php.net/bug.php?id=36501)
.. _`full PHPCR-ODM documentation`: http://www.doctrine-project.org/projects/phpcr-odm.html
.. _`DoctrineBundle`: https://github.com/doctrine/DoctrineBundle
.. _`official PHPCR website`: http://phpcr.github.com
.. _`Databases and Doctrine`: http://symfony.com/doc/current/book/doctrine.html
.. _`Jackrabbit Downloads page`: http://jackrabbit.apache.org/downloads.html
.. _`running a Jackrabbit server`: https://github.com/jackalope/jackalope/wiki/Running-a-jackrabbit-server
.. _`official Midgard PHPCR documentation`: http://midgard-project.org/phpcr/
.. _`custom node type`: https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged
