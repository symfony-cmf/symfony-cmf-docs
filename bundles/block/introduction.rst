.. index::
    single: Block; Bundles
    single: BlockBundle

The BlockBundle
===============

    This bundle provides integration with `SonataBlockBundle`_. It is used to
    manage fragments of content, so-called blocks, that are persisted in a
    database and can be incorporated into any page layout.

The Symfony2 CMF BlockBundle also provides a few commonly used standard blocks,
including the ability to edit them. See :doc:`types`.

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/BlockBundle);
* Install it via Composer (``symfony-cmf/block-bundle`` on `Packagist`_).

.. _bundle-block-configuration:

Usage
-----

The default settings of a block are defined in the block service. If you use a
third party block, you might want to alter these for your application. Use the
``sonata_block`` key for this. You can define default settings for a block
service type or more specific for a block class. The later is useful when you
are using the same block service for multiple block classes but you only want
specific settings for one of the block classes.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            blocks:
                acme_main.block.news:
                    settings:
                        maxItems: 3
            blocks_by_class:
                Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock:
                    settings:
                        maxItems: 5

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://sonata-project.com/schema/dic/block">
                <blocks id="acme_main.block.rss">
                    <setting id="maxItems">3</setting>
                </blocks>
                <block-by-class class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock">
                    <setting id="maxItems">5</setting>
                </block-by-class>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_block', array(
            'blocks' => array(
                'acme_main.block.rss' => array(
                    'settings' => array(
                        'maxItems' => 3,
                    ),
                ),
            ),
            'blocks_by_class' => array(
                'Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock' => array(
                    'settings' => array(
                        'maxItems' => 5,
                    ),
                ),
            ),
        ));

Sonata Admin Extension for basic block settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If instead of (or in addition to) configuring block settings you want to make
them editable in sonata admin, there is a sonata admin extension you can
activate. This extension will add a tab to edit settings of base block options
like the TTL (time to life, for caching purposes) editable.

Assuming your blocks extend the ``BaseBlock`` class, you can add the following
lines to your sonata admin configuration.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            extensions:
                cmf.block.admin.base.extension:
                    extends:
                        - Symfony\Cmf\Bundle\BlockBundle\Model\BaseBlock

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <extension id="cmf.block.admin.base.extension">
                    <extend>Symfony\Cmf\Bundle\BlockBundle\Model\BaseBlock</extend>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'cmf.block.admin.base.extension' => array(
                    'extends' => array(
                        'Symfony\Cmf\Bundle\BlockBundle\Model\BaseBlock',
                    ),
                ),
            ),
        ));

.. note::

    Admin extensions are a way to configure editing of common features on several
    ``Admin`` classes without needing them to extend each other. If you want to
    learn more about them, please read on in `Sonata Admin Extensions`_ for more
    details.

.. _bundle-block-updated-sonata-defaults:

Updated SonataBlockBundle defaults
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The BlockBundle *automatically* changes some defaults and adds configuration
to the `SonataBlockBundle`_ to make it work nicely. This is done using the
`prepended configuration`_ option of Symfony available since version 2.2.
See ``DependencyInjection\CmfBlockExtension::prepend``.

Updated defaults:

* **templates.block_base** the cmf base template wraps the block output in
  a div and dashifies the PHPCR path as id; The base template is
  kept compatible with the Sonata base template for non-cmf blocks;
* **RssBlock configuration** adds the
  :ref:`default RssBlock settings <bundle-block-rss-settings>`.

.. note::

    Settings are only prepended, define the settings explicitly inside
    the ``app/config/config.yml`` to override them.

.. _bundle-block-document:

Block Document
--------------

Before you can render a block, you need to create a data object representing
your block in the repository. You can do so with the following code snippet::

    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock;

    // ...

    $myBlock = new SimpleBlock();
    $myBlock->setParentDocument($parentDocument);
    $myBlock->setName('sidebarBlock');
    $myBlock->setTitle('My first block');
    $myBlock->setBody('Hello block world!');

    $documentManager->persist($myBlock);

Note the ``sidebarBlock`` is the identifier we chose for the block. Together
with the parent document of the block, this defines the unique identifier of
the block. The other properties (title and body) are specific to the
``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock``.

The simple block is now ready to be rendered, see
:ref:`bundle-block-rendering`.

.. note::

    Make sure to always have your blocks implement the interface
    ``Sonata\BlockBundle\Model\BlockInterface`` or extend an existing block
    document like ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock``.

Block Context
-------------

The BlockContext contains all information and the block document needed to
render the block. It aggregates and merges all settings from configuration,
the block service, the block document and settings passed to the twig template
helper. Therefore, use the BlockContext to get or alter a setting if needed.

.. _bundle-block-service:

Block Service
-------------

If you look inside the ``SimpleBlock`` class, you will notice the method
``getType``. This defines the name of the block service that processes the
block when it is rendered.

A block service contains:

* An execute method;
* Default settings;
* Dorm configuration;
* Cache configuration;
* Javascript and stylesheet assets to be loaded;
* A load method.

The block services provided by the Symfony2 CMF BlockBundle are in the
namespace ``Symfony\Cmf\Bundle\BlockBundle\Block``.

.. note::

    Always make sure you implement the interface
    ``Sonata\BlockBundle\Block\BlockServiceInterface`` or extend a block
    service like ``Sonata\BlockBundle\Block\BaseBlockService``.

.. _bundle-block-execute:

The Execute Method
~~~~~~~~~~~~~~~~~~

This method of a block service contains *controller* logic::

    // ...
    if ($block->getEnabled()) {
        $feed = false;
        if ($blockContext->getSetting('url', false)) {
            $feed = $this->feedReader->import($block);
        }

        return $this->renderResponse($blockContext->getTemplate(), array(
            'feed'     => $feed,
            'block'    => $blockContext->getBlock(),
            'settings' => $blockContext->getSettings(),
        ), $response);
    }
    // ...

.. note::

    If you need complex logic to handle a block, it is recommended to move that
    logic into a dedicated service and inject that service into the block
    service and call it in the ``execute`` method.

Default Settings
~~~~~~~~~~~~~~~~

The method ``setDefaultSettings`` specifies the default settings for a block.
Settings can be altered in multiple places afterwards, cascading as follows:

* Default settings are stored in the block service;
* If you use a 3rd party bundle you might want to change them in the bundle
  configuration for your application see :ref:`bundle-block-configuration`;
* Settings can be altered through template helpers (see example);
* And settings can also be altered in a block document, the advantage is that
  settings are stored in the database and are individual to the specific block
  instead of all blocks of a type.

Example of how settings can be specified through a template helper:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({'name': 'rssBlock'}, {
            'title': 'Symfony2 CMF news',
            'url': 'http://cmf.symfony.com/news.rss'
        }) }}

    .. code-block:: html+php

        <?php $view['blocks']->render(array('name' => 'rssBlock'), array(
            'title' => 'Symfony2 CMF news',
            'url'   => 'http://cmf.symfony.com/news.rss',
        )) ?>

Form Configuration
~~~~~~~~~~~~~~~~~~

The methods ``buildEditForm`` and ``buildCreateForm`` specify how to build the
the forms for editing using a frontend or backend UI. The method
``validateBlock`` contains the validation configuration.

Cache Configuration
~~~~~~~~~~~~~~~~~~~

The method ``getCacheKeys`` contains cache keys to be used for caching the
block.

Javascript and Stylesheets
~~~~~~~~~~~~~~~~~~~~~~~~~~

The methods ``getJavascripts`` and ``getStylesheets`` can be used to define
javascript and stylesheet assets needed by a block. Use the twig helpers
``sonata_block_include_javascripts`` and ``sonata_block_include_stylesheets``
to render them:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_include_javascripts() }}
        {{ sonata_block_include_stylesheets() }}

    .. code-block:: html+php

        <?php $view['blocks']->includeJavaScripts() ?>
        <?php $view['blocks']->includeStylesheets() ?>

.. note::

    This will output the javascripts and stylesheets for all blocks loaded in
    the service container of your application.

The Load Method
~~~~~~~~~~~~~~~

The method ``load`` can be used to load additional data. It is called each
time a block is rendered before the ``execute`` method is called.

.. _bundle-block-rendering:

Block rendering
---------------

To render the example from the :ref:`bundle-block-document` section, just add
the following code to your Twig template:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({'name': '/cms/content/blocks/sidebarBlock'}) }}

    .. code-block:: html+php

        <?php echo $view['blocks']->render(array(
            'name' => '/cms/content/blocks/sidebarBlock',
        )) ?>

In this example, we specify an absolute path. However, if the block is the
child of a content document, then you can simply specify the **name** of the
block as follows:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({'name': 'sidebarBlock'}) }}

    .. code-block:: html+php

        <?php echo $view['blocks']->render(array(
            'name' => 'sidebarBlock',
        )) ?>

This will make the BlockBundle render the specified block if the main content
(as per the :ref:`routing <bundle-routing-dynamic-match>`) maps a field named
``sidebarBlock``. If different main contents are rendered using different
templates, make sure all that should support this block actually include the
snippet above.

When a block being rendered, the following things happen:

* The block document is loaded based on its name or absolute path;
* If caching is configured, the cache is checked and content is returned if
  found;
* Otherwise, the ``execute`` method of the corresponding block service is
  called.

The ``execute`` method is the equivalent of a normal Symfony controller. It
receives the block object (equivalent to a Request object) and a ``Response``
object. The purpose of the ``execute`` method to set the content of the
response object - typically by rendering a Twig template.

You can also :ref:`embed blocks in WYSIWYG content <tutorial-block-embed>`
using the ``cmf_embed_blocks`` filter.

Examples
--------

You can find example usages of this bundle in the `Symfony CMF Sandbox`_
(have a look at the BlockBundle). It also shows you how to make blocks
editable using the :doc:`CreateBundle <../create>`.

Read on
-------

* :doc:`types`
* :doc:`create_your_own_blocks`
* :doc:`cache`
* :doc:`relation_to_sonata_block_bundle`

.. _`Packagist`: https://packagist.org/packages/symfony-cmf/block-bundle
.. _`Symfony CMF Sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`prepended configuration`: http://symfony.com/doc/current/components/dependency_injection/compilation.html#prepending-configuration-passed-to-the-extension
.. _`SonataBlockBundle`: https://github.com/sonata-project/SonataBlockBundle
.. _`Sonata Admin Extensions`: http://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
