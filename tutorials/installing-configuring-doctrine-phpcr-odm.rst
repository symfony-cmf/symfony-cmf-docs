Installing and configuring Doctrine PHPCR-ODM
=============================================

The Symfony2 CMF needs somewhere to store the content. Many of the bundles provide documents and
mappings for the PHP Content Repository - Object Document Mapper (PHPCR-ODM), and the documentation
is currently based around using this. However, it should also be possible to use any other form of
content storage, such as another ORM/ODM or MongoDB.

The goal of this tutorial is to install and configure Doctrine PHPCR-ODM, ready for you to get
started with the CMF.

For more details see the `full PHPCR-ODM documentation <http://www.doctrine-project.org/projects/phpcr-odm.html>`_.
Some additional information can be found in the :doc:`../bundles/phpcr-odm` reference,
which for the most part mimics the standard `DoctrineBundle <https://github.com/doctrine/DoctrineBundle>`_.

Finally for information about PHPCR see the `official PHPCR website <http://phpcr.github.com>`_.

.. tip::

    If you just want to use PHPCR but not the PHPCR-ODM, you can skip the step about registering
    annotations and the part of the configuration section starting with `odm`.

.. index:: PHPCR, ODM

Preconditions
-------------
- php >= 5.3
- libxml version >= 2.7.0 (due to a bug in libxml http://bugs.php.net/bug.php?id=36501)
- phpunit >= 3.6 (if you want to run the tests)
- Symfony2 (version 2.1.x)

Installation
------------

Choosing a content repository
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The first thing to decide is what content repository to use. A content repository is essentially
a database that will be responsible for storing all the content you want to persist. It provides
an API that is optimized for the needs of a CMS (tree structures, references, versioning, full
text search etc.). While every content repository can have very different requirements and
performance characteristics, the API is the same for all of them.

Furthermore, since the API defines an export/import format, you can always switch to a different
content repository implementation later on.

These are the available choices:

* **Jackalope with Jackrabbit** (Jackrabbit requires Java, it can persist into the file system, a database etc.)
* **Jackalope with Doctrine DBAL** (requires an RDBMS like MySQL, PostgreSQL or SQLite)
* **Midgard** (requires the midgard2 PHP extension and an RDBMS like MySQL, PostgreSQL or SQLite)

The following documentation includes examples for all of the above options.

.. tip::

    If you are just getting started with the CMF, it is best to choose a content repository based
    on a storage engine that you are already familiar with. For example,
    **Jackalope with Doctrine DBAL** will work with your existing RDBMS and does not require you
    to install Java or the midgard2 PHP extension. Once you have a working application it should be
    easy to switch to another option.


Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file, depending on your chosen content repository.

| **Jackalope with Jackrabbit**

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "jackalope/jackalope-jackrabbit": "1.0.*",
        "doctrine/phpcr-bundle": "1.0.*",
        "doctrine/phpcr-odm": "1.0.*"
    }

| **Jackalope with Doctrine DBAL**

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "jackalope/jackalope-doctrine-dbal": "dev-master",
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
    ``"doctrine/orm": "2.3.*"``, otherwise composer can't resolve the dependencies as Doctrine
    PHPCR-ODM depends on the newer 2.3 Doctrine Commons. (Symfony2.1 standard edition uses "2.2.*".)

To install the above dependencies, run:

.. code-block:: bash

    php composer.phar update

Register annotations
~~~~~~~~~~~~~~~~~~~~
PHPCR-ODM uses annotations and these need to be registered in your ``app/autoload.php``
file. Add the following line, immediately after the last ``AnnotationRegistry::registerFile``
line:

.. code-block:: php

    // app/autoload.php

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the
``registerBundle`` method:

.. code-block:: php

    // app/AppKernel.php

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

Basic configuration for each content repository is shown below; add the appropriate lines to your
``app/config/config.yml``. More information on configuring this bundle can be found in the reference
chapter :doc:`../bundles/phpcr-odm`.

The workspace, username and password parameters are for the PHPCR repository and should not be
confused with possible database credentials. They come from your content repository setup. If you
want to use a different workspace than *default* you have to create it first in your repository.

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
                    connection: doctrine.dbal.default_connection
                workspace: default
                username: admin
                password: admin
            # odm configuration see below

.. note::

    Make sure you also configure the main ``doctrine:`` section for your chosen RDBMS.
    If you want to use a different than the default connection, configure it in the dbal
    section and specify it in the connection parameter. A typical example configuration is:

        doctrine:
            dbal:
                driver:   %database_driver%
                host:     %database_host%
                port:     %database_port%
                dbname:   %database_name%
                user:     %database_user%
                password: %database_password%
                charset:  UTF8

     See `Databases and Doctrine <http://symfony.com/doc/2.1/book/doctrine.html>`_ for more information.

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

Any of the above configurations will give you a valid PHPCR session. If you want to use the
Object-Document manager, you need to configure it as well. The simplest is to set
``auto_mapping: true`` to make the PHPCR bundle recognize documents in the ``<Bundle>/Document``
folder and look for mappings in ``<Bundle>/Resources/config/doctrine/<Document>.phpcr.xml`` resp.
``...yml``. Otherwise you need to manually configure the mappings section. See the
:ref:`PHPCR-ODM configuration reference<reference-phpcr-odm-configuration>` for details.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                ...
            odm:
                auto_mapping: true


Setting up the content repository
---------------------------------

.. _tutorials-installing-phpcr-jackrabbit:

**Jackalope Jackrabbit**

.. index:: Jackrabbit

These are the steps necessary to install Apache Jackrabbit:

- Make sure you have Java Virtual Machine installed on your box. If not, you can
  grab one from here: http://www.java.com/en/download/manual.jsp
- Download the latest version from the `Jackrabbit Downloads page <http://jackrabbit.apache.org/downloads.html>`_
- Run the server. Go to the folder where you downloaded the .jar file and launch it

.. code-block:: bash

    java -jar jackrabbit-standalone-*.jar

Going to http://localhost:8080/ should now display a Apache Jackrabbit page.

More information about `running a Jackrabbit server <https://github.com/jackalope/jackalope/wiki/Running-a-jackrabbit-server>`_
can be found on the Jackalope wiki.

.. _tutorials-installing-phpcr-doctrinedbal:

**Jackalope Doctrine DBAL**

.. index:: Doctrine, DBAL, RDBMS

Run the following commands to create the database and set up a default schema:

.. code-block:: bash

    app/console doctrine:database:create
    app/console doctrine:phpcr:init:dbal

For more information on how to configure Doctrine DBAL with Symfony2, see the
`Doctrine chapter in the Symfony2 documentation <http://symfony.com/doc/current/book/doctrine.html>`_
and the explanations in the :ref:`PHPCR reference chapter <reference-phpcr-doctrinedbal>`.

.. _tutorials-installing-phpcr-midgard:

**Midgard**

.. index:: Midgard, RDBMS

Midgard is a C extension that implements the PHPCR API on top of a standard RDBMS.

See the `official Midgard PHPCR documentation <http://midgard-project.org/phpcr/>`_.

Registering system node types
-----------------------------
PHPCR-ODM uses a `custom node type <https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged>`_
to track meta information without interfering with your content. There is a command that makes it
trivial to register this type and the PHPCR namespace:

.. code-block:: bash

    php app/console doctrine:phpcr:register-system-node-types

Using the ValidPhpcrOdm constraint validator
-----------------------------

The bundle provides a ``ValidPhpcrOdm`` constraint validator you can use to check if your document ``Id`` or ``Nodename`` and ``Parent`` fields are correct :

.. code-block:: php
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

        ...
