The BlockBundle
===============

The `BlockBundle`_ provides integration with SonataBlockBundle. It assists you
in managing fragments of contents, so-called blocks. What the BlockBundle does
is similar to what Twig does, but for blocks that are persisted in a DB.
Thus, the blocks can be made editable for an editor.  Also the BlockBundle
provides the logic to determine which block should be rendered on which pages.

The BlockBundle does not provide an editing functionality for blocks itself.
However, you can find examples on how making blocks editable in the
`Symfony CMF Sandbox`_.

.. index:: BlockBundle

Dependencies
------------

This bundle is based on the `SonataBlockBundle`_.

.. _bundle-block-configuration:

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_block``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_block:
            document_manager_name:  default

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:cmf-block="http://cmf.symfony.com/schema/dic/block"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <cmf-block:config
                document-manager-name="default"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('symfony_cmf_block', array(
            'document_manager_name' => 'default',
        ));

The default settings of a block are defined in the block service. If you use a
3rd party block you might want to alter these for your application. Use the
``sonata_block`` key for this.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            blocks:
                acme_main.block.rss:
                    settings:
                        maxItems: 3

.. _bundle-block-document:

Block Document
--------------

Before you can render a block, you need to create a data object representing
your block in the repository. You can do so with the following code snippet::

    <?php

    $myBlock = new SimpleBlock();
    $myBlock->setParentDocument($parentDocument);
    $myBlock->setName('sidebarBlock');
    $myBlock->setTitle('My first block');
    $myBlock->setContent('Hello block world!');

    $documentManager->persist($myBlock);

Note the ``sidebarBlock`` is the identifier we chose for the block. Together
with the parent document of the block, this makes the block unique. The other
properties are specific to
``Symfony\Cmf\Bundle\BlockBundle\Document\SimpleBlock``.

The simple block is now ready to be rendered, see
:ref:`bundle-block-rendering`.

.. note::

    Always make sure you implement the interface
    ``Sonata\BlockBundle\Model\BlockInterface`` or an existing block document
    like ``Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock``.

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

This method contains ``controller`` logic. Merge the default settings in this
method if you would like to use them::

    <?php

    // ...
    if ($block->getEnabled()) {
        // merge settings
        $settings = array_merge($this->getDefaultSettings(), $block->getSettings());

        $feed = false;
        if ($settings['url']) {
            $feed = $this->feedReader->import($block);
        }

        return $this->renderResponse($this->getTemplate(), array(
            'feed' => $feed,
            'block' => $block,
            'settings' => $settings
        ), $response);
    }
    // ...

.. note::

    If you have much logic to be used, you can move that to a specific service
    and inject it in the block service. Then use this specific service in the
    execute method.

Default Settings
~~~~~~~~~~~~~~~~

The method ``getDefaultSettings`` specifies the default settings for a block.
Settings can be altered on multiple places afterwards, it cascades like this:

* Default settings are stored in the block service;
* If you use a 3rd party bundle you might want to change them in the bundle
  configuration for your application see :ref:`configuration`;
* Settings can be altered through template helpers (see example);
* And settings can also be altered in a block document, the advantage is that
  settings are stored in PHPCR and allows to implement a frontend or backend UI
  to change some or all settings.

Example of how settings can be specified through a template helper:

.. code-block:: jinja

    {{ sonata_block_render({
      'type': 'acme_main.block.rss',
      'settings': {
          'title': 'Symfony2 CMF news',
          'url': 'http://cmf.symfony.com/news.rss'
      }
    }) }}

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

.. code-block:: jinja

    {{ sonata_block_include_javascripts() }}
    {{ sonata_block_include_stylesheets() }}

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

.. code-block:: jinja

    {{ sonata_block_render({'name': '/cms/content/blocks/sidebarBlock'}) }}

In this example we specify an absolute path, however, if the block is a child
of a content document, then you can simply specify the **name** of the block
as follows:

.. code-block:: jinja

    {{ sonata_block_render({'name': 'sidebarBlock'}) }}

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

The BlockBundle comes with five general purpose blocks:

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

.. note::

    You may be thinking "Why would I use this instead of directly calling the
    action from my template?", well, imagine the following case: You provide a
    block that renders teasers of your latest news. However, there is no rule
    where they should appear. Instead, your customer wants to decide himself
    on what pages this block is to be displayed. By using an ActionBlock, you
    could allow your customer to do so without calling you to change some
    templates (over and over again!).

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
editable using the :doc:`CreateBundle <create>`.

Reference
---------

.. toctree::
    :maxdepth: 1
    :numbered:

    types
    create_your_own_blocks
    cache
    relation_to_sonata_block_bundle

.. _`BlockBundle`: https://github.com/symfony-cmf/BlockBundle#readme
.. _`Symfony CMF Sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`SonataBlockBundle`: https://github.com/sonata-project/SonataBlockBundle
