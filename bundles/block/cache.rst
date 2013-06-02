Cache
=====

The BlockBundle integrates with the `SonataCacheBundle`_ to provide several
caching solutions. Have a look at the available adapters in the
SonataCacheBundle to see all options.

The BlockBundle additionally provides its own adapters for:

* `ESI`_
* `SSI`_
* Asynchronous javascript
* Synchronous javascript

.. note::

  It is advised to store all settings in the block document when using cache.

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

3. *SonataBlockBundle* - Use the ``sonata_block`` key to configure the cache
   adapter for each block service:

   .. configuration-block::

       .. code-block:: yaml

           # app/config/config.yml
           sonata_block:
           # ...
               blocks:
                   cmf.block.action:
                       # use the service id of the cache adapter
                       cache: cmf.block.cache.js_async

Workflow
--------

The following happens when a block is rendered using cache:

* A document is loaded based on the name
* If caching is configured, the cache is checked and content is returned if
  found

  * Cache keys are computed using:

    * The cache keys of the block service
    * The extraCacheKeys passed from the template

  * The cache adapter is asked for a cache element

    * The ESI and SSI adapter add a specific tag and a url to retrieve the
      block content
    * The Javascript adapter adds javascript and a url to retrieve the block
      content

  * If the cache element is not expired and has data it is returned
* The template is rendered:

  * For ESI and SSI the url is called to retrieve the block content
  * For Javascript the browser calls a url and replaces a placeholder with the
  * returned block content

.. note::

    The additional cache adapters of the BlockBundle always return that the
    cache is found, have a look at the ``has`` method of the adapters in the
    SonataCacheBundle to see how they respond.

If cache is checked and the cache adapter returned that no cache was found,
the workflow proceeds like this:

* Each block document also has a block service, the execute method of it is
  called to render the block and return a response
* If the response is cacheable the configured adapter creates a cache element,
  it contains

  * The computed cache keys
  * The ttl of the response
  * The response
  * And additional contextual keys

* The template is rendered

Cache Keys
----------

The block service has the responsibility to generate the cache keys, the
method ``getCacheKeys`` returns these keys, see :ref:`bundle-block-service`.

The block services shipped with the BlockBunde use the ``getCacheKeys`` method
of the ``Sonata\BlockBundle\Block\BaseBlockService``, and return:

* ``block_id``
* ``updated_at``

.. note::

    If block settings need to be persisted between requests it is advised to
    store them in the block document. Alternatively they can be added to the
    cache keys. However be very cautious because, depending on the adapter,
    the cache keys can be send to the browser and are not secure.

Extra Cache Keys
~~~~~~~~~~~~~~~~

The extra cache keys array is used to store metadata along the cache element.
The metadata can be used to invalidate a set of cache elements.

Contextual Keys
~~~~~~~~~~~~~~~

The contextual cache array hold the object class and id used inside the
template. This contextual cache array is then added to the extra cache key.

This feature can be use like this ``$cacheManager->remove(array('objectId' => 'id'))``.

Of course not all cache adapters support this feature, varnish and MongoDB do.

The BlockBundle also has a cache invalidation listener that calls the
``flush`` method of a cache adapter automatically when a cached block document
is updated or removed.

Block Rendering
---------------

The following parameters can be used in the ``sonata_block_render`` code in
your Twig template when using cache:

* **useCache**: use the configured cache for a block (*default*: true)
* **extraCachekeys**: expects an array with extra cache keys (*default*: empty array)

.. code-block:: jinja

    {{ sonata_block_render(
        { 'name': 'rssBlock' },
        true,
        { 'extra_key': 'my_block' }
    ) }}

Adapters
--------

ESI
~~~

This extends the default EsiCache adapter of the SonataCacheBundle.

Configuration
.............

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_block:
            # ...
            caches:
                esi:
                    token: a unique security key # a random one is generated by default
                    servers:
                        - varnishadm -T 127.0.0.1:2000 {{ COMMAND }} "{{ EXPRESSION }}"

SSI
~~~

This extends the default SsiCache adapter of the SonataCacheBundle.

Configuration
.............

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_block:
            # ...
            caches:
                ssi:
                   token: a unique security key # a random one is generated by default

Javascript
~~~~~~~~~~

Renders the block using javascript, the page is loaded and not waiting for the
block to be finished rendering or retrieving data. The block is then
asynchronously or synchronously loaded and added to the page.

.. _`SonataCacheBundle`: https://github.com/sonata-project/SonataCacheBundle
.. _`ESI`: http://wikipedia.org/wiki/Edge_Side_Includes
.. _`SSI`: http://wikipedia.org/wiki/Server_Side_Includes
.. _`SonataCacheBundle documentation`: http://sonata-project.org/bundles/cache/master/doc/index.html
