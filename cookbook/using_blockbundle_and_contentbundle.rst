.. index::
    single: PHPCR and BlockBundle and ContentBundle
    single: BlockBundle
    single: ContentBundle

Using the BlockBundle and ContentBundle with PHPCR
==================================================

The goal of this tutorial is to demonstrate how the CMF
:doc:`BlockBundle <../bundles/block/index>` and :doc:`ContentBundle
<../bundles/content>` can be used as stand-alone components, and to show how
they fit into the PHPCR.

This tutorial demonstrates the simplest possible usage, to get you up and
running quickly. Once you are familiar with basic usage, the in-depth
documentation of both bundles will help you to adapt these basic examples to
serve more advanced use cases.

We will begin with using only BlockBundle, with content blocks linked directly
into the PHPCR.  Next, we will introduce the ContentBundle to show how it can
represent content pages containing blocks.

.. note::

    Although not a requirement for using BlockBundle or ContentBundle, this
    tutorial will also make use of `DoctrineFixturesBundle`_. This is because
    it provides an easy way to load in some test content.

Preconditions
-------------

* `Installation of Symfony2`_ (2.1.x)
* :doc:`installing_configuring_doctrine_phpcr_odm`

.. note::

    This tutorial is based on using PHPCR-ODM set up with Jackalope, Doctrine
    DBAL and a MySQL database. It should be easy to adapt this to work with
    one of the other PHPCR options documented in
    :doc:`installing_configuring_doctrine_phpcr_odm`.

Create and Configure the Database
---------------------------------

You can use an existing database, or create one now to help you follow this
tutorial. For a new database, run these commands in MySQL:

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

Configure the Doctrine PHPCR Component
--------------------------------------

.. note::

    If you have followed :doc:`installing_configuring_doctrine_phpcr_odm`, you
    can skip this section.

You need to install the PHPCR-ODM components. Add the following to your
``composer.json`` file:

.. code-block:: javascript

    "require": {
        ...
        "jackalope/jackalope-jackrabbit": "1.0.*",
        "jackalope/jackalope-doctrine-dbal": "dev-master",
        "doctrine/phpcr-bundle": "1.0.*",
        "doctrine/phpcr-odm": "1.0.*"
    }

To install the above, run:

.. code-block:: bash

    $ php composer.phar update

In your ``config.yml`` file, add following configuration for ``doctrine_phpcr``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: doctrinedbal
                    # connection: default
                workspace: default
            odm:
                auto_mapping: true

Add the following line to the ``registerBundles()`` method of the
``AppKernel``::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),
        );

        // ...
    }

Add the following line to your ``autoload.php`` file, immediately after the
last ``AnnotationRegistry::registerFile`` line::

    // app/autoload.php

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Create the database schema and register the PHPCR node types using the
following console commands:

.. code-block:: bash

    $ php app/console doctrine:phpcr:init:dbal
    $ php app/console doctrine:phpcr:repository:init

Now you should have a number of tables in your MySQL database with the
``phpcr_`` prefix.

Install the needed Symfony CMF Components
-----------------------------------------

To install the BlockBundle, run:

.. code-block:: bash

    $ php composer.phar require symfony-cmf/block-bundle:master

Add the following lines to ``AppKernel``::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Sonata\BlockBundle\SonataBlockBundle(),
            new Symfony\Cmf\Bundle\BlockBundle\CmfBlockBundle(),
        );

        // ...
    }

SonataBlockBundle is a dependency of the CMF BlockBundle and needs to be
configured. Add the following to your ``config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            default_contexts: [cms]

Install DoctrineFixturesBundle
------------------------------

.. note::

    As mentioned at the start, this is not a requirement for BlockBundle or
    ContentBundle; nevertheless it is a good way to manage example or default
    content.

To install the DoctrineFixturesBundle, run:

.. code-block:: bash

    $ php composer.phar require doctrine/doctrine-fixtures-bundle:dev-master

Add the following line to the ``registerBundles()`` method of the
``AppKernel``::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
        );

        // ...
    }

Loading Fixtures
----------------

Based on the `DoctrineFixturesBundle documentation`_, you will need to create
a fixtures class.

To start with, create a ``DataFixtures`` directory inside your own bundle
(e.g. "MainBundle"), and inside there, create a directory named ``PHPCR``. As
you follow the examples further below, the DoctrineFixturesBundle will
automatically load the fixtures classes placed here.

Within a fixtures loader, an example of creating a content block might look
like this::

    $myBlock = new SimpleBlock();
    $myBlock->setParentDocument($parentPage);
    $myBlock->setName('sidebarBlock');
    $myBlock->setTitle('My first block');
    $myBlock->setBody('Hello block world!');

    $documentManager->persist($myBlock);

The above on its own will not be enough however, because there is no parent
(``$parentPage``) to link the blocks to. There are several possible options
that you can use as the parent:

* Link the blocks directly to the root document (not shown)
* Create a document from the PHPCR bundle (shown below using the ``Generic``
  document type)
* Create a document from the CMF ContentBundle (shown below using
  ``StaticContent`` document type)

Using the PHPCR
---------------

To store a CMF block directly in the PHPCR, create the following class inside
your ``DataFixtures/PHPCR`` directory::

    <?php
    // src/Acme/MainBundle/DataFixtures/PHPCR/LoadBlockWithPhpcrParent.php
    namespace Acme\MainBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\ODM\PHPCR\Document\Generic;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock;

    class LoadBlockWithPhpcrParent extends AbstractFixture implements ContainerAwareInterface
    {
        public function load(ObjectManager $manager)
        {
            // Get the root document from the PHPCR
            $rootDocument = $manager->find(null, '/');

            // Create a generic PHPCR document under the root, to use as a kind of category for the blocks
            $document = new Generic();
            $document->setParent($rootDocument);
            $document->setNodename('blocks');
            $manager->persist($document);

            // Create a new SimpleBlock (see http://symfony.com/doc/master/cmf/bundles/block.html#block-types)
            $myBlock = new SimpleBlock();
            $myBlock->setParentDocument($document);
            $myBlock->setName('testBlock');
            $myBlock->setTitle('CMF BlockBundle only');
            $myBlock->setBody('Block from CMF BlockBundle, parent from the PHPCR (Generic document).');
            $manager->persist($myBlock);

            // Commit $document and $block to the database
            $manager->flush();
        }

        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }
    }

This class loads an example content block using the CMF BlockBundle (without
needing any other CMF bundle). To ensure the block has a parent in the
repository, the loader also creates a ``Generic`` document named 'blocks'
within the PHPCR.

Now load the fixtures using the console:

.. code-block:: bash

    $ php app/console doctrine:phpcr:fixtures:load

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

Follow this example to use both the CMF Block and Content components together.

The ContentBundle is best used together with the RoutingBundle. Add the
following to ``composer.json``:

.. code-block:: javascript

    "require": {
        ...
        "symfony-cmf/content-bundle": "dev-master",
        "symfony-cmf/routing-bundle": "dev-master"
    }

Install as before:

.. code-block:: bash

    $ php composer.phar update

Add the following line to ``AppKernel``:

.. code-block:: php

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Cmf\Bundle\ContentBundle\CmfContentBundle(),
        );

        // ...
    }

Now you should have everything needed to load a sample content page with a
sample block, so create the ``LoadBlockWithCmfParent.php`` class::

    <?php
    // src/Acme/Bundle/MainBundle/DataFixtures/PHPCR/LoadBlockWithCmfParent.php
    namespace Acme\MainBundle\DataFixtures\PHPCR;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\Persistence\ObjectManager;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use PHPCR\Util\NodeHelper;
    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock;
    use Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent;

    class LoadBlockWithCmfParent extends AbstractFixture implements ContainerAwareInterface
    {
        public function load(ObjectManager $manager)
        {
            // Get the base path name to use from the configuration
            $session = $manager->getPhpcrSession();
            $basepath = $this->container->getParameter('cmf_content.static_basepath');

            // Create the path in the repository
            NodeHelper::createPath($session, $basepath);

            // Create a new document using StaticContent from the CMF ContentBundle
            $document = new StaticContent();
            $document->setPath($basepath . '/blocks');
            $manager->persist($document);

            // Create a new SimpleBlock (see http://symfony.com/doc/master/cmf/bundles/block.html#block-types)
            $myBlock = new SimpleBlock();
            $myBlock->setParentDocument($document);
            $myBlock->setName('testBlock');
            $myBlock->setTitle('CMF BlockBundle and ContentBundle');
            $myBlock->setBody('Block from CMF BlockBundle, parent from CMF ContentBundle (StaticContent).');
            $manager->persist($myBlock);

            // Commit $document and $block to the database
            $manager->flush();
        }

        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }
    }

This class creates an example content page using the CMF ContentBundle. It
then loads our example block as before, using the new content page as its
parent.

By default, the base path for the content is ``/cms/content/static``. To show
how it can be configured to any path, add the following, optional entry to
your ``config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_content:
            static_basepath: /content

Now it should be possible to load in the above fixtures:

.. code-block:: bash

    $ php app/console doctrine:phpcr:fixtures:load

All being well, the content in your database should look something like this
(if you also followed the ``LoadBlockWithPhpcrParent`` example, you should
still have two ``/blocks`` entries as well):

.. code-block:: sql

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

Rendering the Blocks
--------------------

This is handled by the Sonata BlockBundle. ``sonata_block_render`` is already
registered as a Twig extension by including ``SonataBlockBundle`` in
``AppKernel.php``. Therefore, you can render any block within any template by
referring to its path.

The following code shows the rendering of both ``testBlock`` instances from
the examples above.  If you only followed one of the examples, make sure to
only include that block:

.. code-block:: jinja

    {# src/Acme/Bundle/MainBundle/resources/views/Default/index.html.twig #}

    {# include this if you followed the BlockBundle with PHPCR example #}
    {{ sonata_block_render({
        'name': '/blocks/testBlock'
    }) }}

    {# include this if you followed the BlockBundle with ContentBundle example #}
    {{ sonata_block_render({
        'name': '/content/blocks/testBlock'
    }) }}

Now your index page should show the following (assuming you followed both
examples):

.. code-block:: text

    CMF BlockBundle only
    Block from CMF BlockBundle, parent from the PHPCR (Generic document).

    CMF BlockBundle and ContentBundle
    Block from CMF BlockBundle, parent from CMF ContentBundle (StaticContent).

This happens when a block is rendered, see the :doc:`../bundles/block/index`
for more details:

* a document is loaded based on the name
* if caching is configured, the cache is checked and content is returned if found
* each block document also has a block service, the execute method of it is called:

  * you can put here logic like in a controller
  * it calls a template
  * the result is a Response object

.. note::

    A block can also be configured using settings, this allows you to create
    more advanced blocks and reuse it. The default settings are configured in
    the block service and can be altered in the bundle configuration, the twig
    helper and the block document.  An example is an rss reader block, the url
    and title are stored in the settings of the block document, the maximum
    amount of items to display is specified when calling
    ``sonata_block_render``.

.. _tutorial-block-embed:

Embedding Blocks in WYSIWYG Content
-----------------------------------

The CmfBlockBundle provides a twig filter ``cmf_embed_blocks`` that
looks through the content and looks for special tags to render blocks. To use
the tag, you need to apply the ``cmf_embed_blocks`` filter to your output.  If
you can, render your blocks directly in the template. This feature is only a
cheap solution for web editors to place blocks anywhere in their HTML content.
A better solution to build composed pages is to build it from blocks (there
might be a CMF bundle at some point for this).

.. code-block:: jinja

    {# template.twig.html #}
    {{ page.content|cmf_embed_blocks }}

Make sure to only place this filter where you display the content and not where
editing it, as otherwise your users would start to edit the rendered output of
their blocks. When you apply the filter, your users can use this tag to embed a
block in their HTML content:

.. code-block:: html

    %embed-block|/absolute/path/to/block|end%

    %embed-block|local-block|end%

The path to the block is either absolute or relative to the current main
content. The actual path to the block must be enclosed with double quotes
``"``. But the prefix and postfix are configurable. The default prefix is
``%embed-block|`` and the default postfix is ``|end%``. Say you want
to use ``%%%block:"/absolute/path"%%%`` then you do:

.. configuration-block::

     .. code-block:: yaml

        # app/config/config.yml
        cmf_block:
            twig:
                cmf_embed_blocks:
                    prefix: %%%block:"
                    postfix: "%%%

See also the :ref:`the configuration reference
<reference-config-block-twig-cmf-embed-blocks>`.

.. caution::

    Currently there is no security built into this feature. Only enable the
    filter for content for which you are sure only trusted users may edit it.
    Restrictions about what block can be where that are built into an admin
    interface are not respected here.

.. note::

    The block embed filter ignores all errors that might occur when rendering a
    block and returns an empty string for each failed block instead. The errors
    are logged at level WARNING.

Next Steps
----------

You should now be ready to use the BlockBundle and/or the ContentBundle in
your application, or to explore the other available CMF bundles.

* See the :doc:`BlockBundle <../bundles/block/index>` and :doc:`ContentBundle
  <../bundles/content>` documentation to learn about more advanced usage of
  these bundles
* To see a better way of loading fixtures, look at the
  `fixtures in the CMF Sandbox`_
* Take a look at the `PHPCR Tutorial`_ for a better understanding of the
  underlying content repository

Troubleshooting
---------------

If you run into problems, it might be easiest to start with a fresh Symfony2
installation. You can also try running and modifying the code in the external
`CMF Block Sandbox`_ working example.

Doctrine configuration
~~~~~~~~~~~~~~~~~~~~~~

If you started with the standard Symfony2 distribution (version 2.1.x), this
should already be configured correctly in your ``config.yml`` file. If not,
try using the following section:

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

.. code-block:: text

    [InvalidArgumentException]
    There are no commands defined in the "doctrine:phpcr:fixtures" namespace.

Make sure ``AppKernel.php`` contains the following lines:

.. code-block:: php

    new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
    new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),

"You did not configure a session"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    [InvalidArgumentException]
    You did not configure a session for the document managers

Make sure you have the following in your config file:

.. configuration-block::

    .. code-block:: yaml

        # app/config.yml
        doctrine_phpcr:
            session:
                backend:
                    type: doctrinedbal
                    # connection: default
                workspace: default
            odm:
                auto_mapping: true

"Annotation does not exist"
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    [Doctrine\Common\Annotations\AnnotationException]
    [Semantical Error] The annotation "@Doctrine\ODM\PHPCR\Mapping\Annotations\Document" in class Doctrine\ODM\PHPCR\Document\Generic does not exist, or could not be auto-loaded.

Make sure you add this line to your ``app/autoload.php`` (immediately after
the ``AnnotationRegistry::registerLoader`` line)::

    AnnotationRegistry::registerFile(__DIR__.'/../vendor/doctrine/phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');

SimpleBlock class not found
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    [Doctrine\Common\Persistence\Mapping\MappingException]
    The class 'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock' was not found in the chain configured namespaces Doctrine\ODM\PHPCR\Document, Sonata\UserBundle\Document, FOS\UserBundle\Document

Make sure the CMF BlockBundle is installed and loaded in ``app/AppKernel.php``::

    new Symfony\Cmf\Bundle\BlockBundle\CmfBlockBundle(),

RouteReferrersInterface not found
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    Fatal error: Interface 'Symfony\Cmf\Component\Routing\RouteReferrersInterface' not found in /var/www/your-site/vendor/symfony-cmf/content-bundle/Symfony/Cmf/Bundle/ContentBundle/Document/StaticContent.php on line 15

If you are using ContentBundle, make sure you have also installed the RoutingBundle:

.. code-block:: bash

    $ php composer.phar require symfony-cmf/routing-bundle:dev-master

.. _`DoctrineFixturesBundle`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`Installation of Symfony2`: http://symfony.com/doc/2.1/book/installation.html
.. _`DoctrineFixturesBundle documentation`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`fixtures in the CMF Sandbox`: https://github.com/symfony-cmf/cmf-sandbox/tree/master/src/Sandbox/MainBundle/DataFixtures/PHPCR
.. _`PHPCR Tutorial`: https://github.com/phpcr/phpcr-docs/blob/master/tutorial/Tutorial.md
.. _`CMF Block Sandbox`: https://github.com/fazy/cmf-block-sandbox
