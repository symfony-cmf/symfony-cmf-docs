Cache
=====

The CmfBlockBundle integrates with the `SonataCacheBundle`_ to
provide several caching solutions. Have a look at the available adapters in the
`SonataCacheBundle`_ to see all options.

The CmfBlockBundle additionally provides its own adapters for:

* `ESI`_ (service: ``cmf.block.cache.varnish``)
* `SSI`_ (service: ``cmf.block.cache.ssi``)
* Asynchronous JavaScript (service: ``cmf.block.cache.js_async``)
* Synchronous JavaScript (service: ``cmf.block.cache.js_sync``)

.. note::

  It is advised to store all settings in the block document when using cache.
  See also :ref:`bundle-block-cache-rendering`.

Dependencies
------------

The cache functionality is optional and depends on the `SonataCacheBundle`_.

Installation
------------

The installation is split between the SonataCacheBundle, the
CmfBlockBundle and the SonataBlockBundle:

1. *SonataCacheBundle* - Follow the installation instructions from the
   `SonataCacheBundle documentation`_.
2. *CmfBlockBundle* - At the end of your routing file, add the
   following lines:

   .. configuration-block::

       .. code-block:: yaml

           # app/config/routing.yml

           # ...
           # routes CmfBlockBundle cache adapters
           block_cache:
               resource: "@CmfBlockBundle/Resources/config/routing/cache.xml"
               prefix: /

       .. code-block:: xml

           <!-- app/config/routing.xml -->
           <?xml version="1.0" encoding="UTF-8" ?>
           <routes xmlns="http://symfony.com/schema/routing"
               xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
               xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

               <!-- ... -->

               <import resource="@SymfonyCmfBlockBundle/Resources/config/routing/cache.xml"
                   prefix="/"
               />
           </routes>

       .. code-block:: php

           // app/config/routing.php
           $collection->addCollection(
               $loader->import("@SymfonyCmfBlockBundle/Resources/config/routing/cache.xml"),
               '/'
           );

           return $collection;

3. *SonataBlockBundle* - Use the ``sonata_block`` key to configure the cache
   adapter for each block service:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            sonata_block:
                # ...
                blocks:
                    symfony_cmf.block.action:
                        # use the service id of the cache adapter
                        cache: cmf.block.cache.js_async
                blocks_by_class:
                     # cache only the RssBlock and not all action blocks
                     Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock:
                         cache: cmf.block.cache.js_async

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" charset="UTF-8" ?>
            <container xmlns="http://symfony.com/schema/dic/services">

                <config xmlns="http://sonata-project.org/schema/dic/block">
                    <!-- use the service id of the cache adapter -->
                    <block
                        id="symfony_cmf.block.action"
                        cache="symfony_cmf.block.cache.js_async"
                    />
                    <block-by-class
                        class="Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock"
                        cache="symfony_cmf.block.cache.js_async"
                    />
                </config>
            </container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('sonata_block', array(
                'blocks' => array(
                    'symfony_cmf.block.action' => array(
                        // use the service id of the cache adapter
                        'cache' => 'symfony_cmf.block.cache.js_async',
                    ),
                ),
                'blocks_by_class' => array(
                    Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\RssBlock::class => array(
                            'cache' => 'symfony_cmf.block.cache.js_async',
                        ),
                    ),
                ),
            ));

Workflow
--------

When a block having a cache configured is rendered, the following process
is triggered:

* The document is loaded based on the name;
* The cache is checked and content is returned if found.

  * Cache keys are computed using:

    * The cache keys of the block service;
    * The ``extraCacheKeys`` passed from the template.

  * The cache adapter is asked for a cache element:

    * The ESI and SSI adapter add a specific tag and a url to retrieve the
      block content;
    * The JavaScript adapter adds JavaScript and a url to retrieve the block
      content.

  * If the cache element is not expired and has data it is returned.
* The template is rendered:

  * For ESI and SSI the url is called to retrieve the block content
  * For JavaScript the browser calls a url and replaces a placeholder with the
  * returned block content

.. note::

    The additional cache adapters of the BlockBundle always return that the
    cache is found, have a look at the ``has`` method of the adapters in the
    SonataCacheBundle to see how they respond.

If the cache is checked and the cache adapter returned that no cache was found,
the workflow proceeds like this:

* The block service is asked to render the block
  :ref:`as usual <bundle-block-execute>`;
* If the ``Response`` is cacheable, the configured adapter creates a cache
  element containing:

  * The computed cache keys;
  * The time to live (TTL) of the response;
  * The ``Response``;
  * Any additional contextual keys.

* The template is rendered.

Cache Keys
----------

It is the responsibility of the :ref:`block service <bundle-block-service>` to generate the cache keys in
the method ``getCacheKeys``.

The block services shipped with the Symfony CMF BlockBunde use the
``getCacheKeys`` method of the ``Sonata\BlockBundle\Block\BaseBlockService``
which returns:

* ``block_id``
* ``updated_at``

.. caution::

    If block settings need to be persisted between requests, it is advised to
    store them in the block document. If you add them to the cache keys, you
    have to be aware that depending on the adapter, the cache keys can be sent
    to the browser and thus are neither hidden nor safe from manipulation by a
    client.

Extra Cache Keys
~~~~~~~~~~~~~~~~

The extra cache keys array is used to store metadata along the cache element.
The metadata can be used to invalidate a set of cached elements.

Contextual Keys
~~~~~~~~~~~~~~~

The contextual cache array hold the object class and id used inside the
template. This contextual cache array is then added to the extra cache key.

This feature can be use like this::

    $cacheManager->remove(array('objectId' => 'id'));

While not all cache adapters support this feature, the Varnish and MongoDB
adapters do.

The BlockBundle also has a cache invalidation listener that calls the
``flush`` method of a cache adapter automatically when a cached block document
is updated or removed.

.. _bundle-block-cache-rendering:

Block Rendering
---------------

The following parameters can be used in the ``sonata_block_render`` code in
your Twig template when using cache:

* **use_cache**: use the configured cache for a block (*default*: true)
* **extra_cache_keys**: expects an array with extra cache keys (*default*: empty array)

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({ 'name': 'rssBlock' }, {
            use_cache: true,
            extra_cache_keys: { 'extra_key': 'my_block' }
        }) }}

    .. code-block:: html+php

        <?php echo $view['blocks']->render(array(
            'name' => 'rssBlock',
        ), array(
            'use_cache' => true,
            'extra_cache_keys' => array(
                'extra_key' => 'my_block'
            ),
        )) ?>

When using the Esi, Ssi or Js cache adapters, the settings passed here are remembered:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({ 'name': 'rssBlock' }, {
            'title': 'Symfony2 CMF news',
            'url': 'http://cmf.symfony.com/news.rss',
            'maxItems': 2
        }) }}

    .. code-block:: html+php

        <?php echo $view['blocks']->render(array(
            'name' => 'rssBlock',
        ), array(
            'title'    => 'Symfony2 CMF news',
            'url'      => 'http://cmf.symfony.com/news.rss',
            'maxItems' => 2,
        )) ?>

The default ``BlockContextManager`` of the SonataBlockBundle automatically adds
settings passed from the template to the ``extra_cache_keys`` with the key
``context``. This allows the cache adapters to rebuild the BlockContext. See
also the `SonataBlockBundle Advanced usage`_ documentation.

.. note::

    Secure the cache adapter url if needed as the settings from
    ``sonata_block_render`` are added to the url as parameters.

.. caution::

    Because, as mentioned above, settings can be added to the URL as
    parameters, you have to avoid exposing sensitive settings from
    ``sonata_block_render`` and store them in the block document instead.

Adapters
--------

ESI
~~~

This extends the default VarnishCache adapter of the SonataCacheBundle.

See :ref:`the configuration reference <reference-config-block-caches-esi>` to
learn how to configure the esi adapter.

SSI
~~~

This extends the default SsiCache adapter of the SonataCacheBundle.

See :ref:`the configuration reference <reference-config-block-caches-ssi>` to
learn how to configure the ssi adapter.

JavaScript
~~~~~~~~~~

Renders the block using JavaScript, the page is loaded and not waiting for the
block to be finished rendering or retrieving data. The block is then
asynchronously or synchronously loaded and added to the page.

.. _`SonataCacheBundle`: https://github.com/sonata-project/SonataCacheBundle
.. _`ESI`: https://en.wikipedia.org/wiki/Edge_Side_Includes
.. _`SSI`: https://en.wikipedia.org/wiki/Server_Side_Includes
.. _`SonataCacheBundle documentation`: https://sonata-project.org/bundles/cache/master/doc/index.html
.. _`SonataBlockBundle Advanced usage`: https://sonata-project.org/bundles/block/master/doc/reference/advanced_usage.html#block-context-manager-context-cache
