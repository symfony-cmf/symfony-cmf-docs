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


Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_block``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_block:
            document_manager_name:  default

Block rendering
---------------

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
the block, this makes the block unique. The other properties are specific to ``Simpleblock``.
Now to have this block actually rendered, you just add the following code to your Twig template:

.. code-block:: jinja

    {{ sonata_block_render({'name': 'sidebarBlock'}) }}

This will make the BlockBundle rendering the according block on every page that has a block named 'sidebarBlock'. Of course, the actual page needs to be rendered by the template that contains the snippet above.

Block types
-----------

The BlockBundle comes with four general purpose blocks:
 * SimpleBlock: A simple block with nothing but a title and a field of hypertext. This would usually be what an editor edits directly, for example contact information
 * ContainerBlock: A block that contains 0 to n child blocks
 * ReferenceBlock: A block that references a block stored somewhere else in the content tree. For example you might want to refer parts of the contact information from the homepage
 * ActionBlock: A block that calls a Symfony2 action. "Why would I use this instead of directly calling the action from my template?", you might wonder. Well imagine the following case: You provide a block that renders teasers of your latest news. However, there is no rule where they should appear. Instead, your customer wants to decide himself on what pages this block is to be displayed. Providing an according ActionBlock, you allow your customer to do so without calling you to change some templates (over and over again!).

Create your own blocks
----------------------

Lets say you are working on a project where you have to integrate data received from several RSS feeds.
Of course you could create an ActionBlock for each of these feeds, but wouldn't this be silly? In
fact all those actions would look similar: Receive data from a feed, sanitize it and pass the data to a
template. So instead you decide to create your own block, the RSSBlock.

The first thing you need is an entity that contains the data. It is recommended to extend the BaseBlock
contained in this bundle (however you are not forced to do so, as long as you implement
``Sonata\BlockBundle\Model\BlockInterface``). In your entity, you add two properties, 'feedURL' and
'templateName', as well as getters and setters for them. Also, you need to define the ``getType``
method which just returns 'my_bundle.block.rss'.

The second thing required is a service that knows how to handle RSSBlocks. In the case of the RSSBlock
this would be: Fetch the data from whatever is stored in 'feedURL', sanitize it, and pass it to the
template stored in 'templateName'. You can again extend ``Sonata\BlockBundle\Block\BaseBlockService``.
It is important, that the 'name' property of the service is called 'my_bundle.block.rss' (this makes
sure the relation between entity and service can be found).

The last thing you need is to define the service in a config file. It is important to tag your
BlockService with 'sonata.block', otherwise it will not be known by the Bundle.

Examples
--------

You can find example usages of this bundle in the `Symfony CMF Sandbox <https://github.com/symfony-cmf/cmf-sandbox>`_.
Have a look at the BlockBundle in the Sandbox. It also shows you how to make blocks editable using the
`CreateBundle <https://github.com/symfony-cmf/CreateBundle>`_.

Relation to Sonata Block Bundle
-------------------------------

The BlockBundle is based on the `SonataBlockBundle <https://github.com/sonata-project/SonataBlockBundle>`_.
It replaces components of the bundle where needed to be compatible with PHPCR.

The following picture shows where we use our own components (blue):

.. image:: ../images/bundles/classdiagram.jpg
   :align: center
