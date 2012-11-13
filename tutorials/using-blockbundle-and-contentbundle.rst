Using the BlockBundle and ContentBundle with PHPCR
==================================================

The goal of this tutorial is to demonstrate how the CMF
:doc:`../bundles/block`  and :doc:`../bundles/content` can be used as stand-alone
components, and to show how they fit into the PHPCR.

Once you are familiar with basic usage, the in-depth documentation of both bundles (linked above)
will help you to adapt these basic examples to serve more advanced use cases.

We will begin with using only BlockBundle, with content blocks linked directly into the PHPCR.
Having shown this, we will introduce the ContentBundle to show how it can represent content pages.

Although not a requirement for using BlockBundle or ContentBundle, this tutorial will also make use
of `DoctrineFixturesBundle <http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html>`_.
This is because it provides an easy way to load in some test content.


Preconditions
-------------
- `Installation of Symfony2 <http://symfony.com/doc/2.1/book/installation.html>`_ (2.1.x)
- :doc:`installing-configuring-doctrine-phpcr-odm`

.. note::

    This tutorial is based on using PHPCR-ODM set up with Jackalope, Doctrine DBAL and a MySQL
    database. It should be easy to adapt this to work with one of the other PHPCR options documented
    in :doc:`installing-configuring-doctrine-phpcr-odm`.

Install the needed Symfony CMF components
-----------------------------------------

Add the following to composer.json:

.. code-block:: javascript

    "symfony-cmf/block-bundle": "dev-master"

To install the above dependencies, run:

.. code-block:: bash

    php composer.phar update

Add the following lines to AppKernel.php:

.. code-block:: php

    // app/AppKernel.php

    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Sonata\BlockBundle\SonataBlockBundle(),
            new Symfony\Cmf\Bundle\BlockBundle\SymfonyCmfBlockBundle(),
        );

        // ...
    }

SonataBlockBundle is a dependency of the CMF BlockBundle and needs to be configured. Add the
following to your ``config.yml``:

.. code-block:: yaml

    # app/config/config.yml
    sonata_block:
        default_contexts: [cms]


Create and configure the database
---------------------------------

You can use an existing database, or create one now to help you follow this tutorial.

Run these commands in MySQL:

.. code-block:: sql

    CREATE DATABASE symfony DEFAULT CHARACTER SET utf8 DEFAULT COLLATE utf8_general_ci;
    CREATE USER 'symfony'@'localhost' IDENTIFIED BY 'UseABetterPassword';
    GRANT ALL ON symfony.* TO 'symfony'@'localhost';

Your ``parameters.yml`` file needs to match the above, for example:

.. configuration-block::

    .. code-block:: yaml

        # app/config/parameters.yml
        parameters:
            database_driver:   pdo_mysql
            database_host:     localhost
            database_port:     ~
            database_name:     symfony
            database_user:     symfony
            database_password: UseABetterPassword


Configure the Doctrine PHPCR component
--------------------------------------

.. note::

    If you have followed :doc:`installing-configuring-doctrine-phpcr-odm`, you can skip this section.

In your ``config.yml`` file, add following configuration for `doctrine_phpcr`:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: doctrinedbal
                    connection: doctrine.dbal.default_connection
                workspace: default
            odm:
                auto_mapping: true

Add the following line to the registerBundles() method in ``AppKernel.php``:

.. code-block:: php

    // app/AppKernel.php

    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),
        );

        // ...
    }

Add the following line to your ``autoload.php`` file, immediately after the last
``AnnotationRegistry::registerFile`` line:

.. code-block:: php

    // app/autoload.php

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Create the database schema and register the PHPCR node types using the following console commands:

.. code-block:: bash

    php app/console doctrine:phpcr:init:dbal
    php app/console doctrine:phpcr:register-system-node-types

Now you should have a number of tables in your MySQL database with the ``phpcr_`` prefix.


Install DoctrineFixturesBundle
------------------------------

.. note::

    As mentioned at the start, this is not a requirement for BlockBundle or ContentBundle; nevertheless
    it is a good way to manage example or default content.

Add the following to composer.json:

.. code-block:: javascript

    "doctrine/doctrine-fixtures-bundle": "dev-master"

To install the above dependencies, run:

.. code-block:: bash

    php composer.phar update

Add the following line to the registerBundles() method in ``AppKernel.php``:

.. code-block:: php

    // app/AppKernel.php

    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
        );

        // ...
    }


Loading fixtures
----------------

Based on the
`DoctrineFixturesBundle documentation <http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html>`_,
you will need to create a fixtures class.

To start with, create a ``DataFixtures`` directory inside your bundle, and inside there, create a
directory named ``PHPCR``. As you follow the examples further below, the DoctrineFixturesBundle will
automatically load the fixtures classes placed here.

Within a fixtures loader, an example of creating a content block might look like this:

.. code-block:: php

    $myBlock = new SimpleBlock();
    $myBlock->setParentDocument($parentPage);
    $myBlock->setName('sidebarBlock');
    $myBlock->setTitle('My first block');
    $myBlock->setContent('Hello block world!');

    $documentManager->persist($myBlock);

The above on its own will not be enough however, because there is no parent (``$parentPage``) to link
the blocks to. There are several possible options for what you can use as the parent:

- Link the blocks directly to the root document (not shown)
- Create a document from the PHPCR bundle (shown below using the Generic document type)
- Create a document from the CMF ContentBundle (shown below using StaticContent document type)


Using the PHPCR
---------------

Create the following class inside your ``DataFixtures/PHPCR`` directory:

.. code-block:: html+php

    <?php
    // src/Acme/MainBundle/DataFixtures/PHPCR/LoadBlockWithPhpcrParent.php

    namespace Acme\MainBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\ODM\PHPCR\Document\Generic;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Symfony\Cmf\Bundle\BlockBundle\Document\SimpleBlock;

    /**
     * Load an example content block using the CMF BlockBundle (without needing any other CMF bundle).
     *
     * To ensure the block has a home in the repository, this loader also creates a 'blocks' document within the PHPCR,
     * to use as the parent.
     */
    class LoadBlockWithPhpcrParent extends AbstractFixture implements ContainerAwareInterface
    {
        public function load(ObjectManager $manager)
        {
            // Get the root document from the PHPCR
            $rootDocument = $manager->find(null, '/');

            // Create a generic PHPCR document under the root, to use as a kind of category for the blocks;
            // we could just use $rootDocument, but that's potentially messy.
            $document = new Generic();
            $document->setParent($rootDocument);
            $document->setNodename('blocks');
            $manager->persist($document);

            // Create a new SimpleBlock from the SymfonyCmfBlockBundle
            // (see http://symfony.com/doc/master/cmf/bundles/block.html#block-types)
            $myBlock = new SimpleBlock();
            $myBlock->setParentDocument($document);
            $myBlock->setName('testBlock');
            $myBlock->setTitle('CMF BlockBundle only');
            $myBlock->setContent('Block from CMF BlockBundle, parent from the PHPCR (Generic document).');
            $manager->persist($myBlock);

            // Commit $document and $block to the database
            $manager->flush();
        }

        /**
         * {@inheritDoc}
         */
        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }
    }

Now load the fixtures using the console:

.. code-block:: bash

    php app/console doctrine:phpcr:fixtures:load

The content in your database should now look something like this:


.. code-block:: sql

    SELECT path, parent, local_name FROM phpcr_nodes;

+-------------------+---------+------------+
| path              | parent  | local_name |
+===================+=========+============+
| /                 |         |            |
+-------------------+---------+------------+
| /blocks           | /       | blocks     |
+-------------------+---------+------------+
| /blocks/testBlock |/ blocks | testBlock  |
+-------------------+---------+------------+


Using the CMF ContentBundle
---------------------------

The ContentBundle also requires RoutingExtraBundle, so to save time you can install both together.
Add the following to composer.json:

.. code-block:: javascript

    "symfony-cmf/content-bundle": "dev-master",
    "symfony-cmf/routing-extra-bundle": "dev-master"

Install as before:

.. code-block:: bash

    php composer.phar update symfony-cmf/content-bundle

Add the following lines to AppKernel.php:

.. code-block:: php

    // app/AppKernel.php

    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Cmf\Bundle\BlockBundle\SymfonyCmfBlockBundle(),
            new Symfony\Cmf\Bundle\ContentBundle\SymfonyCmfContentBundle(),
            new Symfony\Cmf\Bundle\RoutingExtraBundle\SymfonyCmfRoutingExtraBundle(),
        );

        // ...
    }

Now you should have everything needed to load a sample content page with a sample block.

Create the LoadBlockWithCmfParent.php class with the following use statements and load method:

.. code-block:: html+php

    <?php
    // src/Acme/Bundle/MainBundle/DataFixtures/PHPCR/LoadBlockWithCmfParent.php

    namespace Acme\MainBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\Persistence\ObjectManager;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use PHPCR\Util\NodeHelper;
    use Symfony\Cmf\Bundle\BlockBundle\Document\SimpleBlock;
    use Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent;

    /**
     * Load an example content block using the CMF BlockBundle (without needing any other CMF bundle).
     *
     * To ensure the block has a home in the repository, this loader also creates a 'blocks' document within the PHPCR,
     * to use as the parent.
     */
    class LoadBlockWithPhpcrParent extends AbstractFixture implements ContainerAwareInterface
    {
        public function load(ObjectManager $manager)
        {
            // Get the base path name to use from the configuration
            $session = $manager->getPhpcrSession();
            $basepath = $this->container->getParameter('symfony_cmf_content.static_basepath');

            // Create the path in the repository
            NodeHelper::createPath($session, $basepath);

            // Create a new document using StaticContent from the CMF ContentBundle
            $document = new StaticContent();
            $document->setPath($basepath . '/blocks');
            $manager->persist($document);

            // Create a new SimpleBlock from the SymfonyCmfBlockBundle
            // (see http://symfony.com/doc/master/cmf/bundles/block.html#block-types)
            $myBlock = new SimpleBlock();
            $myBlock->setParentDocument($document);
            $myBlock->setName('testBlock');
            $myBlock->setTitle('CMF BlockBundle and ContentBundle');
            $myBlock->setContent('Block from CMF BlockBundle, parent from CMF ContentBundle (StaticContent).');
            $manager->persist($myBlock);

            // Commit $document and $block to the database
            $manager->flush();
        }

        /**
         * {@inheritDoc}
         */
        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }
    }

By default, the base path for the content is /cms/content/static. To show how it can be configured
to any path, add the following, optional entry to your config.yml:

.. code:: yaml

    # app/config/config.yml
    symfony_cmf_content:
        static_basepath: /content

Now it should be possible to load in the above fixtures:

    php app/console doctrine:phpcr:fixtures:load

All being well, the content in your database should look something like this (if you also followed
the LoadBlockWithPhpcrParent example, you should still have two /blocks entries as well):

.. code:: sql

    SELECT path, parent, local_name FROM phpcr_nodes;

+---------------------------+-----------------+------------+
| path                      | parent          | local_name |
+===========================+=================+============+
| /                         |                 |            |
+---------------------------+-----------------+------------+
| /content                  | /               | content    |
+---------------------------+-----------------+------------+
| /content/blocks           | /content        | blocks     |
+---------------------------+-----------------+------------+
| /content/blocks/testBlock | /content/blocks | testBlock  |
+---------------------------+-----------------+------------+


Rendering the blocks
--------------------

This is handled by the Sonata BlockBundle. ``sonata_block_render`` is already registered as a Twig
extension by including ``SonataBlockBundle`` in ``AppKernel.php``. Therefore, you can render any
block within any template by referring to its path.

The following code shows the rendering of both ``testBlock`` instances from the examples above:

.. code::

    {# src/Acme/Bundle/MainBundle/resources/views/Default/index.html.twig #}

    {{ sonata_block_render({
        'name': '/blocks/testBlock'
    }) }}

    <hr />

    {{ sonata_block_render({
        'name': '/content/blocks/testBlock'
    }) }}

Now your index page should show the following (assuming you followed both examples):

.. code::

    CMF BlockBundle only
    Block from CMF BlockBundle, parent from the PHPCR (Generic document).

    CMF BlockBundle and ContentBundle
    Block from CMF BlockBundle, parent from CMF ContentBundle (StaticContent).

Troubleshooting
---------------

If you run into problems, it might be easiest to start with a fresh Symfony2 installation. You can
also try running and modifying the code in the external
`CMF Block Sandbox <https://github.com/fazy/cmf-block-sandbox>`_ working example.

Doctrine configuration
~~~~~~~~~~~~~~~~~~~~~~
If you started with the standard Symfony2 distribution (version 2.1.x), this should already be
configured correctly in your ``config.yml`` file. If not, try using the following section:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   "%database_driver%"
                host:     "%database_host%"
                port:     "%database_port%"
                dbname:   "%database_name%"
                user:     "%database_user%"
                password: "%database_password%"
                charset:  UTF8
            orm:
                auto_generate_proxy_classes: "%kernel.debug%"
                auto_mapping: true

"No commands defined" when loading fixtures
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. code::

    [InvalidArgumentException]
    There are no commands defined in the "doctrine:phpcr:fixtures" namespace.

Make sure AppKernel.php contains the following lines:

.. code:: php

    new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
    new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),

"You did not configure a session" when loading fixtures
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. code::

    [InvalidArgumentException]
    You did not configure a session for the document managers

Make sure you have the following in your app/config.yml:

.. code:: yaml

    doctrine_phpcr:
        session:
            backend:
                type: doctrinedbal
                connection: doctrine.dbal.default_connection
            workspace: default
        odm:
            auto_mapping: true

"Annotation does not exist, or could not be auto-loaded"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. code::

    [Doctrine\Common\Annotations\AnnotationException]
    [Semantical Error] The annotation "@Doctrine\ODM\PHPCR\Mapping\Annotations\Document" in class Doctrine\ODM\PHPCR\Document\Generic does not exist, or could not be auto-loaded.

Make sure you add this line to your app/autoload.php (immediately after the
AnnotationRegistry::registerLoader line):

.. code:: php

    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');

"Class 'Symfony\Cmf\Bundle\BlockBundle\Document\SimpleBlock' was not found"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code::

    [Doctrine\Common\Persistence\Mapping\MappingException]
    The class 'Symfony\Cmf\Bundle\BlockBundle\Document\SimpleBlock' was not found in the chain configured namespaces Doctrine\ODM\PHPCR\Document, Sonata\UserBundle\Document, FOS\UserBundle\Document

Make sure the CMF BlockBundle is installed and loaded in app/AppKernel.php:

.. code:: php

    new Symfony\Cmf\Bundle\BlockBundle\SymfonyCmfBlockBundle(),
