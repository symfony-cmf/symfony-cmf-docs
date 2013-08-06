The BlockBundle
===============

The `BlockBundle`_ provides integration with `SonataBlockBundle`_.
It is used to manage fragments of content, so-called blocks, that are persisted
in a database and can be incorporated into any page layout.

The BlockBundle also provides a few commonly used standard blocks, including the
ability to edit them. See :doc:`types`.

.. index:: BlockBundle

Dependencies
------------

This bundle is based on the `SonataBlockBundle`_.

.. _bundle-block-configuration:

Configuration
-------------

.. sidebar:: Updated SonataBlockBundle defaults
    :subtitle: Prepended Configuration

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

The configuration key for this bundle is ``cmf_block``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_block:
            persistence:
                phpcr:
                    enabled: true
                    manager_name: default

The default settings of a block are defined in the block service. If you use a
3rd party block you might want to alter these for your application. Use the
``sonata_block`` key for this. You can define default settings for a block
service type or more specific for a block class. The later is usefull as a
block service can be used by multiple block classes and sometimes you only want
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
                        maxItems: 3

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://sonata-project.com/schema/dic/block">
                <blocks id="acme_main.block.rss">
                    <settings id="maxItems">3</settings>
                </blocks>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_block', array(
            'blocks' => array(
                'acme_main.block.rss' => array(
                    'settings' => 3,
                ),
            ),
        ));

If you want to make the base fields (f.e. the TTL for caching) of your 
block document based on ``BaseBlock`` editable, just use the existing 
admin extension by adding the following lines to your sonata admin 
configuration ``sonata_admin``. Admin extensions allow you to add or 
change features of one or more Admin instances. Read more about 
`Sonata Admin Extensions`_ for more details.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            extensions:
                cmf.block.admin.base.extension:
                    extends:
                        - Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <extension id="cmf.block.admin.base.extension">
                    <extend>Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock</extend>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'cmf.block.admin.base.extension' => array(
                    'extends' => array(
                        'Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock',
                    ),
                ),
            ),
        ));

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
with the parent document of the block, this makes the block unique. The other
properties are specific to
``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock``.

The simple block is now ready to be rendered, see
:ref:`bundle-block-rendering`.

.. note::

    Always make sure you implement the interface
    ``Sonata\BlockBundle\Model\BlockInterface`` or an existing block document
    like ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock``.

Block Context
-------------

The BlockContext contains all information and the block document needed to
render the block. It aggregates and merges all settings from configuration,
the block service, the block document and settings passed to the twig template
helper. Therefore use the BlockContext to get or alter a setting if needed.

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

Take a look at the block services in ``Symfony\Cmf\Bundle\BlockBundle\Block``
to see some examples.

.. note::

    Always make sure you implement the interface
    ``Sonata\BlockBundle\Block\BlockServiceInterface`` or an existing block
    service like ``Sonata\BlockBundle\Block\BaseBlockService``.

.. _bundle-block-execute:

The Execute Method
~~~~~~~~~~~~~~~~~~

This method contains ``controller`` logic::

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

    If you have much logic to be used, you can move that to a specific service
    and inject it in the block service. Then use this specific service in the
    execute method.

Default Settings
~~~~~~~~~~~~~~~~

The method ``setDefaultSettings`` specifies the default settings for a block.
Settings can be altered on multiple places afterwards, it cascades like this:

* Default settings are stored in the block service;
* If you use a 3rd party bundle you might want to change them in the bundle
  configuration for your application see :ref:`bundle-block-configuration`;
* Settings can be altered through template helpers (see example);
* And settings can also be altered in a block document, the advantage is that
  settings are stored in PHPCR and allows to implement a frontend or backend UI
  to change some or all settings.

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
javascript and stylesheet assets. Use the twig helpers
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

In this example we specify an absolute path, however, if the block is a child
of a content document, then you can simply specify the **name** of the block
as follows:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({'name': 'sidebarBlock'}) }}

    .. code-block:: html+php

        <?php echo $view['blocks']->render(array(
            'name' => 'sidebarBlock',
        )) ?>

This will make the BlockBundle render the specified block on every page that
has a child block document named ``sidebarBlock``.  Of course, the actual page
needs to be rendered by the template that contains the snippet above.

When a block is rendered the following things happen:

* The block document is loaded based on its name or absolute path;
* If caching is configured, the cache is checked and content is returned if
  found;
* The ``execute`` method of the corresponding block service is called.

The execute method is the equivalent of a normal Symfony controller. It
receives the block object (equivalent to a Request object) and a ``Response``
object. The purpose of the ``execute`` method to set the content of the
response object - typically by rendering a Twig template.

You can also :ref:`embed blocks in content <tutorial-block-embed>` using the
``cmf_embed_blocks`` filter.

Block types
-----------

The block bundle comes with a couple of predefined blocks. You may write
your own blocks, but often, the supplied implementations will be sufficient.
This is just a quick overview, more details on each block type can be found
in the :doc:`Block Types <types>` section.

There are five general purpose blocks:

* **StringBlock**: A block only containing a string that is rendered without
  any decoration. Useful for page fragments;
* **SimpleBlock**: A simple block with nothing but a title and a field of
  hypertext. This would usually be what an editor edits directly, for example
  contact information;
* **ContainerBlock**: A block that contains zero, one or many child blocks;
* **ReferenceBlock**: A block that references a block stored somewhere else in
  the content tree. For example you might want; to refer parts of the contact
  information from the homepage
* **ActionBlock**: A block that calls a Symfony2 action.

The BlockBundle also provides a couple of blocks for specific tasks,
integrating third party libraries. You should to read the :doc:`types` section
relevant to those blocks to figure out what third party libraries you need to
load into your project.

* **RssBlock**: This block extends the ``ActionBlock``, the block document
  saves the feed url and the controller action fetches the feed items. The
  default implementation uses the `EkoFeedBundle
  <https://github.com/eko/FeedBundle>`_ to read the feed items.

* **ImagineBlock**: A block containing an image child, the imagine filter name
  and optional link url and title.

* **SlideshowBlock**: A special case of a container block suitable for building
  a slideshow of blocks. Note that this block doesn't provide any Javascript
  code to make the slideshow work in the frontend. You can use your favourite
  Javascript library to do the animation.

Examples
--------

You can find example usages of this bundle in the `Symfony CMF Sandbox`_
(have a look at the BlockBundle). It also shows you how to make blocks
editable using the :doc:`CreateBundle <../create>`.

.. _`BlockBundle`: https://github.com/symfony-cmf/BlockBundle#readme
.. _`Symfony CMF Sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`prepended configuration`: http://symfony.com/doc/current/components/dependency_injection/compilation.html#prepending-configuration-passed-to-the-extension
.. _`SonataBlockBundle`: https://github.com/sonata-project/SonataBlockBundle
.. _`Sonata Admin Extensions`: http://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
