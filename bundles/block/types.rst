Block Types
===========

For each purpose a different block type can be used. The general purpose blocks can be used for several solutions.
The Block Bundle ships with more specific block types.

RSSBlock
--------

The RssBlock extends the ActionBlock and allows you to read feed items and display them in a list.

Create a document:

.. code-block:: php

    $myRssBlock = new RssBlock();
    $myRssBlock->setParentDocument($parentPage);
    $myRssBlock->setName('rssBlock');
    $myRssBlock->setSetting('title', 'Symfony2 CMF news');
    $myRssBlock->setSetting('url', 'http://cmf.symfony.com/news.rss');
    $myRssBlock->setSetting('maxItems', 3);

    $documentManager->persist($myRssBlock);

All available settings are:

* **url**: the url of the rss feed (*required*)
* **title**: the title for the list (*default*: Insert the rss title)
* **maxItems**: the maximum amount of items to return to the template (*default*: 10)
* **template**: the template to render the feed items (*default*: SymfonyCmfBlockBundle:Block:block_rss.html.twig)
* **ItemClass**: the class used for the item objects that are passed to the template (*default*: Symfony\Cmf\Bundle\BlockBundle\Model\FeedItem)

The controller to get the feed items can also be changed:

* define a different class for the controller service in your configuration using the DI service parameter ``symfony_cmf_block.rss_controller_class``
* or set the actionName of your RssBlock document

.. note::

    The `Symfony CMF Sandbox <https://github.com/symfony-cmf/cmf-sandbox>`_ contains an example of the RssBlock.