.. index::
    single: Block; Bundles
    single: BlockBundle

BlockBundle
===========

    This bundle provides integration with `SonataBlockBundle`_. It is used to
    manage fragments of content, so-called blocks, that are persisted in a
    database and can be incorporated into any page layout.

The CmfBlockBundle also provides a few commonly used standard blocks,
including the ability to edit them. See :doc:`types`.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/block-bundle`_ package.

As the bundle is using the `SonataBlockBundle`_, you need to instantiate some
sonata bundles in addition to the CmfBlockBundle::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Sonata\BlockBundle\SonataBlockBundle(),
                new Sonata\CoreBundle\SonataCoreBundle(),
                new Symfony\Cmf\Bundle\BlockBundle\CmfBlockBundle(),
            );

            // ...
        }

        // ...
    }

.. _bundle-block-configuration:

Usage
-----

The BlockBundle needs a persistence layer to be configured. This can either be
done globally with the :ref:`Core Bundle <bundles-core-persistence>` or
individually as follows:

.. configuration-block::

    .. code-block:: yaml

        cmf_block:
            persistence:
                phpcr:
                    block_basepath: /cms/content

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/block">
                <persistence>
                    <phpcr
                        block-basepath="/cms/block"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_block', array(
            'persistence' => array(
                'phpcr' => array(
                    'block_basepath' => '/cms/block',
                ),
            ),
        ));

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
                app.news_block:
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
                <blocks id="app.news_block">
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
                'app.news_block' => array(
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

.. tip::

    You can also store settings in the single block objects themselves. This
    allows to individually configure options per block instance.

    If you edit the blocks using the Sonata admin, there is also the
    :ref:`Block Sonata Admin Extension <bundles-block-types-admin_extension>`
    that adds editing of the ``BaseBlock`` general block options.

Updated SonataBlockBundle Defaults
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The BlockBundle *automatically* changes some defaults and adds configuration
to the `SonataBlockBundle`_ to make it work nicely. This is done using the
`prepended configuration`_ option of Symfony. The following defaults are
updated:

* **templates.block_base** the CMF base template wraps the block output in
  a ``div`` and slugifies the PHPCR path as id; The base template is
  kept compatible with the Sonata base template for non-cmf blocks;
* **RssBlock configuration** adds the
  :ref:`default RssBlock settings <bundle-block-rss-settings>`.

.. note::

    Settings are only prepended, meaning the default value is changed. You can
    still change the values by setting the configuration values in your
    application configuration file.

.. _bundle-block-document:

Block Document
--------------

Before you can render a block, you need to create a data object representing
your block in the repository. You can do so with the following code snippet::

    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock;

    // ...
    /** @var $dm \Doctrine\ODM\PHPCR\DocumentManager */
    $parentDocument = $dm->find(null, '/cms/content/home');

    $myBlock = new SimpleBlock();
    $myBlock->setParentDocument($parentDocument);
    $myBlock->setName('sidebarBlock');
    $myBlock->setTitle('My first block');
    $myBlock->setBody('Hello block world!');

    $dm->persist($myBlock);

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
the block service, the block document and settings passed to the Twig template
helper. Therefore, use the BlockContext to get or alter a setting if needed.

.. _bundle-block-service:

Block Service
-------------

Internally, the block bundle uses a block service to work with each type
of block. The service is configured to handle a *type* of block. The
blocks themselves identify their type in the ``getType`` method.

When using the provided blocks, you do not need to worry about the block
service. It is only relevant when
:doc:`creating your own blocks <create_your_own_blocks>`.

.. _bundle-block-rendering:

Block rendering
---------------

Rendering is handled by the SonataBlockBundle ``sonata_block_render`` Twig
function. The block name is either an absolute PHPCR path or the name of the
block relative to the ``cmfMainContent`` document.

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

.. _bundle-block-embed:

Embedding Blocks in WYSIWYG Content
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The CmfBlockBundle provides a Twig filter ``cmf_embed_blocks`` that
looks through the content and looks for special tags to render blocks. To use
the tag, you need to apply the ``cmf_embed_blocks`` filter to your output. If
you can, render your blocks directly in the template. This feature is only a
cheap solution for web editors to place blocks anywhere in their HTML content.
A better solution to build composed pages is to build it from blocks (there
might be a CMF bundle at some point for this).

.. configuration-block::

    .. code-block:: jinja

        {{ page.content|cmf_embed_blocks }}

    .. code-block:: html+php

        <?php echo $view['blocks']->embedBlocks(
            $page->getContent()
        ) ?>

.. caution::

    Make sure to only place this filter where you display the content and not
    where editing it, as otherwise your users would start to edit the rendered
    output of their blocks.
    This feature conflicts with the front-end editing provided by CreateBundle,
    as create.js operates on the rendered content as displayed to the user.
    There is an ongoing `discussion how to fix this`_.

When you apply the filter, your users can use this tag to embed a block in
their content:

.. code-block:: text

    %embed-block|/absolute/path/to/block|end%

    %embed-block|local-block|end%

The path to the block is either absolute or relative to the current main
content. The prefix and postfix are configurable. The default prefix is
``%embed-block|`` and the default postfix is ``|end%``. Say you want
to use ``%%%block:"/absolute/path"%%%`` then you do:

.. configuration-block::

     .. code-block:: yaml

        # app/config/config.yml
        cmf_block:
            twig:
                cmf_embed_blocks:
                    prefix: '%%%block:"'
                    postfix: '"%%%'

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/block">
                <twig>
                    <cmf-embed-blocks
                        prefix="%%%block:&quot;"
                        postfix="&quot;%%%"
                    />
                </twig>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_block', array(
            'twig' => array(
                'cmf_embed_blocks' => array(
                    'prefix' => '%%%block:"',
                    'postfix' => '"%%%',
                ),
            ),
        );

See also the :ref:`the configuration reference <reference-config-block-twig-cmf-embed-blocks>`.

.. caution::

    Currently there is no security built into this feature. Only enable the
    filter for content for which you are sure only trusted users may edit it.
    Restrictions about what block can be where that are built into an admin
    interface are not respected here.

.. note::

    The block embed filter ignores all errors that might occur when rendering a
    block and returns an empty string for each failed block instead. The errors
    are logged at level WARNING.

SonataAdminBundle Integration
-----------------------------

The BlockBundle also provides Admin classes to enable creating, editing and
removing blocks from the admin panel. To enable the admin, use the
``cmf_block.persistence.phpcr.use_sonata_admin`` setting. Both the
:ref:`BlockBundle <bundles-block-types-admin_extension>` and
:ref:`CoreBundle <bundles-core-persistence>` provide several extensions for
SonataAdminBundle.

Examples
--------

You can find example usages of this bundle in the `Symfony CMF Sandbox`_
(have a look at the BlockBundle). It also shows you how to make blocks
editable using the :doc:`CreateBundle <../create/introduction>`.

Read on
-------

* :doc:`types`
* :doc:`create_your_own_blocks`
* :doc:`cache`
* :doc:`relation_to_sonata_block_bundle`
* :doc:`configuration`

.. _`symfony-cmf/block-bundle`: https://packagist.org/packages/symfony-cmf/block-bundle
.. _`with composer`: https://getcomposer.org
.. _`Symfony CMF Sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`prepended configuration`: https://symfony.com/doc/current/components/dependency_injection/compilation.html#prepending-configuration-passed-to-the-extension
.. _`SonataBlockBundle`: https://github.com/sonata-project/SonataBlockBundle
.. _`discussion how to fix this`: https://github.com/symfony-cmf/block-bundle/issues/143
