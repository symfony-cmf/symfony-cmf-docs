Block Types
===========

The BlockBundle provides a couple of default block types for general use
cases. It also has a couple of more specific blocks that integrate third
party libraries. Those can be handy for some use cases and also serve as
examples to build your own blocks.

You can also :doc:`create your own blocks <create_your_own_blocks>`, but
the provided block types should cover many standard cases.

Common Behaviour
----------------

There is an ``AbstractBlock`` base class that defines common behaviour for all
blocks provided by this bundle. It also implements handling for locales, even
though only those block that actually contain string content implement the
``TranslatableInterface``.

Publish Workflow Interfaces
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``AbstractBlock`` implements the write interfaces for publishable and
publish time period, see the
:doc:`publish workflow documentation <../core/publish_workflow>` for more
information.

Sonata Admin
~~~~~~~~~~~~

All block types provided by the Symfony2 CMF BlockBundle come with a admin
classes for SonataDoctrinePHPCRAdminBundle_. To activate the admin services,
it is enough to load the SonataDoctrinePHPCRAdminBundle in your application
kernel and configure the sonata dashboard as desired.

.. _bundles-block-types-admin_extension:

Sonata Admin Extension for Basic Block Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This bundle also provides a sonata admin *extension* that you can activate.
It adds a tab to edit settings of basic block options like the time to life
(TTL, for caching purposes).

Assuming your blocks extend the ``BaseBlock`` class (as all blocks provided by
the CMF BlockBundle do), you can add the following lines to your sonata admin
configuration.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            extensions:
                cmf_block.admin_extension.cache:
                    extends:
                        - Symfony\Cmf\Bundle\BlockBundle\Model\AbstractBlock

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <extension id="cmf_block.admin_extension.cache">
                    <extend>Symfony\Cmf\Bundle\BlockBundle\Model\AbstractBlock</extend>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'cmf.block.admin_extension.cache' => array(
                    'extends' => array(
                        'Symfony\Cmf\Bundle\BlockBundle\Model\AbstractBlock',
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


StringBlock
-----------

This is a very simple block that just provides one string field called
``body`` and the default template renders the content as ``raw`` to
allow HTML in the field. The template outputs no HTML tags around the string
at all. This can be useful for editable page fragments.

SimpleBlock
-----------

Just a text block with a ``title`` and a ``body``. The default template
renders both title and body with the twig ``raw`` filter, meaning HTML is
allowed in those fields.

This block is useful to edit static text fragments and for example display
it in several places using the ``ReferenceBlock``.

ContainerBlock
--------------

A container can hold a list of arbitrary child blocks (even other
``ContainerBlocks``) and just renders one child after the other. The list can
also be empty, in which case only the wrapping element of the container block
will be rendered.

This block has the methods ``setChildren`` to overwrite the current
children with a new list and ``addChild`` and ``removeChild`` to individually
add resp. remove child blocks.

ReferenceBlock
--------------

This block has no content of its own, but points to a target block.
When rendered, this block renders the target block as if the target
block was directly used in that place.

This block simply has the method ``setReferencedBlock`` that accepts any
block mapped by the persistence layer as argument. If you set this to
something that is not a valid block, the problem is only detected when
rendering the block.

ActionBlock
-----------

The action block allows to configure a controller action that will be called
in a `sub-request`_ when rendering the block. Instead of directly calling the
action from a template, your CMS users can define and parametrize what action
to call, and decide where they want to put this block.

This block is also a good base to implement specific actions, if you want
something more user friendly. See the ``RssBlock`` below for an example.

As the ``ActionBlock`` does a sub-request, you may also need to control the
parameters that are passed to the sub-request. The block service calls
``resolveRequestParams($request, $blockContext)`` to let the block decide
what needs to be passed to the sub-request. The ``ActionBlock`` lets you
configure the fields with ``setRequestParams`` and persists them in the
database. It does not matter whether the field is found in the request
attributes or the request parameters, it is found in both by using
``$request->get()``. The only request attribute propagated by default is
the ``_locale``.

RssBlock
--------

The ``RssBlock`` extends the ``ActionBlock`` and allows you to read feed items and
display them in a list. It depends on the ``eko/feedbundle`` which you need to add
to your ``composer.json`` and instantiate in the ``AppKernel``.

Create a document::

    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock;

    // ...

    $myRssBlock = new RssBlock();
    $myRssBlock->setParentDocument($parentPage);
    $myRssBlock->setName('rssBlock');
    $myRssBlock->setSetting('title', 'Symfony2 CMF news');
    $myRssBlock->setSetting('url', 'http://cmf.symfony.com/news.rss');
    $myRssBlock->setSetting('maxItems', 3);

    $documentManager->persist($myRssBlock);

.. _bundle-block-rss-settings:

The available settings are:

* **url**: the url of the rss feed (*required*)
* **title**: the title for the list (*default*: Insert the rss title)
* **maxItems**: the maximum amount of items to return to the template
  (*default*: 10)
* **template**: the template to render the feed items (*default*:
  ``CmfBlockBundle:Block:block_rss.html.twig``)
* **ItemClass**: the class used for the item objects that are passed to the
  template (*default*: ``Symfony\Cmf\Bundle\BlockBundle\Model\FeedItem``)

The controller used to fetch the feed items can also be changed:

* Define a different class for the controller service in your configuration
  using the DI service parameter ``cmf_block.rss_controller_class``
* or set the actionName of your RssBlock document

.. note::

        The `Symfony CMF Sandbox`_ contains an example of the ``RssBlock``.

ImagineBlock
------------

The imagine block uses the `LiipImagineBundle`_ to display images directly
out of PHPCR. The block has a child of type
``Symfony\Cmf\Bundle\MediaBundle\ImageInterface`` for the image, and fields for
the name of the imagine filter to use, an URL and an image caption. To use this
block, you need to add ``liip/imagine-bundle`` to your ``composer.json`` and
define the imagine filter you specify in the block. The default name is
``cmf_block``. The filter must use the ``cmf_media_doctrine_phpcr`` driver if
you use the PHPCR-ODM ``ImagineBlock``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        liip_imagine:
            # ...
            filter_sets:
                cmf_block:
                    data_loader: cmf_media_doctrine_phpcr
                    quality: 85
                    filters:
                        thumbnail: { size: [616, 419], mode: outbound }
                # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/dic/schema/liip_imagine">
                <!-- ... -->
                <filter-set name="cmf_block" data-loader="cmf_media_doctrine_phpcr" quality="85">
                    <filter name="thumbnail" size="616,419" mode="outbound"/>
                </filter-set>
                <!-- ... -->
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('liip_imagine', array(
            // ...
            'filter_sets' => array(
                'cmf_block' => array(
                    'data_loader' => 'cmf_media_doctrine_phpcr',
                    'quality'     => 85,
                    'filters'     => array(
                        'thumbnail' => array(
                            'size' => array(616, 419),
                            'mode' => 'outbound',
                        ),
                    ),
                ),
                // ...
            ),
        ));

The ``ImagineBlock`` uses the template ``BlockBundle:Block:block_imagine.html.twig``
to render the layout. You may override this or configure a different template if
you need a specific markup.

See also the example below for how to create an ``ImagineBlock`` programmatically.
Please refer to the `LiipImagineBundle documentation`_ for further information.

SlideshowBlock
--------------

The ``SlideshowBlock`` is a special kind of ``ContainerBlock``. It can contain
any kind of blocks that will be rendered with a wrapper div to help a
JavaScript slideshow library to slide them.

The ``ImagineBlock`` is particularly suited if you want to do an image
slideshow, but the ``SlideshowBlock`` can handle any kind of blocks, also mixed
types of blocks in the same slideshow.

.. note::

    This bundle does not attempt to provide a JavaScript library for animating
    the slideshow. Chose your preferred library that plays well with the rest
    of your site and hook it on the slideshows. (See also below).

Create your first Slideshow
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Building a slideshow consists of creating the container ``SlideshowBlock`` and
adding blocks to it. Those blocks can be any kind of blocks, but the
``ImagineBlock`` makes a lot of sense. Make sure to have configured the imagine
filter as explained `above <ImagineBlock>`_::

    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SlideshowBlock;
    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ImagineBlock;
    use Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Image;

    // create slideshow
    $mySlideshow = new SlideshowBlock();
    $mySlideshow->setName('slideshow');
    $mySlideshow->setParentDocument($parentPage);
    $mySlideshow->setTitle('My first Slideshow');
    $documentManager->persist($mySlideshow);

    // add first slide to slideshow
    $mySlideshowItem = new ImagineBlock();
    $mySlideshowItem->setName('first_item');
    $mySlideshowItem->setLabel('label of first item');
    $mySlideshowItem->setParentDocument($mySlideshow);
    $manager->persist($mySlideshowItem);

    $image = new Image();
    $image->setFileContentFromFilesystem('path/to/my/image.jpg');
    $mySlideshowItem->setImage($image);

Render the slideshow
~~~~~~~~~~~~~~~~~~~~

Rendering your slideshow simply means rendering the ``SlideshowBlock`` in your
template. If your ``contentDocument`` has a field ``slideshow`` that contains
a ``SlideshowBlock`` object, you can render it with:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({
            'name': 'slideshow'
        }) }}

    .. code-block:: html+php

        <?php echo $view['blocks']->render(array(
            'name' => 'slideshow',
        )) ?>

Make the slideshow work in the frontend
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since the BlockBundle doesn't contain anything to make the slideshow work
in the frontend, you need to do this yourself. Use your favourite JavaScript
library to make the slideshow interactive. If special markup is needed for
your slideshow code to work, you can override
``BlockBundle:Block:block_slideshow.html.twig`` and/or the templates of the
blocks you use as slideshow items and adapt them to your needs.

Use the Sonata admin class
~~~~~~~~~~~~~~~~~~~~~~~~~~

The BlockBundle comes with a sonata admin class for managing slideshow blocks.
All you need to do to administrate slideshows in your project is to add the
following line to your sonata admin configuration:

.. configuration-block::

    .. code-block:: yaml

        sonata_admin:
            dashboard:
                groups:
                    blocks:
                        label: Blocks
                        items:
                            - cmf_block.imagine.slideshow_admin

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/sonata_admin">
                <dashboard>
                    <group id="blocks"
                        label="Blocks">
                        <item>cmf_block.imagine.slideshow_admin</item>
                    </group>
                </dashboard>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('sonata_admin', array(
            'dashboard' => array(
                'groups' => array(
                    'blocks' => array(
                        'label' => 'Blocks',
                        'items' => array(
                            'cmf_block.imagine.slideshow_admin',
                        ),
                    ),
                ),
            ),
        ));

However, you can also embed the slideshow administration directly into
other admin classes using the ``sonata_type_admin`` form type. The admin
service to use in that case is ``cmf_block.slideshow_admin``.
Please refer to the `Sonata Admin documentation`_
for further information.

.. _`Symfony CMF Sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`Sonata Admin documentation`: http://sonata-project.org/bundles/admin/master/doc/reference/form_types.html
.. _`Sonata Admin Extensions`: http://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _`LiipImagineBundle`: https://github.com/liip/LiipImagineBundle
.. _`LiipImagineBundle documentation`: https://github.com/liip/LiipImagineBundle/tree/master/Resources/doc
.. _`sub-request`: http://symfony.com/doc/current/book/internals.html#internal-requests
.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
