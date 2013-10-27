.. index::
    single: PHPCR-ODM

Getting Started with PHPCR-ODM
==============================

This article will demonstrate the various ways in which you can start a
Symfony project using the PHPCR-ODM.

The "Quick Start" section will demonstrate how to create a new Symfony project
and configure it to use PHPCR-ODM with the **Doctrine DBAL** backend with
MySQL.

The "Alternative Configurations" will show you other ways of configuring the
PHPCR-ODM backend.

Quick Start
-----------

Create a new Symfony project with composer based on the standard edition:

.. code-block:: bash

    $ php composer.phar create-project symfony/framework-standard-edition path/

Add the required packages to ``composer.json``:

.. code-block:: javascript

    ...
    "require": {
        ...
        "doctrine/phpcr-bundle": "1.0.0",
        "doctrine/doctrine-bundle": "1.2.*",
        "doctrine/phpcr-odm": "1.0.0",
        "jackalope/jackalope-doctrine-dbal": "1.0.0",
    },

Add the DoctrinePHPCRBundle to the AppKernel::

    // app/AppKernel.php
    // ...

    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),
            );

            // ...
        }
    }

Register the PHPCR-ODM annotations in ``app/autoload.php``::

    // app/autoload.php
    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');

Modify ``parameters.yml``, adding the required PHPCR-ODM settings:

.. code-block:: yaml

    // app/config/parameters.yml.dist
    parameters:
        # ...
        phpcr_backend:
            type: doctrinedbal
            connection: default
        phpcr_workspace: default
        phpcr_user: admin
        phpcr_pass: admin 

Add the Doctrine PHPCR configuration to the main application configuration:

.. code-block:: yaml

    # ...
    doctrine_phpcr:
       # configure the PHPCR session
       session:
           backend: %phpcr_backend%
           workspace: %phpcr_workspace%
           username: %phpcr_user%
           password: %phpcr_pass%
       # enable the ODM layer
       odm:
           auto_mapping: true
           auto_generate_proxy_classes: %kernel.debug%   



PHPCR and Implementations
-------------------------

PHPCR is a PHP implementation of JCR and provides a standard interface for
interactive with content repositories. There exist currently only two
stable implementations:

* **Doctrine DBAL**: Uses a standard MySQL database as a content repository;
* **Jackrabbit**: A java based content repository server.

Alternative Configurations
--------------------------

Jackrabbit
~~~~~~~~~~

Mongo DB
~~~~~~~~
