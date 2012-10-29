Installing and configuring Doctrine PHPCR-ODM
=============================================
The goal of this tutorial is to install and configure Doctrine PHPCR-ODM.

For more details see the `official PHPCR-ODM documentation <http://www.doctrine-project.org/projects/phpcr-odm.html>`_
Some additional information can be found on the
`DoctrinePhpcrBundle github.com project <https://github.com/doctrine/DoctrinePHPCRBundle>`_
which for the most part mimics the standard `DoctrineBundle <https://github.com/doctrine/DoctrineBundle>`_.

Finally for information about PHPCR see the `official PHPCR website <http://phpcr.github.com>`_.

.. index:: PHPCR, ODM

Preconditions
-------------
- php >= 5.3
- libxml version >= 2.7.0 (due to a bug in libxml http://bugs.php.net/bug.php?id=36501)
- phpunit >= 3.6 (if you want to run the tests)
- Symfony2.1 (currently master)

Installation
------------

Choosing a content repository
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first thing to decide is what content repository to use. A content repository is essentially
a database that will be responsible for storing all the content you want to persist. It provides
and API that is optimized for the needs of a CMS (tree structures, references, versioning, full
text search etc.). While every content repository can have very different requirements and
performance characteristics, the API is the same for all of them.

Furthermore since the API defines an export/import format, you can always switch to a different
content repository implementation later on.

These are the available choices:

* Jackalope with Jackrabbit (Jackrabbit requires Java, it can persist into the file system, a database etc.)
* Jackalope with Doctrine DBAL (requires an RDBMS like MySQL, PostgreSQL or SQLite)
* Midgard (requires an RDBMS like MySQL, PostgreSQL or SQLite)

Depending on your choice you can omit certain steps in the following documentation.

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file

.. code-block:: javascript

    "require": {
        ...
        "jackalope/jackalope-jackrabbit": "1.0.*",
        "jackalope/jackalope-doctrine-dbal": "dev-master",
        "midgard/phpcr": "dev-master",
        "doctrine/phpcr-bundle": "1.0.*",
        "doctrine/phpcr-odm": "1.0.*"
    }

.. Note::

    If you are also using Doctrine ORM, make sure to use ``"doctrine/orm": "2.3.*"``,
    otherwise composer can't resolve the dependencies as Doctrine PHPCR-ODM depends on
    the newer 2.3 Doctrine Commons. (Symfony 2.1 standard edition uses "2.2.*".

And then run

.. code-block:: bash

    php composer.phar update


Register annotations
~~~~~~~~~~~~~~~~~~~~
Add file to annotation registry in ``app/autoload.php`` for the ODM annotations
right after the last ``AnnotationRegistry::registerFile`` line

.. code-block:: php

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the
``registerBundle`` method

.. code-block:: php

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

Doctrine PHPCR-ODM
~~~~~~~~~~~~~~~~~~

Basic configuration, add to your application configuration

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    # Jackalope Jackrabbit
                    type: jackrabbit
                    url: http://localhost:8080/server/
                    # Jackalope Doctrine DBAL (make sure to also configure the DoctrineBundle accordingly)
                    type: doctrinedbal
                    connection: doctrine.dbal.default_connection
                    # Midgard
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

More information on configuring this bundle can be found
`here <https://github.com/doctrine/DoctrinePHPCRBundle#readme>`_.

Setting up the content repository
---------------------------------

.. _tutorials-installing-phpcr-jackrabbit:

Jackalope Jackrabbit
~~~~~~~~~~~~~~~~~~~~


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

As we are using Jackalope as our PHPCR implementation we could also chose other storage backends
like relational databases but for this tutorial we're going to use Jackrabbit.


.. _tutorials-installing-phpcr-doctrinedbal:

Jackalope Doctrine DBAL
~~~~~~~~~~~~~~~~~~~~~~~

.. index:: Doctrine, DBAL, RDBMS

In order to setup the database, run the following commands to create the
database and setup a default schema

.. code-block:: bash

    app/console doctrine:database:create
    app/console doctrine:phpcr:init:dbal

For more information of how to configure Doctrine DBAL with Symfony2 see the
`Doctrine chapter in the Symfony2 documentation <http://symfony.com/doc/current/book/doctrine.html>`_
and the explanations in the :ref:`PHPCR reference chapter <reference-phpcr-doctrinedbal>`).

.. _tutorials-installing-phpcr-midgard:

Midgard
~~~~~~~

.. index:: Midgard, RDBMS

Midgard is a C extension that implements the PHPCR API on top of a standard RDBMS.

See `official Midgard PHPCR documentation <http://midgard-project.org/phpcr/>`_

Registering system node types
-----------------------------
PHPCR-ODM uses a `custom node type <https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged>`_
to track meta information without interfering with your content. There is a command that makes it trivial to
register this type and the PHPCR namespace

.. code-block:: bash

    php app/console doctrine:phpcr:register-system-node-types
