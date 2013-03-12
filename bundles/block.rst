BlockBundle
===========

The `BlockBundle <https://github.com/symfony-cmf/BlockBundle#readme>`_ provides integration with SonataBlockBundle.
It assists you in managing fragments of contents, so-called blocks. What the BlockBundle does is similar
to what Twig does, but for blocks that are persisted in a DB. Thus, the blocks can be made editable for an editor.
Also the BlockBundle provides the logic to determine which block should be rendered on which pages.

The BlockBundle does not provide an editing functionality for blocks itself. However, you can find examples
on how making blocks editable in the `Symfony CMF Sandbox <https://github.com/symfony-cmf/cmf-sandbox>`_.

.. index:: BlockBundle

Dependencies
------------

This bundle is based on the `SonataBlockBundle <https://github.com/sonata-project/SonataBlockBundle>`_

.. _configuration:

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_block``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_block:
            document_manager_name:  default

.. _block-document:

Block Document
--------------

Before you can render a block, you need to create a data object representing your block in the repository.
You can do so with the following code snippet (Note that ``$parentPage`` needs to be an instance of
a page defined by the `ContentBundle <https://github.com/symfony-cmf/ContentBundle>`_):

.. code-block:: php

    $myBlock = new SimpleBlock();
    $myBlock->setParentDocument($parentPage);
    $myBlock->setName('sidebarBlock');
    $myBlock->setTitle('My first block');
    $myBlock->setContent('Hello block world!');

    $documentManager->persist($myBlock);

Note the 'sidebarBlock' is the identifier we chose for the block. Together with the parent document of
the block, this makes the block unique. The other properties are specific to ``Symfony\Cmf\Bundle\BlockBundle\Document\SimpleBlock``.

.. note::

    The simple block is now ready to be rendered, see :ref:`block-rendering`.

.. note::

    Always make sure you implement the interface ``Sonata\BlockBundle\Model\BlockInterface`` or an existing block document like
    ``Symfony\Cmf\Bundle\BlockBundle\Document\BaseBlock``.

.. _block-service:

Block Service
-------------

If you look inside the SimpleBlock class, you will notice the method ``getType``. This defines the name of the block
service that processes the block when it is rendered.

A block service contains:

* an execute method
* default settings
* form configuration
* cache configuration
* js and css assets to be loaded
* a load method

Take a look at the block services in ``Symfony\Cmf\Bundle\BlockBundle\Block`` to see some examples.

.. note::

    Always make sure you implement the interface ``Sonata\BlockBundle\Block\BlockServiceInterface`` or an existing block
    service like ``Sonata\BlockBundle\Block\BaseBlockService``.

.. _block-execute:

execute method
^^^^^^^^^^^^^^

This contains ``controller`` logic. Merge the default settings in this method if you would like to use them:

.. code-block:: php

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

    If you have much logic to be used, you can move that to a specific service and inject it in the block service. Then
    use this specific service in the execute method.

default settings
^^^^^^^^^^^^^^^^

The method ``getDefaultSettings`` contains the default settings of a block. Settings can be altered on multiple places
afterward, it cascades like this:

* default settings are stored in the block service
* settings can be altered through template helpers:

  .. code-block:: jinja

      {{ sonata_block_render({
        'type': 'acme_main.block.rss',
        'settings': {
            'title': 'Symfony2 CMF news',
            'url': 'http://cmf.symfony.com/news.rss'
        }
      }) }}

* and settings can also be altered in a block document, the advantage is that settings are stored in PHPCR and allows to implement a
  frontend or backend UI to change some or all settings

form configuration
^^^^^^^^^^^^^^^^^^

The methods ``buildEditForm`` and ``buildCreateForm`` contain form configuration for editing using a frontend or
backend UI. The method ``validateBlock`` contains the validation configuration.

cache configuration
^^^^^^^^^^^^^^^^^^^

The method ``getCacheKeys`` contains cache keys to be used for caching the block.

js and css
^^^^^^^^^^

The methods ``getJavascripts`` and ``getStylesheets`` can be used to define js and css assets. Use the twig helpers
``sonata_block_include_javascripts`` and ``sonata_block_include_stylesheets`` to render them.

.. code-block:: jinja

    {{ sonata_block_include_javascripts() }}
    {{ sonata_block_include_stylesheets() }}

.. note::

    This will output the js and css of all blocks loaded in the service container of your application.

load method
^^^^^^^^^^^

The method ``load`` can be used to load additional data. It is called each time a block is rendered before the
``execute`` method is called.

.. _block-rendering:

Block rendering
---------------

To have the block from the example of the Block Document section actually rendered, you just add the following code to
your Twig template:

.. code-block:: jinja

    {{ sonata_block_render({'name': 'sidebarBlock'}) }}

This will make the BlockBundle rendering the according block on every page that has a block named 'sidebarBlock'.
Of course, the actual page needs to be rendered by the template that contains the snippet above.

This happens when a block is rendered, see the separate sections for more details:

* a document is loaded based on the name
* if caching is configured, the cache is checked and content is returned if found
* each block document also has a block service, the execute method of it is called:

  * you can put here logic like in a controller
  * it calls a template
  * the result is a Response object

Block types
-----------

The BlockBundle comes with four general purpose blocks:

* SimpleBlock: A simple block with nothing but a title and a field of hypertext. This would usually be what an editor
  edits directly, for example contact information
* ContainerBlock: A block that contains 0 to n child blocks
* ReferenceBlock: A block that references a block stored somewhere else in the content tree. For example you might want
  to refer parts of the contact information from the homepage
* ActionBlock: A block that calls a Symfony2 action. "Why would I use this instead of directly calling the action from
  my template?", you might wonder. Well imagine the following case: You provide a block that renders teasers of your
  latest news. However, there is no rule where they should appear. Instead, your customer wants to decide himself on
  what pages this block is to be displayed. Providing an according ActionBlock, you allow your customer to do so without
  calling you to change some templates (over and over again!).

Also the BlockBundle has more specific blocks:

* RssBlock: This block extends the ActionBlock: the block document saves the feed url and the controller action fetches
  the feed items. The default implementation uses the `EkoFeedBundle <https://github.com/eko/FeedBundle>`_ to read the
  feed items.
* SlideshowBlock / SlideshowItemBlock: Blocks for helping the management of Slideshows in the Backend. Note that this
  block doesn't the logic to make the slideshow work in the frontend - Feel free to use your favourite JS library to do
  this.

Examples
--------

You can find example usages of this bundle in the `Symfony CMF Sandbox <https://github.com/symfony-cmf/cmf-sandbox>`_.
Have a look at the BlockBundle in the Sandbox. It also shows you how to make blocks editable using the
`CreateBundle <https://github.com/symfony-cmf/CreateBundle>`_.

Reference
---------

.. toctree::
   :maxdepth: 1
   :numbered:

   block/types
   block/create_your_own_blocks
   block/cache
   block/relation_to_sonata_block_bundle
