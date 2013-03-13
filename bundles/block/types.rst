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

SlideshowBlock
--------

The ``SlideshowBlock`` is just a special kind of ``ContainerBlock``, a container for the slides of a slideshow. The
BlockBundle also comes with a ``SlideshowItemBlock`` which represents a very simple kind of slide, a ``SlideshowItem``,
which contains an image and a label. However you are free to use whatever block you want as an item for a slideshow.
You can also mix different sorts of slides in the same slideshow.

Create your first slideshow
`````````````

.. code-block:: php

    // create slideshow
    $mySlideshow = new SlideshowBlock();
    $mySlideshow->setName('slideshow');
    $mySlideshow->setParentDocument($parentPage);
    $mySlideshow->setTitle('My first Slideshow');
    $documentManager->persist($mySlideshow);

    // add first slide to slideshow
    $mySlideshowItem = new SlideshowItemBlock();
    $mySlideshowItem->setName('first_item');
    $mySlideshowItem->setLabel('label of first item');
    $mySlideshowItem->setParentDocument($mySlideshow);
    $manager->persist($mySlideshowItem);

    $file = new File();
    $file->setFileContentFromFilesystem('path/to/my/image.jpg');
    $image = new Image();
    $image->setFile($file);
    $mySlideshowItem->setImage($image);

Use the admin class
`````````````

The BlockBundle comes with admin classes for managing slideshows and slideshow items directly in SonataAdmin. All you
need to do to administrate slideshows in your project is to add the following line to your sonata admin configuration:

.. code-block:: php

    sonata_admin:
        dashboard:
            groups:
                blocks:
                    label: Blocks
                    items:
                        - symfony_cmf_block.slideshow_admin

However, you can also integrate the slideshow administration directly in another AdminClass using
``symfony_cmf_block.minimal_slideshow_admin``. Please refer to `the Sonata Admin docs
<http://sonata-project.org/bundles/admin/master/doc/reference/form_types.html>` for further information.

Make the slideshow work in the frontend
`````````````

Since the BlockBundle doesn't contain anything to make the slideshow work in the frontend, you need to do this
yourself. Just use your favourite JS library to make the slideshow interactive. If special markup is needed for your
slideshow code to work, just override ``block_slideshow.html.twig`` and ``block_slideshow_item.html.twig`` and adapt
them to your needs.